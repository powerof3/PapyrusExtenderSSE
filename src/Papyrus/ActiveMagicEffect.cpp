#include "Papyrus/ActiveMagicEffect.h"


void papyrusActiveMagicEffect::RegisterForActorKilled(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnActorKillRegSet::GetSingleton();
	regs->Register(a_activeEffect);
}


void papyrusActiveMagicEffect::RegisterForFECReset(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect, std::uint32_t a_type)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = FECEvents::OnFECResetRegMap::GetSingleton();
	regs->Register(a_activeEffect, a_type);
}


void papyrusActiveMagicEffect::RegisterForActorReanimateStart(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = HookedEvents::OnActorReanimateStartRegSet::GetSingleton();
	regs->Register(a_activeEffect);
}


void papyrusActiveMagicEffect::RegisterForActorReanimateStop(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = HookedEvents::OnActorReanimateStopRegSet::GetSingleton();
	regs->Register(a_activeEffect);
}


void papyrusActiveMagicEffect::RegisterForActorResurrected(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = HookedEvents::OnActorResurrectRegSet::GetSingleton();
	regs->Register(a_activeEffect);
}


void papyrusActiveMagicEffect::RegisterForBookRead(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnBooksReadRegSet::GetSingleton();
	regs->Register(a_activeEffect);
}


void papyrusActiveMagicEffect::RegisterForCellFullyLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = ScriptEvents::OnCellFullyLoadedRegSet::GetSingleton();
	regs->Register(a_activeEffect);
}


void papyrusActiveMagicEffect::RegisterForCriticalHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnCriticalHitRegSet::GetSingleton();
	regs->Register(a_activeEffect);
}


void papyrusActiveMagicEffect::RegisterForDisarmed(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnDisarmedRegSet::GetSingleton();
	regs->Register(a_activeEffect);
}


void papyrusActiveMagicEffect::RegisterForDragonSoulGained(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnDragonSoulsGainedRegSet::GetSingleton();
	regs->Register(a_activeEffect);
}


void papyrusActiveMagicEffect::RegisterForItemHarvested(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnItemHarvestedRegSet::GetSingleton();
	regs->Register(a_activeEffect);
}


void papyrusActiveMagicEffect::RegisterForLevelIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnLevelIncreaseRegSet::GetSingleton();
	regs->Register(a_activeEffect);
}


void papyrusActiveMagicEffect::RegisterForLocationDiscovery(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnLocationDiscoveryRegSet::GetSingleton();
	regs->Register(a_activeEffect);
}


void papyrusActiveMagicEffect::RegisterForObjectGrab(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto grab = ScriptEvents::OnGrabRegSet::GetSingleton();
	grab->Register(a_activeEffect);

	auto release = ScriptEvents::OnReleaseRegSet::GetSingleton();
	release->Register(a_activeEffect);
}


void papyrusActiveMagicEffect::RegisterForObjectLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect, std::uint32_t a_formType)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto formType = static_cast<RE::FormType>(a_formType);

	auto load = ScriptEvents::OnObjectLoadedRegMap::GetSingleton();
	load->Register(a_activeEffect, formType);

	auto unload = ScriptEvents::OnObjectUnloadedRegMap::GetSingleton();
	unload->Register(a_activeEffect, formType);
}


void papyrusActiveMagicEffect::RegisterForQuest(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect, RE::TESQuest* a_quest)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_quest) {
		a_vm->TraceStack("Quest is None", a_stackID, Severity::kWarning);
		return;	
	}

	auto start = ScriptEvents::OnQuestStartRegMap::GetSingleton();
	start->Register(a_activeEffect, a_quest->GetFormID());

	auto stop = ScriptEvents::OnQuestStopRegMap::GetSingleton();
	stop->Register(a_activeEffect, a_quest->GetFormID());
}


void papyrusActiveMagicEffect::RegisterForQuestStage(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect, RE::TESQuest* a_quest)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_quest) {
		a_vm->TraceStack("Quest is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = ScriptEvents::OnQuestStageRegMap::GetSingleton();
	regs->Register(a_activeEffect, a_quest->GetFormID());
}


void papyrusActiveMagicEffect::RegisterForShoutAttack(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnShoutAttackRegSet::GetSingleton();
	regs->Register(a_activeEffect);
}


void papyrusActiveMagicEffect::RegisterForSkillIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnSkillIncreaseRegSet::GetSingleton();
	regs->Register(a_activeEffect);
}


