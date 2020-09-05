#include "Papyrus/PapyrusHazard.h"

void papyrusHazard::ClearHazardFlag(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, std::uint32_t a_flag)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return;
	}

	a_hazard->data.flags.reset(static_cast<RE::BGSHazardData::BGSHazardFlags>(a_flag));
}


RE::BSFixedString papyrusHazard::GetHazardArt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return "";
	}

	return a_hazard->GetModel();
}


RE::TESImageSpaceModifier* papyrusHazard::GetHazardIMOD(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	return a_hazard->imageSpaceModifying;
}


float papyrusHazard::GetHazardIMODRadius(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_hazard->data.imageSpaceRadius;
}


RE::BGSImpactDataSet* papyrusHazard::GetHazardIPDS(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	return a_hazard->data.impactDataSet;
}


float papyrusHazard::GetHazardLifetime(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_hazard->data.lifetime;
}


RE::TESObjectLIGH* papyrusHazard::GetHazardLight(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	return a_hazard->data.light;
}


std::int32_t papyrusHazard::GetHazardLimit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return -1;
	}

	return a_hazard->data.limit;
}


float papyrusHazard::GetHazardRadius(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_hazard->data.radius;
}


RE::BGSSoundDescriptorForm* papyrusHazard::GetHazardSound(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	return a_hazard->data.sound;
}


RE::SpellItem* papyrusHazard::GetHazardSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	return a_hazard->data.spell;
}


float papyrusHazard::GetHazardTargetInterval(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_hazard->data.targetInterval;
}


bool papyrusHazard::IsHazardFlagSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, std::uint32_t a_flag)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return false;
	}

	return a_hazard->data.flags.all(static_cast<RE::BGSHazardData::BGSHazardFlags>(a_flag));
}


void papyrusHazard::SetHazardArt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, RE::BSFixedString a_path)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return;
	}

	a_hazard->SetModel(a_path.data());
}


void papyrusHazard::SetHazardFlag(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, std::uint32_t a_flag)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return;
	}

	a_hazard->data.flags.set(static_cast<RE::BGSHazardData::BGSHazardFlags>(a_flag));
}


void papyrusHazard::SetHazardIMOD(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, RE::TESImageSpaceModifier* a_imod)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return;
	}

	a_hazard->imageSpaceModifying = a_imod;
}


void papyrusHazard::SetHazardIMODRadius(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, float a_radius)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return;
	}

	a_hazard->data.imageSpaceRadius = a_radius;
}


void papyrusHazard::SetHazardIPDS(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, RE::BGSImpactDataSet* a_ipds)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return;
	}

	a_hazard->data.impactDataSet = a_ipds;
}


void papyrusHazard::SetHazardLifetime(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, float a_lifetime)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return;
	}

	a_hazard->data.lifetime = a_lifetime;
}


void papyrusHazard::SetHazardLight(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, RE::TESObjectLIGH* a_light)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return;
	}

	a_hazard->data.light = a_light;
}


void papyrusHazard::SetHazardLimit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, std::uint32_t a_limit)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return;
	}

	a_hazard->data.limit = a_limit;
}


void papyrusHazard::SetHazardRadius(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, float a_radius)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return;
	}

	a_hazard->data.radius =  a_radius;
}


void papyrusHazard::SetHazardSound(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, RE::BGSSoundDescriptorForm* a_sound)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return;
	}

	a_hazard->data.sound = a_sound;
}


void papyrusHazard::SetHazardSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, RE::SpellItem* a_spell)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return;
	}

	a_hazard->data.spell = a_spell;
}


void papyrusHazard::SetHazardTargetInterval(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, float a_interval)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return;
	}

	a_hazard->data.targetInterval = a_interval;
}


bool papyrusHazard::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("papyrusHazard - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("ClearHazardFlag", "PO3_SKSEFunctions", ClearHazardFlag);

	a_vm->RegisterFunction("GetHazardArt", "PO3_SKSEFunctions", GetHazardArt, true);

	a_vm->RegisterFunction("GetHazardIMOD", "PO3_SKSEFunctions", GetHazardIMOD, true);

	a_vm->RegisterFunction("GetHazardIMODRadius", "PO3_SKSEFunctions", GetHazardIMODRadius, true);

	a_vm->RegisterFunction("GetHazardIPDS", "PO3_SKSEFunctions", GetHazardIPDS, true);

	a_vm->RegisterFunction("GetHazardLifetime", "PO3_SKSEFunctions", GetHazardLifetime, true);

	a_vm->RegisterFunction("GetHazardLight", "PO3_SKSEFunctions", GetHazardLight, true);

	a_vm->RegisterFunction("GetHazardLimit", "PO3_SKSEFunctions", GetHazardLimit, true);

	a_vm->RegisterFunction("GetHazardRadius", "PO3_SKSEFunctions", GetHazardRadius, true);

	a_vm->RegisterFunction("GetHazardSound", "PO3_SKSEFunctions", GetHazardSound, true);

	a_vm->RegisterFunction("GetHazardSpell", "PO3_SKSEFunctions", GetHazardSpell, true);

	a_vm->RegisterFunction("GetHazardTargetInterval", "PO3_SKSEFunctions", GetHazardTargetInterval, true);

	a_vm->RegisterFunction("IsHazardFlagSet", "PO3_SKSEFunctions", IsHazardFlagSet, true);

	a_vm->RegisterFunction("SetHazardArt", "PO3_SKSEFunctions", SetHazardArt);

	a_vm->RegisterFunction("SetHazardIMOD", "PO3_SKSEFunctions", SetHazardIMOD);

	a_vm->RegisterFunction("SetHazardIMODRadius", "PO3_SKSEFunctions", SetHazardIMODRadius);

	a_vm->RegisterFunction("SetHazardIPDS", "PO3_SKSEFunctions", SetHazardIPDS);

	a_vm->RegisterFunction("SetHazardLifetime", "PO3_SKSEFunctions", SetHazardLifetime);

	a_vm->RegisterFunction("SetHazardLight", "PO3_SKSEFunctions", SetHazardLight);

	a_vm->RegisterFunction("SetHazardLimit", "PO3_SKSEFunctions", SetHazardLimit);

	a_vm->RegisterFunction("SetHazardRadius", "PO3_SKSEFunctions", SetHazardRadius);

	a_vm->RegisterFunction("SetHazardSound", "PO3_SKSEFunctions", SetHazardSound);

	a_vm->RegisterFunction("SetHazardSpell", "PO3_SKSEFunctions", SetHazardSpell);

	a_vm->RegisterFunction("SetHazardTargetInterval", "PO3_SKSEFunctions", SetHazardTargetInterval);

	return true;
}
