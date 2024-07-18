#pragma once
#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
// Interface code based on https://github.com/adamhynek/higgs

namespace DescriptionFrameworkAPI
{
	constexpr const auto DescriptionFrameworkPluginName = "DescriptionFramework";
	// A message used to fetch DescriptionFramework's interface
	struct DescriptionFrameworkMessage
	{
		enum : uint32_t
		{
			kMessage_GetInterface = 0xfbdfacfe
		};  // Randomly generated
		void* (*GetApiFunction)(unsigned int revisionNumber) = nullptr;
	};

	// Returns an IDescriptionFrameworke001 object compatible with the API shown below
	// This should only be called after SKSE sends kMessage_PostLoad to your plugin
	struct IDescriptionFrameworkInterface001;
	IDescriptionFrameworkInterface001* GetDescriptionFrameworkInterface001();

	// This object provides access to DescriptionFramework's mod support API
	struct IDescriptionFrameworkInterface001
	{
		// Gets the DescriptionFramework build number
		virtual unsigned int GetBuildNumber() = 0;

		virtual const char* GetDescription(RE::TESForm* a_form) = 0;
	};

}  // namespace DescriptionFrameworkAPI
extern DescriptionFrameworkAPI::IDescriptionFrameworkInterface001* g_DescriptionFrameworkInterface;
