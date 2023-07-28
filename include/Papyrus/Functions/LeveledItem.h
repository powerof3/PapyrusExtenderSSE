#pragma once

namespace Papyrus::LeveledItem
{
	inline std::vector<RE::TESForm*> GetContentFromLeveledItem(STATIC_ARGS, RE::TESLevItem* a_lvlItem, const RE::TESObjectREFR* a_refr)
	{
		if (!a_lvlItem) {
			a_vm->TraceStack("Leveled Item is None", a_stackID);
			return {};
		}

		if (!a_refr) {
			a_vm->TraceStack("Reference is None", a_stackID);
			return {};
		}

		std::vector<RE::TESForm*> forms;

		RE::BSScrapArray<RE::CALCED_OBJECT> calcedObjects{};
		a_lvlItem->CalculateCurrentFormList(a_refr->GetCalcLevel(false), 1, calcedObjects, 0, true);
		for (auto& calcedObj : calcedObjects) {
			forms.push_back(calcedObj.form);
		}

		return forms;
	}

	inline void Bind(VM& a_vm)
	{
		BIND(GetContentFromLeveledItem);

		logger::info("Registered leveled item functions"sv);
	}
}
