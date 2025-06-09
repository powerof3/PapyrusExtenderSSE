#pragma once

namespace Papyrus::Quest
{
	std::vector<std::uint32_t> GetAllQuestObjectives(STATIC_ARGS, RE::TESQuest* a_quest);
	std::vector<std::uint32_t> GetAllQuestStages(STATIC_ARGS, RE::TESQuest* a_quest);
	void                       SetObjectiveText(STATIC_ARGS, RE::TESQuest* a_quest, RE::BSFixedString a_text, std::int32_t a_index);

	void Bind(VM& a_vm);
}
