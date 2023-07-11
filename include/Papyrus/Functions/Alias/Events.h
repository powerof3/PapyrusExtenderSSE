#pragma once

#include "Serialization/EventHolder.h"

namespace Papyrus::Alias::Events
{
	inline void RegisterForActorFallLongDistance(STATIC_ARGS, RE::BGSRefAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->actorFallLongDistance;
		regs.Register(a_alias);
	}

	inline void RegisterForActorKilled(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->actorKill;
		regs.Register(a_alias);
	}

	inline void RegisterForActorReanimateStart(STATIC_ARGS, RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto refAlias = skyrim_cast<RE::BGSRefAlias*>(a_alias);
		if (!refAlias) {
			a_vm->TraceStack("Alias is not a Reference Alias", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->actorReanimateStart;
		regs.Register(refAlias);
	}

	inline void RegisterForActorReanimateStop(STATIC_ARGS, RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto refAlias = skyrim_cast<RE::BGSRefAlias*>(a_alias);
		if (!refAlias) {
			a_vm->TraceStack("Alias is not a Reference Alias", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->actorReanimateStop;
		regs.Register(refAlias);
	}

	inline void RegisterForActorResurrected(STATIC_ARGS, RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto refAlias = skyrim_cast<RE::BGSRefAlias*>(a_alias);
		if (!refAlias) {
			a_vm->TraceStack("Alias is not a Reference Alias", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->actorResurrect;
		regs.Register(refAlias);
	}

	inline void RegisterForBookRead(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->booksRead;
		regs.Register(a_alias);
	}

	inline void RegisterForCellFullyLoaded(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::ScriptEventHolder::GetSingleton()->cellFullyLoaded;
		regs.Register(a_alias);
	}

	inline void RegisterForCriticalHit(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->criticalHit;
		regs.Register(a_alias);
	}

	inline void RegisterForDisarmed(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->disarmed;
		regs.Register(a_alias);
	}

	inline void RegisterForDragonSoulGained(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->dragonSoulsGained;
		regs.Register(a_alias);
	}

	inline void RegisterForOnPlayerFastTravelEnd(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}
#ifdef SKYRIMVR
		auto& regs = Event::GameEventHolder::GetSingleton()->fastTravelEnd;
		regs.Register(a_alias);
#else  // dummy function to avoid papyrus error Unbound native function
#endif
	}

	inline void RegisterForFastTravelConfirmed(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->fastTravelConfirmed;
		regs.Register(a_alias);
	}

	inline void RegisterForFastTravelPrompt(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->fastTravelPrompt;
		regs.Register(a_alias);
	}

	inline void RegisterForHitEventEx(STATIC_ARGS, RE::BGSRefAlias* a_alias,
		RE::TESForm* a_aggressorFilter,
		RE::TESForm* a_sourceFilter,
		RE::TESForm* a_projectileFilter,
		std::int32_t a_powerFilter,
		std::int32_t a_sneakFilter,
		std::int32_t a_bashFilter,
		std::int32_t a_blockFilter,
		bool         a_match)
	{
		if (!a_alias) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->onHit;
		regs.Register(a_alias, { a_aggressorFilter, a_sourceFilter, a_projectileFilter, a_powerFilter, a_sneakFilter, a_bashFilter, a_blockFilter }, a_match);
	}

	inline void RegisterForItemCrafted(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->itemCrafted;
		regs.Register(a_alias);
	}

	inline void RegisterForItemHarvested(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->itemHarvested;
		regs.Register(a_alias);
	}

	inline void RegisterForLevelIncrease(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->levelIncrease;
		regs.Register(a_alias);
	}

	inline void RegisterForLocationDiscovery(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->locationDiscovery;
		regs.Register(a_alias);
	}

	inline void RegisterForMagicEffectApplyEx(STATIC_ARGS,
		RE::BGSRefAlias* a_alias,
		RE::TESForm*     a_effectFilter,
		bool             a_match)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->magicApply;
		regs.Register(a_alias, a_effectFilter, a_match);
	}

	inline void RegisterForMagicHit(STATIC_ARGS, RE::BGSRefAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->magicHit;
		regs.Register(a_alias);
	}

	inline void RegisterForObjectGrab(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& grab = Event::ScriptEventHolder::GetSingleton()->objectGrab;
		grab.Register(a_alias);

		auto& release = Event::ScriptEventHolder::GetSingleton()->objectRelease;
		release.Register(a_alias);
	}

	inline void RegisterForObjectLoaded(STATIC_ARGS,
		const RE::BGSBaseAlias* a_alias,
		std::uint32_t           a_formType)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto formType = static_cast<RE::FormType>(a_formType);

		auto& load = Event::ScriptEventHolder::GetSingleton()->objectLoaded;
		load.Register(a_alias, formType);

		auto& unload = Event::ScriptEventHolder::GetSingleton()->objectUnloaded;
		unload.Register(a_alias, formType);
	}

	inline void RegisterForProjectileHit(STATIC_ARGS, RE::BGSRefAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->projectileHit;
		regs.Register(a_alias);
	}

	inline void RegisterForQuest(STATIC_ARGS, const RE::BGSBaseAlias* a_alias, RE::TESQuest* a_quest)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}
		if (!a_quest) {
			a_vm->TraceStack("Quest is None", a_stackID);
			return;
		}

		auto& start = Event::ScriptEventHolder::GetSingleton()->questStart;
		start.Register(a_alias, a_quest->GetFormID());

		auto& stop = Event::ScriptEventHolder::GetSingleton()->questStop;
		stop.Register(a_alias, a_quest->GetFormID());
	}

	inline void RegisterForQuestStage(STATIC_ARGS,
		const RE::BGSBaseAlias* a_alias,
		const RE::TESQuest*     a_quest)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}
		if (!a_quest) {
			a_vm->TraceStack("Quest is None", a_stackID);
			return;
		}

