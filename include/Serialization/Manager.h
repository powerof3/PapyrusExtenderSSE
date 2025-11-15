#pragma once

#include "Registration/DataMap.h"
#include "Registration/DataSet.h"

namespace Serialization
{
	enum : std::uint32_t
	{
		kSerializationVersion = 10,

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

	namespace FORM
	{
		enum : std::uint32_t
		{
			kRemove = 0,
			kAdd = 1
		};
	}

	namespace DETECTION
	{
		enum : std::uint32_t
		{
			kAlert = 0,
			kHide = 1,
		};
	}

	class KeywordHolder : public FormMapPair<RE::TESForm, RE::BGSKeyword>
	{
	public:
		KeywordHolder() = default;
		~KeywordHolder() override = default;

		bool Process(RE::TESForm* a_form, RE::BGSKeyword* a_data, std::uint32_t a_index) override;

		// members
		static constexpr std::uint32_t addUUID{ kAddKeywords };
		static constexpr std::uint32_t removeUUID{ kRemoveKeywords };
	};

	class PerkHolder : public FormMapPair<RE::Actor, RE::BGSPerk>
	{
	public:
		PerkHolder() = default;
		~PerkHolder() override = default;

		bool Process(RE::Actor* a_form, RE::BGSPerk* a_data, std::uint32_t a_index) override;

		// members
		static constexpr std::uint32_t addUUID{ kAddPerks };
		static constexpr std::uint32_t removeUUID{ kRemovePerks };
	};

	//target
	class TargetHolder : public FormSetPair<RE::Actor>
	{
	public:
		// members
		static constexpr std::uint32_t addUUID{ kTargetAlert };
		static constexpr std::uint32_t removeUUID{ kTargetHide };
	};

	//searcher
	class SourceHolder : public FormSetPair<RE::Actor>
	{
	public:
		// members
		static constexpr std::uint32_t addUUID{ kSourceAlert };
		static constexpr std::uint32_t removeUUID{ kSourceHide };
	};

	struct MGEFData
	{
		MGEFData() = default;

		MGEFData(RE::EffectSetting* a_effect, float a_mag, std::uint32_t a_area, std::uint32_t a_duration, float a_cost, const std::vector<std::string>& a_conditionList = {}) :
			mgef(a_effect),
			mgefFormID(a_effect->GetFormID()),
			mag(a_mag),
			area(a_area),
			dur(a_duration),
			cost(a_cost),
			conditionList(a_conditionList)
		{}

		bool operator<(const MGEFData& a_rhs) const { return mgefFormID < a_rhs.mgefFormID; }

		bool operator==(const MGEFData& a_rhs) const { return std::tie(mgefFormID, mag, area, dur, cost, conditionList) == std::tie(a_rhs.mgefFormID, a_rhs.mag, a_rhs.area, a_rhs.dur, a_rhs.cost, a_rhs.conditionList); }

		bool        equals(RE::Effect* a_effect) const;
		RE::Effect* get_matching_effect(RE::MagicItem* a_item) const;
		bool        add_magic_effect(RE::MagicItem* a_item) const;
		bool        remove_magic_effect(RE::MagicItem* a_item) const;

		bool save(SKSE::SerializationInterface* a_intfc) const;
		bool load(SKSE::SerializationInterface* a_intfc, std::size_t index);

		// members
		RE::EffectSetting*       mgef;
		RE::FormID               mgefFormID;
		float                    mag;
		std::uint32_t            area;
		std::uint32_t            dur;
		float                    cost;
		std::vector<std::string> conditionList;
	};

	struct EffectData
	{
		EffectData() = default;

		EffectData(RE::MagicItem* a_effectItem, std::uint32_t a_index, float a_cost) :
			magicItem(a_effectItem),
			magicItemFormID(a_effectItem->GetFormID()),
			index(a_index),
			cost(a_cost)
		{}

		bool operator<(const EffectData& a_rhs) const { return magicItemFormID < a_rhs.magicItemFormID; }

