#include "Papyrus/MagicEffect.h"


auto papyrusMagicEffect::GetAssociatedForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef) -> RE::TESForm*
{
	if (!a_mgef) {
		a_vm->TraceStack("MagicEffect is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	return a_mgef->data.associatedForm;
}


auto papyrusMagicEffect::GetEffectArchetypeAsInt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef) -> std::uint32_t
{
	if (!a_mgef) {
		a_vm->TraceStack("MagicEffect is None", a_stackID, Severity::kWarning);
		return 0;
	}

	return to_underlying(a_mgef->data.archetype);
}


auto papyrusMagicEffect::GetEffectArchetypeAsString(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef) -> RE::BSFixedString
{
	if (!a_mgef) {
		a_vm->TraceStack("MagicEffect is None", a_stackID, Severity::kWarning);
		return RE::BSFixedString();
	}

	return a_mgef->GetArchetypeAsString();
}


auto papyrusMagicEffect::GetMagicEffectSound(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef, std::uint32_t a_type) -> RE::BGSSoundDescriptorForm*
{
	if (!a_mgef) {
		a_vm->TraceStack("MagicEffect is None", a_stackID, Severity::kWarning);
		return nullptr;
	}
	if (a_type > 6) {
		a_vm->TraceStack("Sound index is invalid", a_stackID, Severity::kWarning);
		return nullptr;
	}

	const auto soundID = static_cast<RE::MagicSystem::SoundID>(a_type);
	for (const auto& effectSound : a_mgef->effectSounds) {
		if (effectSound.id == soundID) {
			return effectSound.sound;
		}
	}

	return nullptr;
}


auto papyrusMagicEffect::GetPrimaryActorValue(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef) -> RE::BSFixedString
{
	if (!a_mgef) {
		a_vm->TraceStack("MagicEffect is None", a_stackID, Severity::kWarning);
		return RE::BSFixedString();
	}

	RE::BSFixedString avStr;

	if (auto actorValueList = RE::ActorValueList::GetSingleton(); actorValueList) {
		const auto actorValue = actorValueList->GetActorValue(a_mgef->data.primaryAV);
		if (actorValue) {
			avStr = actorValue->enumName;
		}
	}

	return avStr;
}


auto papyrusMagicEffect::GetSecondaryActorValue(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef) -> RE::BSFixedString
{
	if (!a_mgef) {
		a_vm->TraceStack("MagicEffect is None", a_stackID, Severity::kWarning);
		return RE::BSFixedString();
	}

	RE::BSFixedString avStr;

	if (auto actorValueList = RE::ActorValueList::GetSingleton(); actorValueList) {
		const auto actorValue = actorValueList->GetActorValue(a_mgef->data.secondaryAV);
		if (actorValue) {
			avStr = actorValue->enumName;
		}
	}

	return avStr;
}


void papyrusMagicEffect::SetMagicEffectSound(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef, RE::BGSSoundDescriptorForm* a_sound, std::uint32_t a_type)
{
	if (!a_mgef) {
		a_vm->TraceStack("MagicEffect is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_sound) {
		a_vm->TraceStack("Sound is None", a_stackID, Severity::kWarning);
		return;
	}
	if (a_type > 6) {
		a_vm->TraceStack("Sound index is invalid", a_stackID, Severity::kWarning);
		return;
	}

	const auto soundID = static_cast<RE::MagicSystem::SoundID>(a_type);
	for (auto& effectSound : a_mgef->effectSounds) {
		if (effectSound.id == soundID) {
			effectSound.sound = a_sound;
			break;
		}
	}
}


auto papyrusMagicEffect::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusMagicEffect - couldn't get VMState"sv);
		return false;
	}

	auto constexpr Functions = "PO3_SKSEFunctions"sv;

	a_vm->RegisterFunction("GetAssociatedForm"sv, Functions, GetAssociatedForm, true);

	a_vm->RegisterFunction("GetEffectArchetypeAsInt"sv, Functions, GetEffectArchetypeAsInt, true);

	a_vm->RegisterFunction("GetEffectArchetypeAsString"sv, Functions, GetEffectArchetypeAsString, true);

	a_vm->RegisterFunction("GetPrimaryActorValue"sv, Functions, GetPrimaryActorValue, true);

	a_vm->RegisterFunction("GetSecondaryActorValue"sv, Functions, GetSecondaryActorValue, true);

	a_vm->RegisterFunction("GetMagicEffectSound"sv, Functions, GetMagicEffectSound);

	a_vm->RegisterFunction("SetMagicEffectSound"sv, Functions, SetMagicEffectSound);

	return true;
}
