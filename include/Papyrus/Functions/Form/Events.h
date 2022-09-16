#pragma once

#include "Serialization/EventHolder.h"

namespace Papyrus::Form::Events
{
	inline void RegisterForActorFallLongDistance(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->actorFallLongDistance;
		regs.Register(a_form);
	}

	inline void RegisterForActorKilled(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->actorKill;
		regs.Register(a_form);
	}

	inline void RegisterForActorReanimateStart(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->actorReanimateStart;
		regs.Register(a_form);
	}

	inline void RegisterForActorReanimateStop(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->actorReanimateStop;
		regs.Register(a_form);
	}

	inline void RegisterForActorResurrected(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->actorResurrect;
		regs.Register(a_form);
	}

	inline void RegisterForBookRead(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->booksRead;
		regs.Register(a_form);
	}

	inline void RegisterForCellFullyLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::ScriptEventHolder::GetSingleton()->cellFullyLoaded;
		regs.Register(a_form);
	}

	inline void RegisterForCriticalHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->criticalHit;
		regs.Register(a_form);
	}

	inline void RegisterForDisarmed(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->disarmed;
		regs.Register(a_form);
	}

	inline void RegisterForDragonSoulGained(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->dragonSoulsGained;
		regs.Register(a_form);
	}

	inline void RegisterForOnPlayerFastTravelEnd(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}
#ifdef SKYRIMVR
		auto& regs = Event::GameEventHolder::GetSingleton()->fastTravelEnd;
		regs.Register(a_form);
#else  // dummy function to avoid papyrus error Unbound native function
#endif
	}

	inline void RegisterForFastTravelPrompt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->fastTravelPrompt;
		regs.Register(a_form);
	}

	inline void RegisterForHitEventEx(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form,
		RE::TESForm* a_aggressorFilter,
		RE::TESForm* a_sourceFilter,
		RE::TESForm* a_projectileFilter,
		std::int32_t a_powerFilter,
		std::int32_t a_sneakFilter,
		std::int32_t a_bashFilter,
		std::int32_t a_blockFilter,
		bool a_match)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->onHit;
		regs.Register(a_form, { a_aggressorFilter, a_sourceFilter, a_projectileFilter, a_powerFilter, a_sneakFilter, a_bashFilter, a_blockFilter }, a_match);
	}

	inline void RegisterForItemCrafted(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->itemCrafted;
		regs.Register(a_form);
	}

	inline void RegisterForItemHarvested(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->itemHarvested;
		regs.Register(a_form);
	}

	inline void RegisterForLevelIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->levelIncrease;
		regs.Register(a_form);
	}

	inline void RegisterForLocationDiscovery(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->locationDiscovery;
		regs.Register(a_form);
	}

	inline void RegisterForMagicEffectApplyEx(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESForm* a_form,
		RE::TESForm* a_effectFilter,
		bool a_match)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->magicApply;
		regs.Register(a_form, a_effectFilter, a_match);
	}

	inline void RegisterForMagicHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->magicHit;
		regs.Register(a_form);
	}

	inline void RegisterForObjectGrab(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& grab = Event::ScriptEventHolder::GetSingleton()->objectGrab;
		grab.Register(a_form);

		auto& release = Event::ScriptEventHolder::GetSingleton()->objectRelease;
		release.Register(a_form);
	}

	inline void RegisterForObjectLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::TESForm* a_form,
		std::uint32_t a_formType)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		const auto formType = static_cast<RE::FormType>(a_formType);

		auto& load = Event::ScriptEventHolder::GetSingleton()->objectLoaded;
		load.Register(a_form, formType);

		auto& unload = Event::ScriptEventHolder::GetSingleton()->objectUnloaded;
		unload.Register(a_form, formType);
	}

	inline void RegisterForProjectileHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->projectileHit;
		regs.Register(a_form);
	}

	inline void RegisterForQuest(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form, RE::TESQuest* a_quest)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}
		if (!a_quest) {
			a_vm->TraceStack("Quest is None", a_stackID);
			return;
		}

		auto& start = Event::ScriptEventHolder::GetSingleton()->questStart;
		start.Register(a_form, a_quest->GetFormID());

		auto& stop = Event::ScriptEventHolder::GetSingleton()->questStop;
		stop.Register(a_form, a_quest->GetFormID());
	}

	inline void RegisterForQuestStage(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::TESForm* a_form,
		const RE::TESQuest* a_quest)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}
		if (!a_quest) {
			a_vm->TraceStack("Quest is None", a_stackID);
			return;
		}

		auto& regs = Event::ScriptEventHolder::GetSingleton()->questStage;
		regs.Register(a_form, a_quest->GetFormID());
	}

	inline void RegisterForShoutAttack(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->shoutAttack;
		regs.Register(a_form);
	}

	inline void RegisterForSkillIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->skillIncrease;
		regs.Register(a_form);
	}

	inline void RegisterForSoulTrapped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->soulsTrapped;
		regs.Register(a_form);
	}

	inline void RegisterForSpellLearned(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->spellsLearned;
		regs.Register(a_form);
	}

	inline void RegisterForWeatherChange(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->weatherChange;
		regs.Register(a_form);
	}

	inline void RegisterForWeaponHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->weaponHit;
		regs.Register(a_form);
	}

	inline void UnregisterForActorFallLongDistance(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->actorFallLongDistance;
		regs.Unregister(a_form);
	}

	inline void UnregisterForActorKilled(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->actorKill;
		regs.Unregister(a_form);
	}

	inline void UnregisterForActorReanimateStart(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->actorReanimateStart;
		regs.Unregister(a_form);
	}

	inline void UnregisterForActorReanimateStop(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->actorReanimateStop;
		regs.Unregister(a_form);
	}

	inline void UnregisterForActorResurrected(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->actorResurrect;
		regs.Unregister(a_form);
	}

	inline void UnregisterForBookRead(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->booksRead;
		regs.Unregister(a_form);
	}

	inline void UnregisterForCellFullyLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::ScriptEventHolder::GetSingleton()->cellFullyLoaded;
		regs.Unregister(a_form);
	}

	inline void UnregisterForCriticalHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->criticalHit;
		regs.Unregister(a_form);
	}

	inline void UnregisterForDisarmed(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->disarmed;
		regs.Unregister(a_form);
	}

	inline void UnregisterForDragonSoulGained(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->dragonSoulsGained;
		regs.Unregister(a_form);
	}

	inline void UnregisterForOnPlayerFastTravelEnd(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}
