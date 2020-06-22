#pragma once

#include "main.h"


namespace papyrusStrings
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;


	RE::BSFixedString IntToString(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, UInt32 a_int, bool a_hex);

	SInt32 StringToInt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_string);


	bool RegisterFuncs(VM* a_vm);
}