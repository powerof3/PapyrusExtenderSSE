#pragma once


namespace papyrusHazard
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;


	void ClearHazardFlag(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, std::uint32_t a_flag);

	RE::BSFixedString GetHazardArt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard);
	
	RE::TESImageSpaceModifier* GetHazardIMOD(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard);

	float GetHazardIMODRadius(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard);
	
	RE::BGSImpactDataSet* GetHazardIPDS(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard);
	
	float GetHazardLifetime(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard);
	
	RE::TESObjectLIGH* GetHazardLight(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard);

	std::int32_t GetHazardLimit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard);
	
	float GetHazardRadius(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard);

	RE::BGSSoundDescriptorForm* GetHazardSound(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard);
	
	RE::SpellItem* GetHazardSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard);

	float GetHazardTargetInterval(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard);

	bool IsHazardFlagSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, std::uint32_t a_flag);

	void SetHazardArt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, RE::BSFixedString a_path);

	void SetHazardFlag(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, std::uint32_t a_flag);

	void SetHazardIMOD(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, RE::TESImageSpaceModifier* a_imod);

	void SetHazardIMODRadius(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, float a_radius);
	
	void SetHazardIPDS(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, RE::BGSImpactDataSet* a_ipds);
	
	void SetHazardLifetime(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, float a_lifetime);
	
	void SetHazardLight(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, RE::TESObjectLIGH* a_light);

	void SetHazardLimit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, std::uint32_t a_limit);
	
	void SetHazardRadius(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, float a_radius);

	void SetHazardSound(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, RE::BGSSoundDescriptorForm* a_sound);

	void SetHazardSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, RE::SpellItem* a_spell);

	void SetHazardTargetInterval(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSHazard* a_hazard, float a_interval);


	bool RegisterFuncs(VM* a_vm);
}