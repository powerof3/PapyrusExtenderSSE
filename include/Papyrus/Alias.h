#pragma once

#include "Serialization/Events.h"


namespace papyrusAlias
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;
	using namespace Serialization;


	void RegisterForActorKilled(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void RegisterForActorReanimateStart(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void RegisterForActorReanimateStop(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void RegisterForActorResurrected(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void RegisterForBookRead(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void RegisterForCellFullyLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void RegisterForCriticalHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void RegisterForDisarmed(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void RegisterForDragonSoulGained(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void RegisterForItemHarvested(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void RegisterForLevelIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void RegisterForLocationDiscovery(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void RegisterForMagicEffectApplyEx(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSRefAlias* a_alias, RE::TESForm* a_effectFilter, bool a_match);

	void RegisterForMagicHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSRefAlias* a_alias);

	void RegisterForObjectGrab(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void RegisterForObjectLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias, std::uint32_t a_formType);

	void RegisterForProjectileHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSRefAlias* a_alias);

	void RegisterForQuest(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias, RE::TESQuest* a_quest);

	void RegisterForQuestStage(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias, RE::TESQuest* a_quest);

	void RegisterForShoutAttack(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void RegisterForSkillIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void RegisterForSoulTrapped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void RegisterForSpellLearned(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void RegisterForWeatherChange(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void RegisterForWeaponHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSRefAlias* a_alias);


	void UnregisterForActorKilled(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void UnregisterForActorReanimateStart(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void UnregisterForActorReanimateStop(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void UnregisterForActorResurrected(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void UnregisterForBookRead(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void UnregisterForCellFullyLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void UnregisterForCriticalHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void UnregisterForDisarmed(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void UnregisterForDragonSoulGained(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void UnregisterForItemHarvested(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void UnregisterForLevelIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void UnregisterForLocationDiscovery(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void UnregisterForMagicEffectApplyEx(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSRefAlias* a_alias, RE::TESForm* a_effectFilter, bool a_match);

	void UnregisterForAllMagicEffectApplyEx(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSRefAlias* a_alias);

	void UnregisterForMagicHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSRefAlias* a_alias);

	void UnregisterForObjectGrab(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void UnregisterForObjectLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias, std::uint32_t a_formType);

	void UnregisterForAllObjectsLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void UnregisterForProjectileHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSRefAlias* a_alias);

	void UnregisterForQuest(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias, RE::TESQuest* a_quest);

	void UnregisterForAllQuests(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void UnregisterForQuestStage(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias, RE::TESQuest* a_quest);

	void UnregisterForAllQuestStages(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void UnregisterForShoutAttack(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void UnregisterForSkillIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void UnregisterForSoulTrapped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void UnregisterForSpellLearned(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void UnregisterForWeatherChange(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias);

	void UnregisterForWeaponHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSRefAlias* a_alias);


	bool RegisterFuncs(VM* a_vm);
}