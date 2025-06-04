#include "Papyrus/Util/Magic.h"

void MAGIC::RemoveMagicEffect(VM* a_vm, StackID a_stackID, RE::MagicItem* a_spell, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost)
{
	if (!a_mgef) {
		a_vm->TraceStack("MagicEffect is None", a_stackID);
		return;
	}

	MGEFData data(a_mgef,a_mag,a_area,a_dur,a_cost);
	if (Manager::GetSingleton()->mgefs.Remove(a_spell, data)) {
		a_vm->TraceForm(a_spell, "Failed to remove magic effect", a_stackID);
	}
}

void MAGIC::RemoveEffectItem(VM* a_vm, StackID a_stackID, RE::MagicItem* a_spell, RE::MagicItem* a_copySpell, std::uint32_t a_index, float a_cost)
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

	EffectData data(a_copySpell, a_index, a_cost);
	if (Manager::GetSingleton()->effectItems.Remove(a_spell, data)) {
		a_vm->TraceForm(a_spell, "Failed to remove magic effect", a_stackID);
	}
}

void MAGIC::SetMagicEffect(VM* a_vm, StackID a_stackID, RE::MagicItem* a_spell, RE::EffectSetting* a_magicEffect, std::uint32_t a_index)
{
	if (!a_magicEffect) {
		a_vm->TraceStack("MagicEffect is None", a_stackID);
		return;
	}

	if (a_index > a_spell->effects.size()) {
		a_vm->TraceForm(a_spell, "Index exceeds effect list size", a_stackID);
		return;
	}

	if (auto effectItem = a_spell->effects[a_index]) {
		effectItem->baseEffect = a_magicEffect;
	}
}
