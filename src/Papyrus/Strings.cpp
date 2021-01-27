#include "Papyrus/Strings.h"


auto papyrusStrings::IntToString(VM*, StackID, RE::StaticFunctionTag*, std::uint32_t a_int, bool a_hex) -> RE::BSFixedString
{
	if (a_hex) {
		std::stringstream stream;
		stream << "0x" << std::uppercase << std::hex << a_int;
		return stream.str().c_str();
	}
	return std::to_string(a_int).c_str();
}


auto papyrusStrings::StringToInt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_string) -> std::int32_t
{
	std::int32_t value = -1;
	if (!a_string.empty()) {
		try {
			value = std::stoi(a_string.c_str(), nullptr, 0);
		} catch (...) {
			a_vm->TraceStack("String is out of range - cannot be converted to an integer", a_stackID, Severity::kWarning);
		}
	}
	return value;
}


auto papyrusStrings::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusStrings - couldn't get VMState"sv);
		return false;
	}

	auto constexpr Functions = "PO3_SKSEFunctions"sv;

	a_vm->RegisterFunction("IntToString"sv, Functions, IntToString);

	a_vm->RegisterFunction("StringToInt", Functions, StringToInt);

	return true;
}
