#pragma once


namespace Serialization
{
	namespace ScriptEvents
	{
		class OnCellFullyLoadedRegSet : public SKSE::RegistrationSet<const RE::TESObjectCELL*>
		{
		public:
			using Base = SKSE::RegistrationSet<const RE::TESObjectCELL*>;


			static OnCellFullyLoadedRegSet* GetSingleton();

		private:
			OnCellFullyLoadedRegSet();
			OnCellFullyLoadedRegSet(const OnCellFullyLoadedRegSet&) = delete;
			OnCellFullyLoadedRegSet(OnCellFullyLoadedRegSet&&) = delete;
			~OnCellFullyLoadedRegSet() = default;

			OnCellFullyLoadedRegSet& operator=(const OnCellFullyLoadedRegSet&) = delete;
			OnCellFullyLoadedRegSet& operator=(OnCellFullyLoadedRegSet&&) = delete;
		};


		class OnQuestStartRegMap : public SKSE::RegistrationMap<const RE::TESQuest*>
		{
		public:
			using Base = SKSE::RegistrationMap<const RE::TESQuest*>;


			static OnQuestStartRegMap* GetSingleton();

		private:
			OnQuestStartRegMap();
			OnQuestStartRegMap(const OnQuestStartRegMap&) = delete;
			OnQuestStartRegMap(OnQuestStartRegMap&&) = delete;
			~OnQuestStartRegMap() = default;

			OnQuestStartRegMap& operator=(const OnQuestStartRegMap&) = delete;
			OnQuestStartRegMap& operator=(OnQuestStartRegMap&&) = delete;
		};


		class OnQuestStopRegMap : public SKSE::RegistrationMap<const RE::TESQuest*>
		{
		public:
			using Base = SKSE::RegistrationMap<const RE::TESQuest*>;


			static OnQuestStopRegMap* GetSingleton();

		private:
			OnQuestStopRegMap();
			OnQuestStopRegMap(const OnQuestStopRegMap&) = delete;
			OnQuestStopRegMap(OnQuestStopRegMap&&) = delete;
			~OnQuestStopRegMap() = default;

			OnQuestStopRegMap& operator=(const OnQuestStopRegMap&) = delete;
			OnQuestStopRegMap& operator=(OnQuestStopRegMap&&) = delete;
		};


		class OnQuestStageRegMap : public SKSE::RegistrationMap<const RE::TESQuest*, std::uint32_t>
		{
		public:
			using Base = SKSE::RegistrationMap<const RE::TESQuest*, std::uint32_t>;


			static OnQuestStageRegMap* GetSingleton();

		private:
			OnQuestStageRegMap();
			OnQuestStageRegMap(const OnQuestStageRegMap&) = delete;
			OnQuestStageRegMap(OnQuestStageRegMap&&) = delete;
			~OnQuestStageRegMap() = default;

			OnQuestStageRegMap& operator=(const OnQuestStageRegMap&) = delete;
			OnQuestStageRegMap& operator=(OnQuestStageRegMap&&) = delete;
		};


		class OnObjectLoadedRegMap : public SKSE::RegistrationMap<const RE::TESObjectREFR*, RE::FormType>
		{
		public:
			using Base = SKSE::RegistrationMap<const RE::TESObjectREFR*, RE::FormType>;


			static OnObjectLoadedRegMap* GetSingleton();

		private:
			OnObjectLoadedRegMap();
			OnObjectLoadedRegMap(const OnObjectLoadedRegMap&) = delete;
			OnObjectLoadedRegMap(OnObjectLoadedRegMap&&) = delete;
			~OnObjectLoadedRegMap() = default;

			OnObjectLoadedRegMap& operator=(const OnObjectLoadedRegMap&) = delete;
			OnObjectLoadedRegMap& operator=(OnObjectLoadedRegMap&&) = delete;
		};


		class OnObjectUnloadedRegMap : public SKSE::RegistrationMap<const RE::TESObjectREFR*, RE::FormType>
		{
		public:
			using Base = SKSE::RegistrationMap<const RE::TESObjectREFR*, RE::FormType>;


			static OnObjectUnloadedRegMap* GetSingleton();

		private:
			OnObjectUnloadedRegMap();
			OnObjectUnloadedRegMap(const OnObjectUnloadedRegMap&) = delete;
			OnObjectUnloadedRegMap(OnObjectUnloadedRegMap&&) = delete;
			~OnObjectUnloadedRegMap() = default;

