#pragma once

namespace Papyrus::Furniture
{
	inline std::int32_t GetFurnitureType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESFurniture* a_furniture)
	{
		using FLAGS = RE::TESFurniture::ActiveMarker;

		if (!a_furniture) {
			a_vm->TraceStack("Furniture is None", a_stackID);
			return -1;
		}

		const auto flags = a_furniture->furnFlags;
		if (flags.any(FLAGS::kIsPerch)) {
			return 0;
		}
		if (flags.any(FLAGS::kCanLean)) {
			return 1;
		}
		if (flags.any(FLAGS::kCanSit)) {
			return 2;
		}
		if (flags.any(FLAGS::kCanSleep)) {
			return 3;
		}

	    return -1;
	}

	inline void Bind(VM& a_vm)
	{
		BIND(GetFurnitureType, true);

		logger::info("Registered furniture functions"sv);
	}
}
