#include "Papyrus/Functions/Ammo.h"

namespace Papyrus::Ammo
{
	RE::EnchantmentItem* GetBaseAmmoEnchantment(STATIC_ARGS, const RE::TESAmmo* a_ammo)
	{
		if (!a_ammo) {
			a_vm->TraceStack("Ammo is None", a_stackID);
			return nullptr;
		}

		const auto explosion = a_ammo->data.projectile ? a_ammo->data.projectile->data.explosionType : nullptr;

		return explosion ? explosion->formEnchanting : nullptr;
	}

	void Bind(VM& a_vm)
	{
		BIND(GetBaseAmmoEnchantment);

		logger::info("Registered ammo functions"sv);
	}
}
