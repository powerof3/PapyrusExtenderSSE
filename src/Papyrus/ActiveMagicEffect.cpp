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


void papyrusActiveMagicEffect::RegisterForMagicEffectApplyEx(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::ActiveEffect* a_activeEffect, RE::TESForm* a_effectFilter, bool a_match)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_effectFilter) {
		a_vm->TraceStack("Effect Filter is None", a_stackID, Severity::kWarning);
		return;
	}

	auto key = std::make_pair(a_effectFilter->GetFormID(), a_match);
	auto regs = HookedEvents::OnMagicEffectApplyRegMap::GetSingleton();
	regs->Register(a_activeEffect, key);
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
	}
	if (!a_quest) {
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
	}
	if (!a_quest) {
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


void papyrusActiveMagicEffect::UnregisterForMagicEffectApplyEx(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::ActiveEffect* a_activeEffect, RE::TESForm* a_effectFilter, bool a_match)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_effectFilter) {
		a_vm->TraceStack("Effect Filter is None", a_stackID, Severity::kWarning);
		return;
	}

	auto key = std::make_pair(a_effectFilter->GetFormID(), a_match);
	auto regs = HookedEvents::OnMagicEffectApplyRegMap::GetSingleton();
	regs->Unregister(a_activeEffect, key);
}


