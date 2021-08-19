#pragma once

namespace Papyrus::Enchantment
{
	inline void AddMagicEffectToEnchantment(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::EnchantmentItem* a_enchantment,
		RE::EffectSetting* a_mgef,
		float a_mag,
		std::uint32_t a_area,
		std::uint32_t a_dur,
		float a_cost,
        std::vector<RE::BSFixedString> a_conditionList)
	{
		if (!a_enchantment) {
			a_vm->TraceStack("Enchantment is None", a_stackID);
			return;
		}
		if (!a_mgef) {
			a_vm->TraceStack("MagicEffect is None", a_stackID);
			return;
		}
		if (a_mgef->data.castingType != a_enchantment->data.castingType) {
			a_vm->TraceForm(a_mgef, "Casting types don't match", a_stackID);
			return;
		}
		if (a_mgef->data.delivery != a_enchantment->data.delivery) {
			a_vm->TraceForm(a_mgef, "Delivery types don't match", a_stackID);
			return;
		}

		auto result = MAGIC::add_magic_effect(
			a_enchantment, a_mgef, a_mag, a_area, a_dur, a_cost,
			a_conditionList);
		if (result == MAGIC::RESULT::kFailParse) {
			a_vm->TraceForm(a_enchantment, "failed to parse condition list", a_stackID);
		} else if (result == MAGIC::RESULT::kFailExists) {
			a_vm->TraceForm(a_enchantment, "effect already exists", a_stackID);
		}
	}

	inline std::int32_t GetEnchantmentType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EnchantmentItem* a_enchantment)
	{
		if (!a_enchantment) {
			a_vm->TraceStack("Enchantment is None", a_stackID);
			return -1;
		}

		return stl::to_underlying(a_enchantment->data.spellType);
	}

	inline void AddEffectItemToEnchantment(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::EnchantmentItem* a_enchantment,
		RE::EnchantmentItem* a_copyEnchantment,
		std::uint32_t a_index,
		float a_cost)
	{
		if (!a_enchantment) {
			a_vm->TraceStack("Enchantment is None", a_stackID);
			return;
		}
		if (!a_copyEnchantment) {
			a_vm->TraceStack("Copy Enchantment is None", a_stackID);
			return;
		}
		if (a_index > a_copyEnchantment->effects.size()) {
			a_vm->TraceForm(a_copyEnchantment, "Copy Enchantment index exceeds effect list size", a_stackID);
			return;
		}
		if (a_enchantment->data.castingType != a_copyEnchantment->data.castingType) {
			a_vm->TraceForm(a_enchantment, "Casting types don't match", a_stackID);
			return;
		}
		if (a_enchantment->data.delivery != a_copyEnchantment->data.delivery) {
			a_vm->TraceForm(a_enchantment, "Delivery types don't match", a_stackID);
			return;
		}

		MAGIC::add_effect_item(a_enchantment, a_copyEnchantment, a_index, a_cost);
	}

	inline void RemoveMagicEffectFromEnchantment(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::EnchantmentItem* a_enchantment,
		RE::EffectSetting* a_mgef, 
		float a_mag,
		std::uint32_t a_area,
		std::uint32_t a_dur,
		float a_cost)
	{
		if (!a_enchantment) {
			a_vm->TraceStack("Enchantment is None", a_stackID);
			return;
		}
		if (!a_mgef) {
			a_vm->TraceStack("MagicEffect is None", a_stackID);
			return;
		}

		MAGIC::remove_magic_effect(a_enchantment, a_mgef, a_mag, a_area, a_dur, a_cost);
	}

	inline void RemoveEffectItemFromEnchantment(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		RE::EnchantmentItem* a_enchantment, 
		RE::EnchantmentItem* a_copyEnchantment, 
		std::uint32_t a_index)
	{
		if (!a_enchantment) {
			a_vm->TraceStack("Enchantment is None", a_stackID);
			return;
		}
		if (!a_copyEnchantment) {
			a_vm->TraceStack("Copy Enchantment is None", a_stackID);
			return;
		}
		if (a_index > a_copyEnchantment->effects.size()) {
			a_vm->TraceForm(a_copyEnchantment, "Copy Enchantment index exceeds effect list size", a_stackID);
			return;
		}

		MAGIC::remove_effect_item(a_enchantment, a_copyEnchantment, a_index);
	}

	inline void Bind(VM& a_vm)
	{
		BIND(AddMagicEffectToEnchantment);
		BIND(AddEffectItemToEnchantment);
		BIND(GetEnchantmentType);
		BIND(RemoveMagicEffectFromEnchantment);
		BIND(RemoveEffectItemFromEnchantment);

		logger::info("Registered enchantment functions"sv);
	}
}
