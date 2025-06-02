#pragma once

namespace Papyrus::Sound
{
	void SetSoundDescriptor(STATIC_ARGS, RE::TESSound* a_sound, RE::BGSSoundDescriptorForm* a_soundDescriptor);

	void Bind(VM& a_vm);
}
