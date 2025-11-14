#pragma once

#include "Serialization/Manager.h"

namespace RE
{
	std::string_view EffectArchetypeToString(EffectArchetype a_archetype) noexcept;
}

namespace MAGIC
{
	using namespace Serialization;

	template <class T>
	void AddMagicEffect(VM* a_vm, StackID a_stackID, T* a_spell, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost, std::vector<std::string> a_conditionList)
	{
		if (!a_mgef) {
			a_vm->TraceStack("MagicEffect is None", a_stackID);
			return;
		}

		if constexpr (std::is_same_v<T, RE::AlchemyItem>) {
			if (a_mgef->data.castingType != RE::MagicSystem::CastingType::kFireAndForget) {
				a_vm->TraceForm(a_mgef, "Casting type isn't fire and forget", a_stackID);
				return;
			}
		} else {
			if (a_mgef->data.castingType != a_spell->GetCastingType()) {
				a_vm->TraceForm(a_mgef, "Casting types don't match", a_stackID);
				return;
			}
			if (a_mgef->data.delivery != a_spell->GetDelivery()) {
				a_vm->TraceForm(a_mgef, "Delivery types don't match", a_stackID);
				return;
			}
		}

		MGEFData data(a_mgef, a_mag, a_area, a_dur, a_cost, a_conditionList);
		if (!Manager::GetSingleton()->mgefs.Add(a_spell, data)) {
			a_vm->TraceForm(a_spell, "Failed to add magic effect", a_stackID);
		}
	}

	template <class T>
	void AddEffectItem(VM* a_vm, StackID a_stackID, T* a_spell, T* a_copySpell, std::uint32_t a_index, float a_cost)
	{
		if (!a_copySpell) {
			a_vm->TraceStack("Copy Item is None", a_stackID);
			return;
		}
		if (a_index > a_copySpell->effects.size()) {
			a_vm->TraceForm(a_spell, "Copy Item index exceeds effect list size", a_stackID);
			return;
		}

		if constexpr (std::is_convertible_v<T, RE::SpellItem>) {
			if (a_spell->GetCastingType() != a_copySpell->GetCastingType()) {
				a_vm->TraceForm(a_spell, "Casting types don't match", a_stackID);
				return;
			}
			if (a_spell->GetDelivery() != a_copySpell->GetDelivery()) {
				a_vm->TraceForm(a_spell, "Delivery types don't match", a_stackID);
				return;
			}
		}

		EffectData data{ a_copySpell, a_index, a_cost };
		if (Manager::GetSingleton()->effectItems.Add(a_spell, data)) {
			a_vm->TraceForm(a_spell, "Failed to add magic effect", a_stackID);
		}
	}

	void RemoveMagicEffect(VM* a_vm, StackID a_stackID, RE::MagicItem* a_spell, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost);
	void RemoveEffectItem(VM* a_vm, StackID a_stackID, RE::MagicItem* a_spell, RE::MagicItem* a_copySpell, std::uint32_t a_index, float a_cost = -1.0f);
	void SetMagicEffect(VM* a_vm, StackID a_stackID, RE::MagicItem* a_spell, RE::EffectSetting* a_magicEffect, std::uint32_t a_index);
}
