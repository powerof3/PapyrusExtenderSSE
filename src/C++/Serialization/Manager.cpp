#include "Serialization/Manager.h"

#include "Serialization/Events.h"
#include "Serialization/Services.h"

namespace Serialization
{
	using namespace Events::Script;
	using namespace Events::Story;
	using namespace Events::Game;
	using namespace Events::FEC;

	std::string DecodeTypeCode(std::uint32_t a_typeCode)
	{
		constexpr std::size_t SIZE = sizeof(std::uint32_t);

		std::string sig;
		sig.resize(SIZE);
		const char* iter = reinterpret_cast<char*>(&a_typeCode);
		for (std::size_t i = 0, j = SIZE - 2; i < SIZE - 1; ++i, --j) {
			sig[j] = iter[i];
		}

		return sig;
	}

	void SaveCallback(SKSE::SerializationInterface* a_intfc)
	{
		SAVE<OnCellFullyLoadedRegSet>(a_intfc, kOnCellFullyLoaded);
		SAVE<OnQuestStartRegMap>(a_intfc, kQuestStart);
		SAVE<OnQuestStopRegMap>(a_intfc, kQuestStop);
		SAVE<OnQuestStageRegMap>(a_intfc, kQuestStage);
		SAVE<OnObjectLoadedRegMap>(a_intfc, kObjectLoaded);
		SAVE<OnObjectUnloadedRegMap>(a_intfc, kObjectUnloaded);
		SAVE<OnGrabRegSet>(a_intfc, kGrab);
		SAVE<OnReleaseRegSet>(a_intfc, kRelease);

		SAVE<OnActorKillRegSet>(a_intfc, kActorKill);
		SAVE<OnBooksReadRegSet>(a_intfc, kBookRead);
		SAVE<OnCriticalHitRegSet>(a_intfc, kCritHit);
		SAVE<OnDisarmedRegSet>(a_intfc, kDisarm);
		SAVE<OnDragonSoulsGainedRegSet>(a_intfc, kDragonSoul);
		SAVE<OnItemHarvestedRegSet>(a_intfc, kHarvest);
		SAVE<OnLevelIncreaseRegSet>(a_intfc, kLevelIncrease);
		SAVE<OnLocationDiscoveryRegSet>(a_intfc, kLocDiscovery);
		SAVE<OnSkillIncreaseRegSet>(a_intfc, kSkillIncrease);
		SAVE<OnShoutAttackRegSet>(a_intfc, kShoutAttack);
		SAVE<OnSoulsTrappedRegSet>(a_intfc, kSoulTrap);
		SAVE<OnSpellsLearnedRegSet>(a_intfc, kSpellLearned);

		SAVE<OnActorResurrectRegSet>(a_intfc, kActorResurrect);
		SAVE<OnActorReanimateStartRegSet>(a_intfc, kActorReanimateStart);
		SAVE<OnActorReanimateStopRegSet>(a_intfc, kActorReanimateStop);
		SAVE<OnWeatherChangeRegSet>(a_intfc, kWeatherChange);
		SAVE<OnMagicEffectApplyRegMap>(a_intfc, kMagicEffectApply);
		SAVE<OnWeaponHitRegSet>(a_intfc, kWeaponHit);
		SAVE<OnMagicHitRegSet>(a_intfc, kMagicHit);
		SAVE<OnProjectileHitRegSet>(a_intfc, kProjectileHit);

		SAVE<OnFECResetRegMap>(a_intfc, kFECReset);

		SAVE<FORM::PerkManager>(a_intfc, kAddPerks, kRemovePerks);
		SAVE<FORM::KeywordManager>(a_intfc, kAddKeywords, kRemoveKeywords);

		SAVE<DETECTION::TargetManager>(a_intfc, kTargetHide, kTargetAlert);
		SAVE<DETECTION::SourceManager>(a_intfc, kSourceHide, kSourceAlert);

		SAVE<MAGIC::MGEFManager>(a_intfc, kAddMGEF, kRemoveMGEF);
		SAVE<MAGIC::EffectManager>(a_intfc, kAddEffect, kRemoveEffect);

		logger::info("Finished saving data"sv);
	}

