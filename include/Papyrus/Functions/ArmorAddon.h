#pragma once

namespace Papyrus::ArmorAddon
{
	RE::BGSFootstepSet* GetFootstepSet(STATIC_ARGS, const RE::TESObjectARMA* a_arma);
	void SetFootstepSet(STATIC_ARGS, RE::TESObjectARMA* a_arma, RE::BGSFootstepSet* a_footstepSet);

	void Bind(VM& a_vm);
}
