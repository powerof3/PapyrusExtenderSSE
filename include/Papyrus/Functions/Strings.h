#pragma once

namespace Papyrus::Strings
{
	RE::BSFixedString IntToString(RE::StaticFunctionTag*, std::uint32_t a_int, bool a_hex);
	std::uint32_t StringToInt(STATIC_ARGS, RE::BSFixedString a_string);

	void Bind(VM& a_vm);
}
