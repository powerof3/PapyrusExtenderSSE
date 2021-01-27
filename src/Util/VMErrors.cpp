#include "Util/VMErrors.h"


auto VMError::to_id(RE::TESObjectREFR* a_ref) -> std::string
{
	std::stringstream stream;
	stream << a_ref->GetName() << " [" << "0x" << std::uppercase << std::hex << a_ref->GetFormID() << "]";
	return stream.str();
}


auto VMError::no_3D(RE::TESObjectREFR* a_ref) -> std::string
{
	std::stringstream stream;
	stream << to_id(a_ref) << " has no 3D";
	return stream.str();
}


auto VMError::generic_error(RE::TESObjectREFR* a_ref, const std::string_view a_str) -> std::string
{
	std::stringstream stream;
	stream << to_id(a_ref) << " " << a_str;	
	return stream.str();
}