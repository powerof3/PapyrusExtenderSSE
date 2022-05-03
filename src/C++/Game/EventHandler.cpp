#include "HookedEventHandler.h"

#include "Game/EventHandler.h"
#include "Serialization/EventHolder.h"

namespace Event
{
	EventResult ScriptEventHandler::ProcessEvent(const RE::TESCellFullyLoadedEvent* a_event, RE::BSTEventSource<RE::TESCellFullyLoadedEvent>*)
	{
		if (!a_event || !a_event->cell) {
			return EventResult::kContinue;
		}

		ScriptEventHolder::GetSingleton()->cellFullyLoaded.QueueEvent(a_event->cell);

		return EventResult::kContinue;
	}

	EventResult ScriptEventHandler::ProcessEvent(const RE::TESQuestStartStopEvent* a_event, RE::BSTEventSource<RE::TESQuestStartStopEvent>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID); quest) {
			auto& event = a_event->started ? ScriptEventHolder::GetSingleton()->questStart : ScriptEventHolder::GetSingleton()->questStop;
			event.QueueEvent(a_event->formID, quest);
		}

		return EventResult::kContinue;
	}

	EventResult ScriptEventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID); quest) {
			ScriptEventHolder::GetSingleton()->questStage.QueueEvent(a_event->formID, quest, a_event->stage);
		}

		return EventResult::kContinue;
	}

	EventResult ScriptEventHandler::ProcessEvent(const RE::TESObjectLoadedEvent* a_event, RE::BSTEventSource<RE::TESObjectLoadedEvent>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		const auto object = RE::TESForm::LookupByID<RE::TESObjectREFR>(a_event->formID);
		const auto base = object ? object->GetBaseObject() : nullptr;

		if (base) {
			auto& event = a_event->loaded ? ScriptEventHolder::GetSingleton()->objectLoaded : ScriptEventHolder::GetSingleton()->objectUnloaded;
			event.QueueEvent(base->GetFormType(), object, base->GetFormType());
		}

		return EventResult::kContinue;
	}

	EventResult ScriptEventHandler::ProcessEvent(const RE::TESGrabReleaseEvent* a_event, RE::BSTEventSource<RE::TESGrabReleaseEvent>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		if (const auto object = a_event->ref; object) {
			auto& event = a_event->grabbed ? ScriptEventHolder::GetSingleton()->objectGrab : ScriptEventHolder::GetSingleton()->objectRelease;
			event.QueueEvent(object.get());
		}

		return EventResult::kContinue;
	}

}

namespace Event
{
	EventResult StoryEventHandler::ProcessEvent(const RE::ActorKill::Event* a_event, RE::BSTEventSource<RE::ActorKill::Event>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		const auto victim = a_event->victim;
		const auto killer = a_event->killer;

		if (victim) {
			StoryEventHolder::GetSingleton()->actorKill.QueueEvent(victim, killer);
		}

		return EventResult::kContinue;
	}

	EventResult StoryEventHandler::ProcessEvent(const RE::CriticalHit::Event* a_event, RE::BSTEventSource<RE::CriticalHit::Event>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		const auto aggressor = a_event->aggressor;
		const auto aggressorActor = aggressor ? aggressor->As<RE::Actor>() : nullptr;

		const auto weapon = a_event->weapon;

		if (aggressorActor && weapon) {
			StoryEventHolder::GetSingleton()->criticalHit.QueueEvent(aggressorActor, weapon, a_event->sneakHit);
		}

		return EventResult::kContinue;
	}

	EventResult StoryEventHandler::ProcessEvent(const RE::DisarmedEvent::Event* a_event, RE::BSTEventSource<RE::DisarmedEvent::Event>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		const auto source = a_event->source;
		const auto target = a_event->target;

		if (source && target) {
			StoryEventHolder::GetSingleton()->disarmed.QueueEvent(source, target);
		}

		return EventResult::kContinue;
	}

	EventResult StoryEventHandler::ProcessEvent(const RE::DragonSoulsGained::Event* a_event, RE::BSTEventSource<RE::DragonSoulsGained::Event>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		StoryEventHolder::GetSingleton()->dragonSoulsGained.QueueEvent(a_event->souls);

		return EventResult::kContinue;
	}

	EventResult StoryEventHandler::ProcessEvent(const RE::TESHarvestedEvent::ItemHarvested* a_event, RE::BSTEventSource<RE::TESHarvestedEvent::ItemHarvested>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		if (const auto produce = a_event->produceItem; produce) {
			StoryEventHolder::GetSingleton()->itemHarvested.QueueEvent(produce);
		}

		return EventResult::kContinue;
	}

	EventResult StoryEventHandler::ProcessEvent(const RE::LevelIncrease::Event* a_event, RE::BSTEventSource<RE::LevelIncrease::Event>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		StoryEventHolder::GetSingleton()->levelIncrease.QueueEvent(a_event->newLevel);

		return EventResult::kContinue;
	}

	EventResult StoryEventHandler::ProcessEvent(const RE::LocationDiscovery::Event* a_event, RE::BSTEventSource<RE::LocationDiscovery::Event>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		if (const auto data = a_event->mapMarkerData; data) {
			StoryEventHolder::GetSingleton()->locationDiscovery.QueueEvent(data->locationName.fullName, a_event->worldspaceID);
		}

		return EventResult::kContinue;
	}

	EventResult StoryEventHandler::ProcessEvent(const RE::ShoutAttack::Event* a_event, RE::BSTEventSource<RE::ShoutAttack::Event>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		if (const auto shout = a_event->shout; shout) {
			StoryEventHolder::GetSingleton()->shoutAttack.QueueEvent(shout);
		}

		return EventResult::kContinue;
	}

	EventResult StoryEventHandler::ProcessEvent(const RE::SkillIncrease::Event* a_event, RE::BSTEventSource<RE::SkillIncrease::Event>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		StoryEventHolder::GetSingleton()->skillIncrease.QueueEvent(stl::to_underlying(a_event->actorValue));

		return EventResult::kContinue;
	}

	EventResult StoryEventHandler::ProcessEvent(const RE::SoulsTrapped::Event* a_event, RE::BSTEventSource<RE::SoulsTrapped::Event>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		const auto trapper = a_event->trapper;
		const auto target = a_event->target;

		if (trapper && target) {
			StoryEventHolder::GetSingleton()->soulsTrapped.QueueEvent(target, trapper);
		}

		return EventResult::kContinue;
	}

	EventResult StoryEventHandler::ProcessEvent(const RE::SpellsLearned::Event* a_event, RE::BSTEventSource<RE::SpellsLearned::Event>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}
#ifndef SKYRIM_AE  //AE crashes when this event is sent from Papyrus AddSpell (why??)
		if (const auto spell = a_event->spell; spell) {
			StoryEventHolder::GetSingleton()->spellsLearned.QueueEvent(spell);
		}
#endif
		return EventResult::kContinue;
	}
}

void Event::Register()
{
	ScriptEventHandler::Register();
	StoryEventHandler::Register();

	RegisterHookEvents();
}
