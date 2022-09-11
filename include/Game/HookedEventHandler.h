#pragma once

namespace Event
{
	void RegisterHookEvents();

	namespace FastTravel
	{
		RE::TESObjectREFR* GetMapMarkerObject(RE::TESObjectREFR* a_refr);
		RE::TESObjectREFR* GetMapMarkerObject(const RE::FormID a_formID);
		RE::TESObjectREFR* GetMapMarkerObject(const char* a_name);

		bool SetFastTravelTarget(const RE::FormID a_formID);
		bool SetFastTravelTarget(RE::TESObjectREFR* a_refr);
		bool SetFastTravelTarget(const char* a_name);

	};
}