			OnObjectUnloadedRegMap& operator=(const OnObjectUnloadedRegMap&) = delete;
			OnObjectUnloadedRegMap& operator=(OnObjectUnloadedRegMap&&) = delete;
		};


		class OnGrabRegSet : public SKSE::RegistrationSet<const RE::TESObjectREFR*>
		{
		public:
			using Base = SKSE::RegistrationSet<const RE::TESObjectREFR*>;


			static OnGrabRegSet* GetSingleton();

		private:
			OnGrabRegSet();
			OnGrabRegSet(const OnGrabRegSet&) = delete;
			OnGrabRegSet(OnGrabRegSet&&) = delete;
			~OnGrabRegSet() = default;

			OnGrabRegSet& operator=(const OnGrabRegSet&) = delete;
			OnGrabRegSet& operator=(OnGrabRegSet&&) = delete;
		};


		class OnReleaseRegSet : public SKSE::RegistrationSet<const RE::TESObjectREFR*>
		{
		public:
			using Base = SKSE::RegistrationSet<const RE::TESObjectREFR*>;


			static OnReleaseRegSet* GetSingleton();

		private:
			OnReleaseRegSet();
			OnReleaseRegSet(const OnReleaseRegSet&) = delete;
			OnReleaseRegSet(OnReleaseRegSet&&) = delete;
			~OnReleaseRegSet() = default;

			OnReleaseRegSet& operator=(const OnReleaseRegSet&) = delete;
			OnReleaseRegSet& operator=(OnReleaseRegSet&&) = delete;
		};
	}


	namespace StoryEvents
	{
		class OnActorKillRegSet : public SKSE::RegistrationSet<const RE::Actor*, const RE::Actor*>
		{
		public:
			using Base = SKSE::RegistrationSet<const RE::Actor*, const RE::Actor*>;


			static OnActorKillRegSet* GetSingleton();

		private:
			OnActorKillRegSet();
			OnActorKillRegSet(const OnActorKillRegSet&) = delete;
			OnActorKillRegSet(OnActorKillRegSet&&) = delete;
			~OnActorKillRegSet() = default;

			OnActorKillRegSet& operator=(const OnActorKillRegSet&) = delete;
			OnActorKillRegSet& operator=(OnActorKillRegSet&&) = delete;
		};


		class OnBooksReadRegSet : public SKSE::RegistrationSet<const RE::TESObjectBOOK*>
		{
		public:
			using Base = SKSE::RegistrationSet<const RE::TESObjectBOOK*>;


			static OnBooksReadRegSet* GetSingleton();

		private:
			OnBooksReadRegSet();
			OnBooksReadRegSet(const OnBooksReadRegSet&) = delete;
			OnBooksReadRegSet(OnBooksReadRegSet&&) = delete;
			~OnBooksReadRegSet() = default;

			OnBooksReadRegSet& operator=(const OnBooksReadRegSet&) = delete;
			OnBooksReadRegSet& operator=(OnBooksReadRegSet&&) = delete;
		};


		class OnCriticalHitRegSet : public SKSE::RegistrationSet<const RE::Actor*, const RE::TESObjectWEAP*, bool>
		{
		public:
			using Base = SKSE::RegistrationSet<const RE::Actor*, const RE::TESObjectWEAP*, bool>;


			static OnCriticalHitRegSet* GetSingleton();

		private:
			OnCriticalHitRegSet();
			OnCriticalHitRegSet(const OnCriticalHitRegSet&) = delete;
			OnCriticalHitRegSet(OnCriticalHitRegSet&&) = delete;
			~OnCriticalHitRegSet() = default;

			OnCriticalHitRegSet& operator=(const OnCriticalHitRegSet&) = delete;
			OnCriticalHitRegSet& operator=(OnCriticalHitRegSet&&) = delete;
		};


		class OnDisarmedRegSet : public SKSE::RegistrationSet<const RE::Actor*, const RE::Actor*>
		{
		public:
			using Base = SKSE::RegistrationSet<const RE::Actor*, const RE::Actor*>;


			static OnDisarmedRegSet* GetSingleton();

		private:
			OnDisarmedRegSet();
			OnDisarmedRegSet(const OnDisarmedRegSet&) = delete;
			OnDisarmedRegSet(OnDisarmedRegSet&&) = delete;
			~OnDisarmedRegSet() = default;

