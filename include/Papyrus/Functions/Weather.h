#pragma once

namespace Papyrus::Weather
{
	float        GetWindSpeedAsFloat(STATIC_ARGS, const RE::TESWeather* a_weather);
	std::int32_t GetWindSpeedAsInt(STATIC_ARGS, const RE::TESWeather* a_weather);
	std::int32_t GetWeatherType(STATIC_ARGS, [[maybe_unused]] RE::TESWeather* a_weather);

	void Bind(VM& a_vm);
}
