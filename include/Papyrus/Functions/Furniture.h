#pragma once

namespace Papyrus::Furniture
{
	std::int32_t GetFurnitureType(STATIC_ARGS, const RE::TESFurniture* a_furniture);

	void Bind(VM& a_vm);
}
