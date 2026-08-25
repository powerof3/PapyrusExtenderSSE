#include "Game/Manager.h"
#include "Papyrus/Manager.h"
#include "Serialization/Manager.h"

void OnInit(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kPostPostLoad:
		{
			REX::INFO("{:*^30}", "POSTLOAD API"sv);
			DescriptionFrameworkAPI::GetDescriptionFrameworkInterface001();
			REX::INFO("Description Framework installed: {}", g_DescriptionFrameworkInterface != nullptr);
		}
		break;
	case SKSE::MessagingInterface::kDataLoaded:
		{
			Game::Register();
			Serialization::Manager::Register();
		}
		break;
	case SKSE::MessagingInterface::kPostLoadGame:
	case SKSE::MessagingInterface::kNewGame:
		{
			REX::INFO("{:*^30}", "POSTLOAD API"sv);
			DismemberingFrameworkAPI::LoadAPI();
			REX::INFO("Dismembering Framework installed: {} (version {})", DismemberingFrameworkAPI::g_API != nullptr, DismemberingFrameworkAPI::g_API ? DismemberingFrameworkAPI::g_API->GetVersion() : -1);
			NGDecapitationsAPI::LoadAPI();
			REX::INFO("Next Gen Decapitations installed: {} (version {})", NGDecapitationsAPI::g_API != nullptr, NGDecapitationsAPI::g_API ? NGDecapitationsAPI::g_API->GetVersion() : -1);
		}
		break;
	default:
		break;
	}
}

#ifdef SKYRIM_SUPPORT_AE
SKSE_PLUGIN_VERSION = []() {
	SKSE::PluginVersionData v;
	v.PluginVersion(REL::Version{ Version::MAJOR, Version::MINOR, Version::PATCH });
	v.PluginName("powerofthree's Papyrus Extender");
	v.AuthorName("powerofthree");
	v.UsesAddressLibrary();
	v.UsesUpdatedStructs();
	v.CompatibleVersions({ SKSE::RUNTIME_SSE_LATEST });

	if constexpr (SKSE::RUNTIME_SSE_LATEST < Runtime::MIN_ADDRESS_LIBRARY_V5) {
		v.MinimumRequiredXSEVersion(REL::Version{ 2, 2, 5 });
	} else {
		v.MinimumRequiredXSEVersion(REL::Version{ 2, 3, 0 });
	}

	return v;
}();
#else
SKSE_PLUGIN_QUERY(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = "powerofthree's Papyrus Extender";
	a_info->version = Version::MAJOR;

	if (a_skse->IsEditor()) {
		REX::CRITICAL("Loaded in editor, marking as incompatible");
		return false;
	}

	const auto ver = a_skse->RuntimeVersion();
	if (ver
#	ifndef SKYRIMVR
		< SKSE::RUNTIME_SSE_1_5_39
#	else
		> SKSE::RUNTIME_VR_1_4_15_1
#	endif
	) {
		REX::CRITICAL("Unsupported runtime version {}", ver.string());
		return false;
	}

	return true;
}
#endif

SKSE_PLUGIN_LOAD(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse, { .log = true,
						   .logName = Version::PROJECT.data(),
						   .trampoline = true,
						   .trampolineSize = 264 });

	Runtime::Init(a_skse->RuntimeVersion());

	const auto papyrus = SKSE::GetPapyrusInterface();
	papyrus->Register(Papyrus::Bind);

	const auto serialization = SKSE::GetSerializationInterface();
	serialization->SetUniqueID(Serialization::kPapyrusExtender);
	serialization->SetSaveCallback(Serialization::SaveCallback);
	serialization->SetLoadCallback(Serialization::LoadCallback);
	serialization->SetRevertCallback(Serialization::RevertCallback);
	serialization->SetFormDeleteCallback(Serialization::FormDeleteCallback);

	const auto messaging = SKSE::GetMessagingInterface();
	messaging->RegisterListener(OnInit);

	return true;
}

extern "C" DLLEXPORT const char* APIENTRY GetPluginVersion()
{
	return Version::NAME.data();
}
