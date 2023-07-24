#pragma once

namespace Papyrus::Debug
{
	struct detail
	{
		static bool get_god_mode_impl()
		{
			using func_t = decltype(&detail::get_god_mode_impl);
			REL::Relocation<func_t> func{ RELOCATION_ID(39425, 40501) };
			return func();
		}
	};

	inline void GivePlayerSpellBook(RE::StaticFunctionTag*)
	{
		const auto player = RE::PlayerCharacter::GetSingleton();
		const auto dataHandler = RE::TESDataHandler::GetSingleton();

		if (player && dataHandler) {
			for (const auto& book : dataHandler->GetFormArray<RE::TESObjectBOOK>()) {
				if (book && book->TeachesSpell()) {
					if (const auto spell = book->GetSpell(); spell && !spell->fullName.empty()) {
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

		constexpr auto get_value = [](const RE::AnimVariableCacheInfo& a_var) {
			if (a_var.variable) {
				const std::string variable(a_var.variableName);
				if (variable[0] == 'b' || variable.starts_with("Is")) {
					return std::string(a_var.variable->b ? "true" : "false");
				}
				if (variable[0] == 'i') {
					return std::to_string(a_var.variable->i);
				}
				if (a_var.variable->f >= RE::NI_INFINITY && a_var.variable->i >= std::numeric_limits<std::int32_t>::max()) {
					return std::string(a_var.variable->b ? "true" : "false");
				}
				if (a_var.variable->f >= RE::NI_INFINITY) {
					return std::to_string(a_var.variable->i);
				}
				return std::to_string(a_var.variable->f);
			}
			return std::string();
		};

		RE::BSTSmartPointer<RE::BSAnimationGraphManager> manager;
		if (a_actor->GetAnimationGraphManager(manager); manager) {
			const auto middleHigh = a_actor->GetMiddleHighProcess();
			const auto cache = middleHigh ? middleHigh->animationVariableCache : nullptr;

			if (cache) {
				logger::info("{} [0x{:X}] ANIMATION VARIABLES ({})", a_actor->GetName(), a_actor->GetFormID(), a_prefix);

				RE::BSSpinLockGuard locker(cache->updateLock);
				for (auto& var : cache->variableCache) {
					logger::info("\t{} : {}", var.variableName, get_value(var));
				}
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
