#pragma once

namespace Papyrus::LeveledList
{
	std::vector<RE::TESForm*> GetContentFromLeveledActor(STATIC_ARGS, RE::TESLevCharacter* a_lvlActor, const RE::TESObjectREFR* a_refr);
	std::vector<RE::TESForm*> GetContentFromLeveledItem(STATIC_ARGS, RE::TESLevItem* a_lvlItem, const RE::TESObjectREFR* a_refr);
	std::vector<RE::TESForm*> GetContentFromLeveledSpell(STATIC_ARGS, RE::TESLevSpell* a_lvlSpell, const RE::TESObjectREFR* a_refr);

	void Bind(VM& a_vm);
}
