#pragma once

namespace Papyrus::Scroll
{
	inline void AddMagicEffectToScroll(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::ScrollItem* a_scroll,
		RE::EffectSetting* a_mgef,
		float a_mag,
		std::uint32_t a_area,
		std::uint32_t a_dur,
		float a_cost,
        std::vector<RE::BSFixedString> a_conditionList)
	{
		if (!a_scroll) {
			a_vm->TraceStack("Scroll is None", a_stackID);
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

        const auto result = MAGIC::add_magic_effect(
			a_scroll, a_mgef, a_mag, a_area, a_dur, a_cost,
			a_conditionList);
		if (result == MAGIC::RESULT::kFailParse) {
			a_vm->TraceForm(a_scroll, "failed to parse condition list", a_stackID);
		} else if (result == MAGIC::RESULT::kFailExists) {
			a_vm->TraceForm(a_scroll, "effect already exists", a_stackID);
		}
	}

	inline void AddEffectItemToScroll(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		RE::ScrollItem* a_scroll, 
		RE::ScrollItem* a_copyScroll, 
		std::uint32_t a_index, 
		float a_cost)
	{
		if (!a_scroll) {
			a_vm->TraceStack("Scroll is None", a_stackID);
			return;
		}
		if (!a_copyScroll) {
			a_vm->TraceStack("Copy Scroll is None", a_stackID);
			return;
		}
		if (a_index > a_copyScroll->effects.size()) {
			a_vm->TraceForm(a_scroll, "Copy Scroll index exceeds effect list size", a_stackID);
			return;
		}

		MAGIC::add_effect_item(a_scroll, a_scroll, a_index, a_cost);
	}

	inline void RemoveMagicEffectFromScroll(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::ScrollItem* a_scroll,
		RE::EffectSetting* a_mgef,
		float a_mag,
		std::uint32_t a_area,
		std::uint32_t a_dur,
		float a_cost)
	{
		if (!a_scroll) {
			a_vm->TraceStack("Scroll is None", a_stackID);
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

		MAGIC::remove_magic_effect(a_scroll, a_mgef, a_mag, a_area, a_dur, a_cost);
	}

	inline void RemoveEffectItemFromScroll(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		RE::ScrollItem* a_scroll, 
		RE::ScrollItem* a_copyScroll,
		std::uint32_t a_index)
	{
		if (!a_scroll) {
			a_vm->TraceStack("Scroll is None", a_stackID);
			return;
		}
		if (!a_copyScroll) {
			a_vm->TraceStack("Copy Scroll is None", a_stackID);
			return;
		}
		if (a_index > a_copyScroll->effects.size()) {
			a_vm->TraceStack("Copy Scroll index exceeds effect list size", a_stackID);
			return;
		}

		MAGIC::remove_effect_item(a_scroll, a_copyScroll, a_index);
	}

	inline void Bind(VM& a_vm)
	{
		BIND(AddMagicEffectToScroll);
		BIND(AddEffectItemToScroll);
		BIND(RemoveMagicEffectFromScroll);
		BIND(RemoveEffectItemFromScroll);

		logger::info("Registered scroll functions"sv);
	}
}
