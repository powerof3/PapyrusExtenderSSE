#include "Papyrus/PapyrusDebug.h"


void papyrusDebug::GivePlayerSpellBook(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*)
{
	auto player = RE::PlayerCharacter::GetSingleton();
	auto dataHandler = RE::TESDataHandler::GetSingleton();

	if (player && dataHandler) {
		for (const auto& book : dataHandler->GetFormArray<RE::TESObjectBOOK>()) {
			if (book && book->HasKeywordString("VendorItemSpellTome")) {  //fixes crash
				auto spell = book->data.teaches.spell;
				if (spell) {
					if (!spell->fullName.empty()) {
						auto mod = spell->GetDescriptionOwnerFile();
						if (mod) {
							auto modName = "[" + std::string(mod->fileName).substr(0, 4) + "] ";
							std::string name = modName + spell->fullName.c_str();

							spell->fullName = name.c_str();
						}
					}
					player->AddSpell(spell);
				}
			}
		}
	}
}


bool papyrusDebug::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("papyrusDebug - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("GivePlayerSpellBook", "PO3_SKSEFunctions", GivePlayerSpellBook);

	return true;
}
