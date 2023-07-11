#pragma once

namespace Papyrus::Alias::Functions
{
	inline bool IsScriptAttachedToAlias(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::BGSBaseAlias* a_alias,
		RE::BSFixedString       a_scriptName)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return false;
		}

		return SCRIPT::is_script_attached(a_alias, a_scriptName);
	}

	inline void Bind(VM& a_vm)
	{
		BIND(IsScriptAttachedToAlias);

		logger::info("Registered alias functions"sv);
	}
}
