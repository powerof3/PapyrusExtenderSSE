#pragma once

template <class K, class D>
class DataMapPair
{
public:
	using Type = Map<K, std::vector<D>>;

	DataMapPair() = default;
	~DataMapPair() = default;

	DataMapPair(const DataMapPair& a_rhs) :
		_addMap(),
		_removeMap(),
		_lock()
	{
		Locker locker(a_rhs._lock);
		_addMap = a_rhs._addMap;
		_removeMap = a_rhs._removeMap;
	}

	DataMapPair(DataMapPair&& a_rhs) noexcept :
		_addMap(),
		_removeMap(),
		_lock()
	{
		Locker locker(a_rhs._lock);
		_addMap = std::move(a_rhs._addMap);
		_removeMap = std::move(a_rhs._removeMap);
		a_rhs._addMap.clear();
		a_rhs._removeMap.clear();
	}

	DataMapPair& operator=(const DataMapPair& a_rhs)
	{
		if (this == &a_rhs) {
			return *this;
		}

		Locker lhsLocker(_lock);
		Locker rhsLocker(a_rhs._lock);
		_addMap = a_rhs._addMap;
		_removeMap = a_rhs._removeMap;

		return *this;
	}

	DataMapPair& operator=(DataMapPair&& a_rhs) noexcept
	{
		if (this == &a_rhs) {
			return *this;
		}

		Locker lhsLocker(_lock);
		Locker rhsLocker(a_rhs._lock);

		_addMap = std::move(a_rhs._addMap);
		_removeMap = std::move(a_rhs._removeMap);

		a_rhs._addMap.clear();
		a_rhs._removeMap.clear();

		return *this;
	}

	Type& GetData(std::uint32_t a_index)
	{
		return a_index == 1 ? _addMap : _removeMap;
	}

	const Type& GetData(std::uint32_t a_index) const
	{
		return a_index == 1 ? _addMap : _removeMap;
	}

	void AddData(K a_key, D a_data, std::uint32_t a_index)
	{
		Locker locker(_lock);

		auto& otherDataMap = GetData(!a_index);
		if (auto it = otherDataMap.find(a_key); it != otherDataMap.end()) {
			std::erase(it->second, a_data);
			if (it->second.empty()) {
				otherDataMap.erase(it);
			}
		}

		GetData(a_index)[a_key].push_back(a_data);
	}

	void RemoveData(K a_key, D a_data, std::uint32_t a_index)
	{
		Locker locker(_lock);

		auto& dataMap = GetData(a_index);
		if (auto it = dataMap.find(a_key); it != dataMap.end()) {
			std::erase(it->second, a_data);
			if (it->second.empty()) {
				dataMap.erase(it);
			}
		}
	}

	void Remove(K a_key)
	{
		Locker locker(_lock);
		_addMap.erase(a_key);
		_removeMap.erase(a_key);
	}

	void Clear()
	{
		Locker locker(_lock);
		_addMap.clear();
		_removeMap.clear();
	}

	void Clear(std::uint32_t a_index)
	{
		Locker locker(_lock);
		GetData(a_index).clear();
	}

	void Revert(SKSE::SerializationInterface*)
	{
		Clear();
	}

protected:
	using Lock = std::recursive_mutex;
	using Locker = std::scoped_lock<Lock>;

	Type         _addMap;
	Type         _removeMap;
	mutable Lock _lock;
};

template <class F, class D>
class FormMapPair : public DataMapPair<RE::FormID, RE::FormID>
{
public:
	FormMapPair() = default;
	FormMapPair(const FormMapPair&) = default;
	FormMapPair(FormMapPair&&) = default;

	virtual ~FormMapPair() = default;

	FormMapPair& operator=(const FormMapPair&) = default;
	FormMapPair& operator=(FormMapPair&&) = default;

	bool Add(F* a_form, D* a_data)
	{
		if (Process(a_form, a_data, 1)) {
			DataMapPair::AddData(a_form->GetFormID(), a_data->GetFormID(), 1);
			return true;
		}
		return false;
	}

	bool Remove(F* a_form, D* a_data)
	{
		if (Process(a_form, a_data, 0)) {
			DataMapPair::RemoveData(a_form->GetFormID(), a_data->GetFormID(), 0);
			return true;
		}
		return false;
	}

