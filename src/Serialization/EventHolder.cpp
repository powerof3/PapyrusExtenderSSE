#include "Serialization/EventHolder.h"

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

namespace Event::Filter
{
	bool MagicEffectApply::Load(SKSE::SerializationInterface* a_intfc)
	{
		return stl::read_formID(a_intfc, effectID);
	}
	bool MagicEffectApply::Save(SKSE::SerializationInterface* a_intfc) const
	{
		return a_intfc->WriteRecordData(effectID);
	}
	bool MagicEffectApply::PassesFilter(RE::EffectSetting* a_baseEffect) const
	{
		return detail::passes_simple_filter(a_baseEffect, RE::TESForm::LookupByID(effectID));
	}

	bool Hit::Load(SKSE::SerializationInterface* a_intfc)
	{
		if (!stl::read_formID(a_intfc, aggressorID) || !stl::read_formID(a_intfc, sourceID) || !stl::read_formID(a_intfc, projectileID)) {
			return false;
		}
		if (!a_intfc->ReadRecordData(powerAttack) || !a_intfc->ReadRecordData(sneakAttack) || !a_intfc->ReadRecordData(bashAttack) || !a_intfc->ReadRecordData(blockAttack)) {
			return false;
		}
		return true;
	}
	bool Hit::Save(SKSE::SerializationInterface* a_intfc) const
	{
		if (!a_intfc->WriteRecordData(aggressorID) || !a_intfc->WriteRecordData(sourceID) || !a_intfc->WriteRecordData(projectileID)) {
			return false;
		}
		if (!a_intfc->WriteRecordData(powerAttack) || !a_intfc->WriteRecordData(sneakAttack) || !a_intfc->WriteRecordData(bashAttack) || !a_intfc->WriteRecordData(blockAttack)) {
			return false;
		}
		return true;
	}
	bool Hit::PassesFilter(RE::TESObjectREFR* a_aggressor, RE::TESForm* a_source, RE::BGSProjectile* a_projectile, bool a_powerAttack, bool a_sneakAttack, bool a_bashAttack, bool a_blockAttack) const
	{
		bool result = true;

		if (a_aggressor) {
			result = detail::passes_ref_filter(a_aggressor, RE::TESForm::LookupByID(aggressorID));
		}
		if (result && a_source) {
			result = detail::passes_simple_filter(a_source, RE::TESForm::LookupByID(sourceID));
		}
		if (result && a_projectile) {
			result = detail::passes_simple_filter(a_projectile, RE::TESForm::LookupByID(projectileID));
		}
		if (result) {
			result = detail::passes_hit_filter(a_powerAttack, powerAttack);
		}
		if (result) {
			result = detail::passes_hit_filter(a_sneakAttack, sneakAttack);
		}
		if (result) {
			result = detail::passes_hit_filter(a_bashAttack, bashAttack);
		}
		if (result) {
			result = detail::passes_hit_filter(a_blockAttack, blockAttack);
		}

		return result;
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
		booksRead.Save(a_intfc, kBookRead, a_version);
		fastTravelPrompt.Save(a_intfc, kFastTravelPrompt, a_version);
#ifdef SKYRIMVR
		fastTravelEnd.Save(a_intfc, kFastTravelEnd, a_version);
#endif
		itemCrafted.Save(a_intfc, kItemCrafted, a_version);
		magicApply.Save(a_intfc, kMagicEffectApply, a_version);
		magicHit.Save(a_intfc, kMagicHit, a_version);
		onHit.Save(a_intfc, kHit, a_version);
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
		case kBookRead:
			booksRead.Load(a_intfc);
			break;
		case kFastTravelPrompt:
			fastTravelPrompt.Load(a_intfc);
			break;
#ifdef SKYRIMVR
		case kFastTravelEnd :
			fastTravelEnd.Load(a_intfc);
			break;
#endif
		case kItemCrafted:
			itemCrafted.Load(a_intfc);
			break;
		case kMagicEffectApply:
			magicApply.Load(a_intfc);
			break;
		case kMagicHit:
			magicHit.Load(a_intfc);
			break;
		case kHit:
			onHit.Load(a_intfc);
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
		booksRead.Revert(a_intfc);
		fastTravelPrompt.Revert(a_intfc);
#ifdef SKYRIMVR
		fastTravelEnd.Revert(a_intfc);
#endif
		itemCrafted.Revert(a_intfc);
		magicApply.Revert(a_intfc);
		magicHit.Revert(a_intfc);
		onHit.Revert(a_intfc);
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
		booksRead.Unregister(a_handle);
		fastTravelPrompt.Unregister(a_handle);
#ifdef SKYRIMVR
		fastTravelEnd.Unregister(a_handle);
#endif
		itemCrafted.Unregister(a_handle);
		magicApply.UnregisterAll(a_handle);
		magicHit.Unregister(a_handle);
		onHit.UnregisterAll(a_handle);
		projectileHit.Unregister(a_handle);
		weaponHit.Unregister(a_handle);
		weatherChange.Unregister(a_handle);
	}

	void GameEventHolder::FormDelete(RE::FormID a_uniqueID)
	{
		actorFallLongDistance.Unregister(a_uniqueID);
		actorReanimateStart.Unregister(a_uniqueID);
		actorReanimateStop.Unregister(a_uniqueID);
		actorResurrect.Unregister(a_uniqueID);
		fastTravelPrompt.Unregister(a_uniqueID);
#ifdef SKYRIMVR
		fastTravelEnd.Unregister(a_uniqueID);
#endif
		magicApply.UnregisterAll(a_uniqueID);
		magicHit.Unregister(a_uniqueID);
		onHit.UnregisterAll(a_uniqueID);
		projectileHit.Unregister(a_uniqueID);
		weaponHit.Unregister(a_uniqueID);
	}
}
