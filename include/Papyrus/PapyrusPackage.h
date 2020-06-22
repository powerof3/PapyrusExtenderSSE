#pragma once

#include "main.h"


namespace papyrusPackage
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;

	SInt32 GetPackageType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESPackage* a_package);


	bool RegisterFuncs(VM* a_vm);
}
