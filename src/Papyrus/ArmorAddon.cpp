#include "Papyrus\ArmorAddon.h"


RE::BGSFootstepSet* papyrusArmorAddon::GetFootstepSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectARMA* a_arma)
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


bool papyrusArmorAddon::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("papyrusArmorAddon - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("GetFootstepSet", "PO3_SKSEFunctions", GetFootstepSet);
	a_vm->RegisterFunction("SetFootstepSet", "PO3_SKSEFunctions", SetFootstepSet);

	return true;
}
