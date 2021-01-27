#include "Papyrus/ActorBase.h"


auto papyrusActorBase::GetPerkCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESNPC* a_actorbase) -> std::uint32_t
{
	if (!a_actorbase) {
		a_vm->TraceStack("Actorbase is None", a_stackID, Severity::kWarning);
		return 0;
	}

	return a_actorbase->perkCount;
}


auto papyrusActorBase::GetNthPerk(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESNPC* a_actorbase, std::uint32_t a_index) -> RE::BGSPerk*
{
	if (!a_actorbase) {
		a_vm->TraceStack("Actorbase is None", a_stackID, Severity::kWarning);
		return nullptr;
	}
    if (a_index >= a_actorbase->perkCount) {
        a_vm->TraceStack("Index is out of bounds", a_stackID, Severity::kWarning);
        return nullptr;
    }

    return a_actorbase->perks[a_index].perk;
}


auto papyrusActorBase::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusActorBase - couldn't get VMState"sv);
		return false;
	}

	auto constexpr Functions = "PO3_SKSEFunctions"sv;

	a_vm->RegisterFunction("GetPerkCount"sv, Functions, GetPerkCount);

	a_vm->RegisterFunction("GetNthPerk"sv, Functions, GetNthPerk);

	return true;
}