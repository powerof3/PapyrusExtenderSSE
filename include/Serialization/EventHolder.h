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

		SKSE::RegistrationMap<RE::FormID, const RE::TESQuest*> questStart{ "OnQuestStart"sv };
		SKSE::RegistrationMap<RE::FormID, const RE::TESQuest*> questStop{ "OnQuestStop"sv };

		SKSE::RegistrationMap<RE::FormID, const RE::TESQuest*, std::uint32_t> questStage{ "OnQuestStageChange"sv };

		SKSE::RegistrationMap<RE::FormType, const RE::TESObjectREFR*, RE::FormType> objectLoaded{ "OnObjectLoaded"sv };
		SKSE::RegistrationMap<RE::FormType, const RE::TESObjectREFR*, RE::FormType> objectUnloaded{ "OnObjectUnloaded"sv };

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

	namespace Filter
	{
		struct detail
		{
			static bool passes_simple_filter(RE::TESForm* a_form, RE::TESForm* a_formFilter)
			{
				if (!a_formFilter) {
					return true;
				}

				switch (a_formFilter->GetFormType()) {
				case RE::FormType::Keyword:
					{
						if (const auto keyword = a_formFilter->As<RE::BGSKeyword>(); keyword) {
							if (auto keywordForm = a_form->As<RE::BGSKeywordForm>(); keywordForm) {
								return keywordForm->HasKeyword(keyword);
							}
						}
						return false;
					}
				case RE::FormType::FormList:
					{
						if (const auto list = a_formFilter->As<RE::BGSListForm>(); list) {
							if (list->ContainsOnlyType(RE::FormType::Keyword)) {
								return a_form->HasKeywordInList(list, false);
							}
							return list->HasForm(a_form);
						} else {
							return list->HasForm(a_form);
						}
					}
				default:
					return a_form == a_formFilter;
				}
			}

			static bool passes_ref_filter(RE::TESObjectREFR* a_ref, RE::TESForm* a_refFilter)
			{
				if (!a_refFilter) {
					return true;
				}

				const auto actor = a_ref->As<RE::Actor>();

				switch (a_refFilter->GetFormType()) {
				case RE::FormType::Reference:
				case RE::FormType::ActorCharacter:
					{
						return a_ref == a_refFilter;
					}
				case RE::FormType::Keyword:
					{
						if (const auto keyword = a_refFilter->As<RE::BGSKeyword>(); keyword) {
							return a_ref->HasKeyword(keyword);
						}
						return false;
					}
				case RE::FormType::Faction:
					{
						if (const auto faction = a_refFilter->As<RE::TESFaction>(); faction) {
							return actor && actor->IsInFaction(faction);
						}
						return false;
					}
				case RE::FormType::Race:
					{
						if (const auto race = a_refFilter->As<RE::TESRace>(); race) {
							return actor && actor->GetRace() == race;
						}
						return false;
					}
				case RE::FormType::FormList:
					{
						if (const auto list = a_refFilter->As<RE::BGSListForm>(); list) {
							if (list->ContainsOnlyType(RE::FormType::Keyword)) {
								return a_ref->HasKeywordInList(list, false);
							} else {
								bool result = false;
								list->ForEachForm([&](RE::TESForm& a_form) {
									if (result = passes_ref_filter(a_ref, &a_form); result == true) {
										return RE::BSContainer::ForEachResult::kStop;
									}
									return RE::BSContainer::ForEachResult::kContinue;
								});
								return result;
							}
						}
						return false;
					}
				case RE::FormType::NPC:
					{
						if (actor) {
							RE::TESNPC* actorbase = actor->GetActorBase();
							if (const auto xLvlBase = actor->extraList.GetByType<RE::ExtraLeveledCreature>(); xLvlBase) {
								actorbase = skyrim_cast<RE::TESNPC*>(xLvlBase->originalBase);
							}
							return actorbase && actorbase == a_refFilter;
						}
						return false;
					}
				default:
					return a_ref->GetBaseObject() == a_refFilter;
				}
			}

			static bool passes_hit_filter(bool a_hitFlag, std::int32_t a_hitFilter)
			{
				switch (a_hitFilter) {
				case -1:
					return true;
				case 0:
					return !a_hitFlag;
				case 1:
					return a_hitFlag;
				default:
					return false;
				}
			}
		};

		struct MagicEffectApply
		{
			MagicEffectApply() = default;
			MagicEffectApply(RE::TESForm* a_effectFilter) :
				effectID(a_effectFilter ? a_effectFilter->GetFormID() : 0)
			{}

			bool operator<(const MagicEffectApply& a_rhs) const
			{
				return effectID < a_rhs.effectID;
			}

			bool Load(SKSE::SerializationInterface* a_intfc);
			bool Save(SKSE::SerializationInterface* a_intfc) const;
			bool PassesFilter(RE::EffectSetting* a_baseEffect) const;

			RE::FormID effectID{ 0 };
		};

		struct Hit
		{
			Hit() = default;
			Hit(RE::TESForm* a_aggressor, RE::TESForm* a_source, RE::TESForm* a_projectile, std::int32_t powerAttack, std::int32_t sneakAttack, std::int32_t bashAttack, std::int32_t blockAttack) :
				aggressorID(a_aggressor ? a_aggressor->GetFormID() : 0),
				sourceID(a_source ? a_source->GetFormID() : 0),
				projectileID(a_projectile ? a_projectile->GetFormID() : 0),
				powerAttack(powerAttack),
				sneakAttack(sneakAttack),
				bashAttack(bashAttack),
				blockAttack(blockAttack)
			{}

			bool operator<(const Hit& a_rhs) const
			{
				return std::tie(aggressorID, sourceID, projectileID, powerAttack, sneakAttack, bashAttack, blockAttack) <
				       std::tie(a_rhs.aggressorID, a_rhs.sourceID, a_rhs.projectileID, a_rhs.powerAttack, a_rhs.sneakAttack, a_rhs.bashAttack, a_rhs.blockAttack);
			}

			bool Load(SKSE::SerializationInterface* a_intfc);
			bool Save(SKSE::SerializationInterface* a_intfc) const;
			bool PassesFilter(RE::TESObjectREFR* a_aggressor, RE::TESForm* a_source, RE::BGSProjectile* a_projectile, bool a_powerAttack, bool a_sneakAttack, bool a_bashAttack, bool a_blockAttack) const;

			RE::FormID aggressorID{ 0 };
			RE::FormID sourceID{ 0 };
			RE::FormID projectileID{ 0 };
			std::int32_t powerAttack{ -1 };
			std::int32_t sneakAttack{ -1 };
			std::int32_t bashAttack{ -1 };
			std::int32_t blockAttack{ -1 };
		};
	}

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
			kBookRead = 'BOOK',
			kFastTravelPrompt = 'FTPT',