			OnDisarmedRegSet& operator=(const OnDisarmedRegSet&) = delete;
			OnDisarmedRegSet& operator=(OnDisarmedRegSet&&) = delete;
		};


		class OnDragonSoulsGainedRegSet : public SKSE::RegistrationSet<float>
		{
		public:
			using Base = SKSE::RegistrationSet<float>;


			static OnDragonSoulsGainedRegSet* GetSingleton();

		private:
			OnDragonSoulsGainedRegSet();
			OnDragonSoulsGainedRegSet(const OnDragonSoulsGainedRegSet&) = delete;
			OnDragonSoulsGainedRegSet(OnDragonSoulsGainedRegSet&&) = delete;
			~OnDragonSoulsGainedRegSet() = default;

			OnDragonSoulsGainedRegSet& operator=(const OnDragonSoulsGainedRegSet&) = delete;
			OnDragonSoulsGainedRegSet& operator=(OnDragonSoulsGainedRegSet&&) = delete;
		};


		class OnItemHarvestedRegSet : public SKSE::RegistrationSet<const RE::TESForm*>
		{
		public:
			using Base = SKSE::RegistrationSet<const RE::TESForm*>;


			static OnItemHarvestedRegSet* GetSingleton();

		private:
			OnItemHarvestedRegSet();
			OnItemHarvestedRegSet(const OnItemHarvestedRegSet&) = delete;
			OnItemHarvestedRegSet(OnItemHarvestedRegSet&&) = delete;
			~OnItemHarvestedRegSet() = default;

			OnItemHarvestedRegSet& operator=(const OnItemHarvestedRegSet&) = delete;
			OnItemHarvestedRegSet& operator=(OnItemHarvestedRegSet&&) = delete;
		};


		class OnLevelIncreaseRegSet : public SKSE::RegistrationSet<std::uint32_t>
		{
		public:
			using Base = SKSE::RegistrationSet<std::uint32_t>;


			static OnLevelIncreaseRegSet* GetSingleton();

		private:
			OnLevelIncreaseRegSet();
			OnLevelIncreaseRegSet(const OnLevelIncreaseRegSet&) = delete;
			OnLevelIncreaseRegSet(OnLevelIncreaseRegSet&&) = delete;
			~OnLevelIncreaseRegSet() = default;

			OnLevelIncreaseRegSet& operator=(const OnLevelIncreaseRegSet&) = delete;
			OnLevelIncreaseRegSet& operator=(OnLevelIncreaseRegSet&&) = delete;
		};


		class OnLocationDiscoveryRegSet : public SKSE::RegistrationSet<RE::BSFixedString, RE::BSFixedString>
		{
		public:
			using Base = SKSE::RegistrationSet<RE::BSFixedString, RE::BSFixedString>;


			static OnLocationDiscoveryRegSet* GetSingleton();

		private:
			OnLocationDiscoveryRegSet();
			OnLocationDiscoveryRegSet(const OnLocationDiscoveryRegSet&) = delete;
			OnLocationDiscoveryRegSet(OnLocationDiscoveryRegSet&&) = delete;
			~OnLocationDiscoveryRegSet() = default;

			OnLocationDiscoveryRegSet& operator=(const OnLocationDiscoveryRegSet&) = delete;
			OnLocationDiscoveryRegSet& operator=(OnLocationDiscoveryRegSet&&) = delete;
		};


		class OnShoutAttackRegSet : public SKSE::RegistrationSet<const RE::TESShout*>
		{
		public:
			using Base = SKSE::RegistrationSet<const RE::TESShout*>;


			static OnShoutAttackRegSet* GetSingleton();

		private:
			OnShoutAttackRegSet();
			OnShoutAttackRegSet(const OnShoutAttackRegSet&) = delete;
			OnShoutAttackRegSet(OnShoutAttackRegSet&&) = delete;
			~OnShoutAttackRegSet() = default;

			OnShoutAttackRegSet& operator=(const OnShoutAttackRegSet&) = delete;
			OnShoutAttackRegSet& operator=(OnShoutAttackRegSet&&) = delete;
		};


		class OnSkillIncreaseRegSet : public SKSE::RegistrationSet<RE::BSFixedString>
		{
		public:
			using Base = SKSE::RegistrationSet<RE::BSFixedString>;


			static OnSkillIncreaseRegSet* GetSingleton();

