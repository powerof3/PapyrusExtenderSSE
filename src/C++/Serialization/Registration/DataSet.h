#pragma once

template <class D>
class DataSetPair
{
public:
	DataSetPair() :
		_pair(),
		_lock()
	{
	}
	DataSetPair(const DataSetPair& a_rhs) :
		_pair(),
		_lock()
	{
		a_rhs._lock.lock();
		_pair = a_rhs._pair;
		a_rhs._lock.unlock();
	}
	DataSetPair(DataSetPair&& a_rhs) :
		_pair(),
		_lock()
	{
		Locker locker(a_rhs._lock);
		_pair = std::move(a_rhs._pair);
		a_rhs._pair.first.clear();
		a_rhs._pair.second.clear();
	}

	~DataSetPair() = default;

	DataSetPair& operator=(const DataSetPair& a_rhs)
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
	DataSetPair& operator=(DataSetPair&& a_rhs)
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

	std::set<D>& GetData(std::uint32_t a_index)
	{
		return a_index == 1 ? _pair.first :
                              _pair.second;
	}
	void Add(D a_data, std::uint32_t a_index)
	{
		Locker locker(_lock);

		GetData(!a_index).erase(a_data);
		GetData(a_index).insert(a_data);
	}
	void Remove(D a_data, std::uint32_t a_index)
	{
		Locker locker(_lock);

		GetData(a_index).erase(a_data);
	}
	bool Contains(D a_data, std::uint32_t a_index)
	{
		Locker locker(_lock);
		return GetData(a_index).contains(a_data);
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
		GetData(a_index).clear();
	}
	void Revert(SKSE::SerializationInterface*)
	{
		Clear();
	}

protected:
	using Lock = std::recursive_mutex;
	using Locker = std::lock_guard<Lock>;

	std::pair<
		std::set<D>,
		std::set<D>>
		_pair;
	mutable Lock _lock;
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
		return DataSetPair::Add(a_form->GetFormID(), a_index);
	}
	void Remove(F* a_form, std::uint32_t a_index)
	{
		return DataSetPair::Remove(a_form->GetFormID(), a_index);
	}
	void Remove(F* a_form)
	{
		return Remove(a_form->GetFormID());
	}
	bool Contains(F* a_form, std::uint32_t a_index)
	{
		return DataSetPair::Contains(a_form->GetFormID(), a_index);
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
		std::size_t numRegs;
		a_intfc->ReadRecordData(numRegs);

		Locker locker(_lock);
		auto& formSet = GetData(a_index);
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
			GetData(i).erase(a_formID);
		}
	}
};
