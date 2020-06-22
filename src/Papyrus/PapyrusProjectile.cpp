#include "Papyrus/PapyrusProjectile.h"


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
    return  a_projectile->data.speed;
}


UInt32 papyrusProjectile::GetProjectileType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSProjectile* a_projectile)
{
    using Type = RE::BGSProjectileData::Type;
    
    if (!a_projectile) {
        a_vm->TraceStack("Projectile is None", a_stackID, Severity::kWarning);
        return 0;
    }
    const auto types = a_projectile->data.types;

    if ((types & Type::kMissile) == Type::kMissile) //Missile
    {
        return 1;
    }
    if ((types & Type::kGrenade) == Type::kGrenade) //Lobber (runes)
    {
        return 2;
    }
    if ((types & Type::kBeam) == Type::kBeam) //Beam
    {
        return 3;
    }
    if ((types & Type::kFlamethrower) == Type::kFlamethrower) //Flame
    {
        return 4;
    }
    if ((types & Type::kCone) == Type::kCone) //Cone
    {
        return 5;
    }
    if ((types & Type::kBarrier) == Type::kBarrier) //Barrier
    {
        return 6;
    }
    if ((types & Type::kArrow) == Type::kArrow) //Arrow
    {
        return 7;
    }

    return 0;
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
        _MESSAGE("papyrusProjectile - couldn't get VMState");
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
