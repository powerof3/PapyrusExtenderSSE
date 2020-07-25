#include "Serialization/Base.h"


namespace Serialize
{
	Base::Base() :
		_add(),
		_remove()
	{
	}


	std::vector<Base::FormData>& Base::GetData(std::uint32_t a_add)
	{
		return a_add == kAdd ? _add : _remove;
	}


	void Base::SaveData(FormData a_newData, std::uint32_t a_add)
	{
		auto otherData = GetData(!a_add);
		const auto p = std::find_if(otherData.begin(), otherData.end(), [a_newData](const FormData& data) {
			return data._formID == a_newData._formID && data._dataID == a_newData._dataID;
		});
		if (p != otherData.end()) {
			otherData.erase(p);
		}
		GetData(a_add).push_back(a_newData);
	}


	void Base::Clear(std::uint32_t a_add)
	{
		GetData(a_add).clear();
	}


	bool Base::Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_type, std::uint32_t a_version, std::uint32_t a_add)
	{
		if (!a_intfc->OpenRecord(a_type, a_version)) {
			logger::error("Failed to open serialization record!\n");
			return false;
		} else {
			return Save(a_intfc, a_add);
		}
	}


	bool Base::Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_add)
	{
		std::uint32_t size = GetData(a_add).size();
		a_intfc->WriteRecordData(&size, sizeof(size));

		for (auto& formData : GetData(a_add)) {
			a_intfc->WriteRecordData(&formData._formID, sizeof(formData._formID));
			a_intfc->WriteRecordData(&formData._dataID, sizeof(formData._dataID));
		}

		return true;
	}


	bool Base::Load(SKSE::SerializationInterface* a_intfc, std::uint32_t a_add)
	{
		std::uint32_t size;
		a_intfc->ReadRecordData(&size, sizeof(size));
		for (std::uint32_t i = 0; i < size; i++) {
			FormData data;
			for (std::uint32_t k = 0; k < 2; k++) {
				RE::FormID formID;
				a_intfc->ReadRecordData(&formID, sizeof(formID));
				if (!a_intfc->ResolveFormID(formID, formID)) {
					logger::error("[%s] : [%u] [%u] : Failed to resolve formID", GetType(a_add), i, k);
					formID = kInvalid;
				}
				k == 0 ? data._formID = formID : data._dataID = formID;
			}
			LoadData(data, a_add);
		}
		return true;
	}
}