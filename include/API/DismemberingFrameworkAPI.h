#pragma once

/*******************************************************************
* DISMEMBERING FRAMEWORK - API
* Do not forget to include this source file to your project!
*******************************************************************/

/* How to create a hook to the API and use it:
SKSE::GetMessagingInterface()->RegisterListener([](MessagingInterface::Message* message) 
{
	switch (message->type) 
	{
		case MessagingInterface::kPostLoadGame:
		case MessagingInterface::kNewGame:
		{
			if (!DismemberingFrameworkAPI::LoadAPI()) {
				util::report_and_fail("Failed to bound to the Dismembering Framework API");
			}
			DismemberingFrameworkAPI::g_API->GetVersion();
		}
		break;
	}
});
*/

// Define the API type key
#define DF_API_TYPE_KEY static_cast<uint32_t>(0x46440000 /*0x00000107*/)  // 'DF'

// Define the API version in a structured format
#define DF_API_VERSION_MAJOR 1
#define DF_API_VERSION_MINOR 0
#define DF_API_VERSION_PATCH 1

// Combine the version numbers into a single value
#define DF_API_VERSION ((DF_API_VERSION_MAJOR << 16) | (DF_API_VERSION_MINOR << 8) | DF_API_VERSION_PATCH)

namespace DismemberingFrameworkAPI
{
	struct DismembermentParams
	{
		bool         forceExecution = false;
		std::string  specificNode = "";
		bool         ignoreArmorClass = false;
		bool         noLimbImpulse = false;
		bool         noSoundEffect = false;
		bool         noPlayerEffect = false;
		RE::HitData* hitData = nullptr;
	};

	class DismemberingFrameworkAPI
	{
	public:
		// API functions
		virtual size_t GetVersion() const;

		virtual void Dismember(RE::Actor* target, const RE::BSFixedString& node, RE::Actor* aggressor = nullptr, RE::TESObjectWEAP* weapon = nullptr, const DismembermentParams* params = nullptr) const;

		virtual bool IsDismembered(RE::Actor* actor) const;

		virtual bool IsDismemberedNode(RE::Actor* actor, const RE::BSFixedString& node) const;

		virtual void RefreshActorDismemberedState(RE::Actor* actor) const;
	};

	// Global API pointer
	inline extern DismemberingFrameworkAPI* g_API = nullptr;

	// Call this function only after the kDataLoaded event
	inline bool LoadAPI()
	{
		if (g_API != nullptr)
			return true;
		SKSE::GetMessagingInterface()->Dispatch(DF_API_TYPE_KEY, (void*)&g_API, sizeof(void*), NULL);
		if (g_API) {  // API successfully received!
			// Check if the API version matches
			return (g_API->GetVersion() == DF_API_VERSION);
		}
		return false;
	}
}
