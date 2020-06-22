#pragma once

#include "main.h"


namespace papyrusArray
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;

	template<class T>
	using reference_array = RE::reference_array<T>;


	bool AddActorToArray(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, reference_array<RE::Actor*> a_actors);

	bool AddStringToArray(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_string, reference_array<RE::BSFixedString> a_strings);

	UInt32 ArrayStringCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_string, reference_array<RE::BSFixedString> a_strings);

	std::vector<RE::BSFixedString> SortArrayString(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, reference_array<RE::BSFixedString> a_strings);

	std::vector<RE::BSFixedString> GetSortedActorNameArray(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSKeyword* a_keyword, bool a_invert);


	bool RegisterFuncs(VM* a_vm);
}
