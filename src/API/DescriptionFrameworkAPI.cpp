#include "API/DescriptionFrameworkAPI.h"
// Interface code based on https://github.com/adamhynek/higgs

// Stores the API after it has already been fetched
DescriptionFrameworkAPI::IDescriptionFrameworkInterface001* g_DescriptionFrameworkInterface = nullptr;

// Fetches the interface to use from DescriptionFramework
DescriptionFrameworkAPI::IDescriptionFrameworkInterface001* DescriptionFrameworkAPI::GetDescriptionFrameworkInterface001()
{
	// If the interface has already been fetched, return the same object
	if (g_DescriptionFrameworkInterface) {
		return g_DescriptionFrameworkInterface;
	}

	// Dispatch a message to get the plugin interface from DescriptionFramework
	DescriptionFrameworkMessage message;
	const auto                  skseMessaging = SKSE::GetMessagingInterface();
	skseMessaging->Dispatch(DescriptionFrameworkMessage::kMessage_GetInterface, (void*)&message,
		sizeof(DescriptionFrameworkMessage*), DescriptionFrameworkPluginName);
	if (!message.GetApiFunction) {
		return nullptr;
	}

	// Fetch the API for this version of the DescriptionFramework interface
	g_DescriptionFrameworkInterface = static_cast<IDescriptionFrameworkInterface001*>(message.GetApiFunction(1));
	return g_DescriptionFrameworkInterface;
}
