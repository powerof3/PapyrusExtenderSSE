#pragma once

namespace Event
{
	class ScriptEventHolder
	{
	public:
		[[nodiscard]] static ScriptEventHolder* GetSingleton()
		{
			static ScriptEventHolder singleton;
			return &singleton;
		}

		enum : std::uint32_t
		{
			kCellFullyLoaded = 'CELL',
			kQuestStart = 'QSTR',
			kQuestStop = 'QSTP',
			kQuestStage = 'QSTG',
			kObjectLoaded = 'LOAD',
			kObjectUnloaded = 'UNLD',
			kObjectGrab = 'GRAB',
			kObjectRelease = 'RELS',
		};

		SKSE::RegistrationSet<const RE::TESObjectCELL*> cellFullyLoaded{ "OnCellFullyLoaded"sv };
		SKSE::RegistrationMap<const RE::TESQuest*> questStart{ "OnQuestStart"sv };
		SKSE::RegistrationMap<const RE::TESQuest*> questStop{ "OnQuestStop"sv };
		SKSE::RegistrationMap<const RE::TESQuest*, std::uint32_t> questStage{ "OnQuestStageChange"sv };
		SKSE::RegistrationMap<const RE::TESObjectREFR*, RE::FormType> objectLoaded{ "OnObjectLoaded"sv };
		SKSE::RegistrationMap<const RE::TESObjectREFR*, RE::FormType> objectUnloaded{ "OnObjectUnloaded"sv };
		SKSE::RegistrationSet<const RE::TESObjectREFR*> objectGrab{ "OnObjectGrab"sv };
		SKSE::RegistrationSet<const RE::TESObjectREFR*> objectRelease{ "OnObjectRelease"sv };

		void Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_version);
		void Load(SKSE::SerializationInterface* a_intfc, std::uint32_t a_type);
		void Revert(SKSE::SerializationInterface* a_intfc);
		void FormDelete(RE::VMHandle a_handle);

	private:
		ScriptEventHolder() = default;
		ScriptEventHolder(const ScriptEventHolder&) = delete;
		ScriptEventHolder(ScriptEventHolder&&) = delete;
		~ScriptEventHolder() = default;

