#include "Papyrus/Alias.h"


void papyrusAlias::RegisterForActorKilled(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnActorKillRegSet::GetSingleton();
	regs->Register(a_alias);
}


void papyrusAlias::RegisterForActorReanimateStart(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = HookedEvents::OnActorReanimateStartRegSet::GetSingleton();
	regs->Register(a_alias);
}


void papyrusAlias::RegisterForActorReanimateStop(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = HookedEvents::OnActorReanimateStopRegSet::GetSingleton();
	regs->Register(a_alias);
}


void papyrusAlias::RegisterForActorResurrected(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = HookedEvents::OnActorResurrectRegSet::GetSingleton();
	regs->Register(a_alias);
}


void papyrusAlias::RegisterForBookRead(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnBooksReadRegSet::GetSingleton();
	regs->Register(a_alias);
}


void papyrusAlias::RegisterForCellFullyLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = ScriptEvents::OnCellFullyLoadedRegSet::GetSingleton();
	regs->Register(a_alias);
}


void papyrusAlias::RegisterForCriticalHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnCriticalHitRegSet::GetSingleton();
	regs->Register(a_alias);
}


void papyrusAlias::RegisterForDisarmed(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnDisarmedRegSet::GetSingleton();
	regs->Register(a_alias);
}


void papyrusAlias::RegisterForDragonSoulGained(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnDragonSoulsGainedRegSet::GetSingleton();
	regs->Register(a_alias);
}


void papyrusAlias::RegisterForItemHarvested(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnItemHarvestedRegSet::GetSingleton();
	regs->Register(a_alias);
}


void papyrusAlias::RegisterForLevelIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnLevelIncreaseRegSet::GetSingleton();
	regs->Register(a_alias);
}


void papyrusAlias::RegisterForLocationDiscovery(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnLocationDiscoveryRegSet::GetSingleton();
	regs->Register(a_alias);
}


void papyrusAlias::RegisterForMagicEffectApplyEx(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSRefAlias* a_alias, RE::TESForm* a_effectFilter, bool a_match)
{
	if (!a_alias) {
		a_vm->TraceStack("Reference Alias is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_effectFilter) {
		a_vm->TraceStack("Effect Filter is None", a_stackID, Severity::kWarning);
		return;
	}

	auto key = std::make_pair(a_effectFilter->GetFormID(), a_match);
	auto regs = HookedEvents::OnMagicEffectApplyRegMap::GetSingleton();
	regs->Register(a_alias, key);
}


void papyrusAlias::RegisterForObjectGrab(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto grab = ScriptEvents::OnGrabRegSet::GetSingleton();
	grab->Register(a_alias);

	auto release = ScriptEvents::OnReleaseRegSet::GetSingleton();
	release->Register(a_alias);
}


void papyrusAlias::RegisterForObjectLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias, std::uint32_t a_formType)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto formType = static_cast<RE::FormType>(a_formType);

	auto load = ScriptEvents::OnObjectLoadedRegMap::GetSingleton();
	load->Register(a_alias, formType);

	auto unload = ScriptEvents::OnObjectUnloadedRegMap::GetSingleton();
	unload->Register(a_alias, formType);
}


void papyrusAlias::RegisterForQuest(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias, RE::TESQuest* a_quest)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}
    if (!a_quest) {
        a_vm->TraceStack("Quest is None", a_stackID, Severity::kWarning);
        return;
    }

    auto start = ScriptEvents::OnQuestStartRegMap::GetSingleton();
	start->Register(a_alias, a_quest->GetFormID());

	auto stop = ScriptEvents::OnQuestStopRegMap::GetSingleton();
	stop->Register(a_alias, a_quest->GetFormID());
}


void papyrusAlias::RegisterForQuestStage(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias, RE::TESQuest* a_quest)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}
    if (!a_quest) {
        a_vm->TraceStack("Quest is None", a_stackID, Severity::kWarning);
        return;
    }

    auto regs = ScriptEvents::OnQuestStageRegMap::GetSingleton();
	regs->Register(a_alias, a_quest->GetFormID());
}


void papyrusAlias::RegisterForShoutAttack(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnShoutAttackRegSet::GetSingleton();
	regs->Register(a_alias);
}


void papyrusAlias::RegisterForSkillIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnSkillIncreaseRegSet::GetSingleton();
	regs->Register(a_alias);
}


