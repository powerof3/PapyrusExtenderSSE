#pragma once

namespace Papyrus::Debug
{
	inline void GivePlayerSpellBook(RE::StaticFunctionTag*)
	{
		const auto player = RE::PlayerCharacter::GetSingleton();
		const auto dataHandler = RE::TESDataHandler::GetSingleton();

		if (player && dataHandler) {
			for (const auto& book : dataHandler->GetFormArray<RE::TESObjectBOOK>()) {
				if (book && book->data.flags.all(RE::OBJ_BOOK::Flag::kTeachesSpell)) {
					const auto spell = book->data.teaches.spell; 
					if (spell && !spell->fullName.empty()) {
						if (const auto mod = spell->GetDescriptionOwnerFile(); mod) {
							auto modName{ "[" + std::string(mod->fileName).substr(0, 4) + "] " };
							spell->fullName = modName + spell->fullName.c_str();
						}
						player->AddSpell(spell);
					}
				}
			}
		}
	}

	inline void Register(VM& a_vm)
	{
		BIND(GivePlayerSpellBook);

		logger::info("Registered debug functions"sv);
	}
}
