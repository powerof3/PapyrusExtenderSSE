#pragma once

#include "Game/Cache.h"
#include "Game/HookedEventHandler.h"
#include "Papyrus/Util/Inventory.h"
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
				if (const auto magicItem = a_form->As<RE::MagicItem>()) {
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
				result = effect->conditions.IsTrue(a_actionRef, a_target);
			}
			break;
		case RE::FormType::Info:
			{
				const auto topic = a_form->As<RE::TESTopicInfo>();
				result = topic->objConditions.IsTrue(a_actionRef, a_target);
			}
			break;
		case RE::FormType::Package:
			{
				const auto package = a_form->As<RE::TESPackage>();
				result = package->packConditions.IsTrue(a_actionRef, a_target);
			}
			break;
		case RE::FormType::Perk:
			{
				const auto perk = a_form->As<RE::BGSPerk>();
				result = perk->perkConditions.IsTrue(a_actionRef, a_target);
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

		const auto formConditions = CONDITION::GetConditions(*a_form, a_index);
		if (!formConditions || !formConditions->head) {
			a_vm->TraceStack("Form does not have a condition stack", a_stackID);
			return result;
		}

		return CONDITION::BuildConditionList(formConditions);
	}

	inline RE::BSFixedString GetDescription(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return {};
		}

		if (const auto description = a_form->As<RE::TESDescription>(); description) {
			RE::BSString str;
			description->GetDescription(str, nullptr);

			std::string temp(str);
			string::replace_all(temp, "\r"sv, ""sv);  //remove escape character not supported by BSFixedString
			return temp;
		}

		return {};
	}

	inline RE::BSFixedString GetFormEditorID(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return {};
		}

		return Cache::EditorID::GetFormEditorID(a_form);
	}

	inline RE::BSFixedString GetFormModName(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form, bool a_lastModified)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return {};
		}

		if (const auto file =
				a_lastModified ?
                    a_form->GetDescriptionOwnerFile() :
                    a_form->GetFile(0);
			file) {
			return file->GetFilename();
		}

		return {};
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

	inline void MarkItemAsFavorite(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		if (a_form->Is(RE::FormType::Spell, RE::FormType::Shout)) {
			const auto magicFavorites = RE::MagicFavorites::GetSingleton();
			if (magicFavorites && std::ranges::find(magicFavorites->spells, a_form) == magicFavorites->spells.end()) {
				magicFavorites->SetFavorite(a_form);
			}
		} else {
			const auto player = RE::PlayerCharacter::GetSingleton();
			if (const auto invChanges = player->GetInventoryChanges()) {
				auto inv = player->GetInventory();
				for (const auto& [item, data] : inv) {
					const auto& [count, entry] = data;
					if (count > 0 && item == a_form && !entry->IsFavorited()) {
						const auto extralist = entry->extraLists ? entry->extraLists->front() : nullptr;
						invChanges->SetFavorite(entry.get(), extralist);
						break;
					}
				}
			}
		}
	}

	inline void RemoveConditionList(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESForm* a_form,
		std::uint32_t a_index,
		std::vector<std::string> a_conditionList)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		if (a_conditionList.empty()) {
			a_vm->TraceStack("Condition List is empty", a_stackID);
			return;
		}

		const auto formConditions = CONDITION::GetConditions(*a_form, a_index);
		if (!formConditions || !formConditions->head) {
			a_vm->TraceStack("Form does not have a condition stack", a_stackID);
			return;
		}

		if (const auto conditions = CONDITION::ParseConditionList(a_conditionList); !conditions.empty()) {
			auto* currentNode = formConditions->head;
			auto** previousNode = &formConditions->head;

			while (currentNode != nullptr) {
				if (std::ranges::find(conditions, currentNode) != conditions.end()) {
					*previousNode = currentNode->next;
					delete currentNode;
				} else {
					previousNode = &(currentNode->next);
				}
				currentNode = *previousNode;
			}
		}
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
					if (const auto keyword = keywordForm->keywords[i]) {
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

	inline void SetConditionList(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESForm* a_form,
		std::uint32_t a_index,
		std::vector<std::string> a_conditionList)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		if (a_conditionList.empty()) {
			a_vm->TraceStack("Condition List is empty", a_stackID);
			return;
		}

		const auto formConditions = CONDITION::GetConditions(*a_form, a_index);
		if (!formConditions) {
			a_vm->TraceStack("Form does not have a condition stack", a_stackID);
			return;
		}

		if (auto conditions = CONDITION::ParseConditionList(a_conditionList); !conditions.empty()) {
			for (auto& [object, functionID, param1, param2, opCode, value, ANDOR] : conditions) {
				if (const auto newNode = new RE::TESConditionItem) {
					newNode->next = nullptr;
					newNode->data.object = object;
					newNode->data.functionData.function = functionID;
					newNode->data.functionData.params[0] = param1;
					newNode->data.functionData.params[1] = param2;
					newNode->data.flags.opCode = opCode;
					newNode->data.comparisonValue.f = value;
					newNode->data.flags.isOR = ANDOR;

					if (formConditions->head == nullptr) {
						formConditions->head = newNode;
					} else {
						auto* current = formConditions->head;
						while (current->next != nullptr) {
							current = current->next;
						}
						current->next = newNode;
					}
				}
			}
		}
	}

	inline bool SetFastTravelDisabled(VM*, StackID, RE::StaticFunctionTag*, bool a_disable)
	{
		return Event::FastTravel::SetFastTravelDisabled(a_disable);
	}

	inline bool SetFastTravelTargetFormID(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::FormID a_formID)
	{
		if (!a_formID) {
			a_vm->TraceStack("Form is None", a_stackID);
			return false;
		}
		return Event::FastTravel::SetFastTravelTarget(a_formID);
	}

	inline bool SetFastTravelTargetRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) {
			a_vm->TraceStack("Form is None", a_stackID);
			return false;
		}
		return Event::FastTravel::SetFastTravelTarget(a_ref);
	}

	inline bool SetFastTravelTargetString(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_name)
	{
		if (a_name.empty()) {
			a_vm->TraceStack("Name is empty", a_stackID);
			return false;
		}
		return Event::FastTravel::SetFastTravelTarget(a_name.c_str());
	}

	inline float SetFastTravelWaitTimeout(VM*, StackID, RE::StaticFunctionTag*, float a_timeout)
	{
		return Event::FastTravel::SetFastTravelWaitTimeout(a_timeout);
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
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		if (a_form->Is(RE::FormType::Spell, RE::FormType::Shout)) {
			const auto magicFavorites = RE::MagicFavorites::GetSingleton();
			if (magicFavorites && std::ranges::find(magicFavorites->spells, a_form) != magicFavorites->spells.end()) {
				magicFavorites->RemoveFavorite(a_form);
			}
		} else {
			const auto player = RE::PlayerCharacter::GetSingleton();
			if (const auto invChanges = player->GetInventoryChanges()) {
				auto inv = player->GetInventory();
				for (const auto& [item, data] : inv) {
					const auto& [count, entry] = data;
					if (count > 0 && item == a_form) {
						if (const auto extralist = INV::get_hotkey_extralist(entry.get()); extralist) {
							invChanges->RemoveFavorite(entry.get(), extralist);
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
		BIND(RemoveConditionList);
		BIND(RemoveKeywordOnForm);
		BIND(ReplaceKeywordOnForm);
		BIND(SetConditionList);
		BIND(SetFastTravelDisabled);
		BIND(SetFastTravelTargetFormID);
		BIND(SetFastTravelTargetRef);
		BIND(SetFastTravelTargetString);
		BIND(SetFastTravelWaitTimeout);
		BIND(SetRecordFlag);
		BIND(UnmarkItemAsFavorite);

		logger::info("Registered form functions"sv);
	}
}
