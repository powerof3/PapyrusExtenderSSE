#pragma once

#include "Serialization/Form/Base.h"


namespace Serialization
{
	namespace Form
	{
		class Keywords : public Base
		{
		public:
			static Keywords* GetSingleton();

			virtual void LoadData(std::uint32_t a_add) override;

			bool Apply(RE::TESForm* a_form, RE::BGSKeyword* a_keyword, std::uint32_t a_add);
			bool PapyrusApply(RE::TESForm* a_form, RE::BGSKeyword* a_keyword, std::uint32_t a_add);

		protected:
			Keywords() = default;
			Keywords(const Keywords&) = delete;
			Keywords(Keywords&&) = delete;

			Keywords& operator=(const Keywords&) = delete;
			Keywords& operator=(Keywords&&) = delete;
		};
	}
}