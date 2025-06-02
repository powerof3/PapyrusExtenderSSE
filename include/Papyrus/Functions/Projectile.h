#pragma once

namespace Papyrus::Projectile
{
	float         GetProjectileGravity(STATIC_ARGS, const RE::BGSProjectile* a_projectile);
	float         GetProjectileImpactForce(STATIC_ARGS, const RE::BGSProjectile* a_projectile);
	float         GetProjectileRange(STATIC_ARGS, const RE::BGSProjectile* a_projectile);
	float         GetProjectileSpeed(STATIC_ARGS, const RE::BGSProjectile* a_projectile);
	std::uint32_t GetProjectileType(STATIC_ARGS, const RE::BGSProjectile* a_projectile);
	void          SetProjectileGravity(STATIC_ARGS, RE::BGSProjectile* a_projectile, float a_gravity);
	void          SetProjectileImpactForce(STATIC_ARGS, RE::BGSProjectile* a_projectile, float a_force);
	void          SetProjectileRange(STATIC_ARGS, RE::BGSProjectile* a_projectile, float a_range);
	void          SetProjectileSpeed(STATIC_ARGS, RE::BGSProjectile* a_projectile, float a_speed);

	void Bind(VM& a_vm);
}
