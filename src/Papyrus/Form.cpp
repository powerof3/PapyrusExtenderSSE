#include "Papyrus/Form.h"

#include "Serialization/Form/Keywords.h"
#include "Util/ConditionParser.h"


void papyrusForm::AddKeywordToForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form, RE::BGSKeyword* a_add)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_add) {
		a_vm->TraceStack("Keyword is None", a_stackID, Severity::kWarning);
		return;
	}

	Form::Keywords::GetSingleton()->PapyrusApply(a_form, a_add, Form::kAdd);
}


auto papyrusForm::EvaluateConditionList(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form, RE::TESObjectREFR* a_actionRef, RE::TESObjectREFR* a_target) -> bool
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return false;
	}
	if (!a_actionRef) {
		a_vm->TraceStack("Source is None", a_stackID, Severity::kWarning);
		return false;
	}
	if (!a_target) {
		a_vm->TraceStack("Target is None", a_stackID, Severity::kWarning);
		return false;
	}

	bool result = false;

	switch (a_form->GetFormType()) {
	case RE::FormType::Spell:
	case RE::FormType::Enchantment:
	case RE::FormType::Ingredient:
	case RE::FormType::AlchemyItem:
	case RE::FormType::Scroll:
		{
			auto magicItem = a_form->As<RE::MagicItem>();
			if (magicItem) {
				for (auto& effect : magicItem->effects) {
					if (result) {
						break;
					}
					
					if (effect) {
						bool effectValid = false;
						bool mgefValid = false;
						
						effectValid = effect->conditions.IsTrue(a_actionRef, a_target);
						
						auto baseEffect = effect->baseEffect;
						mgefValid = baseEffect && baseEffect->conditions.IsTrue(a_actionRef, a_target);
						
						if (effectValid && mgefValid) {
							result = true;
						}
					}
				}
			}
		}
		break;
	case RE::FormType::MagicEffect:
		{
			auto effect = a_form->As<RE::EffectSetting>();
			if (effect && effect->conditions.IsTrue(a_actionRef, a_target)) {
				result = true;
			}
		}
		break;
	default:
		result = true;
		break;
	}

	return result;
}


auto papyrusForm::GetConditionList(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form, std::uint32_t a_index) -> std::vector<RE::BSFixedString>
{
	std::vector<RE::BSFixedString> a_vec;

	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return a_vec;
	}
	auto condition = Condition::GetCondition(*a_form, a_index);
	if (!condition) {
		a_vm->TraceStack("Form does not have a condition stack", a_stackID, Severity::kWarning);
		return a_vec;
	}

	return Condition::BuildConditions(condition);
}


auto papyrusForm::IsGeneratedForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form) -> bool
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return false;
	}

	return a_form->IsDynamicForm();
}


auto papyrusForm::RemoveKeywordOnForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form, RE::BGSKeyword* a_remove) -> bool
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return false;
	}
	if (!a_remove) {
		a_vm->TraceStack("Keyword is None", a_stackID, Severity::kWarning);
		return false;
	}

	return Form::Keywords::GetSingleton()->PapyrusApply(a_form, a_remove, Form::kRemove);
}


