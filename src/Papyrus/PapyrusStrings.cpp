#include "Papyrus/PapyrusStrings.h"


RE::BSFixedString papyrusStrings::IntToString(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, UInt32 a_int, bool a_hex)
{
	if (a_hex) {
		std::stringstream stream;
		stream << "0x" << std::hex << a_int;
		return stream.str().c_str();
	}
	return std::to_string(a_int).c_str();
}


SInt32 papyrusStrings::StringToInt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_string)
{
	SInt32 value = -1;
	if (!a_string.empty()) {
		try {
			value = std::stoi(a_string.c_str(), nullptr, 0);
		}
		catch (...) {
			a_vm->TraceStack("String is out of range - cannot be converted to an integer", a_stackID, Severity::kWarning);
		}
	}
	return value;
}


bool papyrusStrings::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		_MESSAGE("papyrusObjectStrings - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("IntToString", "PO3_SKSEFunctions", IntToString);

	a_vm->RegisterFunction("StringToInt", "PO3_SKSEFunctions", StringToInt);

	return true;
}
