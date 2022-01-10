#pragma once

namespace Serialization
{
	enum : std::uint32_t
	{
		kSerializationVersion = 4,

		kPapyrusExtender = 'P3PE',

		kAddPerks = 'APTN',
		kRemovePerks = 'RPFN',
		kAddKeywords = 'AKTF',
		kRemoveKeywords = 'RKOF',

		kOnCellFullyLoaded = 'CELL',
		kQuestStart = 'QSTR',
		kQuestStop = 'QSTP',
		kQuestStage = 'QSTG',
		kObjectLoaded = 'LOAD',
		kObjectUnloaded = 'UNLD',
		kGrab = 'GRAB',
		kRelease = 'RELS',

		kActorKill = 'KILL',
		kBookRead = 'BOOK',
		kCritHit = 'CRIT',
		kDisarm = 'DSRM',
		kDragonSoul = 'DRGN',
		kHarvest = 'HVST',
		kLevelIncrease = 'LEVL',
		kLocDiscovery = 'DISC',
		kSkillIncrease = 'SKIL',
		kSoulTrap = 'SOUL',
		kShoutAttack = 'SHOU',
		kSpellLearned = 'SPEL',

		kActorFallLongDistance = 'FALL',
		kActorResurrect = 'RSUR',
		kActorReanimateStart = 'RSTA',
		kActorReanimateStop = 'REND',
		kWeatherChange = 'WEAT',
		kMagicEffectApply = 'MGEF',
		kWeaponHit = 'WHIT',
		kMagicHit = 'MHIT',
		kProjectileHit = 'PHIT',

		kFECReset = 'FECR',

		kTargetHide = 'TGHI',
		kTargetAlert = 'TGAL',
		kSourceHide = 'SRHI',
		kSourceAlert = 'SRAL',

		kAddMGEF = 'AMGE',
		kRemoveMGEF = 'RMGE',
		kAddEffect = 'AEFF',
		kRemoveEffect = 'REFF'
	};

	void SaveCallback(SKSE::SerializationInterface* a_intfc);
	void LoadCallback(SKSE::SerializationInterface* a_intfc);
	void RevertCallback(SKSE::SerializationInterface* a_intfc);
	void FormDeleteCallback(RE::VMHandle a_handle);

	template <class T>
	void SAVE(SKSE::SerializationInterface* a_intfc, std::uint32_t a_version1, std::uint32_t a_version0)
	{
		const auto regs = T::GetSingleton();
		if (!regs->GetData(1).empty()) {
			if (!regs->Save(a_intfc, a_version1, kSerializationVersion, 1)) {
				logger::critical("{} {} : Failed to save regs!"sv, typeid(T).name(), 1);
			}
		}
		if (!regs->GetData(0).empty()) {
			if (!regs->Save(a_intfc, a_version0, kSerializationVersion, 0)) {
				logger::critical("{} {} : Failed to save regs!"sv, typeid(T).name(), 0);
			}
		}
	}

	template <class T>
	void SAVE(SKSE::SerializationInterface* a_intfc, std::uint32_t a_version)
	{
		const auto regs = T::GetSingleton();
		if (!regs->Save(a_intfc, a_version, kSerializationVersion)) {
			logger::critical("Failed to save {} regs!"sv, typeid(T).name());
		}
	}

	template <class T>
	void LOAD(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index)
	{
		const auto regs = T::GetSingleton();
		if (!regs->Load(a_intfc, a_index)) {
			logger::critical("Failed to load {} reg at {} index!"sv, typeid(T).name(), a_index);
		}
	}

	template <class T>
	void LOAD(SKSE::SerializationInterface* a_intfc)
	{
		const auto regs = T::GetSingleton();
		if (!regs->Load(a_intfc)) {
			logger::critical("Failed to load {} regs!"sv, typeid(T).name());
		}
	}

	template <class T>
	void REVERT(SKSE::SerializationInterface* a_intfc)
	{
		const auto regs = T::GetSingleton();
		regs->Revert(a_intfc);
	}

	template <class T>
	void FORM_DELETE(RE::FormID a_formID)
	{
		const auto regs = T::GetSingleton();
		regs->Remove(a_formID);
	}

	template <class T>
	void FORM_DELETE(RE::VMHandle a_handle)
	{
		const auto regs = T::GetSingleton();
		if constexpr (std::disjunction_v<
						  typename std::is_base_of<SKSE::Impl::RegistrationMapBase, T>::type,
						  typename std::is_base_of<SKSE::Impl::RegistrationMapUniqueBase, T>::type>) {
			regs->UnregisterAll(a_handle);
		} else {
			regs->Unregister(a_handle);
		}
	}

	namespace FormDeletion
	{
		using EventResult = RE::BSEventNotifyControl;

		class EventHandler : public RE::BSTEventSink<RE::TESFormDeleteEvent>
		{
		public:
			static EventHandler* GetSingleton();

			EventResult ProcessEvent(const RE::TESFormDeleteEvent* a_event, RE::BSTEventSource<RE::TESFormDeleteEvent>*) override;

		private:
			EventHandler() = default;
			EventHandler(const EventHandler&) = delete;
			EventHandler(EventHandler&&) = delete;
			virtual ~EventHandler() = default;

			EventHandler& operator=(const EventHandler&) = delete;
			EventHandler& operator=(EventHandler&&) = delete;
		};

		void Register();
	}
}
