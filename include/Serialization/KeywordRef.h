#pragma once

#include "Serialization/KeywordForm.h"


namespace Serialize
{
	class KeywordRef : public Base
	{
	public:
		static KeywordRef* GetSingleton();

		virtual const char* GetType(UInt32 a_add) override;
		virtual void ApplyData(FormRecordPair& a_pair, UInt32 a_add) override;

		bool ApplyKeywords(RE::TESForm* a_form, RE::BGSKeyword* a_keyword, UInt32 a_add);
		bool PapyrusApplyKeywords(RE::TESObjectREFR* a_ref, RE::BGSKeyword* a_keyword, UInt32 a_add);

	protected:
		KeywordRef() = default;
		KeywordRef(const KeywordRef&) = delete;
		KeywordRef(KeywordRef&&) = delete;
		~KeywordRef() = default;

		KeywordRef& operator=(const KeywordRef&) = delete;
		KeywordRef& operator=(KeywordRef&&) = delete;
	};
}
