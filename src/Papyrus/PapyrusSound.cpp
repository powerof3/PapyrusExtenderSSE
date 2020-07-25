#include "Papyrus/PapyrusSound.h"


void papyrusSound::SetSoundDescriptor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESSound* a_sound, RE::BGSSoundDescriptorForm* a_soundDescriptor)
{
	if (!a_sound) {
		a_vm->TraceStack("Sound is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_soundDescriptor) {
		a_vm->TraceStack("Sound Descriptor is None", a_stackID, Severity::kWarning);
		return;
	}

	a_sound->descriptor = a_soundDescriptor;
}


bool papyrusSound::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("papyrusSound - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("SetSoundDescriptor", "PO3_SKSEFunctions", SetSoundDescriptor);

	return true;
}
