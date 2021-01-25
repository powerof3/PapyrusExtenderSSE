#include "Papyrus/ActorBase.h"


std::uint32_t papyrusActorBase::GetPerkCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESNPC* a_actorbase)
{
	if (!a_actorbase) {
		a_vm->TraceStack("Actorbase is None", a_stackID, Severity::kWarning);
		return 0;
	}

	return a_actorbase->perkCount;
}


RE::BGSPerk* papyrusActorBase::GetNthPerk(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESNPC* a_actorbase, std::uint32_t a_index)
{
	if (!a_actorbase) {
		a_vm->TraceStack("Actorbase is None", a_stackID, Severity::kWarning);
		return nullptr;
	} else if (a_index >= a_actorbase->perkCount) {
		a_vm->TraceStack("Index is out of bounds", a_stackID, Severity::kWarning);
		return nullptr;
	}

	return a_actorbase->perks[a_index].perk;
}


bool papyrusActorBase::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("papyrusActorBase - couldn't get VMState"sv);
		return false;
	}

	a_vm->RegisterFunction("GetPerkCount", "PO3_SKSEFunctions", GetPerkCount);

	a_vm->RegisterFunction("GetNthPerk", "PO3_SKSEFunctions", GetNthPerk);

	return true;
}