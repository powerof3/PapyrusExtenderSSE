#pragma once

namespace Papyrus::Scroll
{
	void AddMagicEffectToScroll(STATIC_ARGS,
		RE::ScrollItem*          a_scroll,
		RE::EffectSetting*       a_mgef,
		float                    a_mag,
		std::uint32_t            a_area,
		std::uint32_t            a_dur,
		float                    a_cost,
		std::vector<std::string> a_conditionList);

	void AddEffectItemToScroll(STATIC_ARGS, RE::ScrollItem* a_scroll, RE::ScrollItem* a_copyScroll, std::uint32_t a_index, float a_cost);

	void RemoveMagicEffectFromScroll(STATIC_ARGS,
		RE::ScrollItem*    a_scroll,
		RE::EffectSetting* a_mgef,
		float              a_mag,
		std::uint32_t      a_area,
		std::uint32_t      a_dur,
		float              a_cost);

	void RemoveEffectItemFromScroll(STATIC_ARGS, RE::ScrollItem* a_scroll, RE::ScrollItem* a_copyScroll, std::uint32_t a_index);
	void SetScrollMagicEffect(STATIC_ARGS, RE::ScrollItem* a_scroll, RE::EffectSetting* a_magicEffect, std::uint32_t a_index);

	void Bind(VM& a_vm);
}
