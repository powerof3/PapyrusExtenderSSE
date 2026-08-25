#include "Papyrus/Functions/Strings.h"

namespace Papyrus::Strings
{
	RE::BSFixedString IntToString(RE::StaticFunctionTag*, std::uint32_t a_int, bool a_hex)
	{
		if (a_hex) {
			return std::format("0x{:X}", a_int);
		}
		return std::to_string(a_int);
	}

	std::uint32_t StringToInt(STATIC_ARGS, RE::BSFixedString a_string)
	{
		std::int32_t value = -1;
		if (!a_string.empty()) {
			try {
				const std::string string = a_string.c_str();
				value = REX::STR::TO_NUM<std::uint32_t>(string, REX::STR::IS_ONLY_HEX(string, true));
			} catch (...) {
				a_vm->TraceStack("String is out of range - cannot be converted to an integer", a_stackID);
			}
		}
		return value;
	}

	void Bind(VM& a_vm)
	{
		std::uint32_t count = 0;

		BIND(IntToString);
		BIND(StringToInt);

		REX::INFO("Registered {} string functions"sv, count);
	}
}
