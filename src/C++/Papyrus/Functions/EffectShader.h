#pragma once

namespace Papyrus::EffectShader
{
	inline void ClearEffectShaderFlag(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESEffectShader* a_effectShader,
		std::uint32_t a_flag)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return;
		}

		a_effectShader->data.flags.reset(static_cast<RE::EffectShaderData::Flags>(a_flag));
	}

	inline RE::BGSDebris* GetAddonModels(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESEffectShader* a_effectShader)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return nullptr;
		}

		return a_effectShader->data.addonModels;
	}

	inline std::uint32_t GetEffectShaderTotalCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, bool a_active)
	{
		std::uint32_t count = 0;

		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return count;
		}

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			processLists->GetShaderEffects([&](const RE::ShaderReferenceEffect& a_shaderEffect) {
				if (a_shaderEffect.effectData == a_effectShader) {
					if (!a_active || !a_shaderEffect.finished) {
						count++;
					}
				}
				return true;
			});
		}

		return count;
	}

	inline RE::BSFixedString GetMembraneFillTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESEffectShader* a_effectShader)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return RE::BSFixedString();
		}

		return a_effectShader->fillTexture.textureName;
	}

	inline RE::BSFixedString GetMembraneHolesTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESEffectShader* a_effectShader)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return RE::BSFixedString();
		}

		return a_effectShader->holesTexture.textureName;
	}

	inline RE::BSFixedString GetMembranePaletteTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESEffectShader* a_effectShader)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return RE::BSFixedString();
		}

		return a_effectShader->membranePaletteTexture.textureName;
	}

	inline float GetParticleFullCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESEffectShader* a_effectShader)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return 0.0f;
		}

		return a_effectShader->data.particleShaderFullParticleBirthRatio;
	}

	inline RE::BSFixedString GetParticlePaletteTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESEffectShader* a_effectShader)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return RE::BSFixedString();
		}

		return a_effectShader->particlePaletteTexture.textureName;
	}

	inline float GetParticlePersistentCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESEffectShader* a_effectShader)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return 0.0f;
		}

		return a_effectShader->data.particleShaderPersistantParticleCount;
	}

	inline RE::BSFixedString GetParticleShaderTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESEffectShader* a_effectShader)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return RE::BSFixedString();
		}

		return a_effectShader->particleShaderTexture.textureName;
	}

	inline bool IsEffectShaderFlagSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::TESEffectShader* a_effectShader,
		std::uint32_t a_flag)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return false;
		}

		return a_effectShader->data.flags.all(static_cast<RE::EffectShaderData::Flags>(a_flag));
	}

	inline void SetAddonModels(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESEffectShader* a_effectShader,
		RE::BGSDebris* a_debris)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return;
		}

		a_effectShader->data.addonModels = a_debris;
	}

	inline void SetEffectShaderFlag(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESEffectShader* a_effectShader,
		std::uint32_t a_flag)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return;
		}

		a_effectShader->data.flags.set(static_cast<RE::EffectShaderData::Flags>(a_flag));
	}

	inline void SetMembraneColorKeyData(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESEffectShader* a_effectShader,
		std::uint32_t a_colorKey,
		std::vector<std::int32_t> a_rgb,
		float a_alpha,
		float a_time)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return;
		}

		switch (a_colorKey) {
		case 1:
			{
				auto& color = a_effectShader->data.fillTextureEffectColorKey1;
				for (std::size_t i = 0; i < 3; ++i) {
					color[i] = static_cast<std::uint8_t>(a_rgb[i]);
				}
				a_effectShader->data.fillTextureEffectColorKeyScaleTimeColorKey1Scale = a_alpha;
				a_effectShader->data.fillTextureEffectColorKeyScaleTimeColorKey1Time = a_time;
			}
			break;
		case 2:
			{
				auto& color = a_effectShader->data.fillTextureEffectColorKey2;
				for (std::size_t i = 0; i < 3; ++i) {
					color[i] = static_cast<std::uint8_t>(a_rgb[i]);
				}
				a_effectShader->data.fillTextureEffectColorKeyScaleTimeColorKey2Scale = a_alpha;
				a_effectShader->data.fillTextureEffectColorKeyScaleTimeColorKey2Time = a_time;
			}
			break;
		case 3:
			{
				auto& color = a_effectShader->data.fillTextureEffectColorKey3;
				for (std::size_t i = 0; i < 3; ++i) {
					color[i] = static_cast<std::uint8_t>(a_rgb[i]);
				}
				a_effectShader->data.fillTextureEffectColorKeyScaleTimeColorKey3Scale = a_alpha;
				a_effectShader->data.fillTextureEffectColorKeyScaleTimeColorKey3Time = a_time;
			}
			break;
		default:
			break;
		}
	}

	inline void SetMembraneHolesTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESEffectShader* a_effectShader,
		RE::BSFixedString a_textureName)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return;
		}

		a_effectShader->holesTexture.textureName = std::move(a_textureName);
	}

	inline void SetMembraneFillTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESEffectShader* a_effectShader,
		RE::BSFixedString a_textureName)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return;
		}

		a_effectShader->fillTexture.textureName = std::move(a_textureName);
	}

	inline void SetMembranePaletteTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESEffectShader* a_effectShader,
		RE::BSFixedString a_textureName)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return;
		}

		a_effectShader->membranePaletteTexture.textureName = std::move(a_textureName);
	}

	inline void SetParticleColorKeyData(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESEffectShader* a_effectShader,
		std::uint32_t a_colorKey,
		std::vector<std::int32_t> a_rgb,
		float a_alpha,
		float a_time)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return;
		}

		switch (a_colorKey) {
		case 1:
			{
				auto& color = a_effectShader->data.colorKey1;
				for (std::size_t i = 0; i < 3; ++i) {
					color[i] = static_cast<std::uint8_t>(a_rgb[i]);
				}
				a_effectShader->data.colorKey1ColorAlpha = a_alpha;
				a_effectShader->data.colorKey1ColorKeyTime = a_time;
			}
			break;
		case 2:
			{
				auto& color = a_effectShader->data.colorKey2;
				for (std::size_t i = 0; i < 3; ++i) {
					color[i] = static_cast<std::uint8_t>(a_rgb[i]);
				}
				a_effectShader->data.colorKey2ColorAlpha = a_alpha;
				a_effectShader->data.colorKey2ColorKeyTime = a_time;
			}
			break;
		case 3:
			{
				auto& color = a_effectShader->data.colorKey3;
				for (std::size_t i = 0; i < 3; ++i) {
					color[i] = static_cast<std::uint8_t>(a_rgb[i]);
				}
				a_effectShader->data.colorKey3ColorAlpha = a_alpha;
				a_effectShader->data.colorKey3ColorKeyTime = a_time;
			}
			break;
		default:
			break;
		}
	}

	inline void SetParticleFullCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESEffectShader* a_effectShader,
		float a_particleCount)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return;
		}

		a_effectShader->data.particleShaderFullParticleBirthRatio = a_particleCount;
	}

	inline void SetParticlePaletteTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESEffectShader* a_effectShader,
		RE::BSFixedString a_textureName)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return;
		}

		a_effectShader->particlePaletteTexture.textureName = std::move(a_textureName);
	}

	inline void SetParticlePersistentCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESEffectShader* a_effectShader,
		float a_particleCount)
	{
		if (!a_effectShader) {
			a_vm->TraceStack("EffectShader is None", a_stackID);
			return;
		}

		a_effectShader->data.particleShaderPersistantParticleCount = a_particleCount;
	}

	inline void SetParticleShaderTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESEffectShader* a_effectShader,
		RE::BSFixedString a_textureName)
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
