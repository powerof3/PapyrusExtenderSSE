#include "Papyrus/ExtendedObjectTypes.h"


auto extendedObjectTypes::RegisterTypes(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("Object types - couldn't get VMState"sv);
		return false;
	}

	a_vm->RegisterObjectType(vm_cast<RE::BGSFootstepSet>(), "FootstepSet");
	logger::info("Registered footstep set object type"sv);

	a_vm->RegisterObjectType(vm_cast<RE::BGSLightingTemplate>(), "LightingTemplate");
	logger::info("Registered lighting template object type"sv);

	a_vm->RegisterObjectType(vm_cast<RE::BGSDebris>(), "Debris");
	logger::info("Registered debris object type"sv);

	return true;
}
