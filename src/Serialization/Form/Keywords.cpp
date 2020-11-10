#include "Serialization/Form/Keywords.h"


namespace Serialization
{
	using namespace Form;

	Keywords* Keywords::GetSingleton()
	{
		static Keywords singleton;
		return &singleton;
	}


	void Keywords::LoadData(std::uint32_t a_add)
	{
		Locker locker(_lock);
		
		auto& dataSet = GetData(a_add);
		for (auto& [formID, dataID] : dataSet) {
		
			auto form = RE::TESForm::LookupByID(formID);
			auto keyword = RE::TESForm::LookupByID<RE::BGSKeyword>(dataID);

			if (form && keyword) {
				Apply(form, keyword, a_add);
			}
		}
	}


	bool Keywords::Apply(RE::TESForm* a_form, RE::BGSKeyword* a_keyword, std::uint32_t a_add)
	{
		bool success = false;

		auto keywordForm = a_form->As<RE::BGSKeywordForm>();
		if (keywordForm) {
			success = a_add == kAdd ? keywordForm->AddKeyword(a_keyword) : keywordForm->RemoveKeyword(a_keyword);
		}

		return success;
	}


	bool Keywords::PapyrusApply(RE::TESForm* a_form, RE::BGSKeyword* a_keyword, std::uint32_t a_add)
	{	
		if (Apply(a_form, a_keyword, a_add)) {
			if (a_form->IsDynamicForm()) {
				logger::warn("Cannot serialize temporary objects - [0x{:08X}] {}", a_form->formID, a_form->GetName());
				return true;
			}
			SaveData({ a_form->formID, a_keyword->formID }, a_add);
			return true;
		}
		return false;
	}
}