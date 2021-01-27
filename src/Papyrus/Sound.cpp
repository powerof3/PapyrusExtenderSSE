#include "Papyrus/Sound.h"


void papyrusSound::SetSoundDescriptor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESSound* a_sound, RE::BGSSoundDescriptorForm* a_soundDescriptor)
{
	if (!a_sound) {
		a_vm->TraceStack("Sound is None", a_stackID, Severity::kWarning);
		return;
	}
    if (!a_soundDescriptor) {
        a_vm->TraceStack("Sound Descriptor is None", a_stackID, Severity::kWarning);
        return;
    }

    a_sound->descriptor = a_soundDescriptor;
}


auto papyrusSound::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusSound - couldn't get VMState"sv);
		return false;
	}

	a_vm->RegisterFunction("SetSoundDescriptor"sv, "PO3_SKSEFunctions"sv, SetSoundDescriptor);

	return true;
}
