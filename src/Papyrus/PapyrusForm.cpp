#include "Papyrus/PapyrusForm.h"

#include "Serialization/Keywords.h"


void papyrusForm::AddKeywordToForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form, RE::BGSKeyword* a_add)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_add) {
		a_vm->TraceStack("Keyword is None", a_stackID, Severity::kWarning);
		return;
	}

	auto keywords = Serialize::Keywords::GetSingleton();
	keywords->PapyrusApplyKeywords(a_form, a_add, Serialize::kAdd);
}


bool papyrusForm::IsGeneratedForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return false;
	}

	return a_form->formID >= 0xFF000000;
}


bool papyrusForm::RemoveKeywordOnForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form, RE::BGSKeyword* a_remove)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return false;
	} else if (!a_remove) {
		a_vm->TraceStack("Keyword is None", a_stackID, Severity::kWarning);
		return false;
	}

	auto keywords = Serialize::Keywords::GetSingleton();
	return keywords->PapyrusApplyKeywords(a_form, a_remove, Serialize::kRemove);
}


void papyrusForm::ReplaceKeywordOnForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form, RE::BGSKeyword* a_remove, RE::BGSKeyword* a_add)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_remove) {
		a_vm->TraceStack("Remove keyword is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_add) {
		a_vm->TraceStack("Add keyword is None", a_stackID, Severity::kWarning);
		return;
	}

	auto keywordForm = a_form->As<RE::BGSKeywordForm>();
	if (keywordForm) {
		std::uint32_t removeIndex = 0;
		bool found = false;
		if (keywordForm->keywords) {
			for (std::uint32_t i = 0; i < keywordForm->numKeywords; i++) {
				auto keyword = keywordForm->keywords[i];
				if (keyword) {
					if (keyword == a_add) {
						return;
					}
					if (keyword == a_remove) {
						removeIndex = i;
						found = true;
					}
				}
			}
			if (found) {
				keywordForm->keywords[removeIndex] = a_add;
			}
		}
	}
}


bool papyrusForm::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("papyrusForm - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("AddKeywordToForm", "PO3_SKSEFunctions", AddKeywordToForm);

	a_vm->RegisterFunction("IsGeneratedForm", "PO3_SKSEFunctions", IsGeneratedForm, true);

	a_vm->RegisterFunction("RemoveKeywordOnForm", "PO3_SKSEFunctions", RemoveKeywordOnForm);

	a_vm->RegisterFunction("ReplaceKeywordOnForm", "PO3_SKSEFunctions", ReplaceKeywordOnForm);

	return true;
}
