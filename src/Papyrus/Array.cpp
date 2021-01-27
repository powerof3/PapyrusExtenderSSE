#include "Papyrus/Array.h"


auto papyrusArray::AddActorToArray(VM*, StackID, RE::StaticFunctionTag*, RE::Actor* a_actor, reference_array<RE::Actor*> a_actors) -> bool
{
	for (auto& actor : a_actors) {
		if (!actor) {
			actor = a_actor;
			return true;
		}
	}
	return false;
}


auto papyrusArray::AddStringToArray(VM*, StackID, RE::StaticFunctionTag*, RE::BSFixedString a_string, reference_array<RE::BSFixedString> a_strings) -> bool
{
	for (auto& string : a_strings) {
		if (string.empty()) {
			string = a_string;
			return true;
		}
	}
	return false;
}


auto papyrusArray::ArrayStringCount(VM*, StackID, RE::StaticFunctionTag*, RE::BSFixedString a_string, reference_array<RE::BSFixedString> a_strings) -> std::uint32_t
{
	return static_cast<std::uint32_t>(std::count(a_strings.begin(), a_strings.end(), a_string));
}


auto papyrusArray::SortArrayString(VM*, StackID, RE::StaticFunctionTag*, reference_array<RE::BSFixedString> a_strings) -> std::vector<RE::BSFixedString>
{
	std::vector<RE::BSFixedString> strings(a_strings);
	strings.erase(std::remove_if(strings.begin(), strings.end(), [](const RE::BSFixedString& str) {
		return str.empty();
	}),
		strings.end());
	std::sort(strings.begin(), strings.end(), [](const RE::BSFixedString& a_lhs, const RE::BSFixedString& a_rhs) {
		return _stricmp(a_lhs.c_str(), a_rhs.c_str()) < 0;
	});

	return strings;
}


auto papyrusArray::GetSortedActorNameArray(VM*, StackID, RE::StaticFunctionTag*, RE::BGSKeyword* a_keyword, RE::BSFixedString a_pronoun, bool a_invert) -> std::vector<RE::BSFixedString>
{
	std::unordered_map<std::string, size_t> nameMap;

	bool noKeyword = !a_keyword ? true : false;
	bool hasKeyword = false;

	if (auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
		for (const auto& handle : processLists->highActorHandles) {
			auto actorPtr = handle.get();
			auto actor = actorPtr.get();
			if (actor) {
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

	std::sort(names.begin(), names.end(), [](const auto& a_lhs, const auto& a_rhs) {
		return _stricmp(a_lhs.c_str(), a_rhs.c_str()) < 0;
	});

	return names;
}


auto papyrusArray::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusArray - couldn't get VMState"sv);
		return false;
	}

	auto constexpr Functions = "PO3_SKSEFunctions"sv;

	a_vm->RegisterFunction("AddActorToArray"sv, Functions, AddActorToArray);

    a_vm->RegisterFunction("AddStringToArray"sv, Functions, AddStringToArray);

    a_vm->RegisterFunction("ArrayStringCount"sv, Functions, ArrayStringCount);

    a_vm->RegisterFunction("SortArrayString"sv, Functions, SortArrayString);

    a_vm->RegisterFunction("GetSortedActorNameArray"sv, Functions, GetSortedActorNameArray);

	return true;
}