		private:
			OnSkillIncreaseRegSet();
			OnSkillIncreaseRegSet(const OnSkillIncreaseRegSet&) = delete;
			OnSkillIncreaseRegSet(OnSkillIncreaseRegSet&&) = delete;
			~OnSkillIncreaseRegSet() = default;

			OnSkillIncreaseRegSet& operator=(const OnSkillIncreaseRegSet&) = delete;
			OnSkillIncreaseRegSet& operator=(OnSkillIncreaseRegSet&&) = delete;
		};


		class OnSoulsTrappedRegSet : public SKSE::RegistrationSet<const RE::Actor*, const RE::Actor*>
		{
		public:
			using Base = SKSE::RegistrationSet<const RE::Actor*, const RE::Actor*>;


			static OnSoulsTrappedRegSet* GetSingleton();

		private:
			OnSoulsTrappedRegSet();
			OnSoulsTrappedRegSet(const OnSoulsTrappedRegSet&) = delete;
			OnSoulsTrappedRegSet(OnSoulsTrappedRegSet&&) = delete;
			~OnSoulsTrappedRegSet() = default;

			OnSoulsTrappedRegSet& operator=(const OnSoulsTrappedRegSet&) = delete;
			OnSoulsTrappedRegSet& operator=(OnSoulsTrappedRegSet&&) = delete;
		};


		class OnSpellsLearnedRegSet : public SKSE::RegistrationSet<const RE::SpellItem*>
		{
		public:
			using Base = SKSE::RegistrationSet<const RE::SpellItem*>;


			static OnSpellsLearnedRegSet* GetSingleton();

		private:
			OnSpellsLearnedRegSet();
			OnSpellsLearnedRegSet(const OnSpellsLearnedRegSet&) = delete;
			OnSpellsLearnedRegSet(OnSpellsLearnedRegSet&&) = delete;
			~OnSpellsLearnedRegSet() = default;

			OnSpellsLearnedRegSet& operator=(const OnSpellsLearnedRegSet&) = delete;
			OnSpellsLearnedRegSet& operator=(OnSpellsLearnedRegSet&&) = delete;
		};
	}


	namespace HookedEvents
	{
		class OnActorResurrectRegSet : public SKSE::RegistrationSetUnique<const RE::Actor*, bool>
		{
		public:
			using Base = SKSE::RegistrationSetUnique<const RE::Actor*, bool>;


			static OnActorResurrectRegSet* GetSingleton();

		private:
			OnActorResurrectRegSet();
			OnActorResurrectRegSet(const OnActorResurrectRegSet&) = delete;
			OnActorResurrectRegSet(OnActorResurrectRegSet&&) = delete;
			~OnActorResurrectRegSet() = default;

			OnActorResurrectRegSet& operator=(const OnActorResurrectRegSet&) = delete;
			OnActorResurrectRegSet& operator=(OnActorResurrectRegSet&&) = delete;
		};


		class OnActorReanimateStartRegSet : public SKSE::RegistrationSetUnique<const RE::Actor*, const RE::Actor*>
		{
		public:
			using Base = SKSE::RegistrationSetUnique<const RE::Actor*, const RE::Actor*>;


			static OnActorReanimateStartRegSet* GetSingleton();

		private:
			OnActorReanimateStartRegSet();
			OnActorReanimateStartRegSet(const OnActorReanimateStartRegSet&) = delete;
			OnActorReanimateStartRegSet(OnActorReanimateStartRegSet&&) = delete;
			~OnActorReanimateStartRegSet() = default;

			OnActorReanimateStartRegSet& operator=(const OnActorReanimateStartRegSet&) = delete;
			OnActorReanimateStartRegSet& operator=(OnActorReanimateStartRegSet&&) = delete;
		};


		class OnActorReanimateStopRegSet : public SKSE::RegistrationSetUnique<const RE::Actor*, const RE::Actor*>
		{
		public:
			using Base = SKSE::RegistrationSetUnique<const RE::Actor*, const RE::Actor*>;


			static OnActorReanimateStopRegSet* GetSingleton();

		private:
			OnActorReanimateStopRegSet();
			OnActorReanimateStopRegSet(const OnActorReanimateStopRegSet&) = delete;
			OnActorReanimateStopRegSet(OnActorReanimateStopRegSet&&) = delete;
			~OnActorReanimateStopRegSet() = default;

