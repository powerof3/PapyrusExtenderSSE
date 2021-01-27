#include "Papyrus/Weather.h"


auto papyrusWeather::GetWindSpeedAsFloat(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESWeather* a_weather) -> float
{
	if (!a_weather) {
		a_vm->TraceStack("Weather is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return static_cast<float>(a_weather->data.windSpeed / 255.0f);
}


auto papyrusWeather::GetWindSpeedAsInt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESWeather* a_weather) -> std::int32_t
{
	if (!a_weather) {
		a_vm->TraceStack("Weather is None", a_stackID, Severity::kWarning);
		return 0;
	}

	return a_weather->data.windSpeed;
}


auto papyrusWeather::GetWeatherType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESWeather* a_weather) -> std::int32_t
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
		a_vm->TraceStack("Weather/current Weather is None", a_stackID, Severity::kWarning);
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


auto papyrusWeather::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusWeather - couldn't get VMState"sv);
		return false;
	}

	auto constexpr Functions = "PO3_SKSEFunctions"sv;

	a_vm->RegisterFunction("GetWindSpeedAsFloat"sv, Functions, GetWindSpeedAsFloat);

	a_vm->RegisterFunction("GetWindSpeedAsInt"sv, Functions, GetWindSpeedAsInt);

	a_vm->RegisterFunction("GetWeatherType"sv, Functions, GetWeatherType);

	return true;
}
