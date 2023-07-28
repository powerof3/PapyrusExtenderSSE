#pragma once

namespace Event
{
	using EventResult = RE::BSEventNotifyControl;

	class ScriptEventHandler final :
		public ISingleton<ScriptEventHandler>,
		public RE::BSTEventSink<RE::TESCellFullyLoadedEvent>,
		public RE::BSTEventSink<RE::TESQuestStartStopEvent>,
		public RE::BSTEventSink<RE::TESQuestStageEvent>,
		public RE::BSTEventSink<RE::TESObjectLoadedEvent>,
		public RE::BSTEventSink<RE::TESGrabReleaseEvent>,
		public RE::BSTEventSink<RE::TESFurnitureEvent>
	{
	public:
		static void Register()
		{
			logger::info("{:*^30}", "SCRIPT EVENTS"sv);

			register_event<RE::TESCellFullyLoadedEvent>();
			register_event<RE::TESQuestStartStopEvent>();
			register_event<RE::TESQuestStageEvent>();
			register_event<RE::TESObjectLoadedEvent>();
			register_event<RE::TESGrabReleaseEvent>();
			register_event<RE::TESFurnitureEvent>();
		}

		EventResult ProcessEvent(const RE::TESCellFullyLoadedEvent* a_event, RE::BSTEventSource<RE::TESCellFullyLoadedEvent>*) override;
		EventResult ProcessEvent(const RE::TESQuestStartStopEvent* a_event, RE::BSTEventSource<RE::TESQuestStartStopEvent>*) override;
		EventResult ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) override;
		EventResult ProcessEvent(const RE::TESObjectLoadedEvent* a_event, RE::BSTEventSource<RE::TESObjectLoadedEvent>*) override;
		EventResult ProcessEvent(const RE::TESGrabReleaseEvent* a_event, RE::BSTEventSource<RE::TESGrabReleaseEvent>*) override;
		EventResult ProcessEvent(const RE::TESFurnitureEvent* a_event, RE::BSTEventSource<RE::TESFurnitureEvent>*) override;

	private:
		template <class T>
		static void register_event()
		{
			RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<T>(GetSingleton());
			logger::info("Registered {} handler"sv, typeid(T).name());
		}
	};

	class StoryEventHandler final :
		public ISingleton<StoryEventHandler>,
		public RE::BSTEventSink<RE::ActorKill::Event>,
		public RE::BSTEventSink<RE::CriticalHit::Event>,
		public RE::BSTEventSink<RE::DisarmedEvent::Event>,
		public RE::BSTEventSink<RE::DragonSoulsGained::Event>,
		public RE::BSTEventSink<RE::TESHarvestedEvent::ItemHarvested>,
		public RE::BSTEventSink<RE::LevelIncrease::Event>,
		public RE::BSTEventSink<RE::LocationDiscovery::Event>,
		public RE::BSTEventSink<RE::ShoutAttack::Event>,
		public RE::BSTEventSink<RE::SkillIncrease::Event>,
		public RE::BSTEventSink<RE::SoulsTrapped::Event>,
		public RE::BSTEventSink<RE::SpellsLearned::Event>
	{
	public:
//weird compile error with template function (it worked perfectly fine for ages???)
#define register_story_event(T)                                           \
	{                                                                     \
		T::GetEventSource()->AddEventSink<T::Event>(GetSingleton());      \
		logger::info("Registered {} handler"sv, typeid(T::Event).name()); \
	}

		static void Register()
		{
			logger::info("{:*^30}", "STORY EVENTS"sv);

			register_story_event(RE::ActorKill);
			register_story_event(RE::CriticalHit);
			register_story_event(RE::DisarmedEvent);
			register_story_event(RE::DragonSoulsGained);

			RE::TESHarvestedEvent::GetEventSource()->AddEventSink<RE::TESHarvestedEvent::ItemHarvested>(GetSingleton());
			logger::info("Registered {} handler"sv, typeid(RE::TESHarvestedEvent::ItemHarvested).name());

			register_story_event(RE::LevelIncrease);
			register_story_event(RE::LocationDiscovery);
			register_story_event(RE::ShoutAttack);
			register_story_event(RE::SkillIncrease);
			register_story_event(RE::SoulsTrapped);
			register_story_event(RE::SpellsLearned);
		}
#undef register_story_event

		EventResult ProcessEvent(const RE::ActorKill::Event* a_event, RE::BSTEventSource<RE::ActorKill::Event>*) override;
		EventResult ProcessEvent(const RE::CriticalHit::Event* a_event, RE::BSTEventSource<RE::CriticalHit::Event>*) override;
		EventResult ProcessEvent(const RE::DisarmedEvent::Event* a_event, RE::BSTEventSource<RE::DisarmedEvent::Event>*) override;
		EventResult ProcessEvent(const RE::DragonSoulsGained::Event* a_event, RE::BSTEventSource<RE::DragonSoulsGained::Event>*) override;
		EventResult ProcessEvent(const RE::TESHarvestedEvent::ItemHarvested* a_event, RE::BSTEventSource<RE::TESHarvestedEvent::ItemHarvested>*) override;
		EventResult ProcessEvent(const RE::LevelIncrease::Event* a_event, RE::BSTEventSource<RE::LevelIncrease::Event>*) override;
		EventResult ProcessEvent(const RE::LocationDiscovery::Event* a_event, RE::BSTEventSource<RE::LocationDiscovery::Event>*) override;
		EventResult ProcessEvent(const RE::ShoutAttack::Event* a_event, RE::BSTEventSource<RE::ShoutAttack::Event>*) override;
		EventResult ProcessEvent(const RE::SkillIncrease::Event* a_event, RE::BSTEventSource<RE::SkillIncrease::Event>*) override;
		EventResult ProcessEvent(const RE::SoulsTrapped::Event* a_event, RE::BSTEventSource<RE::SoulsTrapped::Event>*) override;
		EventResult ProcessEvent(const RE::SpellsLearned::Event* a_event, RE::BSTEventSource<RE::SpellsLearned::Event>*) override;
	};

	void Register();
}
