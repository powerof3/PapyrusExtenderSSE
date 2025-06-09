#include "Papyrus/Functions/LeveledList.h"

namespace Papyrus::LeveledList
{
	struct detail
	{
		static std::vector<RE::TESForm*> GetContentFromLeveledList(RE::TESLeveledList* a_lvlList, const RE::TESObjectREFR* a_refr)
		{
			std::vector<RE::TESForm*> forms;

			RE::BSScrapArray<RE::CALCED_OBJECT> calcedObjects{};
			a_lvlList->CalculateCurrentFormList(a_refr->GetCalcLevel(false), 1, calcedObjects, 0, true);
			for (auto& calcedObj : calcedObjects) {
				forms.push_back(calcedObj.form);
			}

			return forms;
		}
	};

	std::vector<RE::TESForm*> GetContentFromLeveledItem(STATIC_ARGS, RE::TESLevItem* a_lvlItem, const RE::TESObjectREFR* a_refr)
	{
		if (!a_lvlItem) {
			a_vm->TraceStack("Leveled Item is None", a_stackID);
			return {};
		}

		if (!a_refr) {
			a_vm->TraceStack("Reference is None", a_stackID);
			return {};
		}

		return detail::GetContentFromLeveledList(a_lvlItem, a_refr)
	}

	std::vector<RE::TESForm*> GetContentFromLeveledNPC(STATIC_ARGS, RE::TESLevCharacter* a_lvlActor, const RE::TESObjectREFR* a_refr)
	{
		if (!a_lvlActor) {
			a_vm->TraceStack("Leveled NPC is None", a_stackID);
			return {};
		}

		if (!a_refr) {
			a_vm->TraceStack("Reference is None", a_stackID);
			return {};
		}

		return detail::GetContentFromLeveledList(a_lvlActor, a_refr);
	}

	std::vector<RE::TESForm*> GetContentFromLeveledSpell(STATIC_ARGS, RE::TESLevSpell* a_lvlSpell, const RE::TESObjectREFR* a_refr)
	{
		if (!a_lvlSpell) {
			a_vm->TraceStack("Leveled Spell is None", a_stackID);
			return {};
		}

		if (!a_refr) {
			a_vm->TraceStack("Reference is None", a_stackID);
			return {};
		}

		return detail::GetContentFromLeveledList(a_lvlSpell, a_refr);
	}

	void Bind(VM& a_vm)
	{
		BIND(GetContentFromLeveledItem);
		BIND(GetContentFromLeveledNPC);
		BIND(GetContentFromLeveledSpell);

		logger::info("Registered leveled item functions"sv);
	}
}
