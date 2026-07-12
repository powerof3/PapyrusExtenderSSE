#pragma once

namespace Event
{
	void RegisterHookEvents();

	namespace FallLongDistance
	{
		template <std::size_t N>
		struct CalcDoDamage
		{
			static float thunk(RE::Actor* a_this, float a_fallDistance, float a_defaultMult)
			{
				const auto fallDamage = func(a_this, a_fallDistance, a_defaultMult);
				if (fallDamage > 0.0f) {
					GameEventHolder::GetSingleton()->actorFallLongDistance.QueueEvent(a_this, a_fallDistance, fallDamage);
				}
				return fallDamage;
			}

			static inline REL::Relocation<decltype(thunk)> func;
		};
	}

	namespace FastTravel
	{
		RE::TESObjectREFR* GetMapMarkerObject(const RE::NiPointer<RE::TESObjectREFR>& a_refr);
		RE::TESObjectREFR* GetMapMarkerObject(RE::FormID a_formID);
		RE::TESObjectREFR* GetMapMarkerObject(const char* a_name);

		bool  SetFastTravelDisabled(bool a_disable);
		bool  SetFastTravelTarget(RE::FormID a_formID);
		bool  SetFastTravelTarget(RE::TESObjectREFR* a_refr);
		bool  SetFastTravelTarget(const char* a_name);
		float SetFastTravelWaitTimeout(float a_timeout);
	}

	namespace ItemCrafted
	{
		struct StoryItemCraft
		{
			RE::ObjectRefHandle objectHandle;  // 00
			RE::BGSLocation*    location;      // 08
			RE::TESForm*        form;          // 10
		};

		static_assert(sizeof(StoryItemCraft) == 0x18);

		template <std::size_t N>
		struct StoryCraftItem
		{
			static StoryItemCraft* thunk(StoryItemCraft* a_event, RE::TESObjectREFR* a_refr, RE::BGSLocation* a_loc, RE::TESForm* a_form)
			{
				GameEventHolder::GetSingleton()->itemCrafted.QueueEvent(a_refr, a_loc, a_form);

				return func(a_event, a_refr, a_loc, a_form);
			}

			static inline REL::Relocation<decltype(thunk)> func;
		};
	}
}
