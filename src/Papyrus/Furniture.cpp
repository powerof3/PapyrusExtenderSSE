#include "Papyrus/Furniture.h"


auto papyrusFurniture::GetFurnitureType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESFurniture* a_furniture) -> std::int32_t
{
	using FLAGS = RE::TESFurniture::ActiveMarker;

	if (!a_furniture) {
		a_vm->TraceStack("Furniture is None", a_stackID, Severity::kWarning);
		return -1;
	}

	const auto flags = a_furniture->furnFlags;
	if (flags.all(FLAGS::kIsPerch)) {
		return 0;
	}
	if (flags.all(FLAGS::kCanLean)) {
		return 1;
	}
	if (flags.all(FLAGS::kCanSit)) {
		return 2;
	}
	if (flags.all(FLAGS::kCanSleep)) {
		return 3;
	}
	return -1;
}


auto papyrusFurniture::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusFurniture - couldn't get VMState"sv);
		return false;
	}

	a_vm->RegisterFunction("GetFurnitureType"sv, "PO3_SKSEFunctions"sv, GetFurnitureType, true);

	return true;
}
