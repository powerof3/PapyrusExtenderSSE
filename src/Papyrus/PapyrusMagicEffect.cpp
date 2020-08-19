#include "Papyrus/PapyrusMagicEffect.h"

RE::TESForm* papyrusMagicEffect::GetAssociatedForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef)
{
	if (!a_mgef) {
		a_vm->TraceStack("MagicEffect is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	return a_mgef->data.associatedForm;
}


std::uint32_t papyrusMagicEffect::GetEffectArchetypeAsInt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef)
{
	if (!a_mgef) {
		a_vm->TraceStack("MagicEffect is None", a_stackID, Severity::kWarning);
		return 0;
	}

	return to_underlying(a_mgef->data.archetype);
}


RE::BSFixedString papyrusMagicEffect::GetEffectArchetypeAsString(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef)
{
	if (!a_mgef) {
		a_vm->TraceStack("MagicEffect is None", a_stackID, Severity::kWarning);
		return RE::BSFixedString();
	}

	return a_mgef->GetArchetypeAsString();
}


RE::BGSSoundDescriptorForm* papyrusMagicEffect::GetMagicEffectSound(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef, std::uint32_t a_type)
{
	if (!a_mgef) {
		a_vm->TraceStack("MagicEffect is None", a_stackID, Severity::kWarning);
		return nullptr;
	} else if (a_type < 0 || a_type > 6) {
		a_vm->TraceStack("Sound index is invalid", a_stackID, Severity::kWarning);
		return nullptr;
	}

	auto soundID = static_cast<RE::MagicSystem::SoundID>(a_type);
	for (const auto& effectSound : a_mgef->effectSounds) {
		if (effectSound.id == soundID) {
			return effectSound.sound;
		}
	}

	return nullptr;
}


RE::BSFixedString papyrusMagicEffect::GetPrimaryActorValue(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef)
{
	if (!a_mgef) {
		a_vm->TraceStack("MagicEffect is None", a_stackID, Severity::kWarning);
		return "";
	}

	auto actorValueList = RE::ActorValueList::GetSingleton();
	if (actorValueList) {
		auto av = actorValueList->GetActorValue(a_mgef->data.primaryAV);
		return av ? av->enumName : "";
	}

	return "";
}


RE::BSFixedString papyrusMagicEffect::GetSecondaryActorValue(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef)
{
	if (!a_mgef) {
		a_vm->TraceStack("MagicEffect is None", a_stackID, Severity::kWarning);
		return "";
	}

	auto actorValueList = RE::ActorValueList::GetSingleton();
	if (actorValueList) {
		auto av = actorValueList->GetActorValue(a_mgef->data.secondaryAV);
		return av ? av->enumName : "";
	}

	return "";
}


void papyrusMagicEffect::SetMagicEffectSound(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef, RE::BGSSoundDescriptorForm* a_sound, std::uint32_t a_type)
{
	if (!a_mgef) {
		a_vm->TraceStack("MagicEffect is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_sound) {
		a_vm->TraceStack("Sound is None", a_stackID, Severity::kWarning);
		return;
	} else if (a_type < 0 || a_type > 6) {
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
		logger::critical("papyrusMagicEffect - couldn't get VMState");
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
