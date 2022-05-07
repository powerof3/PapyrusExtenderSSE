#include "Serialization/Manager.h"

#include "Serialization/EventHolder.h"
#include "Serialization/Services.h"

namespace Serialization
{
	using namespace Event;

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
		ScriptEventHolder::GetSingleton()->Save(a_intfc, kSerializationVersion);
		StoryEventHolder::GetSingleton()->Save(a_intfc, kSerializationVersion);
		GameEventHolder::GetSingleton()->Save(a_intfc, kSerializationVersion);

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
				{
					ScriptEventHolder::GetSingleton()->Load(a_intfc, type);
					StoryEventHolder::GetSingleton()->Load(a_intfc, type);
					GameEventHolder::GetSingleton()->Load(a_intfc, type);
				}
				break;
			}
		}
		logger::info("Finished loading data"sv);
	}

	void RevertCallback(SKSE::SerializationInterface* a_intfc)
	{
		ScriptEventHolder::GetSingleton()->Revert(a_intfc);
		StoryEventHolder::GetSingleton()->Revert(a_intfc);
		GameEventHolder::GetSingleton()->Revert(a_intfc);

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
		ScriptEventHolder::GetSingleton()->FormDelete(a_handle);
		StoryEventHolder::GetSingleton()->FormDelete(a_handle);
		GameEventHolder::GetSingleton()->FormDelete(a_handle);
	}

	namespace FormDeletion
	{
		EventResult EventHandler::ProcessEvent(const RE::TESFormDeleteEvent* a_event, RE::BSTEventSource<RE::TESFormDeleteEvent>*)
		{
			if (a_event && a_event->formID != 0) {
				const auto formID = a_event->formID;

				FORM_DELETE<FORM::KeywordManager>(formID);
				FORM_DELETE<FORM::PerkManager>(formID);

				FORM_DELETE<DETECTION::TargetManager>(formID);
				FORM_DELETE<DETECTION::SourceManager>(formID);

				GameEventHolder::GetSingleton()->FormDelete(formID);
			}

			return EventResult::kContinue;
		}
	}
}
