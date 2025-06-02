#pragma once

namespace Papyrus::ActiveMagicEffect::Functions
{
	RE::TESForm*                   GetActiveEffectSpell(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	std::vector<RE::BSFixedString> GetScriptsAttachedToActiveEffect(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect);
	bool                           IsScriptAttachedToActiveEffect(STATIC_ARGS, const RE::ActiveEffect* a_activeEffect, RE::BSFixedString a_scriptName);

	void Bind(VM& a_vm);
}
