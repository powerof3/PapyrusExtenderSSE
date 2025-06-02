#pragma once

#include "Serialization/EventHolder.h"

namespace Papyrus::ActiveMagicEffect::Events
{
	void RegisterForActorFallLongDistance(STATIC_ARGS, RE::ActiveEffect* a_activeEffect);
	void RegisterForActorKilled(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void RegisterForActorReanimateStart(STATIC_ARGS, RE::ActiveEffect* a_activeEffect);
	void RegisterForActorReanimateStop(STATIC_ARGS, RE::ActiveEffect* a_activeEffect);
	void RegisterForActorResurrected(STATIC_ARGS, RE::ActiveEffect* a_activeEffect);
	void RegisterForBookRead(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void RegisterForCellFullyLoaded(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void RegisterForCriticalHit(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void RegisterForDisarmed(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void RegisterForDragonSoulGained(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void RegisterForOnPlayerFastTravelEnd(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void RegisterForFastTravelConfirmed(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void RegisterForFastTravelPrompt(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void RegisterForFurnitureEvent(STATIC_ARGS, RE::ActiveEffect* a_activeEffect);

	void RegisterForHitEventEx(STATIC_ARGS, RE::ActiveEffect* a_activeEffect,
		RE::TESForm* a_aggressorFilter,
		RE::TESForm* a_sourceFilter,
		RE::TESForm* a_projectileFilter,
		std::int32_t a_powerFilter,
		std::int32_t a_sneakFilter,
		std::int32_t a_bashFilter,
		std::int32_t a_blockFilter,
		bool         a_match);

	void RegisterForItemCrafted(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void RegisterForItemHarvested(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void RegisterForLevelIncrease(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void RegisterForLocationDiscovery(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void RegisterForMagicEffectApplyEx(STATIC_ARGS, RE::ActiveEffect* a_activeEffect, RE::TESForm* a_effectFilter, bool a_match);
	void RegisterForMagicHit(STATIC_ARGS, RE::ActiveEffect* a_activeEffect);
	void RegisterForObjectGrab(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void RegisterForObjectLoaded(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect, std::uint32_t a_formType);
	void RegisterForObjectPoisoned(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void RegisterForProjectileHit(STATIC_ARGS, RE::ActiveEffect* a_activeEffect);
	void RegisterForQuest(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect, const RE::TESQuest* a_quest);
	void RegisterForQuestStage(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect, const RE::TESQuest* a_quest);
	void RegisterForShoutAttack(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void RegisterForSkillIncrease(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void RegisterForSoulTrapped(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void RegisterForSpellLearned(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void RegisterForWeatherChange(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void RegisterForWeaponHit(STATIC_ARGS, RE::ActiveEffect* a_activeEffect);
	void UnregisterForActorFallLongDistance(STATIC_ARGS, RE::ActiveEffect* a_activeEffect);
	void UnregisterForActorKilled(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void UnregisterForActorReanimateStart(STATIC_ARGS, RE::ActiveEffect* a_activeEffect);
	void UnregisterForActorReanimateStop(STATIC_ARGS, RE::ActiveEffect* a_activeEffect);
	void UnregisterForActorResurrected(STATIC_ARGS, RE::ActiveEffect* a_activeEffect);
	void UnregisterForBookRead(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void UnregisterForCellFullyLoaded(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void UnregisterForCriticalHit(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void UnregisterForDisarmed(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void UnregisterForDragonSoulGained(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void UnregisterForOnPlayerFastTravelEnd(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void UnregisterForFastTravelConfirmed(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void UnregisterForFastTravelPrompt(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void UnregisterForFurnitureEvent(STATIC_ARGS, RE::ActiveEffect* a_activeEffect);

	void UnregisterForHitEventEx(STATIC_ARGS, RE::ActiveEffect* a_activeEffect,
		RE::TESForm* a_aggressorFilter,
		RE::TESForm* a_sourceFilter,
		RE::TESForm* a_projectileFilter,
		std::int32_t a_powerFilter,
		std::int32_t a_sneakFilter,
		std::int32_t a_bashFilter,
		std::int32_t a_blockFilter,
		bool         a_match);

	void UnregisterForAllHitEventsEx(STATIC_ARGS, RE::ActiveEffect* a_activeEffect);
	void UnregisterForItemCrafted(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void UnregisterForItemHarvested(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void UnregisterForLevelIncrease(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void UnregisterForLocationDiscovery(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void UnregisterForMagicEffectApplyEx(STATIC_ARGS, RE::ActiveEffect* a_activeEffect, RE::TESForm* a_effectFilter, bool a_match);
	void UnregisterForAllMagicEffectApplyEx(STATIC_ARGS, RE::ActiveEffect* a_activeEffect);
	void UnregisterForMagicHit(STATIC_ARGS, RE::ActiveEffect* a_activeEffect);
	void UnregisterForObjectGrab(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void UnregisterForObjectLoaded(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect, std::uint32_t a_formType);
	void UnregisterForObjectPoisoned(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void UnregisterForAllObjectsLoaded(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void UnregisterForProjectileHit(STATIC_ARGS, RE::ActiveEffect* a_activeEffect);
	void UnregisterForQuest(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect, const RE::TESQuest* a_quest);
	void UnregisterForAllQuests(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void UnregisterForQuestStage(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect, const RE::TESQuest* a_quest);
	void UnregisterForAllQuestStages(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void UnregisterForShoutAttack(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void UnregisterForSkillIncrease(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void UnregisterForSoulTrapped(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void UnregisterForSpellLearned(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void UnregisterForWeatherChange(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	void UnregisterForWeaponHit(STATIC_ARGS, RE::ActiveEffect* a_activeEffect);

	void Bind(VM& a_vm);
}
