#include "Papyrus/Functions/Spell.h"

#include "Papyrus/Util/Magic.h"

namespace Papyrus::Spell
{
	void AddMagicEffectToSpell(STATIC_ARGS, RE::SpellItem* a_spell, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost, std::vector<std::string> a_conditionList)
	{
		if (!a_spell) {
			a_vm->TraceStack("Spell is None", a_stackID);
			return;
		}

		MAGIC::AddMagicEffect(a_vm, a_stackID, a_spell, a_mgef, a_mag, a_area, a_dur, a_cost, std::move(a_conditionList));
	}

	void AddEffectItemToSpell(STATIC_ARGS, RE::SpellItem* a_spell, RE::SpellItem* a_copySpell, std::uint32_t a_index, float a_cost)
	{
		if (!a_spell) {
			a_vm->TraceStack("Spell is None", a_stackID);
			return;
		}

		MAGIC::AddEffectItem(a_vm, a_stackID, a_spell, a_copySpell, a_index, a_cost);
	}

	std::int32_t GetSpellType(STATIC_ARGS, const RE::SpellItem* a_spell)
	{
		if (!a_spell) {
			a_vm->TraceStack("Spell is None", a_stackID);
			return -1;
		}

		return std::to_underlying(a_spell->data.spellType);
	}

	void RemoveMagicEffectFromSpell(STATIC_ARGS, RE::SpellItem* a_spell, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost)
	{
		if (!a_spell) {
			a_vm->TraceStack("Spell is None", a_stackID);
			return;
		}

		MAGIC::RemoveMagicEffect(a_vm, a_stackID, a_spell, a_mgef, a_mag, a_area, a_dur, a_cost);
	}

	void RemoveEffectItemFromSpell(STATIC_ARGS, RE::SpellItem* a_spell, RE::SpellItem* a_copySpell, std::uint32_t a_index)
	{
		if (!a_spell) {
			a_vm->TraceStack("Spell is None", a_stackID);
			return;
		}

		MAGIC::RemoveEffectItem(a_vm, a_stackID, a_spell, a_copySpell, a_index);
	}

	void SetSpellCastingType(STATIC_ARGS, RE::SpellItem* a_spell, std::uint32_t a_type)
	{
		if (!a_spell) {
			a_vm->TraceStack("Spell is None", a_stackID);
			return;
		}

		const auto type = static_cast<RE::MagicSystem::CastingType>(a_type);
		if (a_spell->GetCastingType() == type) {
			return;
		}

		a_spell->SetCastingType(type);
		for (const auto& effect : a_spell->effects) {
			if (effect && effect->baseEffect) {
				effect->baseEffect->data.castingType = type;
			}
		}
	}

	void SetSpellDeliveryType(STATIC_ARGS, RE::SpellItem* a_spell, std::uint32_t a_type)
	{
		if (!a_spell) {
			a_vm->TraceStack("Spell is None", a_stackID);
			return;
		}

		const auto type = static_cast<RE::MagicSystem::Delivery>(a_type);
		if (a_spell->GetDelivery() == type) {
			return;
		}

		a_spell->SetDelivery(type);
		for (const auto& effect : a_spell->effects) {
			if (effect && effect->baseEffect) {
				effect->baseEffect->data.delivery = type;
			}
		}
	}

	void SetSpellType(STATIC_ARGS, RE::SpellItem* a_spell, std::uint32_t a_type)
	{
		if (!a_spell) {
			a_vm->TraceStack("Spell is None", a_stackID);
			return;
		}

		const auto type = static_cast<RE::MagicSystem::SpellType>(a_type);
		if (a_spell->GetSpellType() == type) {
			return;
		}

		a_spell->data.spellType = type;
	}

	void SetSpellMagicEffect(STATIC_ARGS, RE::SpellItem* a_spell, RE::EffectSetting* a_magicEffect, std::uint32_t a_index)
	{
		if (!a_spell) {
			a_vm->TraceStack("Spell is None", a_stackID);
			return;
		}

		MAGIC::SetMagicEffect(a_vm, a_stackID, a_spell, a_magicEffect, a_index);
	}

	void Bind(VM& a_vm)
	{
		std::uint32_t count = 0;

		BIND(AddMagicEffectToSpell);
		BIND(AddEffectItemToSpell);
		BIND(GetSpellType);
		BIND(RemoveMagicEffectFromSpell);
		BIND(RemoveEffectItemFromSpell);
		BIND(SetSpellCastingType);
		BIND(SetSpellDeliveryType);
		BIND(SetSpellType);
		BIND(SetSpellMagicEffect);

		logger::info("Registered {} spell functions"sv, count);
	}
}