		bool operator==(const EffectData& a_rhs) const { return std::tie(magicItemFormID, index, cost) == std::tie(a_rhs.magicItemFormID, a_rhs.index, a_rhs.cost); }

		bool equals(const RE::Effect* a_effect, const RE::Effect* a_copyEffect) const;
		bool add_effect_item(RE::MagicItem* a_item) const;
		bool remove_effect_item(RE::MagicItem* a_item) const;

		bool save(SKSE::SerializationInterface* a_intfc) const;
		bool load(SKSE::SerializationInterface* a_intfc, std::size_t index);

		// members
		RE::MagicItem* magicItem;
		RE::FormID     magicItemFormID;
		std::uint32_t  index;
		float          cost;
	};

	class MGEFHolder : public FormDataMapPair<RE::MagicItem, MGEFData>
	{
	public:
		MGEFHolder() = default;
		~MGEFHolder() override = default;

		void ProcessEntry(const RE::FormID a_key, std::vector<MGEFData>& a_data, std::uint32_t a_index) override;
		bool Process(RE::MagicItem* a_form, const MGEFData& a_data, std::uint32_t a_index) override;
		bool SaveImpl(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index) const override;

		// members
		static constexpr std::uint32_t addUUID{ kAddMGEF };
		static constexpr std::uint32_t removeUUID{ kRemoveMGEF };
	};

	class EffectHolder : public FormDataMapPair<RE::MagicItem, EffectData>
	{
	public:
		EffectHolder() = default;
		~EffectHolder() override = default;

		void ProcessEntry(const RE::FormID a_key, std::vector<EffectData>& a_data, std::uint32_t a_index) override;
		bool Process(RE::MagicItem* a_form, const EffectData& a_data, std::uint32_t a_index) override;
		bool SaveImpl(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index) const override;

		// members
		static constexpr std::uint32_t addUUID{ kAddEffect };
		static constexpr std::uint32_t removeUUID{ kRemoveEffect };
	};

	class Manager :
		public REX::Singleton<Manager>,
		public RE::BSTEventSink<RE::TESFormDeleteEvent>
	{
	public:
		static void Register();

		void Save(SKSE::SerializationInterface* a_intfc);
		void Load(SKSE::SerializationInterface* a_intfc);
		void Revert(SKSE::SerializationInterface* a_intfc);
		void FormDelete(RE::VMHandle a_handle);

		RE::BSEventNotifyControl ProcessEvent(const RE::TESFormDeleteEvent* a_event, RE::BSTEventSource<RE::TESFormDeleteEvent>*) override;

		// members
		KeywordHolder keywords;
		PerkHolder    perks;
		TargetHolder  detectionTargets;
		SourceHolder  detectionSources;
		MGEFHolder    mgefs;
		EffectHolder  effectItems;

	private:
		std::string DecodeTypeCode(std::uint32_t a_typeCode);

		template <class T>
		void save(const T& a_regs, SKSE::SerializationInterface* a_intfc)
		{
			if (!a_regs.GetData(1).empty()) {
				if (!a_regs.Save(a_intfc, a_regs.addUUID, kSerializationVersion, 1)) {
					logger::critical("{} {} : Failed to save regs!"sv, typeid(T).name(), 1);
				}
			}
			if (!a_regs.GetData(0).empty()) {
				if (!a_regs.Save(a_intfc, a_regs.removeUUID, kSerializationVersion, 0)) {
					logger::critical("{} {} : Failed to save regs!"sv, typeid(T).name(), 0);
				}
			}
		}

		template <class T>
		void load(T& a_regs, SKSE::SerializationInterface* a_intfc, std::uint32_t a_index)
		{
			if (!a_regs.Load(a_intfc, a_index)) {
				logger::critical("Failed to load {} reg at {} index!"sv, typeid(T).name(), a_index);
			}
		}
	};

	void SaveCallback(SKSE::SerializationInterface* a_intfc);
	void LoadCallback(SKSE::SerializationInterface* a_intfc);
	void RevertCallback(SKSE::SerializationInterface* a_intfc);
	void FormDeleteCallback(RE::VMHandle a_handle);
}
