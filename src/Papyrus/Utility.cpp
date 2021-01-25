#include "Papyrus/Utility.h"


float papyrusUtility::GenerateRandomFloat(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, float a_min, float a_max)
{
	auto RNG = SKSE::RNG::GetSingleton();
	return RNG->GenerateRandomNumber<float>(a_min, a_max);
}


std::uint32_t papyrusUtility::GenerateRandomInt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, std::uint32_t a_min, std::uint32_t a_max)
{
	auto RNG = SKSE::RNG::GetSingleton();
	return RNG->GenerateRandomNumber<std::uint32_t>(a_min, a_max);
}


bool papyrusUtility::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("papyrusUtility - couldn't get VMState"sv);
		return false;
	}

	a_vm->RegisterFunction("GenerateRandomFloat", "PO3_SKSEFunctions", GenerateRandomFloat, true);

	a_vm->RegisterFunction("GenerateRandomInt", "PO3_SKSEFunctions", GenerateRandomInt, true);

	return true;
}
