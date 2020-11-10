#pragma once


namespace VMError
{
	std::string to_id(RE::TESObjectREFR* a_ref);

	std::string no_3D(RE::TESObjectREFR* a_ref);

	std::string generic_error(RE::TESObjectREFR* a_ref, std::string_view a_str);
}
