#include "Papyrus/PapyrusUtility.h"


float papyrusUtility::GenerateRandomFloat(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, float a_min, float a_max)
{
    auto RNG = SKSE::RNG::GetSingleton();
    if (RNG) {
        return RNG->GenerateRandomFloat(a_min, a_max);
    }
    return a_min;
}


UInt32 papyrusUtility::GenerateRandomInt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, UInt32 a_min, UInt32 a_max)
{
    auto RNG = SKSE::RNG::GetSingleton();
    if (RNG) {
        return RNG->GenerateRandomInt(a_min, a_max);
    }
    return a_min;
}


bool papyrusUtility::RegisterFuncs(VM* a_vm)
{
    if (!a_vm) {
        _MESSAGE("papyrusUtility - couldn't get VMState");
        return false;
    }
    
    a_vm->RegisterFunction("GenerateRandomFloat", "PO3_SKSEFunctions", GenerateRandomFloat, true);

    a_vm->RegisterFunction("GenerateRandomInt", "PO3_SKSEFunctions", GenerateRandomInt, true);

    return true;
}
