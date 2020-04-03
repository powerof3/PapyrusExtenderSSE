#pragma once

#include "po3_functions.h"

namespace RE
{
	class Base
	{
	public:
		virtual std::vector<std::vector<FormID>>& GetData(UInt32 add) = 0;
		virtual const char* GetType(UInt32 add) = 0;
		virtual UInt32 GetSize()
		{
			return 2;
		}
		virtual void ApplyData(std::vector<FormID>& a_vec, UInt32 add) = 0;

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
		std::vector<std::vector<FormID>> perkAddData;
		std::vector<std::vector<FormID>> perkRemoveData;

	public:
		static Perks* GetSingleton();

		virtual std::vector<std::vector<FormID>>& GetData(UInt32 add) override
		{
			return add == kAdd ? perkAddData : perkRemoveData;
		}
		virtual const char* GetType(UInt32 add) override
		{
			return add == kAdd ? "Add Perk" : "Remove Perk";
		}
		virtual void ApplyData(std::vector<FormID>& a_vec, UInt32 add) override
		{
			auto actor = a_vec[0] == kInvalid ? nullptr : TESForm::LookupByID<Actor>(a_vec[0]);
			auto perk = a_vec[1] == kInvalid ? nullptr : TESForm::LookupByID<BGSPerk>(a_vec[1]);

			if (actor && perk) {
				_MESSAGE("[%s] : %s", GetType(add), actor->GetName());

				if (ApplyPerks(actor, perk, add)) {
					auto vec = GetData(add);
					vec.push_back(a_vec);
				}
			}
		}

		bool ApplyPerks(Actor* a_actor, BGSPerk* perk, UInt32 add);
		bool PapyrusApplyPerks(Actor* a_actor, BGSPerk* perk, UInt32 add);

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
		std::vector<std::vector<FormID>> keywordAddData;
		std::vector<std::vector<FormID>> keywordRemoveData;

	public:
		static Keywords* GetSingleton();

		virtual std::vector<std::vector<FormID>>& GetData(UInt32 add) override
		{
			return add == kAdd ? keywordAddData : keywordRemoveData;
		}
		virtual const char* GetType(UInt32 add) override
		{
			return add == kAdd ? "Add Keyword" : "Remove Keyword";
		}
		virtual void ApplyData(std::vector<FormID>& a_vec, UInt32 add) override
		{
			auto actor = a_vec[0] == kInvalid ? nullptr : TESForm::LookupByID<Actor>(a_vec[0]);
			auto keyword = a_vec[1] == kInvalid ? nullptr : TESForm::LookupByID<BGSKeyword>(a_vec[1]);

			if (actor && keyword) {
				if (ApplyKeywords(actor, keyword, add)) {
					auto vec = GetData(add);
					vec.push_back(a_vec);
				}
			}
		}

		bool ApplyKeywords(TESForm* a_form, BGSKeyword* keyword, UInt32 add);
		bool PapyrusApplyKeywords(TESForm* a_form, BGSKeyword* keyword, UInt32 add);

	protected:
		Keywords() = default;
		Keywords(const Keywords&) = delete;
		Keywords(Keywords&&) = delete;
		~Keywords() = default;

		Keywords& operator=(const Keywords&) = delete;
		Keywords& operator=(Keywords&&) = delete;
	};
}
