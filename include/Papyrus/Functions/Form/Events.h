#pragma once

namespace Papyrus::Form::Events
{
	void RegisterForActorFallLongDistance(STATIC_ARGS, RE::TESForm* a_form);
	void RegisterForActorKilled(STATIC_ARGS, const RE::TESForm* a_form);
	void RegisterForActorReanimateStart(STATIC_ARGS, RE::TESForm* a_form);
	void RegisterForActorReanimateStop(STATIC_ARGS, RE::TESForm* a_form);
	void RegisterForActorResurrected(STATIC_ARGS, RE::TESForm* a_form);
	void RegisterForBookRead(STATIC_ARGS, const RE::TESForm* a_form);
	void RegisterForCellFullyLoaded(STATIC_ARGS, const RE::TESForm* a_form);
	void RegisterForCriticalHit(STATIC_ARGS, const RE::TESForm* a_form);
	void RegisterForDisarmed(STATIC_ARGS, const RE::TESForm* a_form);
	void RegisterForDragonSoulGained(STATIC_ARGS, const RE::TESForm* a_form);
	void RegisterForOnPlayerFastTravelEnd(STATIC_ARGS, const RE::TESForm* a_form);
	void RegisterForFastTravelConfirmed(STATIC_ARGS, const RE::TESForm* a_form);
	void RegisterForFastTravelPrompt(STATIC_ARGS, const RE::TESForm* a_form);
	void RegisterForFurnitureEvent(STATIC_ARGS, RE::TESForm* a_form);

	void RegisterForHitEventEx(STATIC_ARGS, RE::TESForm* a_form,
		RE::TESForm* a_aggressorFilter,
		RE::TESForm* a_sourceFilter,
		RE::TESForm* a_projectileFilter,
		std::int32_t a_powerFilter,
		std::int32_t a_sneakFilter,
		std::int32_t a_bashFilter,
		std::int32_t a_blockFilter,
		bool         a_match);

	void RegisterForItemCrafted(STATIC_ARGS, const RE::TESForm* a_form);
	void RegisterForItemHarvested(STATIC_ARGS, const RE::TESForm* a_form);
	void RegisterForLevelIncrease(STATIC_ARGS, const RE::TESForm* a_form);
	void RegisterForLocationDiscovery(STATIC_ARGS, const RE::TESForm* a_form);
	void RegisterForMagicEffectApplyEx(STATIC_ARGS, RE::TESForm* a_form, RE::TESForm* a_effectFilter, bool a_match);
	void RegisterForMagicHit(STATIC_ARGS, RE::TESForm* a_form);
	void RegisterForObjectGrab(STATIC_ARGS, const RE::TESForm* a_form);
	void RegisterForObjectLoaded(STATIC_ARGS, const RE::TESForm* a_form, std::uint32_t a_formType);
	void RegisterForObjectPoisoned(STATIC_ARGS, const RE::TESForm* a_form);
	void RegisterForProjectileHit(STATIC_ARGS, RE::TESForm* a_form);
	void RegisterForQuest(STATIC_ARGS, const RE::TESForm* a_form, RE::TESQuest* a_quest);
	void RegisterForQuestStage(STATIC_ARGS, const RE::TESForm* a_form, const RE::TESQuest* a_quest);
	void RegisterForShoutAttack(STATIC_ARGS, const RE::TESForm* a_form);
	void RegisterForSkillIncrease(STATIC_ARGS, const RE::TESForm* a_form);
	void RegisterForSoulTrapped(STATIC_ARGS, const RE::TESForm* a_form);
	void RegisterForSpellLearned(STATIC_ARGS, const RE::TESForm* a_form);
	void RegisterForWeatherChange(STATIC_ARGS, const RE::TESForm* a_form);
	void RegisterForWeaponHit(STATIC_ARGS, RE::TESForm* a_form);

