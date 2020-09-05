#include "Papyrus/PapyrusEffectShader.h"


void papyrusEffectShader::ClearEffectShaderFlag(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, std::uint32_t a_flag)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return;
	}

	a_effectShader->data.flags.reset(static_cast<RE::EffectShaderData::Flags>(a_flag));
}


std::uint32_t papyrusEffectShader::GetEffectShaderTotalCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, bool a_active)
{
	std::uint32_t count = 0;

	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return count;
	}

	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		processLists->GetMagicEffects([&](RE::BSTempEffect* a_tempEffect) {
			auto shaderEffect = a_tempEffect->As<RE::ShaderReferenceEffect>();
			if (shaderEffect) {
				auto effectData = shaderEffect->effectData;
				if (effectData && effectData == a_effectShader) {
					if (!a_active || a_active && !shaderEffect->finished) {
						count++;
					}
				}
			}
			return true;
		});
	}

	return count;
}


RE::BSFixedString papyrusEffectShader::GetMembraneFillTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return "";
	}

	return a_effectShader->fillTexture.textureName;
}


RE::BSFixedString papyrusEffectShader::GetMembraneHolesTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return "";
	}

	return a_effectShader->holesTexture.textureName;
}


RE::BSFixedString papyrusEffectShader::GetMembranePaletteTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return "";
	}

	return a_effectShader->membranePaletteTexture.textureName;
}


float papyrusEffectShader::GetParticleFullCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_effectShader->data.particleShaderFullParticleBirthRatio;
}


RE::BSFixedString papyrusEffectShader::GetParticlePaletteTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return "";
	}

	return a_effectShader->particlePaletteTexture.textureName;
}


float papyrusEffectShader::GetParticlePersistentCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_effectShader->data.particleShaderPersistantParticleCount;
}


RE::BSFixedString papyrusEffectShader::GetParticleShaderTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return "";
	}

	return a_effectShader->particleShaderTexture.textureName;
}


bool papyrusEffectShader::IsEffectShaderFlagSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, std::uint32_t a_flag)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return false;
	}

	return a_effectShader->data.flags.all(static_cast<RE::EffectShaderData::Flags>(a_flag));
}


void papyrusEffectShader::SetEffectShaderFlag(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, std::uint32_t a_flag)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return;
	}

	a_effectShader->data.flags.set(static_cast<RE::EffectShaderData::Flags>(a_flag));
}


void papyrusEffectShader::SetMembraneColorKeyData(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, std::uint32_t a_colorKey, std::vector<std::uint32_t> a_rgb, float a_alpha, float a_time)
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
				color[i] = a_rgb[i];
			}
			a_effectShader->data.fillTextureEffectColorKeyScaleTimeColorKey1Scale = a_alpha;
			a_effectShader->data.fillTextureEffectColorKeyScaleTimeColorKey1Time = a_time;
		}
		break;
	case 2:
		{
			auto& color = a_effectShader->data.fillTextureEffectColorKey2;
			for (std::size_t i = 0; i < 3; ++i) {
				color[i] = a_rgb[i];
			}
			a_effectShader->data.fillTextureEffectColorKeyScaleTimeColorKey2Scale = a_alpha;
			a_effectShader->data.fillTextureEffectColorKeyScaleTimeColorKey2Time = a_time;
		}
		break;
	case 3:
		{
			auto& color = a_effectShader->data.fillTextureEffectColorKey3;
			for (std::size_t i = 0; i < 3; ++i) {
				color[i] = a_rgb[i];
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

	a_effectShader->holesTexture.textureName = a_textureName;
}


void papyrusEffectShader::SetMembraneFillTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, RE::BSFixedString a_textureName)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return;
	}

	a_effectShader->fillTexture.textureName = a_textureName;
}


void papyrusEffectShader::SetMembranePaletteTexture(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, RE::BSFixedString a_textureName)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return;
	}

	a_effectShader->membranePaletteTexture.textureName = a_textureName;
}


void papyrusEffectShader::SetParticleColorKeyData(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, std::uint32_t a_colorKey, std::vector<std::uint32_t> a_rgb, float a_alpha, float a_time)
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
				color[i] = a_rgb[i];
			}
			a_effectShader->data.colorKey1ColorAlpha = a_alpha;
			a_effectShader->data.colorKey1ColorKeyTime = a_time;
		}
		break;
	case 2:
		{
			auto& color = a_effectShader->data.colorKey2;
			for (std::size_t i = 0; i < 3; ++i) {
				color[i] = a_rgb[i];
			}
			a_effectShader->data.colorKey2ColorAlpha = a_alpha;
			a_effectShader->data.colorKey2ColorKeyTime = a_time;
		}
		break;
	case 3:
		{
			auto& color = a_effectShader->data.colorKey3;
			for (std::size_t i = 0; i < 3; ++i) {
				color[i] = a_rgb[i];
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

	a_effectShader->particlePaletteTexture.textureName = a_textureName;
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

	a_effectShader->particleShaderTexture.textureName = a_textureName;
}


bool papyrusEffectShader::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("papyrusEffectShader - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("ClearEffectShaderFlag", "PO3_SKSEFunctions", ClearEffectShaderFlag);

	a_vm->RegisterFunction("GetMembraneHolesTexture", "PO3_SKSEFunctions", GetMembraneHolesTexture, true);

	a_vm->RegisterFunction("GetMembraneFillTexture", "PO3_SKSEFunctions", GetMembraneFillTexture, true);

	a_vm->RegisterFunction("GetParticleFullCount", "PO3_SKSEFunctions", GetParticleFullCount, true);

	a_vm->RegisterFunction("GetMembranePaletteTexture", "PO3_SKSEFunctions", GetParticlePaletteTexture, true);

	a_vm->RegisterFunction("GetParticlePaletteTexture", "PO3_SKSEFunctions", GetParticlePaletteTexture, true);

	a_vm->RegisterFunction("GetParticlePersistentCount", "PO3_SKSEFunctions", GetParticlePersistentCount, true);

	a_vm->RegisterFunction("GetParticleShaderTexture", "PO3_SKSEFunctions", GetParticleShaderTexture, true);

	a_vm->RegisterFunction("GetEffectShaderTotalCount", "PO3_SKSEFunctions", GetEffectShaderTotalCount);

	a_vm->RegisterFunction("IsEffectShaderFlagSet", "PO3_SKSEFunctions", IsEffectShaderFlagSet, true);

	a_vm->RegisterFunction("SetEffectShaderFlag", "PO3_SKSEFunctions", SetEffectShaderFlag);

	a_vm->RegisterFunction("SetMembraneColorKeyData", "PO3_SKSEFunctions", SetMembraneColorKeyData);

	a_vm->RegisterFunction("SetMembraneHolesTexture", "PO3_SKSEFunctions", SetMembraneHolesTexture);

	a_vm->RegisterFunction("SetMembraneFillTexture", "PO3_SKSEFunctions", SetMembraneFillTexture);

	a_vm->RegisterFunction("SetParticleColorKeyData", "PO3_SKSEFunctions", SetParticleColorKeyData);

	a_vm->RegisterFunction("SetParticleFullCount", "PO3_SKSEFunctions", SetParticleFullCount);

	a_vm->RegisterFunction("SetParticlePaletteTexture", "PO3_SKSEFunctions", SetParticlePaletteTexture);

	a_vm->RegisterFunction("SetParticlePersistentCount", "PO3_SKSEFunctions", SetParticlePersistentCount);

	a_vm->RegisterFunction("SetParticleShaderTexture", "PO3_SKSEFunctions", SetParticleShaderTexture);

	return true;
}
