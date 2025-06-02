#pragma once

namespace Papyrus::Utility
{
	float                     GenerateRandomFloat(RE::StaticFunctionTag*, float a_min, float a_max);
	std::uint32_t             GenerateRandomInt(RE::StaticFunctionTag*, std::uint32_t a_min, std::uint32_t a_max);
	std::vector<std::int32_t> GetSystemTime(RE::StaticFunctionTag*);

	void Bind(VM& a_vm);
}