		ScriptEventHolder& operator=(const ScriptEventHolder&) = delete;
		ScriptEventHolder& operator=(ScriptEventHolder&&) = delete;
	};

	class StoryEventHolder
	{
	public:
		static StoryEventHolder* GetSingleton()
		{
			static StoryEventHolder singleton;
			return &singleton;
		}

		enum : std::uint32_t
		{
			kActorKill = 'KILL',
			kBookRead = 'BOOK',
			kCritHit = 'CRIT',
			kDisarm = 'DSRM',
			kDragonSoul = 'DRGN',
			kHarvest = 'HVST',
			kLevelIncrease = 'LEVL',
			kLocDiscovery = 'DISC',
			kSkillIncrease = 'SKIL',
			kShoutAttack = 'SHOU',
			kSoulTrap = 'SOUL',
			kSpellLearned = 'SPEL'
		};

		SKSE::RegistrationSet<const RE::Actor*, const RE::Actor*> actorKill{ "OnActorKilled"sv };
		SKSE::RegistrationSet<const RE::TESObjectBOOK*> booksRead{ "OnBookRead"sv };
		SKSE::RegistrationSet<const RE::Actor*, const RE::TESObjectWEAP*, bool> criticalHit{ "OnCriticalHit"sv };
		SKSE::RegistrationSet<const RE::Actor*, const RE::Actor*> disarmed{ "OnDisarmed"sv };
		SKSE::RegistrationSet<float> dragonSoulsGained{ "OnDragonSoulsGained"sv };
		SKSE::RegistrationSet<const RE::TESForm*> itemHarvested{ "OnItemHarvested"sv };
		SKSE::RegistrationSet<std::uint32_t> levelIncrease{ "OnLevelIncrease"sv };
		SKSE::RegistrationSet<RE::BSFixedString, RE::BSFixedString> locationDiscovery{ "OnLocationDiscovery"sv };
		SKSE::RegistrationSet<const RE::TESShout*> shoutAttack{ "OnPlayerShoutAttack"sv };
		SKSE::RegistrationSet<std::uint32_t> skillIncrease{ "OnSkillIncrease"sv };
		SKSE::RegistrationSet<const RE::Actor*, const RE::Actor*> soulsTrapped{ "OnSoulTrapped"sv };
		SKSE::RegistrationSet<const RE::SpellItem*> spellsLearned{ "OnSpellLearned"sv };

		void Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_version);
		void Load(SKSE::SerializationInterface* a_intfc, std::uint32_t a_type);
		void Revert(SKSE::SerializationInterface* a_intfc);
		void FormDelete(RE::VMHandle a_handle);

	private:
		StoryEventHolder() = default;
		StoryEventHolder(const StoryEventHolder&) = delete;
		StoryEventHolder(StoryEventHolder&&) = delete;
		~StoryEventHolder() = default;

		StoryEventHolder& operator=(const StoryEventHolder&) = delete;
		StoryEventHolder& operator=(StoryEventHolder&&) = delete;
	};

	class GameEventHolder
	{
	public:
		static GameEventHolder* GetSingleton()
		{
			static GameEventHolder singleton;
			return &singleton;
		}

		enum : std::uint32_t
		{
			kActorFallLongDistance = 'FALL',
			kActorResurrect = 'RSUR',
			kActorReanimateStart = 'RSTA',
			kActorReanimateStop = 'REND',
			kItemCrafted = 'ITEM',
			kWeatherChange = 'WEAT',
			kMagicEffectApply = 'MGEF',
			kWeaponHit = 'WHIT',
			kMagicHit = 'MHIT',
			kProjectileHit = 'PHIT'
		};

		SKSE::RegistrationSetUnique<const RE::Actor*, float, float> actorFallLongDistance{ "OnActorFallLongDistance"sv };
		SKSE::RegistrationSetUnique<const RE::Actor*, bool> actorResurrect{ "OnActorResurrected"sv };
		SKSE::RegistrationSetUnique<const RE::Actor*, const RE::Actor*> actorReanimateStart{ "OnActorReanimateStart"sv };
		SKSE::RegistrationSetUnique<const RE::Actor*, const RE::Actor*> actorReanimateStop{ "OnActorReanimateStop"sv };
		SKSE::RegistrationSet<const RE::TESObjectREFR*, const RE::BGSLocation*, const RE::TESForm*> itemCrafted{ "OnItemCrafted"sv };
		SKSE::RegistrationMapUnique<const RE::TESObjectREFR*, const RE::EffectSetting*, const RE::TESForm*, bool> magicApply{ "OnMagicEffectApplyEx"sv };
		SKSE::RegistrationSetUnique<const RE::TESObjectREFR*, const RE::TESForm*, const RE::BGSProjectile*> magicHit{ "OnMagicHit"sv };
		SKSE::RegistrationSetUnique<const RE::TESObjectREFR*, const RE::TESForm*, const RE::BGSProjectile*> projectileHit{ "OnProjectileHit"sv };
		SKSE::RegistrationSetUnique<const RE::TESObjectREFR*, const RE::TESForm*, const RE::BGSProjectile*, std::uint32_t> weaponHit{ "OnWeaponHit"sv };
		SKSE::RegistrationSet<const RE::TESWeather*, const RE::TESWeather*> weatherChange{ "OnWeatherChange"sv };

		void Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_version);
		void Load(SKSE::SerializationInterface* a_intfc, std::uint32_t a_type);
		void Revert(SKSE::SerializationInterface* a_intfc);
		void FormDelete(RE::VMHandle a_handle);

	private:
		GameEventHolder() = default;
		GameEventHolder(const GameEventHolder&) = delete;
		GameEventHolder(GameEventHolder&&) = delete;
		~GameEventHolder() = default;

		GameEventHolder& operator=(const GameEventHolder&) = delete;
		GameEventHolder& operator=(GameEventHolder&&) = delete;
	};
}
