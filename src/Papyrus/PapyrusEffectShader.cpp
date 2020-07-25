#include "Papyrus/PapyrusEffectShader.h"


void papyrusEffectShader::ClearEffectShaderFlag(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, std::uint32_t a_flag)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return;
	}

	a_effectShader->data.flags &= ~static_cast<RE::EffectShaderData::Flags>(a_flag);
}


float papyrusEffectShader::GetEffectShaderFullParticleCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_effectShader->data.particleShaderFullParticleBirthRatio;
}


float papyrusEffectShader::GetEffectShaderPersistentParticleCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_effectShader->data.particleShaderPersistantParticleCount;
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


bool papyrusEffectShader::IsEffectShaderFlagSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, std::uint32_t a_flag)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return false;
	}

	return (to_underlying(a_effectShader->data.flags.get()) & a_flag) == a_flag;
}


void papyrusEffectShader::SetEffectShaderFlag(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, std::uint32_t a_flag)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return;
	}

	a_effectShader->data.flags |= static_cast<RE::EffectShaderData::Flags>(a_flag);
}


void papyrusEffectShader::SetEffectShaderFullParticleCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, float a_particleCount)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return;
	}

	a_effectShader->data.particleShaderFullParticleBirthRatio = a_particleCount;
}


void papyrusEffectShader::SetEffectShaderPersistentParticleCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESEffectShader* a_effectShader, float a_particleCount)
{
	if (!a_effectShader) {
		a_vm->TraceStack("EffectShader is None", a_stackID, Severity::kWarning);
		return;
	}

	a_effectShader->data.particleShaderPersistantParticleCount = a_particleCount;
}


bool papyrusEffectShader::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("papyrusEffectShader - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("ClearEffectShaderFlag", "PO3_SKSEFunctions", ClearEffectShaderFlag);

	a_vm->RegisterFunction("GetEffectShaderFullParticleCount", "PO3_SKSEFunctions", GetEffectShaderFullParticleCount, true);

	a_vm->RegisterFunction("GetEffectShaderPersistentParticleCount", "PO3_SKSEFunctions", GetEffectShaderPersistentParticleCount, true);

	a_vm->RegisterFunction("GetEffectShaderTotalCount", "PO3_SKSEFunctions", GetEffectShaderTotalCount);

	a_vm->RegisterFunction("IsEffectShaderFlagSet", "PO3_SKSEFunctions", IsEffectShaderFlagSet, true);

	a_vm->RegisterFunction("SetEffectShaderFlag", "PO3_SKSEFunctions", SetEffectShaderFlag);

	a_vm->RegisterFunction("SetEffectShaderFullParticleCount", "PO3_SKSEFunctions", SetEffectShaderFullParticleCount);

	a_vm->RegisterFunction("SetEffectShaderPersistentParticleCount", "PO3_SKSEFunctions", SetEffectShaderPersistentParticleCount);

	return true;
}
