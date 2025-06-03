#pragma once

namespace Papyrus::ActorBase
{
	RE::BGSAssociationType*  GetAssociationType(STATIC_ARGS, RE::TESNPC* a_npc1, RE::TESNPC* a_npc2);
	RE::TESLevItem*          GetDeathItem(STATIC_ARGS, RE::TESNPC* a_actorbase);
	RE::BGSPerk*             GetNthPerk(STATIC_ARGS, RE::TESNPC* a_actorbase, std::uint32_t a_index);
	std::uint32_t            GetPerkCount(STATIC_ARGS, const RE::TESNPC* a_actorbase);
	std::vector<RE::TESNPC*> GetRelationships(STATIC_ARGS, RE::TESNPC* a_actorbase, RE::BGSAssociationType* a_assocType);
	void                     SetDeathItem(STATIC_ARGS, RE::TESNPC* a_actorbase, RE::TESLevItem* a_item);

	void Bind(VM& a_vm);
}
