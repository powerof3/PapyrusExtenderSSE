#include "Papyrus/Functions/Location.h"

namespace Papyrus::Location
{
	RE::BGSLocation* GetParentLocation(STATIC_ARGS, const RE::BGSLocation* a_location)
	{
		if (!a_location) {
			a_vm->TraceStack("Location is None", a_stackID);
			return nullptr;
		}

		return a_location->parentLoc;
	}

	void SetParentLocation(STATIC_ARGS, RE::BGSLocation* a_location, RE::BGSLocation* a_newLocation)
	{
		if (!a_location) {
			a_vm->TraceStack("Location is None", a_stackID);
			return;
		}

		a_location->parentLoc = a_newLocation;
	}

	std::vector<RE::TESObjectREFR*> GetLocationBossReferences(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSLocation* a_location)
	{
		std::vector<RE::TESObjectREFR*> result{};
		if (!a_location) {
			a_vm->TraceStack("Location is None", a_stackID);
			return result;
		}
		if (a_location->promotedRefs.empty())
			return result;

		for (const auto& prom : a_location->promotedRefs) {
			auto ref = prom.get();

			if (!ref) {
				continue;
			}
			if (ref->extraList.HasType(RE::ExtraDataType::kLocationRefType)) {
				result.emplace_back(ref.get());
			}
		}
		return result;
	}

	void Bind(VM& a_vm)
	{
		std::uint32_t count = 0;

		BIND(GetParentLocation);
		BIND(SetParentLocation);
		BIND(GetLocationBossReferences);

		logger::info("Registered {} location functions"sv, count);
	}
}
