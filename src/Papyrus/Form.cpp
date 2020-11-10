#include "Papyrus/Form.h"

#include "Serialization/Form/Keywords.h"


void papyrusForm::AddKeywordToForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form, RE::BGSKeyword* a_add)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_add) {
		a_vm->TraceStack("Keyword is None", a_stackID, Severity::kWarning);
		return;
	}

	auto keywords = Form::Keywords::GetSingleton();
	keywords->PapyrusApply(a_form, a_add, Form::kAdd);
}


bool papyrusForm::IsGeneratedForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return false;
	}

	return a_form->IsDynamicForm();
}


bool papyrusForm::RemoveKeywordOnForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form, RE::BGSKeyword* a_remove)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return false;
	} else if (!a_remove) {
		a_vm->TraceStack("Keyword is None", a_stackID, Severity::kWarning);
		return false;
	}

	auto keywords = Form::Keywords::GetSingleton();
	return keywords->PapyrusApply(a_form, a_remove, Form::kRemove);
}


void papyrusForm::ReplaceKeywordOnForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form, RE::BGSKeyword* a_remove, RE::BGSKeyword* a_add)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_remove) {
		a_vm->TraceStack("Remove keyword is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_add) {
		a_vm->TraceStack("Add keyword is None", a_stackID, Severity::kWarning);
		return;
	}

	auto keywordForm = a_form->As<RE::BGSKeywordForm>();
	if (keywordForm) {
		std::uint32_t removeIndex = 0;
		bool found = false;
		if (keywordForm->keywords) {
			for (std::uint32_t i = 0; i < keywordForm->numKeywords; i++) {
				auto keyword = keywordForm->keywords[i];
				if (keyword) {
					if (keyword == a_add) {
						return;
					}
					if (keyword == a_remove) {
						removeIndex = i;
						found = true;
					}
				}
			}
			if (found) {
				keywordForm->keywords[removeIndex] = a_add;
			}
		}
	}
}



void papyrusForm::RegisterForActorKilled(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnActorKillRegSet::GetSingleton();
	regs->Register(a_form);
}


void papyrusForm::RegisterForBookRead(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnBooksReadRegSet::GetSingleton();
	regs->Register(a_form);
}


void papyrusForm::RegisterForCellFullyLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = ScriptEvents::OnCellFullyLoadedRegSet::GetSingleton();
	regs->Register(a_form);
}


void papyrusForm::RegisterForCriticalHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnCriticalHitRegSet::GetSingleton();
	regs->Register(a_form);
}


void papyrusForm::RegisterForDisarmed(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnDisarmedRegSet::GetSingleton();
	regs->Register(a_form);
}


void papyrusForm::RegisterForDragonSoulGained(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnDragonSoulsGainedRegSet::GetSingleton();
	regs->Register(a_form);
}


void papyrusForm::RegisterForItemHarvested(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnItemHarvestedRegSet::GetSingleton();
	regs->Register(a_form);
}


void papyrusForm::RegisterForLevelIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnLevelIncreaseRegSet::GetSingleton();
	regs->Register(a_form);
}


void papyrusForm::RegisterForLocationDiscovery(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnLocationDiscoveryRegSet::GetSingleton();
	regs->Register(a_form);
}


void papyrusForm::RegisterForObjectGrab(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto grab = ScriptEvents::OnGrabRegSet::GetSingleton();
	grab->Register(a_form);

	auto release = ScriptEvents::OnReleaseRegSet::GetSingleton();
	release->Register(a_form);
}


void papyrusForm::RegisterForObjectLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form, std::uint32_t a_formType)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto formType = static_cast<RE::FormType>(a_formType);

	auto load = ScriptEvents::OnObjectLoadedRegMap::GetSingleton();
	load->Register(a_form, formType);

	auto unload = ScriptEvents::OnObjectUnloadedRegMap::GetSingleton();
	unload->Register(a_form, formType);
}


void papyrusForm::RegisterForQuest(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form, RE::TESQuest* a_quest)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_quest) {
		a_vm->TraceStack("Quest is None", a_stackID, Severity::kWarning);
		return;
	}

	auto start = ScriptEvents::OnQuestStartRegMap::GetSingleton();
	start->Register(a_form, a_quest->GetFormID());

	auto stop = ScriptEvents::OnQuestStopRegMap::GetSingleton();
	stop->Register(a_form, a_quest->GetFormID());
}


void papyrusForm::RegisterForQuestStage(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form, RE::TESQuest* a_quest)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_quest) {
		a_vm->TraceStack("Quest is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = ScriptEvents::OnQuestStageRegMap::GetSingleton();
	regs->Register(a_form, a_quest->GetFormID());
}


void papyrusForm::RegisterForShoutAttack(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnShoutAttackRegSet::GetSingleton();
	regs->Register(a_form);
}


void papyrusForm::RegisterForSkillIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnSkillIncreaseRegSet::GetSingleton();
	regs->Register(a_form);
}


