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

	EventResult CellFullyLoadedEventHandler::ProcessEvent(const RE::TESCellFullyLoadedEvent* a_event, RE::BSTEventSource<RE::TESCellFullyLoadedEvent>*)
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

	EventResult QuestStartStopEventHandler::ProcessEvent(const RE::TESQuestStartStopEvent* a_event, RE::BSTEventSource<RE::TESQuestStartStopEvent>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID); quest) {
			a_event->started ?
                OnQuestStartRegMap::GetSingleton()->QueueEvent(a_event->formID, quest) :
                OnQuestStopRegMap::GetSingleton()->QueueEvent(a_event->formID, quest);
		}

		return EventResult::kContinue;
	}

	QuestStageEventHandler* QuestStageEventHandler::GetSingleton()
	{
		static QuestStageEventHandler singleton;
		return &singleton;
	}

	EventResult QuestStageEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID); quest) {
			OnQuestStageRegMap::GetSingleton()->QueueEvent(a_event->formID, quest, a_event->stage);
		}

		return EventResult::kContinue;
	}

	ObjectLoadedEventHandler* ObjectLoadedEventHandler::GetSingleton()
	{
		static ObjectLoadedEventHandler singleton;
		return &singleton;
	}

	EventResult ObjectLoadedEventHandler::ProcessEvent(const RE::TESObjectLoadedEvent* a_event, RE::BSTEventSource<RE::TESObjectLoadedEvent>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		const auto object = RE::TESForm::LookupByID<RE::TESObjectREFR>(a_event->formID);
		const auto base = object ? object->GetBaseObject() : nullptr;

		if (base) {
			auto baseType = base->GetFormType();
			a_event->loaded ?
                OnObjectLoadedRegMap::GetSingleton()->QueueEvent(baseType, object, baseType) :
                OnObjectUnloadedRegMap::GetSingleton()->QueueEvent(baseType, object, baseType);
		}

		return EventResult::kContinue;
	}

	GrabReleaseEventHandler* GrabReleaseEventHandler::GetSingleton()
	{
		static GrabReleaseEventHandler singleton;
		return &singleton;
	}

	EventResult GrabReleaseEventHandler::ProcessEvent(const RE::TESGrabReleaseEvent* a_event, RE::BSTEventSource<RE::TESGrabReleaseEvent>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		if (const auto object = a_event->ref.get(); object) {
			a_event->grabbed ?
                OnGrabRegSet::GetSingleton()->QueueEvent(object) :
                OnReleaseRegSet::GetSingleton()->QueueEvent(object);
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

	EventResult ActorKillEventHandler::ProcessEvent(const RE::ActorKill::Event* a_event, RE::BSTEventSource<RE::ActorKill::Event>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		const auto victim = a_event->victim;
		const auto killer = a_event->killer;

		if (victim) {
			OnActorKillRegSet::GetSingleton()->QueueEvent(victim, killer);
		}

		return EventResult::kContinue;
	}

	BooksReadEventHandler* BooksReadEventHandler::GetSingleton()
	{
		static BooksReadEventHandler singleton;
		return &singleton;
	}

	EventResult BooksReadEventHandler::ProcessEvent(const RE::BooksRead::Event* a_event, RE::BSTEventSource<RE::BooksRead::Event>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		if (const auto book = a_event->book; book) {
			OnBooksReadRegSet::GetSingleton()->QueueEvent(book);
		}

		return EventResult::kContinue;
	}

	CriticalHitEventHandler* CriticalHitEventHandler::GetSingleton()
	{
		static CriticalHitEventHandler singleton;
		return &singleton;
	}

	EventResult CriticalHitEventHandler::ProcessEvent(const RE::CriticalHit::Event* a_event, RE::BSTEventSource<RE::CriticalHit::Event>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		const auto agressor = a_event->aggressor;
		const auto agressorActor = agressor ? agressor->As<RE::Actor>() : nullptr;

		const auto weapon = a_event->weapon;

		if (agressorActor && weapon) {
			OnCriticalHitRegSet::GetSingleton()->QueueEvent(agressorActor, weapon, a_event->sneakHit);
		}

		return EventResult::kContinue;
	}

	DisarmedEventHandler* DisarmedEventHandler::GetSingleton()
	{
		static DisarmedEventHandler singleton;
		return &singleton;
	}

	EventResult DisarmedEventHandler::ProcessEvent(const RE::DisarmedEvent::Event* a_event, RE::BSTEventSource<RE::DisarmedEvent::Event>*)
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

	DragonSoulsGainedEventHandler* DragonSoulsGainedEventHandler::GetSingleton()
	{
		static DragonSoulsGainedEventHandler singleton;
		return &singleton;
	}

	EventResult DragonSoulsGainedEventHandler::ProcessEvent(const RE::DragonSoulsGained::Event* a_event, RE::BSTEventSource<RE::DragonSoulsGained::Event>*)
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

	EventResult ItemHarvestedEventHandler::ProcessEvent(const RE::TESHarvestedEvent::ItemHarvested* a_event, RE::BSTEventSource<RE::TESHarvestedEvent::ItemHarvested>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		if (const auto produce = a_event->produceItem; produce) {
			OnItemHarvestedRegSet::GetSingleton()->QueueEvent(produce);
		}

		return EventResult::kContinue;
	}

	LevelIncreaseEventHandler* LevelIncreaseEventHandler::GetSingleton()
	{
		static LevelIncreaseEventHandler singleton;
		return &singleton;
	}

	EventResult LevelIncreaseEventHandler::ProcessEvent(const RE::LevelIncrease::Event* a_event, RE::BSTEventSource<RE::LevelIncrease::Event>*)
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

	EventResult LocationDiscoveryEventHandler::ProcessEvent(const RE::LocationDiscovery::Event* a_event, RE::BSTEventSource<RE::LocationDiscovery::Event>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		if (const auto data = a_event->mapMarkerData; data) {
			OnLocationDiscoveryRegSet::GetSingleton()->QueueEvent(data->locationName.fullName, a_event->worldspaceID);
		}

		return EventResult::kContinue;
	}

	ShoutAttackEventHandler* ShoutAttackEventHandler::GetSingleton()
	{
		static ShoutAttackEventHandler singleton;
		return &singleton;
	}

	EventResult ShoutAttackEventHandler::ProcessEvent(const RE::ShoutAttack::Event* a_event, RE::BSTEventSource<RE::ShoutAttack::Event>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		if (const auto shout = a_event->shout; shout) {
			OnShoutAttackRegSet::GetSingleton()->QueueEvent(shout);
		}

		return EventResult::kContinue;
	}

	SkillIncreaseEventHandler* SkillIncreaseEventHandler::GetSingleton()
	{
		static SkillIncreaseEventHandler singleton;
		return &singleton;
	}

	EventResult SkillIncreaseEventHandler::ProcessEvent(const RE::SkillIncrease::Event* a_event, RE::BSTEventSource<RE::SkillIncrease::Event>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		const auto actorValueList = RE::ActorValueList::GetSingleton();
		const auto actorValueInfo = actorValueList ? actorValueList->GetActorValue(a_event->actorValue) : nullptr;

		if (actorValueList) {
			OnSkillIncreaseRegSet::GetSingleton()->QueueEvent(actorValueInfo->enumName);
		}

		return EventResult::kContinue;
	}

	SoulsTrappedEventHandler* SoulsTrappedEventHandler::GetSingleton()
	{
		static SoulsTrappedEventHandler singleton;
		return &singleton;
	}

	EventResult SoulsTrappedEventHandler::ProcessEvent(const RE::SoulsTrapped::Event* a_event, RE::BSTEventSource<RE::SoulsTrapped::Event>*)
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

	SpellsLearnedEventHandler* SpellsLearnedEventHandler::GetSingleton()
	{
		static SpellsLearnedEventHandler singleton;
		return &singleton;
	}

	EventResult SpellsLearnedEventHandler::ProcessEvent(const RE::SpellsLearned::Event* a_event, RE::BSTEventSource<RE::SpellsLearned::Event>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		if (const auto spell = a_event->spell; spell) {
			OnSpellsLearnedRegSet::GetSingleton()->QueueEvent(spell);
		}

		return EventResult::kContinue;
	}
}
