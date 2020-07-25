#pragma once

#include "Serialization/Base.h"


namespace Serialize
{
	class Perks : public Base
	{
	public:
		static Perks* GetSingleton();

		virtual const char* GetType(std::uint32_t a_add) override;
		virtual void LoadData(FormData a_data, std::uint32_t a_add) override;

		bool ApplyPerks(RE::Actor* a_actor, RE::BGSPerk* perk, std::uint32_t a_add);
		bool PapyrusApplyPerks(RE::Actor* a_actor, RE::BGSPerk* perk, std::uint32_t a_add);

	protected:
		Perks() = default;
		Perks(const Perks&) = delete;
		Perks(Perks&&) = delete;
		~Perks() = default;

		Perks& operator=(const Perks&) = delete;
		Perks& operator=(Perks&&) = delete;
	};
}
