#pragma once

namespace Papyrus::LeveledItem
{
	std::vector<RE::TESForm*> GetContentFromLeveledItem(STATIC_ARGS, RE::TESLevItem* a_lvlItem, const RE::TESObjectREFR* a_refr);

	void Bind(VM& a_vm);
}
