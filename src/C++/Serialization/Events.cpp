#include "Serialization/Events.h"

namespace Serialization
{
	namespace ScriptEvents
	{
		OnCellFullyLoadedRegSet* OnCellFullyLoadedRegSet::GetSingleton()
		{
			static OnCellFullyLoadedRegSet singleton;
			return &singleton;
		}

		OnCellFullyLoadedRegSet::OnCellFullyLoadedRegSet() :
			Base("OnCellFullyLoaded"sv)
		{}

		OnQuestStartRegMap* OnQuestStartRegMap::GetSingleton()
		{
			static OnQuestStartRegMap singleton;
			return &singleton;
		}

		OnQuestStartRegMap::OnQuestStartRegMap() :
			Base("OnQuestStart"sv)
		{}

		OnQuestStopRegMap* OnQuestStopRegMap::GetSingleton()
		{
			static OnQuestStopRegMap singleton;
			return &singleton;
		}

		OnQuestStopRegMap::OnQuestStopRegMap() :
			Base("OnQuestStop"sv)
		{}

		OnQuestStageRegMap* OnQuestStageRegMap::GetSingleton()
		{
			static OnQuestStageRegMap singleton;
			return &singleton;
		}

		OnQuestStageRegMap::OnQuestStageRegMap() :
			Base("OnQuestStageChange"sv)
		{}

		OnObjectLoadedRegMap* OnObjectLoadedRegMap::GetSingleton()
		{
			static OnObjectLoadedRegMap singleton;
			return &singleton;
		}

		OnObjectLoadedRegMap::OnObjectLoadedRegMap() :
			Base("OnObjectLoaded"sv)
		{}

		OnObjectUnloadedRegMap* OnObjectUnloadedRegMap::GetSingleton()
		{
			static OnObjectUnloadedRegMap singleton;
			return &singleton;
		}

		OnObjectUnloadedRegMap::OnObjectUnloadedRegMap() :
			Base("OnObjectUnloaded"sv)
		{}

		OnGrabRegSet* OnGrabRegSet::GetSingleton()
		{
			static OnGrabRegSet singleton;
			return &singleton;
		}

		OnGrabRegSet::OnGrabRegSet() :
			Base("OnObjectGrab"sv)
		{}

		OnReleaseRegSet* OnReleaseRegSet::GetSingleton()
		{
			static OnReleaseRegSet singleton;
			return &singleton;
		}

		OnReleaseRegSet::OnReleaseRegSet() :
			Base("OnObjectRelease"sv)
		{}
	}

	namespace StoryEvents
	{
		OnActorKillRegSet* OnActorKillRegSet::GetSingleton()
		{
			static OnActorKillRegSet singleton;
			return &singleton;
		}

		OnActorKillRegSet::OnActorKillRegSet() :
			Base("OnActorKilled"sv)
		{}

		OnBooksReadRegSet* OnBooksReadRegSet::GetSingleton()
		{
			static OnBooksReadRegSet singleton;
			return &singleton;
		}

		OnBooksReadRegSet::OnBooksReadRegSet() :
			Base("OnBookRead"sv)
		{}

		OnCriticalHitRegSet* OnCriticalHitRegSet::GetSingleton()
		{
			static OnCriticalHitRegSet singleton;
			return &singleton;
		}

		OnCriticalHitRegSet::OnCriticalHitRegSet() :
			Base("OnCriticalHit"sv)
		{}

		OnDisarmedRegSet* OnDisarmedRegSet::GetSingleton()
		{
			static OnDisarmedRegSet singleton;
			return &singleton;
		}

		OnDisarmedRegSet::OnDisarmedRegSet() :
			Base("OnDisarmed"sv)
		{}

		OnDragonSoulsGainedRegSet* OnDragonSoulsGainedRegSet::GetSingleton()
		{
			static OnDragonSoulsGainedRegSet singleton;
			return &singleton;
		}

		OnDragonSoulsGainedRegSet::OnDragonSoulsGainedRegSet() :
			Base("OnDragonSoulsGained"sv)
		{}

		OnItemHarvestedRegSet* OnItemHarvestedRegSet::GetSingleton()
		{
			static OnItemHarvestedRegSet singleton;
			return &singleton;
		}

		OnItemHarvestedRegSet::OnItemHarvestedRegSet() :
			Base("OnItemHarvested"sv)
		{}

		OnLevelIncreaseRegSet* OnLevelIncreaseRegSet::GetSingleton()
		{
			static OnLevelIncreaseRegSet singleton;
			return &singleton;
		}

