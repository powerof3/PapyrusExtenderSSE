#pragma once

namespace Papyrus::Alias::Events
{
	void RegisterForActorFallLongDistance(STATIC_ARGS, RE::BGSRefAlias* a_alias);
	void RegisterForActorKilled(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void RegisterForActorReanimateStart(STATIC_ARGS, RE::BGSBaseAlias* a_alias);
	void RegisterForActorReanimateStop(STATIC_ARGS, RE::BGSBaseAlias* a_alias);
	void RegisterForActorResurrected(STATIC_ARGS, RE::BGSBaseAlias* a_alias);
	void RegisterForBookRead(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void RegisterForCellFullyLoaded(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void RegisterForCriticalHit(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void RegisterForDisarmed(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void RegisterForDragonSoulGained(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void RegisterForOnPlayerFastTravelEnd(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void RegisterForFastTravelConfirmed(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void RegisterForFastTravelPrompt(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void RegisterForFurnitureEvent(STATIC_ARGS, RE::BGSRefAlias* a_alias);

	void RegisterForHitEventEx(STATIC_ARGS, RE::BGSRefAlias* a_alias,
		RE::TESForm* a_aggressorFilter,
		RE::TESForm* a_sourceFilter,
		RE::TESForm* a_projectileFilter,
		std::int32_t a_powerFilter,
		std::int32_t a_sneakFilter,
		std::int32_t a_bashFilter,
		std::int32_t a_blockFilter,
		bool         a_match);

	void RegisterForItemCrafted(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void RegisterForItemHarvested(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void RegisterForLevelIncrease(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void RegisterForLocationDiscovery(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void RegisterForMagicEffectApplyEx(STATIC_ARGS, RE::BGSRefAlias* a_alias, RE::TESForm* a_effectFilter, bool a_match);
	void RegisterForMagicHit(STATIC_ARGS, RE::BGSRefAlias* a_alias);
	void RegisterForObjectGrab(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void RegisterForObjectLoaded(STATIC_ARGS, const RE::BGSBaseAlias* a_alias, std::uint32_t a_formType);
	void RegisterForObjectPoisoned(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void RegisterForProjectileHit(STATIC_ARGS, RE::BGSRefAlias* a_alias);
	void RegisterForQuest(STATIC_ARGS, const RE::BGSBaseAlias* a_alias, RE::TESQuest* a_quest);
	void RegisterForQuestStage(STATIC_ARGS, const RE::BGSBaseAlias* a_alias, const RE::TESQuest* a_quest);
	void RegisterForShoutAttack(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void RegisterForSkillIncrease(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void RegisterForSoulTrapped(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void RegisterForSpellLearned(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void RegisterForWeatherChange(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void RegisterForWeaponHit(STATIC_ARGS, RE::BGSRefAlias* a_alias);
	void UnregisterForActorFallLongDistance(STATIC_ARGS, RE::BGSRefAlias* a_alias);
	void UnregisterForActorKilled(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void UnregisterForActorReanimateStart(STATIC_ARGS, RE::BGSBaseAlias* a_alias);
	void UnregisterForActorReanimateStop(STATIC_ARGS, RE::BGSBaseAlias* a_alias);
	void UnregisterForActorResurrected(STATIC_ARGS, RE::BGSBaseAlias* a_alias);
	void UnregisterForBookRead(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void UnregisterForCellFullyLoaded(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void UnregisterForCriticalHit(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void UnregisterForDisarmed(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void UnregisterForDragonSoulGained(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void UnregisterForOnPlayerFastTravelEnd(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void UnregisterForFastTravelConfirmed(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void UnregisterForFastTravelPrompt(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void UnregisterForFurnitureEvent(STATIC_ARGS, RE::BGSRefAlias* a_alias);

	void UnregisterForHitEventEx(STATIC_ARGS, RE::BGSRefAlias* a_alias,
		RE::TESForm* a_aggressorFilter,
		RE::TESForm* a_sourceFilter,
		RE::TESForm* a_projectileFilter,
		std::int32_t a_powerFilter,
		std::int32_t a_sneakFilter,
		std::int32_t a_bashFilter,
		std::int32_t a_blockFilter,
		bool         a_match);

	void UnregisterForAllHitEventsEx(STATIC_ARGS, RE::BGSRefAlias* a_alias);
	void UnregisterForItemCrafted(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void UnregisterForItemHarvested(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void UnregisterForLevelIncrease(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void UnregisterForLocationDiscovery(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void UnregisterForMagicEffectApplyEx(STATIC_ARGS, RE::BGSRefAlias* a_alias, RE::TESForm* a_effectFilter, bool a_match);
	void UnregisterForAllMagicEffectApplyEx(STATIC_ARGS, RE::BGSRefAlias* a_alias);
	void UnregisterForMagicHit(STATIC_ARGS, RE::BGSRefAlias* a_alias);
	void UnregisterForObjectGrab(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void UnregisterForObjectLoaded(STATIC_ARGS, const RE::BGSBaseAlias* a_alias, std::uint32_t a_formType);
	void UnregisterForAllObjectsLoaded(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void UnregisterForObjectPoisoned(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void UnregisterForProjectileHit(STATIC_ARGS, RE::BGSRefAlias* a_alias);
	void UnregisterForQuest(STATIC_ARGS, const RE::BGSBaseAlias* a_alias, const RE::TESQuest* a_quest);
	void UnregisterForAllQuests(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void UnregisterForQuestStage(STATIC_ARGS, const RE::BGSBaseAlias* a_alias, const RE::TESQuest* a_quest);
	void UnregisterForAllQuestStages(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void UnregisterForShoutAttack(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void UnregisterForSkillIncrease(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void UnregisterForSoulTrapped(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void UnregisterForSpellLearned(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void UnregisterForWeatherChange(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	void UnregisterForWeaponHit(STATIC_ARGS, RE::BGSRefAlias* a_alias);

	void Bind(VM& a_vm);
}
