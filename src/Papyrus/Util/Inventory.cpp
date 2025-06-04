#include "Papyrus/Util/Inventory.h"

namespace INV
{
	bool can_be_taken(const RE::InventoryEntryData* a_entry, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem)
	{
		if (!a_entry) {
			return false;
		}
		if (a_noEquipped && a_entry->IsWorn()) {
			return false;
		}
		if (a_noFavourited && a_entry->IsFavorited()) {
			return false;
		}
		if (a_noQuestItem && a_entry->IsQuestObject()) {
			return false;
		}
		return true;
	}

	RE::ExtraPoison* get_equipped_weapon_poison_data(const RE::Actor* a_actor, bool a_leftHand)
	{
		if (const auto equippedEntryData = a_actor->GetEquippedEntryData(a_leftHand); equippedEntryData) {
			if (equippedEntryData->extraLists) {
				for (const auto& xList : *equippedEntryData->extraLists) {
					if (xList) {
						if (const auto xPoison = xList->GetByType<RE::ExtraPoison>(); xPoison) {
							return xPoison;
						}
					}
				}
			}
		}

		return nullptr;
	}

	RE::ExtraDataList* get_hotkey_extralist(RE::InventoryEntryData* a_changes)
	{
		if (a_changes->extraLists) {
			for (const auto& xList : *a_changes->extraLists) {
				if (const auto hotkey = xList->HasType<RE::ExtraHotkey>()) {
					return xList;
				}
			}
		}
		return nullptr;
	}

	void remove_item(RE::TESObjectREFR* a_ref, RE::TESBoundObject* a_item, std::uint32_t a_count, bool a_silent, RE::TESObjectREFR* a_otherContainer, StackID a_stackID, VM* a_vm)
	{
		using func_t = decltype(&remove_item);
		static REL::Relocation<func_t> func{ RELOCATION_ID(56261, 56647) };
		return func(a_ref, a_item, a_count, a_silent, a_otherContainer, a_stackID, a_vm);
	}
}
