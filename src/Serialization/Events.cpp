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
			Base("OnCellFullyLoaded")
		{}


		OnQuestStartRegMap* OnQuestStartRegMap::GetSingleton()
		{
			static OnQuestStartRegMap singleton;
			return &singleton;
		}

		OnQuestStartRegMap::OnQuestStartRegMap() :
			Base("OnQuestStart")
		{}


		OnQuestStopRegMap* OnQuestStopRegMap::GetSingleton()
		{
			static OnQuestStopRegMap singleton;
			return &singleton;
		}

		OnQuestStopRegMap::OnQuestStopRegMap() :
			Base("OnQuestStop")
		{}


		OnQuestStageRegMap* OnQuestStageRegMap::GetSingleton()
		{
			static OnQuestStageRegMap singleton;
			return &singleton;
		}

		OnQuestStageRegMap::OnQuestStageRegMap() :
			Base("OnQuestStageChange")
		{}


		OnObjectLoadedRegMap* OnObjectLoadedRegMap::GetSingleton()
		{
			static OnObjectLoadedRegMap singleton;
			return &singleton;
		}

		OnObjectLoadedRegMap::OnObjectLoadedRegMap() :
			Base("OnObjectLoaded")
		{}


		OnObjectUnloadedRegMap* OnObjectUnloadedRegMap::GetSingleton()
		{
			static OnObjectUnloadedRegMap singleton;
			return &singleton;
		}

		OnObjectUnloadedRegMap::OnObjectUnloadedRegMap() :
			Base("OnObjectUnloaded")
		{}


		OnGrabRegSet* OnGrabRegSet::GetSingleton()
		{
			static OnGrabRegSet singleton;
			return &singleton;
		}

		OnGrabRegSet::OnGrabRegSet() :
			Base("OnObjectGrab")
		{}


		OnReleaseRegSet* OnReleaseRegSet::GetSingleton()
		{
			static OnReleaseRegSet singleton;
			return &singleton;
		}

		OnReleaseRegSet::OnReleaseRegSet() :
			Base("OnObjectRelease")
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
			Base("OnActorKilled")
		{}


		OnBooksReadRegSet* OnBooksReadRegSet::GetSingleton()
		{
			static OnBooksReadRegSet singleton;
			return &singleton;
		}

		OnBooksReadRegSet::OnBooksReadRegSet() :
			Base("OnBookRead")
		{}


		OnCriticalHitRegSet* OnCriticalHitRegSet::GetSingleton()
		{
			static OnCriticalHitRegSet singleton;
			return &singleton;
		}

		OnCriticalHitRegSet::OnCriticalHitRegSet() :
			Base("OnCriticalHit")
		{}


		OnDisarmedRegSet* OnDisarmedRegSet::GetSingleton()
		{
			static OnDisarmedRegSet singleton;
			return &singleton;
		}

		OnDisarmedRegSet::OnDisarmedRegSet() :
			Base("OnDisarmed")
		{}


		OnDragonSoulsGainedRegSet* OnDragonSoulsGainedRegSet::GetSingleton()
		{
			static OnDragonSoulsGainedRegSet singleton;
			return &singleton;
		}

		OnDragonSoulsGainedRegSet::OnDragonSoulsGainedRegSet() :
			Base("OnDragonSoulsGained")
		{}

		OnItemHarvestedRegSet* OnItemHarvestedRegSet::GetSingleton()
		{
			static OnItemHarvestedRegSet singleton;
			return &singleton;
		}

		OnItemHarvestedRegSet::OnItemHarvestedRegSet() :
			Base("OnItemHarvested")
		{}


		OnLevelIncreaseRegSet* OnLevelIncreaseRegSet::GetSingleton()
		{
			static OnLevelIncreaseRegSet singleton;
			return &singleton;
		}

		OnLevelIncreaseRegSet::OnLevelIncreaseRegSet() :
			Base("OnLevelIncrease")
		{}


		OnLocationDiscoveryRegSet* OnLocationDiscoveryRegSet::GetSingleton()
		{
			static OnLocationDiscoveryRegSet singleton;
			return &singleton;
		}

		OnLocationDiscoveryRegSet::OnLocationDiscoveryRegSet() :
			Base("OnLocationDiscovery")
		{}


		OnShoutAttackRegSet* OnShoutAttackRegSet::GetSingleton()
		{
			static OnShoutAttackRegSet singleton;
			return &singleton;
		}

		OnShoutAttackRegSet::OnShoutAttackRegSet() :
			Base("OnPlayerShoutAttack")
		{}


		OnSkillIncreaseRegSet* OnSkillIncreaseRegSet::GetSingleton()
		{
			static OnSkillIncreaseRegSet singleton;
			return &singleton;
		}

		OnSkillIncreaseRegSet::OnSkillIncreaseRegSet() :
			Base("OnSkillIncrease")
		{}


		OnSoulsTrappedRegSet* OnSoulsTrappedRegSet::GetSingleton()
		{
			static OnSoulsTrappedRegSet singleton;
			return &singleton;
		}

		OnSoulsTrappedRegSet::OnSoulsTrappedRegSet() :
			Base("OnSoulTrapped")
		{}


		OnSpellsLearnedRegSet* OnSpellsLearnedRegSet::GetSingleton()
		{
			static OnSpellsLearnedRegSet singleton;
			return &singleton;
		}

		OnSpellsLearnedRegSet::OnSpellsLearnedRegSet() :
			Base("OnSpellLearned")
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
			Base("OnActorResurrected")
		{}


		OnActorReanimateStartRegSet* OnActorReanimateStartRegSet::GetSingleton()
		{
			static OnActorReanimateStartRegSet singleton;
			return &singleton;
		}

		OnActorReanimateStartRegSet::OnActorReanimateStartRegSet() :
			Base("OnActorReanimateStart")
		{}


		OnActorReanimateStopRegSet* OnActorReanimateStopRegSet::GetSingleton()
		{
			static OnActorReanimateStopRegSet singleton;
			return &singleton;
		}

		OnActorReanimateStopRegSet::OnActorReanimateStopRegSet() :
			Base("OnActorReanimateStop")
		{}


		OnWeatherChangeRegSet* OnWeatherChangeRegSet::GetSingleton()
		{
			static OnWeatherChangeRegSet singleton;
			return &singleton;
		}

		OnWeatherChangeRegSet::OnWeatherChangeRegSet() :
			Base("OnWeatherChanged")
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
			Base("OnFECReset")
		{}
	}
}