void papyrusForm::ReplaceKeywordOnForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form, RE::BGSKeyword* a_remove, RE::BGSKeyword* a_add)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_remove) {
		a_vm->TraceStack("Remove keyword is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_add) {
		a_vm->TraceStack("Add keyword is None", a_stackID, Severity::kWarning);
		return;
	}

	if (const auto keywordForm = a_form->As<RE::BGSKeywordForm>(); keywordForm) {
		std::uint32_t removeIndex = 0;
		bool found = false;
		if (keywordForm->keywords) {
			for (std::uint32_t i = 0; i < keywordForm->numKeywords; i++) {
				const auto keyword = keywordForm->keywords[i];
				if (keyword) {
					if (keyword == a_add) {
						return;
					}
					if (keyword == a_remove) {
						removeIndex = i;
						found = true;
						break;
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


void papyrusForm::RegisterForFECReset(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form, std::uint32_t a_type)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = FECEvents::OnFECResetRegMap::GetSingleton();
	regs->Register(a_form, a_type);
}


void papyrusForm::RegisterForActorReanimateStart(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	/*auto regs = HookedEvents::OnActorReanimateStartRegSet::GetSingleton();
	regs->Register(a_form);*/
}


void papyrusForm::RegisterForActorReanimateStop(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	/*auto regs = HookedEvents::OnActorReanimateStopRegSet::GetSingleton();
	regs->Register(a_form);*/
}


void papyrusForm::RegisterForActorResurrected(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	/*auto regs = HookedEvents::OnActorResurrectRegSet::GetSingleton();
	regs->Register(a_form);*/
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
	}
	if (!a_quest) {
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
	}
	if (!a_quest) {
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


void papyrusForm::RegisterForWeatherChange(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = HookedEvents::OnWeatherChangeRegSet::GetSingleton();
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


void papyrusForm::UnregisterForFECReset(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form, std::uint32_t a_type)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = FECEvents::OnFECResetRegMap::GetSingleton();
	regs->Unregister(a_form, a_type);
}


void papyrusForm::UnregisterForAllFECResets(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = FECEvents::OnFECResetRegMap::GetSingleton();
	regs->UnregisterAll(a_form);
}


void papyrusForm::UnregisterForActorReanimateStart(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	/*auto regs = HookedEvents::OnActorReanimateStartRegSet::GetSingleton();
	regs->Unregister(a_form);*/
}


void papyrusForm::UnregisterForActorReanimateStop(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	/*auto regs = HookedEvents::OnActorReanimateStopRegSet::GetSingleton();
	regs->Unregister(a_form);*/
}


void papyrusForm::UnregisterForActorResurrected(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	/*auto regs = HookedEvents::OnActorResurrectRegSet::GetSingleton();
	regs->Unregister(a_form);*/
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
	}
	if (!a_quest) {
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
	}
	if (!a_quest) {
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


void papyrusForm::UnregisterForWeatherChange(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
{
	if (!a_form) {
		a_vm->TraceStack("Form is None", a_stackID, Severity::kWarning);
		return;
	}

	auto regs = HookedEvents::OnWeatherChangeRegSet::GetSingleton();
	regs->Unregister(a_form);
}


auto papyrusForm::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusForm - couldn't get VMState"sv);
		return false;
	}

	auto constexpr Functions = "PO3_SKSEFunctions"sv;
	auto constexpr Event_Form = "PO3_Events_Form"sv;

	a_vm->RegisterFunction("AddKeywordToForm"sv, Functions, AddKeywordToForm);

	a_vm->RegisterFunction("EvaluateConditionList"sv, Functions, EvaluateConditionList);

	a_vm->RegisterFunction("GetConditionList"sv, Functions, GetConditionList);

	a_vm->RegisterFunction("IsGeneratedForm"sv, Functions, IsGeneratedForm, true);

	a_vm->RegisterFunction("RemoveKeywordOnForm"sv, Functions, RemoveKeywordOnForm);

	a_vm->RegisterFunction("ReplaceKeywordOnForm"sv, Functions, ReplaceKeywordOnForm);


	a_vm->RegisterFunction("RegisterForActorKilled"sv, Event_Form, RegisterForActorKilled, true);

	a_vm->RegisterFunction("RegisterForFECReset"sv, Event_Form, RegisterForFECReset, true);

	a_vm->RegisterFunction("RegisterForActorReanimateStart"sv, Event_Form, RegisterForActorReanimateStart, true);

	a_vm->RegisterFunction("RegisterForActorReanimateStop"sv, Event_Form, RegisterForActorReanimateStop, true);

	a_vm->RegisterFunction("RegisterForActorResurrected"sv, Event_Form, RegisterForActorResurrected, true);

	a_vm->RegisterFunction("RegisterForBookRead"sv, Event_Form, RegisterForBookRead, true);

	a_vm->RegisterFunction("RegisterForCellFullyLoaded"sv, Event_Form, RegisterForCellFullyLoaded, true);

	a_vm->RegisterFunction("RegisterForCriticalHit"sv, Event_Form, RegisterForCriticalHit, true);

	a_vm->RegisterFunction("RegisterForDisarmed"sv, Event_Form, RegisterForDisarmed, true);

	a_vm->RegisterFunction("RegisterForDragonSoulGained"sv, Event_Form, RegisterForDragonSoulGained, true);

	a_vm->RegisterFunction("RegisterForItemHarvested"sv, Event_Form, RegisterForItemHarvested, true);

	a_vm->RegisterFunction("RegisterForLevelIncrease"sv, Event_Form, RegisterForLevelIncrease, true);

	a_vm->RegisterFunction("RegisterForLocationDiscovery"sv, Event_Form, RegisterForLocationDiscovery, true);

	a_vm->RegisterFunction("RegisterForObjectGrab"sv, Event_Form, RegisterForObjectGrab, true);

	a_vm->RegisterFunction("RegisterForObjectLoaded"sv, Event_Form, RegisterForObjectLoaded, true);

	a_vm->RegisterFunction("RegisterForQuest"sv, Event_Form, RegisterForQuest, true);

	a_vm->RegisterFunction("RegisterForQuestStage"sv, Event_Form, RegisterForQuestStage, true);

	a_vm->RegisterFunction("RegisterForShoutAttack"sv, Event_Form, RegisterForShoutAttack, true);

	a_vm->RegisterFunction("RegisterForSkillIncrease"sv, Event_Form, RegisterForSkillIncrease, true);

	a_vm->RegisterFunction("RegisterForSoulTrapped"sv, Event_Form, RegisterForSoulTrapped, true);

	a_vm->RegisterFunction("RegisterForSpellLearned"sv, Event_Form, RegisterForSpellLearned, true);

	a_vm->RegisterFunction("RegisterForWeatherChange"sv, Event_Form, RegisterForWeatherChange, true);


	a_vm->RegisterFunction("UnregisterForActorKilled"sv, Event_Form, UnregisterForActorKilled, true);

	a_vm->RegisterFunction("UnregisterForFECReset"sv, Event_Form, UnregisterForFECReset, true);

	a_vm->RegisterFunction("UnregisterForAllFECResets"sv, Event_Form, UnregisterForAllFECResets, true);

	a_vm->RegisterFunction("UnregisterForActorReanimateStart"sv, Event_Form, UnregisterForActorReanimateStart, true);

	a_vm->RegisterFunction("UnregisterForActorReanimateStop"sv, Event_Form, UnregisterForActorReanimateStop, true);

	a_vm->RegisterFunction("UnregisterForActorResurrected"sv, Event_Form, UnregisterForActorResurrected, true);

	a_vm->RegisterFunction("UnregisterForBookRead"sv, Event_Form, UnregisterForBookRead, true);

	a_vm->RegisterFunction("UnregisterForCellFullyLoaded"sv, Event_Form, UnregisterForCellFullyLoaded, true);

	a_vm->RegisterFunction("UnregisterForCriticalHit"sv, Event_Form, UnregisterForCriticalHit, true);

	a_vm->RegisterFunction("UnregisterForDisarmed"sv, Event_Form, UnregisterForDisarmed, true);

	a_vm->RegisterFunction("UnregisterForDragonSoulGained"sv, Event_Form, UnregisterForDragonSoulGained, true);

	a_vm->RegisterFunction("UnregisterForItemHarvested"sv, Event_Form, UnregisterForItemHarvested, true);

	a_vm->RegisterFunction("UnregisterForLevelIncrease"sv, Event_Form, UnregisterForLevelIncrease, true);

	a_vm->RegisterFunction("UnregisterForLocationDiscovery"sv, Event_Form, UnregisterForLocationDiscovery, true);

	a_vm->RegisterFunction("UnregisterForObjectGrab"sv, Event_Form, UnregisterForObjectGrab, true);

	a_vm->RegisterFunction("UnregisterForObjectLoaded"sv, Event_Form, UnregisterForObjectLoaded, true);

	a_vm->RegisterFunction("UnregisterForAllObjectsLoaded"sv, Event_Form, UnregisterForAllObjectsLoaded, true);

	a_vm->RegisterFunction("UnregisterForQuest"sv, Event_Form, UnregisterForQuest, true);

	a_vm->RegisterFunction("UnregisterForAllQuests"sv, Event_Form, UnregisterForAllQuests, true);

	a_vm->RegisterFunction("UnregisterForQuestStage"sv, Event_Form, UnregisterForQuestStage, true);

	a_vm->RegisterFunction("UnregisterForAllQuestStages"sv, Event_Form, UnregisterForAllQuestStages, true);

	a_vm->RegisterFunction("UnregisterForShoutAttack"sv, Event_Form, UnregisterForShoutAttack, true);

	a_vm->RegisterFunction("UnregisterForSkillIncrease"sv, Event_Form, UnregisterForSkillIncrease, true);

	a_vm->RegisterFunction("UnregisterForSoulTrapped"sv, Event_Form, UnregisterForSoulTrapped, true);

	a_vm->RegisterFunction("UnregisterForSpellLearned"sv, Event_Form, UnregisterForSpellLearned, true);

	a_vm->RegisterFunction("UnregisterForWeatherChange"sv, Event_Form, UnregisterForWeatherChange, true);

	return true;
}
