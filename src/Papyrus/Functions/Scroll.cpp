#include "Papyrus/Functions/Scroll.h"

#include "Papyrus/Util/Magic.h"

namespace Papyrus::Scroll
{
	void AddMagicEffectToScroll(STATIC_ARGS, RE::ScrollItem* a_scroll, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost, std::vector<std::string> a_conditionList)
	{
		if (!a_scroll) {
			a_vm->TraceStack("Scroll is None", a_stackID);
			return;
		}

		MAGIC::AddMagicEffect(a_vm, a_stackID, a_scroll, a_mgef, a_mag, a_area, a_dur, a_cost, std::move(a_conditionList));
	}

	void AddEffectItemToScroll(STATIC_ARGS, RE::ScrollItem* a_scroll, RE::ScrollItem* a_copyScroll, std::uint32_t a_index, float a_cost)
	{
		if (!a_scroll) {
			a_vm->TraceStack("Scroll is None", a_stackID);
			return;
		}

		MAGIC::AddEffectItem(a_vm, a_stackID, a_scroll, a_copyScroll, a_index, a_cost);
	}

	void RemoveMagicEffectFromScroll(STATIC_ARGS, RE::ScrollItem* a_scroll, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost)
	{
		if (!a_scroll) {
			a_vm->TraceStack("Scroll is None", a_stackID);
			return;
		}

		MAGIC::RemoveMagicEffect(a_vm, a_stackID, a_scroll, a_mgef, a_mag, a_area, a_dur, a_cost);
	}

	void RemoveEffectItemFromScroll(STATIC_ARGS, RE::ScrollItem* a_scroll, RE::ScrollItem* a_copyScroll, std::uint32_t a_index)
	{
		if (!a_scroll) {
			a_vm->TraceStack("Scroll is None", a_stackID);
			return;
		}

		MAGIC::RemoveEffectItem(a_vm, a_stackID, a_scroll, a_copyScroll, a_index);
	}

	void SetScrollMagicEffect(STATIC_ARGS, RE::ScrollItem* a_scroll, RE::EffectSetting* a_magicEffect, std::uint32_t a_index)
	{
		if (!a_scroll) {
			a_vm->TraceStack("Scroll is None", a_stackID);
			return;
		}

		MAGIC::SetMagicEffect(a_vm, a_stackID, a_scroll, a_magicEffect, a_index);
	}

	void Bind(VM& a_vm)
	{
		BIND(AddMagicEffectToScroll);
		BIND(AddEffectItemToScroll);
		BIND(RemoveMagicEffectFromScroll);
		BIND(RemoveEffectItemFromScroll);
		BIND(SetScrollMagicEffect);

		logger::info("Registered scroll functions"sv);
	}
}
