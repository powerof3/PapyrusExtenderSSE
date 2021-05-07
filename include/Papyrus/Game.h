#pragma once


namespace papyrusGame
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;


	template <class T>
	void GetAllForms(std::vector<T*>& a_vec, const std::vector<RE::BGSKeyword*>& a_keywords)
	{
		if (auto dataHandler = RE::TESDataHandler::GetSingleton(); dataHandler) {
			for (const auto& form : dataHandler->GetFormArray<T>()) {
				if (!form || !a_keywords.empty() && !form->HasKeywords(a_keywords)) {
					continue;
				}
				a_vec.push_back(form);
			}
		}
	}
	
	template <class T>
	void GetAllFormsInMod(const RE::TESFile* a_modInfo, std::vector<T*>& a_vec, const std::vector<RE::BGSKeyword*>& a_keywords)
	{
		if (auto dataHandler = RE::TESDataHandler::GetSingleton(); dataHandler) {
			for (const auto& form : dataHandler->GetFormArray<T>()) {
				if (!form || !a_modInfo->IsFormInMod(form->formID) || !a_keywords.empty() && !form->HasKeywords(a_keywords)) {
					continue;
				}
				a_vec.push_back(form);
			}
		}
	}


	std::vector<RE::Actor*> GetActorsByProcessingLevel(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, std::int32_t a_level);

	std::vector<RE::EnchantmentItem*> GetAllEnchantments(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, std::vector<RE::BGSKeyword*> a_keywords);

	std::vector<RE::TESRace*> GetAllRaces(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, std::vector<RE::BGSKeyword*> a_keywords);

	std::vector<RE::SpellItem*> GetAllSpells(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, std::vector<RE::BGSKeyword*> a_keywords, bool a_playable);

	std::vector<RE::EnchantmentItem*> GetAllEnchantmentsInMod(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_name, std::vector<RE::BGSKeyword*> a_keywords);

	std::vector<RE::TESRace*> GetAllRacesInMod(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_name, std::vector<RE::BGSKeyword*> a_keywords);

	std::vector<RE::SpellItem*> GetAllSpellsInMod(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_name, std::vector<RE::BGSKeyword*> a_keywords, bool a_playable);

	std::vector<RE::TESObjectCELL*> GetAttachedCells(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*);

	std::int32_t GetGameSettingBool(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_gamesetting);

	std::vector<float> GetLocalGravity(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*);

	std::int32_t GetNumActorsInHigh(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*);

	std::vector<std::int32_t> GetPapyrusExtenderVersion(VM*, StackID, RE::StaticFunctionTag*);

	bool IsPluginFound(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_name);

	bool IsSurvivalModeActive(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*);

	void SetLocalGravity(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, float a_x, float a_y, float a_z);


	bool RegisterFuncs(VM* a_vm);
}