void papyrusAlias::RegisterForSoulTrapped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnSoulsTrappedRegSet::GetSingleton();
	regs->Register(a_alias);
}

void papyrusAlias::RegisterForSpellLearned(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnSpellsLearnedRegSet::GetSingleton();
	regs->Register(a_alias);
}


void papyrusAlias::RegisterForWeatherChange(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = HookedEvents::OnWeatherChangeRegSet::GetSingleton();
	regs->Register(a_alias);
}


void papyrusAlias::UnregisterForActorKilled(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnActorKillRegSet::GetSingleton();
	regs->Unregister(a_alias);
}


void papyrusAlias::UnregisterForActorReanimateStart(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = HookedEvents::OnActorReanimateStartRegSet::GetSingleton();
	regs->Unregister(a_alias);
}


void papyrusAlias::UnregisterForActorReanimateStop(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = HookedEvents::OnActorReanimateStopRegSet::GetSingleton();
	regs->Unregister(a_alias);
}


void papyrusAlias::UnregisterForActorResurrected(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = HookedEvents::OnActorResurrectRegSet::GetSingleton();
	regs->Unregister(a_alias);
}


void papyrusAlias::UnregisterForBookRead(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnBooksReadRegSet::GetSingleton();
	regs->Unregister(a_alias);
}


void papyrusAlias::UnregisterForCellFullyLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = ScriptEvents::OnCellFullyLoadedRegSet::GetSingleton();
	regs->Unregister(a_alias);
}


void papyrusAlias::UnregisterForCriticalHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnCriticalHitRegSet::GetSingleton();
	regs->Unregister(a_alias);
}


void papyrusAlias::UnregisterForDisarmed(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnDisarmedRegSet::GetSingleton();
	regs->Unregister(a_alias);
}


void papyrusAlias::UnregisterForDragonSoulGained(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnDragonSoulsGainedRegSet::GetSingleton();
	regs->Unregister(a_alias);
}


void papyrusAlias::UnregisterForItemHarvested(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnItemHarvestedRegSet::GetSingleton();
	regs->Unregister(a_alias);
}


void papyrusAlias::UnregisterForLevelIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnLevelIncreaseRegSet::GetSingleton();
	regs->Unregister(a_alias);
}


void papyrusAlias::UnregisterForLocationDiscovery(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnLocationDiscoveryRegSet::GetSingleton();
	regs->Unregister(a_alias);
}


void papyrusAlias::UnregisterForMagicEffectApplyEx(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSRefAlias* a_alias, RE::TESForm* a_effectFilter, bool a_match)
{
	if (!a_alias) {
		a_vm->TraceStack("Reference Alias is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_effectFilter) {
		a_vm->TraceStack("Effect Filter is None", a_stackID, Severity::kWarning);
		return;
	}

	auto key = std::make_pair(a_effectFilter->GetFormID(), a_match);
	auto regs = HookedEvents::OnMagicEffectApplyRegMap::GetSingleton();
	regs->Unregister(a_alias, key);
}


void papyrusAlias::UnregisterForAllMagicEffectApplyEx(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSRefAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Reference Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = HookedEvents::OnMagicEffectApplyRegMap::GetSingleton();
	regs->UnregisterAll(a_alias);
}


void papyrusAlias::UnregisterForObjectGrab(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto grab = ScriptEvents::OnGrabRegSet::GetSingleton();
	grab->Unregister(a_alias);

	auto release = ScriptEvents::OnReleaseRegSet::GetSingleton();
	release->Unregister(a_alias);
}


void papyrusAlias::UnregisterForObjectLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias, std::uint32_t a_formType)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto formType = static_cast<RE::FormType>(a_formType);

	auto load = ScriptEvents::OnObjectLoadedRegMap::GetSingleton();
	load->Unregister(a_alias, formType);

	auto unload = ScriptEvents::OnObjectUnloadedRegMap::GetSingleton();
	unload->Unregister(a_alias, formType);
}


void papyrusAlias::UnregisterForAllObjectsLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto load = ScriptEvents::OnObjectLoadedRegMap::GetSingleton();
	load->UnregisterAll(a_alias);

	auto unload = ScriptEvents::OnObjectUnloadedRegMap::GetSingleton();
	unload->UnregisterAll(a_alias);
}


