#pragma once

#include "main.h"

namespace Serialize
{
	using FormRecordPair = std::pair<RE::FormID, RE::FormID>;

	class Base
	{
	public:
		virtual std::set<FormRecordPair>& GetData(UInt32 add) = 0;
		virtual const char* GetType(UInt32 add) = 0;
		virtual void ApplyData(FormRecordPair& a_pair, UInt32 add) = 0;

		enum : UInt32
		{
			kInvalid = static_cast<UInt32>(-1),
			kRemove = 0,
			kAdd = 1
		};

		void Clear(UInt32 add);
		bool Save(SKSE::SerializationInterface* a_intfc, UInt32 a_type, UInt32 a_version, UInt32 add);
		bool Save(SKSE::SerializationInterface* a_intfc, UInt32 add);
		bool Load(SKSE::SerializationInterface* a_intfc, UInt32 add);
	};

	//----------------------------------------------------------------------

	class Perks : public Base
	{
	private:
		std::set<FormRecordPair> perkAddData;
		std::set<FormRecordPair> perkRemoveData;

	public:
		static Perks* GetSingleton();

		virtual std::set<FormRecordPair>& GetData(UInt32 add) override
		{
			return add == kAdd ? perkAddData : perkRemoveData;
		}
		virtual const char* GetType(UInt32 add) override
		{
			return add == kAdd ? "Add Perk" : "Remove Perk";
		}
		virtual void ApplyData(FormRecordPair& a_pair, UInt32 add) override
		{
			auto actor = a_pair.first == kInvalid ? nullptr : RE::TESForm::LookupByID<RE::Actor>(a_pair.first);
			auto perk = a_pair.second == kInvalid ? nullptr : RE::TESForm::LookupByID<RE::BGSPerk>(a_pair.second);

			if (actor && perk) {
				_MESSAGE("[%s] : %s", GetType(add), actor->GetName());
				if (ApplyPerks(actor, perk, add)) {
					auto set = GetData(add);
					set.insert(a_pair);
				}
			}
		}

		bool ApplyPerks(RE::Actor* a_actor, RE::BGSPerk* perk, UInt32 add);
		bool PapyrusApplyPerks(RE::Actor* a_actor, RE::BGSPerk* perk, UInt32 add);

	protected:
		Perks() = default;
		Perks(const Perks&) = delete;
		Perks(Perks&&) = delete;
		~Perks() = default;

		Perks& operator=(const Perks&) = delete;
		Perks& operator=(Perks&&) = delete;
	};

	//-----------------------------------------------------------------------

	class Keywords : public Base
	{
	private:
		std::set<FormRecordPair> keywordAddData;
		std::set<FormRecordPair> keywordRemoveData;

	public:
		static Keywords* GetSingleton();

		virtual std::set<FormRecordPair>& GetData(UInt32 add) override
		{
			return add == kAdd ? keywordAddData : keywordRemoveData;
		}
		virtual const char* GetType(UInt32 add) override
		{
			return add == kAdd ? "Add Keyword" : "Remove Keyword";
		}
		virtual void ApplyData(FormRecordPair& a_pair, UInt32 add) override
		{
			auto actor = a_pair.first == kInvalid ? nullptr : RE::TESForm::LookupByID<RE::Actor>(a_pair.first);
			auto keyword = a_pair.second == kInvalid ? nullptr : RE::TESForm::LookupByID<RE::BGSKeyword>(a_pair.second);

			if (actor && keyword) {
				if (ApplyKeywords(actor, keyword, add)) {
					auto set = GetData(add);
					set.insert(a_pair);
				}
			}
		}

		bool ApplyKeywords(RE::TESForm* a_form, RE::BGSKeyword* keyword, UInt32 add);
		bool PapyrusApplyKeywords(RE::TESForm* a_form, RE::BGSKeyword* keyword, UInt32 add);

	protected:
		Keywords() = default;
		Keywords(const Keywords&) = delete;
		Keywords(Keywords&&) = delete;
		~Keywords() = default;

		Keywords& operator=(const Keywords&) = delete;
		Keywords& operator=(Keywords&&) = delete;
	};
}
