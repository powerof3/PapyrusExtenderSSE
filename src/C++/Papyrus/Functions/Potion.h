#pragma once

#include "Serialization/Services.h"

namespace Papyrus::Potion
{
	inline void AddMagicEffectToPotion(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::AlchemyItem* a_potion,
		RE::EffectSetting* a_mgef,
		float a_mag,
		std::uint32_t a_area,
		std::uint32_t a_dur,
		float a_cost,
        std::vector<std::string> a_conditionList)
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

		MAGIC::MGEFData data = {
			std::make_pair(a_mgef, a_mgef->GetFormID()),
			a_mag,
			a_area,
			a_dur,
			a_cost,
			a_conditionList
		};

		MAGIC::MGEFManager::GetSingleton()->Add(a_potion, data);
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

		MAGIC::EffectData data = {
			std::make_pair(a_copyPotion, a_copyPotion->GetFormID()),
			a_index,
			a_cost
		};

		MAGIC::EffectManager::GetSingleton()->Add(a_potion, data);
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

		MAGIC::MGEFData data = {
			std::make_pair(a_mgef, a_mgef->GetFormID()),
			a_mag,
			a_area,
			a_dur,
			a_cost,
			std::vector<std::string>()
		};

		MAGIC::MGEFManager::GetSingleton()->Remove(a_potion, data);
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

		MAGIC::EffectData data = {
			std::make_pair(a_copyPotion, a_copyPotion->GetFormID()),
			a_index,
			-1.0f
		};

		MAGIC::EffectManager::GetSingleton()->Remove(a_potion, data);
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
