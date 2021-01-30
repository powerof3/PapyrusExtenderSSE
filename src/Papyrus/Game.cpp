#include "Papyrus/Game.h"


auto papyrusGame::GetActorsByProcessingLevel(VM*, StackID, RE::StaticFunctionTag*, std::int32_t a_level) -> std::vector<RE::Actor*>
{
	std::vector<RE::Actor*> vec;

	if (auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
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
					vec.push_back(actor);
				}
			}
		}
	}

	return vec;
}


auto papyrusGame::GetAllEnchantments(VM*, StackID, RE::StaticFunctionTag*, std::vector<RE::BGSKeyword*> a_keywords) -> std::vector<RE::EnchantmentItem*>
{
	std::vector<RE::EnchantmentItem*> vec;
	GetAllForms<RE::EnchantmentItem>(vec, a_keywords);
	return vec;
}


auto papyrusGame::GetAllRaces(VM*, StackID, RE::StaticFunctionTag*, std::vector<RE::BGSKeyword*> a_keywords) -> std::vector<RE::TESRace*>
{
	std::vector<RE::TESRace*> vec;
	GetAllForms<RE::TESRace>(vec, a_keywords);
	return vec;
}


auto papyrusGame::GetAllSpells(VM*, StackID, RE::StaticFunctionTag*, std::vector<RE::BGSKeyword*> a_keywords, bool a_playable) -> std::vector<RE::SpellItem*>
{
	std::vector<RE::SpellItem*> vec;

	if (a_playable) {
		if (auto dataHandler = RE::TESDataHandler::GetSingleton(); dataHandler) {
			for (const auto& book : dataHandler->GetFormArray<RE::TESObjectBOOK>()) {
				if (book && book->data.flags.all(RE::OBJ_BOOK::Flag::kTeachesSpell)) {
					auto spell = book->data.teaches.spell;
					if (!spell || !a_keywords.empty() && !spell->HasKeywords(a_keywords)) {
						continue;
					}
					vec.push_back(spell);
				}
			}
		}
	} else {
		GetAllForms<RE::SpellItem>(vec, a_keywords);
	}

	return vec;
}


auto papyrusGame::GetAllEnchantmentsInMod(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_name, std::vector<RE::BGSKeyword*> a_keywords) -> std::vector<RE::EnchantmentItem*>
{
	std::vector<RE::EnchantmentItem*> vec;

	if (a_name.empty()) {
		a_vm->TraceStack("Mod name is empty", a_stackID, Severity::kWarning);
		return vec;
	}

	if (auto dataHandler = RE::TESDataHandler::GetSingleton(); dataHandler) {
		const auto modInfo = dataHandler->LookupModByName(a_name);
		if (!modInfo) {
            const auto msg = a_name.c_str() + std::string(" is not loaded"sv);
			a_vm->TraceStack(msg.c_str(), a_stackID, Severity::kWarning);
		} else {
			GetAllFormsInMod<RE::EnchantmentItem>(modInfo, vec, a_keywords);
		}
	}

	return vec;
}


auto papyrusGame::GetAllRacesInMod(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_name, std::vector<RE::BGSKeyword*> a_keywords) -> std::vector<RE::TESRace*>
{
	std::vector<RE::TESRace*> vec;

	if (a_name.empty()) {
		a_vm->TraceStack("Mod name is empty", a_stackID, Severity::kWarning);
		return vec;
	}

	if (auto dataHandler = RE::TESDataHandler::GetSingleton(); dataHandler) {
		const auto modInfo = dataHandler->LookupModByName(a_name);
		if (!modInfo) {
            const auto msg = a_name.c_str() + std::string(" is not loaded"sv);
			a_vm->TraceStack(msg.c_str(), a_stackID, Severity::kWarning);
		} else {
			GetAllFormsInMod<RE::TESRace>(modInfo, vec, a_keywords);
		}
	}

	return vec;
}


auto papyrusGame::GetAllSpellsInMod(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_name, std::vector<RE::BGSKeyword*> a_keywords, bool a_playable) -> std::vector<RE::SpellItem*>
{
	std::vector<RE::SpellItem*> vec;

	if (a_name.empty()) {
		a_vm->TraceStack("Mod name is empty", a_stackID, Severity::kWarning);
		return vec;
	}

	if (auto dataHandler = RE::TESDataHandler::GetSingleton(); dataHandler) {
		const auto modInfo = dataHandler->LookupModByName(a_name);
		if (!modInfo) {
            const auto msg = a_name.c_str() + std::string(" is not loaded"sv);
			a_vm->TraceStack(msg.c_str(), a_stackID, Severity::kWarning);
			return vec;
		}
		if (a_playable) {
			for (const auto& book : dataHandler->GetFormArray<RE::TESObjectBOOK>()) {
				if (!book || !modInfo->IsFormInMod(book->formID) || book->data.flags.none(RE::OBJ_BOOK::Flag::kTeachesSpell)) {
					continue;
				}
				auto spell = book->data.teaches.spell;
				if (!spell || !a_keywords.empty() && !spell->HasKeywords(a_keywords)) {
					continue;
				}
				vec.push_back(spell);
			}
		} else {
			GetAllFormsInMod<RE::SpellItem>(modInfo, vec, a_keywords);
		}
	}

	return vec;
}


