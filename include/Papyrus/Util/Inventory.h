#pragma once

namespace INV
{
	bool can_be_taken(const RE::InventoryEntryData* a_entry, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem);

	RE::ExtraPoison* get_equipped_weapon_poison_data(const RE::Actor* a_actor, bool a_leftHand);

	RE::ExtraDataList* get_hotkey_extralist(RE::InventoryEntryData* a_changes);

	void remove_item(RE::TESObjectREFR* a_ref, RE::TESBoundObject* a_item, std::uint32_t a_count, bool a_silent, RE::TESObjectREFR* a_otherContainer, Papyrus::StackID a_stackID, Papyrus::VM* a_vm);
}
