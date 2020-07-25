#pragma once

#include "Serialization/Base.h"


namespace Serialize
{
	class Keywords : public Base
	{
	public:
		static Keywords* GetSingleton();

		virtual const char* GetType(std::uint32_t a_add) override;
		virtual void LoadData(FormData a_data, std::uint32_t a_add) override;

		bool ApplyKeywords(RE::TESForm* a_form, RE::BGSKeyword* a_keyword, std::uint32_t a_add);
		bool PapyrusApplyKeywords(RE::TESForm* a_form, RE::BGSKeyword* a_keyword, std::uint32_t a_add);

	protected:
		Keywords() = default;
		Keywords(const Keywords&) = delete;
		Keywords(Keywords&&) = delete;
		~Keywords() = default;

		Keywords& operator=(const Keywords&) = delete;
		Keywords& operator=(Keywords&&) = delete;
	};
}