void papyrusActiveMagicEffect::RegisterForSoulTrapped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnSoulsTrappedRegSet::GetSingleton();
	regs->Register(a_activeEffect);
}

void papyrusActiveMagicEffect::RegisterForSpellLearned(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnSpellsLearnedRegSet::GetSingleton();
	regs->Register(a_activeEffect);
}


void papyrusActiveMagicEffect::RegisterForWeatherChange(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = HookedEvents::OnWeatherChangeRegSet::GetSingleton();
	regs->Register(a_activeEffect);
}



void papyrusActiveMagicEffect::UnregisterForActorKilled(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnActorKillRegSet::GetSingleton();
	regs->Unregister(a_activeEffect);
}


void papyrusActiveMagicEffect::UnregisterForFECReset(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect, std::uint32_t a_type)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = FECEvents::OnFECResetRegMap::GetSingleton();
	regs->Unregister(a_activeEffect, a_type);
}


void papyrusActiveMagicEffect::UnregisterForAllFECResets(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = FECEvents::OnFECResetRegMap::GetSingleton();
	regs->UnregisterAll(a_activeEffect);
}


void papyrusActiveMagicEffect::UnregisterForActorReanimateStart(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = HookedEvents::OnActorReanimateStartRegSet::GetSingleton();
	regs->Unregister(a_activeEffect);
}


void papyrusActiveMagicEffect::UnregisterForActorReanimateStop(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = HookedEvents::OnActorReanimateStopRegSet::GetSingleton();
	regs->Unregister(a_activeEffect);
}


void papyrusActiveMagicEffect::UnregisterForActorResurrected(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = HookedEvents::OnActorResurrectRegSet::GetSingleton();
	regs->Unregister(a_activeEffect);
}


void papyrusActiveMagicEffect::UnregisterForBookRead(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnBooksReadRegSet::GetSingleton();
	regs->Unregister(a_activeEffect);
}


void papyrusActiveMagicEffect::UnregisterForCellFullyLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = ScriptEvents::OnCellFullyLoadedRegSet::GetSingleton();
	regs->Unregister(a_activeEffect);
}


void papyrusActiveMagicEffect::UnregisterForCriticalHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnCriticalHitRegSet::GetSingleton();
	regs->Unregister(a_activeEffect);
}


void papyrusActiveMagicEffect::UnregisterForDisarmed(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnDisarmedRegSet::GetSingleton();
	regs->Unregister(a_activeEffect);
}


void papyrusActiveMagicEffect::UnregisterForDragonSoulGained(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnDragonSoulsGainedRegSet::GetSingleton();
	regs->Unregister(a_activeEffect);
}


void papyrusActiveMagicEffect::UnregisterForItemHarvested(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnItemHarvestedRegSet::GetSingleton();
	regs->Unregister(a_activeEffect);
}


void papyrusActiveMagicEffect::UnregisterForLevelIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnLevelIncreaseRegSet::GetSingleton();
	regs->Unregister(a_activeEffect);
}


void papyrusActiveMagicEffect::UnregisterForLocationDiscovery(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnLocationDiscoveryRegSet::GetSingleton();
	regs->Unregister(a_activeEffect);
}


void papyrusActiveMagicEffect::UnregisterForObjectGrab(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto grab = ScriptEvents::OnGrabRegSet::GetSingleton();
	grab->Unregister(a_activeEffect);

	auto release = ScriptEvents::OnReleaseRegSet::GetSingleton();
	release->Unregister(a_activeEffect);
}


void papyrusActiveMagicEffect::UnregisterForObjectLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect, std::uint32_t a_formType)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto formType = static_cast<RE::FormType>(a_formType);

	auto load = ScriptEvents::OnObjectLoadedRegMap::GetSingleton();
	load->Unregister(a_activeEffect, formType);

	auto unload = ScriptEvents::OnObjectUnloadedRegMap::GetSingleton();
	unload->Unregister(a_activeEffect, formType);
}


void papyrusActiveMagicEffect::UnregisterForAllObjectsLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto load = ScriptEvents::OnObjectLoadedRegMap::GetSingleton();
	load->UnregisterAll(a_activeEffect);

	auto unload = ScriptEvents::OnObjectUnloadedRegMap::GetSingleton();
	unload->UnregisterAll(a_activeEffect);
}


