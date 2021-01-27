#include "Papyrus/Location.h"


auto papyrusLocation::GetParentLocation(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSLocation* a_location) -> RE::BGSLocation*
{
	if (!a_location) {
		a_vm->TraceStack("Location is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	return a_location->parentLoc;
}


void papyrusLocation::SetParentLocation(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSLocation* a_location, RE::BGSLocation* a_newLocation)
{
	if (!a_location) {
		a_vm->TraceStack("Location is None", a_stackID, Severity::kWarning);
		return;
	}

	a_location->parentLoc = a_newLocation;
}


auto papyrusLocation::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusLocation - couldn't get VMState"sv);
		return false;
	}

	auto constexpr Functions = "PO3_SKSEFunctions"sv;

    a_vm->RegisterFunction("GetParentLocation"sv, Functions, GetParentLocation);

	a_vm->RegisterFunction("SetParentLocation"sv, Functions, SetParentLocation);

	return true;
}
