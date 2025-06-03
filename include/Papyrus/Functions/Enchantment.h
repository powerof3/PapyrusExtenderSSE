#pragma once

#include "Serialization/Services.h"

namespace Papyrus::Enchantment
{
	void AddMagicEffectToEnchantment(STATIC_ARGS,
		RE::EnchantmentItem*     a_enchantment,
		RE::EffectSetting*       a_mgef,
		float                    a_mag,
		std::uint32_t            a_area,
		std::uint32_t            a_dur,
		float                    a_cost,
		std::vector<std::string> a_conditionList);

	std::int32_t GetEnchantmentType(STATIC_ARGS, RE::EnchantmentItem* a_enchantment);
	void         AddEffectItemToEnchantment(STATIC_ARGS, RE::EnchantmentItem* a_enchantment, RE::EnchantmentItem* a_copyEnchantment, std::uint32_t a_index, float a_cost);

	void RemoveMagicEffectFromEnchantment(STATIC_ARGS,
		RE::EnchantmentItem* a_enchantment,
		RE::EffectSetting*   a_mgef,
		float                a_mag,
		std::uint32_t        a_area,
		std::uint32_t        a_dur,
		float                a_cost);

	void RemoveEffectItemFromEnchantment(STATIC_ARGS, RE::EnchantmentItem* a_enchantment, RE::EnchantmentItem* a_copyEnchantment, std::uint32_t a_index);
	void SetEnchantmentMagicEffect(STATIC_ARGS, RE::EnchantmentItem* a_enchantment, RE::EffectSetting* a_magicEffect, std::uint32_t a_index);

	void Bind(VM& a_vm);
}