void papyrusForm::RegisterForSoulTrapped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnSoulsTrappedRegSet::GetSingleton();
	regs->Register(a_form);
}

void papyrusForm::RegisterForSpellLearned(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnSpellsLearnedRegSet::GetSingleton();
	regs->Register(a_form);
}


void papyrusForm::UnregisterForActorKilled(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnActorKillRegSet::GetSingleton();
	regs->Unregister(a_form);
}


void papyrusForm::UnregisterForBookRead(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnBooksReadRegSet::GetSingleton();
	regs->Unregister(a_form);
}


void papyrusForm::UnregisterForCellFullyLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = ScriptEvents::OnCellFullyLoadedRegSet::GetSingleton();
	regs->Unregister(a_form);
}


void papyrusForm::UnregisterForCriticalHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnCriticalHitRegSet::GetSingleton();
	regs->Unregister(a_form);
}


void papyrusForm::UnregisterForDisarmed(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnDisarmedRegSet::GetSingleton();
	regs->Unregister(a_form);
}


void papyrusForm::UnregisterForDragonSoulGained(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnDragonSoulsGainedRegSet::GetSingleton();
	regs->Unregister(a_form);
}


void papyrusForm::UnregisterForItemHarvested(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnItemHarvestedRegSet::GetSingleton();
	regs->Unregister(a_form);
}


void papyrusForm::UnregisterForLevelIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnLevelIncreaseRegSet::GetSingleton();
	regs->Unregister(a_form);
}


void papyrusForm::UnregisterForLocationDiscovery(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnLocationDiscoveryRegSet::GetSingleton();
	regs->Unregister(a_form);
}


void papyrusForm::UnregisterForObjectGrab(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto grab = ScriptEvents::OnGrabRegSet::GetSingleton();
	grab->Unregister(a_form);

	auto release = ScriptEvents::OnReleaseRegSet::GetSingleton();
	release->Unregister(a_form);
}


void papyrusForm::UnregisterForObjectLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form, std::uint32_t a_formType)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto formType = static_cast<RE::FormType>(a_formType);

	auto load = ScriptEvents::OnObjectLoadedRegMap::GetSingleton();
	load->Unregister(a_form, formType);

	auto unload = ScriptEvents::OnObjectUnloadedRegMap::GetSingleton();
	unload->Unregister(a_form, formType);
}


void papyrusForm::UnregisterForAllObjectsLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto load = ScriptEvents::OnObjectLoadedRegMap::GetSingleton();
	load->UnregisterAll(a_form);

	auto unload = ScriptEvents::OnObjectUnloadedRegMap::GetSingleton();
	unload->UnregisterAll(a_form);
}


void papyrusForm::UnregisterForQuest(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form, RE::TESQuest* a_quest)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_quest) {
		a_vm->TraceStack("Quest is None", a_stackID, Severity::kWarning);
		return;
	}

	auto start = ScriptEvents::OnQuestStartRegMap::GetSingleton();
	start->Unregister(a_form, a_quest->GetFormID());

	auto stop = ScriptEvents::OnQuestStartRegMap::GetSingleton();
	stop->Unregister(a_form, a_quest->GetFormID());
}


void papyrusForm::UnregisterForAllQuests(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto start = ScriptEvents::OnQuestStartRegMap::GetSingleton();
	start->UnregisterAll(a_form);

	auto stop = ScriptEvents::OnQuestStartRegMap::GetSingleton();
	stop->UnregisterAll(a_form);
}


void papyrusForm::UnregisterForQuestStage(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form, RE::TESQuest* a_quest)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_quest) {
		a_vm->TraceStack("Quest is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = ScriptEvents::OnQuestStageRegMap::GetSingleton();
	regs->Unregister(a_form, a_quest->GetFormID());
}


void papyrusForm::UnregisterForAllQuestStages(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = ScriptEvents::OnQuestStageRegMap::GetSingleton();
	regs->UnregisterAll(a_form);
}


void papyrusForm::UnregisterForShoutAttack(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnShoutAttackRegSet::GetSingleton();
	regs->Unregister(a_form);
}


void papyrusForm::UnregisterForSkillIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnShoutAttackRegSet::GetSingleton();
	regs->Unregister(a_form);
}


void papyrusForm::UnregisterForSoulTrapped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnSoulsTrappedRegSet::GetSingleton();
	regs->Unregister(a_form);
}


void papyrusForm::UnregisterForSpellLearned(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = StoryEvents::OnSpellsLearnedRegSet::GetSingleton();
	regs->Unregister(a_form);
}


