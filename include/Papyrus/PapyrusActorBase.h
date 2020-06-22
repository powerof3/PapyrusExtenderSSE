#pragma once

#include "main.h"


namespace papyrusActorBase
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;


	UInt32 GetPerkCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESNPC* a_actorBase);

	RE::BGSPerk* GetNthPerk(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESNPC* a_actorBase, UInt32 a_index);


	bool RegisterFuncs(VM* a_vm);
}