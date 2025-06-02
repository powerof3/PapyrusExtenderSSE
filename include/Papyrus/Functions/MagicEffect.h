#pragma once

namespace Papyrus::MagicEffect
{
	RE::TESForm*                GetAssociatedForm(STATIC_ARGS, const RE::EffectSetting* a_mgef);
	std::uint32_t               GetEffectArchetypeAsInt(STATIC_ARGS, const RE::EffectSetting* a_mgef);
	RE::BSFixedString           GetEffectArchetypeAsString(STATIC_ARGS, const RE::EffectSetting* a_mgef);
	RE::BGSSoundDescriptorForm* GetMagicEffectSound(STATIC_ARGS, RE::EffectSetting* a_mgef, std::uint32_t a_type);
	RE::BSFixedString           GetPrimaryActorValue(STATIC_ARGS, const RE::EffectSetting* a_mgef);
	RE::BSFixedString           GetSecondaryActorValue(STATIC_ARGS, const RE::EffectSetting* a_mgef);
	void                        SetAssociatedForm(STATIC_ARGS, RE::EffectSetting* a_mgef, RE::TESForm* a_form);
	void                        SetMagicEffectSound(STATIC_ARGS, RE::EffectSetting* a_mgef, RE::BGSSoundDescriptorForm* a_sound, std::uint32_t a_type);

	void Bind(VM& a_vm);
}