#ifdef SKYRIMVR
			kFastTravelEnd = 'FTED',
#endif
			kItemCrafted = 'ITEM',
			kWeatherChange = 'WEAT',
			kMagicEffectApply = 'MGEF',
			kHit = 'OHIT',
			kWeaponHit = 'WHIT',
			kMagicHit = 'MHIT',
			kProjectileHit = 'PHIT'
		};

		SKSE::RegistrationSetUnique<const RE::Actor*, float, float> actorFallLongDistance{ "OnActorFallLongDistance"sv };
		SKSE::RegistrationSetUnique<const RE::Actor*, bool> actorResurrect{ "OnActorResurrected"sv };
		SKSE::RegistrationSetUnique<const RE::Actor*, const RE::Actor*> actorReanimateStart{ "OnActorReanimateStart"sv };
		SKSE::RegistrationSetUnique<const RE::Actor*, const RE::Actor*> actorReanimateStop{ "OnActorReanimateStop"sv };
		SKSE::RegistrationSet<const RE::TESObjectBOOK*> booksRead{ "OnBookRead"sv };
		SKSE::RegistrationSet<const RE::TESObjectREFR*> fastTravelPrompt{ "OnFastTravelPrompt"sv };
#ifdef SKYRIMVR
		SKSE::RegistrationSet<float> fastTravelEnd{ "OnPlayerFastTravelEnd"sv };
#endif
		SKSE::RegistrationSet<const RE::TESObjectREFR*, const RE::BGSLocation*, const RE::TESForm*> itemCrafted{ "OnItemCrafted"sv };

		SKSE::RegistrationMapUnique<Filter::MagicEffectApply, const RE::TESObjectREFR*, const RE::EffectSetting*, const RE::TESForm*, bool> magicApply{ "OnMagicEffectApplyEx"sv };
		SKSE::RegistrationMapUnique<Filter::Hit, const RE::TESObjectREFR*, const RE::TESForm*, const RE::BGSProjectile*, bool, bool, bool, bool> onHit{ "OnHitEx"sv };

		SKSE::RegistrationSetUnique<const RE::TESObjectREFR*, const RE::TESForm*, const RE::BGSProjectile*> magicHit{ "OnMagicHit"sv };
		SKSE::RegistrationSetUnique<const RE::TESObjectREFR*, const RE::TESForm*, const RE::BGSProjectile*> projectileHit{ "OnProjectileHit"sv };
		SKSE::RegistrationSetUnique<const RE::TESObjectREFR*, const RE::TESForm*, const RE::BGSProjectile*, std::uint32_t> weaponHit{ "OnWeaponHit"sv };
		SKSE::RegistrationSet<const RE::TESWeather*, const RE::TESWeather*> weatherChange{ "OnWeatherChange"sv };

		void Save(SKSE::SerializationInterface* a_intfc, std::uint32_t a_version);
		void Load(SKSE::SerializationInterface* a_intfc, std::uint32_t a_type);
		void Revert(SKSE::SerializationInterface* a_intfc);
		void FormDelete(RE::VMHandle a_handle);
		void FormDelete(RE::FormID a_uniqueID);

	private:
		GameEventHolder() = default;
		GameEventHolder(const GameEventHolder&) = delete;
		GameEventHolder(GameEventHolder&&) = delete;
		~GameEventHolder() = default;

		GameEventHolder& operator=(const GameEventHolder&) = delete;
		GameEventHolder& operator=(GameEventHolder&&) = delete;
	};
}
