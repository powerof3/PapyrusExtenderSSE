#pragma once

namespace Papyrus::Sound
{
	inline void SetSoundDescriptor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		RE::TESSound* a_sound, 
		RE::BGSSoundDescriptorForm* a_soundDescriptor)
	{
		if (!a_sound) {
			a_vm->TraceStack("Sound is None", a_stackID);
			return;
		}
		if (!a_soundDescriptor) {
			a_vm->TraceStack("Sound Descriptor is None", a_stackID);
			return;
		}

		a_sound->descriptor = a_soundDescriptor;
	}

	inline void Register(VM& a_vm)
	{
		BIND(SetSoundDescriptor);

		logger::info("Registered sound functions"sv);
	}
}
