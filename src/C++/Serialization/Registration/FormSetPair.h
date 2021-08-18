#pragma once

template <class T>
class FormSetPairBase
{
public:
	FormSetPairBase() :
		_pair(),
		_lock()
	{
	}
	FormSetPairBase(const FormSetPairBase& a_rhs) :
		_pair(),
		_lock()
	{
		a_rhs._lock.lock();
		_pair = a_rhs._pair;
		a_rhs._lock.unlock();
	}
	FormSetPairBase(FormSetPairBase&& a_rhs) :
		_pair(),
		_lock()
	{
		Locker locker(a_rhs._lock);
		_pair = std::move(a_rhs._pair);
		a_rhs._pair.first.clear();
		a_rhs._pair.second.clear();
	}
	~FormSetPairBase() = default;

	FormSetPairBase& operator=(const FormSetPairBase& a_rhs)
	{
		if (this == &a_rhs) {
			return *this;
		}

		Locker lhsLocker(_lock);
		_pair.first.clear();
		_pair.second.clear();

		{
			Locker rhsLocker(a_rhs._lock);
			_pair = a_rhs._pair;
		}

		return *this;
	}
	FormSetPairBase& operator=(FormSetPairBase&& a_rhs)
	{
		if (this == &a_rhs) {
			return *this;
		}

		Locker lhsLocker(_lock);
		Locker rhsLocker(a_rhs._lock);

		_pair.first.clear();
		_pair.second.clear();

		_pair = std::move(a_rhs._pair);
		a_rhs._pair.first.clear();
		a_rhs._pair.second.clear();

		return *this;
	}

	inline std::set<T>& GetFormSet(std::uint32_t a_index)
	{
		return a_index == 1 ? _pair.first :
                              _pair.second;
	}
	inline void Add(T a_data, std::uint32_t a_index)
	{
		Locker locker(_lock);

		auto& otherDataSet = GetFormSet(!a_index);
		const auto it = std::ranges::find(otherDataSet, a_data);
		if (it != otherDataSet.end()) {
			otherDataSet.erase(it);
		}

		GetFormSet(a_index).insert(a_data);
	}
	inline void Remove(T a_data, std::uint32_t a_set)
	{
		Locker locker(_lock);

		auto& formSet = GetFormSet(a_set);
		const auto it = formSet.find(a_data);
		if (it != formSet.end()) {
			formSet.erase(it);
		}
	}
	inline bool Contains(T a_data, std::uint32_t a_index)
	{
		Locker locker(_lock);

		auto& formSet = GetFormSet(a_index);
		return formSet.contains(a_data);
	}

	inline void Clear()
	{
		Locker locker(_lock);

		_pair.first.clear();
		_pair.second.clear();
	}
	inline void Clear(std::uint32_t a_index)
	{
		Locker locker(_lock);

		auto& formSet = GetFormSet(a_index);
		formSet.clear();
	}

	virtual bool Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_type, std::uint32_t a_version, std::uint32_t a_index) = 0;
	virtual bool Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index) = 0;
	virtual bool Load(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index) = 0;
	virtual void Remove(RE::FormID a_formID) = 0;
	void Revert(SKSE::SerializationInterface*)
	{
		Clear();
	}

protected:
	using Lock = std::recursive_mutex;
	using Locker = std::lock_guard<Lock>;

	std::pair<std::set<T>, std::set<T>> _pair;
	mutable Lock _lock;
};

using FormData = std::pair<RE::FormID, RE::FormID>;
template <class F, class D>
class FormDataSetPair : public FormSetPairBase<FormData>
{
private:
	using super = FormSetPairBase;

public:
	FormDataSetPair() :
		super()
	{}

	FormDataSetPair(const FormDataSetPair&) = default;
	FormDataSetPair(FormDataSetPair&&) = default;
	~FormDataSetPair() = default;

	FormDataSetPair& operator=(const FormDataSetPair&) = default;
	FormDataSetPair& operator=(FormDataSetPair&&) = default;

	inline void Load(std::uint32_t a_index)
	{
		Locker locker(_lock);

		auto& dataSet = GetFormSet(a_index);
		for (const auto& [formID, dataID] : dataSet) {
			const auto form = RE::TESForm::LookupByID<F>(formID);
			const auto data = RE::TESForm::LookupByID<D>(dataID);
			if (form && data) {
				Process(form, data, a_index);
			}
		}
	}
	inline bool Add(F* a_form, D* a_data)
	{
		if (Process(a_form, a_data, 1)) {
			FormSetPairBase::Add({ a_form->GetFormID(), a_form->GetFormID() }, 1);
			return true;
		}
		return false;
	}
	inline bool Remove(F* a_form, D* a_data)
	{
		if (Process(a_form, a_data, 0)) {
			FormSetPairBase::Add({ a_form->GetFormID(), a_form->GetFormID() }, 0);
			return true;
		}
		return false;
	}

