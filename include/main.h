#pragma once

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"



extern "C"
{
	__declspec(dllexport) float GetPluginVersion();
};


namespace Constants
{
	inline constexpr float PI = 3.1415927f;
}


namespace Version
{
	inline constexpr float version = 3.0f;
}

