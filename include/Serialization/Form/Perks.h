#pragma once

#include "Serialization/Form/Base.h"


namespace Serialization
{
	namespace Form
	{
		class Perks : public Base
		{
		public:
			static Perks* GetSingleton();

			virtual void LoadData(std::uint32_t a_add) override;

			bool Apply(RE::Actor* a_actor, RE::BGSPerk* perk, std::uint32_t a_add);
			bool PapyrusApply(RE::Actor* a_actor, RE::BGSPerk* perk, std::uint32_t a_add);

		protected:
			Perks() = default;
			Perks(const Perks&) = delete;
			Perks(Perks&&) = delete;

			Perks& operator=(const Perks&) = delete;
			Perks& operator=(Perks&&) = delete;
		};
	}
}
