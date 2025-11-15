#include "Papyrus/Functions/Sound.h"

namespace Papyrus::Sound
{
	void SetSoundDescriptor(STATIC_ARGS, RE::TESSound* a_sound, RE::BGSSoundDescriptorForm* a_soundDescriptor)
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

	void Bind(VM& a_vm)
	{
		std::uint32_t count = 0;

		BIND(SetSoundDescriptor);

		logger::info("Registered {} sound functions"sv, count);
	}
}
