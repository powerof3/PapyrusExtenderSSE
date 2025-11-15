#include "Papyrus/Functions/ActiveMagicEffect/Events.h"

#include "Serialization/EventHolder.h"

namespace Papyrus::ActiveMagicEffect::Events
{
	void RegisterForActorFallLongDistance(STATIC_ARGS, RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->actorFallLongDistance;
		regs.Register(a_activeEffect);
	}

	void RegisterForActorKilled(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->actorKill;
		regs.Register(a_activeEffect);
	}

	void RegisterForActorReanimateStart(STATIC_ARGS, RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->actorReanimateStart;
		regs.Register(a_activeEffect);
	}

	void RegisterForActorReanimateStop(STATIC_ARGS, RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->actorReanimateStop;
		regs.Register(a_activeEffect);
	}

	void RegisterForActorResurrected(STATIC_ARGS, RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->actorResurrect;
		regs.Register(a_activeEffect);
	}

	void RegisterForBookRead(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->booksRead;
		regs.Register(a_activeEffect);
	}

	void RegisterForCellFullyLoaded(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::ScriptEventHolder::GetSingleton()->cellFullyLoaded;
		regs.Register(a_activeEffect);
	}

	void RegisterForCriticalHit(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->criticalHit;
		regs.Register(a_activeEffect);
	}

	void RegisterForDisarmed(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->disarmed;
		regs.Register(a_activeEffect);
	}

	void RegisterForDragonSoulGained(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->dragonSoulsGained;
		regs.Register(a_activeEffect);
	}

	void RegisterForOnPlayerFastTravelEnd(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}
#ifdef SKYRIMVR
		auto& regs = Event::GameEventHolder::GetSingleton()->fastTravelEnd;
		regs.Register(a_activeEffect);
#else  // dummy function to avoid papyrus error Unbound native function
#endif
	}

	void RegisterForFastTravelConfirmed(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->fastTravelConfirmed;
		regs.Register(a_activeEffect);
	}

	void RegisterForFastTravelPrompt(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->fastTravelPrompt;
		regs.Register(a_activeEffect);
	}

	void RegisterForFurnitureEvent(STATIC_ARGS, RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& furnitureEnter = Event::ScriptEventHolder::GetSingleton()->furnitureEnter;
		furnitureEnter.Register(a_activeEffect);

		auto& furnitureExit = Event::ScriptEventHolder::GetSingleton()->furnitureExit;
		furnitureExit.Register(a_activeEffect);
	}

	void RegisterForHitEventEx(STATIC_ARGS, RE::ActiveEffect* a_activeEffect,
		RE::TESForm* a_aggressorFilter,
		RE::TESForm* a_sourceFilter,
		RE::TESForm* a_projectileFilter,
		std::int32_t a_powerFilter,
		std::int32_t a_sneakFilter,
		std::int32_t a_bashFilter,
		std::int32_t a_blockFilter,
		bool         a_match)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->onHit;
		regs.Register(a_activeEffect, { a_aggressorFilter, a_sourceFilter, a_projectileFilter, a_powerFilter, a_sneakFilter, a_bashFilter, a_blockFilter }, a_match);
	}

	void RegisterForItemCrafted(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->itemCrafted;
		regs.Register(a_activeEffect);
	}

	void RegisterForItemHarvested(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->itemHarvested;
		regs.Register(a_activeEffect);
	}

	void RegisterForLevelIncrease(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->levelIncrease;
		regs.Register(a_activeEffect);
	}

	void RegisterForLocationDiscovery(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->locationDiscovery;
		regs.Register(a_activeEffect);
	}

	void RegisterForMagicEffectApplyEx(STATIC_ARGS, RE::ActiveEffect* a_activeEffect, RE::TESForm* a_effectFilter, bool a_match)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->magicApply;
		regs.Register(a_activeEffect, a_effectFilter, a_match);
	}

	void RegisterForMagicHit(STATIC_ARGS, RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->magicHit;
		regs.Register(a_activeEffect);
	}

	void RegisterForObjectGrab(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& grab = Event::ScriptEventHolder::GetSingleton()->objectGrab;
		grab.Register(a_activeEffect);

		auto& release = Event::ScriptEventHolder::GetSingleton()->objectRelease;
		release.Register(a_activeEffect);
	}

	void RegisterForObjectLoaded(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect, std::uint32_t a_formType)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		const auto formType = static_cast<RE::FormType>(a_formType);

		auto& load = Event::ScriptEventHolder::GetSingleton()->objectLoaded;
		load.Register(a_activeEffect, formType);

		auto& unload = Event::ScriptEventHolder::GetSingleton()->objectUnloaded;
		unload.Register(a_activeEffect, formType);
	}

	void RegisterForObjectPoisoned(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->objectPoisoned;
		regs.Register(a_activeEffect);
	}

	void RegisterForProjectileHit(STATIC_ARGS, RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->projectileHit;
		regs.Register(a_activeEffect);
	}

	void RegisterForQuest(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect, const RE::TESQuest* a_quest)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}
		if (!a_quest) {
			a_vm->TraceStack("Quest is None", a_stackID);
			return;
		}

		auto& start = Event::ScriptEventHolder::GetSingleton()->questStart;
		start.Register(a_activeEffect, a_quest->GetFormID());

		auto& stop = Event::ScriptEventHolder::GetSingleton()->questStop;
		stop.Register(a_activeEffect, a_quest->GetFormID());
	}

	void RegisterForQuestStage(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect, const RE::TESQuest* a_quest)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}
		if (!a_quest) {
			a_vm->TraceStack("Quest is None", a_stackID);
			return;
		}

		auto& regs = Event::ScriptEventHolder::GetSingleton()->questStage;
		regs.Register(a_activeEffect, a_quest->GetFormID());
	}

	void RegisterForShoutAttack(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->shoutAttack;
		regs.Register(a_activeEffect);
	}

	void RegisterForSkillIncrease(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->skillIncrease;
		regs.Register(a_activeEffect);
	}

	void RegisterForSoulTrapped(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->soulsTrapped;
		regs.Register(a_activeEffect);
	}

	void RegisterForSpellLearned(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->spellsLearned;
		regs.Register(a_activeEffect);
	}

	void RegisterForWeatherChange(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->weatherChange;
		regs.Register(a_activeEffect);
	}

	void RegisterForWeaponHit(STATIC_ARGS, RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->weaponHit;
		regs.Register(a_activeEffect);
	}

	void UnregisterForActorFallLongDistance(STATIC_ARGS, RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->actorFallLongDistance;
		regs.Unregister(a_activeEffect);
	}

	void UnregisterForActorKilled(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->actorKill;
		regs.Unregister(a_activeEffect);
	}

	void UnregisterForActorReanimateStart(STATIC_ARGS, RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->actorReanimateStart;
		regs.Unregister(a_activeEffect);
	}

	void UnregisterForActorReanimateStop(STATIC_ARGS, RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->actorReanimateStop;
		regs.Unregister(a_activeEffect);
	}

	void UnregisterForActorResurrected(STATIC_ARGS, RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->actorResurrect;
		regs.Unregister(a_activeEffect);
	}

	void UnregisterForBookRead(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->booksRead;
		regs.Unregister(a_activeEffect);
	}

	void UnregisterForCellFullyLoaded(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::ScriptEventHolder::GetSingleton()->cellFullyLoaded;
		regs.Unregister(a_activeEffect);
	}

	void UnregisterForCriticalHit(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->criticalHit;
		regs.Unregister(a_activeEffect);
	}

	void UnregisterForDisarmed(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->disarmed;
		regs.Unregister(a_activeEffect);
	}

	void UnregisterForDragonSoulGained(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->dragonSoulsGained;
		regs.Unregister(a_activeEffect);
	}

	void UnregisterForOnPlayerFastTravelEnd(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}
