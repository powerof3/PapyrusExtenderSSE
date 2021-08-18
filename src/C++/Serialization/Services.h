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

class KeywordManager : public FormDataSetPair<RE::TESForm, RE::BGSKeyword>
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
		const auto keywordForm = a_form->As<RE::BGSKeywordForm>();
		if (keywordForm) {
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

class PerkManager : public FormDataSetPair<RE::Actor, RE::BGSPerk>
{
public:
	static PerkManager* GetSingleton()
	{
		static PerkManager singleton;
		return &singleton;
	}

private:
	bool Process(RE::Actor* a_form, RE::BGSPerk* a_data, std::uint32_t a_set) override
	{
		bool success = false;

		const auto actorbase = a_form->GetActorBase();
		if (actorbase) {
			success = a_set == Form::kAdd ?
                          actorbase->AddPerk(a_data, 1) :
                          actorbase->RemovePerk(a_data);
			if (success) {
				a_form->ApplyPerksFromBase();
			}
		}

		return success;
	}

protected:
	PerkManager() = default;
	PerkManager(const PerkManager&) = delete;
	PerkManager(PerkManager&&) = delete;
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
	class TargetManager : public FormSetPair<RE::Actor>
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
	class SourceManager : public FormSetPair<RE::Actor>
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
