#pragma once


namespace papyrusArmorAddon
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;


	RE::BGSFootstepSet* GetFootstepSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectARMA* a_arma);

	void SetFootstepSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectARMA* a_arma, RE::BGSFootstepSet* a_footstepSet);


	bool RegisterFuncs(VM* a_vm);
}