#pragma once

namespace Papyrus::Projectile
{
	inline float GetProjectileGravity(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) {
			a_vm->TraceStack("Projectile is None", a_stackID);
			return 0.0f;
		}

		return a_projectile->data.gravity;
	}

	inline float GetProjectileImpactForce(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) {
			a_vm->TraceStack("Projectile is None", a_stackID);
			return 0.0f;
		}

		return a_projectile->data.force;
	}

	inline float GetProjectileRange(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) {
			a_vm->TraceStack("Projectile is None", a_stackID);
			return 0.0f;
		}

		return a_projectile->data.range;
	}

	inline float GetProjectileSpeed(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) {
			a_vm->TraceStack("Projectile is None", a_stackID);
			return 0.0f;
		}

		return a_projectile->data.speed;
	}

	inline std::uint32_t GetProjectileType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSProjectile* a_projectile)
	{
		if (!a_projectile) {
			a_vm->TraceStack("Projectile is None", a_stackID);
			return 0;
		}

		return a_projectile->GetType();
	}

	inline void SetProjectileGravity(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		RE::BGSProjectile* a_projectile, 
		float a_gravity)
	{
		if (!a_projectile) {
			a_vm->TraceStack("Projectile is None", a_stackID);
			return;
		}

		a_projectile->data.gravity = a_gravity;
	}

	inline void SetProjectileImpactForce(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		RE::BGSProjectile* a_projectile, 
		float a_force)
	{
		if (!a_projectile) {
			a_vm->TraceStack("Projectile is None", a_stackID);
			return;
		}

		a_projectile->data.force = a_force;
	}

	inline void SetProjectileRange(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		RE::BGSProjectile* a_projectile, 
		float a_range)
	{
		if (!a_projectile) {
			a_vm->TraceStack("Projectile is None", a_stackID);
			return;
		}

		a_projectile->data.range = a_range;
	}

	inline void SetProjectileSpeed(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		RE::BGSProjectile* a_projectile, 
		float a_speed)
	{
		if (!a_projectile) {
			a_vm->TraceStack("Projectile is None", a_stackID);
			return;
		}

		a_projectile->data.speed = a_speed;
	}

	inline void Bind(VM& a_vm)
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
