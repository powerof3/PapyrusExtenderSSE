#include "po3_serializedclasses.h"

namespace RE
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
			for (auto& vec : GetData(add)) {
				for (auto& data : vec) {
					if (!a_intfc->WriteRecordData(&data, sizeof(data))) {
						_ERROR("[%s] : Failed to write formID %X to save", GetType(add), data);
						break;
					}
				}
			}
		}

		return true;
	}

	bool Base::Load(SKSE::SerializationInterface* a_intfc, UInt32 add)
	{
		size_t size = a_intfc->ReadRecordData(&size, sizeof(size));
		for (size_t i = 0; i < size; i++) {
			std::vector<FormID> vec;
			for (size_t k = 0; k < GetSize(); k++) {
				FormID formID;
				a_intfc->ReadRecordData(&formID, sizeof(formID));
				if (k > 0) {
					if (!a_intfc->ResolveFormID(formID, formID)) {
						_ERROR("[%s] : [%u] [%u] : Failed to resolve formID", GetType(add), i, k);
						formID = kInvalid;
					}
				}
				vec.push_back(formID);
			}
			ApplyData(vec, add);
		}
		return true;
	}

	//-------------------------------------------------------------------------------------

	Perks* Perks::GetSingleton()
	{
		static Perks singleton;
		return &singleton;
	}

	bool Perks::ApplyPerks(Actor* a_actor, BGSPerk* perk, UInt32 add)
	{
		bool success = false;

		auto actorbase = a_actor->GetActorBase();
		if (actorbase) {
			UInt32 index = kInvalid;
			for (UInt32 i = 0; i < actorbase->perkCount; i++) {
				if (actorbase->perks[i].perk && actorbase->perks[i].perk == perk) {
					index = i;
					break;
				}
			}

			if (add == kAdd) {
				if (index == kInvalid) {
					success = true;

					auto oldData = actorbase->perks;
					actorbase->perks = calloc<PerkRankData>(++actorbase->perkCount);
					if (oldData) {
						for (UInt32 i = 0; i < actorbase->perkCount - 1; i++) {
							actorbase->perks[i] = oldData[i];
						}
						free(oldData);
						oldData = nullptr;
					}

					PerkRankData* newPerk;
					newPerk = static_cast<PerkRankData*>(malloc(sizeof(*newPerk)));
					newPerk->perk = perk;
					newPerk->currentRank = 1;
					newPerk->pad09 = 0;
					newPerk->pad0A = 0;
					newPerk->pad0C = 0;

					actorbase->perks[actorbase->perkCount - 1] = *newPerk;
				}
			}
			else if (add == kRemove) {
				if (index != kInvalid) {
					success = true;

					auto oldData = actorbase->perks;
					actorbase->perks = calloc<PerkRankData>(--actorbase->perkCount);
					if (oldData) {
						for (UInt32 i = 0; i < actorbase->perkCount + 1; i++) {
							if (index != i) {
								actorbase->perks[i] = oldData[i];
							}
						}
						free(oldData);
						oldData = nullptr;
					}
				}
			}

			a_actor->ApplyPerksFromBase();
		}

		return success;
	}

	bool Perks::PapyrusApplyPerks(Actor* a_actor, BGSPerk* perk, UInt32 add)
	{
		if (ApplyPerks(a_actor, perk, add)) {
			std::vector<FormID> _data = { a_actor->formID, perk->formID };

			auto& vec = GetData(add);
			auto& otherVec = GetData(!add);

			auto it = find(otherVec.begin(), otherVec.end(), _data);
			if (it != otherVec.end()) {
				otherVec.erase(it);
			}
			vec.push_back(_data);

			return true;
		}

		return false;
	}

	//-------------------------------------------------------------------------------------

	Keywords* Keywords::GetSingleton()
	{
		static Keywords singleton;
		return &singleton;
	}

	bool Keywords::ApplyKeywords(TESForm* thisForm, BGSKeyword* keyword, UInt32 add)
	{
		auto pKeywords = skyrim_cast<BGSKeywordForm*>(thisForm);
		if (pKeywords) {
			UInt32 index = kInvalid;
			for (UInt32 i = 0; i < pKeywords->numKeywords; ++i) {
				if (pKeywords->keywords[i] && pKeywords->keywords[i] == keyword) {
					index = i;
					break;
				}
			}

			if (add == kAdd) {
				if (index != kInvalid) {
					return false;
				}

				auto oldData = pKeywords->keywords;
				pKeywords->keywords = calloc<BGSKeyword*>(++pKeywords->numKeywords);
				if (oldData) {
					for (UInt32 i = 0; i < pKeywords->numKeywords - 1; ++i) {
						pKeywords->keywords[i] = oldData[i];
					}
					free(oldData);
					oldData = nullptr;
				}
				pKeywords->keywords[pKeywords->numKeywords - 1] = keyword;
			}
			else if (add == kRemove) {
				if (index == kInvalid) {
					return false;
				}

				auto oldData = pKeywords->keywords;
				pKeywords->keywords = calloc<BGSKeyword*>(--pKeywords->numKeywords);
				if (oldData) {
					for (UInt32 i = 0; i < pKeywords->numKeywords + 1; ++i) {
						if (index != i) {
							pKeywords->keywords[i] = oldData[i];
						}
					}
					free(oldData);
					oldData = nullptr;
				}
			}

			return true;
		}

		return false;
	}

	bool Keywords::PapyrusApplyKeywords(TESForm* thisForm, BGSKeyword* keyword, UInt32 add)
	{
		if (ApplyKeywords(thisForm, keyword, add)) {
			std::vector<FormID> _data = { thisForm->formID, keyword->formID };

			auto& vec = GetData(add);
			auto& otherVec = GetData(!add);

			auto it = find(otherVec.begin(), otherVec.end(), _data);
			if (it != otherVec.end()) {
				otherVec.erase(it);
			}
			vec.push_back(_data);

			return true;
		}

		return false;
	}
};