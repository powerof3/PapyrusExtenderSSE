#pragma once

namespace Papyrus::Weather
{
	inline float GetWindSpeedAsFloat(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESWeather* a_weather)
	{
		if (!a_weather) {
			a_vm->TraceStack("Weather is None", a_stackID);
			return 0.0f;
		}

		return static_cast<float>(a_weather->data.windSpeed / 255.0f);
	}

	inline std::int32_t GetWindSpeedAsInt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESWeather* a_weather)
	{
		if (!a_weather) {
			a_vm->TraceStack("Weather is None", a_stackID);
			return 0;
		}

		return a_weather->data.windSpeed;
	}

	inline std::int32_t GetWeatherType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, [[maybe_unused]] RE::TESWeather* a_weather)
	{
		using Type = RE::TESWeather::WeatherDataFlag;

		RE::TESWeather* currentWeather = nullptr;
		if (a_weather) {
			currentWeather = a_weather;
		} else {
			if (const auto sky = RE::Sky::GetSingleton(); sky) {
				currentWeather = sky->currentWeather;
			}
		}

		if (!currentWeather) {
			a_vm->TraceStack("Weather/current Weather is None", a_stackID);
			return -1;
		}

		const auto flags = currentWeather->data.flags;
		if (flags.all(Type::kPleasant)) {
			return 0;
		}
		if (flags.all(Type::kCloudy)) {
			return 1;
		}
		if (flags.all(Type::kRainy)) {
			return 2;
		}
		if (flags.all(Type::kSnow)) {
			return 3;
		}
		return -1;
	}

	inline void Register(VM& a_vm)
	{
		BIND(GetWindSpeedAsFloat);
		BIND(GetWindSpeedAsInt);
		BIND(GetWeatherType);

		logger::info("Registered weather functions"sv);
	}
}
