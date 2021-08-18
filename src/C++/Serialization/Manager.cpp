#include "Serialization/Manager.h"

#include "Serialization/Events.h"
#include "Serialization/Services.h"

namespace Serialization
{
	using namespace Form;
	using namespace Events::Script;
	using namespace Events::Story;
	using namespace Events::Game;
	using namespace Events::FEC;

	std::string DecodeTypeCode(std::uint32_t a_typeCode)
	{
		constexpr std::size_t SIZE = sizeof(std::uint32_t);

		std::string sig;
		sig.resize(SIZE);
		char* iter = reinterpret_cast<char*>(&a_typeCode);
		for (std::size_t i = 0, j = SIZE - 2; i < SIZE - 1; ++i, --j) {
			sig[j] = iter[i];
		}

		return sig;
	}

	void SaveCallback(SKSE::SerializationInterface* a_intfc)
	{
		SAVE_FORMS<PerkManager>(a_intfc, kAddPerks, kRemovePerks);
		SAVE_FORMS<KeywordManager>(a_intfc, kAddKeywords, kRemoveKeywords);

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

		SAVE_FORMS<Detection::TargetManager>(a_intfc, kTargetHide, kTargetAlert);
		SAVE_FORMS<Detection::SourceManager>(a_intfc, kSourceHide, kSourceAlert);

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
			case kAddPerks:
				LOAD_FORMS<PerkManager>(a_intfc, Form::kAdd);
				break;
			case kRemovePerks:
				LOAD_FORMS<PerkManager>(a_intfc, Form::kRemove);
				break;
			case kAddKeywords:
				LOAD_FORMS<KeywordManager>(a_intfc, Form::kAdd);
				break;
			case kRemoveKeywords:
				LOAD_FORMS<KeywordManager>(a_intfc, Form::kRemove);
				break;
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
			case kTargetHide:
				LOAD_FORMS<Detection::TargetManager>(a_intfc, Detection::kHide);
				break;
			case kTargetAlert:
				LOAD_FORMS<Detection::TargetManager>(a_intfc, Detection::kAlert);
				break;
			case kSourceHide:
				LOAD_FORMS<Detection::SourceManager>(a_intfc, Detection::kHide);
				break;
			case kSourceAlert:
				LOAD_FORMS<Detection::SourceManager>(a_intfc, Detection::kAlert);
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
		REVERT<PerkManager>(a_intfc);
		REVERT<KeywordManager>(a_intfc);

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

		REVERT<Detection::TargetManager>(a_intfc);
		REVERT<Detection::SourceManager>(a_intfc);

		logger::info("Finished reverting data"sv);
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
				
				FORM_DELETE<KeywordManager>(formID);
				FORM_DELETE<PerkManager>(formID);
				
				FORM_DELETE<Detection::TargetManager>(formID);
				FORM_DELETE<Detection::SourceManager>(formID);
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
