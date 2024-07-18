#pragma once

#include "Serialization/Services.h"

namespace Papyrus::Enchantment
{
	inline void AddMagicEffectToEnchantment(STATIC_ARGS,
		RE::EnchantmentItem*     a_enchantment,
		RE::EffectSetting*       a_mgef,
		float                    a_mag,
		std::uint32_t            a_area,
		std::uint32_t            a_dur,
		float                    a_cost,
		std::vector<std::string> a_conditionList)
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

		MAGIC::MGEFData data{
			std::make_pair(a_mgef, a_mgef->GetFormID()),
			a_mag,
			a_area,
			a_dur,
			a_cost,
			std::move(a_conditionList)
		};

		MAGIC::MGEFManager::GetSingleton()->Add(a_enchantment, data);
	}

	inline std::int32_t GetEnchantmentType(STATIC_ARGS, RE::EnchantmentItem* a_enchantment)
	{
		if (!a_enchantment) {
			a_vm->TraceStack("Enchantment is None", a_stackID);
			return -1;
		}

		return std::to_underlying(a_enchantment->data.spellType);
	}

	inline void AddEffectItemToEnchantment(STATIC_ARGS, RE::EnchantmentItem* a_enchantment, RE::EnchantmentItem* a_copyEnchantment, std::uint32_t a_index, float a_cost)
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

		MAGIC::EffectData data{
			std::make_pair(a_copyEnchantment, a_copyEnchantment->GetFormID()),
			a_index,
			a_cost
		};

		MAGIC::EffectManager::GetSingleton()->Add(a_enchantment, data);
	}

	inline void RemoveMagicEffectFromEnchantment(STATIC_ARGS,
		RE::EnchantmentItem* a_enchantment,
		RE::EffectSetting*   a_mgef,
		float                a_mag,
		std::uint32_t        a_area,
		std::uint32_t        a_dur,
		float                a_cost)
	{
		if (!a_enchantment) {
			a_vm->TraceStack("Enchantment is None", a_stackID);
			return;
		}
		if (!a_mgef) {
			a_vm->TraceStack("MagicEffect is None", a_stackID);
			return;
		}

		MAGIC::MGEFData data{
			std::make_pair(a_mgef, a_mgef->GetFormID()),
			a_mag,
			a_area,
			a_dur,
			a_cost,
			std::vector<std::string>()
		};

		MAGIC::MGEFManager::GetSingleton()->Remove(a_enchantment, data);
	}

	inline void RemoveEffectItemFromEnchantment(STATIC_ARGS, RE::EnchantmentItem* a_enchantment, RE::EnchantmentItem* a_copyEnchantment, std::uint32_t a_index)
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

		MAGIC::EffectData data{
			std::make_pair(a_copyEnchantment, a_copyEnchantment->GetFormID()),
			a_index,
			-1.0f
		};

		MAGIC::EffectManager::GetSingleton()->Remove(a_enchantment, data);
	}

	inline void SetEnchantmentMagicEffect(STATIC_ARGS, RE::EnchantmentItem* a_enchantment, RE::EffectSetting* a_magicEffect, std::uint32_t a_index)
	{
		if (!a_enchantment) {
			a_vm->TraceStack("Enchantment is None", a_stackID);
			return;
		}

		if (!a_magicEffect) {
			a_vm->TraceStack("MagicEffect is None", a_stackID);
			return;
		}

		if (a_index > a_enchantment->effects.size()) {
			a_vm->TraceForm(a_enchantment, "Index exceeds effect list size", a_stackID);
			return;
		}

		if (auto effectItem = a_enchantment->effects[a_index]) {
			effectItem->baseEffect = a_magicEffect;
		}
	}

	inline void Bind(VM& a_vm)
	{
		BIND(AddMagicEffectToEnchantment);
		BIND(AddEffectItemToEnchantment);
		BIND(GetEnchantmentType);
		BIND(RemoveMagicEffectFromEnchantment);
		BIND(RemoveEffectItemFromEnchantment);
		BIND(SetEnchantmentMagicEffect);

		logger::info("Registered enchantment functions"sv);
	}
}
