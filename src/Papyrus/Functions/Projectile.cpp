#include "Papyrus/Functions/Projectile.h"

namespace Papyrus::Projectile
{
	float GetProjectileGravity(STATIC_ARGS, const RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) {
			a_vm->TraceStack("Projectile is None", a_stackID);
			return 0.0f;
		}

		return a_projectile->data.gravity;
	}

	float GetProjectileImpactForce(STATIC_ARGS, const RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) {
			a_vm->TraceStack("Projectile is None", a_stackID);
			return 0.0f;
		}

		return a_projectile->data.force;
	}

	float GetProjectileRange(STATIC_ARGS, const RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) {
			a_vm->TraceStack("Projectile is None", a_stackID);
			return 0.0f;
		}

		return a_projectile->data.range;
	}

	float GetProjectileSpeed(STATIC_ARGS, const RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) {
			a_vm->TraceStack("Projectile is None", a_stackID);
			return 0.0f;
		}

		return a_projectile->data.speed;
	}

	std::uint32_t GetProjectileType(STATIC_ARGS, const RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) {
			a_vm->TraceStack("Projectile is None", a_stackID);
			return 0;
		}

		if (a_projectile->IsMissile()) {
			return 1;
		}
		if (a_projectile->IsGrenade()) {
			return 2;
		}
		if (a_projectile->IsBeam()) {
			return 3;
		}
		if (a_projectile->IsFlamethrower()) {
			return 4;
		}
		if (a_projectile->IsCone()) {
			return 5;
		}
		if (a_projectile->IsBarrier()) {
			return 6;
		}
		if (a_projectile->IsArrow()) {
			return 7;
		}

		return 0;
	}

	void SetProjectileGravity(STATIC_ARGS, RE::BGSProjectile* a_projectile, float a_gravity)
	{
		if (!a_projectile) {
			a_vm->TraceStack("Projectile is None", a_stackID);
			return;
		}

		a_projectile->data.gravity = a_gravity;
	}

	void SetProjectileImpactForce(STATIC_ARGS, RE::BGSProjectile* a_projectile, float a_force)
	{
		if (!a_projectile) {
			a_vm->TraceStack("Projectile is None", a_stackID);
			return;
		}

		a_projectile->data.force = a_force;
	}

	void SetProjectileRange(STATIC_ARGS, RE::BGSProjectile* a_projectile, float a_range)
	{
		if (!a_projectile) {
			a_vm->TraceStack("Projectile is None", a_stackID);
			return;
		}

		a_projectile->data.range = a_range;
	}

	void SetProjectileSpeed(STATIC_ARGS, RE::BGSProjectile* a_projectile, float a_speed)
	{
		if (!a_projectile) {
			a_vm->TraceStack("Projectile is None", a_stackID);
			return;
		}

		a_projectile->data.speed = a_speed;
	}

	void Bind(VM& a_vm)
	{
		BIND(GetProjectileGravity, true);
		BIND(GetProjectileImpactForce, true);
		BIND(GetProjectileRange, true);
		BIND(GetProjectileSpeed, true);
		BIND(GetProjectileType, true);
		BIND(SetProjectileGravity);
		BIND(SetProjectileImpactForce);
		BIND(SetProjectileRange);
		BIND(SetProjectileSpeed);

		logger::info("Registered projectile functions"sv);
	}
}