void papyrusAlias::UnregisterForQuest(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias, RE::TESQuest* a_quest)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}
    if (!a_quest) {
        a_vm->TraceStack("Quest is None", a_stackID, Severity::kWarning);
        return;
    }

    auto start = ScriptEvents::OnQuestStartRegMap::GetSingleton();
	start->Unregister(a_alias, a_quest->GetFormID());

	auto stop = ScriptEvents::OnQuestStartRegMap::GetSingleton();
	stop->Unregister(a_alias, a_quest->GetFormID());
}


void papyrusAlias::UnregisterForAllQuests(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto start = ScriptEvents::OnQuestStartRegMap::GetSingleton();
	start->UnregisterAll(a_alias);

	auto stop = ScriptEvents::OnQuestStartRegMap::GetSingleton();
	stop->UnregisterAll(a_alias);
}


void papyrusAlias::UnregisterForQuestStage(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias, RE::TESQuest* a_quest)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}
    if (!a_quest) {
        a_vm->TraceStack("Quest is None", a_stackID, Severity::kWarning);
        return;
    }

    auto regs = ScriptEvents::OnQuestStageRegMap::GetSingleton();
	regs->Unregister(a_alias, a_quest->GetFormID());
}


void papyrusAlias::UnregisterForAllQuestStages(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = ScriptEvents::OnQuestStageRegMap::GetSingleton();
	regs->UnregisterAll(a_alias);
}


void papyrusAlias::UnregisterForShoutAttack(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnShoutAttackRegSet::GetSingleton();
	regs->Unregister(a_alias);
}


void papyrusAlias::UnregisterForSkillIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnShoutAttackRegSet::GetSingleton();
	regs->Unregister(a_alias);
}


void papyrusAlias::UnregisterForSoulTrapped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnSoulsTrappedRegSet::GetSingleton();
	regs->Unregister(a_alias);
}


void papyrusAlias::UnregisterForSpellLearned(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnSpellsLearnedRegSet::GetSingleton();
	regs->Unregister(a_alias);
}


