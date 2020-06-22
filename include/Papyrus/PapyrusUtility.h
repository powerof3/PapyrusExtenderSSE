#pragma once

#include "main.h"


namespace papyrusUtility
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;


	float GenerateRandomFloat(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, float a_min, float a_max);

	UInt32 GenerateRandomInt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, UInt32 a_min, UInt32 a_max);


	bool RegisterFuncs(VM* a_vm);
}
