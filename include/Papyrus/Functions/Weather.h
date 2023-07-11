#pragma once

namespace Papyrus::Weather
{
	inline float GetWindSpeedAsFloat(STATIC_ARGS, const RE::TESWeather* a_weather)
	{
		if (!a_weather) {
			a_vm->TraceStack("Weather is None", a_stackID);
			return 0.0f;
		}

		return a_weather->data.windSpeed / 255.0f;
	}

	inline std::int32_t GetWindSpeedAsInt(STATIC_ARGS, const RE::TESWeather* a_weather)
	{
		if (!a_weather) {
			a_vm->TraceStack("Weather is None", a_stackID);
			return 0;
		}

		return a_weather->data.windSpeed;
	}

	inline std::int32_t GetWeatherType(STATIC_ARGS, [[maybe_unused]] RE::TESWeather* a_weather)
	{
		RE::TESWeather* currentWeather = nullptr;

		if (a_weather) {
			currentWeather = a_weather;
		} else {
			if (const auto sky = RE::Sky::GetSingleton()) {
				currentWeather = sky->currentWeather;
			}
		}

		if (!currentWeather) {
			a_vm->TraceStack("Weather/current Weather is None", a_stackID);
			return -1;
		}

		const auto flags = currentWeather->data.flags;
		if (flags.any(RE::TESWeather::WeatherDataFlag::kPleasant)) {
			return 0;
		}
		if (flags.any(RE::TESWeather::WeatherDataFlag::kCloudy)) {
			return 1;
		}
		if (flags.any(RE::TESWeather::WeatherDataFlag::kRainy)) {
			return 2;
		}
		if (flags.any(RE::TESWeather::WeatherDataFlag::kSnow)) {
			return 3;
		}

		return -1;
	}

	inline void Bind(VM& a_vm)
	{
		BIND(GetWindSpeedAsFloat);
		BIND(GetWindSpeedAsInt);
		BIND(GetWeatherType);

		logger::info("Registered weather functions"sv);
	}
}