		auto& regs = Event::ScriptEventHolder::GetSingleton()->questStage;
		regs.Register(a_alias, a_quest->GetFormID());
	}

	inline void RegisterForShoutAttack(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->shoutAttack;
		regs.Register(a_alias);
	}

	inline void RegisterForSkillIncrease(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->skillIncrease;
		regs.Register(a_alias);
	}

	inline void RegisterForSoulTrapped(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->soulsTrapped;
		regs.Register(a_alias);
	}

	inline void RegisterForSpellLearned(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->spellsLearned;
		regs.Register(a_alias);
	}

	inline void RegisterForWeatherChange(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->weatherChange;
		regs.Register(a_alias);
	}

	inline void RegisterForWeaponHit(STATIC_ARGS, RE::BGSRefAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->weaponHit;
		regs.Register(a_alias);
	}

	inline void UnregisterForActorFallLongDistance(STATIC_ARGS, RE::BGSRefAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->actorFallLongDistance;
		regs.Unregister(a_alias);
	}

	inline void UnregisterForActorKilled(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->actorKill;
		regs.Unregister(a_alias);
	}

	inline void UnregisterForActorReanimateStart(STATIC_ARGS, RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto refAlias = skyrim_cast<RE::BGSRefAlias*>(a_alias);
		if (!refAlias) {
			a_vm->TraceStack("Alias is not a Reference Alias", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->actorReanimateStart;
		regs.Unregister(refAlias);
	}

	inline void UnregisterForActorReanimateStop(STATIC_ARGS, RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto refAlias = skyrim_cast<RE::BGSRefAlias*>(a_alias);
		if (!refAlias) {
			a_vm->TraceStack("Alias is not a Reference Alias", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->actorReanimateStop;
		regs.Unregister(refAlias);
	}

	inline void UnregisterForActorResurrected(STATIC_ARGS, RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto refAlias = skyrim_cast<RE::BGSRefAlias*>(a_alias);
		if (!refAlias) {
			a_vm->TraceStack("Alias is not a Reference Alias", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->actorResurrect;
		regs.Unregister(refAlias);
	}

	inline void UnregisterForBookRead(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->booksRead;
		regs.Unregister(a_alias);
	}

	inline void UnregisterForCellFullyLoaded(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::ScriptEventHolder::GetSingleton()->cellFullyLoaded;
		regs.Unregister(a_alias);
	}

	inline void UnregisterForCriticalHit(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->criticalHit;
		regs.Unregister(a_alias);
	}

	inline void UnregisterForDisarmed(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->disarmed;
		regs.Unregister(a_alias);
	}

	inline void UnregisterForDragonSoulGained(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->dragonSoulsGained;
		regs.Unregister(a_alias);
	}

	inline void UnregisterForOnPlayerFastTravelEnd(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}
#ifdef SKYRIMVR
		auto& regs = Event::GameEventHolder::GetSingleton()->fastTravelEnd;
		regs.Unregister(a_alias);
#else  // dummy function to avoid papyrus error Unbound native function
#endif
	}

	inline void UnregisterForFastTravelConfirmed(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->fastTravelConfirmed;
		regs.Unregister(a_alias);
	}

	inline void UnregisterForFastTravelPrompt(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->fastTravelPrompt;
		regs.Unregister(a_alias);
	}

	inline void UnregisterForHitEventEx(STATIC_ARGS, RE::BGSRefAlias* a_alias,
		RE::TESForm* a_aggressorFilter,
		RE::TESForm* a_sourceFilter,
		RE::TESForm* a_projectileFilter,
		std::int32_t a_powerFilter,
		std::int32_t a_sneakFilter,
		std::int32_t a_bashFilter,
		std::int32_t a_blockFilter,
		bool         a_match)
	{
		if (!a_alias) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->onHit;
		regs.Unregister(a_alias, { a_aggressorFilter, a_sourceFilter, a_projectileFilter, a_powerFilter, a_sneakFilter, a_bashFilter, a_blockFilter }, a_match);
	}

	inline void UnregisterForAllHitEventsEx(STATIC_ARGS, RE::BGSRefAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->onHit;
		regs.UnregisterAll(a_alias);
	}

	inline void UnregisterForItemCrafted(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->itemCrafted;
		regs.Unregister(a_alias);
	}

	inline void UnregisterForItemHarvested(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->itemHarvested;
		regs.Unregister(a_alias);
	}

	inline void UnregisterForLevelIncrease(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->levelIncrease;
		regs.Unregister(a_alias);
	}

	inline void UnregisterForLocationDiscovery(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->locationDiscovery;
		regs.Unregister(a_alias);
	}

	inline void UnregisterForMagicEffectApplyEx(STATIC_ARGS,
		RE::BGSRefAlias* a_alias,
		RE::TESForm*     a_effectFilter,
		bool             a_match)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->magicApply;
		regs.Unregister(a_alias, a_effectFilter, a_match);
	}

	inline void UnregisterForAllMagicEffectApplyEx(STATIC_ARGS, RE::BGSRefAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->magicApply;
		regs.UnregisterAll(a_alias);
	}

	inline void UnregisterForMagicHit(STATIC_ARGS, RE::BGSRefAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->magicHit;
		regs.Unregister(a_alias);
	}

	inline void UnregisterForObjectGrab(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& grab = Event::ScriptEventHolder::GetSingleton()->objectGrab;
		grab.Unregister(a_alias);

		auto& release = Event::ScriptEventHolder::GetSingleton()->objectRelease;
		release.Unregister(a_alias);
	}

	inline void UnregisterForObjectLoaded(STATIC_ARGS,
		const RE::BGSBaseAlias* a_alias,
		std::uint32_t           a_formType)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto formType = static_cast<RE::FormType>(a_formType);

		auto& load = Event::ScriptEventHolder::GetSingleton()->objectLoaded;
		load.Unregister(a_alias, formType);

		auto& unload = Event::ScriptEventHolder::GetSingleton()->objectUnloaded;
		unload.Unregister(a_alias, formType);
	}

	inline void UnregisterForAllObjectsLoaded(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& load = Event::ScriptEventHolder::GetSingleton()->objectLoaded;
		load.UnregisterAll(a_alias);

		auto& unload = Event::ScriptEventHolder::GetSingleton()->objectUnloaded;
		unload.UnregisterAll(a_alias);
	}

	inline void UnregisterForProjectileHit(STATIC_ARGS, RE::BGSRefAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->projectileHit;
		regs.Unregister(a_alias);
	}

	inline void UnregisterForQuest(STATIC_ARGS,
		const RE::BGSBaseAlias* a_alias,
		const RE::TESQuest*     a_quest)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}
		if (!a_quest) {
			a_vm->TraceStack("Quest is None", a_stackID);
			return;
		}

		auto& start = Event::ScriptEventHolder::GetSingleton()->questStart;
		start.Unregister(a_alias, a_quest->GetFormID());

		auto& stop = Event::ScriptEventHolder::GetSingleton()->questStop;
		stop.Unregister(a_alias, a_quest->GetFormID());
	}

	inline void UnregisterForAllQuests(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& start = Event::ScriptEventHolder::GetSingleton()->questStart;
		start.UnregisterAll(a_alias);

		auto& stop = Event::ScriptEventHolder::GetSingleton()->questStop;
		stop.UnregisterAll(a_alias);
	}

	inline void UnregisterForQuestStage(STATIC_ARGS,
		const RE::BGSBaseAlias* a_alias,
		const RE::TESQuest*     a_quest)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}
		if (!a_quest) {
			a_vm->TraceStack("Quest is None", a_stackID);
			return;
		}

		auto& regs = Event::ScriptEventHolder::GetSingleton()->questStage;
		regs.Unregister(a_alias, a_quest->GetFormID());
	}

	inline void UnregisterForAllQuestStages(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::ScriptEventHolder::GetSingleton()->questStage;
		regs.UnregisterAll(a_alias);
	}

	inline void UnregisterForShoutAttack(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->shoutAttack;
		regs.Unregister(a_alias);
	}

	inline void UnregisterForSkillIncrease(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->skillIncrease;
		regs.Unregister(a_alias);
	}

	inline void UnregisterForSoulTrapped(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->soulsTrapped;
		regs.Unregister(a_alias);
	}

	inline void UnregisterForSpellLearned(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->spellsLearned;
		regs.Unregister(a_alias);
	}

	inline void UnregisterForWeatherChange(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->weatherChange;
		regs.Unregister(a_alias);
	}

	inline void UnregisterForWeaponHit(STATIC_ARGS, RE::BGSRefAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->weaponHit;
		regs.Unregister(a_alias);
	}

	inline void Bind(VM& a_vm)
	{
		auto constexpr obj = "PO3_Events_Alias"sv;

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
		BIND_EVENT(RegisterForFastTravelConfirmed, true);
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
		BIND_EVENT(RegisterForProjectileHit, true);
		BIND_EVENT(RegisterForQuest, true);
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
		BIND_EVENT(UnregisterForFastTravelConfirmed, true);
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

		logger::info("Registered alias events"sv);
	}
}
