#pragma once


namespace papyrusStrings
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;


	RE::BSFixedString IntToString(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, std::uint32_t a_int, bool a_hex);

	std::int32_t StringToInt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_string);


	bool RegisterFuncs(VM* a_vm);
}