	void LoadCallback(SKSE::SerializationInterface* a_intfc)
	{
		std::uint32_t type;
		std::uint32_t version;
		std::uint32_t length;
		while (a_intfc->GetNextRecordInfo(type, version, length)) {
			if (version != kSerializationVersion) {
				logger::critical("Loaded data is out of date! Read ({}), expected ({}) for type code ({})", version, kSerializationVersion, DecodeTypeCode(type));
				continue;
			}
			switch (type) {
			case kOnCellFullyLoaded:
				LOAD<OnCellFullyLoadedRegSet>(a_intfc);
				break;
			case kQuestStart:
				LOAD<OnQuestStartRegMap>(a_intfc);
				break;
			case kQuestStop:
				LOAD<OnQuestStopRegMap>(a_intfc);
				break;
			case kQuestStage:
				LOAD<OnQuestStageRegMap>(a_intfc);
				break;
			case kObjectLoaded:
				LOAD<OnObjectLoadedRegMap>(a_intfc);
				break;
			case kObjectUnloaded:
				LOAD<OnObjectUnloadedRegMap>(a_intfc);
				break;
			case kGrab:
				LOAD<OnGrabRegSet>(a_intfc);
				break;
			case kRelease:
				LOAD<OnReleaseRegSet>(a_intfc);
				break;
			case kActorKill:
				LOAD<OnActorKillRegSet>(a_intfc);
				break;
			case kBookRead:
				LOAD<OnBooksReadRegSet>(a_intfc);
				break;
			case kCritHit:
				LOAD<OnCriticalHitRegSet>(a_intfc);
				break;
			case kDisarm:
				LOAD<OnDisarmedRegSet>(a_intfc);
				break;
			case kDragonSoul:
				LOAD<OnDragonSoulsGainedRegSet>(a_intfc);
				break;
			case kHarvest:
				LOAD<OnItemHarvestedRegSet>(a_intfc);
				break;
			case kLevelIncrease:
				LOAD<OnLevelIncreaseRegSet>(a_intfc);
				break;
			case kLocDiscovery:
				LOAD<OnLocationDiscoveryRegSet>(a_intfc);
				break;
			case kShoutAttack:
				LOAD<OnShoutAttackRegSet>(a_intfc);
				break;
			case kSkillIncrease:
				LOAD<OnSkillIncreaseRegSet>(a_intfc);
				break;
			case kSoulTrap:
				LOAD<OnSoulsTrappedRegSet>(a_intfc);
				break;
			case kSpellLearned:
				LOAD<OnSpellsLearnedRegSet>(a_intfc);
				break;
			case kActorResurrect:
				LOAD<OnActorResurrectRegSet>(a_intfc);
				break;
			case kActorReanimateStart:
				LOAD<OnActorReanimateStartRegSet>(a_intfc);
				break;
			case kActorReanimateStop:
				LOAD<OnActorReanimateStopRegSet>(a_intfc);
				break;
			case kWeatherChange:
				LOAD<OnWeatherChangeRegSet>(a_intfc);
				break;
			case kMagicEffectApply:
				LOAD<OnMagicEffectApplyRegMap>(a_intfc);
				break;
			case kWeaponHit:
				LOAD<OnWeaponHitRegSet>(a_intfc);
				break;
			case kMagicHit:
				LOAD<OnMagicHitRegSet>(a_intfc);
				break;
			case kProjectileHit:
				LOAD<OnProjectileHitRegSet>(a_intfc);
				break;
			case kFECReset:
				LOAD<OnFECResetRegMap>(a_intfc);
				break;
			case kAddPerks:
				LOAD<FORM::PerkManager>(a_intfc, FORM::kAdd);
				break;
			case kRemovePerks:
				LOAD<FORM::PerkManager>(a_intfc, FORM::kRemove);
				break;
			case kAddKeywords:
				LOAD<FORM::KeywordManager>(a_intfc, FORM::kAdd);
				break;
			case kRemoveKeywords:
				LOAD<FORM::KeywordManager>(a_intfc, FORM::kRemove);
				break;
			case kTargetHide:
				LOAD<DETECTION::TargetManager>(a_intfc, DETECTION::kHide);
				break;
			case kTargetAlert:
				LOAD<DETECTION::TargetManager>(a_intfc, DETECTION::kAlert);
				break;
			case kSourceHide:
				LOAD<DETECTION::SourceManager>(a_intfc, DETECTION::kHide);
				break;
			case kSourceAlert:
				LOAD<DETECTION::SourceManager>(a_intfc, DETECTION::kAlert);
				break;
			case kAddMGEF:
				LOAD<MAGIC::MGEFManager>(a_intfc, FORM::kAdd);
				break;
			case kRemoveMGEF:
				LOAD<MAGIC::MGEFManager>(a_intfc, FORM::kRemove);
				break;
			case kAddEffect:
				LOAD<MAGIC::EffectManager>(a_intfc, FORM::kAdd);
				break;
			case kRemoveEffect:
				LOAD<MAGIC::EffectManager>(a_intfc, FORM::kRemove);
				break;
			default:
				logger::critical("Unrecognized record type ({})!"sv, DecodeTypeCode(type));
				break;
			}
		}
		logger::info("Finished loading data"sv);
	}

