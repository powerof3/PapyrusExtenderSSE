#include "Papyrus/Package.h"


auto papyrusPackage::GetPackageType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESPackage* a_package) -> std::int32_t
{
	if (!a_package) {
		a_vm->TraceStack("Package is None", a_stackID, Severity::kWarning);
		return -1;
	}

	return to_underlying(*a_package->packData.packType);
}


auto papyrusPackage::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusPackage - couldn't get VMState"sv);
		return false;
	}

	a_vm->RegisterFunction("GetPackageType"sv, "PO3_SKSEFunctions"sv, GetPackageType);

	return true;
}
