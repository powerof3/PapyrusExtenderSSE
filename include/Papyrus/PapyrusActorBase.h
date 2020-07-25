#pragma once


namespace papyrusActorBase
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;


	std::uint32_t GetPerkCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESNPC* a_actorBase);

	RE::BGSPerk* GetNthPerk(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESNPC* a_actorBase, std::uint32_t a_index);


	bool RegisterFuncs(VM* a_vm);
}