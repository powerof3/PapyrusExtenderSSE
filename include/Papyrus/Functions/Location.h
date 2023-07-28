#pragma once

namespace Papyrus::Location
{
	inline RE::BGSLocation* GetParentLocation(STATIC_ARGS, const RE::BGSLocation* a_location)
	{
		if (!a_location) {
			a_vm->TraceStack("Location is None", a_stackID);
			return nullptr;
		}

		return a_location->parentLoc;
	}

	inline void SetParentLocation(STATIC_ARGS, RE::BGSLocation* a_location, [[maybe_unused]] RE::BGSLocation* a_newLocation)
	{
		if (!a_location) {
			a_vm->TraceStack("Location is None", a_stackID);
			return;
		}

		a_location->parentLoc = a_newLocation;
	}

	inline void Bind(VM& a_vm)
	{
		BIND(GetParentLocation);
		BIND(SetParentLocation);

		logger::info("Registered location functions"sv);
	}
}
