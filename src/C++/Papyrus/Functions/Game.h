#pragma once

namespace Papyrus::Game
{
	struct forms
	{
		template <class T>
		static std::vector<T*> get_all(const std::vector<RE::BGSKeyword*>& a_keywords)
		{
			std::vector<T*> result;

			if (const auto dataHandler = RE::TESDataHandler::GetSingleton(); dataHandler) {
				for (const auto& form : dataHandler->GetFormArray<T>()) {
					if (!form || !a_keywords.empty() && !form->HasKeywords(a_keywords, false)) {
						continue;
					}
					result.push_back(form);
				}
			}

			return result;
		}

		template <class T>
		static std::vector<T*> get_in_mod(const RE::TESFile* a_modInfo, const std::vector<RE::BGSKeyword*>& a_keywords)
		{
			std::vector<T*> result;

			if (const auto dataHandler = RE::TESDataHandler::GetSingleton(); dataHandler) {
				for (const auto& form : dataHandler->GetFormArray<T>()) {
					if (!form || !a_modInfo->IsFormInMod(form->formID) || !a_keywords.empty() && !form->HasKeywords(a_keywords, false)) {
						continue;
					}
					result.push_back(form);
				}
			}

			return result;
		}
	};

	inline std::vector<RE::Actor*> GetActorsByProcessingLevel(RE::StaticFunctionTag*, std::int32_t a_level)
	{
		std::vector<RE::Actor*> result;

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			RE::BSTArray<RE::ActorHandle>* arr = nullptr;

			switch (a_level) {
			case 0:
				arr = &processLists->highActorHandles;
				break;
			case 1:
				arr = &processLists->middleHighActorHandles;
				break;
			case 2:
				arr = &processLists->middleLowActorHandles;
				break;
			case 3:
				arr = &processLists->lowActorHandles;
				break;
			default:
				break;
			}

			if (arr) {
				for (auto& actorHandle : *arr) {
					auto actorPtr = actorHandle.get();
					auto actor = actorPtr.get();
					if (actor) {
						result.push_back(actor);
					}
				}
			}
		}

