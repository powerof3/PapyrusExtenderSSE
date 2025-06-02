#pragma once

#include "Serialization/Services.h"

namespace Papyrus::Potion
{
	void AddMagicEffectToPotion(STATIC_ARGS,
		RE::AlchemyItem*         a_potion,
		RE::EffectSetting*       a_mgef,
		float                    a_mag,
		std::uint32_t            a_area,
		std::uint32_t            a_dur,
		float                    a_cost,
		std::vector<std::string> a_conditionList);

	void AddEffectItemToPotion(STATIC_ARGS, RE::AlchemyItem* a_potion, RE::AlchemyItem* a_copyPotion, std::uint32_t a_index, float a_cost);

	void RemoveMagicEffectFromPotion(STATIC_ARGS,
		RE::AlchemyItem*   a_potion,
		RE::EffectSetting* a_mgef,
		float              a_mag,
		std::uint32_t      a_area,
		std::uint32_t      a_dur,
		float              a_cost);

	void RemoveEffectItemFromPotion(STATIC_ARGS, RE::AlchemyItem* a_potion, RE::AlchemyItem* a_copyPotion, std::uint32_t a_index);
	void SetPotionMagicEffect(STATIC_ARGS, RE::AlchemyItem* a_potion, RE::EffectSetting* a_magicEffect, std::uint32_t a_index);

	void Bind(VM& a_vm);
}
