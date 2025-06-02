#pragma once

namespace Papyrus::Array
{
	bool AddActorToArray(RE::StaticFunctionTag*, RE::Actor* a_actor, RE::reference_array<RE::Actor*> a_actors);
	bool AddStringToArray(RE::StaticFunctionTag*, std::string a_string, RE::reference_array<std::string> a_strings);
	std::uint32_t ArrayStringCount(RE::StaticFunctionTag*, std::string a_string, const RE::reference_array<std::string> a_strings);
	std::vector<std::string> SortArrayString(RE::StaticFunctionTag*, const RE::reference_array<std::string> a_strings);
	std::vector<std::string> GetSortedActorNames(RE::StaticFunctionTag*, const RE::BGSKeyword* a_keyword, std::string a_pronoun, bool a_invert);
	std::vector<std::string> GetSortedNPCNames(VM*, StackID, RE::StaticFunctionTag*, const std::vector<RE::TESNPC*> a_npcs, std::string a_pronoun);

	void Bind(VM& a_vm);
}
