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
		if (flags.all(FLAGS::kIsPerch)) {
			return 0;
		}
		if (flags.all(FLAGS::kCanLean)) {
			return 1;
		}
		if (flags.all(FLAGS::kCanSit)) {
			return 2;
		}
		if (flags.all(FLAGS::kCanSleep)) {
			return 3;
		}
		return -1;
	}

	void Register(VM& a_vm)
	{
		BIND(GetFurnitureType, true);

		logger::info("Registered furniture functions"sv);
	}
}
