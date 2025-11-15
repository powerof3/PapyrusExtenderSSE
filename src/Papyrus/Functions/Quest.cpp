#include "Papyrus/Functions/Quest.h"

namespace Papyrus::Quest
{
	std::vector<std::uint32_t> GetAllQuestObjectives(STATIC_ARGS, RE::TESQuest* a_quest)
	{
		if (!a_quest) {
			a_vm->TraceStack("Quest is None", a_stackID);
			return {};
		}

		std::vector<std::uint32_t> indices;
		for (const auto& objective : a_quest->objectives) {
			if (objective) {
				indices.push_back(objective->index);
			}
		}
		return indices;
	}

	std::vector<std::uint32_t> GetAllQuestStages(STATIC_ARGS, RE::TESQuest* a_quest)
	{
		if (!a_quest) {
			a_vm->TraceStack("Quest is None", a_stackID);
			return {};
		}

		std::set<std::uint32_t> indices;
		if (a_quest->executedStages) {
			for (const auto& stage : *a_quest->executedStages) {
				indices.insert(stage.data.index);
			}
		}
		if (a_quest->waitingStages) {
			for (const auto& stage : *a_quest->waitingStages) {
				if (stage) {
					indices.insert(stage->data.index);
				}
			}
		}

		std::vector<std::uint32_t> indicesVec(indices.begin(), indices.end());

		return indicesVec;
	}

	void SetObjectiveText(STATIC_ARGS, RE::TESQuest* a_quest, RE::BSFixedString a_text, std::int32_t a_index)
	{
		if (!a_quest) {
			a_vm->TraceStack("Quest is None", a_stackID);
		}

		for (const auto& objective : a_quest->objectives) {
			if (objective && objective->index == a_index) {
				objective->displayText = a_text;
				break;
			}
		}
	}

	void Bind(VM& a_vm)
	{
		std::uint32_t count = 0;
		
		BIND(GetAllQuestObjectives);
		BIND(GetAllQuestStages);
		BIND(SetObjectiveText);

		logger::info("Registered {} quest functions"sv, count);
	}
}
