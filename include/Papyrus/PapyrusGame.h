#pragma once


namespace papyrusGame
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;


	std::vector<RE::Actor*> GetActorsByProcessingLevel(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, std::int32_t a_level);

	std::vector<RE::TESRace*> GetAllRaces(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, std::vector<RE::BGSKeyword*> a_keywords);

	std::vector<RE::SpellItem*> GetAllSpells(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, std::vector<RE::BGSKeyword*> a_keywords, bool a_playable);

	std::vector<RE::TESRace*> GetAllRacesInMod(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_name, std::vector<RE::BGSKeyword*> a_keywords);

	std::vector<RE::SpellItem*> GetAllSpellsInMod(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_name, std::vector<RE::BGSKeyword*> a_keywords, bool a_playable);

	std::vector<RE::TESObjectCELL*> GetAttachedCells(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*);

	std::int32_t GetNumActorsInHigh(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*);

	bool IsPluginFound(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_name);


	bool RegisterFuncs(VM* a_vm);
}