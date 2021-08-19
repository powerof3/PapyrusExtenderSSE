#pragma once

namespace Papyrus::Spell
{
	inline void AddMagicEffectToSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::SpellItem* a_spell,
		RE::EffectSetting* a_mgef,
		float a_mag,
		std::uint32_t a_area,
		std::uint32_t a_dur,
		float a_cost,
        std::vector<RE::BSFixedString> a_conditionList)
	{
		if (!a_spell) {
			a_vm->TraceStack("Spell is None", a_stackID);
			return;
		}
		if (!a_mgef) {
			a_vm->TraceStack("MagicEffect is None", a_stackID);
			return;
		}
		if (a_mgef->data.castingType != a_spell->data.castingType) {
			a_vm->TraceForm(a_mgef, "Casting types don't match", a_stackID);
			return;
		}
		if (a_mgef->data.delivery != a_spell->data.delivery) {
			a_vm->TraceForm(a_mgef, "Delivery types don't match", a_stackID);
			return;
		}

        const auto result = MAGIC::add_magic_effect(
			a_spell, a_mgef, a_mag, a_area, a_dur, a_cost,
			a_conditionList);
		if (result == MAGIC::RESULT::kFailParse) {
			a_vm->TraceForm(a_spell, "failed to parse condition list", a_stackID);
		} else if (result == MAGIC::RESULT::kFailExists) {
			a_vm->TraceForm(a_spell, "effect already exists", a_stackID);
		}
	}

	inline void AddEffectItemToSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::SpellItem* a_spell,
		RE::SpellItem* a_copySpell,
		std::uint32_t a_index,
		float a_cost)
	{
		if (!a_spell) {
			a_vm->TraceStack("Spell is None", a_stackID);
			return;
		}
		if (!a_copySpell) {
			a_vm->TraceStack("Copy Spell is None", a_stackID);
			return;
		}
		if (a_index > a_copySpell->effects.size()) {
			a_vm->TraceForm(a_spell, "Copy Spell index exceeds effect list size", a_stackID);
			return;
		}
		if (a_spell->data.castingType != a_copySpell->data.castingType) {
			a_vm->TraceForm(a_spell, "Casting types don't match", a_stackID);
			return;
		}
		if (a_spell->data.delivery != a_copySpell->data.delivery) {
			a_vm->TraceForm(a_spell, "Delivery types don't match", a_stackID);
			return;
		}

		MAGIC::add_effect_item(a_spell, a_spell, a_index, a_cost);
	}

	inline std::int32_t GetSpellType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::SpellItem* a_spell)
	{
		if (!a_spell) {
			a_vm->TraceStack("Spell is None", a_stackID);
			return -1;
		}

		return stl::to_underlying(a_spell->data.spellType);
	}

	inline void RemoveMagicEffectFromSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::SpellItem* a_spell,
		RE::EffectSetting* a_mgef,
		float a_mag,
		std::uint32_t a_area,
		std::uint32_t a_dur,
		float a_cost)
	{
		if (!a_spell) {
			a_vm->TraceStack("Spell is None", a_stackID);
			return;
		}
		if (!a_mgef) {
			a_vm->TraceStack("MagicEffect is None", a_stackID);
			return;
		}

		MAGIC::remove_magic_effect(a_spell, a_mgef, a_mag, a_area, a_dur, a_cost);
	}

	inline void RemoveEffectItemFromSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::SpellItem* a_spell,
		RE::SpellItem* a_copySpell,
		std::uint32_t a_index)
	{
		if (!a_spell) {
			a_vm->TraceStack("Spell is None", a_stackID);
			return;
		}
		if (!a_copySpell) {
			a_vm->TraceStack("Copy Spell is None", a_stackID);
			return;
		}
		if (a_index > a_copySpell->effects.size()) {
			a_vm->TraceStack("Copy Spell index exceeds effect list size", a_stackID);
			return;
		}

		MAGIC::remove_effect_item(a_spell, a_copySpell, a_index);
	}

	inline void SetSpellCastingType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::SpellItem* a_spell,
		std::uint32_t a_type)
	{
		if (!a_spell) {
			a_vm->TraceStack("Spell is None", a_stackID);
			return;
		}

        const auto type = static_cast<RE::MagicSystem::CastingType>(a_type);
		if (a_spell->GetCastingType() == type) {
			return;
		}

		a_spell->data.castingType = type;
		for (const auto& effect : a_spell->effects) {
			if (const auto baseEffect = effect ? effect->baseEffect : nullptr; baseEffect) {
				baseEffect->data.castingType = type;
			}
		}
	}

	inline void SetSpellDeliveryType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::SpellItem* a_spell,
		std::uint32_t a_type)
	{
		if (!a_spell) {
			a_vm->TraceStack("Spell is None", a_stackID);
			return;
		}

        const auto type = static_cast<RE::MagicSystem::Delivery>(a_type);
		if (a_spell->GetDelivery() == type) {
			return;
		}

		a_spell->data.delivery = type;
		for (const auto& effect : a_spell->effects) {
			if (const auto baseEffect = effect ? effect->baseEffect : nullptr; baseEffect) {
				baseEffect->data.delivery = type;
			}
		}
	}

	inline void Bind(VM& a_vm)
	{
		BIND(AddMagicEffectToSpell);
		BIND(AddEffectItemToSpell);
		BIND(GetSpellType, true);
		BIND(RemoveMagicEffectFromSpell);
		BIND(RemoveEffectItemFromSpell);
		BIND(SetSpellCastingType);
		BIND(SetSpellDeliveryType);

		logger::info("Registered spell functions"sv);
	}
}
