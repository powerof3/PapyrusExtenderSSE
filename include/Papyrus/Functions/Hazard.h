#pragma once

namespace Papyrus::Hazard
{
	void                        ClearHazardFlag(STATIC_ARGS, RE::BGSHazard* a_hazard, std::uint32_t a_flag);
	RE::BSFixedString           GetHazardArt(STATIC_ARGS, const RE::BGSHazard* a_hazard);
	RE::TESImageSpaceModifier*  GetHazardIMOD(STATIC_ARGS, const RE::BGSHazard* a_hazard);
	float                       GetHazardIMODRadius(STATIC_ARGS, const RE::BGSHazard* a_hazard);
	RE::BGSImpactDataSet*       GetHazardIPDS(STATIC_ARGS, const RE::BGSHazard* a_hazard);
	float                       GetHazardLifetime(STATIC_ARGS, const RE::BGSHazard* a_hazard);
	RE::TESObjectLIGH*          GetHazardLight(STATIC_ARGS, const RE::BGSHazard* a_hazard);
	std::int32_t                GetHazardLimit(STATIC_ARGS, const RE::BGSHazard* a_hazard);
	float                       GetHazardRadius(STATIC_ARGS, const RE::BGSHazard* a_hazard);
	RE::BGSSoundDescriptorForm* GetHazardSound(STATIC_ARGS, const RE::BGSHazard* a_hazard);
	RE::SpellItem*              GetHazardSpell(STATIC_ARGS, const RE::BGSHazard* a_hazard);
	float                       GetHazardTargetInterval(STATIC_ARGS, const RE::BGSHazard* a_hazard);
	bool                        IsHazardFlagSet(STATIC_ARGS, const RE::BGSHazard* a_hazard, std::uint32_t a_flag);
	void                        SetHazardArt(STATIC_ARGS, RE::BGSHazard* a_hazard, RE::BSFixedString a_path);
	void                        SetHazardFlag(STATIC_ARGS, RE::BGSHazard* a_hazard, std::uint32_t a_flag);
	void                        SetHazardIMOD(STATIC_ARGS, RE::BGSHazard* a_hazard, RE::TESImageSpaceModifier* a_imod);
	void                        SetHazardIMODRadius(STATIC_ARGS, RE::BGSHazard* a_hazard, float a_radius);
	void                        SetHazardIPDS(STATIC_ARGS, RE::BGSHazard* a_hazard, RE::BGSImpactDataSet* a_ipds);
	void                        SetHazardLifetime(STATIC_ARGS, RE::BGSHazard* a_hazard, float a_lifetime);
	void                        SetHazardLight(STATIC_ARGS, RE::BGSHazard* a_hazard, RE::TESObjectLIGH* a_light);
	void                        SetHazardLimit(STATIC_ARGS, RE::BGSHazard* a_hazard, std::uint32_t a_limit);
	void                        SetHazardRadius(STATIC_ARGS, RE::BGSHazard* a_hazard, float a_radius);
	void                        SetHazardSound(STATIC_ARGS, RE::BGSHazard* a_hazard, RE::BGSSoundDescriptorForm* a_sound);
	void                        SetHazardSpell(STATIC_ARGS, RE::BGSHazard* a_hazard, RE::SpellItem* a_spell);
	void                        SetHazardTargetInterval(STATIC_ARGS, RE::BGSHazard* a_hazard, float a_interval);

	void Bind(VM& a_vm);
}
