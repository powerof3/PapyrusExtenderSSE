#pragma once


namespace papyrusSpell
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;


	void AddMagicEffectToSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::SpellItem* a_spell, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost, std::vector<RE::BSFixedString> a_conditionList);

	void AddEffectItemToSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::SpellItem* a_targetSpell, RE::SpellItem* a_copySpell, std::uint32_t a_index);

	std::int32_t GetSpellType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::SpellItem* a_spell);

	void RemoveMagicEffectFromSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::SpellItem* a_spell, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost);

	void RemoveEffectItemFromSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::SpellItem* a_spell, RE::SpellItem* a_copySpell, std::uint32_t a_index);


	bool RegisterFuncs(VM* a_vm);
}
