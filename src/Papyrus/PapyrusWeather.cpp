#include "Papyrus/PapyrusWeather.h"


float papyrusWeather::GetWindSpeedAsFloat(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESWeather* a_weather)
{
    if (!a_weather) {
        a_vm->TraceStack("Weather is None", a_stackID, Severity::kWarning);
        return 0.0f;
    }
    return a_weather->data.windSpeed / 255.0f;
}


SInt32 papyrusWeather::GetWindSpeedAsInt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESWeather* a_weather)
{
    if (!a_weather) {
        a_vm->TraceStack("Weather is None", a_stackID, Severity::kWarning);
        return 0;
    }
    return a_weather->data.windSpeed;
}


SInt32 papyrusWeather::GetWeatherType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESWeather* a_weather)
{
	using Type = RE::TESWeather::WeatherDataFlag;
	
	RE::TESWeather* currentWeather = nullptr;
	if (a_weather) {
		currentWeather = a_weather;
	}
	if (!currentWeather) {
		auto sky = RE::Sky::GetSingleton();
		if (sky) {
			currentWeather = sky->currentWeather;
		}
	}
	if (currentWeather) {
		const auto flags = currentWeather->data.flags;

		if ((flags & Type::kPleasant) == Type::kPleasant) {
			return 0;
		}
		if ((flags & Type::kCloudy) == Type::kCloudy) {
			return 1;
		}
		if ((flags & Type::kRainy) == Type::kRainy) {
			return 2;
		}
		if ((flags & Type::kSnow) == Type::kSnow) {
			return 3;
		}
	}
	else {
		a_vm->TraceStack("Weather/current weather is None", a_stackID, Severity::kWarning);
	}
	return -1;
}


bool papyrusWeather::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		_MESSAGE("papyrusWeather - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("GetWindSpeedAsFloat", "PO3_SKSEFunctions", GetWindSpeedAsFloat);

	a_vm->RegisterFunction("GetWindSpeedAsInt", "PO3_SKSEFunctions", GetWindSpeedAsInt);

	a_vm->RegisterFunction("GetWeatherType", "PO3_SKSEFunctions", GetWeatherType);

	return true;
}
