#pragma once

namespace Papyrus::Faction
{
	RE::TESObjectREFR* GetVendorFactionContainer(STATIC_ARGS, const RE::TESFaction* a_faction);
	std::vector<RE::Actor*> GetAllActorsInFaction(STATIC_ARGS, const RE::TESFaction* a_faction); // SeaSparrow

	void Bind(VM& a_vm);
}
