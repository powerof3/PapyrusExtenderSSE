#pragma once

namespace Papyrus::Array
{
	inline bool AddActorToArray(VM*, StackID, RE::StaticFunctionTag*, 
		RE::Actor* a_actor, 
		RE::reference_array<RE::Actor*> a_actors)
	{
        const auto it = std::ranges::find(a_actors, nullptr);
		if (it != a_actors.end()) {
			*it = a_actor; 
			return true;
		}
		return false;
	}

	inline bool AddStringToArray(VM*, StackID, RE::StaticFunctionTag*, 
		RE::BSFixedString a_string, 
		RE::reference_array<RE::BSFixedString> a_strings)
	{
        const auto it = std::ranges::find(a_strings, "");
		if (it != a_strings.end()) {
			*it = a_string;
			return true;
		}
		return false;
	}

	inline std::uint32_t ArrayStringCount(VM*, StackID, RE::StaticFunctionTag*, 
		RE::BSFixedString a_string, 
		RE::reference_array<RE::BSFixedString> a_strings)
	{
		return static_cast<std::uint32_t>(std::ranges::count(a_strings, a_string));
	}

	inline std::vector<RE::BSFixedString> SortArrayString(VM*, StackID, RE::StaticFunctionTag*, RE::reference_array<RE::BSFixedString> a_strings)
	{
		std::vector<RE::BSFixedString> strings(a_strings);
		strings.erase(std::ranges::remove_if(strings, [](const RE::BSFixedString& str) {
                return str.empty();
            }).begin(),
			strings.end());
		std::ranges::sort(strings, [](const RE::BSFixedString& a_lhs, const RE::BSFixedString& a_rhs) {
			return _stricmp(a_lhs.c_str(), a_rhs.c_str()) < 0;
		});
		return strings;
	}

	inline std::vector<RE::BSFixedString> GetSortedActorNames(VM*, StackID, RE::StaticFunctionTag*, 
		const RE::BGSKeyword* a_keyword,
		RE::BSFixedString a_pronoun, 
		bool a_invert)
	{
		std::unordered_map<std::string, size_t> nameMap;

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			bool hasKeyword = false;
			bool noKeyword = !a_keyword;

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

		std::vector<RE::BSFixedString> names;
		for (const auto& [name, count] : nameMap) {
			std::string fullName = count > 1 ? std::to_string(count) + " " + name + a_pronoun.c_str() : name;
			names.emplace_back(fullName);
		}

		std::ranges::sort(names, [](const auto& a_lhs, const auto& a_rhs) {
			return _stricmp(a_lhs.c_str(), a_rhs.c_str()) < 0;
		});

		return names;
	}

	inline std::vector<RE::BSFixedString> GetSortedNPCNames(VM*, StackID, RE::StaticFunctionTag*, 
		std::vector<RE::TESNPC*> a_npcs, 
		RE::BSFixedString a_pronoun)
	{
		std::vector<RE::BSFixedString> names;

		if (a_npcs.empty()) {
			return names;
		}

		std::unordered_map<std::string, size_t> nameMap;
		for (auto& npc : a_npcs) {
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
			std::string fullName{ count > 1 ? std::to_string(count) + " " + name + a_pronoun.c_str() : name };
			names.emplace_back(fullName);
		}

		std::ranges::sort(names, [](const auto& a_lhs, const auto& a_rhs) {
			return _stricmp(a_lhs.c_str(), a_rhs.c_str()) < 0;
		});

		return names;
	}

	inline void Bind(VM& a_vm)
	{
		BIND(AddActorToArray);
		BIND(AddStringToArray);
		BIND(ArrayStringCount);
		BIND(SortArrayString);
		BIND(GetSortedActorNames);
		BIND(GetSortedNPCNames);

		logger::info("Registered array functions"sv);
	}
}
