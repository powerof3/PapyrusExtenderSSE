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

	std::set<T>& GetFormSet(std::uint32_t a_index)
	{
		return a_index == 1 ? _pair.first :
                              _pair.second;
	}
	void Add(T a_data, std::uint32_t a_index)
	{
		Locker locker(_lock);

		auto& otherDataSet = GetFormSet(!a_index);
		auto it = std::ranges::find(otherDataSet, a_data);
		if (it != otherDataSet.end()) {
			otherDataSet.erase(it);
		}

		GetFormSet(a_index).insert(a_data);
	}
	void Remove(T a_data, std::uint32_t a_index)
	{
		Locker locker(_lock);

		auto& formSet = GetFormSet(a_index);
		auto it = formSet.find(a_data);
		if (it != formSet.end()) {
			formSet.erase(it);
		}
	}
	bool Contains(T a_data, std::uint32_t a_index)
	{
		Locker locker(_lock);
		return GetFormSet(a_index).contains(a_data);
	}
	void Clear()
	{
		Locker locker(_lock);
		_pair.first.clear();
		_pair.second.clear();
	}
	void Clear(std::uint32_t a_index)
	{
		Locker locker(_lock);
		GetFormSet(a_index).clear();
	}
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
public:
	FormDataSetPair() :
		FormSetPairBase()
	{}

	FormDataSetPair(const FormDataSetPair&) = default;
	FormDataSetPair(FormDataSetPair&&) = default;
	~FormDataSetPair() = default;

	FormDataSetPair& operator=(const FormDataSetPair&) = default;
	FormDataSetPair& operator=(FormDataSetPair&&) = default;

	bool Add(F* a_form, D* a_data)
	{
		if (Process(a_form, a_data, 1)) {
			FormSetPairBase::Add({ a_form->GetFormID(), a_form->GetFormID() }, 1);
			return true;
		}
		return false;
	}
	bool Remove(F* a_form, D* a_data)
	{
		if (Process(a_form, a_data, 0)) {
			FormSetPairBase::Add({ a_form->GetFormID(), a_form->GetFormID() }, 0);
			return true;
		}
		return false;
	}

	bool Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_type, std::uint32_t a_version, std::uint32_t a_index)
	{
		if (!a_intfc->OpenRecord(a_type, a_version)) {
			logger::error("Failed to open serialization record!"sv);
			return false;
		}

		return Save(a_intfc, a_index);
	}
	bool Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index)
	{
		assert(a_intfc);
		Locker locker(_lock);

		auto& dataSet = GetFormSet(a_index);
		const std::size_t numRegs = dataSet.size();
		if (!a_intfc->WriteRecordData(numRegs)) {
			logger::error("Failed to save number of regs ({})", numRegs);
			return false;
		}

		for (const auto& [formID, dataID] : dataSet) {
			if (!a_intfc->WriteRecordData(formID)) {
				logger::error("Failed to save formID ({:X})", formID);
				return false;			    
			}
			if (!a_intfc->WriteRecordData(dataID)) {
				logger::error("Failed to save dataID ({:X})", dataID);
				return false;
			}
		}

		return true;
	}
	bool Load(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index)
	{
		assert(a_intfc);
		std::size_t numRegs;
		a_intfc->ReadRecordData(numRegs);

		Locker locker(_lock);

		auto& dataSet = GetFormSet(a_index);
		dataSet.clear();

		RE::FormID formID;
		RE::FormID dataID;
	    for (std::size_t i = 0; i < numRegs; i++) {
			a_intfc->ReadRecordData(formID);
			if (!a_intfc->ResolveFormID(formID, formID)) {
				logger::warn("{} : {} : Failed to resolve formID {:X}"sv, a_index, i, formID);
				continue;
			}
			a_intfc->ReadRecordData(dataID);
			if (!a_intfc->ResolveFormID(dataID, dataID)) {
				logger::warn("{} : {} : Failed to resolve dataID {:X}"sv, a_index, i, dataID);
				continue;
			}
			dataSet.insert({ formID, dataID });
		}

		for (const auto& [fID, dID] : dataSet) {
			const auto form = RE::TESForm::LookupByID<F>(fID);
			const auto data = RE::TESForm::LookupByID<D>(dID);
			if (form && data) {
				Process(form, data, a_index);
			}
		}

		return true;
	}
	void Remove(RE::FormID a_formID)
	{
		Locker locker(_lock);

		for (std::uint32_t i = 0; i < 2; i++) {
			auto& dataSet = GetFormSet(i);
			std::erase_if(dataSet, [a_formID](const auto& formData) {
				return formData.first == a_formID;
			});
		}
	}

private:
	virtual bool Process(F* a_form, D* a_data, std::uint32_t a_index) = 0;
};

template <class F>
class FormSetPair : public FormSetPairBase<RE::FormID>
{
public:
	FormSetPair() :
		FormSetPairBase()
	{}

	FormSetPair(const FormSetPair&) = default;
	FormSetPair(FormSetPair&&) = default;
	~FormSetPair() = default;

	FormSetPair& operator=(const FormSetPair&) = default;
	FormSetPair& operator=(FormSetPair&&) = default;

	void Add(F* a_form, std::uint32_t a_index)
	{
		return FormSetPairBase::Add(a_form->GetFormID(), a_index);
	}
	void Remove(F* a_form, std::uint32_t a_index)
	{
		return FormSetPairBase::Remove(a_form->GetFormID(), a_index);
	}
	void Remove(F* a_form)
	{
		return Remove(a_form->GetFormID());
	}
	bool Contains(F* a_form, std::uint32_t a_index)
	{
		return FormSetPairBase::Contains(a_form->GetFormID(), a_index);
	}

	bool Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_type, std::uint32_t a_version, std::uint32_t a_index)
	{
		if (!a_intfc->OpenRecord(a_type, a_version)) {
			logger::error("Failed to open serialization record!"sv);
			return false;
		}
		return Save(a_intfc, a_index);
	}
	bool Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index)
	{
		assert(a_intfc);
		Locker locker(_lock);

		const auto& formSet = GetFormSet(a_index);
		const std::size_t numRegs = formSet.size();
		if (!a_intfc->WriteRecordData(numRegs)) {
			logger::error("Failed to save number of regs ({})", numRegs);
			return false;
		}

		for (const auto& formID : formSet) {
			if (!a_intfc->WriteRecordData(formID)) {
				logger::error("Failed to save reg ({:X})", formID);
				return false;
			}
		}

		return true;
	}
	bool Load(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index)
	{
		assert(a_intfc);
		std::size_t numRegs;
		a_intfc->ReadRecordData(numRegs);

		Locker locker(_lock);
		auto& formSet = GetFormSet(a_index);
		formSet.clear();

		RE::FormID formID;
	    for (std::size_t i = 0; i < numRegs; i++) {
			a_intfc->ReadRecordData(formID);
			if (!a_intfc->ResolveFormID(formID, formID)) {
				logger::warn("{} : Failed to resolve formID {:X}"sv, i, formID);
			} else {
				formSet.insert(formID);
			}
		}

		return true;
	}
	void Remove(RE::FormID a_formID)
	{
		Locker locker(_lock);

		for (std::uint32_t i = 0; i < 2; i++) {
			auto& formSet = GetFormSet(i);
			std::erase_if(formSet, [a_formID](const auto& ID) {
				return ID == a_formID;
			});
		}
	}
};
