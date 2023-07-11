#pragma once

#include "Serialization/Services.h"

namespace Papyrus::Scroll
{
	inline void AddMagicEffectToScroll(STATIC_ARGS,
		RE::ScrollItem*          a_scroll,
		RE::EffectSetting*       a_mgef,
		float                    a_mag,
		std::uint32_t            a_area,
		std::uint32_t            a_dur,
		float                    a_cost,
		std::vector<std::string> a_conditionList)
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

		MAGIC::MGEFData data{
			std::make_pair(a_mgef, a_mgef->GetFormID()),
			a_mag,
			a_area,
			a_dur,
			a_cost,
			std::move(a_conditionList)
		};

		if (MAGIC::MGEFManager::GetSingleton()->Add(a_scroll, data)) {
			a_vm->TraceForm(a_scroll, "Failed to add magic effect", a_stackID);
		}
	}

	inline void AddEffectItemToScroll(STATIC_ARGS, RE::ScrollItem* a_scroll, RE::ScrollItem* a_copyScroll, std::uint32_t a_index, float a_cost)
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

		MAGIC::EffectData data{
			std::make_pair(a_copyScroll, a_copyScroll->GetFormID()),
			a_index,
			a_cost
		};

		if (MAGIC::EffectManager::GetSingleton()->Add(a_scroll, data)) {
			a_vm->TraceForm(a_scroll, "Failed to add magic effect", a_stackID);
		}
	}

	inline void RemoveMagicEffectFromScroll(STATIC_ARGS,
		RE::ScrollItem*    a_scroll,
		RE::EffectSetting* a_mgef,
		float              a_mag,
		std::uint32_t      a_area,
		std::uint32_t      a_dur,
		float              a_cost)
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

		MAGIC::MGEFData data{
			std::make_pair(a_mgef, a_mgef->GetFormID()),
			a_mag,
			a_area,
			a_dur,
			a_cost,
			std::vector<std::string>()
		};

		if (MAGIC::MGEFManager::GetSingleton()->Remove(a_scroll, data)) {
			a_vm->TraceForm(a_scroll, "Failed to remove magic effect", a_stackID);
		}
	}

	inline void RemoveEffectItemFromScroll(STATIC_ARGS, RE::ScrollItem* a_scroll, RE::ScrollItem* a_copyScroll, std::uint32_t a_index)
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

		MAGIC::EffectData data{
			std::make_pair(a_copyScroll, a_copyScroll->GetFormID()),
			a_index,
			-1.0f
		};

		if (MAGIC::EffectManager::GetSingleton()->Remove(a_scroll, data)) {
			a_vm->TraceForm(a_scroll, "Failed to remove magic effect", a_stackID);
		}
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
