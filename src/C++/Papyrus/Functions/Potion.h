#pragma once

namespace Papyrus::Potion
{
	inline void AddMagicEffectToPotion(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::AlchemyItem* a_potion,
		RE::EffectSetting* a_mgef,
		float a_mag,
		std::uint32_t a_area,
		std::uint32_t a_dur,
		float a_cost,
        std::vector<RE::BSFixedString> a_conditionList)
	{
		if (!a_potion) {
			a_vm->TraceStack("Potion is None", a_stackID);
			return;
		}
		if (!a_mgef) {
			a_vm->TraceStack("MagicEffect is None", a_stackID);
			return;
		}
		if (a_mgef->data.castingType != RE::MagicSystem::CastingType::kFireAndForget) {
			a_vm->TraceForm(a_mgef, "Casting type isn't fire and forget", a_stackID);
			return;
		}

		auto result = MAGIC::add_magic_effect(
			a_potion, a_mgef, a_mag, a_area, a_dur, a_cost,
			a_conditionList);
		if (result == MAGIC::RESULT::kFailParse) {
			a_vm->TraceForm(a_potion, "failed to parse condition list", a_stackID);
		} else if (result == MAGIC::RESULT::kFailExists) {
			a_vm->TraceForm(a_potion, "effect already exists", a_stackID);
		}
	}

	inline void AddEffectItemToPotion(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::AlchemyItem* a_potion,
		RE::AlchemyItem* a_copyPotion,
		std::uint32_t a_index,
		float a_cost)
	{
		if (!a_potion) {
			a_vm->TraceStack("Potion is None", a_stackID);
			return;
		}
		if (!a_copyPotion) {
			a_vm->TraceStack("Copy Potion is None", a_stackID);
			return;
		}
		if (a_index > a_copyPotion->effects.size()) {
			a_vm->TraceForm(a_copyPotion, "Copy Potion index exceeds effect list size", a_stackID);
			return;
		}

		MAGIC::add_effect_item(a_potion, a_copyPotion, a_index, a_cost);
	}

	inline void RemoveMagicEffectFromPotion(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::AlchemyItem* a_potion,
		RE::EffectSetting* a_mgef,
		float a_mag,
		std::uint32_t a_area,
		std::uint32_t a_dur,
		float a_cost)
	{
		if (!a_potion) {
			a_vm->TraceStack("Potion is None", a_stackID);
			return;
		}
		if (!a_mgef) {
			a_vm->TraceStack("MagicEffect is None", a_stackID);
			return;
		}
		if (a_mgef->data.castingType != RE::MagicSystem::CastingType::kFireAndForget) {
			a_vm->TraceForm(a_mgef, "Casting type isn't fire and forget", a_stackID);
			return;
		}

		MAGIC::remove_magic_effect(a_potion, a_mgef, a_mag, a_area, a_dur, a_cost);
	}

	inline void RemoveEffectItemFromPotion(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::AlchemyItem* a_potion,
		RE::AlchemyItem* a_copyPotion,
		std::uint32_t a_index)
	{
		if (!a_potion) {
			a_vm->TraceStack("Potion is None", a_stackID);
			return;
		}
		if (!a_copyPotion) {
			a_vm->TraceStack("Copy Potion is None", a_stackID);
			return;
		}
		if (a_index > a_copyPotion->effects.size()) {
			a_vm->TraceForm(a_copyPotion, "Copy Potion index exceeds effect list size", a_stackID);
			return;
		}

		MAGIC::remove_effect_item(a_potion, a_copyPotion, a_index);
	}

	inline void Bind(VM& a_vm)
	{
		BIND(AddMagicEffectToPotion);
		BIND(AddEffectItemToPotion);
		BIND(RemoveMagicEffectFromPotion);
		BIND(RemoveEffectItemFromPotion);

		logger::info("Registered potion functions"sv);
	}
}
