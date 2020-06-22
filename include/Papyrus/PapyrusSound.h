#pragma once

#include "main.h"


namespace papyrusSound
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;


	void SetSoundDescriptor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESSound* a_sound, RE::BGSSoundDescriptorForm* a_soundDescriptor);


	bool RegisterFuncs(VM* a_vm);
}