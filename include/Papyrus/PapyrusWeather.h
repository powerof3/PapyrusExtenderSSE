#pragma once


namespace papyrusWeather
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;


	float GetWindSpeedAsFloat(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESWeather* a_weather);

	std::int32_t GetWindSpeedAsInt(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESWeather* a_weather);

	std::int32_t GetWeatherType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESWeather* a_weather);


	bool RegisterFuncs(VM* a_vm);
}