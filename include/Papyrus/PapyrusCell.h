#pragma once


namespace papyrusCell
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;

	RE::BGSLightingTemplate* GetLightingTemplate(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectCELL* a_cell);

	void SetLightingTemplate(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectCELL* a_cell, RE::BGSLightingTemplate* a_lightingTemplate);

	bool RegisterFuncs(VM* a_vm);
}
