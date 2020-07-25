#include "Serialization/Perks.h"


namespace Serialize
{
	Perks* Perks::GetSingleton()
	{
		static Perks singleton;
		return &singleton;
	}


	const char* Perks::GetType(std::uint32_t a_add)
	{
		return a_add == kAdd ? "Perk[ADD]" : "Perk[REMOVE]";
	}


	void Perks::LoadData(FormData a_data, std::uint32_t a_add)
	{
		auto actor = a_data._formID == kInvalid ? nullptr : RE::TESForm::LookupByID<RE::Actor>(a_data._formID);
		auto perk = a_data._dataID == kInvalid ? nullptr : RE::TESForm::LookupByID<RE::BGSPerk>(a_data._dataID);

		if (actor && perk) {
			if (ApplyPerks(actor, perk, a_add)) {
				GetData(a_add).push_back(a_data);
			}
		}
	}


	bool Perks::ApplyPerks(RE::Actor* a_actor, RE::BGSPerk* a_perk, std::uint32_t a_add)
	{
		bool success = false;

		auto actorbase = a_actor->GetActorBase();
		if (actorbase) {
			success = a_add == kAdd ? actorbase->AddPerk(a_perk, 1) : actorbase->RemovePerk(a_perk);
			a_actor->ApplyPerksFromBase();
		}

		return success;
	}


	bool Perks::PapyrusApplyPerks(RE::Actor* a_actor, RE::BGSPerk* a_perk, std::uint32_t a_add)
	{
		if (ApplyPerks(a_actor, a_perk, a_add)) {
			SaveData({ a_actor->formID, a_perk->formID }, a_add);
			return true;
		}
		return false;
	}
}