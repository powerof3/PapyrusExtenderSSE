#pragma once

namespace Papyrus::MagicEffect
{
	inline RE::TESForm* GetAssociatedForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::EffectSetting* a_mgef)
	{
		if (!a_mgef) {
			a_vm->TraceStack("MagicEffect is None", a_stackID);
			return nullptr;
		}

		return a_mgef->data.associatedForm;
	}

	inline std::uint32_t GetEffectArchetypeAsInt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::EffectSetting* a_mgef)
	{
		if (!a_mgef) {
			a_vm->TraceStack("MagicEffect is None", a_stackID);
			return 0;
		}

		return stl::to_underlying(a_mgef->GetArchetype());
	}

	inline RE::BSFixedString GetEffectArchetypeAsString(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::EffectSetting* a_mgef)
	{
		if (!a_mgef) {
			a_vm->TraceStack("MagicEffect is None", a_stackID);
			return RE::BSFixedString();
		}

		return RE::EffectArchetypeToString(a_mgef->GetArchetype());
	}

	inline RE::BGSSoundDescriptorForm* GetMagicEffectSound(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::EffectSetting* a_mgef,
		std::uint32_t a_type)
	{
		if (!a_mgef) {
			a_vm->TraceStack("MagicEffect is None", a_stackID);
			return nullptr;
		}
		if (a_type > 6) {
			a_vm->TraceForm(a_mgef, "Sound index is invalid", a_stackID);
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

	inline RE::BSFixedString GetPrimaryActorValue(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::EffectSetting* a_mgef)
	{
		if (!a_mgef) {
			a_vm->TraceStack("MagicEffect is None", a_stackID);
			return RE::BSFixedString();
		}

		const auto actorValueList = RE::ActorValueList::GetSingleton();
		const auto actorValueInfo = actorValueList ? actorValueList->GetActorValue(a_mgef->data.primaryAV) : nullptr;

		return actorValueInfo ? actorValueInfo->enumName : RE::BSFixedString();
	}

	inline RE::BSFixedString GetSecondaryActorValue(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::EffectSetting* a_mgef)
	{
		if (!a_mgef) {
			a_vm->TraceStack("MagicEffect is None", a_stackID);
			return RE::BSFixedString();
		}

		const auto actorValueList = RE::ActorValueList::GetSingleton();
		const auto actorValueInfo = actorValueList ? actorValueList->GetActorValue(a_mgef->data.secondaryAV) : nullptr;

		return actorValueInfo ? actorValueInfo->enumName : RE::BSFixedString();
	}

	inline void SetAssociatedForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef, RE::TESForm* a_form)
	{
		if (!a_mgef) {
			a_vm->TraceStack("MagicEffect is None", a_stackID);
			return;
		}

		a_mgef->data.associatedForm = a_form;
	}

	inline void SetMagicEffectSound(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::EffectSetting* a_mgef,
		RE::BGSSoundDescriptorForm* a_sound,
		std::uint32_t a_type)
	{
		if (!a_mgef) {
			a_vm->TraceStack("MagicEffect is None", a_stackID);
			return;
		}
		if (!a_sound) {
			a_vm->TraceStack("Sound is None", a_stackID);
			return;
		}
		if (a_type > 6) {
			a_vm->TraceForm(a_mgef, "Sound index is invalid", a_stackID);
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

	inline void Bind(VM& a_vm)
	{
		BIND(GetAssociatedForm, true);
		BIND(GetEffectArchetypeAsInt, true);
		BIND(GetEffectArchetypeAsString, true);
		BIND(GetPrimaryActorValue, true);
		BIND(GetSecondaryActorValue, true);
		BIND(GetMagicEffectSound);
		BIND(SetAssociatedForm);
		BIND(SetMagicEffectSound);

		logger::info("Registered magiceffect functions"sv);
	}
}
