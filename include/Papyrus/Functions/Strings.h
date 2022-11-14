#pragma once

namespace Papyrus::Strings
{
	inline RE::BSFixedString IntToString(RE::StaticFunctionTag*, std::uint32_t a_int, bool a_hex)
	{
		if (a_hex) {
			return fmt::format("0x{:X}", a_int);
		}
		return std::to_string(a_int);
	}

	inline std::int32_t StringToInt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BSFixedString a_string)
	{
		std::int32_t value = -1;
		if (!a_string.empty()) {
			try {
				value = string::to_num<std::int32_t>(a_string.c_str());
			} catch (...) {
				a_vm->TraceStack("String is out of range - cannot be converted to an integer", a_stackID);
			}
		}
		return value;
	}

	inline void Bind(VM& a_vm)
	{
		BIND(IntToString);
		BIND(StringToInt);

		logger::info("Registered string functions"sv);
	}
}
