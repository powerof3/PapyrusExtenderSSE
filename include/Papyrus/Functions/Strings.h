#pragma once

namespace Papyrus::Strings
{
	inline RE::BSFixedString IntToString(RE::StaticFunctionTag*, std::uint32_t a_int, bool a_hex)
	{
		if (a_hex) {
			return std::format("0x{:X}", a_int);
		}
		return std::to_string(a_int);
	}

	inline std::uint32_t StringToInt(STATIC_ARGS, RE::BSFixedString a_string)
	{
		std::int32_t value = -1;
		if (!a_string.empty()) {
			try {
				const std::string string = a_string.c_str();
				value = string::to_num<std::uint32_t>(string, string::is_only_hex(string, true));
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
