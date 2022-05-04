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

	inline RE::TESFaction* GetVendorFactionOwner(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESObjectREFR* a_refr)
	{
		if (!a_refr) {
			a_vm->TraceStack("Container is None", a_stackID);
			return nullptr;
		}

		if (const auto owner = a_refr->GetOwner()) {
			if (const auto factionOwner = owner->As<RE::TESFaction>(); factionOwner->IsVendor()) {
				return factionOwner;
			}
		}

		return nullptr;
	}

	inline void Bind(VM& a_vm)
	{
		BIND(GetVendorFactionContainer, true);
		BIND(GetVendorFactionOwner, true);

		logger::info("Registered faction functions"sv);
	}
}
