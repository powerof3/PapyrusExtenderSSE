#pragma once


namespace papyrusUtility
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;
	using RNG = SKSE::RNG;


	float GenerateRandomFloat(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, float a_min, float a_max);

	std::uint32_t GenerateRandomInt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, std::uint32_t a_min, std::uint32_t a_max);


	bool RegisterFuncs(VM* a_vm);
}