	void RevertCallback(SKSE::SerializationInterface* a_intfc)
	{
		REVERT<OnCellFullyLoadedRegSet>(a_intfc);
		REVERT<OnQuestStartRegMap>(a_intfc);
		REVERT<OnQuestStopRegMap>(a_intfc);
		REVERT<OnQuestStageRegMap>(a_intfc);
		REVERT<OnObjectLoadedRegMap>(a_intfc);
		REVERT<OnObjectUnloadedRegMap>(a_intfc);
		REVERT<OnGrabRegSet>(a_intfc);
		REVERT<OnReleaseRegSet>(a_intfc);
		REVERT<OnGrabRegSet>(a_intfc);

		REVERT<OnActorKillRegSet>(a_intfc);
		REVERT<OnBooksReadRegSet>(a_intfc);
		REVERT<OnCriticalHitRegSet>(a_intfc);
		REVERT<OnDisarmedRegSet>(a_intfc);
		REVERT<OnDragonSoulsGainedRegSet>(a_intfc);
		REVERT<OnItemHarvestedRegSet>(a_intfc);
		REVERT<OnLevelIncreaseRegSet>(a_intfc);
		REVERT<OnLocationDiscoveryRegSet>(a_intfc);
		REVERT<OnSkillIncreaseRegSet>(a_intfc);
		REVERT<OnShoutAttackRegSet>(a_intfc);
		REVERT<OnSoulsTrappedRegSet>(a_intfc);
		REVERT<OnSpellsLearnedRegSet>(a_intfc);

		REVERT<OnActorResurrectRegSet>(a_intfc);
		REVERT<OnActorReanimateStartRegSet>(a_intfc);
		REVERT<OnActorReanimateStopRegSet>(a_intfc);
		REVERT<OnWeatherChangeRegSet>(a_intfc);
		REVERT<OnMagicEffectApplyRegMap>(a_intfc);
		REVERT<OnWeaponHitRegSet>(a_intfc);
		REVERT<OnMagicHitRegSet>(a_intfc);
		REVERT<OnProjectileHitRegSet>(a_intfc);

		REVERT<OnFECResetRegMap>(a_intfc);

		REVERT<FORM::PerkManager>(a_intfc);
		REVERT<FORM::KeywordManager>(a_intfc);

		REVERT<DETECTION::TargetManager>(a_intfc);
		REVERT<DETECTION::SourceManager>(a_intfc);

		REVERT<MAGIC::MGEFManager>(a_intfc);
		REVERT<MAGIC::EffectManager>(a_intfc);

		logger::info("Finished reverting data"sv);
	}

	void FormDeleteCallback(RE::VMHandle a_handle)
	{
		FORM_DELETE<OnCellFullyLoadedRegSet>(a_handle);
		FORM_DELETE<OnQuestStartRegMap>(a_handle);
		FORM_DELETE<OnQuestStopRegMap>(a_handle);
		FORM_DELETE<OnQuestStageRegMap>(a_handle);
		FORM_DELETE<OnObjectLoadedRegMap>(a_handle);
		FORM_DELETE<OnObjectUnloadedRegMap>(a_handle);
		FORM_DELETE<OnGrabRegSet>(a_handle);
		FORM_DELETE<OnReleaseRegSet>(a_handle);
		FORM_DELETE<OnGrabRegSet>(a_handle);

		FORM_DELETE<OnActorKillRegSet>(a_handle);
		FORM_DELETE<OnBooksReadRegSet>(a_handle);
		FORM_DELETE<OnCriticalHitRegSet>(a_handle);
		FORM_DELETE<OnDisarmedRegSet>(a_handle);
		FORM_DELETE<OnDragonSoulsGainedRegSet>(a_handle);
		FORM_DELETE<OnItemHarvestedRegSet>(a_handle);
		FORM_DELETE<OnLevelIncreaseRegSet>(a_handle);
		FORM_DELETE<OnLocationDiscoveryRegSet>(a_handle);
		FORM_DELETE<OnSkillIncreaseRegSet>(a_handle);
		FORM_DELETE<OnShoutAttackRegSet>(a_handle);
		FORM_DELETE<OnSoulsTrappedRegSet>(a_handle);
		FORM_DELETE<OnSpellsLearnedRegSet>(a_handle);

		FORM_DELETE<OnActorResurrectRegSet>(a_handle);
		FORM_DELETE<OnActorReanimateStartRegSet>(a_handle);
		FORM_DELETE<OnActorReanimateStopRegSet>(a_handle);
		FORM_DELETE<OnWeatherChangeRegSet>(a_handle);
		FORM_DELETE<OnMagicEffectApplyRegMap>(a_handle);
		FORM_DELETE<OnWeaponHitRegSet>(a_handle);
		FORM_DELETE<OnMagicHitRegSet>(a_handle);
		FORM_DELETE<OnProjectileHitRegSet>(a_handle);

		FORM_DELETE<OnFECResetRegMap>(a_handle);
	}

	namespace FormDeletion
	{
		EventHandler* EventHandler::GetSingleton()
		{
			static EventHandler singleton;
			return &singleton;
		}

		EventResult EventHandler::ProcessEvent(const RE::TESFormDeleteEvent* a_event, RE::BSTEventSource<RE::TESFormDeleteEvent>*)
		{
			if (a_event && a_event->formID != 0) {
				const auto formID = a_event->formID;

				FORM_DELETE<FORM::KeywordManager>(formID);
				FORM_DELETE<FORM::PerkManager>(formID);

				FORM_DELETE<DETECTION::TargetManager>(formID);
				FORM_DELETE<DETECTION::SourceManager>(formID);
			}

			return EventResult::kContinue;
		}

		void Register()
		{
			auto scripts = RE::ScriptEventSourceHolder::GetSingleton();
			if (scripts) {
				scripts->AddEventSink(EventHandler::GetSingleton());
				logger::info("Registered form deletion event handler"sv);
			}
		}
	}
}
