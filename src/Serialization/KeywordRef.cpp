#include "Serialization/KeywordRef.h"


namespace Serialize
{

	KeywordRef* KeywordRef::GetSingleton()
	{
		static KeywordRef singleton;
		return &singleton;
	}


	const char* KeywordRef::GetType(UInt32 a_add)
	{
		return a_add == kAdd ? "Keyword[ADD]" : "Keyword[REMOVE]";
	}


	void KeywordRef::ApplyData(FormRecordPair& a_pair, UInt32 a_add)
	{
		auto form = a_pair.first == kInvalid ? nullptr : RE::TESForm::LookupByID(a_pair.first);
		auto a_keyword = a_pair.second == kInvalid ? nullptr : RE::TESForm::LookupByID<RE::BGSKeyword>(a_pair.second);

		_MESSAGE("RESTORE : [%X] form : %s", a_pair.first, form->GetName(), a_pair.second, a_keyword->formEditorID.c_str());

		if (form && a_keyword) {
			if (ApplyKeywords(form, a_keyword, a_add)) {
				auto set = GetData(a_add);
				set.insert(a_pair);
			}
		}
	}


	bool KeywordRef::ApplyKeywords(RE::TESForm* a_form, RE::BGSKeyword* a_keyword, UInt32 a_add)
	{
		bool success = false;

		auto keywordForm = a_form->As<RE::BGSKeywordForm>();
		if (keywordForm) {
			success = a_add == kAdd ? keywordForm->AddKeyword(a_keyword) : keywordForm->RemoveKeyword(a_keyword);
		}

		return success;
	}


	bool KeywordRef::PapyrusApplyKeywords(RE::TESObjectREFR* a_ref, RE::BGSKeyword* a_keyword, UInt32 a_add)
	{
		if (ApplyKeywords(a_ref, a_keyword, a_add)) {
			FormRecordPair pair = std::make_pair(a_ref->formID, a_keyword->formID);
			_MESSAGE("PAPYRUS : [%X] ref : %s", pair.first, a_ref->GetName());

			auto& set = GetData(a_add);
			auto& otherSet = GetData(!a_add);

			auto it = otherSet.find(pair);
			if (it != otherSet.end()) {
				otherSet.erase(it);
			}
			set.insert(pair);

			return true;
		}

		return false;
	}
}