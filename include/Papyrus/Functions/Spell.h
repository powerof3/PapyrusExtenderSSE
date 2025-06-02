#pragma once

namespace Papyrus::Spell
{
	void AddMagicEffectToSpell(STATIC_ARGS,
		RE::SpellItem*           a_spell,
		RE::EffectSetting*       a_mgef,
		float                    a_mag,
		std::uint32_t            a_area,
		std::uint32_t            a_dur,
		float                    a_cost,
		std::vector<std::string> a_conditionList);

	void         AddEffectItemToSpell(STATIC_ARGS, RE::SpellItem* a_spell, RE::SpellItem* a_copySpell, std::uint32_t a_index, float a_cost);
	std::int32_t GetSpellType(STATIC_ARGS, const RE::SpellItem* a_spell);

	void RemoveMagicEffectFromSpell(STATIC_ARGS,
		RE::SpellItem*     a_spell,
		RE::EffectSetting* a_mgef,
		float              a_mag,
		std::uint32_t      a_area,
		std::uint32_t      a_dur,
		float              a_cost);

	void RemoveEffectItemFromSpell(STATIC_ARGS, RE::SpellItem* a_spell, RE::SpellItem* a_copySpell, std::uint32_t a_index);
	void SetSpellCastingType(STATIC_ARGS, RE::SpellItem* a_spell, std::uint32_t a_type);
	void SetSpellDeliveryType(STATIC_ARGS, RE::SpellItem* a_spell, std::uint32_t a_type);
	void SetSpellType(STATIC_ARGS, RE::SpellItem* a_spell, std::uint32_t a_type);
	void SetSpellMagicEffect(STATIC_ARGS, RE::SpellItem* a_spell, RE::EffectSetting* a_magicEffect, std::uint32_t a_index);

	void Bind(VM& a_vm);
}
