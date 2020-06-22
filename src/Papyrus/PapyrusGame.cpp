#include "Papyrus/PapyrusGame.h"


std::vector<RE::Actor*> papyrusGame::GetActorsByProcessingLevel(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, SInt32 a_level)
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


std::vector<RE::TESRace*> papyrusGame::GetAllRaces(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, std::vector<RE::BGSKeyword*> a_keywords)
{
	std::vector<RE::TESRace*> vec;

	auto dataHandler = RE::TESDataHandler::GetSingleton();
	if (dataHandler) {
		for (auto& race : dataHandler->GetFormArray<RE::TESRace>()) {
			if (!race || !race->HasKeywords(a_keywords)) {
				continue;
			}
			vec.push_back(race);
		}
	}

	return vec;
}


std::vector<RE::SpellItem*> papyrusGame::GetAllSpells(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, std::vector<RE::BGSKeyword*> a_keywords, bool a_playable)
{
	std::vector<RE::SpellItem*> vec;
	
	auto dataHandler = RE::TESDataHandler::GetSingleton();
	if (dataHandler) {
		if (a_playable) {
			for (auto& book : dataHandler->GetFormArray<RE::TESObjectBOOK>()) {
				if (book) {
					auto spell = book->data.teaches.spell;
					if (!spell || !spell->HasKeywords(a_keywords)) {
						continue;
					}
					vec.push_back(spell);
				}
			}
		}
		else {
			for (auto& spell : dataHandler->GetFormArray<RE::SpellItem>()) {
				if (!spell || !spell->HasKeywords(a_keywords)) {
					continue;
				}
				vec.push_back(spell);
			}
		}
	}

	return vec;
}


std::vector<RE::TESRace*> papyrusGame::GetAllRacesInMod(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_name, std::vector<RE::BGSKeyword*> a_keywords)
{
	std::vector<RE::TESRace*> vec;

	if (!a_name.empty()) {
		auto dataHandler = RE::TESDataHandler::GetSingleton();
		if (dataHandler) {
			const auto modInfo = dataHandler->LookupModByName(a_name.c_str());
			if (!modInfo || !modInfo->IsLoaded()) {
				a_vm->TraceStack("Mod is not loaded", a_stackID, Severity::kWarning);
				return vec;
			}
			for (auto& race : dataHandler->GetFormArray<RE::TESRace>()) {
				if (!race || !modInfo->IsFormInMod(race->formID) || !race->HasKeywords(a_keywords)) {
					continue;
				}
				vec.push_back(race);
			}
		}
	}

	return vec;
}


std::vector<RE::SpellItem*> papyrusGame::GetAllSpellsInMod(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_name, std::vector<RE::BGSKeyword*> a_keywords, bool a_playable)
{
	std::vector<RE::SpellItem*> vec;

	if (!a_name.empty()) {
		auto dataHandler = RE::TESDataHandler::GetSingleton();
		if (dataHandler) {
			const auto modInfo = dataHandler->LookupModByName(a_name.c_str());
			if (!modInfo || !modInfo->IsLoaded()) {
				a_vm->TraceStack("Mod is not loaded", a_stackID, Severity::kWarning);
				return vec;
			}
			if (a_playable) {
				for (auto& book : dataHandler->GetFormArray<RE::TESObjectBOOK>()) {
					if (!book || !modInfo->IsFormInMod(book->formID)) {
						continue;
					}
					auto spell = book->data.teaches.spell;
					if (!spell || !spell->HasKeywords(a_keywords)) {
						continue;
					}
					vec.push_back(spell);
				}
			}
			else {
				for (auto& spell : dataHandler->GetFormArray<RE::SpellItem>()) {
					if (!spell || !modInfo->IsFormInMod(spell->formID) || !spell->HasKeywords(a_keywords)) {
						continue;
					}
					vec.push_back(spell);
				}
			}
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
		}
		else {
			auto gridCellArray = TES->gridCellArray;
			if (gridCellArray) {
				auto gridLength = gridCellArray->length;
				if (gridLength > 0) {
					UInt32 x = 0;
					UInt32 y = 0;
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


SInt32 papyrusGame::GetNumActorsInHigh(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*)
{
	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		return processLists->numberHighActors;
	}
	return -1;
}


bool papyrusGame::IsPluginFound(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_name)
{
	if (!a_name.empty()) {
		auto dataHandler = RE::TESDataHandler::GetSingleton();
		if (dataHandler) {
			const auto modInfo = dataHandler->LookupModByName(a_name.c_str());
			if (modInfo) {
				return modInfo->IsLoaded();
			}
		}
	}
	else {
		a_vm->TraceStack("Mod name is empty", a_stackID, Severity::kWarning);
	}
	return false;
}


bool papyrusGame::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		_MESSAGE("papyrusGame - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("GetActorsByProcessingLevel", "PO3_SKSEFunctions", GetActorsByProcessingLevel);

	a_vm->RegisterFunction("GetAllRaces", "PO3_SKSEFunctions", GetAllRaces);

	a_vm->RegisterFunction("GetAllSpells", "PO3_SKSEFunctions", GetAllSpells);

	a_vm->RegisterFunction("GetAllRacesInMod", "PO3_SKSEFunctions", GetAllRacesInMod);
	
	a_vm->RegisterFunction("GetAllSpellsInMod", "PO3_SKSEFunctions", GetAllSpellsInMod);

	a_vm->RegisterFunction("GetAttachedCells", "PO3_SKSEFunctions", GetAttachedCells);

	a_vm->RegisterFunction("GetNumActorsInHigh", "PO3_SKSEFunctions", GetNumActorsInHigh);

	a_vm->RegisterFunction("IsPluginFound", "PO3_SKSEFunctions", IsPluginFound);
	
	return true;
}
