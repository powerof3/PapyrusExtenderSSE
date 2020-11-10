#include "Serialization/Form/Perks.h"


namespace Serialization
{
	using namespace Form;
	
	Perks* Form::Perks::GetSingleton()
	{
		static Perks singleton;
		return &singleton;
	}


	void Perks::LoadData(std::uint32_t a_add)
	{
		Locker locker(_lock);

		auto& dataSet = GetData(a_add);
		for (auto& [form, data] : dataSet) {
			auto actor = RE::TESForm::LookupByID<RE::Actor>(form);
			auto perk = RE::TESForm::LookupByID<RE::BGSPerk>(data);

			if (actor && perk) {
				Apply(actor, perk, a_add);
			}
		}
	}


	bool Perks::Apply(RE::Actor* a_actor, RE::BGSPerk* a_perk, std::uint32_t a_add)
	{
		bool success = false;

		auto actorbase = a_actor->GetActorBase();
		if (actorbase) {
			success = (a_add == kAdd) ? actorbase->AddPerk(a_perk, 1) : actorbase->RemovePerk(a_perk);
			if (success) {
				a_actor->ApplyPerksFromBase();
			}
		}

		return success;
	}


	bool Perks::PapyrusApply(RE::Actor* a_actor, RE::BGSPerk* a_perk, std::uint32_t a_add)
	{
		if (Apply(a_actor, a_perk, a_add)) {
			if (a_actor->formID >= 0xFF000000) {
				logger::warn("Cannot serialize temporary objects - [0x{:08X}] {}", a_actor->formID, a_actor->GetName());
				return true;
			}
			SaveData({ a_actor->formID, a_perk->formID }, a_add);
			return true;
		}
		return false;
	}
}