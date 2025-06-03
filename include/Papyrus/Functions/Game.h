#pragma once

namespace Papyrus::Game
{
	struct forms
	{
		template <class T>
		static std::vector<T*> get_all(const std::vector<RE::BGSKeyword*>& a_keywords)
		{
			std::vector<T*> result;

			if (const auto dataHandler = RE::TESDataHandler::GetSingleton(); dataHandler) {
				for (const auto& form : dataHandler->GetFormArray<T>()) {
					if (!form || !a_keywords.empty() && !form->HasKeywordInArray(a_keywords, false)) {
						continue;
					}
					result.push_back(form);
				}
			}

			return result;
		}

		template <class T>
		static std::vector<T*> get_in_mod(const RE::TESFile* a_modInfo, const std::vector<RE::BGSKeyword*>& a_keywords)
		{
			std::vector<T*> result;

			if (const auto dataHandler = RE::TESDataHandler::GetSingleton(); dataHandler) {
				for (const auto& form : dataHandler->GetFormArray<T>()) {
					if (!form || !a_modInfo->IsFormInMod(form->formID) || !a_keywords.empty() && !form->HasKeywordInArray(a_keywords, false)) {
						continue;
					}
					result.push_back(form);
				}
			}

			return result;
		}
	};

	void                              ClearCachedFactionFightReactions(RE::StaticFunctionTag*);
	std::vector<RE::TESObjectREFR*>   FindAllReferencesOfFormType(RE::StaticFunctionTag*, RE::TESObjectREFR* a_origin, std::uint32_t a_formType, float a_radius);
	std::vector<RE::TESObjectREFR*>   FindAllReferencesOfType(STATIC_ARGS, [[maybe_unused]] RE::TESObjectREFR* a_ref, const RE::TESForm* a_formOrList, float a_radius);
	std::vector<RE::TESObjectREFR*>   FindAllReferencesWithKeyword(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::TESForm* a_formOrList, float a_radius, bool a_matchAll);
	std::vector<RE::Actor*>           GetActorsByProcessingLevel(RE::StaticFunctionTag*, std::int32_t a_level);
	std::vector<RE::EnchantmentItem*> GetAllEnchantments(RE::StaticFunctionTag*, std::vector<RE::BGSKeyword*> a_keywords);
	std::vector<RE::TESForm*>         GetAllForms(RE::StaticFunctionTag*, std::int32_t a_formType, std::vector<RE::BGSKeyword*> a_keywords);
	std::vector<RE::TESRace*>         GetAllRaces(RE::StaticFunctionTag*, std::vector<RE::BGSKeyword*> a_keywords);
	std::vector<RE::SpellItem*>       GetAllSpells(RE::StaticFunctionTag*, std::vector<RE::BGSKeyword*> a_keywords, bool a_playable);
	std::vector<RE::EnchantmentItem*> GetAllEnchantmentsInMod(RE::StaticFunctionTag*, RE::BSFixedString a_name, std::vector<RE::BGSKeyword*> a_keywords);
	std::vector<RE::TESForm*>         GetAllFormsInMod(RE::StaticFunctionTag*, RE::BSFixedString a_name, std::int32_t a_formType, std::vector<RE::BGSKeyword*> a_keywords);
	std::vector<RE::TESRace*>         GetAllRacesInMod(RE::StaticFunctionTag*, RE::BSFixedString a_name, std::vector<RE::BGSKeyword*> a_keywords);
	std::vector<RE::SpellItem*>       GetAllSpellsInMod(RE::StaticFunctionTag*, RE::BSFixedString a_name, std::vector<RE::BGSKeyword*> a_keywords, bool a_playable);
	std::vector<RE::TESObjectCELL*>   GetAttachedCells(RE::StaticFunctionTag*);
	RE::TESForm*                      GetFormFromEditorID(RE::StaticFunctionTag*, RE::BSFixedString a_editorID);
	std::int32_t                      GetGameSettingBool(RE::StaticFunctionTag*, RE::BSFixedString a_gamesetting);
	float                             GetLandHeight(RE::StaticFunctionTag*, float a_x, float a_y, float a_z);
	RE::BSFixedString                 GetLandMaterialType(RE::StaticFunctionTag*, float a_x, float a_y, float a_z);
	std::vector<float>                GetLocalGravity(RE::StaticFunctionTag*);
	std::int32_t                      GetNumActorsInHigh(RE::StaticFunctionTag*);
	std::vector<std::int32_t>         GetPapyrusExtenderVersion(RE::StaticFunctionTag*);
	std::vector<RE::Actor*>           GetPlayerFollowers(RE::StaticFunctionTag*);
	RE::BSFixedString                 GetSurfaceMaterialType(VM*, StackID, RE::StaticFunctionTag*, const float a_x, const float a_y, const float a_z);
	bool                              IsPluginFound(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_name);
	bool                              IsSurvivalModeActive(RE::StaticFunctionTag*);
	void                              SetLocalGravity(RE::StaticFunctionTag*, float a_x, float a_y, float a_z);
	void                              UpdateCrosshairs(RE::StaticFunctionTag*);

	void Bind(VM& a_vm);
}
