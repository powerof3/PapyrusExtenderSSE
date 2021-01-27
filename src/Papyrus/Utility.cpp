#include "Papyrus/Utility.h"


auto papyrusUtility::GenerateRandomFloat(VM*, StackID, RE::StaticFunctionTag*, float a_min, float a_max) -> float
{
	return RNG::GetSingleton()->Generate<float>(a_min, a_max);
}


auto papyrusUtility::GenerateRandomInt(VM*, StackID, RE::StaticFunctionTag*, std::uint32_t a_min, std::uint32_t a_max) -> std::uint32_t
{
	return RNG::GetSingleton()->Generate<std::uint32_t>(a_min, a_max);
}


auto papyrusUtility::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusUtility - couldn't get VMState"sv);
		return false;
	}

	auto constexpr Functions = "PO3_SKSEFunctions"sv;

	a_vm->RegisterFunction("GenerateRandomFloat"sv, Functions, GenerateRandomFloat, true);

	a_vm->RegisterFunction("GenerateRandomInt"sv, Functions, GenerateRandomInt, true);

	return true;
}
