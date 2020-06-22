#include "Papyrus/PapyrusPackage.h"


SInt32 papyrusPackage::GetPackageType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESPackage* a_package)
{
	if (!a_package) {
		a_vm->TraceStack("Cannot check type of a None package", a_stackID, Severity::kWarning);
		return -1;
	}
	return to_underlying(a_package->packData.packType);
}


bool papyrusPackage::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		_MESSAGE("papyrusPackage - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("GetPackageType", "PO3_SKSEFunctions", GetPackageType);

	return true;
}
