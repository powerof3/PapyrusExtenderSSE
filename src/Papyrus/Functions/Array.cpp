#include "Papyrus/Functions/Array.h"

namespace Papyrus::Array
{
	bool AddActorToArray(RE::StaticFunctionTag*, RE::Actor* a_actor, RE::reference_array<RE::Actor*> a_actors)
	{
		if (const auto it = std::ranges::find(a_actors, nullptr); it != a_actors.end()) {
			*it = a_actor;
			return true;
		}
		return false;
	}

	bool AddStringToArray(RE::StaticFunctionTag*, std::string a_string, RE::reference_array<std::string> a_strings)
	{
		if (const auto it = std::ranges::find(a_strings, ""); it != a_strings.end()) {
			*it = a_string;
			return true;
		}
		return false;
	}

	std::uint32_t ArrayStringCount(RE::StaticFunctionTag*, std::string a_string, const RE::reference_array<std::string> a_strings)
	{
		return static_cast<std::uint32_t>(std::ranges::count(a_strings, a_string));
	}

	std::vector<std::string> SortArrayString(RE::StaticFunctionTag*, const RE::reference_array<std::string> a_strings)
	{
		std::vector<std::string> strings(a_strings);

		strings.erase(std::ranges::remove_if(strings, [](const std::string& str) {
			return str.empty();
		}).begin(),
			strings.end());

		std::ranges::sort(strings);

		return strings;
	}

	std::vector<std::string> GetSortedActorNames(RE::StaticFunctionTag*, const RE::BGSKeyword* a_keyword, std::string a_pronoun, bool a_invert)
	{
		std::unordered_map<std::string, std::size_t> nameMap;

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			bool       hasKeyword = false;
			const bool noKeyword = !a_keyword;

			for (const auto& handle : processLists->highActorHandles) {
				if (const auto actor = handle.get(); actor) {
					if (!noKeyword) {
						hasKeyword = actor->HasKeyword(a_keyword);
						if (a_invert) {
							hasKeyword = !hasKeyword;
						}
					}
					if (noKeyword || hasKeyword) {
						++nameMap[actor->GetName()];
					}
				}
			}
		}

		std::vector<std::string> names;
		for (const auto& [name, count] : nameMap) {
			std::string fullName = count > 1 ?
			                           std::to_string(count).append(" ").append(name).append(a_pronoun) :
			                           name;
			names.emplace_back(fullName);
		}

		std::ranges::sort(names);

		return names;
	}

	std::vector<std::string> GetSortedNPCNames(VM*, StackID, RE::StaticFunctionTag*, const std::vector<RE::TESNPC*> a_npcs, std::string a_pronoun)
	{
		std::vector<std::string> names;

		if (a_npcs.empty()) {
			return names;
		}

		std::unordered_map<std::string, std::size_t> nameMap;
		for (const auto& npc : a_npcs) {
			if (!npc) {
				continue;
			}
			std::string name{ npc->GetName() };
			if (name.empty()) {
				name = npc->shortName;
			}
			++nameMap[name];
		}

		for (const auto& [name, count] : nameMap) {
			std::string fullName = count > 1 ?
			                           std::to_string(count).append(" ").append(name).append(a_pronoun) :
			                           name;
			names.emplace_back(fullName);
		}

		std::ranges::sort(names);

		return names;
	}

	void Bind(VM& a_vm)
	{
		std::uint32_t count = 0;

		BIND(AddActorToArray);
		BIND(AddStringToArray);
		BIND(ArrayStringCount);
		BIND(SortArrayString);
		BIND(GetSortedActorNames);
		BIND(GetSortedNPCNames);

		logger::info("Registered {} array functions"sv, count);
	}
}
