#pragma once

#include "Game/Cache.h"
#include "Game/HookedEventHandler.h"
#include "Serialization/Services.h"

namespace Papyrus::Form::Functions
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
		case RE::FormType::Perk:
			{
				const auto perk = a_form->As<RE::BGSPerk>();
				if (perk && perk->perkConditions.IsTrue(a_actionRef, a_target)) {
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

	inline RE::BSFixedString GetDescription(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return RE::BSFixedString();
		}

		if (const auto description = a_form->As<RE::TESDescription>(); description) {
			RE::BSString str;
			description->GetDescription(str, nullptr);

			std::string temp(str);
			string::replace_all(temp, "\r"sv, ""sv);  //remove escape character not supported by BSFixedString
			return temp;
		}

		return RE::BSFixedString();
	}

	inline RE::BSFixedString GetFormEditorID(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return RE::BSFixedString();
		}

		return Cache::EditorID::GetSingleton()->GetEditorID(a_form->GetFormID());
	}

	inline RE::BSFixedString GetFormModName(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form, bool a_lastModified)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return RE::BSFixedString();
		}

		if (const auto file =
				a_lastModified ?
                    a_form->GetDescriptionOwnerFile() :
                    a_form->GetFile(0);
			file) {
			return file->GetFilename();
		}

		return RE::BSFixedString();
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
				REL::Relocation<func_t> func{ REL_ID(15858, 16098) };
				return func(a_changes, a_entryData, a_list);
			}

			inline void unfavorite(RE::InventoryChanges* a_changes, RE::InventoryEntryData* a_entryData, RE::ExtraDataList* a_list)
			{
				using func_t = decltype(&unfavorite);
				REL::Relocation<func_t> func{ REL_ID(15859, 16099) };
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
				REL::Relocation<func_t> func{ REL_ID(51121, 52004) };
				return func(a_magicFavorites, a_form);
			}

			inline void unfavorite(RE::MagicFavorites* a_magicFavorites, RE::TESForm* a_form)
			{
				using func_t = decltype(&unfavorite);
				REL::Relocation<func_t> func{ REL_ID(51122, 52005) };
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
			if (const auto magicFavorites = RE::MagicFavorites::GetSingleton(); magicFavorites && std::ranges::find(magicFavorites->spells, a_form) == magicFavorites->spells.end()) {
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
					if (count > 0 && item == a_form && !entry->IsFavorited()) {
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

	inline bool SetFastTravelTargetFormID(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::FormID a_formID)
	{
		if (!a_formID) {
			a_vm->TraceStack("Form is None", a_stackID);
			return false;
		}
		return Event::FastTravel::SetFastTravelTarget(a_formID);
	}

	inline bool SetFastTravelTargetRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref
	)
	{
		if (!a_ref) {
			a_vm->TraceStack("Form is None", a_stackID);
			return false;
		}
		return Event::FastTravel::SetFastTravelTarget(a_ref);
	}

	inline bool SetFastTravelTargetString(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::BSFixedString a_name)
	{
		if (a_name.empty()) {
			a_vm->TraceStack("Name is empty", a_stackID);
			return false;
		}
		return Event::FastTravel::SetFastTravelTarget(a_name.c_str());
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
			if (const auto magicFavorites = RE::MagicFavorites::GetSingleton(); magicFavorites && std::ranges::find(magicFavorites->spells, a_form) != magicFavorites->spells.end()) {
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
						if (const auto extralist = item::get_hotkeyed(entry.get()); extralist) {
							item::unfavorite(invChanges, entry.get(), extralist);
						}
						break;
					}
				}
			}
		}
	}

	inline void Bind(VM& a_vm)
	{
		BIND(AddKeywordToForm);
		BIND(ClearRecordFlag);
		BIND(EvaluateConditionList);
		BIND(GetConditionList);
		BIND(GetDescription);
		BIND(GetFormEditorID);
		BIND(GetFormModName, true);
		BIND(IsFormInMod, true);
		BIND(IsGeneratedForm, true);
		BIND(IsRecordFlagSet);
		BIND(IsScriptAttachedToForm);
		BIND(MarkItemAsFavorite);
		BIND(RemoveKeywordOnForm);
		BIND(ReplaceKeywordOnForm);
		BIND(SetFastTravelTargetFormID);
		BIND(SetFastTravelTargetRef);
		BIND(SetFastTravelTargetString);
		BIND(SetRecordFlag);
		BIND(UnmarkItemAsFavorite);

		logger::info("Registered form functions"sv);
	}
}
