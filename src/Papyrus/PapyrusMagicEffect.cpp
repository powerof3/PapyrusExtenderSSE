#include "Papyrus/PapyrusMagicEffect.h"

RE::TESForm* papyrusMagicEffect::GetAssociatedForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef)
{
	if (!a_mgef) {
		a_vm->TraceStack("Cannot get associated form on a None magic effect", a_stackID, Severity::kWarning);
		return nullptr;
	}
	return a_mgef->data.associatedForm;
}

UInt32 papyrusMagicEffect::GetEffectArchetypeAsInt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef)
{
	if (!a_mgef) {
		a_vm->TraceStack("Cannot get archetype on a None magic effect", a_stackID, Severity::kWarning);
		return 0;
	}
	return to_underlying(a_mgef->data.archetype);
}


RE::BSFixedString papyrusMagicEffect::GetEffectArchetypeAsString(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef)
{
	if (!a_mgef) {
		a_vm->TraceStack("Cannot get archetype on a None magic effect", a_stackID, Severity::kWarning);
		return RE::BSFixedString();
	}
	return a_mgef->GetArchetypeAsString();
}


RE::BGSSoundDescriptorForm* papyrusMagicEffect::GetMagicEffectSound(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef, UInt32 a_type)
{
	if (!a_mgef) {
		a_vm->TraceStack("Cannot get sound on a None magic effect", a_stackID, Severity::kWarning);
		return nullptr;
	}
	if (a_type < 0 || a_type > 6) {
		a_vm->TraceStack("Sound index is invalid", a_stackID, Severity::kWarning);
		return nullptr;
	}
	auto soundID = static_cast<RE::MagicSystem::SoundID>(a_type);
	for (auto& effectSound : a_mgef->effectSounds) {
		if (effectSound.id == soundID) {
			return effectSound.sound;
		}
	}
	return nullptr;
}


RE::BSFixedString papyrusMagicEffect::GetPrimaryActorValue(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef)
{
	if (!a_mgef) {
		a_vm->TraceStack("Cannot get primary AV on a None magic effect", a_stackID, Severity::kWarning);
		return "";
	}
	auto actorValueList = RE::ActorValueList::GetSingleton();
	if (actorValueList) {
		auto av = actorValueList->GetActorValue(a_mgef->data.primaryAV);
		{
			if (av) {
				return av->enumName;
			}
		}
	}
	return "";
}


RE::BSFixedString papyrusMagicEffect::GetSecondaryActorValue(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef)
{
	if (!a_mgef) {
		a_vm->TraceStack("Cannot get secondary AV on a None magic effect", a_stackID, Severity::kWarning);
		return "";
	}
	auto actorValueList = RE::ActorValueList::GetSingleton();
	if (actorValueList) {
		auto av = actorValueList->GetActorValue(a_mgef->data.secondaryAV);
		{
			if (av) {
				return av->enumName;
			}
		}
	}
	return "";
}


void papyrusMagicEffect::SetMagicEffectSound(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef, RE::BGSSoundDescriptorForm* a_sound, UInt32 a_type)
{
	if (!a_mgef) {
		a_vm->TraceStack("Cannot set sound on a None magic effect", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_sound) {
		a_vm->TraceStack("Cannot set a None sound", a_stackID, Severity::kWarning);
		return;
	}
	if (a_type < 0 || a_type > 6) {
		a_vm->TraceStack("Sound index is invalid", a_stackID, Severity::kWarning);
		return;
	}
	auto soundID = static_cast<RE::MagicSystem::SoundID>(a_type);
	for (auto& effectSound : a_mgef->effectSounds) {
		if (effectSound.id == soundID) {
			effectSound.sound = a_sound;
			break;
		}
	}
}


bool papyrusMagicEffect::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		_MESSAGE("papyrusMagicEffect - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("GetAssociatedForm", "PO3_SKSEFunctions", GetAssociatedForm, true);
	
	a_vm->RegisterFunction("GetEffectArchetypeAsInt", "PO3_SKSEFunctions", GetEffectArchetypeAsInt, true);

	a_vm->RegisterFunction("GetEffectArchetypeAsString", "PO3_SKSEFunctions", GetEffectArchetypeAsString, true);

	a_vm->RegisterFunction("GetPrimaryActorValue", "PO3_SKSEFunctions", GetPrimaryActorValue, true);

	a_vm->RegisterFunction("GetSecondaryActorValue", "PO3_SKSEFunctions", GetSecondaryActorValue, true);

	a_vm->RegisterFunction("GetMagicEffectSound", "PO3_SKSEFunctions", GetMagicEffectSound);

	a_vm->RegisterFunction("SetMagicEffectSound", "PO3_SKSEFunctions", SetMagicEffectSound);

	return true;
}
