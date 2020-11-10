#include "Papyrus/Package.h"


std::int32_t papyrusPackage::GetPackageType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESPackage* a_package)
{
	if (!a_package) {
		a_vm->TraceStack("Package is None", a_stackID, Severity::kWarning);
		return -1;
	}

	return to_underlying(*a_package->packData.packType);
}


bool papyrusPackage::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("papyrusPackage - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("GetPackageType", "PO3_SKSEFunctions", GetPackageType);

	return true;
}
