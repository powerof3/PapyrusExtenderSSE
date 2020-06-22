#include "Serialization/Serialize.h"


namespace Serialize
{
	void Base::Clear(UInt32 add)
	{
		GetData(add).clear();
	}


	bool Base::Save(SKSE::SerializationInterface* a_intfc, UInt32 a_type, UInt32 a_version, UInt32 add)
	{
		if (!a_intfc->OpenRecord(a_type, a_version)) {
			_ERROR("Failed to open serialization record!\n");
			return false;
		}
		else {
			return Save(a_intfc, add);
		}
	}


	bool Base::Save(SKSE::SerializationInterface* a_intfc, UInt32 add)
	{
		size_t size = GetData(add).size();
		if (!a_intfc->WriteRecordData(&size, sizeof(size))) {
			_ERROR("[%s] : Failed to write size to save!", GetType(add));
			return false;
		}
		else {
			for (auto& pair : GetData(add)) {
				if (!a_intfc->WriteRecordData(&pair.first, sizeof(pair.first))) {
					_ERROR("[%s] : Failed to write formID %X to save", GetType(add), pair.first);
					break;
				}
				if (!a_intfc->WriteRecordData(&pair.second, sizeof(pair.second))) {
					_ERROR("[%s] : Failed to write formID %X to save", GetType(add), pair.second);
					break;
				}
			}
		}

		return true;
	}

	bool Base::Load(SKSE::SerializationInterface* a_intfc, UInt32 add)
	{
		size_t size = a_intfc->ReadRecordData(&size, sizeof(size));
		for (size_t i = 0; i < size; i++) {
			FormRecordPair pair;
			for (size_t k = 0; k < 2; k++) {
				RE::FormID formID;
				a_intfc->ReadRecordData(&formID, sizeof(formID));
				if (k == 0) {
					if (!a_intfc->ResolveFormID(formID, formID)) {
						_ERROR("[%s] : [%u] [%u] : Failed to resolve formID", GetType(add), i, k);
						formID = kInvalid;
					}
					pair.first = formID;
				}
				if (k == 1) {
					if (!a_intfc->ResolveFormID(formID, formID)) {
						_ERROR("[%s] : [%u] [%u] : Failed to resolve formID", GetType(add), i, k);
						formID = kInvalid;
					}
					pair.second = formID;
				}
			}
			ApplyData(pair, add);
		}
		return true;
	}


	Perks* Perks::GetSingleton()
	{
		static Perks singleton;
		return &singleton;
	}


	bool Perks::ApplyPerks(RE::Actor* a_actor, RE::BGSPerk* perk, UInt32 add)
	{
		bool success = false;
		auto actorbase = a_actor->GetActorBase();
		if (actorbase) {
			if (add == kAdd) {
				if (actorbase->AddPerk(perk, 1)) {
					success = true;
				}
			}
			else if (add == kRemove) {
				if (actorbase->RemovePerk(perk)) {
					success = true;
				}
			}
			a_actor->ApplyPerksFromBase();
		}
		return success;
	}

	bool Perks::PapyrusApplyPerks(RE::Actor* a_actor, RE::BGSPerk* perk, UInt32 add)
	{
		if (ApplyPerks(a_actor, perk, add)) {
			FormRecordPair pair = std::make_pair(a_actor->formID, perk->formID);

			auto& set = GetData(add);
			auto& otherSet = GetData(!add);

			auto it = otherSet.find(pair);
			if (it != otherSet.end()) {
				otherSet.erase(it);
			}
			set.insert(pair);

			return true;
		}

		return false;
	}


	Keywords* Keywords::GetSingleton()
	{
		static Keywords singleton;
		return &singleton;
	}


	bool Keywords::ApplyKeywords(RE::TESForm* a_form, RE::BGSKeyword* keyword, UInt32 add)
	{
		auto pKeywords = a_form->As<RE::BGSKeywordForm>();
		if (pKeywords) {
			if (add == kAdd) {
				if (pKeywords->AddKeyword(keyword)) {
					return true;
				}
			}
			else if (add == kRemove) {
				if (pKeywords->RemoveKeyword(keyword)) {
					return true;
				}
			}
		}
		return false;
	}


	bool Keywords::PapyrusApplyKeywords(RE::TESForm* a_form, RE::BGSKeyword* keyword, UInt32 add)
	{
		if (ApplyKeywords(a_form, keyword, add)) {
			FormRecordPair pair = std::make_pair(a_form->formID, keyword->formID);

			auto& set = GetData(add);
			auto& otherSet = GetData(!add);

			auto it = otherSet.find(pair);
			if (it != otherSet.end()) {
				otherSet.erase(it);
			}
			set.insert(pair);

			return true;
		}

		return false;
	}
}