#pragma once

namespace Papyrus::ActorBase
{
	inline std::uint32_t GetPerkCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESNPC* a_actorbase)
	{
		if (!a_actorbase) {
			a_vm->TraceStack("ActorBase is None", a_stackID);
			return 0;
		}

		return a_actorbase->perkCount;
	}

	inline RE::BGSPerk* GetNthPerk(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		RE::TESNPC* a_actorbase, 
		std::uint32_t a_index)
	{
		if (!a_actorbase) {
			a_vm->TraceStack("ActorBase is None", a_stackID);
			return 0;
		}

		if (a_index >= a_actorbase->perkCount) {
			a_vm->TraceForm(a_actorbase, "Invalid perk index", a_stackID);
			return 0;
		}

		return a_actorbase->perks[a_index].perk;
	}

	inline void Register(VM& a_vm)
	{
		BIND(GetPerkCount);
		BIND(GetNthPerk);

		logger::info("Registered actorbase functions"sv);
	}
}
