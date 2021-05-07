#pragma once


namespace papyrusBook
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;


	void ClearReadFlag(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectBOOK* a_book);

	void SetReadFlag(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectBOOK* a_book);


	bool RegisterFuncs(VM* a_vm);
}