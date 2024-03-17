#pragma once

namespace Papyrus::Utility
{
	inline float GenerateRandomFloat(RE::StaticFunctionTag*, float a_min, float a_max)
	{
		return clib_util::RNG().generate<float>(a_min, a_max);
	}

	inline std::uint32_t GenerateRandomInt(RE::StaticFunctionTag*, std::uint32_t a_min, std::uint32_t a_max)
	{
		return clib_util::RNG().generate<std::uint32_t>(a_min, a_max);
	}

	inline std::vector<std::int32_t> GetSystemTime(RE::StaticFunctionTag*)
	{
		SYSTEMTIME localTime;
		GetLocalTime(&localTime);

		return {
			localTime.wYear,
			localTime.wMonth,
			localTime.wDayOfWeek + 1,
			localTime.wDay, localTime.wHour,
			localTime.wMinute,
			localTime.wSecond,
			localTime.wMilliseconds
		};
	}

	inline void Bind(VM& a_vm)
	{
		BIND(GenerateRandomFloat, true);
		BIND(GenerateRandomInt, true);
		BIND(GetSystemTime, true);

		logger::info("Registered utility functions"sv);
	}
}
