#pragma once

template <class K, class D>
class DataMapPair
{
public:
	using ValueType = std::vector<D>;
	using Container = Map<K, ValueType>;

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

	Container& GetData(std::uint32_t a_index)
	{
		return a_index == 1 ? _addMap : _removeMap;
	}

	const Container& GetData(std::uint32_t a_index) const
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

	void Clear(std::uint32_t a_index)
	{
		Locker locker(_lock);
		GetData(a_index).clear();
	}

	void Revert(SKSE::SerializationInterface*)
	{
		Locker locker(_lock);

		for (auto& [key, data] : _addMap) {
			ProcessEntry(key, data, 0); // flip add/remove on revert
		}
		_addMap.clear();

		for (auto& [key, data] : _removeMap) {
			ProcessEntry(key, data, 1);
		}
		_removeMap.clear();
	}

protected:
	using Lock = std::recursive_mutex;
	using Locker = std::scoped_lock<Lock>;

	virtual void ProcessEntry(const K a_key, ValueType& a_data, std::uint32_t a_index) = 0;

	Container    _addMap;
	Container    _removeMap;
	mutable Lock _lock;
};

template <class F, class D>
class FormMapPair : public DataMapPair<RE::FormID, RE::FormID>
{
public:
	using Base = DataMapPair<RE::FormID, RE::FormID>;

	FormMapPair() = default;
	FormMapPair(const FormMapPair&) = default;
	FormMapPair(FormMapPair&&) = default;

	virtual ~FormMapPair() = default;

	FormMapPair& operator=(const FormMapPair&) = default;
	FormMapPair& operator=(FormMapPair&&) = default;

	bool Add(F* a_form, D* a_data)
	{
		if (Process(a_form, a_data, 1)) {
			Base::AddData(a_form->GetFormID(), a_data->GetFormID(), 1);
			return true;
		}
		return false;
	}

	bool Remove(F* a_form, D* a_data)
	{
		if (Process(a_form, a_data, 0)) {
			Base::RemoveData(a_form->GetFormID(), a_data->GetFormID(), 0);
			return true;
		}
		return false;
	}

	void Remove(RE::FormID a_formID)
	{
		Locker locker(_lock);

		if (auto it = _addMap.find(a_formID); it != _addMap.end()) {
			ProcessEntry(it->first, it->second, 0);
			_addMap.erase(it);
		}

		if (auto it = _removeMap.find(a_formID); it != _removeMap.end()) {
			ProcessEntry(it->first, it->second, 1);
			_removeMap.erase(it);
		}
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

		for (auto& [key, data] : formMap) {
			ProcessEntry(key, data, a_index);
		}

		return true;
	}

protected:
	void ProcessEntry(const RE::FormID a_key, std::vector<RE::FormID>& a_data, std::uint32_t a_index) override
	{
		if (const auto form = RE::TESForm::LookupByID<F>(a_key); form) {
			for (auto& dID : a_data) {
				if (const auto data = RE::TESForm::LookupByID<D>(dID); data) {
					logger::info("processing {} : {}", editorID::get_editorID(data), a_index);
					Process(form, data, a_index);
				}
			}
		}
	}

	virtual bool Process(F* a_form, D* a_data, std::uint32_t a_index) = 0;
};

template <class F, class D>
class FormDataMapPair : public DataMapPair<RE::FormID, D>
{
public:
	using Base = DataMapPair<RE::FormID, D>;

	FormDataMapPair() = default;
	FormDataMapPair(const FormDataMapPair&) = default;
	FormDataMapPair(FormDataMapPair&&) = default;

	virtual ~FormDataMapPair() = default;

	FormDataMapPair& operator=(const FormDataMapPair&) = default;
	FormDataMapPair& operator=(FormDataMapPair&&) = default;

	bool Add(F* a_form, const D& a_data)
	{
		if (Process(a_form, a_data, 1)) {
			Base::AddData(a_form->GetFormID(), a_data, 1);
			return true;
		}
		return false;
	}

	bool Remove(F* a_form, const D& a_data)
	{
		if (Process(a_form, a_data, 0)) {
			Base::RemoveData(a_form->GetFormID(), a_data, 0);
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
		return SaveImpl(a_intfc, a_index);
	}

	bool Load(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index)
	{
		assert(a_intfc);
		typename Base::Locker locker(this->_lock);

		auto& formMap = this->GetData(a_index);
		formMap.clear();

		std::size_t numRegs;
		a_intfc->ReadRecordData(numRegs);

		RE::FormID  formID;
		std::size_t numData;

		for (std::size_t i = 0; i < numRegs; i++) {
			if (!stl::read_formID(a_intfc, formID)) {
				logger::warn("{} : Failed to resolve formID {:X}"sv, i, formID);
				continue;
			}
			a_intfc->ReadRecordData(numData);
			for (std::size_t j = 0; j < numData; j++) {
				D data{};
				if (!data.load(a_intfc, j)) {
					continue;
				}
				formMap[formID].emplace_back(std::move(data));
			}
		}

		for (auto& [dataID, dataVec] : formMap) {
			this->ProcessEntry(dataID, dataVec, a_index);
		}

		return true;
	}

protected:
	virtual bool Process(F* a_form, const D& a_data, std::uint32_t a_index) = 0;
	virtual bool SaveImpl(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index) const = 0;
};
