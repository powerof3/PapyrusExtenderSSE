#include <utility>

#include "Papyrus/EffectShader.h"


void papyrusEffectShader::ClearEffectShaderFlag(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, std::uint32_t a_flag)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return;
	}

	a_effectShader->data.flags.reset(static_cast<RE::EffectShaderData::Flags>(a_flag));
}


auto papyrusEffectShader::GetAddonModels(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader) -> RE::BGSDebris*
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	return a_effectShader->data.addonModels;
}


auto papyrusEffectShader::GetEffectShaderTotalCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, bool a_active) -> std::uint32_t
{
	std::uint32_t count = 0;

	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return count;
	}

	if (auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
		processLists->GetMagicEffects([&](RE::BSTempEffect& a_tempEffect) {
			const auto shaderEffect = a_tempEffect.As<RE::ShaderReferenceEffect>();
			if (shaderEffect && shaderEffect->effectData == a_effectShader) {
				if (!a_active || a_active && !shaderEffect->finished) {
					count++;
				}
			}
			return true;
		});
	}

	return count;
}


auto papyrusEffectShader::GetMembraneFillTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader) -> RE::BSFixedString
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return RE::BSFixedString();
	}

	return a_effectShader->fillTexture.textureName;
}


auto papyrusEffectShader::GetMembraneHolesTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader) -> RE::BSFixedString
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return RE::BSFixedString();
	}

	return a_effectShader->holesTexture.textureName;
}


auto papyrusEffectShader::GetMembranePaletteTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader) -> RE::BSFixedString
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return RE::BSFixedString();
	}

	return a_effectShader->membranePaletteTexture.textureName;
}


auto papyrusEffectShader::GetParticleFullCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader) -> float
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_effectShader->data.particleShaderFullParticleBirthRatio;
}


auto papyrusEffectShader::GetParticlePaletteTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader) -> RE::BSFixedString
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return RE::BSFixedString();
	}

	return a_effectShader->particlePaletteTexture.textureName;
}


auto papyrusEffectShader::GetParticlePersistentCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader) -> float
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_effectShader->data.particleShaderPersistantParticleCount;
}


auto papyrusEffectShader::GetParticleShaderTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader) -> RE::BSFixedString
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return RE::BSFixedString();
	}

	return a_effectShader->particleShaderTexture.textureName;
}


auto papyrusEffectShader::IsEffectShaderFlagSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, std::uint32_t a_flag) -> bool
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return false;
	}

	return a_effectShader->data.flags.all(static_cast<RE::EffectShaderData::Flags>(a_flag));
}


void papyrusEffectShader::SetAddonModels(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, RE::BGSDebris* a_debris)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return;
	}

	a_effectShader->data.addonModels = a_debris;
}


void papyrusEffectShader::SetEffectShaderFlag(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, std::uint32_t a_flag)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return;
	}

	a_effectShader->data.flags.set(static_cast<RE::EffectShaderData::Flags>(a_flag));
}


void papyrusEffectShader::SetMembraneColorKeyData(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, std::uint32_t a_colorKey, std::vector<std::int32_t> a_rgb, float a_alpha, float a_time)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
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


void papyrusEffectShader::SetMembraneHolesTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, RE::BSFixedString a_textureName)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return;
	}

	a_effectShader->holesTexture.textureName = std::move(a_textureName);
}


void papyrusEffectShader::SetMembraneFillTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, RE::BSFixedString a_textureName)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return;
	}

	a_effectShader->fillTexture.textureName = std::move(a_textureName);
}


void papyrusEffectShader::SetMembranePaletteTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, RE::BSFixedString a_textureName)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return;
	}

	a_effectShader->membranePaletteTexture.textureName = std::move(a_textureName);
}


void papyrusEffectShader::SetParticleColorKeyData(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, std::uint32_t a_colorKey, std::vector<std::int32_t> a_rgb, float a_alpha, float a_time)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
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


void papyrusEffectShader::SetParticleFullCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, float a_particleCount)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return;
	}

	a_effectShader->data.particleShaderFullParticleBirthRatio = a_particleCount;
}


void papyrusEffectShader::SetParticlePaletteTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, RE::BSFixedString a_textureName)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return;
	}

	a_effectShader->particlePaletteTexture.textureName = std::move(a_textureName);
}


void papyrusEffectShader::SetParticlePersistentCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, float a_particleCount)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return;
	}

	a_effectShader->data.particleShaderPersistantParticleCount = a_particleCount;
}


void papyrusEffectShader::SetParticleShaderTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, RE::BSFixedString a_textureName)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return;
	}

	a_effectShader->particleShaderTexture.textureName = std::move(a_textureName);
}


auto papyrusEffectShader::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusEffectShader - couldn't get VMState"sv);
		return false;
	}

	auto constexpr Functions = "PO3_SKSEFunctions"sv;

	a_vm->RegisterFunction("ClearEffectShaderFlag"sv, Functions, ClearEffectShaderFlag);

	a_vm->RegisterFunction("GetAddonModels"sv, Functions, GetAddonModels, true);

	a_vm->RegisterFunction("GetMembraneHolesTexture"sv, Functions, GetMembraneHolesTexture, true);

	a_vm->RegisterFunction("GetMembraneFillTexture"sv, Functions, GetMembraneFillTexture, true);

	a_vm->RegisterFunction("GetParticleFullCount"sv, Functions, GetParticleFullCount, true);

	a_vm->RegisterFunction("GetMembranePaletteTexture"sv, Functions, GetParticlePaletteTexture, true);

	a_vm->RegisterFunction("GetParticlePaletteTexture"sv, Functions, GetParticlePaletteTexture, true);

	a_vm->RegisterFunction("GetParticlePersistentCount"sv, Functions, GetParticlePersistentCount, true);

	a_vm->RegisterFunction("GetParticleShaderTexture"sv, Functions, GetParticleShaderTexture, true);

	a_vm->RegisterFunction("GetEffectShaderTotalCount"sv, Functions, GetEffectShaderTotalCount);

	a_vm->RegisterFunction("IsEffectShaderFlagSet"sv, Functions, IsEffectShaderFlagSet, true);

	a_vm->RegisterFunction("SetAddonModels"sv, Functions, SetAddonModels);

	a_vm->RegisterFunction("SetEffectShaderFlag"sv, Functions, SetEffectShaderFlag);

	a_vm->RegisterFunction("SetMembraneColorKeyData"sv, Functions, SetMembraneColorKeyData);

	a_vm->RegisterFunction("SetMembraneHolesTexture"sv, Functions, SetMembraneHolesTexture);

	a_vm->RegisterFunction("SetMembraneFillTexture"sv, Functions, SetMembraneFillTexture);

	a_vm->RegisterFunction("SetParticleColorKeyData"sv, Functions, SetParticleColorKeyData);

	a_vm->RegisterFunction("SetParticleFullCount"sv, Functions, SetParticleFullCount);

	a_vm->RegisterFunction("SetParticlePaletteTexture"sv, Functions, SetParticlePaletteTexture);

	a_vm->RegisterFunction("SetParticlePersistentCount"sv, Functions, SetParticlePersistentCount);

	a_vm->RegisterFunction("SetParticleShaderTexture"sv, Functions, SetParticleShaderTexture);

	return true;
}
