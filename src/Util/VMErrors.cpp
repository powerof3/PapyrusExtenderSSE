#include "Util/VMErrors.h"


std::string VMError::to_id(RE::TESObjectREFR* a_ref)
{
	std::stringstream stream;
	stream << a_ref->GetName() << " [" << "0x" << std::uppercase << std::hex << a_ref->GetFormID() << "]";
	return stream.str();
}


std::string VMError::no_3D(RE::TESObjectREFR* a_ref)
{
	std::stringstream stream;
	stream << to_id(a_ref) << " has no 3D";
	return stream.str();
}


std::string VMError::generic_error(RE::TESObjectREFR* a_ref, std::string_view a_str)
{
	std::stringstream stream;
	stream << to_id(a_ref) << " " << a_str;	
	return stream.str();
}