			OnActorReanimateStopRegSet& operator=(const OnActorReanimateStopRegSet&) = delete;
			OnActorReanimateStopRegSet& operator=(OnActorReanimateStopRegSet&&) = delete;
		};


		class OnWeatherChangeRegSet : public SKSE::RegistrationSet<const RE::TESWeather*, const RE::TESWeather*>
		{
		public:
			using Base = SKSE::RegistrationSet<const RE::TESWeather*, const RE::TESWeather*>;


			static OnWeatherChangeRegSet* GetSingleton();

		private:
			OnWeatherChangeRegSet();
			OnWeatherChangeRegSet(const OnWeatherChangeRegSet&) = delete;
			OnWeatherChangeRegSet(OnWeatherChangeRegSet&&) = delete;
			~OnWeatherChangeRegSet() = default;

			OnWeatherChangeRegSet& operator=(const OnWeatherChangeRegSet&) = delete;
			OnWeatherChangeRegSet& operator=(OnWeatherChangeRegSet&&) = delete;
		};


		class OnMagicEffectApplyRegMap : public SKSE::RegistrationMapUnique<const RE::TESObjectREFR*, const RE::EffectSetting*, const RE::TESForm*, bool>
		{
		public:
			using Base = SKSE::RegistrationMapUnique<const RE::TESObjectREFR*, const RE::EffectSetting*, const RE::TESForm*, bool>;


			static OnMagicEffectApplyRegMap* GetSingleton();

		private:
			OnMagicEffectApplyRegMap();
			OnMagicEffectApplyRegMap(const OnMagicEffectApplyRegMap&) = delete;
			OnMagicEffectApplyRegMap(OnMagicEffectApplyRegMap&&) = delete;
			~OnMagicEffectApplyRegMap() = default;

			OnMagicEffectApplyRegMap& operator=(const OnMagicEffectApplyRegMap&) = delete;
			OnMagicEffectApplyRegMap& operator=(OnMagicEffectApplyRegMap&&) = delete;
		};


		class OnWeaponHitRegSet : public SKSE::RegistrationSetUnique<const RE::TESObjectREFR*, const RE::TESForm*, const RE::BGSProjectile*, std::uint32_t>
		{
		public:
			using Base = SKSE::RegistrationSetUnique<const RE::TESObjectREFR*, const RE::TESForm*, const RE::BGSProjectile*, std::uint32_t>;


			static OnWeaponHitRegSet* GetSingleton();

		private:
			OnWeaponHitRegSet();
			OnWeaponHitRegSet(const OnWeaponHitRegSet&) = delete;
			OnWeaponHitRegSet(OnWeaponHitRegSet&&) = delete;
			~OnWeaponHitRegSet() = default;

			OnWeaponHitRegSet& operator=(const OnWeaponHitRegSet&) = delete;
			OnWeaponHitRegSet& operator=(OnWeaponHitRegSet&&) = delete;
		};


		class OnMagicHitRegSet : public SKSE::RegistrationSetUnique<const RE::TESObjectREFR*, const RE::TESForm*, const RE::BGSProjectile*>
		{
		public:
			using Base = SKSE::RegistrationSetUnique<const RE::TESObjectREFR*, const RE::TESForm*, const RE::BGSProjectile*>;


			static OnMagicHitRegSet* GetSingleton();

		private:
			OnMagicHitRegSet();
			OnMagicHitRegSet(const OnMagicHitRegSet&) = delete;
			OnMagicHitRegSet(OnMagicHitRegSet&&) = delete;
			~OnMagicHitRegSet() = default;

			OnMagicHitRegSet& operator=(const OnMagicHitRegSet&) = delete;
			OnMagicHitRegSet& operator=(OnMagicHitRegSet&&) = delete;
		};
	}


	namespace FECEvents
	{
		class OnFECResetRegMap : public SKSE::RegistrationMap<const RE::Actor*, std::uint32_t, bool>
		{
		public:
			using Base = SKSE::RegistrationMap<const RE::Actor*, std::uint32_t, bool>;


			static OnFECResetRegMap* GetSingleton();

		private:
			OnFECResetRegMap();
			OnFECResetRegMap(const OnFECResetRegMap&) = delete;
			OnFECResetRegMap(OnFECResetRegMap&&) = delete;
			~OnFECResetRegMap() = default;

			OnFECResetRegMap& operator=(const OnFECResetRegMap&) = delete;
			OnFECResetRegMap& operator=(OnFECResetRegMap&&) = delete;
		};
	}
}
