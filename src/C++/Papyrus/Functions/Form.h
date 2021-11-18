#pragma once

#include "Serialization/Events.h"
#include "Serialization/Services.h"
#include "Game/Cache.h"

using namespace Events::Script;
using namespace Events::Story;
using namespace Events::Game;
using namespace Events::FEC;

namespace Papyrus::Form
{
	inline void AddKeywordToForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESForm* a_form,
		RE::BGSKeyword* a_keyword)
	{
		using namespace Form;

		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}
		if (!a_keyword) {
			a_vm->TraceStack("Keyword is None", a_stackID);
			return;
		}

		FORM::KeywordManager::GetSingleton()->Add(a_form, a_keyword);
	}

	inline void ClearRecordFlag(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESForm* a_form,
		std::uint32_t a_flag)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		a_form->formFlags &= ~a_flag;
	}

	inline bool EvaluateConditionList(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESForm* a_form,
		RE::TESObjectREFR* a_actionRef,
		RE::TESObjectREFR* a_target)
	{
		bool result = false;

		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return result;
		}
		if (!a_actionRef) {
			a_vm->TraceStack("Source is None", a_stackID);
			return result;
		}
		if (!a_target) {
			a_vm->TraceStack("Target is None", a_stackID);
			return result;
		}

		switch (a_form->GetFormType()) {
		case RE::FormType::Spell:
		case RE::FormType::Enchantment:
		case RE::FormType::Ingredient:
		case RE::FormType::AlchemyItem:
		case RE::FormType::Scroll:
			{
				if (const auto magicItem = a_form->As<RE::MagicItem>(); magicItem) {
					for (const auto& effect : magicItem->effects) {
						if (result) {
							break;
						}

						if (effect) {
							const bool effectValid = effect->conditions.IsTrue(a_actionRef, a_target);

							const auto baseEffect = effect->baseEffect;
							const bool mgefValid = baseEffect && baseEffect->conditions.IsTrue(a_actionRef, a_target);

							if (effectValid && mgefValid) {
								result = true;
							}
						}
					}
				}
			}
			break;
		case RE::FormType::MagicEffect:
			{
				const auto effect = a_form->As<RE::EffectSetting>();
				if (effect && effect->conditions.IsTrue(a_actionRef, a_target)) {
					result = true;
				}
			}
			break;
		case RE::FormType::Info:
			{
				const auto topic = a_form->As<RE::TESTopicInfo>();
				if (topic && topic->objConditions.IsTrue(a_actionRef, a_target)) {
					result = true;
				}
			}
			break;
		case RE::FormType::Package:
			{
				const auto package = a_form->As<RE::TESPackage>();
				if (package && package->packConditions.IsTrue(a_actionRef, a_target)) {
					result = true;
				}
			}
			break;
		default:
			result = true;
			break;
		}

		return result;
	}

	inline std::vector<std::string> GetConditionList(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESForm* a_form,
		std::uint32_t a_index)
	{
		std::vector<std::string> result;

		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return result;
		}
		auto condition = CONDITION::GetCondition(*a_form, a_index);
		if (!condition) {
			a_vm->TraceStack("Form does not have a condition stack", a_stackID);
			return result;
		}

		return CONDITION::BuildConditions(condition);
	}

	inline RE::BSFixedString GetFormEditorID(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return RE::BSFixedString();
		}

		return Cache::EditorID::GetSingleton()->GetEditorID(a_form->GetFormID());
	}

	inline bool IsFormInMod(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form, RE::BSFixedString a_modName)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return false;
		}

		const auto dataHandler = RE::TESDataHandler::GetSingleton();
		const auto modInfo = dataHandler ? dataHandler->LookupModByName(a_modName) : nullptr;

		return modInfo ? modInfo->IsFormInMod(a_form->GetFormID()) : false;
	}

	inline bool IsGeneratedForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return false;
		}

		return a_form->IsDynamicForm();
	}

	namespace fave_util
	{
		namespace item
		{
			inline void favorite(RE::InventoryChanges* a_changes, RE::InventoryEntryData* a_entryData, RE::ExtraDataList* a_list)
			{
				using func_t = decltype(&favorite);
				REL::Relocation<func_t> func{ REL::ID(15858) };
				return func(a_changes, a_entryData, a_list);
			}

			inline void unfavorite(RE::InventoryChanges* a_changes, RE::InventoryEntryData* a_entryData, RE::ExtraDataList* a_list)
			{
				using func_t = decltype(&unfavorite);
				REL::Relocation<func_t> func{ REL::ID(15859) };
				return func(a_changes, a_entryData, a_list);
			}

			inline RE::ExtraDataList* get_hotkeyed(RE::InventoryEntryData* a_changes)
			{
				if (a_changes->extraLists) {
					for (const auto& xList : *a_changes->extraLists) {
						const auto hotkey = xList->HasType<RE::ExtraHotkey>();
						if (hotkey) {
							return xList;
						}
					}
				}
				return nullptr;
			}
		}

		namespace magic
		{
			inline void favorite(RE::MagicFavorites* a_magicFavorites, RE::TESForm* a_form)
			{
				using func_t = decltype(&favorite);
				REL::Relocation<func_t> func{ REL::ID(51121) };
				return func(a_magicFavorites, a_form);
			}

			inline void unfavorite(RE::MagicFavorites* a_magicFavorites, RE::TESForm* a_form)
			{
				using func_t = decltype(&unfavorite);
				REL::Relocation<func_t> func{ REL::ID(51122) };
				return func(a_magicFavorites, a_form);
			}
		}
	}

	inline void MarkItemAsFavorite(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		using namespace fave_util;

		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		if (a_form->Is(RE::FormType::Spell, RE::FormType::Shout)) {
			const auto magicFavorites = RE::MagicFavorites::GetSingleton();
			if (magicFavorites && std::ranges::find(magicFavorites->spells, a_form) == magicFavorites->spells.end()) {
				magic::favorite(magicFavorites, a_form);
			}
		} else {
			const auto player = RE::PlayerCharacter::GetSingleton();
			const auto xContainer = player ? player->extraList.GetByType<RE::ExtraContainerChanges>() : nullptr;
			const auto invChanges = xContainer ? xContainer->changes : nullptr;

			if (invChanges) {
				auto inv = player->GetInventory();
				for (const auto& [item, data] : inv) {
					const auto& [count, entry] = data;
					if (count > 0 && item == a_form && !item::get_hotkeyed(entry.get())) {
						const auto extralist = entry->extraLists ? entry->extraLists->front() : nullptr;
						item::favorite(invChanges, entry.get(), extralist);
						break;
					}
				}
			}
		}
	}

	inline bool IsRecordFlagSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::TESForm* a_form,
		std::uint32_t a_flag)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return false;
		}

		return (a_form->formFlags & a_flag) != 0;
	}

	inline bool IsScriptAttachedToForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::TESForm* a_form,
		RE::BSFixedString a_scriptName)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return false;
		}

		return SCRIPT::is_script_attached(a_form, a_scriptName);
	}

	inline bool RemoveKeywordOnForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESForm* a_form,
		RE::BGSKeyword* a_keyword)
	{
		using namespace Form;

		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return false;
		}
		if (!a_keyword) {
			a_vm->TraceStack("Keyword is None", a_stackID);
			return false;
		}

		return FORM::KeywordManager::GetSingleton()->Remove(a_form, a_keyword);
	}

	inline void ReplaceKeywordOnForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESForm* a_form,
		const RE::BGSKeyword* a_remove,
		RE::BGSKeyword* a_add)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}
		if (!a_remove) {
			a_vm->TraceStack("Remove keyword is None", a_stackID);
			return;
		}
		if (!a_add) {
			a_vm->TraceStack("Add keyword is None", a_stackID);
			return;
		}

		if (const auto keywordForm = a_form->As<RE::BGSKeywordForm>(); keywordForm) {
			if (keywordForm->keywords) {
				bool found = false;
				std::uint32_t removeIndex = 0;
				for (std::uint32_t i = 0; i < keywordForm->numKeywords; i++) {
					const auto keyword = keywordForm->keywords[i];
					if (keyword) {
						if (keyword == a_add) {
							return;
						}
						if (keyword == a_remove) {
							removeIndex = i;
							found = true;
							break;
						}
					}
				}
				if (found) {
					keywordForm->keywords[removeIndex] = a_add;
				}
			}
		}
	}

	inline void SetRecordFlag(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESForm* a_form,
		std::uint32_t a_flag)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		a_form->formFlags |= a_flag;
	}

	inline void UnmarkItemAsFavorite(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		using namespace fave_util;

		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		if (a_form->Is(RE::FormType::Spell, RE::FormType::Shout)) {
			const auto magicFavorites = RE::MagicFavorites::GetSingleton();
			if (magicFavorites && std::ranges::find(magicFavorites->spells, a_form) != magicFavorites->spells.end()) {
				magic::unfavorite(magicFavorites, a_form);
			}
		} else {
			const auto player = RE::PlayerCharacter::GetSingleton();
			const auto xContainer = player ? player->extraList.GetByType<RE::ExtraContainerChanges>() : nullptr;
			const auto invChanges = xContainer ? xContainer->changes : nullptr;

			if (invChanges) {
				auto inv = player->GetInventory();
				for (const auto& [item, data] : inv) {
					const auto& [count, entry] = data;
					if (count > 0 && item == a_form) {
						const auto extralist = item::get_hotkeyed(entry.get());
						if (extralist) {
							item::unfavorite(invChanges, entry.get(), extralist);
						}
						break;
					}
				}
			}
		}
	}

	inline void RegisterForActorKilled(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnActorKillRegSet::GetSingleton();
		regs->Register(a_form);
	}

	inline void RegisterForFECReset(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form, std::uint32_t a_type)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnFECResetRegMap::GetSingleton();
		regs->Register(a_form, a_type);
	}

	inline void RegisterForBookRead(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnBooksReadRegSet::GetSingleton();
		regs->Register(a_form);
	}

	inline void RegisterForCellFullyLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnCellFullyLoadedRegSet::GetSingleton();
		regs->Register(a_form);
	}

	inline void RegisterForCriticalHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnCriticalHitRegSet::GetSingleton();
		regs->Register(a_form);
	}

	inline void RegisterForDisarmed(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnDisarmedRegSet::GetSingleton();
		regs->Register(a_form);
	}

	inline void RegisterForDragonSoulGained(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnDragonSoulsGainedRegSet::GetSingleton();
		regs->Register(a_form);
	}

	inline void RegisterForItemHarvested(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnItemHarvestedRegSet::GetSingleton();
		regs->Register(a_form);
	}

	inline void RegisterForLevelIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnLevelIncreaseRegSet::GetSingleton();
		regs->Register(a_form);
	}

	inline void RegisterForLocationDiscovery(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnLocationDiscoveryRegSet::GetSingleton();
		regs->Register(a_form);
	}

	inline void RegisterForObjectGrab(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto grab = OnGrabRegSet::GetSingleton();
		grab->Register(a_form);

		auto release = OnReleaseRegSet::GetSingleton();
		release->Register(a_form);
	}

	inline void RegisterForObjectLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::TESForm* a_form,
		std::uint32_t a_formType)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto formType = static_cast<RE::FormType>(a_formType);

		auto load = OnObjectLoadedRegMap::GetSingleton();
		load->Register(a_form, formType);

		auto unload = OnObjectUnloadedRegMap::GetSingleton();
		unload->Register(a_form, formType);
	}

	inline void RegisterForQuest(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::TESForm* a_form,
		const RE::TESQuest* a_quest)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}
		if (!a_quest) {
			a_vm->TraceStack("Quest is None", a_stackID);
			return;
		}

		auto start = OnQuestStartRegMap::GetSingleton();
		start->Register(a_form, a_quest->GetFormID());

		auto stop = OnQuestStopRegMap::GetSingleton();
		stop->Register(a_form, a_quest->GetFormID());
	}

	inline void RegisterForQuestStage(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::TESForm* a_form,
		const RE::TESQuest* a_quest)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}
		if (!a_quest) {
			a_vm->TraceStack("Quest is None", a_stackID);
			return;
		}

		const auto regs = OnQuestStageRegMap::GetSingleton();
		regs->Register(a_form, a_quest->GetFormID());
	}

	inline void RegisterForShoutAttack(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnShoutAttackRegSet::GetSingleton();
		regs->Register(a_form);
	}

	inline void RegisterForSkillIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnSkillIncreaseRegSet::GetSingleton();
		regs->Register(a_form);
	}

	inline void RegisterForSoulTrapped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnSoulsTrappedRegSet::GetSingleton();
		regs->Register(a_form);
	}

	inline void RegisterForSpellLearned(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnSpellsLearnedRegSet::GetSingleton();
		regs->Register(a_form);
	}

	inline void RegisterForWeatherChange(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnWeatherChangeRegSet::GetSingleton();
		regs->Register(a_form);
	}

	inline void UnregisterForActorKilled(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnActorKillRegSet::GetSingleton();
		regs->Unregister(a_form);
	}

	inline void UnregisterForFECReset(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::TESForm* a_form,
		std::uint32_t a_type)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnFECResetRegMap::GetSingleton();
		regs->Unregister(a_form, a_type);
	}

	inline void UnregisterForAllFECResets(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnFECResetRegMap::GetSingleton();
		regs->UnregisterAll(a_form);
	}

	inline void UnregisterForBookRead(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnBooksReadRegSet::GetSingleton();
		regs->Unregister(a_form);
	}

	inline void UnregisterForCellFullyLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnCellFullyLoadedRegSet::GetSingleton();
		regs->Unregister(a_form);
	}

	inline void UnregisterForCriticalHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnCriticalHitRegSet::GetSingleton();
		regs->Unregister(a_form);
	}

	inline void UnregisterForDisarmed(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnDisarmedRegSet::GetSingleton();
		regs->Unregister(a_form);
	}

	inline void UnregisterForDragonSoulGained(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnDragonSoulsGainedRegSet::GetSingleton();
		regs->Unregister(a_form);
	}

	inline void UnregisterForItemHarvested(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnItemHarvestedRegSet::GetSingleton();
		regs->Unregister(a_form);
	}

	inline void UnregisterForLevelIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnLevelIncreaseRegSet::GetSingleton();
		regs->Unregister(a_form);
	}

	inline void UnregisterForLocationDiscovery(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnLocationDiscoveryRegSet::GetSingleton();
		regs->Unregister(a_form);
	}

	inline void UnregisterForObjectGrab(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto grab = OnGrabRegSet::GetSingleton();
		grab->Unregister(a_form);

		auto release = OnReleaseRegSet::GetSingleton();
		release->Unregister(a_form);
	}

	inline void UnregisterForObjectLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::TESForm* a_form,
		std::uint32_t a_formType)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto formType = static_cast<RE::FormType>(a_formType);

		auto load = OnObjectLoadedRegMap::GetSingleton();
		load->Unregister(a_form, formType);

		auto unload = OnObjectUnloadedRegMap::GetSingleton();
		unload->Unregister(a_form, formType);
	}

	inline void UnregisterForAllObjectsLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto load = OnObjectLoadedRegMap::GetSingleton();
		load->UnregisterAll(a_form);

		auto unload = OnObjectUnloadedRegMap::GetSingleton();
		unload->UnregisterAll(a_form);
	}

	inline void UnregisterForQuest(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::TESForm* a_form,
		const RE::TESQuest* a_quest)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}
		if (!a_quest) {
			a_vm->TraceStack("Quest is None", a_stackID);
			return;
		}

		auto start = OnQuestStartRegMap::GetSingleton();
		start->Unregister(a_form, a_quest->GetFormID());

		auto stop = OnQuestStartRegMap::GetSingleton();
		stop->Unregister(a_form, a_quest->GetFormID());
	}

	inline void UnregisterForAllQuests(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto start = OnQuestStartRegMap::GetSingleton();
		start->UnregisterAll(a_form);

		auto stop = OnQuestStartRegMap::GetSingleton();
		stop->UnregisterAll(a_form);
	}

	inline void UnregisterForQuestStage(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::TESForm* a_form,
		const RE::TESQuest* a_quest)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}
		if (!a_quest) {
			a_vm->TraceStack("Quest is None", a_stackID);
			return;
		}

		const auto regs = OnQuestStageRegMap::GetSingleton();
		regs->Unregister(a_form, a_quest->GetFormID());
	}

	inline void UnregisterForAllQuestStages(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnQuestStageRegMap::GetSingleton();
		regs->UnregisterAll(a_form);
	}

	inline void UnregisterForShoutAttack(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnShoutAttackRegSet::GetSingleton();
		regs->Unregister(a_form);
	}

	inline void UnregisterForSkillIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnShoutAttackRegSet::GetSingleton();
		regs->Unregister(a_form);
	}

	inline void UnregisterForSoulTrapped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnSoulsTrappedRegSet::GetSingleton();
		regs->Unregister(a_form);
	}

	inline void UnregisterForSpellLearned(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnSpellsLearnedRegSet::GetSingleton();
		regs->Unregister(a_form);
	}

	inline void UnregisterForWeatherChange(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto regs = OnWeatherChangeRegSet::GetSingleton();
		regs->Unregister(a_form);
	}

	inline void Bind(VM& a_vm)
	{
		auto const obj = "PO3_Events_Form"sv;

		BIND(AddKeywordToForm);
		BIND(ClearRecordFlag);
		BIND(EvaluateConditionList);
		BIND(GetConditionList);
		BIND(GetFormEditorID);
		BIND(IsFormInMod, true);
		BIND(IsGeneratedForm, true);
		BIND(IsRecordFlagSet);
		BIND(IsScriptAttachedToForm);
		BIND(MarkItemAsFavorite);
		BIND(RemoveKeywordOnForm);
		BIND(ReplaceKeywordOnForm);
		BIND(SetRecordFlag);
		BIND(UnmarkItemAsFavorite);

		BIND_EVENT(RegisterForActorKilled, true);
		BIND_EVENT(RegisterForFECReset, true);
		BIND_EVENT(RegisterForBookRead, true);
		BIND_EVENT(RegisterForCellFullyLoaded, true);
		BIND_EVENT(RegisterForCriticalHit, true);
		BIND_EVENT(RegisterForDisarmed, true);
		BIND_EVENT(RegisterForDragonSoulGained, true);
		BIND_EVENT(RegisterForItemHarvested, true);
		BIND_EVENT(RegisterForLevelIncrease, true);
		BIND_EVENT(RegisterForLocationDiscovery, true);
		BIND_EVENT(RegisterForObjectGrab, true);
		BIND_EVENT(RegisterForObjectLoaded, true);
		BIND_EVENT(RegisterForQuest, true);
		BIND_EVENT(RegisterForQuestStage, true);
		BIND_EVENT(RegisterForShoutAttack, true);
		BIND_EVENT(RegisterForSkillIncrease, true);
		BIND_EVENT(RegisterForSoulTrapped, true);
		BIND_EVENT(RegisterForSpellLearned, true);
		BIND_EVENT(RegisterForWeatherChange, true);

		BIND_EVENT(UnregisterForActorKilled, true);
		BIND_EVENT(UnregisterForFECReset, true);
		BIND_EVENT(UnregisterForAllFECResets, true);
		BIND_EVENT(UnregisterForBookRead, true);
		BIND_EVENT(UnregisterForCellFullyLoaded, true);
		BIND_EVENT(UnregisterForCriticalHit, true);
		BIND_EVENT(UnregisterForDisarmed, true);
		BIND_EVENT(UnregisterForDragonSoulGained, true);
		BIND_EVENT(UnregisterForItemHarvested, true);
		BIND_EVENT(UnregisterForLevelIncrease, true);
		BIND_EVENT(UnregisterForLocationDiscovery, true);
		BIND_EVENT(UnregisterForObjectGrab, true);
		BIND_EVENT(UnregisterForObjectLoaded, true);
		BIND_EVENT(UnregisterForAllObjectsLoaded, true);
		BIND_EVENT(UnregisterForQuest, true);
		BIND_EVENT(UnregisterForAllQuests, true);
		BIND_EVENT(UnregisterForQuestStage, true);
		BIND_EVENT(UnregisterForAllQuestStages, true);
		BIND_EVENT(UnregisterForShoutAttack, true);
		BIND_EVENT(UnregisterForSkillIncrease, true);
		BIND_EVENT(UnregisterForSoulTrapped, true);
		BIND_EVENT(UnregisterForSpellLearned, true);
		BIND_EVENT(UnregisterForWeatherChange, true);

		logger::info("Registered form functions"sv);
	}
}
