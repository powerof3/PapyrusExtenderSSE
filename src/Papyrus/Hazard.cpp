#include "Papyrus/Hazard.h"


void papyrusHazard::ClearHazardFlag(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, std::uint32_t a_flag)
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return;
	}

	a_hazard->data.flags.reset(static_cast<RE::BGSHazardData::BGSHazardFlags>(a_flag));
}


auto papyrusHazard::GetHazardArt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard) -> RE::BSFixedString
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return RE::BSFixedString();
	}

	return a_hazard->GetModel();
}


auto papyrusHazard::GetHazardIMOD(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard) -> RE::TESImageSpaceModifier*
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	return a_hazard->imageSpaceModifying;
}


auto papyrusHazard::GetHazardIMODRadius(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard) -> float
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_hazard->data.imageSpaceRadius;
}


auto papyrusHazard::GetHazardIPDS(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard) -> RE::BGSImpactDataSet*
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	return a_hazard->data.impactDataSet;
}


auto papyrusHazard::GetHazardLifetime(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard) -> float
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_hazard->data.lifetime;
}


auto papyrusHazard::GetHazardLight(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard) -> RE::TESObjectLIGH*
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	return a_hazard->data.light;
}


auto papyrusHazard::GetHazardLimit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard) -> std::int32_t
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return -1;
	}

	return a_hazard->data.limit;
}


auto papyrusHazard::GetHazardRadius(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard) -> float
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_hazard->data.radius;
}


auto papyrusHazard::GetHazardSound(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard) -> RE::BGSSoundDescriptorForm*
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	return a_hazard->data.sound;
}


auto papyrusHazard::GetHazardSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard) -> RE::SpellItem*
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	return a_hazard->data.spell;
}


auto papyrusHazard::GetHazardTargetInterval(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard) -> float
{
	if (!a_hazard) {
		a_vm->TraceStack("Hazard is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_hazard->data.targetInterval;
}


auto papyrusHazard::IsHazardFlagSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, std::uint32_t a_flag) -> bool
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


auto papyrusHazard::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusHazard - couldn't get VMState"sv);
		return false;
	}

	auto constexpr Functions = "PO3_SKSEFunctions"sv;

    a_vm->RegisterFunction("ClearHazardFlag"sv, Functions, ClearHazardFlag);

	a_vm->RegisterFunction("GetHazardArt"sv, Functions, GetHazardArt, true);

	a_vm->RegisterFunction("GetHazardIMOD"sv, Functions, GetHazardIMOD, true);

	a_vm->RegisterFunction("GetHazardIMODRadius"sv, Functions, GetHazardIMODRadius, true);

	a_vm->RegisterFunction("GetHazardIPDS"sv, Functions, GetHazardIPDS, true);

	a_vm->RegisterFunction("GetHazardLifetime"sv, Functions, GetHazardLifetime, true);

	a_vm->RegisterFunction("GetHazardLight"sv, Functions, GetHazardLight, true);

	a_vm->RegisterFunction("GetHazardLimit"sv, Functions, GetHazardLimit, true);

	a_vm->RegisterFunction("GetHazardRadius"sv, Functions, GetHazardRadius, true);

	a_vm->RegisterFunction("GetHazardSound"sv, Functions, GetHazardSound, true);

	a_vm->RegisterFunction("GetHazardSpell"sv, Functions, GetHazardSpell, true);

	a_vm->RegisterFunction("GetHazardTargetInterval"sv, Functions, GetHazardTargetInterval, true);

	a_vm->RegisterFunction("IsHazardFlagSet"sv, Functions, IsHazardFlagSet, true);

	a_vm->RegisterFunction("SetHazardArt"sv, Functions, SetHazardArt);

	a_vm->RegisterFunction("SetHazardIMOD"sv, Functions, SetHazardIMOD);

	a_vm->RegisterFunction("SetHazardIMODRadius"sv, Functions, SetHazardIMODRadius);

	a_vm->RegisterFunction("SetHazardIPDS"sv, Functions, SetHazardIPDS);

	a_vm->RegisterFunction("SetHazardLifetime"sv, Functions, SetHazardLifetime);

	a_vm->RegisterFunction("SetHazardLight"sv, Functions, SetHazardLight);

	a_vm->RegisterFunction("SetHazardLimit"sv, Functions, SetHazardLimit);

	a_vm->RegisterFunction("SetHazardRadius"sv, Functions, SetHazardRadius);

	a_vm->RegisterFunction("SetHazardSound"sv, Functions, SetHazardSound);

	a_vm->RegisterFunction("SetHazardSpell"sv, Functions, SetHazardSpell);

	a_vm->RegisterFunction("SetHazardTargetInterval"sv, Functions, SetHazardTargetInterval);

	return true;
}
