#pragma once

namespace Papyrus::Faction
{
	inline RE::TESObjectREFR* GetVendorFactionContainer(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESFaction* a_faction)
	{
		if (!a_faction) {
			a_vm->TraceStack("Faction is None", a_stackID);
			return nullptr;
		}

		return a_faction->vendorData.merchantContainer;
	}

	inline void Bind(VM& a_vm)
	{
		BIND(GetVendorFactionContainer, true);

		logger::info("Registered faction functions"sv);
	}
}
