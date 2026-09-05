#include "Papyrus/Functions/Utility.h"

namespace Papyrus::Utility
{
	float GenerateRandomFloat(RE::StaticFunctionTag*, float a_min, float a_max)
	{
		thread_local auto rng = REX::TRandom<float>();

		return rng.Generate(a_min, a_max);
	}

	std::uint32_t GenerateRandomInt(RE::StaticFunctionTag*, std::uint32_t a_min, std::uint32_t a_max)
	{
		thread_local auto rng = REX::TRandom<std::uint32_t>();

		return rng.Generate(a_min, a_max);
	}

	std::vector<std::int32_t> GetSystemTime(RE::StaticFunctionTag*)
	{
		SYSTEMTIME localTime;
		GetLocalTime(&localTime);

		return {
			localTime.wYear,
			localTime.wMonth,
			localTime.wDayOfWeek + 1,
			localTime.wDay,
			localTime.wHour,
			localTime.wMinute,
			localTime.wSecond,
			localTime.wMilliseconds
		};
	}

	void Bind(VM& a_vm)
	{
		std::uint32_t count = 0;

		BIND(GenerateRandomFloat, true);
		BIND(GenerateRandomInt, true);
		BIND(GetSystemTime, true);

		REX::INFO("Registered {} utility functions"sv, count);
	}
}