void papyrusActiveMagicEffect::UnregisterForQuest(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect, RE::TESQuest* a_quest)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_quest) {
		a_vm->TraceStack("Quest is None", a_stackID, Severity::kWarning);
		return;
	}

	auto start = ScriptEvents::OnQuestStartRegMap::GetSingleton();
	start->Unregister(a_activeEffect, a_quest->GetFormID());

	auto stop = ScriptEvents::OnQuestStartRegMap::GetSingleton();
	stop->Unregister(a_activeEffect, a_quest->GetFormID());
}


void papyrusActiveMagicEffect::UnregisterForAllQuests(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto start = ScriptEvents::OnQuestStartRegMap::GetSingleton();
	start->UnregisterAll(a_activeEffect);

	auto stop = ScriptEvents::OnQuestStartRegMap::GetSingleton();
	stop->UnregisterAll(a_activeEffect);
}


void papyrusActiveMagicEffect::UnregisterForQuestStage(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect, RE::TESQuest* a_quest)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_quest) {
		a_vm->TraceStack("Quest is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = ScriptEvents::OnQuestStageRegMap::GetSingleton();
	regs->Unregister(a_activeEffect, a_quest->GetFormID());
}


void papyrusActiveMagicEffect::UnregisterForAllQuestStages(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = ScriptEvents::OnQuestStageRegMap::GetSingleton();
	regs->UnregisterAll(a_activeEffect);
}


void papyrusActiveMagicEffect::UnregisterForShoutAttack(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnShoutAttackRegSet::GetSingleton();
	regs->Unregister(a_activeEffect);
}


void papyrusActiveMagicEffect::UnregisterForSkillIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnShoutAttackRegSet::GetSingleton();
	regs->Unregister(a_activeEffect);
}


void papyrusActiveMagicEffect::UnregisterForSoulTrapped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnSoulsTrappedRegSet::GetSingleton();
	regs->Unregister(a_activeEffect);
}


void papyrusActiveMagicEffect::UnregisterForSpellLearned(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnSpellsLearnedRegSet::GetSingleton();
	regs->Unregister(a_activeEffect);
}


void papyrusActiveMagicEffect::UnregisterForWeatherChange(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = HookedEvents::OnWeatherChangeRegSet::GetSingleton();
	regs->Unregister(a_activeEffect);
}


