#pragma once


namespace papyrusEffectShader
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;


	void ClearEffectShaderFlag(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, std::uint32_t a_flag);

	RE::BGSDebris* GetAddonModels(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader);
	
	std::uint32_t GetEffectShaderTotalCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, bool a_active);

	RE::BSFixedString GetMembraneFillTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader);

	RE::BSFixedString GetMembraneHolesTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader);
	
	RE::BSFixedString GetMembranePaletteTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader);
	
	float GetParticleFullCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader);

	RE::BSFixedString GetParticlePaletteTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader);
	
	float GetParticlePersistentCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader);

	RE::BSFixedString GetParticleShaderTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader);

	bool IsEffectShaderFlagSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, std::uint32_t a_flag);

	void SetAddonModels(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, RE::BGSDebris* a_debris);
	
	void SetEffectShaderFlag(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, std::uint32_t a_flag);

	void SetMembraneColorKeyData(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, std::uint32_t a_colorKey, std::vector<std::int32_t> a_rgb, float a_alpha, float a_time);

	void SetParticleFullCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, float a_particleCount);

	void SetParticlePersistentCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, float a_particleCount);

	void SetMembraneHolesTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, RE::BSFixedString a_textureName);

	void SetMembraneFillTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, RE::BSFixedString a_textureName);

	void SetMembranePaletteTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, RE::BSFixedString a_textureName);

	void SetParticleColorKeyData(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, std::uint32_t a_colorKey, std::vector<std::int32_t> a_rgb, float a_alpha, float a_time);

	void SetParticlePaletteTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, RE::BSFixedString a_textureName);

	void SetParticleShaderTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, RE::BSFixedString a_textureName);


	bool RegisterFuncs(VM* a_vm);

}