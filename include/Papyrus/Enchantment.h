#pragma once

namespace papyrusEnchantment
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;


	void AddMagicEffectToEnchantment(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EnchantmentItem* a_enchantment, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost, std::vector<RE::BSFixedString> a_conditionList);

	auto GetEnchantmentType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EnchantmentItem* a_enchantment) -> std::int32_t;

	void AddEffectItemToEnchantment(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EnchantmentItem* a_enchantment, RE::EnchantmentItem* a_copyEnchantment, std::uint32_t a_index);

	void RemoveMagicEffectFromEnchantment(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EnchantmentItem* a_enchantment, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost);

	void RemoveEffectItemFromEnchantment(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EnchantmentItem* a_enchantment, RE::EnchantmentItem* a_copyEnchantment, std::uint32_t a_index);


	bool RegisterFuncs(VM* a_vm);
}