#include "Papyrus/Functions/ArmorAddon.h"

namespace Papyrus::ArmorAddon
{
	RE::BGSFootstepSet* GetFootstepSet(STATIC_ARGS, const RE::TESObjectARMA* a_arma)
	{
		if (!a_arma) {
			a_vm->TraceStack("Armor Addon is None", a_stackID);
			return nullptr;
		}

		return a_arma->footstepSet;
	}

	void SetFootstepSet(STATIC_ARGS, RE::TESObjectARMA* a_arma, RE::BGSFootstepSet* a_footstepSet)
	{
		if (!a_arma) {
			a_vm->TraceStack("Armor Addon is None", a_stackID);
			return;
		}

		a_arma->footstepSet = a_footstepSet;
	}

	void Bind(VM& a_vm)
	{
		BIND(GetFootstepSet);
		BIND(SetFootstepSet);

		logger::info("Registered armor addon functions"sv);
	}
}