	bool Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_type, std::uint32_t a_version, std::uint32_t a_index) override
	{
		if (!a_intfc->OpenRecord(a_type, a_version)) {
			logger::error("Failed to open serialization record!"sv);
			return false;
		}

		return Save(a_intfc, a_index);
	}
	bool Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index) override
	{
		assert(a_intfc);
		Locker locker(_lock);

		auto& dataSet = GetFormSet(a_index);
		auto size = static_cast<std::uint32_t>(dataSet.size());
		a_intfc->WriteRecordData(size);

		for (const auto& [formID, dataID] : dataSet) {
			a_intfc->WriteRecordData(formID);
			a_intfc->WriteRecordData(dataID);
		}

		return true;
	}
	bool Load(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index) override
	{
		assert(a_intfc);
		std::uint32_t size;
		a_intfc->ReadRecordData(size);

		Locker locker(_lock);

		auto& dataSet = GetFormSet(a_index);
		dataSet.clear();

		for (std::uint32_t i = 0; i < size; i++) {
			RE::FormID formID;
			a_intfc->ReadRecordData(formID);
			if (!a_intfc->ResolveFormID(formID, formID)) {
				logger::error("{} : {} : Failed to resolve formID {}"sv, a_index, i, formID);
				continue;
			}

			RE::FormID dataID;
			a_intfc->ReadRecordData(dataID);
			if (!a_intfc->ResolveFormID(dataID, dataID)) {
				logger::error("{} : {} : Failed to resolve dataID {}"sv, a_index, i, dataID);
				continue;
			}

			dataSet.insert({ formID, dataID });
		}

		return true;
	}
	void Remove(RE::FormID a_formID) override
	{
		Locker locker(_lock);

		for (std::uint32_t i = 0; i < 2; i++) {
			auto& dataSet = GetFormSet(i);
			const auto it = std::ranges::find_if(dataSet,
				[&a_formID](const auto& formData) { return formData.first == a_formID; });
			if (it != dataSet.end()) {
				dataSet.erase(it);
			}
		}
	}

private:
	virtual bool Process(F* a_form, D* a_data, std::uint32_t a_index) = 0;
};

template <class F>
class FormSetPair : public FormSetPairBase<RE::FormID>
{
private:
	using super = FormSetPairBase;

public:
	FormSetPair() :
		super()
	{}

	FormSetPair(const FormSetPair&) = default;
	FormSetPair(FormSetPair&&) = default;
	~FormSetPair() = default;

	FormSetPair& operator=(const FormSetPair&) = default;
	FormSetPair& operator=(FormSetPair&&) = default;

	inline void Add(F* a_form, std::uint32_t a_index)
	{
		return FormSetPairBase::Add(a_form->GetFormID(), a_index);
	}
	inline void Remove(F* a_form, std::uint32_t a_index)
	{
		return FormSetPairBase::Remove(a_form->GetFormID(), a_index);
	}
	inline void Remove(F* a_form)
	{
		return Remove(a_form->GetFormID());
	}
	inline bool Contains(F* a_form, std::uint32_t a_index)
	{
		return FormSetPairBase::Contains(a_form->GetFormID(), a_index);
	}

	bool Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_type, std::uint32_t a_version, std::uint32_t a_index) override
	{
		if (!a_intfc->OpenRecord(a_type, a_version)) {
			logger::error("Failed to open serialization record!"sv);
			return false;
		}
		return Save(a_intfc, a_index);
	}
	bool Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index) override
	{
		assert(a_intfc);
		Locker locker(_lock);

		auto& formSet = GetFormSet(a_index);
		auto size = static_cast<std::uint32_t>(formSet.size());
		a_intfc->WriteRecordData(size);

		for (const auto& formID : formSet) {
			a_intfc->WriteRecordData(formID);
		}

		return true;
	}
	bool Load(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index) override
	{
		assert(a_intfc);
		std::uint32_t size;
		a_intfc->ReadRecordData(size);

		Locker locker(_lock);

		auto& formSet = GetFormSet(a_index);
		formSet.clear();

		for (std::uint32_t i = 0; i < size; i++) {
			RE::FormID formID;
			a_intfc->ReadRecordData(formID);
			if (!a_intfc->ResolveFormID(formID, formID)) {
				logger::error("{} : Failed to resolve formID {}"sv, i, formID);
				continue;
			}
			formSet.insert(formID);
		}

		return true;
	}
	void Remove(RE::FormID a_formID) override
	{
		Locker locker(_lock);

		for (std::uint32_t i = 0; i < 2; i++) {
			auto& formSet = GetFormSet(i);
			const auto it = std::ranges::find(formSet, a_formID);
			if (it != formSet.end()) {
				formSet.erase(it);
			}
		}
	}
};
