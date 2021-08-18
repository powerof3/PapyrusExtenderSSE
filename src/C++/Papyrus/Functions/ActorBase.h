#pragma once

namespace Papyrus::ActorBase
{
	inline RE::TESLevItem* GetDeathItem(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESNPC* a_actorbase)
	{
		if (!a_actorbase) {
			a_vm->TraceStack("ActorBase is None", a_stackID);
			return nullptr;
		}

		return a_actorbase->deathItem;
	}

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

	inline void SetDeathItem(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESNPC* a_actorbase,
		RE::TESLevItem* a_item)
	{
		if (!a_actorbase) {
			a_vm->TraceStack("ActorBase is None", a_stackID);
			return;
		}

		a_actorbase->deathItem = static_cast<RE::TESLevItem*>(a_item);
	}

	inline void Bind(VM& a_vm)
	{
		BIND(GetDeathItem);
		BIND(GetPerkCount);
		BIND(GetNthPerk);
		BIND(SetDeathItem);

		logger::info("Registered actorbase functions"sv);
	}
}
