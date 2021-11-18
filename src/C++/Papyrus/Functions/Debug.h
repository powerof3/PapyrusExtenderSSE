#pragma once

namespace Papyrus::Debug
{
	struct detail
	{
		static bool get_god_mode_impl()
		{
			using func_t = decltype(&detail::get_god_mode_impl);
			REL::Relocation<func_t> func{ REL::ID(39425) };
			return func();
		}
	};

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

	inline void DumpAnimationVariables(RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BSFixedString a_prefix)
	{
		if (!a_actor) {
			return;
		}

		constexpr auto get_value = [](RE::AnimVariableCacheInfo& a_var) {
			if (a_var.variable) {
				std::string variable(a_var.variableName);
				if (variable[0] == 'b' || variable.starts_with("Is")) {
					return std::string(a_var.variable->b ? "true" : "false");
				} else if (variable[0] == 'i') {
					return std::to_string(a_var.variable->i);
				} else {
					if (a_var.variable->f >= RE::NI_INFINITY && a_var.variable->i >= std::numeric_limits<std::int32_t>::max()) {
						return std::string(a_var.variable->b ? "true" : "false");
					}
					if (a_var.variable->f >= RE::NI_INFINITY) {
						return std::to_string(a_var.variable->i);
					}
					return std::to_string(a_var.variable->f);
				}
			} else {
				return std::string("no value");
			}
		};

		auto process = a_actor->currentProcess;
		auto middle = process ? process->middleHigh : nullptr;

		auto cache = middle ? middle->animationVariableCache : nullptr;
		auto manager = middle ? middle->animationGraphManager : nullptr;

		if (cache && manager) {
			logger::info("{} [0x{:X}] ANIMATION VARIABLES ({})", a_actor->GetName(), a_actor->GetFormID(), a_prefix);
			for (auto& var : cache->variableCache) {
				logger::info("	{} : {}", var.variableName, get_value(var));
			}
		}
	}

	inline bool GetGodMode(RE::StaticFunctionTag*)
	{
		return detail::get_god_mode_impl();
	}

	inline void Bind(VM& a_vm)
	{
		BIND(GivePlayerSpellBook);
		BIND(DumpAnimationVariables);
		BIND(GetGodMode);

		logger::info("Registered debug functions"sv);
	}
}
