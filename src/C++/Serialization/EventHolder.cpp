#include "EventHolder.h"

namespace Event
{
	void ScriptEventHolder::Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_version)
	{
		cellFullyLoaded.Save(a_intfc, kCellFullyLoaded, a_version);
		questStart.Save(a_intfc, kQuestStart, a_version);
		questStop.Save(a_intfc, kQuestStop, a_version);
		questStage.Save(a_intfc, kQuestStage, a_version);
		objectLoaded.Save(a_intfc, kObjectLoaded, a_version);
		objectUnloaded.Save(a_intfc, kObjectUnloaded, a_version);
		objectGrab.Save(a_intfc, kObjectGrab, a_version);
		objectRelease.Save(a_intfc, kObjectRelease, a_version);
	}

	void ScriptEventHolder::Load(SKSE::SerializationInterface* a_intfc, std::uint32_t a_type)
	{
		switch (a_type) {
		case kCellFullyLoaded:
			cellFullyLoaded.Load(a_intfc);
			break;
		case kQuestStart:
			questStart.Load(a_intfc);
			break;
		case kQuestStop:
			questStop.Load(a_intfc);
			break;
		case kObjectLoaded:
			objectLoaded.Load(a_intfc);
			break;
		case kObjectUnloaded:
			objectUnloaded.Load(a_intfc);
			break;
		case kObjectGrab:
			objectGrab.Load(a_intfc);
			break;
		case kObjectRelease:
			objectRelease.Load(a_intfc);
			break;
		default:
			break;
		}
	}

	void ScriptEventHolder::Revert(SKSE::SerializationInterface* a_intfc)
	{
		cellFullyLoaded.Revert(a_intfc);
		questStart.Revert(a_intfc);
		questStop.Revert(a_intfc);
		questStage.Revert(a_intfc);
		objectLoaded.Revert(a_intfc);
		objectUnloaded.Revert(a_intfc);
		objectGrab.Revert(a_intfc);
		objectRelease.Revert(a_intfc);
	}

	void ScriptEventHolder::FormDelete(RE::VMHandle a_handle)
	{
		cellFullyLoaded.Unregister(a_handle);
		questStart.UnregisterAll(a_handle);
		questStop.UnregisterAll(a_handle);
		questStage.UnregisterAll(a_handle);
		objectLoaded.UnregisterAll(a_handle);
		objectUnloaded.UnregisterAll(a_handle);
		objectGrab.Unregister(a_handle);
		objectRelease.Unregister(a_handle);
	}
}

namespace Event
{
	void StoryEventHolder::Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_version)
	{
		actorKill.Save(a_intfc, kActorKill, a_version);
		booksRead.Save(a_intfc, kBookRead, a_version);
		criticalHit.Save(a_intfc, kCritHit, a_version);
		disarmed.Save(a_intfc, kDisarm, a_version);
		dragonSoulsGained.Save(a_intfc, kDragonSoul, a_version);
		itemHarvested.Save(a_intfc, kHarvest, a_version);
		levelIncrease.Save(a_intfc, kLevelIncrease, a_version);
		locationDiscovery.Save(a_intfc, kLocDiscovery, a_version);
		shoutAttack.Save(a_intfc, kShoutAttack, a_version);
		skillIncrease.Save(a_intfc, kSkillIncrease, a_version);
		soulsTrapped.Save(a_intfc, kSoulTrap, a_version);
		spellsLearned.Save(a_intfc, kSpellLearned, a_version);
	}
	void StoryEventHolder::Load(SKSE::SerializationInterface* a_intfc, std::uint32_t a_type)
	{
		switch (a_type) {
		case kActorKill:
			actorKill.Load(a_intfc);
			break;
		case kBookRead:
			booksRead.Load(a_intfc);
			break;
		case kDisarm:
			disarmed.Load(a_intfc);
			break;
		case kDragonSoul:
			dragonSoulsGained.Load(a_intfc);
			break;
		case kHarvest:
			itemHarvested.Load(a_intfc);
			break;
		case kLevelIncrease:
			levelIncrease.Load(a_intfc);
			break;
		case kLocDiscovery:
			locationDiscovery.Load(a_intfc);
			break;
		case kShoutAttack:
			shoutAttack.Load(a_intfc);
			break;
		case kSkillIncrease:
			skillIncrease.Load(a_intfc);
			break;
		case kSoulTrap:
			soulsTrapped.Load(a_intfc);
			break;
		case kSpellLearned:
			spellsLearned.Load(a_intfc);
			break;
		default:
			break;
		}
	}
	void StoryEventHolder::Revert(SKSE::SerializationInterface* a_intfc)
	{
		actorKill.Revert(a_intfc);
		booksRead.Revert(a_intfc);
		criticalHit.Revert(a_intfc);
		disarmed.Revert(a_intfc);
		dragonSoulsGained.Revert(a_intfc);
		itemHarvested.Revert(a_intfc);
		levelIncrease.Revert(a_intfc);
		locationDiscovery.Revert(a_intfc);
		shoutAttack.Revert(a_intfc);
		skillIncrease.Revert(a_intfc);
		soulsTrapped.Revert(a_intfc);
		spellsLearned.Revert(a_intfc);
	}
	void StoryEventHolder::FormDelete(RE::VMHandle a_handle)
	{
		actorKill.Unregister(a_handle);
		booksRead.Unregister(a_handle);
		criticalHit.Unregister(a_handle);
		disarmed.Unregister(a_handle);
		dragonSoulsGained.Unregister(a_handle);
		itemHarvested.Unregister(a_handle);
		levelIncrease.Unregister(a_handle);
		locationDiscovery.Unregister(a_handle);
		shoutAttack.Unregister(a_handle);
		skillIncrease.Unregister(a_handle);
		soulsTrapped.Unregister(a_handle);
		spellsLearned.Unregister(a_handle);
	}
}

