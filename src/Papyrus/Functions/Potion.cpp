#include "Papyrus/Functions/Potion.h"

#include "Papyrus/Util/Magic.h"

namespace Papyrus::Potion
{
	void AddMagicEffectToPotion(STATIC_ARGS, RE::AlchemyItem* a_potion, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost, std::vector<std::string> a_conditionList)
	{
		if (!a_potion) {
			a_vm->TraceStack("Potion is None", a_stackID);
			return;
		}

		MAGIC::AddMagicEffect(a_vm, a_stackID, a_potion, a_mgef, a_mag, a_area, a_dur, a_cost, std::move(a_conditionList));
	}

	void AddEffectItemToPotion(STATIC_ARGS, RE::AlchemyItem* a_potion, RE::AlchemyItem* a_copyPotion, std::uint32_t a_index, float a_cost)
	{
		if (!a_potion) {
			a_vm->TraceStack("Potion is None", a_stackID);
			return;
		}

		MAGIC::AddEffectItem(a_vm, a_stackID, a_potion, a_copyPotion, a_index, a_cost);
	}

	void RemoveMagicEffectFromPotion(STATIC_ARGS, RE::AlchemyItem* a_potion, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost)
	{
		if (!a_potion) {
			a_vm->TraceStack("Potion is None", a_stackID);
			return;
		}

		MAGIC::RemoveMagicEffect(a_vm, a_stackID, a_potion, a_mgef, a_mag, a_area, a_dur, a_cost);
	}

	void RemoveEffectItemFromPotion(STATIC_ARGS, RE::AlchemyItem* a_potion, RE::AlchemyItem* a_copyPotion, std::uint32_t a_index)
	{
		if (!a_potion) {
			a_vm->TraceStack("Potion is None", a_stackID);
			return;
		}

		MAGIC::RemoveEffectItem(a_vm, a_stackID, a_potion, a_copyPotion, a_index);
	}

	void SetPotionMagicEffect(STATIC_ARGS, RE::AlchemyItem* a_potion, RE::EffectSetting* a_magicEffect, std::uint32_t a_index)
	{
		if (!a_potion) {
			a_vm->TraceStack("Potion is None", a_stackID);
			return;
		}

		MAGIC::SetMagicEffect(a_vm, a_stackID, a_potion, a_magicEffect, a_index);
	}

	void Bind(VM& a_vm)
	{
		std::uint32_t count = 0;
		
		BIND(AddMagicEffectToPotion);
		BIND(AddEffectItemToPotion);
		BIND(RemoveMagicEffectFromPotion);
		BIND(RemoveEffectItemFromPotion);
		BIND(SetPotionMagicEffect);

		logger::info("Registered {} potion functions"sv, count);
	}
}
