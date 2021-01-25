#include "Serialization/Form/Base.h"


namespace Serialization
{
	using namespace Form;

	Base::Base() :
		_add(),
		_remove(),
		_lock()
	{
	}


	std::set<Base::FormData>& Base::GetData(std::uint32_t a_add)
	{
		return a_add == kAdd ? _add : _remove;
	}


	void Base::SaveData(FormData a_newData, std::uint32_t a_add)
	{	
		Locker locker(_lock);
		
		auto& otherData = GetData(!a_add);
		auto it = std::find(otherData.begin(), otherData.end(), a_newData);
		if (it != otherData.end()) {
			otherData.erase(it);
		}
		GetData(a_add).insert(a_newData);
	}


	void Base::Clear(std::uint32_t a_add)
	{
		Locker locker(_lock);
		GetData(a_add).clear();
	}


	void Base::ClearAll()
	{
		Locker locker(_lock);
		_add.clear();
		_remove.clear();
	}


	bool Base::Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_type, std::uint32_t a_version, std::uint32_t a_add)
	{
		if (!a_intfc->OpenRecord(a_type, a_version)) {
			logger::error("Failed to open serialization record!"sv);
			return false;
		} else {
			return Save(a_intfc, a_add);
		}
	}


	bool Base::Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_add)
	{
		assert(a_intfc);
		Locker locker(_lock);
		
		auto size = static_cast<std::uint32_t>(GetData(a_add).size());
		a_intfc->WriteRecordData(size);

		for (auto& [formID, dataID] : GetData(a_add)) {
			a_intfc->WriteRecordData(formID);
			a_intfc->WriteRecordData(dataID);
		}

		return true;
	}


	bool Base::Load(SKSE::SerializationInterface* a_intfc, std::uint32_t a_add)
	{
		assert(a_intfc);
		std::uint32_t size;
		a_intfc->ReadRecordData(size);
	
		Locker locker(_lock);
		auto& dataSet = GetData(a_add);
		dataSet.clear();

		for (std::uint32_t i = 0; i < size; i++) {
			
			RE::FormID formID;
			a_intfc->ReadRecordData(formID);
			if (!a_intfc->ResolveFormID(formID, formID)) {
				logger::error("{} : {} : Failed to resolve formID {}"sv, a_add, i, formID);
				continue;
			}

			RE::FormID dataID;
			a_intfc->ReadRecordData(dataID);
			if (!a_intfc->ResolveFormID(dataID, dataID)) {
				logger::error("{} : {} : Failed to resolve dataID {}"sv, a_add, i, dataID);
				continue;
			}
						
			dataSet.insert({ formID, dataID });
		}		
		
		return true;
	}
}