void papyrusActiveMagicEffect::UnregisterForAllMagicEffectApplyEx(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::ActiveEffect* a_activeEffect)
{
	if (!a_activeEffect) {
		a_vm->TraceStack("Active Effect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = HookedEvents::OnMagicEffectApplyRegMap::GetSingleton();
	regs->UnregisterAll(a_activeEffect);
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
	}
	if (!a_quest) {
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
	}
	if (!a_quest) {
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


auto papyrusActiveMagicEffect::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusActiveMagicEffect - couldn't get VMState"sv);
		return false;
	}

	auto constexpr Event_AME = "PO3_Events_AME"sv;

	a_vm->RegisterFunction("RegisterForActorKilled"sv, Event_AME, RegisterForActorKilled, true);

	a_vm->RegisterFunction("RegisterForFECReset"sv, Event_AME, RegisterForFECReset, true);

	a_vm->RegisterFunction("RegisterForActorReanimateStart"sv, Event_AME, RegisterForActorReanimateStart, true);

	a_vm->RegisterFunction("RegisterForActorReanimateStop"sv, Event_AME, RegisterForActorReanimateStop, true);

	a_vm->RegisterFunction("RegisterForActorResurrected"sv, Event_AME, RegisterForActorResurrected, true);

	a_vm->RegisterFunction("RegisterForBookRead"sv, Event_AME, RegisterForBookRead, true);

	a_vm->RegisterFunction("RegisterForCellFullyLoaded"sv, Event_AME, RegisterForCellFullyLoaded, true);

	a_vm->RegisterFunction("RegisterForCriticalHit"sv, Event_AME, RegisterForCriticalHit, true);

	a_vm->RegisterFunction("RegisterForDisarmed"sv, Event_AME, RegisterForDisarmed, true);

	a_vm->RegisterFunction("RegisterForDragonSoulGained"sv, Event_AME, RegisterForDragonSoulGained, true);

	a_vm->RegisterFunction("RegisterForItemHarvested"sv, Event_AME, RegisterForItemHarvested, true);

	a_vm->RegisterFunction("RegisterForLevelIncrease"sv, Event_AME, RegisterForLevelIncrease, true);

	a_vm->RegisterFunction("RegisterForLocationDiscovery"sv, Event_AME, RegisterForLocationDiscovery, true);

	a_vm->RegisterFunction("RegisterForMagicEffectApplyEx"sv, Event_AME, RegisterForMagicEffectApplyEx, true);

	a_vm->RegisterFunction("RegisterForObjectGrab"sv, Event_AME, RegisterForObjectGrab, true);

	a_vm->RegisterFunction("RegisterForObjectLoaded"sv, Event_AME, RegisterForObjectLoaded, true);

	a_vm->RegisterFunction("RegisterForQuest"sv, Event_AME, RegisterForQuest, true);

	a_vm->RegisterFunction("RegisterForQuestStage"sv, Event_AME, RegisterForQuestStage, true);

	a_vm->RegisterFunction("RegisterForShoutAttack"sv, Event_AME, RegisterForShoutAttack, true);

	a_vm->RegisterFunction("RegisterForSkillIncrease"sv, Event_AME, RegisterForSkillIncrease, true);

	a_vm->RegisterFunction("RegisterForSoulTrapped"sv, Event_AME, RegisterForSoulTrapped, true);

	a_vm->RegisterFunction("RegisterForSpellLearned"sv, Event_AME, RegisterForSpellLearned, true);

	a_vm->RegisterFunction("RegisterForWeatherChange"sv, Event_AME, RegisterForWeatherChange, true);


	a_vm->RegisterFunction("UnregisterForActorKilled"sv, Event_AME, UnregisterForActorKilled, true);

	a_vm->RegisterFunction("UnregisterForFECReset"sv, Event_AME, UnregisterForFECReset, true);

	a_vm->RegisterFunction("UnregisterForAllFECResets"sv, Event_AME, UnregisterForAllFECResets, true);

	a_vm->RegisterFunction("UnregisterForActorReanimateStart"sv, Event_AME, UnregisterForActorReanimateStart, true);

	a_vm->RegisterFunction("UnregisterForActorReanimateStop"sv, Event_AME, UnregisterForActorReanimateStop, true);

	a_vm->RegisterFunction("UnregisterForActorResurrected"sv, Event_AME, UnregisterForActorResurrected, true);

	a_vm->RegisterFunction("UnregisterForBookRead"sv, Event_AME, UnregisterForBookRead, true);

	a_vm->RegisterFunction("UnregisterForCellFullyLoaded"sv, Event_AME, UnregisterForCellFullyLoaded, true);

	a_vm->RegisterFunction("UnregisterForCriticalHit"sv, Event_AME, UnregisterForCriticalHit, true);

	a_vm->RegisterFunction("UnregisterForDisarmed"sv, Event_AME, UnregisterForDisarmed, true);

	a_vm->RegisterFunction("UnregisterForDragonSoulGained"sv, Event_AME, UnregisterForDragonSoulGained, true);

	a_vm->RegisterFunction("UnregisterForItemHarvested"sv, Event_AME, UnregisterForItemHarvested, true);

	a_vm->RegisterFunction("UnregisterForLevelIncrease"sv, Event_AME, UnregisterForLevelIncrease, true);

	a_vm->RegisterFunction("UnregisterForLocationDiscovery"sv, Event_AME, UnregisterForLocationDiscovery, true);

	a_vm->RegisterFunction("UnregisterForMagicEffectApplyEx"sv, Event_AME, UnregisterForMagicEffectApplyEx, true);

	a_vm->RegisterFunction("UnregisterForAllMagicEffectApplyEx"sv, Event_AME, UnregisterForAllMagicEffectApplyEx, true);

	a_vm->RegisterFunction("UnregisterForObjectGrab"sv, Event_AME, UnregisterForObjectGrab, true);

	a_vm->RegisterFunction("UnregisterForObjectLoaded"sv, Event_AME, UnregisterForObjectLoaded, true);

	a_vm->RegisterFunction("UnregisterForAllObjectsLoaded"sv, Event_AME, UnregisterForAllObjectsLoaded, true);

	a_vm->RegisterFunction("UnregisterForQuest"sv, Event_AME, UnregisterForQuest, true);

	a_vm->RegisterFunction("UnregisterForAllQuests"sv, Event_AME, UnregisterForAllQuests, true);

	a_vm->RegisterFunction("UnregisterForQuestStage"sv, Event_AME, UnregisterForQuestStage, true);

	a_vm->RegisterFunction("UnregisterForAllQuestStages"sv, Event_AME, UnregisterForAllQuestStages, true);

	a_vm->RegisterFunction("UnregisterForShoutAttack"sv, Event_AME, UnregisterForShoutAttack, true);

	a_vm->RegisterFunction("UnregisterForSkillIncrease"sv, Event_AME, UnregisterForSkillIncrease, true);

	a_vm->RegisterFunction("UnregisterForSoulTrapped"sv, Event_AME, UnregisterForSoulTrapped, true);

	a_vm->RegisterFunction("UnregisterForSpellLearned"sv, Event_AME, UnregisterForSpellLearned, true);

	a_vm->RegisterFunction("UnregisterForWeatherChange"sv, Event_AME, UnregisterForWeatherChange, true);


	return true;
}
