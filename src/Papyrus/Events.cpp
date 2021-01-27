#include "Papyrus/Events.h"

#include "Serialization/Events.h"


namespace ScriptEvents
{
	using namespace Serialization::ScriptEvents;


	auto CellFullyLoadedEventHandler::GetSingleton() -> CellFullyLoadedEventHandler*
	{
		static CellFullyLoadedEventHandler singleton;
		return &singleton;
	}

	auto CellFullyLoadedEventHandler::ProcessEvent(const RE::TESCellFullyLoadedEvent* a_event, RE::BSTEventSource<RE::TESCellFullyLoadedEvent>* a_dispatcher)
		-> EventResult
	{
		if (!a_event || !a_event->cell) {
			return EventResult::kContinue;
		}

		OnCellFullyLoadedRegSet::GetSingleton()->QueueEvent(a_event->cell);

		return EventResult::kContinue;
	}


	auto QuestStartStopEventHandler::GetSingleton() -> QuestStartStopEventHandler*
	{
		static QuestStartStopEventHandler singleton;
		return &singleton;
	}

	auto QuestStartStopEventHandler::ProcessEvent(const RE::TESQuestStartStopEvent* a_event, RE::BSTEventSource<RE::TESQuestStartStopEvent>* a_dispatcher)
		-> EventResult
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID); quest) {
			a_event->started ? OnQuestStartRegMap::GetSingleton()->QueueEvent(a_event->formID, quest) : OnQuestStopRegMap::GetSingleton()->QueueEvent(a_event->formID, quest);
		}

		return EventResult::kContinue;
	}


	auto QuestStageEventHandler::GetSingleton() -> QuestStageEventHandler*
	{
		static QuestStageEventHandler singleton;
		return &singleton;
	}

	auto QuestStageEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>* a_dispatcher)
		-> EventResult
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID); quest) {
			OnQuestStageRegMap::GetSingleton()->QueueEvent(a_event->formID, quest, a_event->stage);
		}

		return EventResult::kContinue;
	}


	auto ObjectLoadedEventHandler::GetSingleton() -> ObjectLoadedEventHandler*
	{
		static ObjectLoadedEventHandler singleton;
		return &singleton;
	}

	auto ObjectLoadedEventHandler::ProcessEvent(const RE::TESObjectLoadedEvent* a_event, RE::BSTEventSource<RE::TESObjectLoadedEvent>* a_dispatcher)
		-> EventResult
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		if (auto object = RE::TESForm::LookupByID<RE::TESObjectREFR>(a_event->formID); object) {
			const auto base = object->GetBaseObject();
			if (base) {
				auto baseType = base->GetFormType();
				a_event->loaded ? OnObjectLoadedRegMap::GetSingleton()->QueueEvent(baseType, object, baseType) : OnObjectUnloadedRegMap::GetSingleton()->QueueEvent(baseType, object, baseType);
			}
		}

		return EventResult::kContinue;
	}


	auto GrabReleaseEventHandler::GetSingleton() -> GrabReleaseEventHandler*
	{
		static GrabReleaseEventHandler singleton;
		return &singleton;
	}

	auto GrabReleaseEventHandler::ProcessEvent(const RE::TESGrabReleaseEvent* a_event, RE::BSTEventSource<RE::TESGrabReleaseEvent>* a_dispatcher)
		-> EventResult
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		if (const auto object = a_event->ref.get(); object) {
			a_event->grabbed ? OnGrabRegSet::GetSingleton()->QueueEvent(object) : OnReleaseRegSet::GetSingleton()->QueueEvent(object);
		}

		return EventResult::kContinue;
	}
}


namespace StoryEvents
{
	using namespace Serialization::StoryEvents;


	auto ActorKillEventHandler::GetSingleton() -> ActorKillEventHandler*
	{
		static ActorKillEventHandler singleton;
		return &singleton;
	}

	auto ActorKillEventHandler::ProcessEvent(const RE::ActorKill::Event* a_event, RE::BSTEventSource<RE::ActorKill::Event>* a_dispatcher)
		-> EventResult
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		const auto victim = a_event->victim;
		const auto killer = a_event->killer;

		if (victim && killer) {
			OnActorKillRegSet::GetSingleton()->QueueEvent(victim, killer);
		}