bool papyrusForm::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("papyrusForm - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("AddKeywordToForm", "PO3_SKSEFunctions", AddKeywordToForm);

	a_vm->RegisterFunction("IsGeneratedForm", "PO3_SKSEFunctions", IsGeneratedForm, true);

	a_vm->RegisterFunction("RemoveKeywordOnForm", "PO3_SKSEFunctions", RemoveKeywordOnForm);

	a_vm->RegisterFunction("ReplaceKeywordOnForm", "PO3_SKSEFunctions", ReplaceKeywordOnForm);


	a_vm->RegisterFunction("RegisterForActorKilled", "PO3_Events_Form", RegisterForActorKilled, true);

	a_vm->RegisterFunction("RegisterForBookRead", "PO3_Events_Form", RegisterForBookRead, true);

	a_vm->RegisterFunction("RegisterForCellFullyLoaded", "PO3_Events_Form", RegisterForCellFullyLoaded, true);

	a_vm->RegisterFunction("RegisterForCriticalHit", "PO3_Events_Form", RegisterForCriticalHit, true);

	a_vm->RegisterFunction("RegisterForDisarmed", "PO3_Events_Form", RegisterForDisarmed, true);

	a_vm->RegisterFunction("RegisterForDragonSoulGained", "PO3_Events_Form", RegisterForDragonSoulGained, true);

	a_vm->RegisterFunction("RegisterForItemHarvested", "PO3_Events_Form", RegisterForItemHarvested, true);

	a_vm->RegisterFunction("RegisterForLevelIncrease", "PO3_Events_Form", RegisterForLevelIncrease, true);

	a_vm->RegisterFunction("RegisterForLocationDiscovery", "PO3_Events_Form", RegisterForLocationDiscovery, true);

	a_vm->RegisterFunction("RegisterForObjectGrab", "PO3_Events_Form", RegisterForObjectGrab, true);

	a_vm->RegisterFunction("RegisterForObjectLoaded", "PO3_Events_Form", RegisterForObjectLoaded, true);

	a_vm->RegisterFunction("RegisterForQuest", "PO3_Events_Form", RegisterForQuest, true);

	a_vm->RegisterFunction("RegisterForQuestStage", "PO3_Events_Form", RegisterForQuestStage, true);

	a_vm->RegisterFunction("RegisterForShoutAttack", "PO3_Events_Form", RegisterForShoutAttack, true);

	a_vm->RegisterFunction("RegisterForSkillIncrease", "PO3_Events_Form", RegisterForSkillIncrease, true);

	a_vm->RegisterFunction("RegisterForSoulTrapped", "PO3_Events_Form", RegisterForSoulTrapped, true);

	a_vm->RegisterFunction("RegisterForSpellLearned", "PO3_Events_Form", RegisterForSpellLearned, true);


	a_vm->RegisterFunction("UnregisterForActorKilled", "PO3_Events_Form", UnregisterForActorKilled, true);

	a_vm->RegisterFunction("UnregisterForBookRead", "PO3_Events_Form", UnregisterForBookRead, true);

	a_vm->RegisterFunction("UnregisterForCellFullyLoaded", "PO3_Events_Form", UnregisterForCellFullyLoaded, true);

	a_vm->RegisterFunction("UnregisterForCriticalHit", "PO3_Events_Form", UnregisterForCriticalHit, true);

	a_vm->RegisterFunction("UnregisterForDisarmed", "PO3_Events_Form", UnregisterForDisarmed, true);

	a_vm->RegisterFunction("UnregisterForDragonSoulGained", "PO3_Events_Form", UnregisterForDragonSoulGained, true);

	a_vm->RegisterFunction("UnregisterForItemHarvested", "PO3_Events_Form", UnregisterForItemHarvested, true);

	a_vm->RegisterFunction("UnregisterForLevelIncrease", "PO3_Events_Form", UnregisterForLevelIncrease, true);

	a_vm->RegisterFunction("UnregisterForLocationDiscovery", "PO3_Events_Form", UnregisterForLocationDiscovery, true);

	a_vm->RegisterFunction("UnregisterForObjectGrab", "PO3_Events_Form", UnregisterForObjectGrab, true);

	a_vm->RegisterFunction("UnregisterForObjectLoaded", "PO3_Events_Form", UnregisterForObjectLoaded, true);

	a_vm->RegisterFunction("UnregisterForAllObjectsLoaded", "PO3_Events_Form", UnregisterForAllObjectsLoaded, true);

	a_vm->RegisterFunction("UnregisterForQuest", "PO3_Events_Form", UnregisterForQuest, true);

	a_vm->RegisterFunction("UnregisterForAllQuests", "PO3_Events_Form", UnregisterForAllQuests, true);

	a_vm->RegisterFunction("UnregisterForQuestStage", "PO3_Events_Form", UnregisterForQuestStage, true);

	a_vm->RegisterFunction("UnregisterForAllQuestStages", "PO3_Events_Form", UnregisterForAllQuestStages, true);

	a_vm->RegisterFunction("UnregisterForShoutAttack", "PO3_Events_Form", UnregisterForShoutAttack, true);

	a_vm->RegisterFunction("UnregisterForSkillIncrease", "PO3_Events_Form", UnregisterForSkillIncrease, true);

	a_vm->RegisterFunction("UnregisterForSoulTrapped", "PO3_Events_Form", UnregisterForSoulTrapped, true);

	a_vm->RegisterFunction("UnregisterForSpellLearned", "PO3_Events_Form", UnregisterForSpellLearned, true);

	return true;
}