#ifdef SKYRIMVR
		auto& regs = Event::GameEventHolder::GetSingleton()->fastTravelEnd;
		regs.Unregister(a_form);
#else  // dummy function to avoid papyrus error Unbound native function
#endif
	}

	inline void UnregisterForFastTravelPrompt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->fastTravelPrompt;
		regs.Unregister(a_form);
	}

	inline void UnregisterForHitEventEx(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESForm* a_form,
		RE::TESForm* a_aggressorFilter,
		RE::TESForm* a_sourceFilter,
		RE::TESForm* a_projectileFilter,
		std::int32_t a_powerFilter,
		std::int32_t a_sneakFilter,
		std::int32_t a_bashFilter,
		std::int32_t a_blockFilter,
		bool a_match)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->onHit;
		regs.Unregister(a_form, { a_aggressorFilter, a_sourceFilter, a_projectileFilter, a_powerFilter, a_sneakFilter, a_bashFilter, a_blockFilter }, a_match);
	}

	inline void UnregisterForAllHitEventsEx(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->onHit;
		regs.UnregisterAll(a_form);
	}

	inline void UnregisterForItemCrafted(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->itemCrafted;
		regs.Unregister(a_form);
	}

	inline void UnregisterForItemHarvested(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->itemHarvested;
		regs.Unregister(a_form);
	}

	inline void UnregisterForLevelIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->levelIncrease;
		regs.Unregister(a_form);
	}

	inline void UnregisterForLocationDiscovery(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->locationDiscovery;
		regs.Unregister(a_form);
	}

	inline void UnregisterForMagicEffectApplyEx(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESForm* a_form,
		RE::TESForm* a_effectFilter,
		bool a_match)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->magicApply;
		regs.Unregister(a_form, a_effectFilter, a_match);
	}

	inline void UnregisterForAllMagicEffectApplyEx(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->magicApply;
		regs.UnregisterAll(a_form);
	}

	inline void UnregisterForMagicHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->magicHit;
		regs.Unregister(a_form);
	}

	inline void UnregisterForObjectGrab(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& grab = Event::ScriptEventHolder::GetSingleton()->objectGrab;
		grab.Unregister(a_form);

		auto& release = Event::ScriptEventHolder::GetSingleton()->objectRelease;
		release.Unregister(a_form);
	}

	inline void UnregisterForObjectLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::TESForm* a_form,
		std::uint32_t a_formType)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto formType = static_cast<RE::FormType>(a_formType);

		auto& load = Event::ScriptEventHolder::GetSingleton()->objectLoaded;
		load.Unregister(a_form, formType);

		auto& unload = Event::ScriptEventHolder::GetSingleton()->objectUnloaded;
		unload.Unregister(a_form, formType);
	}

	inline void UnregisterForAllObjectsLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& load = Event::ScriptEventHolder::GetSingleton()->objectLoaded;
		load.UnregisterAll(a_form);

		auto& unload = Event::ScriptEventHolder::GetSingleton()->objectUnloaded;
		unload.UnregisterAll(a_form);
	}

	inline void UnregisterForProjectileHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->projectileHit;
		regs.Unregister(a_form);
	}

	inline void UnregisterForQuest(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::TESForm* a_form,
		const RE::TESQuest* a_quest)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}
		if (!a_quest) {
			a_vm->TraceStack("Quest is None", a_stackID);
			return;
		}

		auto& start = Event::ScriptEventHolder::GetSingleton()->questStart;
		start.Unregister(a_form, a_quest->GetFormID());

		auto& stop = Event::ScriptEventHolder::GetSingleton()->questStop;
		stop.Unregister(a_form, a_quest->GetFormID());
	}

	inline void UnregisterForAllQuests(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& start = Event::ScriptEventHolder::GetSingleton()->questStart;
		start.UnregisterAll(a_form);

		auto& stop = Event::ScriptEventHolder::GetSingleton()->questStop;
		stop.UnregisterAll(a_form);
	}

	inline void UnregisterForQuestStage(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::TESForm* a_form,
		const RE::TESQuest* a_quest)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}
		if (!a_quest) {
			a_vm->TraceStack("Quest is None", a_stackID);
			return;
		}

		auto& regs = Event::ScriptEventHolder::GetSingleton()->questStage;
		regs.Unregister(a_form, a_quest->GetFormID());
	}

	inline void UnregisterForAllQuestStages(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::ScriptEventHolder::GetSingleton()->questStage;
		regs.UnregisterAll(a_form);
	}

	inline void UnregisterForShoutAttack(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->shoutAttack;
		regs.Unregister(a_form);
	}

	inline void UnregisterForSkillIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->skillIncrease;
		regs.Unregister(a_form);
	}

	inline void UnregisterForSoulTrapped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->soulsTrapped;
		regs.Unregister(a_form);
	}

	inline void UnregisterForSpellLearned(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->spellsLearned;
		regs.Unregister(a_form);
	}

	inline void UnregisterForWeatherChange(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->weatherChange;
		regs.Unregister(a_form);
	}

	inline void UnregisterForWeaponHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->weaponHit;
		regs.Unregister(a_form);
	}

	inline void Bind(VM& a_vm)
	{
		auto constexpr obj = "PO3_Events_Form"sv;

		BIND_EVENT(RegisterForActorFallLongDistance, true);
		BIND_EVENT(RegisterForActorKilled, true);
		BIND_EVENT(RegisterForActorReanimateStart, true);
		BIND_EVENT(RegisterForActorReanimateStop, true);
		BIND_EVENT(RegisterForActorResurrected, true);
		BIND_EVENT(RegisterForBookRead, true);
		BIND_EVENT(RegisterForCellFullyLoaded, true);
		BIND_EVENT(RegisterForCriticalHit, true);
		BIND_EVENT(RegisterForDisarmed, true);
		BIND_EVENT(RegisterForDragonSoulGained, true);
		BIND_EVENT(RegisterForOnPlayerFastTravelEnd, true);
		BIND_EVENT(RegisterForFastTravelPrompt, true);
		BIND_EVENT(RegisterForHitEventEx, true);
		BIND_EVENT(RegisterForItemCrafted, true);
		BIND_EVENT(RegisterForItemHarvested, true);
		BIND_EVENT(RegisterForLevelIncrease, true);
		BIND_EVENT(RegisterForLocationDiscovery, true);
		BIND_EVENT(RegisterForMagicEffectApplyEx, true);
		BIND_EVENT(RegisterForMagicHit, true);
		BIND_EVENT(RegisterForObjectGrab, true);
		BIND_EVENT(RegisterForObjectLoaded, true);
		BIND_EVENT(RegisterForQuest, true);
		BIND_EVENT(RegisterForProjectileHit, true);
		BIND_EVENT(RegisterForQuestStage, true);
		BIND_EVENT(RegisterForShoutAttack, true);
		BIND_EVENT(RegisterForSkillIncrease, true);
		BIND_EVENT(RegisterForSoulTrapped, true);
		BIND_EVENT(RegisterForSpellLearned, true);
		BIND_EVENT(RegisterForWeatherChange, true);
		BIND_EVENT(RegisterForWeaponHit, true);

		BIND_EVENT(UnregisterForActorFallLongDistance, true);
		BIND_EVENT(UnregisterForActorKilled, true);
		BIND_EVENT(UnregisterForActorReanimateStart, true);
		BIND_EVENT(UnregisterForActorReanimateStop, true);
		BIND_EVENT(UnregisterForActorResurrected, true);
		BIND_EVENT(UnregisterForBookRead, true);
		BIND_EVENT(UnregisterForCellFullyLoaded, true);
		BIND_EVENT(UnregisterForCriticalHit, true);
		BIND_EVENT(UnregisterForDisarmed, true);
		BIND_EVENT(UnregisterForDragonSoulGained, true);
		BIND_EVENT(UnregisterForOnPlayerFastTravelEnd, true);
		BIND_EVENT(UnregisterForFastTravelPrompt, true);
		BIND_EVENT(UnregisterForHitEventEx, true);
		BIND_EVENT(UnregisterForAllHitEventsEx, true);
		BIND_EVENT(UnregisterForItemCrafted, true);
		BIND_EVENT(UnregisterForItemHarvested, true);
		BIND_EVENT(UnregisterForLevelIncrease, true);
		BIND_EVENT(UnregisterForLocationDiscovery, true);
		BIND_EVENT(UnregisterForMagicEffectApplyEx, true);
		BIND_EVENT(UnregisterForAllMagicEffectApplyEx, true);
		BIND_EVENT(UnregisterForMagicHit, true);
		BIND_EVENT(UnregisterForObjectGrab, true);
		BIND_EVENT(UnregisterForObjectLoaded, true);
		BIND_EVENT(UnregisterForAllObjectsLoaded, true);
		BIND_EVENT(UnregisterForProjectileHit, true);
		BIND_EVENT(UnregisterForQuest, true);
		BIND_EVENT(UnregisterForAllQuests, true);
		BIND_EVENT(UnregisterForQuestStage, true);
		BIND_EVENT(UnregisterForAllQuestStages, true);
		BIND_EVENT(UnregisterForShoutAttack, true);
		BIND_EVENT(UnregisterForSkillIncrease, true);
		BIND_EVENT(UnregisterForSoulTrapped, true);
		BIND_EVENT(UnregisterForSpellLearned, true);
		BIND_EVENT(UnregisterForWeatherChange, true);
		BIND_EVENT(UnregisterForWeaponHit, true);

		logger::info("Registered form events"sv);
	}
}
