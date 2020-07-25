#include "Serialization/Keywords.h"


namespace Serialize
{
	Keywords* Keywords::GetSingleton()
	{
		static Keywords singleton;
		return &singleton;
	}


	const char* Keywords::GetType(std::uint32_t a_add)
	{
		return a_add == kAdd ? "Keyword[ADD]" : "Keyword[REMOVE]";
	}


	void Keywords::LoadData(FormData a_data, std::uint32_t a_add)
	{
		auto form = a_data._formID == kInvalid ? nullptr : RE::TESForm::LookupByID(a_data._formID);
		auto keyword = a_data._dataID == kInvalid ? nullptr : RE::TESForm::LookupByID<RE::BGSKeyword>(a_data._dataID);

		if (form && keyword) {
			if (ApplyKeywords(form, keyword, a_add)) {
				GetData(a_add).push_back(a_data);
			}
		}
	}


	bool Keywords::ApplyKeywords(RE::TESForm* a_form, RE::BGSKeyword* a_keyword, std::uint32_t a_add)
	{
		bool success = false;

		auto keywordForm = a_form->As<RE::BGSKeywordForm>();
		if (keywordForm) {
			success = a_add == kAdd ? keywordForm->AddKeyword(a_keyword) : keywordForm->RemoveKeyword(a_keyword);
		}

		return success;
	}


	bool Keywords::PapyrusApplyKeywords(RE::TESForm* a_form, RE::BGSKeyword* a_keyword, std::uint32_t a_add)
	{
		if (ApplyKeywords(a_form, a_keyword, a_add)) {
			if (a_form->formID >= 0xFF000000) {
				logger::warn("Cannot serialize temporary objects - [%X] %s", a_form->formID, a_form->GetName());
				return true;
			}
			SaveData({ a_form->formID, a_keyword->formID }, a_add);
			return true;
		}
		return false;
	}
}