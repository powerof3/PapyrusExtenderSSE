#include "Papyrus/Debug.h"


void papyrusDebug::GivePlayerSpellBook(RE::StaticFunctionTag*)
{
	auto player = RE::PlayerCharacter::GetSingleton();
	auto dataHandler = RE::TESDataHandler::GetSingleton();

	if (player && dataHandler) {
		for (const auto& book : dataHandler->GetFormArray<RE::TESObjectBOOK>()) {
			if (book && book->data.flags.all(RE::OBJ_BOOK::Flag::kTeachesSpell)) {
				auto spell = book->data.teaches.spell;
				if (spell && !spell->fullName.empty()) {
					auto mod = spell->GetDescriptionOwnerFile();
					if (mod) {
						auto modName = "[" + std::string(mod->fileName).substr(0, 4) + "] ";
						spell->fullName = modName + spell->fullName.c_str();
					}
					player->AddSpell(spell);
				}
			}
		}
	}
}


auto papyrusDebug::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusDebug - couldn't get VMState"sv);
		return false;
	}

	a_vm->RegisterFunction("GivePlayerSpellBook"sv, "PO3_SKSEFunctions", GivePlayerSpellBook);

	return true;
}
