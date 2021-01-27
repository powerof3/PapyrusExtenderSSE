#include "Papyrus/ArmorAddon.h"


auto papyrusArmorAddon::GetFootstepSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectARMA* a_arma) -> RE::BGSFootstepSet*
{
	if (!a_arma) {
		a_vm->TraceStack("Armor Addon is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	return a_arma->footstepSet;
}


void papyrusArmorAddon::SetFootstepSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectARMA* a_arma, RE::BGSFootstepSet* a_footstepSet)
{
	if (!a_arma) {
		a_vm->TraceStack("Armor Addon is None", a_stackID, Severity::kWarning);
		return;
	}

	a_arma->footstepSet = a_footstepSet;
}


auto papyrusArmorAddon::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusArmorAddon - couldn't get VMState"sv);
		return false;
	}

	auto constexpr Functions = "PO3_SKSEFunctions"sv;

	a_vm->RegisterFunction("GetFootstepSet"sv, Functions, GetFootstepSet);

    a_vm->RegisterFunction("SetFootstepSet"sv, Functions, SetFootstepSet);

	return true;
}
