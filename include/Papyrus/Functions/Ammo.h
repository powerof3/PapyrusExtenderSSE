#pragma once

namespace Papyrus::Ammo
{
	inline RE::EnchantmentItem* GetBaseAmmoEnchantment(STATIC_ARGS, const RE::TESAmmo* a_ammo)
	{
		if (!a_ammo) {
			a_vm->TraceStack("Ammo is None", a_stackID);
			return nullptr;
		}

		const auto projectile = a_ammo ? a_ammo->data.projectile : nullptr;
		const auto explosion = projectile ? projectile->data.explosionType : nullptr;

		return explosion ? explosion->formEnchanting : nullptr;
	}

	inline void Bind(VM& a_vm)
	{
		BIND(GetBaseAmmoEnchantment);

		logger::info("Registered ammo functions"sv);
	}
}
