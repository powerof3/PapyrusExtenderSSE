#pragma once

namespace INV
{
	bool               can_be_taken(const RE::InventoryEntryData* a_entry, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem);
	RE::ExtraPoison*   get_equipped_weapon_poison_data(const RE::Actor* a_actor, bool a_leftHand);
	RE::ExtraDataList* get_hotkey_extralist(RE::InventoryEntryData* a_changes);
	void               remove_item(RE::TESObjectREFR* a_ref, RE::TESBoundObject* a_item, std::uint32_t a_count, bool a_silent, RE::TESObjectREFR* a_otherContainer, StackID a_stackID, VM* a_vm);

	template <class T, class Filter>
	void collect_items(RE::TESObjectREFR* a_ref, T& a_result, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem, Filter&& a_filter)
	{
		const auto inv = a_ref->GetInventory();
		for (const auto& [item, data] : inv) {
			if (!item || item->Is(RE::FormType::LeveledItem)) {
				continue;
			}

			const auto& [count, entry] = data;

			if (count > 0 && a_filter(item, entry.get()) && can_be_taken(entry.get(), a_noEquipped, a_noFavourited, a_noQuestItem)) {
				if constexpr (std::is_same_v<T, RE::BGSListForm*>) {
					a_result->AddForm(item);
				} else if constexpr (std::is_same_v<T, std::vector<std::pair<RE::TESBoundObject*, std::int32_t>>>) {
					a_result.emplace_back(item, count);
				} else {
					a_result.emplace_back(item);
				}
			}
		}
	}

	template <class T = RE::TESForm*, class Filter>
	std::vector<T> collect_items_array(RE::TESObjectREFR* a_ref, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem, Filter&& a_filter)
	{
		std::vector<T> result{};
		collect_items(a_ref, result, a_noEquipped, a_noFavourited, a_noQuestItem, std::forward<Filter>(a_filter));
		return result;
	}
}
