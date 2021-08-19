#pragma once

using EventResult = RE::BSEventNotifyControl;

namespace Events
{
	namespace Script
	{
		class EventHandler final :
			public RE::BSTEventSink<RE::TESCellFullyLoadedEvent>,
			public RE::BSTEventSink<RE::TESQuestStartStopEvent>,
			public RE::BSTEventSink<RE::TESQuestStageEvent>,
			public RE::BSTEventSink<RE::TESObjectLoadedEvent>,
			public RE::BSTEventSink<RE::TESGrabReleaseEvent>
		{
		public:
			[[nodiscard]] static EventHandler* GetSingleton()
			{
				static EventHandler singleton;
				return &singleton;
			}

			static void Register()
			{
				logger::info("{:*^30}", "SCRIPT EVENTS"sv);

				auto scripts = RE::ScriptEventSourceHolder::GetSingleton();
				if (scripts) {
					scripts->AddEventSink<RE::TESCellFullyLoadedEvent>(GetSingleton());
					logger::info("Registered {} handler"sv, typeid(RE::TESCellFullyLoadedEvent).name());

					scripts->AddEventSink<RE::TESQuestStartStopEvent>(GetSingleton());
					logger::info("Registered {} handler"sv, typeid(RE::TESQuestStartStopEvent).name());

					scripts->AddEventSink<RE::TESQuestStageEvent>(GetSingleton());
					logger::info("Registered {} handler"sv, typeid(RE::TESQuestStageEvent).name());

					scripts->AddEventSink<RE::TESObjectLoadedEvent>(GetSingleton());
					logger::info("Registered {} handler"sv, typeid(RE::TESObjectLoadedEvent).name());

					scripts->AddEventSink<RE::TESGrabReleaseEvent>(GetSingleton());
					logger::info("Registered {} handler"sv, typeid(RE::TESGrabReleaseEvent).name());
				}
			}

			EventResult ProcessEvent(const RE::TESCellFullyLoadedEvent* a_event, RE::BSTEventSource<RE::TESCellFullyLoadedEvent>*) override;
			EventResult ProcessEvent(const RE::TESQuestStartStopEvent* a_event, RE::BSTEventSource<RE::TESQuestStartStopEvent>*) override;
			EventResult ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) override;
			EventResult ProcessEvent(const RE::TESObjectLoadedEvent* a_event, RE::BSTEventSource<RE::TESObjectLoadedEvent>*) override;
			EventResult ProcessEvent(const RE::TESGrabReleaseEvent* a_event, RE::BSTEventSource<RE::TESGrabReleaseEvent>*) override;

		private:
			EventHandler() = default;
			EventHandler(const EventHandler&) = delete;
			EventHandler(EventHandler&&) = delete;

			~EventHandler() = default;

			EventHandler& operator=(const EventHandler&) = delete;
			EventHandler& operator=(EventHandler&&) = delete;
		};
	}

	namespace Story
	{
		class EventHandler final :
			public RE::BSTEventSink<RE::ActorKill::Event>,
			public RE::BSTEventSink<RE::BooksRead::Event>,
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
			[[nodiscard]] static EventHandler* GetSingleton()
			{
				static EventHandler singleton;
				return &singleton;
			}

			static void Register()
			{
				logger::info("{:*^30}", "STORY EVENTS"sv);

				auto actorKill = RE::ActorKill::GetEventSource();
				if (actorKill) {
					actorKill->AddEventSink<RE::ActorKill::Event>(GetSingleton());
					logger::info("Registered {} handler"sv, typeid(RE::ActorKill::Event).name());
				}

				auto books = RE::BooksRead::GetEventSource();
				if (books) {
					books->AddEventSink<RE::BooksRead::Event>(GetSingleton());
					logger::info("Registered {} handler"sv, typeid(RE::BooksRead::Event).name());
				}

				auto critHit = RE::CriticalHit::GetEventSource();
				if (critHit) {
					critHit->AddEventSink<RE::CriticalHit::Event>(GetSingleton());
					logger::info("Registered {} handler"sv, typeid(RE::CriticalHit::Event).name());
				}

				auto disarmed = RE::DisarmedEvent::GetEventSource();
				if (disarmed) {
					disarmed->AddEventSink<RE::DisarmedEvent::Event>(GetSingleton());
					logger::info("Registered {} handler"sv, typeid(RE::DisarmedEvent::Event).name());
				}

				auto dragonSoulGained = RE::DragonSoulsGained::GetEventSource();
				if (dragonSoulGained) {
					dragonSoulGained->AddEventSink<RE::DragonSoulsGained::Event>(GetSingleton());
					logger::info("Registered {} handler"sv, typeid(RE::DragonSoulsGained::Event).name());
				}

				auto itemHarvest = RE::TESHarvestedEvent::GetEventSource();
				if (itemHarvest) {
					itemHarvest->AddEventSink<RE::TESHarvestedEvent::ItemHarvested>(GetSingleton());
					logger::info("Registered {} handler"sv, typeid(RE::TESHarvestedEvent::ItemHarvested).name());
				}

				auto levelIncrease = RE::LevelIncrease::GetEventSource();
				if (levelIncrease) {
					levelIncrease->AddEventSink<RE::LevelIncrease::Event>(GetSingleton());
					logger::info("Registered {} handler"sv, typeid(RE::LevelIncrease::Event).name());
				}

				auto locDiscovery = RE::LocationDiscovery::GetEventSource();
				if (locDiscovery) {
					locDiscovery->AddEventSink<RE::LocationDiscovery::Event>(GetSingleton());
					logger::info("Registered {} handler"sv, typeid(RE::LocationDiscovery::Event).name());
				}

				auto shoutAttack = RE::ShoutAttack::GetEventSource();
				if (shoutAttack) {
					shoutAttack->AddEventSink<RE::ShoutAttack::Event>(GetSingleton());
					logger::info("Registered {} handler"sv, typeid(RE::ShoutAttack::Event).name());
				}

				auto skillIncrease = RE::SkillIncrease::GetEventSource();
				if (skillIncrease) {
					skillIncrease->AddEventSink<RE::SkillIncrease::Event>(GetSingleton());
					logger::info("Registered {} handler"sv, typeid(RE::ActorKill::Event).name());
				}

				auto soulsTrapped = RE::SoulsTrapped::GetEventSource();
				if (soulsTrapped) {
					soulsTrapped->AddEventSink<RE::SoulsTrapped::Event>(GetSingleton());
					logger::info("Registered {} handler"sv, typeid(RE::SoulsTrapped::Event).name());
				}

				auto spellsLearned = RE::SpellsLearned::GetEventSource();
				if (spellsLearned) {
					spellsLearned->AddEventSink<RE::SpellsLearned::Event>(GetSingleton());
					logger::info("Registered {} handler"sv, typeid(RE::SpellsLearned::Event).name());
				}
			}

			EventResult ProcessEvent(const RE::ActorKill::Event* a_event, RE::BSTEventSource<RE::ActorKill::Event>*) override;
			EventResult ProcessEvent(const RE::BooksRead::Event* a_event, RE::BSTEventSource<RE::BooksRead::Event>*) override;
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

		private:
			EventHandler() = default;
			EventHandler(const EventHandler&) = delete;
			EventHandler(EventHandler&&) = delete;

			~EventHandler() = default;

			EventHandler& operator=(const EventHandler&) = delete;
			EventHandler& operator=(EventHandler&&) = delete;
		};
	}

	namespace Game
	{
		bool Install();
	}
}
