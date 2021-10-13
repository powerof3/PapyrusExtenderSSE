#pragma once

#include "Registration/FormSetPair.h"

namespace Form
{
	enum : std::uint32_t
	{
		kRemove = 0,
		kAdd = 1
	};
}

class KeywordManager final : public FormDataSetPair<RE::TESForm, RE::BGSKeyword>
{
public:
	static KeywordManager* GetSingleton()
	{
		static KeywordManager singleton;
		return &singleton;
	}

private:
	bool Process(RE::TESForm* a_form, RE::BGSKeyword* a_data, std::uint32_t a_index) override
	{
		if (const auto keywordForm = a_form->As<RE::BGSKeywordForm>(); keywordForm) {
			return a_index == Form::kAdd ?
                       keywordForm->AddKeyword(a_data) :
                       keywordForm->RemoveKeyword(a_data);
		}

		return false;
	}

protected:
	KeywordManager() = default;
	KeywordManager(const KeywordManager&) = delete;
	KeywordManager(KeywordManager&&) = delete;
	~KeywordManager() = default;

	KeywordManager& operator=(const KeywordManager&) = delete;
	KeywordManager& operator=(KeywordManager&&) = delete;
};

class PerkManager final : public FormDataSetPair<RE::Actor, RE::BGSPerk>
{
public:
	static PerkManager* GetSingleton()
	{
		static PerkManager singleton;
		return &singleton;
	}

private:
	bool Process(RE::Actor* a_form, RE::BGSPerk* a_data, std::uint32_t a_index) override
	{
		bool success = false;

		if (const auto actorbase = a_form->GetActorBase(); actorbase) {
			success = a_index == Form::kAdd ?
                          actorbase->AddPerk(a_data, 0) :
                          actorbase->RemovePerk(a_data);
			if (success) {
				for (auto& perkEntry : a_data->perkEntries) {
					if (perkEntry) {
						a_index == Form::kAdd ?
                            perkEntry->ApplyPerkEntry(a_form) :
                            perkEntry->RemovePerkEntry(a_form);
					}
				}				
				a_form->OnArmorActorValueChanged();		
				auto invChanges = a_form->GetInventoryChanges();
				if (invChanges) {
					invChanges->armorWeight = invChanges->totalWeight;
					invChanges->totalWeight = -1.0f;
					a_form->equippedWeight = -1.0f;
				}
			}
		}

		return success;
	}

	protected:
		PerkManager() = default;
		PerkManager(const PerkManager&) = delete;
		PerkManager(PerkManager &&) = delete;
		~PerkManager() = default;

		PerkManager& operator=(const PerkManager&) = delete;
		PerkManager& operator=(PerkManager&&) = delete;
	};

	namespace Detection
	{
		enum : std::uint32_t
		{
			kAlert = 0,
			kHide = 1,
		};

		//target
		class TargetManager final : public FormSetPair<RE::Actor>
		{
		public:
			static TargetManager* GetSingleton()
			{
				static TargetManager singleton;
				return &singleton;
			}

		protected:
			TargetManager() = default;
			TargetManager(const TargetManager&) = delete;
			TargetManager(TargetManager&&) = delete;
			~TargetManager() = default;

			TargetManager& operator=(const TargetManager&) = delete;
			TargetManager& operator=(TargetManager&&) = delete;
		};

		//searcher
		class SourceManager final : public FormSetPair<RE::Actor>
		{
		public:
			static SourceManager* GetSingleton()
			{
				static SourceManager singleton;
				return &singleton;
			}

		protected:
			SourceManager() = default;
			SourceManager(const SourceManager&) = delete;
			SourceManager(SourceManager&&) = delete;
			~SourceManager() = default;

			SourceManager& operator=(const SourceManager&) = delete;
			SourceManager& operator=(SourceManager&&) = delete;
		};
	}