void papyrusAlias::UnregisterForWeatherChange(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
{
	if (!a_alias) {
		a_vm->TraceStack("Alias is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = HookedEvents::OnWeatherChangeRegSet::GetSingleton();
	regs->Unregister(a_alias);
}


auto papyrusAlias::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusAlias - couldn't get VMState"sv);
		return false;
	}

	auto constexpr Event_Alias = "PO3_Events_Alias"sv;

	a_vm->RegisterFunction("RegisterForActorKilled"sv, Event_Alias, RegisterForActorKilled, true);

	a_vm->RegisterFunction("RegisterForActorReanimateStart"sv, Event_Alias, RegisterForActorReanimateStart, true);

	a_vm->RegisterFunction("RegisterForActorReanimateStop"sv, Event_Alias, RegisterForActorReanimateStop, true);

	a_vm->RegisterFunction("RegisterForActorResurrected"sv, Event_Alias, RegisterForActorResurrected, true);

	a_vm->RegisterFunction("RegisterForBookRead"sv, Event_Alias, RegisterForBookRead, true);

	a_vm->RegisterFunction("RegisterForCellFullyLoaded"sv, Event_Alias, RegisterForCellFullyLoaded, true);

	a_vm->RegisterFunction("RegisterForCriticalHit"sv, Event_Alias, RegisterForCriticalHit, true);

	a_vm->RegisterFunction("RegisterForDisarmed"sv, Event_Alias, RegisterForDisarmed, true);

	a_vm->RegisterFunction("RegisterForDragonSoulGained"sv, Event_Alias, RegisterForDragonSoulGained, true);

	a_vm->RegisterFunction("RegisterForItemHarvested"sv, Event_Alias, RegisterForItemHarvested, true);

	a_vm->RegisterFunction("RegisterForLevelIncrease"sv, Event_Alias, RegisterForLevelIncrease, true);

	a_vm->RegisterFunction("RegisterForLocationDiscovery"sv, Event_Alias, RegisterForLocationDiscovery, true);

	a_vm->RegisterFunction("RegisterForMagicEffectApplyEx"sv, Event_Alias, RegisterForMagicEffectApplyEx, true);

	a_vm->RegisterFunction("RegisterForObjectGrab"sv, Event_Alias, RegisterForObjectGrab, true);

	a_vm->RegisterFunction("RegisterForObjectLoaded"sv, Event_Alias, RegisterForObjectLoaded, true);

	a_vm->RegisterFunction("RegisterForQuest"sv, Event_Alias, RegisterForQuest, true);

	a_vm->RegisterFunction("RegisterForQuestStage"sv, Event_Alias, RegisterForQuestStage, true);

	a_vm->RegisterFunction("RegisterForShoutAttack"sv, Event_Alias, RegisterForShoutAttack, true);

	a_vm->RegisterFunction("RegisterForSkillIncrease"sv, Event_Alias, RegisterForSkillIncrease, true);

	a_vm->RegisterFunction("RegisterForSoulTrapped"sv, Event_Alias, RegisterForSoulTrapped, true);

	a_vm->RegisterFunction("RegisterForSpellLearned"sv, Event_Alias, RegisterForSpellLearned, true);

	a_vm->RegisterFunction("RegisterForWeatherChange"sv, Event_Alias, RegisterForWeatherChange, true);


	a_vm->RegisterFunction("UnregisterForActorKilled"sv, Event_Alias, UnregisterForActorKilled, true);

	a_vm->RegisterFunction("UnregisterForActorReanimateStart"sv, Event_Alias, UnregisterForActorReanimateStart, true);

	a_vm->RegisterFunction("UnregisterForActorReanimateStop"sv, Event_Alias, UnregisterForActorReanimateStop, true);

	a_vm->RegisterFunction("UnregisterForActorResurrected"sv, Event_Alias, UnregisterForActorResurrected, true);

	a_vm->RegisterFunction("UnregisterForBookRead"sv, Event_Alias, UnregisterForBookRead, true);

	a_vm->RegisterFunction("UnregisterForCellFullyLoaded"sv, Event_Alias, UnregisterForCellFullyLoaded, true);

	a_vm->RegisterFunction("UnregisterForCriticalHit"sv, Event_Alias, UnregisterForCriticalHit, true);

	a_vm->RegisterFunction("UnregisterForDisarmed"sv, Event_Alias, UnregisterForDisarmed, true);

	a_vm->RegisterFunction("UnregisterForDragonSoulGained"sv, Event_Alias, UnregisterForDragonSoulGained, true);

	a_vm->RegisterFunction("UnregisterForItemHarvested"sv, Event_Alias, UnregisterForItemHarvested, true);

	a_vm->RegisterFunction("UnregisterForLevelIncrease"sv, Event_Alias, UnregisterForLevelIncrease, true);

	a_vm->RegisterFunction("UnregisterForLocationDiscovery"sv, Event_Alias, UnregisterForLocationDiscovery, true);

	a_vm->RegisterFunction("UnregisterForMagicEffectApplyEx"sv, Event_Alias, UnregisterForMagicEffectApplyEx, true);

	a_vm->RegisterFunction("UnregisterForAllMagicEffectApplyEx"sv, Event_Alias, UnregisterForAllMagicEffectApplyEx, true);

	a_vm->RegisterFunction("UnregisterForObjectGrab"sv, Event_Alias, UnregisterForObjectGrab, true);

	a_vm->RegisterFunction("UnregisterForObjectLoaded"sv, Event_Alias, UnregisterForObjectLoaded, true);

	a_vm->RegisterFunction("UnregisterForAllObjectsLoaded"sv, Event_Alias, UnregisterForAllObjectsLoaded, true);

	a_vm->RegisterFunction("UnregisterForQuest"sv, Event_Alias, UnregisterForQuest, true);

	a_vm->RegisterFunction("UnregisterForAllQuests"sv, Event_Alias, UnregisterForAllQuests, true);

	a_vm->RegisterFunction("UnregisterForQuestStage"sv, Event_Alias, UnregisterForQuestStage, true);

	a_vm->RegisterFunction("UnregisterForAllQuestStages"sv, Event_Alias, UnregisterForAllQuestStages, true);

	a_vm->RegisterFunction("UnregisterForShoutAttack"sv, Event_Alias, UnregisterForShoutAttack, true);

	a_vm->RegisterFunction("UnregisterForSkillIncrease"sv, Event_Alias, UnregisterForSkillIncrease, true);

	a_vm->RegisterFunction("UnregisterForSoulTrapped"sv, Event_Alias, UnregisterForSoulTrapped, true);

	a_vm->RegisterFunction("UnregisterForSpellLearned"sv, Event_Alias, UnregisterForSpellLearned, true);

	a_vm->RegisterFunction("UnregisterForWeatherChange"sv, Event_Alias, UnregisterForWeatherChange, true);

	return true;
}
