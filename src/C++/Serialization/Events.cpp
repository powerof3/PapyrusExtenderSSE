#include "Serialization/Events.h"

namespace Events
{
	namespace Script
	{
		OnCellFullyLoadedRegSet::OnCellFullyLoadedRegSet() :
			Base("OnCellFullyLoaded"sv)
		{}

		OnQuestStartRegMap::OnQuestStartRegMap() :
			Base("OnQuestStart"sv)
		{}

		OnQuestStopRegMap::OnQuestStopRegMap() :
			Base("OnQuestStop"sv)
		{}

		OnQuestStageRegMap::OnQuestStageRegMap() :
			Base("OnQuestStageChange"sv)
		{}

		OnObjectLoadedRegMap::OnObjectLoadedRegMap() :
			Base("OnObjectLoaded"sv)
		{}

		OnObjectUnloadedRegMap::OnObjectUnloadedRegMap() :
			Base("OnObjectUnloaded"sv)
		{}

		OnGrabRegSet::OnGrabRegSet() :
			Base("OnObjectGrab"sv)
		{}

		OnReleaseRegSet::OnReleaseRegSet() :
			Base("OnObjectRelease"sv)
		{}
	}

	namespace Story
	{
		OnActorKillRegSet::OnActorKillRegSet() :
			Base("OnActorKilled"sv)
		{}

		OnBooksReadRegSet::OnBooksReadRegSet() :
			Base("OnBookRead"sv)
		{}

		OnCriticalHitRegSet::OnCriticalHitRegSet() :
			Base("OnCriticalHit"sv)
		{}

		OnDisarmedRegSet::OnDisarmedRegSet() :
			Base("OnDisarmed"sv)
		{}

		OnDragonSoulsGainedRegSet::OnDragonSoulsGainedRegSet() :
			Base("OnDragonSoulsGained"sv)
		{}

		OnItemHarvestedRegSet::OnItemHarvestedRegSet() :
			Base("OnItemHarvested"sv)
		{}

		OnLevelIncreaseRegSet::OnLevelIncreaseRegSet() :
			Base("OnLevelIncrease"sv)
		{}

		OnLocationDiscoveryRegSet::OnLocationDiscoveryRegSet() :
			Base("OnLocationDiscovery"sv)
		{}

		OnShoutAttackRegSet::OnShoutAttackRegSet() :
			Base("OnPlayerShoutAttack"sv)
		{}

		OnSkillIncreaseRegSet::OnSkillIncreaseRegSet() :
			Base("OnSkillIncrease"sv)
		{}

		OnSoulsTrappedRegSet::OnSoulsTrappedRegSet() :
			Base("OnSoulTrapped"sv)
		{}

		OnSpellsLearnedRegSet::OnSpellsLearnedRegSet() :
			Base("OnSpellLearned"sv)
		{}
	}

	namespace Game
	{
		OnActorResurrectRegSet::OnActorResurrectRegSet() :
			Base("OnActorResurrected"sv)
		{}

		OnActorReanimateStartRegSet::OnActorReanimateStartRegSet() :
			Base("OnActorReanimateStart"sv)
		{}

		OnActorReanimateStopRegSet::OnActorReanimateStopRegSet() :
			Base("OnActorReanimateStop"sv)
		{}

		OnWeatherChangeRegSet::OnWeatherChangeRegSet() :
			Base("OnWeatherChange"sv)
		{}

		OnMagicEffectApplyRegMap::OnMagicEffectApplyRegMap() :
			Base("OnMagicEffectApplyEx"sv)
		{}

		OnWeaponHitRegSet::OnWeaponHitRegSet() :
			Base("OnWeaponHit"sv)
		{}

		OnMagicHitRegSet::OnMagicHitRegSet() :
			Base("OnMagicHit"sv)
		{}

		OnProjectileHitRegSet::OnProjectileHitRegSet() :
			Base("OnProjectileHit"sv)
		{}
	}

	namespace FEC
	{
		OnFECResetRegMap::OnFECResetRegMap() :
			Base("OnFECReset"sv)
		{}
	}
}
