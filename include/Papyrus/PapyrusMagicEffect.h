#pragma once


namespace papyrusMagicEffect
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;


	RE::TESForm* GetAssociatedForm(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef);

	std::uint32_t GetEffectArchetypeAsInt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef);

	RE::BSFixedString GetEffectArchetypeAsString(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef);

	RE::BGSSoundDescriptorForm* GetMagicEffectSound(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef, std::uint32_t a_type);

	RE::BSFixedString GetPrimaryActorValue(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef);

	RE::BSFixedString GetSecondaryActorValue(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef);

	void SetMagicEffectSound(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EffectSetting* a_mgef, RE::BGSSoundDescriptorForm* a_sound, std::uint32_t a_type);


	bool RegisterFuncs(VM* a_vm);
}