	void Remove(RE::FormID a_formID)
	{
		DataMapPair::Remove(a_formID);
	}

	bool Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_type, std::uint32_t a_version, std::uint32_t a_index) const
	{
		if (!a_intfc->OpenRecord(a_type, a_version)) {
			logger::error("Failed to open serialization record!"sv);
			return false;
		}
		return Save(a_intfc, a_index);
	}

	bool Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index) const
	{
		assert(a_intfc);
		Locker locker(_lock);

		auto& formMap = GetData(a_index);

		const std::size_t numRegs = formMap.size();
		if (!a_intfc->WriteRecordData(numRegs)) {
			logger::error("Failed to save reg count ({})", numRegs);
			return false;
		}

		for (const auto& [formID, data] : formMap) {
			if (!a_intfc->WriteRecordData(formID)) {
				logger::error("Failed to save formID ({:X})", formID);
				return false;
			}
			const std::size_t numData = data.size();
			if (!a_intfc->WriteRecordData(numData)) {
				logger::error("Failed to save data reg count ({})", numData);
				return false;
			}
			for (const auto& dataID : data) {
				if (!a_intfc->WriteRecordData(dataID)) {
					logger::error("Failed to save dataID ({:X})", dataID);
					return false;
				}
			}
		}

		return true;
	}

	bool Load(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index)
	{
		assert(a_intfc);

		Locker locker(_lock);

		auto& formMap = GetData(a_index);
		formMap.clear();

		std::size_t numRegs;
		a_intfc->ReadRecordData(numRegs);

		RE::FormID  formID;
		RE::FormID  dataID;
		std::size_t numData;

		for (std::size_t i = 0; i < numRegs; i++) {
			if (!stl::read_formID(a_intfc, formID)) {
				logger::warn("{} : {} : Failed to resolve formID {:X}"sv, a_index, i, formID);
				continue;
			}
			a_intfc->ReadRecordData(numData);
			for (std::size_t j = 0; j < numData; j++) {
				if (!stl::read_formID(a_intfc, dataID)) {
					logger::warn("{} : {} : Failed to resolve dataID {:X}"sv, a_index, j, dataID);
					continue;
				}
				formMap[formID].push_back(dataID);
			}
		}

		for (const auto& [fID, dataSet] : formMap) {
			if (const auto form = RE::TESForm::LookupByID<F>(fID); form) {
				for (auto& dID : dataSet) {
					if (const auto data = RE::TESForm::LookupByID<D>(dID); data) {
						Process(form, data, a_index);
					}
				}
			}
		}

		return true;
	}

private:
	virtual bool Process(F* a_form, D* a_data, std::uint32_t a_index) = 0;
};

template <class F, class D>
class FormDataMapPair : public DataMapPair<RE::FormID, D>
{
public:
	FormDataMapPair() = default;
	FormDataMapPair(const FormDataMapPair&) = default;
	FormDataMapPair(FormDataMapPair&&) = default;

	virtual ~FormDataMapPair() = default;

	FormDataMapPair& operator=(const FormDataMapPair&) = default;
	FormDataMapPair& operator=(FormDataMapPair&&) = default;

	bool Add(F* a_form, const D& a_data)
	{
		if (Process(a_form, a_data, 1)) {
			DataMapPair<RE::FormID, D>::AddData(a_form->GetFormID(), a_data, 1);
			return true;
		}
		return false;
	}

	bool Remove(F* a_form, const D& a_data)
	{
		if (Process(a_form, a_data, 0)) {
			DataMapPair<RE::FormID, D>::RemoveData(a_form->GetFormID(), a_data, 0);
			return true;
		}
		return false;
	}

	bool Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_type, std::uint32_t a_version, std::uint32_t a_index) const
	{
		if (!a_intfc->OpenRecord(a_type, a_version)) {
			logger::error("Failed to open serialization record!"sv);
			return false;
		}
		return Save_Impl(a_intfc, a_index);
	}

	virtual bool Save_Impl(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index) const = 0;
	virtual bool Load(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index) = 0;

private:
	virtual bool Process(F* a_form, const D& a_data, std::uint32_t a_index) = 0;
};