		OnLevelIncreaseRegSet::OnLevelIncreaseRegSet() :
			Base("OnLevelIncrease"sv)
		{}

		OnLocationDiscoveryRegSet* OnLocationDiscoveryRegSet::GetSingleton()
		{
			static OnLocationDiscoveryRegSet singleton;
			return &singleton;
		}

		OnLocationDiscoveryRegSet::OnLocationDiscoveryRegSet() :
			Base("OnLocationDiscovery"sv)
		{}

		OnShoutAttackRegSet* OnShoutAttackRegSet::GetSingleton()
		{
			static OnShoutAttackRegSet singleton;
			return &singleton;
		}

		OnShoutAttackRegSet::OnShoutAttackRegSet() :
			Base("OnPlayerShoutAttack"sv)
		{}

		OnSkillIncreaseRegSet* OnSkillIncreaseRegSet::GetSingleton()
		{
			static OnSkillIncreaseRegSet singleton;
			return &singleton;
		}

		OnSkillIncreaseRegSet::OnSkillIncreaseRegSet() :
			Base("OnSkillIncrease"sv)
		{}

		OnSoulsTrappedRegSet* OnSoulsTrappedRegSet::GetSingleton()
		{
			static OnSoulsTrappedRegSet singleton;
			return &singleton;
		}

		OnSoulsTrappedRegSet::OnSoulsTrappedRegSet() :
			Base("OnSoulTrapped"sv)
		{}

		OnSpellsLearnedRegSet* OnSpellsLearnedRegSet::GetSingleton()
		{
			static OnSpellsLearnedRegSet singleton;
			return &singleton;
		}

		OnSpellsLearnedRegSet::OnSpellsLearnedRegSet() :
			Base("OnSpellLearned"sv)
		{}
	}

	namespace HookedEvents
	{
		OnActorResurrectRegSet* OnActorResurrectRegSet::GetSingleton()
		{
			static OnActorResurrectRegSet singleton;
			return &singleton;
		}

		OnActorResurrectRegSet::OnActorResurrectRegSet() :
			Base("OnActorResurrected"sv)
		{}

		OnActorReanimateStartRegSet* OnActorReanimateStartRegSet::GetSingleton()
		{
			static OnActorReanimateStartRegSet singleton;
			return &singleton;
		}

		OnActorReanimateStartRegSet::OnActorReanimateStartRegSet() :
			Base("OnActorReanimateStart"sv)
		{}

		OnActorReanimateStopRegSet* OnActorReanimateStopRegSet::GetSingleton()
		{
			static OnActorReanimateStopRegSet singleton;
			return &singleton;
		}

		OnActorReanimateStopRegSet::OnActorReanimateStopRegSet() :
			Base("OnActorReanimateStop"sv)
		{}

		OnWeatherChangeRegSet* OnWeatherChangeRegSet::GetSingleton()
		{
			static OnWeatherChangeRegSet singleton;
			return &singleton;
		}

		OnWeatherChangeRegSet::OnWeatherChangeRegSet() :
			Base("OnWeatherChange"sv)
		{}

		OnMagicEffectApplyRegMap* OnMagicEffectApplyRegMap::GetSingleton()
		{
			static OnMagicEffectApplyRegMap singleton;
			return &singleton;
		}

		OnMagicEffectApplyRegMap::OnMagicEffectApplyRegMap() :
			Base("OnMagicEffectApplyEx"sv)
		{}

		OnWeaponHitRegSet* OnWeaponHitRegSet::GetSingleton()
		{
			static OnWeaponHitRegSet singleton;
			return &singleton;
		}

		OnWeaponHitRegSet::OnWeaponHitRegSet() :
			Base("OnWeaponHit"sv)
		{}

		OnMagicHitRegSet* OnMagicHitRegSet::GetSingleton()
		{
			static OnMagicHitRegSet singleton;
			return &singleton;
		}

		OnMagicHitRegSet::OnMagicHitRegSet() :
			Base("OnMagicHit"sv)
		{}

		OnProjectileHitRegSet* OnProjectileHitRegSet::GetSingleton()
		{
			static OnProjectileHitRegSet singleton;
			return &singleton;
		}

		OnProjectileHitRegSet::OnProjectileHitRegSet() :
			Base("OnProjectileHit"sv)
		{}
	}

	namespace FECEvents
	{
		OnFECResetRegMap* OnFECResetRegMap::GetSingleton()
		{
			static OnFECResetRegMap singleton;
			return &singleton;
		}

		OnFECResetRegMap::OnFECResetRegMap() :
			Base("OnFECReset"sv)
		{}
	}
}
