#include "Papyrus/PapyrusLocation.h"


RE::BGSLocation* papyrusLocation::GetParentLocation(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSLocation* a_location)
{
	if (!a_location) {
		a_vm->TraceStack("Cannot get parent location of a None location", a_stackID, Severity::kWarning);
		return nullptr;
	}
	return a_location->parentLoc;
}


void papyrusLocation::SetParentLocation(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSLocation* a_location, RE::BGSLocation* a_newLocation)
{
	if (!a_location) {
		a_vm->TraceStack("Cannot set parent location of a None location", a_stackID, Severity::kWarning);
		return;
	}
	a_location->parentLoc = a_newLocation;
}


bool papyrusLocation::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		_MESSAGE("papyrusLocation - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("GetParentLocation", "PO3_SKSEFunctions", GetParentLocation);

	a_vm->RegisterFunction("SetParentLocation", "PO3_SKSEFunctions", SetParentLocation);

	return true;
}
