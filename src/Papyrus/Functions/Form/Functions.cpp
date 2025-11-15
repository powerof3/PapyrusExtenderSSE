#include "Papyrus/Functions/Form/Functions.h"

#include "Game/HookedEventHandler.h"
#include "Papyrus/Util/ConditionParser.h"
#include "Papyrus/Util/Inventory.h"
#include "Papyrus/Util/Script.h"
#include "Serialization/Manager.h"

namespace Papyrus::Form::Functions
{
	void AddKeywordToForm(STATIC_ARGS, RE::TESForm* a_form, RE::BGSKeyword* a_keyword)
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

		Serialization::Manager::GetSingleton()->keywords.Add(a_form, a_keyword);
	}

	void ClearRecordFlag(STATIC_ARGS, RE::TESForm* a_form, std::uint32_t a_flag)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		a_form->formFlags &= ~a_flag;
	}

	bool EvaluateConditionList(STATIC_ARGS, RE::TESForm* a_form, RE::TESObjectREFR* a_actionRef, RE::TESObjectREFR* a_target)
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
			result = a_form->As<RE::EffectSetting>()->conditions.IsTrue(a_actionRef, a_target);
			break;
		case RE::FormType::Info:
			result = a_form->As<RE::TESTopicInfo>()->objConditions.IsTrue(a_actionRef, a_target);
			break;
		case RE::FormType::Package:
			result = a_form->As<RE::TESPackage>()->packConditions.IsTrue(a_actionRef, a_target);
			break;
		case RE::FormType::Perk:
			result = a_form->As<RE::BGSPerk>()->perkConditions.IsTrue(a_actionRef, a_target);
			break;
		case RE::FormType::CameraPath:
			result = a_form->As<RE::BGSCameraPath>()->conditions.IsTrue(a_actionRef, a_target);
			break;
		case RE::FormType::ConstructibleObject:
			result = a_form->As<RE::BGSConstructibleObject>()->conditions.IsTrue(a_actionRef, a_target);
			break;
		case RE::FormType::Faction:
			{
				const auto vendorConditions = a_form->As<RE::TESFaction>()->vendorData.vendorConditions;
				result = vendorConditions && vendorConditions->IsTrue(a_actionRef, a_target);
			}
			break;
		case RE::FormType::Idle:
			result = a_form->As<RE::TESIdleForm>()->conditions.IsTrue(a_actionRef, a_target);
			break;
		case RE::FormType::LoadScreen:
			result = a_form->As<RE::TESLoadScreen>()->conditions.IsTrue(a_actionRef, a_target);
			break;
		default:
			result = true;
			break;
		}

		return result;
	}

	std::vector<std::string> GetConditionList(STATIC_ARGS, RE::TESForm* a_form, std::uint32_t a_index)
	{
		std::vector<std::string> result;

		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return result;
		}

		const auto formConditions = CONDITION::GetConditions(*a_form, a_index);
		if (!formConditions || !formConditions->head) {
			a_vm->TraceForm(a_form, "does not have a condition stack", a_stackID);
			return result;
		}

		return CONDITION::BuildConditionList(formConditions);
	}

	RE::BSFixedString GetDescription(STATIC_ARGS, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return {};
		}

		std::string descriptionStr;
		if (g_DescriptionFrameworkInterface) {
			descriptionStr = g_DescriptionFrameworkInterface->GetDescription(a_form);
		}

		if (descriptionStr.empty()) {
			if (const auto description = a_form->As<RE::TESDescription>()) {
				RE::BSString str;
				description->GetDescription(str, nullptr);
				descriptionStr = str;
			}
		}

		string::replace_all(descriptionStr, "\r"sv, ""sv);  //remove escape character not supported by BSFixedString
		return descriptionStr;
	}

	RE::BSFixedString GetFormEditorID(STATIC_ARGS, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return {};
		}

		return editorID::get_editorID(a_form);
	}

	RE::BSFixedString GetFormModName(STATIC_ARGS, const RE::TESForm* a_form, bool a_lastModified)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return {};
		}

		if (const auto file =
				a_lastModified ?
					a_form->GetDescriptionOwnerFile() :
					a_form->GetFile(0)) {
			return file->GetFilename();
		}

		return {};
	}

	std::vector<RE::BSFixedString> GetScriptsAttachedToForm(STATIC_ARGS, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return {};
		}

		return SCRIPT::get_all_scripts(a_form);
	}

	bool IsFormInMod(STATIC_ARGS, const RE::TESForm* a_form, RE::BSFixedString a_modName)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return false;
		}

		const auto modInfo = RE::TESDataHandler::GetSingleton()->LookupModByName(a_modName);
		return modInfo ? modInfo->IsFormInMod(a_form->GetFormID()) : false;
	}

	bool IsGeneratedForm(STATIC_ARGS, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return false;
		}

		return a_form->IsDynamicForm();
	}

	bool IsRecordFlagSet(STATIC_ARGS, const RE::TESForm* a_form, std::uint32_t a_flag)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return false;
		}

		return (a_form->formFlags & a_flag) != 0;
	}

	bool IsScriptAttachedToForm(STATIC_ARGS, const RE::TESForm* a_form, RE::BSFixedString a_scriptName)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return false;
		}

		return SCRIPT::is_script_attached(a_form, a_scriptName);
	}

	void MarkItemAsFavorite(STATIC_ARGS, RE::TESForm* a_form)
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

	void RemoveConditionList(STATIC_ARGS, RE::TESForm* a_form, std::uint32_t a_index, std::vector<std::string> a_conditionList)
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
			a_vm->TraceForm(a_form, "does not have a condition stack", a_stackID);
			return;
		}

		if (const auto conditions = CONDITION::ParseConditionList(a_conditionList); !conditions.empty()) {
			auto*  currentNode = formConditions->head;
			auto** previousNode = &formConditions->head;

			while (currentNode != nullptr) {
				if (std::ranges::find(conditions, currentNode->data) != conditions.end()) {
					*previousNode = currentNode->next;
					delete currentNode;
				} else {
					previousNode = &(currentNode->next);
				}
				currentNode = *previousNode;
			}
		}
	}

	bool RemoveKeywordOnForm(STATIC_ARGS, RE::TESForm* a_form, RE::BGSKeyword* a_keyword)
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

		return Serialization::Manager::GetSingleton()->keywords.Remove(a_form, a_keyword);
	}

	void ReplaceKeywordOnForm(STATIC_ARGS, RE::TESForm* a_form, const RE::BGSKeyword* a_remove, RE::BGSKeyword* a_add)
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
				bool          found = false;
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

	void SetConditionList(STATIC_ARGS, RE::TESForm* a_form, std::uint32_t a_index, std::vector<std::string> a_conditionList)
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
			a_vm->TraceForm(a_form, "does not have a condition stack", a_stackID);
			return;
		}

		if (const auto conditions = CONDITION::ParseConditionList(a_conditionList); !conditions.empty()) {
			for (const auto& conditionData : conditions) {
				if (const auto newNode = new RE::TESConditionItem) {
					newNode->next = nullptr;
					newNode->data = conditionData;

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

	bool SetFastTravelDisabled(STATIC_ARGS, bool a_disable)
	{
		return Event::FastTravel::SetFastTravelDisabled(a_disable);
	}

	bool SetFastTravelTargetFormID(STATIC_ARGS, const RE::FormID a_formID)
	{
		if (!a_formID) {
			a_vm->TraceStack("Form is None", a_stackID);
			return false;
		}

		return Event::FastTravel::SetFastTravelTarget(a_formID);
	}

	bool SetFastTravelTargetRef(STATIC_ARGS, RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) {
			a_vm->TraceStack("Form is None", a_stackID);
			return false;
		}

		return Event::FastTravel::SetFastTravelTarget(a_ref);
	}

	bool SetFastTravelTargetString(STATIC_ARGS, RE::BSFixedString a_name)
	{
		if (a_name.empty()) {
			a_vm->TraceStack("Name is empty", a_stackID);
			return false;
		}

		return Event::FastTravel::SetFastTravelTarget(a_name.c_str());
	}

	float SetFastTravelWaitTimeout(STATIC_ARGS, float a_timeout)
	{
		return Event::FastTravel::SetFastTravelWaitTimeout(a_timeout);
	}

	void SetRecordFlag(STATIC_ARGS, RE::TESForm* a_form, std::uint32_t a_flag)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		a_form->formFlags |= a_flag;
	}

	void UnmarkItemAsFavorite(STATIC_ARGS, RE::TESForm* a_form)
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

	void Bind(VM& a_vm)
	{
		std::uint32_t count = 0;
		
		BIND(AddKeywordToForm);
		BIND(ClearRecordFlag);
		BIND(EvaluateConditionList);
		BIND(GetConditionList);
		BIND(GetDescription);
		BIND(GetFormEditorID);
		BIND(GetFormModName, true);
		BIND(GetScriptsAttachedToForm);
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

		logger::info("Registered {} form functions"sv, count);
	}
}
