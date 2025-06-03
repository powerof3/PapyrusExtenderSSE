#pragma once

#include "Game/HookedEventHandler.h"
#include "Papyrus/Util/Inventory.h"
#include "Serialization/Services.h"

namespace Papyrus::Form::Functions
{
	void                           AddKeywordToForm(STATIC_ARGS, RE::TESForm* a_form, RE::BGSKeyword* a_keyword);
	void                           ClearRecordFlag(STATIC_ARGS, RE::TESForm* a_form, std::uint32_t a_flag);
	bool                           EvaluateConditionList(STATIC_ARGS, RE::TESForm* a_form, RE::TESObjectREFR* a_actionRef, RE::TESObjectREFR* a_target);
	std::vector<std::string>       GetConditionList(STATIC_ARGS, RE::TESForm* a_form, std::uint32_t a_index);
	RE::BSFixedString              GetDescription(STATIC_ARGS, RE::TESForm* a_form);
	RE::BSFixedString              GetFormEditorID(STATIC_ARGS, const RE::TESForm* a_form);
	RE::BSFixedString              GetFormModName(STATIC_ARGS, const RE::TESForm* a_form, bool a_lastModified);
	std::vector<RE::BSFixedString> GetScriptsAttachedToForm(STATIC_ARGS, const RE::TESForm* a_form);
	bool                           IsFormInMod(STATIC_ARGS, const RE::TESForm* a_form, RE::BSFixedString a_modName);
	bool                           IsGeneratedForm(STATIC_ARGS, const RE::TESForm* a_form);
	bool                           IsRecordFlagSet(STATIC_ARGS, const RE::TESForm* a_form, std::uint32_t a_flag);
	bool                           IsScriptAttachedToForm(STATIC_ARGS, const RE::TESForm* a_form, RE::BSFixedString a_scriptName);
	void                           MarkItemAsFavorite(STATIC_ARGS, RE::TESForm* a_form);
	void                           RemoveConditionList(STATIC_ARGS, RE::TESForm* a_form, std::uint32_t a_index, std::vector<std::string> a_conditionList);
	bool                           RemoveKeywordOnForm(STATIC_ARGS, RE::TESForm* a_form, RE::BGSKeyword* a_keyword);
	void                           ReplaceKeywordOnForm(STATIC_ARGS, RE::TESForm* a_form, const RE::BGSKeyword* a_remove, RE::BGSKeyword* a_add);
	void                           SetConditionList(STATIC_ARGS, RE::TESForm* a_form, std::uint32_t a_index, std::vector<std::string> a_conditionList);
	bool                           SetFastTravelDisabled(STATIC_ARGS, bool a_disable);
	bool                           SetFastTravelTargetFormID(STATIC_ARGS, const RE::FormID a_formID);
	bool                           SetFastTravelTargetRef(STATIC_ARGS, RE::TESObjectREFR* a_ref);
	bool                           SetFastTravelTargetString(STATIC_ARGS, RE::BSFixedString a_name);
	float                          SetFastTravelWaitTimeout(STATIC_ARGS, float a_timeout);
	void                           SetRecordFlag(STATIC_ARGS, RE::TESForm* a_form, std::uint32_t a_flag);
	void                           UnmarkItemAsFavorite(STATIC_ARGS, RE::TESForm* a_form);

	void Bind(VM& a_vm);
}
