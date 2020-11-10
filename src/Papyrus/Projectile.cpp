#include "Papyrus/Projectile.h"


float papyrusProjectile::GetProjectileGravity(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSProjectile* a_projectile)
{
	if (!a_projectile) {
		a_vm->TraceStack("Projectile is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_projectile->data.gravity;
}


float papyrusProjectile::GetProjectileImpactForce(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSProjectile* a_projectile)
{
	if (!a_projectile) {
		a_vm->TraceStack("Projectile is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_projectile->data.force;
}


float papyrusProjectile::GetProjectileRange(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSProjectile* a_projectile)
{
	if (!a_projectile) {
		a_vm->TraceStack("Projectile is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_projectile->data.range;
}


float papyrusProjectile::GetProjectileSpeed(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSProjectile* a_projectile)
{
	if (!a_projectile) {
		a_vm->TraceStack("Projectile is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_projectile->data.speed;
}


std::uint32_t papyrusProjectile::GetProjectileType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSProjectile* a_projectile)
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


bool papyrusProjectile::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("papyrusProjectile - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("GetProjectileGravity", "PO3_SKSEFunctions", GetProjectileGravity, true);

	a_vm->RegisterFunction("GetProjectileImpactForce", "PO3_SKSEFunctions", GetProjectileImpactForce, true);

	a_vm->RegisterFunction("GetProjectileRange", "PO3_SKSEFunctions", GetProjectileRange, true);

	a_vm->RegisterFunction("GetProjectileSpeed", "PO3_SKSEFunctions", GetProjectileSpeed, true);

	a_vm->RegisterFunction("GetProjectileType", "PO3_SKSEFunctions", GetProjectileType);

	a_vm->RegisterFunction("SetProjectileGravity", "PO3_SKSEFunctions", SetProjectileGravity);

	a_vm->RegisterFunction("SetProjectileImpactForce", "PO3_SKSEFunctions", SetProjectileImpactForce);

	a_vm->RegisterFunction("SetProjectileRange", "PO3_SKSEFunctions", SetProjectileRange);

	a_vm->RegisterFunction("SetProjectileSpeed", "PO3_SKSEFunctions", SetProjectileSpeed);

	return true;
}