#ifdef SKYRIMVR
		auto& regs = Event::GameEventHolder::GetSingleton()->fastTravelEnd;
		regs.Unregister(a_activeEffect);
#else
		// dummy function to avoid papyrus error Unbound native function
#endif
	}

	void UnregisterForFastTravelConfirmed(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->fastTravelConfirmed;
		regs.Unregister(a_activeEffect);
	}

	void UnregisterForFastTravelPrompt(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->fastTravelPrompt;
		regs.Unregister(a_activeEffect);
	}

	void UnregisterForFurnitureEvent(STATIC_ARGS, RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& furnitureEnter = Event::ScriptEventHolder::GetSingleton()->furnitureEnter;
		furnitureEnter.Unregister(a_activeEffect);

		auto& furnitureExit = Event::ScriptEventHolder::GetSingleton()->furnitureExit;
		furnitureExit.Unregister(a_activeEffect);
	}

	void UnregisterForHitEventEx(STATIC_ARGS, RE::ActiveEffect* a_activeEffect,
		RE::TESForm* a_aggressorFilter,
		RE::TESForm* a_sourceFilter,
		RE::TESForm* a_projectileFilter,
		std::int32_t a_powerFilter,
		std::int32_t a_sneakFilter,
		std::int32_t a_bashFilter,
		std::int32_t a_blockFilter,
		bool         a_match)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->onHit;
		regs.Unregister(a_activeEffect, { a_aggressorFilter, a_sourceFilter, a_projectileFilter, a_powerFilter, a_sneakFilter, a_bashFilter, a_blockFilter }, a_match);
	}

	void UnregisterForAllHitEventsEx(STATIC_ARGS, RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->onHit;
		regs.UnregisterAll(a_activeEffect);
	}

	void UnregisterForItemCrafted(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->itemCrafted;
		regs.Unregister(a_activeEffect);
	}

	void UnregisterForItemHarvested(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->itemHarvested;
		regs.Unregister(a_activeEffect);
	}

	void UnregisterForLevelIncrease(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->levelIncrease;
		regs.Unregister(a_activeEffect);
	}

	void UnregisterForLocationDiscovery(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->locationDiscovery;
		regs.Unregister(a_activeEffect);
	}

	void UnregisterForMagicEffectApplyEx(STATIC_ARGS, RE::ActiveEffect* a_activeEffect, RE::TESForm* a_effectFilter, bool a_match)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->magicApply;
		regs.Unregister(a_activeEffect, a_effectFilter, a_match);
	}

	void UnregisterForAllMagicEffectApplyEx(STATIC_ARGS, RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->magicApply;
		regs.UnregisterAll(a_activeEffect);
	}

	void UnregisterForMagicHit(STATIC_ARGS, RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->magicHit;
		regs.Unregister(a_activeEffect);
	}

	void UnregisterForObjectGrab(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& grab = Event::ScriptEventHolder::GetSingleton()->objectGrab;
		grab.Unregister(a_activeEffect);

		auto& release = Event::ScriptEventHolder::GetSingleton()->objectRelease;
		release.Unregister(a_activeEffect);
	}

	void UnregisterForObjectLoaded(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect, std::uint32_t a_formType)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto formType = static_cast<RE::FormType>(a_formType);

		auto& load = Event::ScriptEventHolder::GetSingleton()->objectLoaded;
		load.Unregister(a_activeEffect, formType);

		auto& unload = Event::ScriptEventHolder::GetSingleton()->objectUnloaded;
		unload.Unregister(a_activeEffect, formType);
	}

	void UnregisterForObjectPoisoned(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->objectPoisoned;
		regs.Unregister(a_activeEffect);
	}

	void UnregisterForAllObjectsLoaded(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& load = Event::ScriptEventHolder::GetSingleton()->objectLoaded;
		load.UnregisterAll(a_activeEffect);

		auto& unload = Event::ScriptEventHolder::GetSingleton()->objectUnloaded;
		unload.UnregisterAll(a_activeEffect);
	}

	void UnregisterForProjectileHit(STATIC_ARGS, RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->projectileHit;
		regs.Unregister(a_activeEffect);
	}

	void UnregisterForQuest(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect, const RE::TESQuest* a_quest)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}
		if (!a_quest) {
			a_vm->TraceStack("Quest is None", a_stackID);
			return;
		}

		auto& start = Event::ScriptEventHolder::GetSingleton()->questStart;
		start.Unregister(a_activeEffect, a_quest->GetFormID());

		auto& stop = Event::ScriptEventHolder::GetSingleton()->questStop;
		stop.Unregister(a_activeEffect, a_quest->GetFormID());
	}

	void UnregisterForAllQuests(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& start = Event::ScriptEventHolder::GetSingleton()->questStart;
		start.UnregisterAll(a_activeEffect);

		auto& stop = Event::ScriptEventHolder::GetSingleton()->questStop;
		stop.UnregisterAll(a_activeEffect);
	}

	void UnregisterForQuestStage(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect, const RE::TESQuest* a_quest)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}
		if (!a_quest) {
			a_vm->TraceStack("Quest is None", a_stackID);
			return;
		}

		auto& regs = Event::ScriptEventHolder::GetSingleton()->questStage;
		regs.Unregister(a_activeEffect, a_quest->GetFormID());
	}

	void UnregisterForAllQuestStages(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::ScriptEventHolder::GetSingleton()->questStage;
		regs.UnregisterAll(a_activeEffect);
	}

	void UnregisterForShoutAttack(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->shoutAttack;
		regs.Unregister(a_activeEffect);
	}

	void UnregisterForSkillIncrease(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->skillIncrease;
		regs.Unregister(a_activeEffect);
	}

	void UnregisterForSoulTrapped(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->soulsTrapped;
		regs.Unregister(a_activeEffect);
	}

	void UnregisterForSpellLearned(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::StoryEventHolder::GetSingleton()->spellsLearned;
		regs.Unregister(a_activeEffect);
	}

	void UnregisterForWeatherChange(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->weatherChange;
		regs.Unregister(a_activeEffect);
	}

	void UnregisterForWeaponHit(STATIC_ARGS, RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->weaponHit;
		regs.Unregister(a_activeEffect);
	}

	void Bind(VM& a_vm)
	{
		auto constexpr obj = "PO3_Events_AME"sv;

		std::uint32_t count = 0;

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
		BIND_EVENT(RegisterForFurnitureEvent, true);
		BIND_EVENT(RegisterForHitEventEx, true);
		BIND_EVENT(RegisterForItemCrafted, true);
		BIND_EVENT(RegisterForItemHarvested, true);
		BIND_EVENT(RegisterForLevelIncrease, true);
		BIND_EVENT(RegisterForLocationDiscovery, true);
		BIND_EVENT(RegisterForMagicEffectApplyEx, true);
		BIND_EVENT(RegisterForMagicHit, true);
		BIND_EVENT(RegisterForObjectGrab, true);
		BIND_EVENT(RegisterForObjectLoaded, true);
		BIND_EVENT(RegisterForObjectPoisoned, true);
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
		BIND_EVENT(UnregisterForFastTravelConfirmed, true);
		BIND_EVENT(UnregisterForFastTravelPrompt, true);
		BIND_EVENT(UnregisterForFurnitureEvent, true);
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
		BIND_EVENT(UnregisterForObjectPoisoned, true);
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

		logger::info("Registered {} activemagiceffect event functions"sv, count);
	}
}
