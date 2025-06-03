#pragma once

namespace Papyrus::Alias::Functions
{
	std::vector<RE::BSFixedString> GetScriptsAttachedToAlias(STATIC_ARGS, const RE::BGSBaseAlias* a_alias);
	bool                           IsScriptAttachedToAlias(STATIC_ARGS, const RE::BGSBaseAlias* a_alias, RE::BSFixedString a_scriptName);

	void Bind(VM& a_vm);
}
