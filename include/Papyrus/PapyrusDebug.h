#pragma once


namespace papyrusDebug
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;

	void GivePlayerSpellBook(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*);

	bool RegisterFuncs(VM* a_vm);
}