#include "Papyrus\ExtendedObjectTypes.h"


bool extendedObjectTypes::RegisterTypes(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("Object types - couldn't get VMState");
		return false;
	}

	a_vm->RegisterObjectType(vm_cast<RE::BGSFootstepSet>(), "FootstepSet");
	logger::info("Registered footstep set object type");

	a_vm->RegisterObjectType(vm_cast<RE::BGSLightingTemplate>(), "LightingTemplate");
	logger::info("Registered lighting template object type");

	return true;
}
