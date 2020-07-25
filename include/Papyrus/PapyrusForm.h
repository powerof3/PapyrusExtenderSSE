#pragma once


namespace papyrusForm
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;


	void AddKeywordToForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form, RE::BGSKeyword* a_add);

	bool IsGeneratedForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form);

	bool RemoveKeywordOnForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form, RE::BGSKeyword* a_remove);

	void ReplaceKeywordOnForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form, RE::BGSKeyword* a_remove, RE::BGSKeyword* a_add);


	bool RegisterFuncs(VM* a_vm);
}