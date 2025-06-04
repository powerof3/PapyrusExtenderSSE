#pragma once

template <class D>
class DataSetPair
{
public:
	DataSetPair() = default;
	~DataSetPair() = default;

	DataSetPair(const DataSetPair& a_rhs)
	{
		Locker lock(a_rhs._lock);
		_addSet = a_rhs._addSet;
		_removeSet = a_rhs._removeSet;
	}

	DataSetPair(DataSetPair&& a_rhs) noexcept
	{
		Locker lock(a_rhs._lock);
		_addSet = std::move(a_rhs._addSet);
		_removeSet = std::move(a_rhs._removeSet);
		a_rhs._addSet.clear();
		a_rhs._removeSet.clear();
	}

	DataSetPair& operator=(const DataSetPair& a_rhs)
	{
		if (this == &a_rhs) {
			return *this;
		}

		Locker lhsLocker(_lock);
		Locker rhsLocker(a_rhs._lock);

		_addSet = a_rhs._addSet;
		_removeSet = a_rhs._removeSet;

		return *this;
	}

	DataSetPair& operator=(DataSetPair&& a_rhs) noexcept
	{
		if (this == &a_rhs) {
			return *this;
		}

		Locker lhsLocker(_lock);
		Locker rhsLocker(a_rhs._lock);

		_addSet = std::move(a_rhs._addSet);
		_removeSet = std::move(a_rhs._removeSet);
		a_rhs._addSet.clear();
		a_rhs._removeSet.clear();

		return *this;
	}

	Set<D>& GetData(std::uint32_t a_index)
	{
		return a_index == 1 ? _addSet : _removeSet;
	}

	const Set<D>& GetData(std::uint32_t a_index) const
	{
		return a_index == 1 ? _addSet : _removeSet;
	}

	void Add(D a_data, std::uint32_t a_index)
	{
		Locker locker(_lock);
		GetData(!a_index).erase(a_data);
		GetData(a_index).insert(a_data);
	}

	void Remove(D a_data)
	{
		Locker locker(_lock);
		_addSet.erase(a_data);
		_removeSet.erase(a_data);
	}

	void Remove(D a_data, std::uint32_t a_index)
	{
		Locker locker(_lock);
		GetData(a_index).erase(a_data);
	}

	bool Contains(D a_data, std::uint32_t a_index) const
	{
		Locker locker(_lock);
		return GetData(a_index).contains(a_data);
	}

	void Clear()
	{
		Locker locker(_lock);
		_addSet.clear();
		_removeSet.clear();
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

	Set<D>       _addSet{};
	Set<D>       _removeSet{};
	mutable Lock _lock{};
};

template <class F>
class FormSetPair : public DataSetPair<RE::FormID>
{
public:
	FormSetPair() :
		DataSetPair()
	{}

	FormSetPair(const FormSetPair&) = default;
	FormSetPair(FormSetPair&&) = default;
	~FormSetPair() = default;

	FormSetPair& operator=(const FormSetPair&) = default;
	FormSetPair& operator=(FormSetPair&&) = default;

	void Add(F* a_form, std::uint32_t a_index)
	{
		DataSetPair::Add(a_form->GetFormID(), a_index);
	}

	void Remove(F* a_form, std::uint32_t a_index)
	{
		DataSetPair::Remove(a_form->GetFormID(), a_index);
	}

	void Remove(F* a_form)
	{
		DataSetPair::Remove(a_form->GetFormID());
	}

	void Remove(RE::FormID a_formID)
	{
		Locker locker(_lock);
		_addSet.erase(a_formID);
		_removeSet.erase(a_formID);
	}

	bool Contains(F* a_form, std::uint32_t a_index) const
	{
		return DataSetPair::Contains(a_form->GetFormID(), a_index);
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

		const auto& formSet = GetData(a_index);

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

		Locker locker(_lock);

		auto& formSet = GetData(a_index);
		formSet.clear();

		std::size_t numRegs;
		a_intfc->ReadRecordData(numRegs);

		RE::FormID formID;
		for (std::size_t i = 0; i < numRegs; i++) {
			if (!stl::read_formID(a_intfc, formID)) {
				logger::warn("{} : Failed to resolve formID {:X}"sv, i, formID);
			} else {
				formSet.insert(formID);
			}
		}

		return true;
	}
};
