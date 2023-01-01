#pragma once

namespace Event
{
	void RegisterHookEvents();

	namespace FastTravel
	{
		RE::TESObjectREFR* GetMapMarkerObject(const RE::NiPointer<RE::TESObjectREFR>& a_refr);
		RE::TESObjectREFR* GetMapMarkerObject(RE::FormID a_formID);
		RE::TESObjectREFR* GetMapMarkerObject(const char* a_name);

		bool SetFastTravelDisabled(bool a_disable);
		bool SetFastTravelTarget(RE::FormID a_formID);
		bool SetFastTravelTarget(RE::TESObjectREFR* a_refr);
		bool SetFastTravelTarget(const char* a_name);
		float SetFastTravelWaitTimeout(float a_timeout);
	}
}
