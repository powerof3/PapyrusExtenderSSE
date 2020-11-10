#pragma once


namespace ScriptEvents
{
	class CellFullyLoadedEventHandler : public RE::BSTEventSink<RE::TESCellFullyLoadedEvent>
	{
	public:
		using EventResult = RE::BSEventNotifyControl;

		static CellFullyLoadedEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::TESCellFullyLoadedEvent* a_event, RE::BSTEventSource<RE::TESCellFullyLoadedEvent>* a_dispatcher) override;

	private:
		CellFullyLoadedEventHandler() = default;
		CellFullyLoadedEventHandler(const CellFullyLoadedEventHandler&) = delete;
		CellFullyLoadedEventHandler(CellFullyLoadedEventHandler&&) = delete;
		virtual ~CellFullyLoadedEventHandler() = default;

		CellFullyLoadedEventHandler& operator=(const CellFullyLoadedEventHandler&) = delete;
		CellFullyLoadedEventHandler& operator=(CellFullyLoadedEventHandler&&) = delete;
	};


	class QuestStartStopEventHandler : public RE::BSTEventSink<RE::TESQuestStartStopEvent>
	{
	public:
		using EventResult = RE::BSEventNotifyControl;

		static QuestStartStopEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::TESQuestStartStopEvent* a_event, RE::BSTEventSource<RE::TESQuestStartStopEvent>* a_dispatcher) override;

	private:
		QuestStartStopEventHandler() = default;
		QuestStartStopEventHandler(const QuestStartStopEventHandler&) = delete;
		QuestStartStopEventHandler(QuestStartStopEventHandler&&) = delete;
		virtual ~QuestStartStopEventHandler() = default;

		QuestStartStopEventHandler& operator=(const QuestStartStopEventHandler&) = delete;
		QuestStartStopEventHandler& operator=(QuestStartStopEventHandler&&) = delete;
	};


	class QuestStageEventHandler : public RE::BSTEventSink<RE::TESQuestStageEvent>
	{
	public:
		using EventResult = RE::BSEventNotifyControl;

		static QuestStageEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>* a_dispatcher) override;

	private:
		QuestStageEventHandler() = default;
		QuestStageEventHandler(const QuestStageEventHandler&) = delete;
		QuestStageEventHandler(QuestStageEventHandler&&) = delete;
		virtual ~QuestStageEventHandler() = default;

		QuestStageEventHandler& operator=(const QuestStageEventHandler&) = delete;
		QuestStageEventHandler& operator=(QuestStageEventHandler&&) = delete;
	};


	class ObjectLoadedEventHandler : public RE::BSTEventSink<RE::TESObjectLoadedEvent>
	{
	public:
		using EventResult = RE::BSEventNotifyControl;

		static ObjectLoadedEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::TESObjectLoadedEvent* a_event, RE::BSTEventSource<RE::TESObjectLoadedEvent>* a_dispatcher) override;

	private:
		ObjectLoadedEventHandler() = default;
		ObjectLoadedEventHandler(const ObjectLoadedEventHandler&) = delete;
		ObjectLoadedEventHandler(ObjectLoadedEventHandler&&) = delete;
		virtual ~ObjectLoadedEventHandler() = default;

		ObjectLoadedEventHandler& operator=(const ObjectLoadedEventHandler&) = delete;
		ObjectLoadedEventHandler& operator=(ObjectLoadedEventHandler&&) = delete;
	};


	class GrabReleaseEventHandler : public RE::BSTEventSink<RE::TESGrabReleaseEvent>
	{
	public:
		using EventResult = RE::BSEventNotifyControl;

		static GrabReleaseEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::TESGrabReleaseEvent* a_event, RE::BSTEventSource<RE::TESGrabReleaseEvent>* a_dispatcher) override;

	private:
		GrabReleaseEventHandler() = default;
		GrabReleaseEventHandler(const GrabReleaseEventHandler&) = delete;
		GrabReleaseEventHandler(GrabReleaseEventHandler&&) = delete;
		virtual ~GrabReleaseEventHandler() = default;

		GrabReleaseEventHandler& operator=(const GrabReleaseEventHandler&) = delete;
		GrabReleaseEventHandler& operator=(GrabReleaseEventHandler&&) = delete;
	};
}


namespace StoryEvents
{
	class ActorKillEventHandler : public RE::BSTEventSink<RE::ActorKill::Event>
	{
	public:
		using EventResult = RE::BSEventNotifyControl;

		static ActorKillEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::ActorKill::Event* a_event, RE::BSTEventSource<RE::ActorKill::Event>* a_dispatcher) override;

	private:
		ActorKillEventHandler() = default;
		ActorKillEventHandler(const ActorKillEventHandler&) = delete;
		ActorKillEventHandler(ActorKillEventHandler&&) = delete;
		virtual ~ActorKillEventHandler() = default;

