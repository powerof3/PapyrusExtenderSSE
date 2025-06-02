#pragma once

namespace Papyrus::Ammo
{
	RE::EnchantmentItem* GetBaseAmmoEnchantment(STATIC_ARGS, const RE::TESAmmo* a_ammo);

	void Bind(VM& a_vm);
}
