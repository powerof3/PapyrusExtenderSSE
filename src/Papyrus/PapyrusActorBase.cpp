#include "Papyrus/PapyrusActorBase.h"


UInt32 papyrusActorBase::GetPerkCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESNPC* a_actorbase)
{
	return a_actorbase ? a_actorbase->perkCount : 0;
}


RE::BGSPerk* papyrusActorBase::GetNthPerk(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESNPC* a_actorbase, UInt32 a_index)
{
	if (a_actorbase) {
		if (a_index >= a_actorbase->perkCount) {
			a_vm->TraceStack("Index is out of bounds", a_stackID, Severity::kWarning);
			return nullptr;
		}
		return a_actorbase->perks[a_index].perk;
	}	
	return nullptr;
}


bool papyrusActorBase::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		_MESSAGE("papyrusActorBase - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("GetPerkCount", "PO3_SKSEFunctions", GetPerkCount);

	a_vm->RegisterFunction("GetNthPerk", "PO3_SKSEFunctions", GetNthPerk);

	return true;
}