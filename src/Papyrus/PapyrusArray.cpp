#include "Papyrus/PapyrusArray.h"


bool papyrusArray::AddActorToArray(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, reference_array<RE::Actor*> a_actors)
{
	for (auto& actor : a_actors) {
		if (!actor) {
			actor = a_actor;
			return true;
		}
	}
	return false;
}


bool papyrusArray::AddStringToArray(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_string, reference_array<RE::BSFixedString> a_strings)
{
	for (auto& string : a_strings) {
		if (string.empty()) {
			string = a_string;
			return true;
		}
	}
	return false;
}


std::uint32_t papyrusArray::ArrayStringCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_string, reference_array<RE::BSFixedString> a_strings)
{
	return static_cast<std::uint32_t>(std::count(a_strings.begin(), a_strings.end(), a_string));
}


std::vector<RE::BSFixedString> papyrusArray::SortArrayString(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, reference_array<RE::BSFixedString> a_strings)
{
	std::vector<RE::BSFixedString> strings;

	strings = a_strings;
	strings.erase(std::remove_if(strings.begin(), strings.end(), [](const RE::BSFixedString& str) {
		return str.empty();
	}),
		strings.end());
	std::sort(strings.begin(), strings.end(), [](const RE::BSFixedString& a_lhs, const RE::BSFixedString& a_rhs) {
		return _stricmp(a_lhs.c_str(), a_rhs.c_str()) < 0;
	});

	return strings;
}


std::vector<RE::BSFixedString> papyrusArray::GetSortedActorNameArray(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSKeyword* a_keyword, bool a_invert)
{
	std::unordered_map<std::string, size_t> nameMap;

	bool noKeyword = !a_keyword ? true : false;
	bool hasKeyword = false;

	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		for (const auto& handle : processLists->highActorHandles) {
			auto actor = handle.get();
			if (actor.get()) {
				if (!noKeyword) {
					hasKeyword = actor->HasKeyword(a_keyword) ? true : false;
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
	for (const auto& name : nameMap) {
		std::string fullName = name.second > 1 ? std::to_string(name.second) + " " + name.first + "(s)" : name.first;
		names.push_back(fullName.c_str());
	}

	std::sort(names.begin(), names.end(), [](const RE::BSFixedString& a_lhs, const RE::BSFixedString& a_rhs) {
		return _stricmp(a_lhs.c_str(), a_rhs.c_str()) < 0;
	});

	return names;
}


bool papyrusArray::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("papyrusArray - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("AddActorToArray", "PO3_SKSEFunctions", AddActorToArray);

	a_vm->RegisterFunction("AddStringToArray", "PO3_SKSEFunctions", AddStringToArray);

	a_vm->RegisterFunction("ArrayStringCount", "PO3_SKSEFunctions", ArrayStringCount);

	a_vm->RegisterFunction("SortArrayString", "PO3_SKSEFunctions", SortArrayString);

	//a_vm->RegisterFunction("GetSortedActorNameArray", "PO3_SKSEFunctions", GetSortedActorNameArray);

	return true;
}
