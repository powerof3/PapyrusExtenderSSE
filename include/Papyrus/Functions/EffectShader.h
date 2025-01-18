#pragma once

namespace Papyrus::EffectShader
{
	inline void ClearEffectShaderFlag(STATIC_ARGS, RE::TESEffectShader* a_effectShader, std::uint32_t a_flag)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return;
		}

		a_effectShader->data.flags.reset(static_cast<RE::EffectShaderData::Flags>(a_flag));
	}

	inline RE::BGSDebris* GetAddonModels(STATIC_ARGS, const RE::TESEffectShader* a_effectShader)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return nullptr;
		}

		return a_effectShader->data.addonModels;
	}

	inline std::uint32_t GetEffectShaderTotalCount(STATIC_ARGS, RE::TESEffectShader* a_effectShader, bool a_active)
	{
		std::uint32_t count = 0;

		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return count;
		}

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			processLists->ForEachShaderEffect([&](const RE::ShaderReferenceEffect* a_shaderEffect) {
				if (a_shaderEffect->effectData == a_effectShader) {
					if (!a_active || !a_shaderEffect->finished) {
						count++;
					}
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
		}

		return count;
	}

	inline RE::BSFixedString GetMembraneFillTexture(STATIC_ARGS, const RE::TESEffectShader* a_effectShader)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return {};
		}

		return a_effectShader->fillTexture.textureName;
	}

	inline RE::BSFixedString GetMembraneHolesTexture(STATIC_ARGS, const RE::TESEffectShader* a_effectShader)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return {};
		}

		return a_effectShader->holesTexture.textureName;
	}

	inline RE::BSFixedString GetMembranePaletteTexture(STATIC_ARGS, const RE::TESEffectShader* a_effectShader)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return {};
		}

		return a_effectShader->membranePaletteTexture.textureName;
	}

	inline float GetParticleFullCount(STATIC_ARGS, const RE::TESEffectShader* a_effectShader)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return 0.0f;
		}

		return a_effectShader->data.particleShaderFullParticleBirthRatio;
	}

	inline RE::BSFixedString GetParticlePaletteTexture(STATIC_ARGS, const RE::TESEffectShader* a_effectShader)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return {};
		}

		return a_effectShader->particlePaletteTexture.textureName;
	}

	inline float GetParticlePersistentCount(STATIC_ARGS, const RE::TESEffectShader* a_effectShader)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return 0.0f;
		}

		return a_effectShader->data.particleShaderPersistantParticleCount;
	}

	inline RE::BSFixedString GetParticleShaderTexture(STATIC_ARGS, const RE::TESEffectShader* a_effectShader)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return {};
		}

		return a_effectShader->particleShaderTexture.textureName;
	}

	inline bool IsEffectShaderFlagSet(STATIC_ARGS, const RE::TESEffectShader* a_effectShader, std::uint32_t a_flag)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return false;
		}

		return a_effectShader->data.flags.all(static_cast<RE::EffectShaderData::Flags>(a_flag));
	}

	inline void SetAddonModels(STATIC_ARGS, RE::TESEffectShader* a_effectShader, RE::BGSDebris* a_debris)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return;
		}

		a_effectShader->data.addonModels = a_debris;
	}

	inline void SetEffectShaderFlag(STATIC_ARGS, RE::TESEffectShader* a_effectShader, std::uint32_t a_flag)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return;
		}

		a_effectShader->data.flags.set(static_cast<RE::EffectShaderData::Flags>(a_flag));
	}

	inline void SetMembraneColorKeyData(STATIC_ARGS, RE::TESEffectShader* a_effectShader, std::uint32_t a_colorKey, std::vector<std::int32_t> a_rgb, float a_alpha, float a_time)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return;
		}

		const auto get_key_data = [a_colorKey, a_effectShader]() -> std::tuple<RE::Color&, float&, float&> {
			if (a_colorKey == 1) {
				return { a_effectShader->data.fillTextureEffectColorKey1, a_effectShader->data.fillTextureEffectColorKeyScaleTimeColorKey1Scale, a_effectShader->data.fillTextureEffectColorKeyScaleTimeColorKey1Time };
			}
			if (a_colorKey == 2) {
				return { a_effectShader->data.fillTextureEffectColorKey2, a_effectShader->data.fillTextureEffectColorKeyScaleTimeColorKey2Scale, a_effectShader->data.fillTextureEffectColorKeyScaleTimeColorKey2Time };
			}
			return { a_effectShader->data.fillTextureEffectColorKey3, a_effectShader->data.fillTextureEffectColorKeyScaleTimeColorKey3Scale, a_effectShader->data.fillTextureEffectColorKeyScaleTimeColorKey3Time };
		};

		auto [color, alpha, time] = get_key_data();

		if (!a_rgb.empty()) {
			for (std::size_t i = 0; i < 3; ++i) {
				color[i] = static_cast<std::uint8_t>(a_rgb[i]);
			}
		}

		if (a_alpha != -1.0f) {
			alpha = a_alpha;
		}

		if (a_time != -1.0f) {
			time = a_time;
		}
	}

	inline void SetMembraneHolesTexture(STATIC_ARGS, RE::TESEffectShader* a_effectShader, RE::BSFixedString a_textureName)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return;
		}

		a_effectShader->holesTexture.textureName = std::move(a_textureName);
	}

	inline void SetMembraneFillTexture(STATIC_ARGS, RE::TESEffectShader* a_effectShader, RE::BSFixedString a_textureName)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return;
		}

		a_effectShader->fillTexture.textureName = std::move(a_textureName);
	}

	inline void SetMembranePaletteTexture(STATIC_ARGS, RE::TESEffectShader* a_effectShader, RE::BSFixedString a_textureName)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return;
		}

		a_effectShader->membranePaletteTexture.textureName = std::move(a_textureName);
	}

	inline void SetParticleColorKeyData(STATIC_ARGS, RE::TESEffectShader* a_effectShader, std::uint32_t a_colorKey, std::vector<std::int32_t> a_rgb, float a_alpha, float a_time)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return;
		}

		const auto get_key_data = [a_colorKey, a_effectShader]() -> std::tuple<RE::Color&, float&, float&> {
			if (a_colorKey == 1) {
				return { a_effectShader->data.colorKey1, a_effectShader->data.colorKey1ColorAlpha, a_effectShader->data.colorKey1ColorKeyTime };
			}
			if (a_colorKey == 2) {
				return { a_effectShader->data.colorKey2, a_effectShader->data.colorKey2ColorAlpha, a_effectShader->data.colorKey2ColorKeyTime };
			}
			return { a_effectShader->data.colorKey3, a_effectShader->data.colorKey3ColorAlpha, a_effectShader->data.colorKey3ColorKeyTime };
		};

		auto [color, alpha, time] = get_key_data();

		if (!a_rgb.empty()) {
			for (std::size_t i = 0; i < 3; ++i) {
				color[i] = static_cast<std::uint8_t>(a_rgb[i]);
			}
		}

		if (a_alpha != -1.0f) {
			alpha = a_alpha;
		}

		if (a_time != -1.0f) {
			time = a_time;
		}
	}

	inline void SetParticleFullCount(STATIC_ARGS, RE::TESEffectShader* a_effectShader, float a_particleCount)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return;
		}

		a_effectShader->data.particleShaderFullParticleBirthRatio = a_particleCount;
	}

	inline void SetParticlePaletteTexture(STATIC_ARGS, RE::TESEffectShader* a_effectShader, RE::BSFixedString a_textureName)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return;
		}

		a_effectShader->particlePaletteTexture.textureName = std::move(a_textureName);
	}

	inline void SetParticlePersistentCount(STATIC_ARGS, RE::TESEffectShader* a_effectShader, float a_particleCount)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return;
		}

		a_effectShader->data.particleShaderPersistantParticleCount = a_particleCount;
	}

	inline void SetParticleShaderTexture(STATIC_ARGS, RE::TESEffectShader* a_effectShader, RE::BSFixedString a_textureName)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return;
		}

		a_effectShader->particleShaderTexture.textureName = std::move(a_textureName);
	}

	inline void Bind(VM& a_vm)
	{
		BIND(ClearEffectShaderFlag);
		BIND(GetAddonModels, true);
		BIND(GetMembraneHolesTexture, true);
		BIND(GetMembraneFillTexture, true);
		BIND(GetParticleFullCount, true);
		BIND(GetParticlePaletteTexture, true);
		BIND(GetParticlePersistentCount, true);
		BIND(GetParticleShaderTexture, true);
		BIND(GetEffectShaderTotalCount);
		BIND(IsEffectShaderFlagSet);
		BIND(SetAddonModels);
		BIND(SetEffectShaderFlag);
		BIND(SetMembraneColorKeyData);
		BIND(SetMembraneHolesTexture);
		BIND(SetMembraneFillTexture);
		BIND(SetParticleColorKeyData);
		BIND(SetParticleFullCount);
		BIND(SetParticlePaletteTexture);
		BIND(SetParticlePersistentCount);
		BIND(SetParticleShaderTexture);

		logger::info("Registered effect shader functions"sv);
	}
}
