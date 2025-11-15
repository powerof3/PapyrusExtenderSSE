#include "Papyrus/Functions/Hazard.h"

namespace Papyrus::Hazard
{
	void ClearHazardFlag(STATIC_ARGS, RE::BGSHazard* a_hazard, std::uint32_t a_flag)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->data.flags.reset(static_cast<RE::BGSHazardData::BGSHazardFlags>(a_flag));
	}

	RE::BSFixedString GetHazardArt(STATIC_ARGS, const RE::BGSHazard* a_hazard)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return {};
		}

		return a_hazard->GetModel();
	}

	RE::TESImageSpaceModifier* GetHazardIMOD(STATIC_ARGS, const RE::BGSHazard* a_hazard)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return nullptr;
		}

		return a_hazard->imageSpaceModifying;
	}

	float GetHazardIMODRadius(STATIC_ARGS, const RE::BGSHazard* a_hazard)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return 0.0f;
		}

		return a_hazard->data.imageSpaceRadius;
	}

	RE::BGSImpactDataSet* GetHazardIPDS(STATIC_ARGS, const RE::BGSHazard* a_hazard)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return nullptr;
		}

		return a_hazard->data.impactDataSet;
	}

	float GetHazardLifetime(STATIC_ARGS, const RE::BGSHazard* a_hazard)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return 0.0f;
		}

		return a_hazard->data.lifetime;
	}

	RE::TESObjectLIGH* GetHazardLight(STATIC_ARGS, const RE::BGSHazard* a_hazard)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return nullptr;
		}

		return a_hazard->data.light;
	}

	std::int32_t GetHazardLimit(STATIC_ARGS, const RE::BGSHazard* a_hazard)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return -1;
		}

		return a_hazard->data.limit;
	}

	float GetHazardRadius(STATIC_ARGS, const RE::BGSHazard* a_hazard)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return 0.0f;
		}

		return a_hazard->data.radius;
	}

	RE::BGSSoundDescriptorForm* GetHazardSound(STATIC_ARGS, const RE::BGSHazard* a_hazard)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return nullptr;
		}

		return a_hazard->data.sound;
	}

	RE::SpellItem* GetHazardSpell(STATIC_ARGS, const RE::BGSHazard* a_hazard)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return nullptr;
		}

		return a_hazard->data.spell;
	}

	float GetHazardTargetInterval(STATIC_ARGS, const RE::BGSHazard* a_hazard)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return 0.0f;
		}

		return a_hazard->data.targetInterval;
	}

	bool IsHazardFlagSet(STATIC_ARGS, const RE::BGSHazard* a_hazard, std::uint32_t a_flag)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return false;
		}

		return a_hazard->data.flags.all(static_cast<RE::BGSHazardData::BGSHazardFlags>(a_flag));
	}

	void SetHazardArt(STATIC_ARGS, RE::BGSHazard* a_hazard, RE::BSFixedString a_path)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->SetModel(a_path.data());
	}

	void SetHazardFlag(STATIC_ARGS, RE::BGSHazard* a_hazard, std::uint32_t a_flag)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->data.flags.set(static_cast<RE::BGSHazardData::BGSHazardFlags>(a_flag));
	}

	void SetHazardIMOD(STATIC_ARGS, RE::BGSHazard* a_hazard, RE::TESImageSpaceModifier* a_imod)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->imageSpaceModifying = a_imod;
	}

	void SetHazardIMODRadius(STATIC_ARGS, RE::BGSHazard* a_hazard, float a_radius)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->data.imageSpaceRadius = a_radius;
	}

	void SetHazardIPDS(STATIC_ARGS, RE::BGSHazard* a_hazard, RE::BGSImpactDataSet* a_ipds)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->data.impactDataSet = a_ipds;
	}

	void SetHazardLifetime(STATIC_ARGS, RE::BGSHazard* a_hazard, float a_lifetime)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->data.lifetime = a_lifetime;
	}

	void SetHazardLight(STATIC_ARGS, RE::BGSHazard* a_hazard, RE::TESObjectLIGH* a_light)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->data.light = a_light;
	}

	void SetHazardLimit(STATIC_ARGS, RE::BGSHazard* a_hazard, std::uint32_t a_limit)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->data.limit = a_limit;
	}

	void SetHazardRadius(STATIC_ARGS, RE::BGSHazard* a_hazard, float a_radius)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->data.radius = a_radius;
	}

	void SetHazardSound(STATIC_ARGS, RE::BGSHazard* a_hazard, RE::BGSSoundDescriptorForm* a_sound)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->data.sound = a_sound;
	}

	void SetHazardSpell(STATIC_ARGS, RE::BGSHazard* a_hazard, RE::SpellItem* a_spell)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->data.spell = a_spell;
	}

	void SetHazardTargetInterval(STATIC_ARGS, RE::BGSHazard* a_hazard, float a_interval)
	{
		if (!a_hazard) {
			a_vm->TraceStack("Hazard is None", a_stackID);
			return;
		}

		a_hazard->data.targetInterval = a_interval;
	}

	void Bind(VM& a_vm)
	{
		std::uint32_t count = 0;

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

		logger::info("Registered {} hazard functions"sv, count);
	}
}
