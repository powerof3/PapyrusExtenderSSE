#pragma once

namespace Papyrus::Hazard
{
	inline void ClearHazardFlag(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		RE::BGSHazard* a_hazard, 
		std::uint32_t a_flag)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->data.flags.reset(static_cast<RE::BGSHazardData::BGSHazardFlags>(a_flag));
	}

	inline RE::BSFixedString GetHazardArt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSHazard* a_hazard)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return RE::BSFixedString();
		}

		return a_hazard->GetModel();
	}

	inline RE::TESImageSpaceModifier* GetHazardIMOD(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSHazard* a_hazard)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return nullptr;
		}

		return a_hazard->imageSpaceModifying;
	}

	inline float GetHazardIMODRadius(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSHazard* a_hazard)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return 0.0f;
		}

		return a_hazard->data.imageSpaceRadius;
	}

	inline RE::BGSImpactDataSet* GetHazardIPDS(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSHazard* a_hazard)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return nullptr;
		}

		return a_hazard->data.impactDataSet;
	}

	inline float GetHazardLifetime(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSHazard* a_hazard)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return 0.0f;
		}

		return a_hazard->data.lifetime;
	}

	inline RE::TESObjectLIGH* GetHazardLight(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSHazard* a_hazard)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return nullptr;
		}

		return a_hazard->data.light;
	}

	inline std::int32_t GetHazardLimit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSHazard* a_hazard)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return -1;
		}

		return a_hazard->data.limit;
	}

	inline float GetHazardRadius(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSHazard* a_hazard)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return 0.0f;
		}

		return a_hazard->data.radius;
	}

	inline RE::BGSSoundDescriptorForm* GetHazardSound(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSHazard* a_hazard)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return nullptr;
		}

		return a_hazard->data.sound;
	}

	inline RE::SpellItem* GetHazardSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSHazard* a_hazard)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return nullptr;
		}

		return a_hazard->data.spell;
	}

	inline float GetHazardTargetInterval(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSHazard* a_hazard)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return 0.0f;
		}

		return a_hazard->data.targetInterval;
	}

	inline bool IsHazardFlagSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		const RE::BGSHazard* a_hazard, 
		std::uint32_t a_flag)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return false;
		}

		return a_hazard->data.flags.all(static_cast<RE::BGSHazardData::BGSHazardFlags>(a_flag));
	}

	inline void SetHazardArt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		RE::BGSHazard* a_hazard, 
		RE::BSFixedString a_path)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->SetModel(a_path.data());
	}

	inline void SetHazardFlag(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		RE::BGSHazard* a_hazard, 
		std::uint32_t a_flag)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->data.flags.set(static_cast<RE::BGSHazardData::BGSHazardFlags>(a_flag));
	}

	inline void SetHazardIMOD(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, RE::TESImageSpaceModifier* a_imod)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->imageSpaceModifying = a_imod;
	}

	inline void SetHazardIMODRadius(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, float a_radius)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->data.imageSpaceRadius = a_radius;
	}

	inline void SetHazardIPDS(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, RE::BGSImpactDataSet* a_ipds)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->data.impactDataSet = a_ipds;
	}

	inline void SetHazardLifetime(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, float a_lifetime)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->data.lifetime = a_lifetime;
	}

	inline void SetHazardLight(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, RE::TESObjectLIGH* a_light)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->data.light = a_light;
	}

	inline void SetHazardLimit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, std::uint32_t a_limit)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->data.limit = a_limit;
	}

	inline void SetHazardRadius(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, float a_radius)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->data.radius = a_radius;
	}

	inline void SetHazardSound(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		RE::BGSHazard* a_hazard, 
		RE::BGSSoundDescriptorForm* a_sound)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->data.sound = a_sound;
	}

	inline void SetHazardSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		RE::BGSHazard* a_hazard, 
		RE::SpellItem* a_spell)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->data.spell = a_spell;
	}

	inline void SetHazardTargetInterval(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		RE::BGSHazard* a_hazard, 
		float a_interval)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->data.targetInterval = a_interval;
	}

	inline void Bind(VM& a_vm)
	{
		BIND(ClearHazardFlag);
		BIND(GetHazardArt, true);
		BIND(GetHazardIMOD, true);
		BIND(GetHazardIMODRadius, true);
		BIND(GetHazardIPDS, true);
		BIND(GetHazardLifetime, true);
		BIND(GetHazardLight, true);
		BIND(GetHazardLimit, true);
		BIND(GetHazardRadius, true);
		BIND(GetHazardSound, true);
		BIND(GetHazardSpell, true);
		BIND(GetHazardTargetInterval, true);
		BIND(IsHazardFlagSet, true);
		BIND(SetHazardArt);
		BIND(SetHazardIMOD);
		BIND(SetHazardIMODRadius);
		BIND(SetHazardIPDS);
		BIND(SetHazardLifetime);
		BIND(SetHazardLight);
		BIND(SetHazardLimit);
		BIND(SetHazardRadius);
		BIND(SetHazardSound);
		BIND(SetHazardSpell);
		BIND(SetHazardTargetInterval);

		logger::info("Registered hazard functions"sv);
	}

}
