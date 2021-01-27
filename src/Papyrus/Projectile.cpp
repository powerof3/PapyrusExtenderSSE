#include "Papyrus/Projectile.h"


auto papyrusProjectile::GetProjectileGravity(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSProjectile* a_projectile) -> float
{
	if (!a_projectile) {
		a_vm->TraceStack("Projectile is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_projectile->data.gravity;
}


auto papyrusProjectile::GetProjectileImpactForce(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSProjectile* a_projectile) -> float
{
	if (!a_projectile) {
		a_vm->TraceStack("Projectile is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_projectile->data.force;
}


auto papyrusProjectile::GetProjectileRange(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSProjectile* a_projectile) -> float
{
	if (!a_projectile) {
		a_vm->TraceStack("Projectile is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_projectile->data.range;
}


auto papyrusProjectile::GetProjectileSpeed(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSProjectile* a_projectile) -> float
{
	if (!a_projectile) {
		a_vm->TraceStack("Projectile is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_projectile->data.speed;
}


auto papyrusProjectile::GetProjectileType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSProjectile* a_projectile) -> std::uint32_t
{
	if (!a_projectile) {
		a_vm->TraceStack("Projectile is None", a_stackID, Severity::kWarning);
		return 0;
	}

	return a_projectile->GetType();
}


void papyrusProjectile::SetProjectileGravity(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSProjectile* a_projectile, float a_gravity)
{
	if (!a_projectile) {
		a_vm->TraceStack("Projectile is None", a_stackID, Severity::kWarning);
		return;
	}

	a_projectile->data.gravity = a_gravity;
}


void papyrusProjectile::SetProjectileImpactForce(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSProjectile* a_projectile, float a_force)
{
	if (!a_projectile) {
		a_vm->TraceStack("Projectile is None", a_stackID, Severity::kWarning);
		return;
	}

	a_projectile->data.force = a_force;
}


void papyrusProjectile::SetProjectileRange(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSProjectile* a_projectile, float a_range)
{
	if (!a_projectile) {
		a_vm->TraceStack("Projectile is None", a_stackID, Severity::kWarning);
		return;
	}

	a_projectile->data.range = a_range;
}


void papyrusProjectile::SetProjectileSpeed(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSProjectile* a_projectile, float a_speed)
{
	if (!a_projectile) {
		a_vm->TraceStack("Projectile is None", a_stackID, Severity::kWarning);
		return;
	}

	a_projectile->data.speed = a_speed;
}


auto papyrusProjectile::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusProjectile - couldn't get VMState"sv);
		return false;
	}

	auto constexpr Functions = "PO3_SKSEFunctions"sv;

	a_vm->RegisterFunction("GetProjectileGravity"sv, Functions, GetProjectileGravity, true);

	a_vm->RegisterFunction("GetProjectileImpactForce"sv, Functions, GetProjectileImpactForce, true);

	a_vm->RegisterFunction("GetProjectileRange"sv, Functions, GetProjectileRange, true);

	a_vm->RegisterFunction("GetProjectileSpeed"sv, Functions, GetProjectileSpeed, true);

	a_vm->RegisterFunction("GetProjectileType"sv, Functions, GetProjectileType);

	a_vm->RegisterFunction("SetProjectileGravity"sv, Functions, SetProjectileGravity);

	a_vm->RegisterFunction("SetProjectileImpactForce"sv, Functions, SetProjectileImpactForce);

	a_vm->RegisterFunction("SetProjectileRange"sv, Functions, SetProjectileRange);

	a_vm->RegisterFunction("SetProjectileSpeed"sv, Functions, SetProjectileSpeed);

	return true;
}
