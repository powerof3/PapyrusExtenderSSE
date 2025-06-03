#pragma once

namespace Papyrus::EffectShader
{
	void              ClearEffectShaderFlag(STATIC_ARGS, RE::TESEffectShader* a_effectShader, std::uint32_t a_flag);
	RE::BGSDebris*    GetAddonModels(STATIC_ARGS, const RE::TESEffectShader* a_effectShader);
	std::uint32_t     GetEffectShaderTotalCount(STATIC_ARGS, RE::TESEffectShader* a_effectShader, bool a_active);
	RE::BSFixedString GetMembraneFillTexture(STATIC_ARGS, const RE::TESEffectShader* a_effectShader);
	RE::BSFixedString GetMembraneHolesTexture(STATIC_ARGS, const RE::TESEffectShader* a_effectShader);
	RE::BSFixedString GetMembranePaletteTexture(STATIC_ARGS, const RE::TESEffectShader* a_effectShader);
	float             GetParticleFullCount(STATIC_ARGS, const RE::TESEffectShader* a_effectShader);
	RE::BSFixedString GetParticlePaletteTexture(STATIC_ARGS, const RE::TESEffectShader* a_effectShader);
	float             GetParticlePersistentCount(STATIC_ARGS, const RE::TESEffectShader* a_effectShader);
	RE::BSFixedString GetParticleShaderTexture(STATIC_ARGS, const RE::TESEffectShader* a_effectShader);
	bool              IsEffectShaderFlagSet(STATIC_ARGS, const RE::TESEffectShader* a_effectShader, std::uint32_t a_flag);
	void              SetAddonModels(STATIC_ARGS, RE::TESEffectShader* a_effectShader, RE::BGSDebris* a_debris);
	void              SetEffectShaderFlag(STATIC_ARGS, RE::TESEffectShader* a_effectShader, std::uint32_t a_flag);
	void              SetMembraneColorKeyData(STATIC_ARGS, RE::TESEffectShader* a_effectShader, std::uint32_t a_colorKey, std::vector<std::int32_t> a_rgb, float a_alpha, float a_time);
	void              SetMembraneHolesTexture(STATIC_ARGS, RE::TESEffectShader* a_effectShader, RE::BSFixedString a_textureName);
	void              SetMembraneFillTexture(STATIC_ARGS, RE::TESEffectShader* a_effectShader, RE::BSFixedString a_textureName);
	void              SetMembranePaletteTexture(STATIC_ARGS, RE::TESEffectShader* a_effectShader, RE::BSFixedString a_textureName);
	void              SetParticleColorKeyData(STATIC_ARGS, RE::TESEffectShader* a_effectShader, std::uint32_t a_colorKey, std::vector<std::int32_t> a_rgb, float a_alpha, float a_time);
	void              SetParticleFullCount(STATIC_ARGS, RE::TESEffectShader* a_effectShader, float a_particleCount);
	void              SetParticlePaletteTexture(STATIC_ARGS, RE::TESEffectShader* a_effectShader, RE::BSFixedString a_textureName);
	void              SetParticlePersistentCount(STATIC_ARGS, RE::TESEffectShader* a_effectShader, float a_particleCount);
	void              SetParticleShaderTexture(STATIC_ARGS, RE::TESEffectShader* a_effectShader, RE::BSFixedString a_textureName);

	void Bind(VM& a_vm);
}