		return result;
	}

	inline std::vector<RE::EnchantmentItem*> GetAllEnchantments(RE::StaticFunctionTag*, std::vector<RE::BGSKeyword*> a_keywords)
	{
		return forms::get_all<RE::EnchantmentItem>(a_keywords);
	}

	inline std::vector<RE::TESForm*> GetAllForms(RE::StaticFunctionTag*, std::int32_t a_formType, std::vector<RE::BGSKeyword*> a_keywords)
	{
		std::vector<RE::TESForm*> result;

		const auto formType = static_cast<RE::FormType>(a_formType);

		if (const auto dataHandler = RE::TESDataHandler::GetSingleton(); dataHandler) {
			for (const auto& form : dataHandler->GetFormArray(formType)) {
				if (!form || !a_keywords.empty() && !form->HasKeywords(a_keywords, false)) {
					continue;
				}
				result.push_back(form);
			}
		}

		return result;
	}

	inline std::vector<RE::TESRace*> GetAllRaces(RE::StaticFunctionTag*, std::vector<RE::BGSKeyword*> a_keywords)
	{
		return forms::get_all<RE::TESRace>(a_keywords);
	}

	inline std::vector<RE::SpellItem*> GetAllSpells(RE::StaticFunctionTag*, std::vector<RE::BGSKeyword*> a_keywords, bool a_playable)
	{
		if (a_playable) {
			std::vector<RE::SpellItem*> result;

			if (const auto dataHandler = RE::TESDataHandler::GetSingleton(); dataHandler) {
				for (const auto& book : dataHandler->GetFormArray<RE::TESObjectBOOK>()) {
					if (book && book->data.flags.all(RE::OBJ_BOOK::Flag::kTeachesSpell)) {
						const auto spell = book->data.teaches.spell;
						if (!spell || !a_keywords.empty() && !spell->HasKeywords(a_keywords, false)) {
							continue;
						}
						result.push_back(spell);
					}
				}
			}

			return result;
		}

		return forms::get_all<RE::SpellItem>(a_keywords);
	}

	inline std::vector<RE::EnchantmentItem*> GetAllEnchantmentsInMod(RE::StaticFunctionTag*, RE::BSFixedString a_name, std::vector<RE::BGSKeyword*> a_keywords)
	{
		const auto dataHandler = RE::TESDataHandler::GetSingleton();
		const auto modInfo = dataHandler ? dataHandler->LookupModByName(a_name) : nullptr;

		return modInfo ?
                   forms::get_in_mod<RE::EnchantmentItem>(modInfo, a_keywords) :
                   std::vector<RE::EnchantmentItem*>();
	}

	inline std::vector<RE::TESForm*> GetAllFormsInMod(RE::StaticFunctionTag*, RE::BSFixedString a_name, std::int32_t a_formType, std::vector<RE::BGSKeyword*> a_keywords)
	{
		const auto dataHandler = RE::TESDataHandler::GetSingleton();
		const auto modInfo = dataHandler ? dataHandler->LookupModByName(a_name) : nullptr;

		std::vector<RE::TESForm*> result;

		if (modInfo) {
			const auto formType = static_cast<RE::FormType>(a_formType);

			for (const auto& form : dataHandler->GetFormArray(formType)) {
				if (!form || !modInfo->IsFormInMod(form->formID) || !a_keywords.empty() && !form->HasKeywords(a_keywords, false)) {
					continue;
				}
				result.push_back(form);
			}
		}

		return result;
	}

	inline std::vector<RE::TESRace*> GetAllRacesInMod(RE::StaticFunctionTag*, RE::BSFixedString a_name, std::vector<RE::BGSKeyword*> a_keywords)
	{
		const auto dataHandler = RE::TESDataHandler::GetSingleton();
		const auto modInfo = dataHandler ? dataHandler->LookupModByName(a_name) : nullptr;

		return modInfo ?
                   forms::get_in_mod<RE::TESRace>(modInfo, a_keywords) :
                   std::vector<RE::TESRace*>();
	}

	inline std::vector<RE::SpellItem*> GetAllSpellsInMod(RE::StaticFunctionTag*,
		RE::BSFixedString a_name,
		std::vector<RE::BGSKeyword*> a_keywords,
		bool a_playable)
	{
		const auto dataHandler = RE::TESDataHandler::GetSingleton();
		const auto modInfo = dataHandler ? dataHandler->LookupModByName(a_name) : nullptr;

		if (!modInfo) {
			return std::vector<RE::SpellItem*>();
		}

		if (a_playable) {
			std::vector<RE::SpellItem*> result;

			for (const auto& book : dataHandler->GetFormArray<RE::TESObjectBOOK>()) {
				if (!book || !modInfo->IsFormInMod(book->formID) || book->data.flags.none(RE::OBJ_BOOK::Flag::kTeachesSpell)) {
					continue;
				}
				const auto spell = book->data.teaches.spell;
				if (!spell || !a_keywords.empty() && !spell->HasKeywords(a_keywords, false)) {
					continue;
				}
				result.push_back(spell);
			}

			return result;
		}

		return forms::get_in_mod<RE::SpellItem>(modInfo, a_keywords);
	}

	inline std::vector<RE::TESObjectCELL*> GetAttachedCells(RE::StaticFunctionTag*)
	{
		std::vector<RE::TESObjectCELL*> result;

		if (const auto TES = RE::TES::GetSingleton(); TES) {
			auto cell = TES->interiorCell;
			if (cell) {
				result.push_back(cell);
			} else {
				const auto gridCells = TES->gridCells;
				const auto gridLength = gridCells ? gridCells->length : 0;
				if (gridLength > 0) {
					std::uint32_t x = 0;
					do {
						std::uint32_t y = 0;
						do {
							cell = gridCells->GetCell(x, y);
							if (cell && cell->IsAttached()) {
								result.push_back(cell);
							}
							++y;
						} while (y < gridLength);
						++x;
					} while (x < gridLength);
				}
			}
		}

		return result;
	}

	inline RE::TESForm* GetFormFromEditorID(RE::StaticFunctionTag*, RE::BSFixedString a_editorID)
	{
		return RE::TESForm::LookupByEditorID(a_editorID);
	}
	
	inline std::int32_t GetGameSettingBool(RE::StaticFunctionTag*, RE::BSFixedString a_gamesetting)
	{
		if (a_gamesetting.empty()) {
			return -1;
		}

		const auto gmstCollection = RE::GameSettingCollection::GetSingleton();
		const auto gmst = gmstCollection ? gmstCollection->GetSetting(a_gamesetting.c_str()) : nullptr;

		return gmst && gmst->GetType() == RE::Setting::Type::kBool ? gmst->GetBool() : -1;
	}

	inline std::vector<float> GetLocalGravity(RE::StaticFunctionTag*)
	{
		std::vector<float> result(3, 0.0f);

		const auto player = RE::PlayerCharacter::GetSingleton();
		const auto cell = player ? player->GetParentCell() : nullptr;
		const auto world = cell ? cell->GetbhkWorld() : nullptr;

		if (world) {
			RE::BSReadLockGuard locker(world->worldLock);

			if (const auto havokWorld = world->GetWorld2(); havokWorld) {
				std::array<float, 4> gravity{};
				_mm_store_ps(gravity.data(), havokWorld->gravity.quad);
				for (std::size_t i = 0; i < 3; ++i) {
					result[i] = gravity[i];
				}
			}
		}

		return result;
	}

	inline std::int32_t GetNumActorsInHigh(RE::StaticFunctionTag*)
	{
		const auto processLists = RE::ProcessLists::GetSingleton();
		return processLists ? processLists->numberHighActors : -1;
	}

	inline std::vector<std::int32_t> GetPapyrusExtenderVersion(RE::StaticFunctionTag*)
	{
		return { Version::MAJOR, Version::MINOR, Version::PATCH };
	}

	inline std::vector<RE::Actor*> GetPlayerFollowers(RE::StaticFunctionTag*)
	{
		std::vector<RE::Actor*> result;

		const auto xFollowers = RE::PlayerCharacter::GetSingleton()->extraList.GetByType<RE::ExtraFollower>();
		if (xFollowers) {
			for (auto& [actorHandle, distance] : xFollowers->actorFollowers) {
				auto actor = actorHandle.get();
				if (actor) {
					result.push_back(actor.get());
				}
			}
		}

		return result;
	}

	inline bool IsPluginFound(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_name)
	{
		if (a_name.empty()) {
			a_vm->TraceStack("Mod name is empty", a_stackID);
			return false;
		}

		const auto dataHandler = RE::TESDataHandler::GetSingleton();
		return dataHandler && dataHandler->LookupModByName(a_name);
	}

	inline bool IsSurvivalModeActive(RE::StaticFunctionTag*)
	{
		const auto manager = RE::BGSDefaultObjectManager::GetSingleton();
		const auto survivalGlobal = manager ? manager->GetObject<RE::TESGlobal>(RE::DEFAULT_OBJECTS::kSurvivalModeToggle) : nullptr;

		return survivalGlobal && survivalGlobal->value == 1.0f;
	}

	inline void SetLocalGravity(RE::StaticFunctionTag*,
		float a_x,
		float a_y,
		float a_z)
	{
		const auto player = RE::PlayerCharacter::GetSingleton();
		const auto cell = player ? player->GetParentCell() : nullptr;
		const auto world = cell ? cell->GetbhkWorld() : nullptr;

		if (world) {
			RE::BSWriteLockGuard locker(world->worldLock);

			if (const auto havokWorld = world->GetWorld2(); havokWorld) {
				havokWorld->gravity = RE::hkVector4(a_x, a_y, a_z, 0.0f);
			}
		}
	}

	inline void Bind(VM& a_vm)
	{
		BIND(GetActorsByProcessingLevel);
		BIND(GetAllEnchantments);
		BIND(GetAllForms);
		BIND(GetAllRaces);
		BIND(GetAllSpells);
		BIND(GetAllEnchantmentsInMod);
		BIND(GetAllFormsInMod);
		BIND(GetAllRacesInMod);
		BIND(GetAllSpellsInMod);
		BIND(GetAttachedCells);
		BIND(GetFormFromEditorID);
		BIND(GetGameSettingBool);
		BIND(GetLocalGravity);
		BIND(GetNumActorsInHigh);
		BIND(GetPapyrusExtenderVersion, true);
		BIND(GetPlayerFollowers);
		BIND(IsSurvivalModeActive, true);
		BIND(IsPluginFound, true);
		BIND(SetLocalGravity);

		logger::info("Registered game functions"sv);
	}
}