namespace Event
{
	void GameEventHolder::Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_version)
	{
		actorFallLongDistance.Save(a_intfc, kActorFallLongDistance, a_version);
		actorReanimateStart.Save(a_intfc, kActorReanimateStart, a_version);
		actorReanimateStop.Save(a_intfc, kActorReanimateStop, a_version);
		actorResurrect.Save(a_intfc, kActorResurrect, a_version);
		magicApply.Save(a_intfc, kMagicEffectApply, a_version);
		magicHit.Save(a_intfc, kMagicHit, a_version);
		projectileHit.Save(a_intfc, kProjectileHit, a_version);
		weaponHit.Save(a_intfc, kWeaponHit, a_version);
		weatherChange.Save(a_intfc, kWeatherChange, a_version);
	}

	void GameEventHolder::Load(SKSE::SerializationInterface* a_intfc, std::uint32_t a_type)
	{
		switch (a_type) {
		case kActorFallLongDistance:
			actorFallLongDistance.Load(a_intfc);
			break;
		case kActorReanimateStart:
			actorReanimateStart.Load(a_intfc);
			break;
		case kActorReanimateStop:
			actorReanimateStop.Load(a_intfc);
			break;
		case kActorResurrect:
			actorResurrect.Load(a_intfc);
			break;
		case kMagicEffectApply:
			magicApply.Load(a_intfc);
			break;
		case kMagicHit:
			magicHit.Load(a_intfc);
			break;
		case kProjectileHit:
			projectileHit.Load(a_intfc);
			break;
		case kWeaponHit:
			weaponHit.Load(a_intfc);
			break;
		case kWeatherChange:
			weatherChange.Load(a_intfc);
			break;
		default:
			break;
		}
	}

	void GameEventHolder::Revert(SKSE::SerializationInterface* a_intfc)
	{
		actorFallLongDistance.Revert(a_intfc);
		actorReanimateStart.Revert(a_intfc);
		actorReanimateStop.Revert(a_intfc);
		actorResurrect.Revert(a_intfc);
		magicApply.Revert(a_intfc);
		magicHit.Revert(a_intfc);
		projectileHit.Revert(a_intfc);
		weaponHit.Revert(a_intfc);
		weatherChange.Revert(a_intfc);
	}

	void GameEventHolder::FormDelete(RE::VMHandle a_handle)
	{
		actorFallLongDistance.Unregister(a_handle);
		actorReanimateStart.Unregister(a_handle);
		actorReanimateStop.Unregister(a_handle);
		actorResurrect.Unregister(a_handle);
		magicApply.UnregisterAll(a_handle);
		magicHit.Unregister(a_handle);
		projectileHit.Unregister(a_handle);
		weaponHit.Unregister(a_handle);
		weatherChange.Unregister(a_handle);
	}
}
