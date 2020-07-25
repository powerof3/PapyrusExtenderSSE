#pragma once


namespace papyrusProjectile
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;


	float GetProjectileGravity(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSProjectile* a_projectile);

	float GetProjectileImpactForce(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSProjectile* a_projectile);

	float GetProjectileRange(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSProjectile* a_projectile);

	float GetProjectileSpeed(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSProjectile* a_projectile);

	std::uint32_t GetProjectileType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSProjectile* a_projectile);

	void SetProjectileGravity(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSProjectile* a_projectile, float a_gravity);

	void SetProjectileImpactForce(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSProjectile* a_projectile, float a_force);

	void SetProjectileRange(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSProjectile* a_projectile, float a_range);

	void SetProjectileSpeed(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSProjectile* a_projectile, float a_speed);


	bool RegisterFuncs(VM* a_vm);
}