#include "Papyrus/Functions/Enchantment.h"

#include "Papyrus/Util/Magic.h"

namespace Papyrus::Enchantment
{
	void AddMagicEffectToEnchantment(STATIC_ARGS, RE::EnchantmentItem* a_enchantment, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost, std::vector<std::string> a_conditionList)
	{
		if (!a_enchantment) {
			a_vm->TraceStack("Enchantment is None", a_stackID);
			return;
		}

		MAGIC::AddMagicEffect(a_vm, a_stackID, a_enchantment, a_mgef, a_mag, a_area, a_dur, a_cost, std::move(a_conditionList));
	}

	std::int32_t GetEnchantmentType(STATIC_ARGS, RE::EnchantmentItem* a_enchantment)
	{
		if (!a_enchantment) {
			a_vm->TraceStack("Enchantment is None", a_stackID);
			return -1;
		}

		return std::to_underlying(a_enchantment->data.spellType);
	}

	void AddEffectItemToEnchantment(STATIC_ARGS, RE::EnchantmentItem* a_enchantment, RE::EnchantmentItem* a_copyEnchantment, std::uint32_t a_index, float a_cost)
	{
		if (!a_enchantment) {
			a_vm->TraceStack("Enchantment is None", a_stackID);
			return;
		}

		MAGIC::AddEffectItem(a_vm, a_stackID, a_enchantment, a_copyEnchantment, a_index, a_cost);
	}

	void RemoveMagicEffectFromEnchantment(STATIC_ARGS, RE::EnchantmentItem* a_enchantment, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost)
	{
		if (!a_enchantment) {
			a_vm->TraceStack("Enchantment is None", a_stackID);
			return;
		}

		MAGIC::RemoveMagicEffect(a_vm, a_stackID, a_enchantment, a_mgef, a_mag, a_area, a_dur, a_cost);
	}

	void RemoveEffectItemFromEnchantment(STATIC_ARGS, RE::EnchantmentItem* a_enchantment, RE::EnchantmentItem* a_copyEnchantment, std::uint32_t a_index)
	{
		if (!a_enchantment) {
			a_vm->TraceStack("Enchantment is None", a_stackID);
			return;
		}

		MAGIC::RemoveEffectItem(a_vm, a_stackID, a_enchantment, a_copyEnchantment, a_index);
	}

	void SetEnchantmentMagicEffect(STATIC_ARGS, RE::EnchantmentItem* a_enchantment, RE::EffectSetting* a_magicEffect, std::uint32_t a_index)
	{
		if (!a_enchantment) {
			a_vm->TraceStack("Enchantment is None", a_stackID);
			return;
		}

		MAGIC::SetMagicEffect(a_vm, a_stackID, a_enchantment, a_magicEffect, a_index);
	}

	void Bind(VM& a_vm)
	{
		std::uint32_t count = 0;
		
		BIND(AddMagicEffectToEnchantment);
		BIND(AddEffectItemToEnchantment);
		BIND(GetEnchantmentType);
		BIND(RemoveMagicEffectFromEnchantment);
		BIND(RemoveEffectItemFromEnchantment);
		BIND(SetEnchantmentMagicEffect);

		logger::info("Registered {} enchantment functions"sv, count);
	}
}
