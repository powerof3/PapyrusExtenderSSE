#pragma once

namespace Papyrus::Quest
{
	inline std::vector<std::uint32_t> GetAllQuestObjectives(STATIC_ARGS, RE::TESQuest* a_quest)
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

	inline void SetObjectiveText(STATIC_ARGS, RE::TESQuest* a_quest, RE::BSFixedString a_text, std::int32_t a_index)
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

	inline void Bind(VM& a_vm)
	{
		BIND(GetAllQuestObjectives);
		BIND(SetObjectiveText);

		logger::info("Registered quest functions"sv);
	}
}
