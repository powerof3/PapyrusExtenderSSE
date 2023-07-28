#pragma once

namespace Papyrus::ActorBase
{
	inline RE::BGSAssociationType* GetAssociationType(STATIC_ARGS, RE::TESNPC* a_npc1, RE::TESNPC* a_npc2)
	{
		if (!a_npc1) {
			a_vm->TraceStack("ActorBase is None", a_stackID);
			return nullptr;
		}
		if (!a_npc2) {
			a_vm->TraceStack("Other ActorBase is None", a_stackID);
			return nullptr;
		}

		const auto relationship = RE::BGSRelationship::GetRelationship(a_npc1, a_npc2);
		return relationship ? relationship->assocType : nullptr;
	}

	inline RE::TESLevItem* GetDeathItem(STATIC_ARGS, RE::TESNPC* a_actorbase)
	{
		if (!a_actorbase) {
			a_vm->TraceStack("ActorBase is None", a_stackID);
			return nullptr;
		}

		return a_actorbase->deathItem;
	}

	inline RE::BGSPerk* GetNthPerk(STATIC_ARGS, RE::TESNPC* a_actorbase, std::uint32_t a_index)
	{
		if (!a_actorbase) {
			a_vm->TraceStack("ActorBase is None", a_stackID);
			return nullptr;
		}

		if (a_index >= a_actorbase->perkCount) {
			a_vm->TraceForm(a_actorbase, "Invalid perk index", a_stackID);
			return nullptr;
		}

		return a_actorbase->perks[a_index].perk;
	}

	inline std::uint32_t GetPerkCount(STATIC_ARGS, const RE::TESNPC* a_actorbase)
	{
		if (!a_actorbase) {
			a_vm->TraceStack("ActorBase is None", a_stackID);
			return 0;
		}

		return a_actorbase->perkCount;
	}

	inline std::vector<RE::TESNPC*> GetRelationships(STATIC_ARGS, RE::TESNPC* a_actorbase, RE::BGSAssociationType* a_assocType)
	{
		if (!a_actorbase) {
			a_vm->TraceStack("ActorBase is None", a_stackID);
			return {};
		}

		std::vector<RE::TESNPC*> actorbases;
		if (a_actorbase->relationships) {
			for (const auto& relationship : *a_actorbase->relationships) {
				if (relationship && (!a_assocType || relationship->assocType == a_assocType)) {
					auto parentNPC = relationship->npc1;
					if (parentNPC == a_actorbase) {
						parentNPC = relationship->npc2;
					}
					actorbases.push_back(parentNPC);
				}
			}
		}
		return actorbases;
	}

	inline void SetDeathItem(STATIC_ARGS, RE::TESNPC* a_actorbase, RE::TESLevItem* a_item)
	{
		if (!a_actorbase) {
			a_vm->TraceStack("ActorBase is None", a_stackID);
			return;
		}

		a_actorbase->deathItem = a_item;
	}

	inline void Bind(VM& a_vm)
	{
		BIND(GetAssociationType);
		BIND(GetDeathItem);
		BIND(GetNthPerk);
		BIND(GetRelationships);
		BIND(GetPerkCount);
		BIND(SetDeathItem);

		logger::info("Registered actorbase functions"sv);
	}
}
