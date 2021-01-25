#include "Papyrus/Furniture.h"


std::int32_t papyrusFurniture::GetFurnitureType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESFurniture* a_furniture)
{
	using FLAGS = RE::TESFurniture::ActiveMarker;

	if (!a_furniture) {
		a_vm->TraceStack("Furniture is None", a_stackID, Severity::kWarning);
		return -1;
	}

	const auto flags = a_furniture->furnFlags;
	if (flags.all(FLAGS::kIsPerch)) {
		return 0;
	} else if (flags.all(FLAGS::kCanLean)) {
		return 1;
	} else if (flags.all(FLAGS::kCanSit)) {
		return 2;
	} else if (flags.all(FLAGS::kCanSleep)) {
		return 3;
	} else {
		return -1;
	}
}


bool papyrusFurniture::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("papyrusFurniture - couldn't get VMState"sv);
		return false;
	}

	a_vm->RegisterFunction("GetFurnitureType", "PO3_SKSEFunctions", GetFurnitureType, true);

	return true;
}
