#include "Papyrus/Functions/Furniture.h"

namespace Papyrus::Furniture
{
	std::int32_t GetFurnitureType(STATIC_ARGS, const RE::TESFurniture* a_furniture)
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

	void Bind(VM& a_vm)
	{
		BIND(GetFurnitureType, true);

		logger::info("Registered furniture functions"sv);
	}
}
