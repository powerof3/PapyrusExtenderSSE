#include "Papyrus/Events.h"

#include "Serialization/Events.h"


namespace ScriptEvents
{
	using namespace Serialization::ScriptEvents;


	CellFullyLoadedEventHandler* CellFullyLoadedEventHandler::GetSingleton()
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


	QuestStartStopEventHandler* QuestStartStopEventHandler::GetSingleton()
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

		auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (quest) {
			a_event->started ? OnQuestStartRegMap::GetSingleton()->QueueEvent(a_event->formID, quest) : OnQuestStopRegMap::GetSingleton()->QueueEvent(a_event->formID, quest);
		}

		return EventResult::kContinue;
	}


	QuestStageEventHandler* QuestStageEventHandler::GetSingleton()
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

		auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID);
		if (quest) {
			OnQuestStageRegMap::GetSingleton()->QueueEvent(a_event->formID, quest, a_event->stage);
		}

		return EventResult::kContinue;
	}


	ObjectLoadedEventHandler* ObjectLoadedEventHandler::GetSingleton()
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

		auto object = RE::TESForm::LookupByID<RE::TESObjectREFR>(a_event->formID);
		if (object) {
			auto base = object->GetBaseObject();
			if (base) {
				auto baseType = base->GetFormType();
				a_event->loaded ? OnObjectLoadedRegMap::GetSingleton()->QueueEvent(baseType, object, baseType) : OnObjectUnloadedRegMap::GetSingleton()->QueueEvent(baseType, object, baseType);
			}
		}

		return EventResult::kContinue;
	}


	GrabReleaseEventHandler* GrabReleaseEventHandler::GetSingleton()
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

		auto object = a_event->ref.get();
		if (object) {
			a_event->grabbed ? OnGrabRegSet::GetSingleton()->QueueEvent(object) : OnReleaseRegSet::GetSingleton()->QueueEvent(object);
		}

		return EventResult::kContinue;
	}
}


namespace StoryEvents
{
	using namespace Serialization::StoryEvents;


	ActorKillEventHandler* ActorKillEventHandler::GetSingleton()
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

		auto victim = a_event->victim;
		auto killer = a_event->killer;
		if (victim && killer) {
			OnActorKillRegSet::GetSingleton()->QueueEvent(victim, killer);
		}

		return EventResult::kContinue;
	}


	BooksReadEventHandler* BooksReadEventHandler::GetSingleton()
	{
		static BooksReadEventHandler singleton;
		return &singleton;
	}

	auto BooksReadEventHandler::ProcessEvent(const RE::BooksRead::Event* a_event, RE::BSTEventSource<RE::BooksRead::Event>* a_dispatcher) -> EventResult
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		auto book = a_event->book;
		if (book) {
			OnBooksReadRegSet::GetSingleton()->QueueEvent(book);
		}

		return EventResult::kContinue;
	}


	CriticalHitEventHandler* CriticalHitEventHandler::GetSingleton()
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
		auto weapon = a_event->weapon;
		if (agressor && weapon) {
			auto agressorActor = agressor->As<RE::Actor>();
			if (agressorActor) {
				OnCriticalHitRegSet::GetSingleton()->QueueEvent(agressorActor, weapon, a_event->sneakHit);
			}
		}

		return EventResult::kContinue;
	}


	DisarmedEventHandler* DisarmedEventHandler::GetSingleton()
	{
		static DisarmedEventHandler singleton;
		return &singleton;
	}

	auto DisarmedEventHandler::ProcessEvent(const RE::DisarmedEvent::Event* a_event, RE::BSTEventSource<RE::DisarmedEvent::Event>* a_dispatcher) -> EventResult
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		auto source = a_event->source;
		auto target = a_event->target;
		if (source && target) {
			OnDisarmedRegSet::GetSingleton()->QueueEvent(source, target);
		}

		return EventResult::kContinue;
	}


	DragonSoulsGainedEventHandler* DragonSoulsGainedEventHandler::GetSingleton()
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


	ItemHarvestedEventHandler* ItemHarvestedEventHandler::GetSingleton()
	{
		static ItemHarvestedEventHandler singleton;
		return &singleton;
	}

	auto ItemHarvestedEventHandler::ProcessEvent(const RE::TESHarvestedEvent::ItemHarvested* a_event, RE::BSTEventSource<RE::TESHarvestedEvent::ItemHarvested>* a_dispatcher) -> EventResult
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		auto produce = a_event->produceItem;
		if (produce) {
			OnItemHarvestedRegSet::GetSingleton()->QueueEvent(produce);
		}

		return EventResult::kContinue;
	}


	LevelIncreaseEventHandler* LevelIncreaseEventHandler::GetSingleton()
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


	LocationDiscoveryEventHandler* LocationDiscoveryEventHandler::GetSingleton()
	{
		static LocationDiscoveryEventHandler singleton;
		return &singleton;
	}

	auto LocationDiscoveryEventHandler::ProcessEvent(const RE::LocationDiscovery::Event* a_event, RE::BSTEventSource<RE::LocationDiscovery::Event>* a_dispatcher) -> EventResult
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		auto data = a_event->mapMarkerData;
		if (data) {
			OnLocationDiscoveryRegSet::GetSingleton()->QueueEvent(data->locationName.fullName, a_event->worldspaceID);
		}

		return EventResult::kContinue;
	}


	ShoutAttackEventHandler* ShoutAttackEventHandler::GetSingleton()
	{
		static ShoutAttackEventHandler singleton;
		return &singleton;
	}

	auto ShoutAttackEventHandler::ProcessEvent(const RE::ShoutAttack::Event* a_event, RE::BSTEventSource<RE::ShoutAttack::Event>* a_dispatcher) -> EventResult
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		auto shout = a_event->shout;
		if (shout) {
			OnShoutAttackRegSet::GetSingleton()->QueueEvent(shout);
		}

		return EventResult::kContinue;
	}

	SkillIncreaseEventHandler* SkillIncreaseEventHandler::GetSingleton()
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
			auto av = actorValueList->GetActorValue(a_event->actorValue);
			if (av) {
				avName = av->enumName;
			}
		}

		if (!avName.empty()) {
			OnSkillIncreaseRegSet::GetSingleton()->QueueEvent(avName);
		}

		return EventResult::kContinue;
	}


	SoulsTrappedEventHandler* SoulsTrappedEventHandler::GetSingleton()
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

		auto trapper = a_event->trapper;
		auto target = a_event->target;
		
		if (trapper && target) {
			OnSoulsTrappedRegSet::GetSingleton()->QueueEvent(target, trapper);
		}

		return EventResult::kContinue;
	}


	SpellsLearnedEventHandler* SpellsLearnedEventHandler::GetSingleton()
	{
		static SpellsLearnedEventHandler singleton;
		return &singleton;
	}

	auto SpellsLearnedEventHandler::ProcessEvent(const RE::SpellsLearned::Event* a_event, RE::BSTEventSource<RE::SpellsLearned::Event>* a_dispatcher) -> EventResult
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		auto spell = a_event->spell;
		if (spell) {
			OnSpellsLearnedRegSet::GetSingleton()->QueueEvent(spell);
		}

		return EventResult::kContinue;
	}
}