		ActorKillEventHandler& operator=(const ActorKillEventHandler&) = delete;
		ActorKillEventHandler& operator=(ActorKillEventHandler&&) = delete;
	};


	class BooksReadEventHandler : public RE::BSTEventSink<RE::BooksRead::Event>
	{
	public:
		using EventResult = RE::BSEventNotifyControl;

		static BooksReadEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::BooksRead::Event* a_event, RE::BSTEventSource<RE::BooksRead::Event>* a_dispatcher) override;

	private:
		BooksReadEventHandler() = default;
		BooksReadEventHandler(const BooksReadEventHandler&) = delete;
		BooksReadEventHandler(BooksReadEventHandler&&) = delete;
		virtual ~BooksReadEventHandler() = default;

		BooksReadEventHandler& operator=(const BooksReadEventHandler&) = delete;
		BooksReadEventHandler& operator=(BooksReadEventHandler&&) = delete;
	};


	class CriticalHitEventHandler : public RE::BSTEventSink<RE::CriticalHit::Event>
	{
	public:
		using EventResult = RE::BSEventNotifyControl;

		static CriticalHitEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::CriticalHit::Event* a_event, RE::BSTEventSource<RE::CriticalHit::Event>* a_dispatcher) override;

	private:
		CriticalHitEventHandler() = default;
		CriticalHitEventHandler(const CriticalHitEventHandler&) = delete;
		CriticalHitEventHandler(CriticalHitEventHandler&&) = delete;
		virtual ~CriticalHitEventHandler() = default;

		CriticalHitEventHandler& operator=(const CriticalHitEventHandler&) = delete;
		CriticalHitEventHandler& operator=(CriticalHitEventHandler&&) = delete;
	};


	class DisarmedEventHandler : public RE::BSTEventSink<RE::DisarmedEvent::Event>
	{
	public:
		using EventResult = RE::BSEventNotifyControl;

		static DisarmedEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::DisarmedEvent::Event* a_event, RE::BSTEventSource<RE::DisarmedEvent::Event>* a_dispatcher) override;

	private:
		DisarmedEventHandler() = default;
		DisarmedEventHandler(const DisarmedEventHandler&) = delete;
		DisarmedEventHandler(DisarmedEventHandler&&) = delete;
		virtual ~DisarmedEventHandler() = default;

		DisarmedEventHandler& operator=(const DisarmedEventHandler&) = delete;
		DisarmedEventHandler& operator=(DisarmedEventHandler&&) = delete;
	};


	class DragonSoulsGainedEventHandler : public RE::BSTEventSink<RE::DragonSoulsGained::Event>
	{
	public:
		using EventResult = RE::BSEventNotifyControl;

		static DragonSoulsGainedEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::DragonSoulsGained::Event* a_event, RE::BSTEventSource<RE::DragonSoulsGained::Event>* a_dispatcher) override;

	private:
		DragonSoulsGainedEventHandler() = default;
		DragonSoulsGainedEventHandler(const DragonSoulsGainedEventHandler&) = delete;
		DragonSoulsGainedEventHandler(DragonSoulsGainedEventHandler&&) = delete;
		virtual ~DragonSoulsGainedEventHandler() = default;

		DragonSoulsGainedEventHandler& operator=(const DragonSoulsGainedEventHandler&) = delete;
		DragonSoulsGainedEventHandler& operator=(DragonSoulsGainedEventHandler&&) = delete;
	};


	class ItemHarvestedEventHandler : public RE::BSTEventSink<RE::TESHarvestedEvent::ItemHarvested>
	{
	public:
		using EventResult = RE::BSEventNotifyControl;

		static ItemHarvestedEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::TESHarvestedEvent::ItemHarvested* a_event, RE::BSTEventSource<RE::TESHarvestedEvent::ItemHarvested>* a_dispatcher) override;

	private:
		ItemHarvestedEventHandler() = default;
		ItemHarvestedEventHandler(const ItemHarvestedEventHandler&) = delete;
		ItemHarvestedEventHandler(ItemHarvestedEventHandler&&) = delete;
		virtual ~ItemHarvestedEventHandler() = default;

		ItemHarvestedEventHandler& operator=(const ItemHarvestedEventHandler&) = delete;
		ItemHarvestedEventHandler& operator=(ItemHarvestedEventHandler&&) = delete;
	};


	class LevelIncreaseEventHandler : public RE::BSTEventSink<RE::LevelIncrease::Event>
	{
	public:
		using EventResult = RE::BSEventNotifyControl;

		static LevelIncreaseEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::LevelIncrease::Event* a_event, RE::BSTEventSource<RE::LevelIncrease::Event>* a_dispatcher) override;

	private:
		LevelIncreaseEventHandler() = default;
		LevelIncreaseEventHandler(const LevelIncreaseEventHandler&) = delete;
		LevelIncreaseEventHandler(LevelIncreaseEventHandler&&) = delete;
		virtual ~LevelIncreaseEventHandler() = default;

		LevelIncreaseEventHandler& operator=(const LevelIncreaseEventHandler&) = delete;
		LevelIncreaseEventHandler& operator=(LevelIncreaseEventHandler&&) = delete;
	};


	class LocationDiscoveryEventHandler : public RE::BSTEventSink<RE::LocationDiscovery::Event>
	{
	public:
		using EventResult = RE::BSEventNotifyControl;

		static LocationDiscoveryEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::LocationDiscovery::Event* a_event, RE::BSTEventSource<RE::LocationDiscovery::Event>* a_dispatcher) override;

	private:
		LocationDiscoveryEventHandler() = default;
		LocationDiscoveryEventHandler(const LocationDiscoveryEventHandler&) = delete;
		LocationDiscoveryEventHandler(LocationDiscoveryEventHandler&&) = delete;
		virtual ~LocationDiscoveryEventHandler() = default;

		LocationDiscoveryEventHandler& operator=(const LocationDiscoveryEventHandler&) = delete;
		LocationDiscoveryEventHandler& operator=(LocationDiscoveryEventHandler&&) = delete;
	};


	class ShoutAttackEventHandler : public RE::BSTEventSink<RE::ShoutAttack::Event>
	{
	public:
		using EventResult = RE::BSEventNotifyControl;

		static ShoutAttackEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::ShoutAttack::Event* a_event, RE::BSTEventSource<RE::ShoutAttack::Event>* a_dispatcher) override;

	private:
		ShoutAttackEventHandler() = default;
		ShoutAttackEventHandler(const ShoutAttackEventHandler&) = delete;
		ShoutAttackEventHandler(ShoutAttackEventHandler&&) = delete;
		virtual ~ShoutAttackEventHandler() = default;

		ShoutAttackEventHandler& operator=(const ShoutAttackEventHandler&) = delete;
		ShoutAttackEventHandler& operator=(ShoutAttackEventHandler&&) = delete;
	};


	class SkillIncreaseEventHandler : public RE::BSTEventSink<RE::SkillIncrease::Event>
	{
	public:
		using EventResult = RE::BSEventNotifyControl;

		static SkillIncreaseEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::SkillIncrease::Event* a_event, RE::BSTEventSource<RE::SkillIncrease::Event>* a_dispatcher) override;

	private:
		SkillIncreaseEventHandler() = default;
		SkillIncreaseEventHandler(const SkillIncreaseEventHandler&) = delete;
		SkillIncreaseEventHandler(SkillIncreaseEventHandler&&) = delete;
		virtual ~SkillIncreaseEventHandler() = default;

		SkillIncreaseEventHandler& operator=(const SkillIncreaseEventHandler&) = delete;
		SkillIncreaseEventHandler& operator=(SkillIncreaseEventHandler&&) = delete;
	};


	class SoulsTrappedEventHandler : public RE::BSTEventSink<RE::SoulsTrapped::Event>
	{
	public:
		using EventResult = RE::BSEventNotifyControl;

		static SoulsTrappedEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::SoulsTrapped::Event* a_event, RE::BSTEventSource<RE::SoulsTrapped::Event>* a_dispatcher) override;

	private:
		SoulsTrappedEventHandler() = default;
		SoulsTrappedEventHandler(const SoulsTrappedEventHandler&) = delete;
		SoulsTrappedEventHandler(SoulsTrappedEventHandler&&) = delete;
		virtual ~SoulsTrappedEventHandler() = default;

		SoulsTrappedEventHandler& operator=(const SoulsTrappedEventHandler&) = delete;
		SoulsTrappedEventHandler& operator=(SoulsTrappedEventHandler&&) = delete;
	};


	class SpellsLearnedEventHandler : public RE::BSTEventSink<RE::SpellsLearned::Event>
	{
	public:
		using EventResult = RE::BSEventNotifyControl;

		static SpellsLearnedEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::SpellsLearned::Event* a_event, RE::BSTEventSource<RE::SpellsLearned::Event>* a_dispatcher) override;

	private:
		SpellsLearnedEventHandler() = default;
		SpellsLearnedEventHandler(const SpellsLearnedEventHandler&) = delete;
		SpellsLearnedEventHandler(SpellsLearnedEventHandler&&) = delete;
		virtual ~SpellsLearnedEventHandler() = default;

		SpellsLearnedEventHandler& operator=(const SpellsLearnedEventHandler&) = delete;
		SpellsLearnedEventHandler& operator=(SpellsLearnedEventHandler&&) = delete;
	};
}