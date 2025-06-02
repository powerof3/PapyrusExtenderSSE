#include "Papyrus/Functions/Alias/Functions.h"

namespace Papyrus::Alias::Functions {

	std::vector<RE::BSFixedString> GetScriptsAttachedToAlias(STATIC_ARGS, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return {};
		}

		return SCRIPT::get_all_scripts(a_alias);
	}

	bool IsScriptAttachedToAlias(STATIC_ARGS, const RE::BGSBaseAlias* a_alias, RE::BSFixedString a_scriptName)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return false;
		}

		return SCRIPT::is_script_attached(a_alias, a_scriptName);
	}

	void Bind(VM& a_vm)
	{
		BIND(GetScriptsAttachedToAlias);
		BIND(IsScriptAttachedToAlias);

		logger::info("Registered alias functions"sv);
	}
}
