#include "Papyrus/PapyrusGame.h"


std::vector<RE::Actor*> papyrusGame::GetActorsByProcessingLevel(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, std::int32_t a_level)
{
	std::vector<RE::Actor*> vec;

	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
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
			arr = nullptr;
			break;
		}

		if (arr) {
			for (auto& actorHandle : *arr) {
				auto actorPtr = actorHandle.get();
				if (actorPtr.get()) {
					vec.push_back(actorPtr.get());
				}
			}
		}
	}

	return vec;
}


std::vector<RE::EnchantmentItem*> papyrusGame::GetAllEnchantments(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, std::vector<RE::BGSKeyword*> a_keywords)
{
	std::vector<RE::EnchantmentItem*> vec;
	GetAllForms<RE::EnchantmentItem>(vec, a_keywords);
	return vec;
}


std::vector<RE::TESRace*> papyrusGame::GetAllRaces(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, std::vector<RE::BGSKeyword*> a_keywords)
{
	std::vector<RE::TESRace*> vec;
	GetAllForms<RE::TESRace>(vec, a_keywords);
	return vec;
}


std::vector<RE::SpellItem*> papyrusGame::GetAllSpells(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, std::vector<RE::BGSKeyword*> a_keywords, bool a_playable)
{
	std::vector<RE::SpellItem*> vec;

	if (a_playable) {
		auto dataHandler = RE::TESDataHandler::GetSingleton();
		if (dataHandler) {
			for (auto& book : dataHandler->GetFormArray<RE::TESObjectBOOK>()) {
				if (book) {
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


std::vector<RE::EnchantmentItem*> papyrusGame::GetAllEnchantmentsInMod(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_name, std::vector<RE::BGSKeyword*> a_keywords)
{
	std::vector<RE::EnchantmentItem*> vec;

	if (a_name.empty()) {
		a_vm->TraceStack("Mod name is empty", a_stackID, Severity::kWarning);
		return vec;
	}

	auto dataHandler = RE::TESDataHandler::GetSingleton();
	if (dataHandler) {
		const auto modInfo = dataHandler->LookupModByName(a_name.c_str());
		if (!modInfo) {
			auto msg = a_name.c_str() + std::string(" is not loaded");
			a_vm->TraceStack(msg.c_str(), a_stackID, Severity::kWarning);
			return vec;
		}
		GetAllFormsInMod<RE::EnchantmentItem>(modInfo, vec, a_keywords);
	}

	return vec;
}


std::vector<RE::TESRace*> papyrusGame::GetAllRacesInMod(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_name, std::vector<RE::BGSKeyword*> a_keywords)
{
	std::vector<RE::TESRace*> vec;

	if (a_name.empty()) {
		a_vm->TraceStack("Mod name is empty", a_stackID, Severity::kWarning);
		return vec;
	}

	auto dataHandler = RE::TESDataHandler::GetSingleton();
	if (dataHandler) {
		const auto modInfo = dataHandler->LookupModByName(a_name.c_str());
		if (!modInfo) {
			auto msg = a_name.c_str() + std::string(" is not loaded");
			a_vm->TraceStack(msg.c_str(), a_stackID, Severity::kWarning);
			return vec;
		}
		GetAllFormsInMod<RE::TESRace>(modInfo, vec, a_keywords);
	}

	return vec;
}


std::vector<RE::SpellItem*> papyrusGame::GetAllSpellsInMod(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_name, std::vector<RE::BGSKeyword*> a_keywords, bool a_playable)
{
	std::vector<RE::SpellItem*> vec;

	if (a_name.empty()) {
		a_vm->TraceStack("Mod name is empty", a_stackID, Severity::kWarning);
		return vec;
	}

	auto dataHandler = RE::TESDataHandler::GetSingleton();
	if (dataHandler) {
		const auto modInfo = dataHandler->LookupModByName(a_name.c_str());
		if (!modInfo) {
			auto msg = a_name.c_str() + std::string(" is not loaded");
			a_vm->TraceStack(msg.c_str(), a_stackID, Severity::kWarning);
			return vec;
		}
		if (a_playable) {
			for (const auto& book : dataHandler->GetFormArray<RE::TESObjectBOOK>()) {
				if (!book || !modInfo->IsFormInMod(book->formID)) {
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


std::vector<RE::TESObjectCELL*> papyrusGame::GetAttachedCells(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*)
{
	std::vector<RE::TESObjectCELL*> vec;

	auto TES = RE::TES::GetSingleton();
	if (TES) {
		auto cell = TES->currentInteriorCell;
		if (cell) {
			vec.push_back(cell);
		} else {
			auto gridCellArray = TES->gridCellArray;
			if (gridCellArray) {
				auto gridLength = gridCellArray->length;
				if (gridLength > 0) {
					std::uint32_t x = 0;
					std::uint32_t y = 0;
					for (x = 0, y = 0; (x < gridLength && y < gridLength); x++, y++) {
						cell = gridCellArray->GetCell(x, y);
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


std::vector<float> papyrusGame::GetLocalGravity(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*)
{
	std::vector<float> vec(3, 0.0f);

	auto player = RE::PlayerCharacter::GetSingleton();
	if (player) {
		auto cell = player->GetParentCell();
		if (cell) {
			auto world = cell->GetHavokWorld();
			if (world) {
				world->LockWorldForRead();
				auto havokWorld = world->GetWorld2();
				if (havokWorld) {
					float gravity[4];
					_mm_store_ps(gravity, havokWorld->gravity.quad);
					for (std::size_t i = 0; i < 3; ++i) {
						vec[i] = gravity[i];
					}
				}
				world->UnlockWorldForRead();
			}
		}
	}

	return vec;
}


std::int32_t papyrusGame::GetNumActorsInHigh(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*)
{
	auto processLists = RE::ProcessLists::GetSingleton();
	return processLists ? processLists->numberHighActors : -1;
}


bool papyrusGame::IsPluginFound(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_name)
{
	if (a_name.empty()) {
		a_vm->TraceStack("Mod name is empty", a_stackID, Severity::kWarning);
	}

	auto dataHandler = RE::TESDataHandler::GetSingleton();
	if (dataHandler) {
		auto modInfo = dataHandler->LookupLoadedModByName(a_name.c_str());
		if (!modInfo) {
			modInfo = dataHandler->LookupLoadedLightModByName(a_name.c_str());
		}
		if (modInfo) {
			return true;
		}
	}

	return false;
}


void papyrusGame::SetLocalGravity(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, float a_x, float a_y, float a_z)
{
	auto player = RE::PlayerCharacter::GetSingleton();
	if (player) {
		auto cell = player->GetParentCell();
		if (cell) {
			auto world = cell->GetHavokWorld();
			if (world) {
				world->LockWorldForWrite();
				auto havokWorld = world->GetWorld2();
				if (havokWorld) {
					havokWorld->gravity = RE::hkVector4(a_x, a_y, a_z, 0.0f);
				}
				world->UnlockWorldForWrite();
			}
		}
	}
}


bool papyrusGame::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("papyrusGame - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("GetActorsByProcessingLevel", "PO3_SKSEFunctions", GetActorsByProcessingLevel);

	a_vm->RegisterFunction("GetAllEnchantments", "PO3_SKSEFunctions", GetAllEnchantments);
	
	a_vm->RegisterFunction("GetAllRaces", "PO3_SKSEFunctions", GetAllRaces);

	a_vm->RegisterFunction("GetAllSpells", "PO3_SKSEFunctions", GetAllSpells);

	a_vm->RegisterFunction("GetAllEnchantmentsInMod", "PO3_SKSEFunctions", GetAllEnchantmentsInMod);
	
	a_vm->RegisterFunction("GetAllRacesInMod", "PO3_SKSEFunctions", GetAllRacesInMod);

	a_vm->RegisterFunction("GetAllSpellsInMod", "PO3_SKSEFunctions", GetAllSpellsInMod);

	a_vm->RegisterFunction("GetAttachedCells", "PO3_SKSEFunctions", GetAttachedCells);

	a_vm->RegisterFunction("GetLocalGravity", "PO3_SKSEFunctions", GetLocalGravity);

	a_vm->RegisterFunction("GetNumActorsInHigh", "PO3_SKSEFunctions", GetNumActorsInHigh);

	a_vm->RegisterFunction("IsPluginFound", "PO3_SKSEFunctions", IsPluginFound);

	a_vm->RegisterFunction("SetLocalGravity", "PO3_SKSEFunctions", SetLocalGravity);

	return true;
}