bool papyrusActiveMagicEffect::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("papyrusActiveMagicEffect - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("RegisterForActorKilled", "PO3_Events_AME", RegisterForActorKilled, true);

	a_vm->RegisterFunction("RegisterForFECReset", "PO3_Events_AME", RegisterForFECReset, true);

	a_vm->RegisterFunction("RegisterForActorReanimateStart", "PO3_Events_AME", RegisterForActorReanimateStart, true);

	a_vm->RegisterFunction("RegisterForActorReanimateStop", "PO3_Events_AME", RegisterForActorReanimateStop, true);

	a_vm->RegisterFunction("RegisterForActorResurrected", "PO3_Events_AME", RegisterForActorResurrected, true);

	a_vm->RegisterFunction("RegisterForBookRead", "PO3_Events_AME", RegisterForBookRead, true);

	a_vm->RegisterFunction("RegisterForCellFullyLoaded", "PO3_Events_AME", RegisterForCellFullyLoaded, true);

	a_vm->RegisterFunction("RegisterForCriticalHit", "PO3_Events_AME", RegisterForCriticalHit, true);

	a_vm->RegisterFunction("RegisterForDisarmed", "PO3_Events_AME", RegisterForDisarmed, true);

	a_vm->RegisterFunction("RegisterForDragonSoulGained", "PO3_Events_AME", RegisterForDragonSoulGained, true);

	a_vm->RegisterFunction("RegisterForItemHarvested", "PO3_Events_AME", RegisterForItemHarvested, true);

	a_vm->RegisterFunction("RegisterForLevelIncrease", "PO3_Events_AME", RegisterForLevelIncrease, true);

	a_vm->RegisterFunction("RegisterForLocationDiscovery", "PO3_Events_AME", RegisterForLocationDiscovery, true);

	a_vm->RegisterFunction("RegisterForObjectGrab", "PO3_Events_AME", RegisterForObjectGrab, true);
	
	a_vm->RegisterFunction("RegisterForObjectLoaded", "PO3_Events_AME", RegisterForObjectLoaded, true);

	a_vm->RegisterFunction("RegisterForQuest", "PO3_Events_AME", RegisterForQuest, true);

	a_vm->RegisterFunction("RegisterForQuestStage", "PO3_Events_AME", RegisterForQuestStage, true);

	a_vm->RegisterFunction("RegisterForShoutAttack", "PO3_Events_AME", RegisterForShoutAttack, true);

	a_vm->RegisterFunction("RegisterForSkillIncrease", "PO3_Events_AME", RegisterForSkillIncrease, true);
	
	a_vm->RegisterFunction("RegisterForSoulTrapped", "PO3_Events_AME", RegisterForSoulTrapped, true);

	a_vm->RegisterFunction("RegisterForSpellLearned", "PO3_Events_AME", RegisterForSpellLearned, true);
	
	a_vm->RegisterFunction("RegisterForWeatherChange", "PO3_Events_AME", RegisterForWeatherChange, true);


	a_vm->RegisterFunction("UnregisterForActorKilled", "PO3_Events_AME", UnregisterForActorKilled, true);

	a_vm->RegisterFunction("UnregisterForFECReset", "PO3_Events_AME", UnregisterForFECReset, true);

	a_vm->RegisterFunction("UnregisterForAllFECResets", "PO3_Events_AME", UnregisterForAllFECResets, true);

	a_vm->RegisterFunction("UnregisterForActorReanimateStart", "PO3_Events_AME", UnregisterForActorReanimateStart, true);

	a_vm->RegisterFunction("UnregisterForActorReanimateStop", "PO3_Events_AME", UnregisterForActorReanimateStop, true);

	a_vm->RegisterFunction("UnregisterForActorResurrected", "PO3_Events_AME", UnregisterForActorResurrected, true);
	
	a_vm->RegisterFunction("UnregisterForBookRead", "PO3_Events_AME", UnregisterForBookRead, true);

	a_vm->RegisterFunction("UnregisterForCellFullyLoaded", "PO3_Events_AME", UnregisterForCellFullyLoaded, true);

	a_vm->RegisterFunction("UnregisterForCriticalHit", "PO3_Events_AME", UnregisterForCriticalHit, true);

	a_vm->RegisterFunction("UnregisterForDisarmed", "PO3_Events_AME", UnregisterForDisarmed, true);

	a_vm->RegisterFunction("UnregisterForDragonSoulGained", "PO3_Events_AME", UnregisterForDragonSoulGained, true);

	a_vm->RegisterFunction("UnregisterForItemHarvested", "PO3_Events_AME", UnregisterForItemHarvested, true);

	a_vm->RegisterFunction("UnregisterForLevelIncrease", "PO3_Events_AME", UnregisterForLevelIncrease, true);

	a_vm->RegisterFunction("UnregisterForLocationDiscovery", "PO3_Events_AME", UnregisterForLocationDiscovery, true);

	a_vm->RegisterFunction("UnregisterForObjectGrab", "PO3_Events_AME", UnregisterForObjectGrab, true);

	a_vm->RegisterFunction("UnregisterForObjectLoaded", "PO3_Events_AME", UnregisterForObjectLoaded, true);

	a_vm->RegisterFunction("UnregisterForAllObjectsLoaded", "PO3_Events_AME", UnregisterForAllObjectsLoaded, true);

	a_vm->RegisterFunction("UnregisterForQuest", "PO3_Events_AME", UnregisterForQuest, true);

	a_vm->RegisterFunction("UnregisterForAllQuests", "PO3_Events_AME", UnregisterForAllQuests, true);

	a_vm->RegisterFunction("UnregisterForQuestStage", "PO3_Events_AME", UnregisterForQuestStage, true);

	a_vm->RegisterFunction("UnregisterForAllQuestStages", "PO3_Events_AME", UnregisterForAllQuestStages, true);

	a_vm->RegisterFunction("UnregisterForShoutAttack", "PO3_Events_AME", UnregisterForShoutAttack, true);

	a_vm->RegisterFunction("UnregisterForSkillIncrease", "PO3_Events_AME", UnregisterForSkillIncrease, true);

	a_vm->RegisterFunction("UnregisterForSoulTrapped", "PO3_Events_AME", UnregisterForSoulTrapped, true);

	a_vm->RegisterFunction("UnregisterForSpellLearned", "PO3_Events_AME", UnregisterForSpellLearned, true);

	a_vm->RegisterFunction("UnregisterForWeatherChange", "PO3_Events_AME", UnregisterForWeatherChange, true);


	return true;
}