		return EventResult::kContinue;
	}


	auto BooksReadEventHandler::GetSingleton() -> BooksReadEventHandler*
	{
		static BooksReadEventHandler singleton;
		return &singleton;
	}

	auto BooksReadEventHandler::ProcessEvent(const RE::BooksRead::Event* a_event, RE::BSTEventSource<RE::BooksRead::Event>* a_dispatcher) -> EventResult
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		if (const auto book = a_event->book; book) {
			OnBooksReadRegSet::GetSingleton()->QueueEvent(book);
		}

		return EventResult::kContinue;
	}


	auto CriticalHitEventHandler::GetSingleton() -> CriticalHitEventHandler*
	{
		static CriticalHitEventHandler singleton;
		return &singleton;
	}

	auto CriticalHitEventHandler::ProcessEvent(const RE::CriticalHit::Event* a_event, RE::BSTEventSource<RE::CriticalHit::Event>* a_dispatcher) -> EventResult
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		auto agressor = a_event->aggressor;
		const auto weapon = a_event->weapon;

		if (agressor && weapon) {
			if (const auto agressorActor = agressor->As<RE::Actor>(); agressorActor) {
				OnCriticalHitRegSet::GetSingleton()->QueueEvent(agressorActor, weapon, a_event->sneakHit);
			}
		}

		return EventResult::kContinue;
	}


	auto DisarmedEventHandler::GetSingleton() -> DisarmedEventHandler*
	{
		static DisarmedEventHandler singleton;
		return &singleton;
	}

	auto DisarmedEventHandler::ProcessEvent(const RE::DisarmedEvent::Event* a_event, RE::BSTEventSource<RE::DisarmedEvent::Event>* a_dispatcher) -> EventResult
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		const auto source = a_event->source;
		const auto target = a_event->target;

		if (source && target) {
			OnDisarmedRegSet::GetSingleton()->QueueEvent(source, target);
		}

		return EventResult::kContinue;
	}


	auto DragonSoulsGainedEventHandler::GetSingleton() -> DragonSoulsGainedEventHandler*
	{
		static DragonSoulsGainedEventHandler singleton;
		return &singleton;
	}

	auto DragonSoulsGainedEventHandler::ProcessEvent(const RE::DragonSoulsGained::Event* a_event, RE::BSTEventSource<RE::DragonSoulsGained::Event>* a_dispatcher) -> EventResult
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		OnDragonSoulsGainedRegSet::GetSingleton()->QueueEvent(a_event->souls);

		return EventResult::kContinue;
	}


	auto ItemHarvestedEventHandler::GetSingleton() -> ItemHarvestedEventHandler*
	{
		static ItemHarvestedEventHandler singleton;
		return &singleton;
	}

	auto ItemHarvestedEventHandler::ProcessEvent(const RE::TESHarvestedEvent::ItemHarvested* a_event, RE::BSTEventSource<RE::TESHarvestedEvent::ItemHarvested>* a_dispatcher) -> EventResult
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		if (const auto produce = a_event->produceItem; produce) {
			OnItemHarvestedRegSet::GetSingleton()->QueueEvent(produce);
		}

		return EventResult::kContinue;
	}


	auto LevelIncreaseEventHandler::GetSingleton() -> LevelIncreaseEventHandler*
	{
		static LevelIncreaseEventHandler singleton;
		return &singleton;
	}

	auto LevelIncreaseEventHandler::ProcessEvent(const RE::LevelIncrease::Event* a_event, RE::BSTEventSource<RE::LevelIncrease::Event>* a_dispatcher) -> EventResult
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		OnLevelIncreaseRegSet::GetSingleton()->QueueEvent(a_event->newLevel);

		return EventResult::kContinue;
	}


	auto LocationDiscoveryEventHandler::GetSingleton() -> LocationDiscoveryEventHandler*
	{
		static LocationDiscoveryEventHandler singleton;
		return &singleton;
	}

	auto LocationDiscoveryEventHandler::ProcessEvent(const RE::LocationDiscovery::Event* a_event, RE::BSTEventSource<RE::LocationDiscovery::Event>* a_dispatcher) -> EventResult
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		if (const auto data = a_event->mapMarkerData; data) {
			OnLocationDiscoveryRegSet::GetSingleton()->QueueEvent(data->locationName.fullName, a_event->worldspaceID);
		}

		return EventResult::kContinue;
	}


	auto ShoutAttackEventHandler::GetSingleton() -> ShoutAttackEventHandler*
	{
		static ShoutAttackEventHandler singleton;
		return &singleton;
	}

	auto ShoutAttackEventHandler::ProcessEvent(const RE::ShoutAttack::Event* a_event, RE::BSTEventSource<RE::ShoutAttack::Event>* a_dispatcher) -> EventResult
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		if (const auto shout = a_event->shout; shout) {
			OnShoutAttackRegSet::GetSingleton()->QueueEvent(shout);
		}

		return EventResult::kContinue;
	}

	auto SkillIncreaseEventHandler::GetSingleton() -> SkillIncreaseEventHandler*
	{
		static SkillIncreaseEventHandler singleton;
		return &singleton;
	}

	auto SkillIncreaseEventHandler::ProcessEvent(const RE::SkillIncrease::Event* a_event, RE::BSTEventSource<RE::SkillIncrease::Event>* a_dispatcher) -> EventResult
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		RE::BSFixedString avName;

		auto actorValueList = RE::ActorValueList::GetSingleton();
		if (actorValueList) {
			if (const auto av = actorValueList->GetActorValue(a_event->actorValue); av) {
				avName = av->enumName;
			}
		}

		if (!avName.empty()) {
			OnSkillIncreaseRegSet::GetSingleton()->QueueEvent(avName);
		}

		return EventResult::kContinue;
	}


	auto SoulsTrappedEventHandler::GetSingleton() -> SoulsTrappedEventHandler*
	{
		static SoulsTrappedEventHandler singleton;
		return &singleton;
	}

	auto SoulsTrappedEventHandler::ProcessEvent(const RE::SoulsTrapped::Event* a_event, RE::BSTEventSource<RE::SoulsTrapped::Event>* a_dispatcher)
		-> EventResult
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		const auto trapper = a_event->trapper;
		const auto target = a_event->target;

		if (trapper && target) {
			OnSoulsTrappedRegSet::GetSingleton()->QueueEvent(target, trapper);
		}

		return EventResult::kContinue;
	}


	auto SpellsLearnedEventHandler::GetSingleton() -> SpellsLearnedEventHandler*
	{
		static SpellsLearnedEventHandler singleton;
		return &singleton;
	}

	auto SpellsLearnedEventHandler::ProcessEvent(const RE::SpellsLearned::Event* a_event, RE::BSTEventSource<RE::SpellsLearned::Event>* a_dispatcher) -> EventResult
	{
		if (!a_event) {
			return EventResult::kContinue;
		}


		if (auto spell = a_event->spell; spell) {
			OnSpellsLearnedRegSet::GetSingleton()->QueueEvent(spell);
		}

		return EventResult::kContinue;
	}
}
