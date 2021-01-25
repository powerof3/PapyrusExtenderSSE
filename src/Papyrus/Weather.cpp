#include "Papyrus/Weather.h"


float papyrusWeather::GetWindSpeedAsFloat(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESWeather* a_weather)
{
	if (!a_weather) {
		a_vm->TraceStack("Weather is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_weather->data.windSpeed / 255.0f;
}


std::int32_t papyrusWeather::GetWindSpeedAsInt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESWeather* a_weather)
{
	if (!a_weather) {
		a_vm->TraceStack("Weather is None", a_stackID, Severity::kWarning);
		return 0;
	}

	return a_weather->data.windSpeed;
}


std::int32_t papyrusWeather::GetWeatherType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESWeather* a_weather)
{
	using Type = RE::TESWeather::WeatherDataFlag;

	RE::TESWeather* currentWeather = nullptr;

	if (a_weather) {
		currentWeather = a_weather;
	} else {
		auto sky = RE::Sky::GetSingleton();
		if (sky) {
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
	} else if (flags.all(Type::kCloudy)) {
		return 1;
	} else if (flags.all(Type::kRainy)) {
		return 2;
	} else if (flags.all(Type::kSnow)) {
		return 3;
	} else {
		return -1;
	}
}


bool papyrusWeather::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("papyrusWeather - couldn't get VMState"sv);
		return false;
	}

	a_vm->RegisterFunction("GetWindSpeedAsFloat", "PO3_SKSEFunctions", GetWindSpeedAsFloat);

	a_vm->RegisterFunction("GetWindSpeedAsInt", "PO3_SKSEFunctions", GetWindSpeedAsInt);

	a_vm->RegisterFunction("GetWeatherType", "PO3_SKSEFunctions", GetWeatherType);

	return true;
}
