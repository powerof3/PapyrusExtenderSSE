#include "Papyrus/PapyrusForm.h"

#include "Serialization/Serialize.h"


void papyrusForm::AddKeywordToForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form, RE::BGSKeyword* a_add)
{
	if (a_form) {
		if (!a_add) {
			a_vm->TraceStack("Cannot add a None keyword", a_stackID, Severity::kWarning);
			return;
		}
		auto keywords = Serialize::Keywords::GetSingleton();
		if (keywords) {
			keywords->PapyrusApplyKeywords(a_form, a_add, Serialize::Base::kAdd);
		}
	}
}


bool papyrusForm::RemoveKeywordOnForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form, RE::BGSKeyword* a_remove)
{
	if (a_form) {
		if (!a_remove) {
			a_vm->TraceStack("Cannot remove a None keyword", a_stackID, Severity::kWarning);
			return false;
		}
		auto keywords = Serialize::Keywords::GetSingleton();
		if (keywords) {
			return keywords->PapyrusApplyKeywords(a_form, a_remove, Serialize::Base::kRemove);
		}
	}
	return false;
}


void papyrusForm::ReplaceKeywordOnForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form, RE::BGSKeyword* a_remove, RE::BGSKeyword* a_add)
{
	if (a_form) {
		if (!a_remove) {
			a_vm->TraceStack("Remove keyword is None", a_stackID, Severity::kWarning);
			return;
		}
		if (!a_add) {
			a_vm->TraceStack("Add keyword is None", a_stackID, Severity::kWarning);
			return;
		}
		auto keywordForm = a_form->As<RE::BGSKeywordForm>();
		if (keywordForm) {
			UInt32 removeIndex = 0;
			bool found = false;
			if (keywordForm->keywords) {
				for (UInt32 i = 0; i < keywordForm->numKeywords; i++) {
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
	else {
		a_vm->TraceStack("Cannot replace keywords on a None form", a_stackID, Severity::kWarning);
	}
}


bool papyrusForm::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		_MESSAGE("papyrusForm - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("AddKeywordToForm", "PO3_SKSEFunctions", AddKeywordToForm);

	a_vm->RegisterFunction("RemoveKeywordOnForm", "PO3_SKSEFunctions", RemoveKeywordOnForm);

	a_vm->RegisterFunction("ReplaceKeywordOnForm", "PO3_SKSEFunctions", ReplaceKeywordOnForm);

	return true;
}
