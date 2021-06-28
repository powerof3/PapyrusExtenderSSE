#pragma once

namespace Papyrus::Location
{
	inline RE::BGSLocation* GetParentLocation(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSLocation* a_location)
	{
		if (!a_location) {
			a_vm->TraceStack("Location is None", a_stackID);
			return nullptr;
		}

		return a_location->parentLoc;
	}

	inline void SetParentLocation(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::BGSLocation* a_location,
		[[maybe_unused]] RE::BGSLocation* a_newLocation)
	{
		if (!a_location) {
			a_vm->TraceStack("Location is None", a_stackID);
			return;
		}

		a_location->parentLoc = a_newLocation;
	}

	inline void Register(VM& a_vm)
	{
		BIND(GetParentLocation);
		BIND(SetParentLocation);

		logger::info("Registered location functions"sv);
	}
}