auto papyrusGame::GetAttachedCells(VM*, StackID, RE::StaticFunctionTag*) -> std::vector<RE::TESObjectCELL*>
{
	std::vector<RE::TESObjectCELL*> vec;

	if (const auto TES = RE::TES::GetSingleton(); TES) {
		auto cell = TES->interiorCell;
		if (cell) {
			vec.push_back(cell);
		} else {
            const auto gridCells = TES->gridCells;
			if (gridCells) {
                const auto gridLength = gridCells->length;
				if (gridLength > 0) {
					std::uint32_t x;
					std::uint32_t y;
					for (x = 0, y = 0; x < gridLength && y < gridLength; x++, y++) {
						cell = gridCells->GetCell(x, y);
						if (cell && cell->IsAttached()) {
							vec.push_back(cell);
						}
					}
				}
			}
		}
		if (vec.empty()) {
			auto worldSpace = TES->worldSpace;
			if (worldSpace) {
				cell = worldSpace->GetOrCreateSkyCell();
				if (cell) {
					vec.push_back(cell);
				}
			}
		}
	}

	return vec;
}


auto papyrusGame::GetGameSettingBool(VM*, StackID, RE::StaticFunctionTag*, RE::BSFixedString a_gamesetting) -> std::int32_t
{
	if (a_gamesetting.empty()) {
		return -1;
	}

	if (auto gmstCollection = RE::GameSettingCollection::GetSingleton(); gmstCollection) {
        const auto gmst = gmstCollection->GetSetting(a_gamesetting.c_str());
		if (gmst && gmst->GetType() == RE::Setting::Type::kBool) {
			return gmst->GetBool();
		}
	}

	return -1;
}


auto papyrusGame::GetLocalGravity(VM*, StackID, RE::StaticFunctionTag*) -> std::vector<float>
{
	std::vector<float> vec(3, 0.0f);

	if (const auto player = RE::PlayerCharacter::GetSingleton(); player) {
        const auto cell = player->GetParentCell();
		if (cell) {
			auto world = cell->GetbhkWorld();
			if (world) {
				RE::BSReadLockGuard lock(world->worldLock);
                const auto havokWorld = world->GetWorld2();
				if (havokWorld) {
					float gravity[4];
					_mm_store_ps(gravity, havokWorld->gravity.quad);
					for (std::size_t i = 0; i < 3; ++i) {
						vec[i] = gravity[i];
					}
				}
			}
		}
	}

	return vec;
}


auto papyrusGame::GetNumActorsInHigh(VM*, StackID, RE::StaticFunctionTag*) -> std::int32_t
{
    const auto processLists = RE::ProcessLists::GetSingleton();
	return processLists ? processLists->numberHighActors : -1;
}


auto papyrusGame::IsPluginFound(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_name) -> bool
{
	if (a_name.empty()) {
		a_vm->TraceStack("Mod name is empty", a_stackID, Severity::kWarning);
	}

	if (auto dataHandler = RE::TESDataHandler::GetSingleton(); dataHandler) {
        const auto modInfo = dataHandler->LookupModByName(a_name);
		if (modInfo) {
			return true;
		}
	}

	return false;
}


auto papyrusGame::IsSurvivalModeActive(VM*, StackID, RE::StaticFunctionTag*) -> bool
{
	if (auto manager = RE::BGSDefaultObjectManager::GetSingleton(); manager) {
        const auto survivalGlobal = manager->GetObject<RE::TESGlobal>(RE::DEFAULT_OBJECTS::kSurvivalModeToggle);
		return survivalGlobal ? survivalGlobal->value == 1.0f : false;
	}
	return false;
}


void papyrusGame::SetLocalGravity(VM*, StackID, RE::StaticFunctionTag*, float a_x, float a_y, float a_z)
{
	if (const auto player = RE::PlayerCharacter::GetSingleton(); player) {
        const auto cell = player->GetParentCell();
		if (cell) {
			auto world = cell->GetbhkWorld();
			if (world) {
				RE::BSWriteLockGuard lock(world->worldLock);
				auto havokWorld = world->GetWorld2();
				if (havokWorld) {
					havokWorld->gravity = RE::hkVector4(a_x, a_y, a_z, 0.0f);
				}
			}
		}
	}
}


auto papyrusGame::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusGame - couldn't get VMState"sv);
		return false;
	}

	auto constexpr Functions = "PO3_SKSEFunctions"sv;

	a_vm->RegisterFunction("GetActorsByProcessingLevel"sv, Functions, GetActorsByProcessingLevel);

	a_vm->RegisterFunction("GetAllEnchantments"sv, Functions, GetAllEnchantments);

	a_vm->RegisterFunction("GetAllRaces"sv, Functions, GetAllRaces);

	a_vm->RegisterFunction("GetAllSpells"sv, Functions, GetAllSpells);

	a_vm->RegisterFunction("GetAllEnchantmentsInMod"sv, Functions, GetAllEnchantmentsInMod);

	a_vm->RegisterFunction("GetAllRacesInMod"sv, Functions, GetAllRacesInMod);

	a_vm->RegisterFunction("GetAllSpellsInMod"sv, Functions, GetAllSpellsInMod);

	a_vm->RegisterFunction("GetAttachedCells"sv, Functions, GetAttachedCells);

	a_vm->RegisterFunction("GetGameSettingBool"sv, Functions, GetGameSettingBool);

	a_vm->RegisterFunction("GetLocalGravity"sv, Functions, GetLocalGravity);

	a_vm->RegisterFunction("GetNumActorsInHigh"sv, Functions, GetNumActorsInHigh);

	a_vm->RegisterFunction("IsSurvivalModeActive"sv, Functions, IsSurvivalModeActive);

	a_vm->RegisterFunction("IsPluginFound"sv, Functions, IsPluginFound);

	a_vm->RegisterFunction("SetLocalGravity"sv, Functions, SetLocalGravity);

	return true;
}
