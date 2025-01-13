#pragma once

namespace Serialization
{
	enum : std::uint32_t
	{
		kSerializationVersion = 8,

		kPapyrusExtender = 'P3PE',

		kAddPerks = 'APTN',
		kRemovePerks = 'RPFN',
		kAddKeywords = 'AKTF',
		kRemoveKeywords = 'RKOF',

		kTargetHide = 'TGHI',
		kTargetAlert = 'TGAL',
		kSourceHide = 'SRHI',
		kSourceAlert = 'SRAL',

		kAddMGEF = 'AMGE',
		kRemoveMGEF = 'RMGE',
		kAddEffect = 'AEFF',
		kRemoveEffect = 'REFF'
	};

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
	void LOAD(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index)
	{
		const auto regs = T::GetSingleton();
		if (!regs->Load(a_intfc, a_index)) {
			logger::critical("Failed to load {} reg at {} index!"sv, typeid(T).name(), a_index);
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

	void SaveCallback(SKSE::SerializationInterface* a_intfc);
	void LoadCallback(SKSE::SerializationInterface* a_intfc);
	void RevertCallback(SKSE::SerializationInterface* a_intfc);
	void FormDeleteCallback(RE::VMHandle a_handle);

	namespace FormDeletion
	{
		using EventResult = RE::BSEventNotifyControl;

		class EventHandler :
			public REX::Singleton<EventHandler>,
			public RE::BSTEventSink<RE::TESFormDeleteEvent>
		{
		public:
			static void Register()
			{
				RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(GetSingleton());
				logger::info("Registered form deletion event handler"sv);
			}

			EventResult ProcessEvent(const RE::TESFormDeleteEvent* a_event, RE::BSTEventSource<RE::TESFormDeleteEvent>*) override;
		};
	}
}
