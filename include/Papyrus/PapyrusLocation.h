#pragma once


namespace papyrusLocation
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;

	RE::BGSLocation* GetParentLocation(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSLocation* a_location);

	void SetParentLocation(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSLocation* a_location, RE::BGSLocation* a_newLocation);


	bool RegisterFuncs(VM* a_vm);
}