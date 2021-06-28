#pragma once

namespace ScriptEvents
{
	using EventResult = RE::BSEventNotifyControl;
	
	class CellFullyLoadedEventHandler : public RE::BSTEventSink<RE::TESCellFullyLoadedEvent>
	{
	public:
		static CellFullyLoadedEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::TESCellFullyLoadedEvent* a_event, RE::BSTEventSource<RE::TESCellFullyLoadedEvent>*) override;

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
		static QuestStartStopEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::TESQuestStartStopEvent* a_event, RE::BSTEventSource<RE::TESQuestStartStopEvent>*) override;

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
		static QuestStageEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) override;

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
		static ObjectLoadedEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::TESObjectLoadedEvent* a_event, RE::BSTEventSource<RE::TESObjectLoadedEvent>*) override;

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
		static GrabReleaseEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::TESGrabReleaseEvent* a_event, RE::BSTEventSource<RE::TESGrabReleaseEvent>*) override;

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
	using EventResult = RE::BSEventNotifyControl;
	
	class ActorKillEventHandler : public RE::BSTEventSink<RE::ActorKill::Event>
	{
	public:
		static ActorKillEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::ActorKill::Event* a_event, RE::BSTEventSource<RE::ActorKill::Event>*) override;

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
		static BooksReadEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::BooksRead::Event* a_event, RE::BSTEventSource<RE::BooksRead::Event>*) override;

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
		static CriticalHitEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::CriticalHit::Event* a_event, RE::BSTEventSource<RE::CriticalHit::Event>*) override;

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
		static DisarmedEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::DisarmedEvent::Event* a_event, RE::BSTEventSource<RE::DisarmedEvent::Event>*) override;

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
		static DragonSoulsGainedEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::DragonSoulsGained::Event* a_event, RE::BSTEventSource<RE::DragonSoulsGained::Event>*) override;

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
		static ItemHarvestedEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::TESHarvestedEvent::ItemHarvested* a_event, RE::BSTEventSource<RE::TESHarvestedEvent::ItemHarvested>*) override;

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
		static LevelIncreaseEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::LevelIncrease::Event* a_event, RE::BSTEventSource<RE::LevelIncrease::Event>*) override;

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
		static LocationDiscoveryEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::LocationDiscovery::Event* a_event, RE::BSTEventSource<RE::LocationDiscovery::Event>*) override;

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
		static ShoutAttackEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::ShoutAttack::Event* a_event, RE::BSTEventSource<RE::ShoutAttack::Event>*) override;

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
		static SkillIncreaseEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::SkillIncrease::Event* a_event, RE::BSTEventSource<RE::SkillIncrease::Event>*) override;

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
		static SoulsTrappedEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::SoulsTrapped::Event* a_event, RE::BSTEventSource<RE::SoulsTrapped::Event>*) override;

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
		static SpellsLearnedEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(const RE::SpellsLearned::Event* a_event, RE::BSTEventSource<RE::SpellsLearned::Event>*) override;

	private:
		SpellsLearnedEventHandler() = default;
		SpellsLearnedEventHandler(const SpellsLearnedEventHandler&) = delete;
		SpellsLearnedEventHandler(SpellsLearnedEventHandler&&) = delete;
		virtual ~SpellsLearnedEventHandler() = default;

		SpellsLearnedEventHandler& operator=(const SpellsLearnedEventHandler&) = delete;
		SpellsLearnedEventHandler& operator=(SpellsLearnedEventHandler&&) = delete;
	};
}