	void UnregisterForActorFallLongDistance(STATIC_ARGS, RE::TESForm* a_form);
	void UnregisterForActorKilled(STATIC_ARGS, const RE::TESForm* a_form);
	void UnregisterForActorReanimateStart(STATIC_ARGS, RE::TESForm* a_form);
	void UnregisterForActorReanimateStop(STATIC_ARGS, RE::TESForm* a_form);
	void UnregisterForActorResurrected(STATIC_ARGS, RE::TESForm* a_form);
	void UnregisterForBookRead(STATIC_ARGS, const RE::TESForm* a_form);
	void UnregisterForCellFullyLoaded(STATIC_ARGS, const RE::TESForm* a_form);
	void UnregisterForCriticalHit(STATIC_ARGS, const RE::TESForm* a_form);
	void UnregisterForDisarmed(STATIC_ARGS, const RE::TESForm* a_form);
	void UnregisterForDragonSoulGained(STATIC_ARGS, const RE::TESForm* a_form);
	void UnregisterForOnPlayerFastTravelEnd(STATIC_ARGS, const RE::TESForm* a_form);
	void UnregisterForFastTravelConfirmed(STATIC_ARGS, const RE::TESForm* a_form);
	void UnregisterForFastTravelPrompt(STATIC_ARGS, const RE::TESForm* a_form);
	void UnregisterForFurnitureEvent(STATIC_ARGS, RE::TESForm* a_form);

	void UnregisterForHitEventEx(STATIC_ARGS, RE::TESForm* a_form,
		RE::TESForm* a_aggressorFilter,
		RE::TESForm* a_sourceFilter,
		RE::TESForm* a_projectileFilter,
		std::int32_t a_powerFilter,
		std::int32_t a_sneakFilter,
		std::int32_t a_bashFilter,
		std::int32_t a_blockFilter,
		bool         a_match);

	void UnregisterForAllHitEventsEx(STATIC_ARGS, RE::TESForm* a_form);
	void UnregisterForItemCrafted(STATIC_ARGS, const RE::TESForm* a_form);
	void UnregisterForItemHarvested(STATIC_ARGS, const RE::TESForm* a_form);
	void UnregisterForLevelIncrease(STATIC_ARGS, const RE::TESForm* a_form);
	void UnregisterForLocationDiscovery(STATIC_ARGS, const RE::TESForm* a_form);
	void UnregisterForMagicEffectApplyEx(STATIC_ARGS, RE::TESForm* a_form, RE::TESForm* a_effectFilter, bool a_match);
	void UnregisterForAllMagicEffectApplyEx(STATIC_ARGS, RE::TESForm* a_form);
	void UnregisterForMagicHit(STATIC_ARGS, RE::TESForm* a_form);
	void UnregisterForObjectGrab(STATIC_ARGS, const RE::TESForm* a_form);
	void UnregisterForObjectLoaded(STATIC_ARGS, const RE::TESForm* a_form, std::uint32_t a_formType);
	void UnregisterForAllObjectsLoaded(STATIC_ARGS, const RE::TESForm* a_form);
	void UnregisterForObjectPoisoned(STATIC_ARGS, const RE::TESForm* a_form);
	void UnregisterForProjectileHit(STATIC_ARGS, RE::TESForm* a_form);
	void UnregisterForQuest(STATIC_ARGS, const RE::TESForm* a_form, const RE::TESQuest* a_quest);
	void UnregisterForAllQuests(STATIC_ARGS, const RE::TESForm* a_form);
	void UnregisterForQuestStage(STATIC_ARGS, const RE::TESForm* a_form, const RE::TESQuest* a_quest);
	void UnregisterForAllQuestStages(STATIC_ARGS, const RE::TESForm* a_form);
	void UnregisterForShoutAttack(STATIC_ARGS, const RE::TESForm* a_form);
	void UnregisterForSkillIncrease(STATIC_ARGS, const RE::TESForm* a_form);
	void UnregisterForSoulTrapped(STATIC_ARGS, const RE::TESForm* a_form);
	void UnregisterForSpellLearned(STATIC_ARGS, const RE::TESForm* a_form);
	void UnregisterForWeatherChange(STATIC_ARGS, const RE::TESForm* a_form);
	void UnregisterForWeaponHit(STATIC_ARGS, RE::TESForm* a_form);

	void Bind(VM& a_vm);
}
