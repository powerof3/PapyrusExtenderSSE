#include "Papyrus/Functions/MagicEffect.h"

#include "Papyrus/Util/Magic.h"

namespace Papyrus::MagicEffect
{
	RE::TESForm* GetAssociatedForm(STATIC_ARGS, const RE::EffectSetting* a_mgef)
	{
		if (!a_mgef) {
			a_vm->TraceStack("MagicEffect is None", a_stackID);
			return nullptr;
		}

		return a_mgef->data.associatedForm;
	}

	std::uint32_t GetEffectArchetypeAsInt(STATIC_ARGS, const RE::EffectSetting* a_mgef)
	{
		if (!a_mgef) {
			a_vm->TraceStack("MagicEffect is None", a_stackID);
			return 0;
		}

		return std::to_underlying(a_mgef->GetArchetype());
	}

	RE::BSFixedString GetEffectArchetypeAsString(STATIC_ARGS, const RE::EffectSetting* a_mgef)
	{
		if (!a_mgef) {
			a_vm->TraceStack("MagicEffect is None", a_stackID);
			return {};
		}

		return RE::EffectArchetypeToString(a_mgef->GetArchetype());
	}

	RE::BGSSoundDescriptorForm* GetMagicEffectSound(STATIC_ARGS, RE::EffectSetting* a_mgef, std::uint32_t a_type)
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

	RE::BSFixedString GetPrimaryActorValue(STATIC_ARGS, const RE::EffectSetting* a_mgef)
	{
		if (!a_mgef) {
			a_vm->TraceStack("MagicEffect is None", a_stackID);
			return {};
		}

		const auto actorValueList = RE::ActorValueList::GetSingleton();
		const auto actorValueInfo = actorValueList ? actorValueList->GetActorValueInfo(a_mgef->data.primaryAV) : nullptr;

		return actorValueInfo ? actorValueInfo->enumName : RE::BSFixedString();
	}

	RE::BSFixedString GetSecondaryActorValue(STATIC_ARGS, const RE::EffectSetting* a_mgef)
	{
		if (!a_mgef) {
			a_vm->TraceStack("MagicEffect is None", a_stackID);
			return {};
		}

		const auto actorValueList = RE::ActorValueList::GetSingleton();
		const auto actorValueInfo = actorValueList ? actorValueList->GetActorValueInfo(a_mgef->data.secondaryAV) : nullptr;

		return actorValueInfo ? actorValueInfo->enumName : RE::BSFixedString();
	}

	void SetAssociatedForm(STATIC_ARGS, RE::EffectSetting* a_mgef, RE::TESForm* a_form)
	{
		if (!a_mgef) {
			a_vm->TraceStack("MagicEffect is None", a_stackID);
			return;
		}

		a_mgef->data.associatedForm = a_form;
	}

	void SetMagicEffectSound(STATIC_ARGS, RE::EffectSetting* a_mgef, RE::BGSSoundDescriptorForm* a_sound, std::uint32_t a_type)
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

	void Bind(VM& a_vm)
	{
		std::uint32_t count = 0;
		
		BIND(GetAssociatedForm, true);
		BIND(GetEffectArchetypeAsInt, true);
		BIND(GetEffectArchetypeAsString, true);
		BIND(GetPrimaryActorValue, true);
		BIND(GetSecondaryActorValue, true);
		BIND(GetMagicEffectSound);
		BIND(SetAssociatedForm);
		BIND(SetMagicEffectSound);

		logger::info("Registered {} magiceffect functions"sv, count);
	}
}
