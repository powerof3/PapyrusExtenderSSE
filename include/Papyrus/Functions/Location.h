#pragma once

namespace Papyrus::Location
{
	RE::BGSLocation* GetParentLocation(STATIC_ARGS, const RE::BGSLocation* a_location);
	void             SetParentLocation(STATIC_ARGS, RE::BGSLocation* a_location, [[maybe_unused]] RE::BGSLocation* a_newLocation);

	void Bind(VM& a_vm);
}
