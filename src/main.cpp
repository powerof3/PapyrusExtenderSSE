#include "Game/Manager.h"
#include "Papyrus/Manager.h"
#include "Serialization/Manager.h"

#ifndef SKYRIM_AE
static std::vector<std::string> DetectOldVersion()
{
	std::vector<std::string> vec;

	const auto papyrusExtender64Handle = GetModuleHandleA("po3_papyrusextender64");

	std::string message;
	std::string info;

	if (papyrusExtender64Handle != nullptr) {
		logger::error("Outdated Papyrus Extender64 version detected");

		info = "Papyrus Extender - Plugin Conflict";

		message = R"(An outdated Papyrus Extender plugin has been found. This may cause script errors and CTDs with the latest version. 

Please remove po3_papyrusextender64.dll (and PO3_SKSEFunctions.pex) from any mods that include this plugin, such as "Diziet's Player Home Bath Undressing for SkyrimSE")";
	}

	if (!message.empty() && !info.empty()) {
		message += R"(


Click Ok to continue, or Cancel to quit the game)";
		vec.push_back(message);
		vec.push_back(info);
	}

	return vec;
}
#endif

void OnInit(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
#ifndef SKYRIM_AE
	case SKSE::MessagingInterface::kPostLoad:
		{
			const auto vec = DetectOldVersion();
			if (!vec.empty() && vec.size() == 2) {
				const auto id = WinAPI::MessageBox(nullptr, vec[0].c_str(), vec[1].c_str(), 0x00000001);
				if (id == 2) {
					std::_Exit(EXIT_FAILURE);
				}
			}
		}
		break;
#endif
	case SKSE::MessagingInterface::kDataLoaded:
		{
			Game::Register();
			Serialization::FormDeletion::EventHandler::Register();
		}
		break;
	default:
		break;
	}
}

#ifdef SKYRIM_AE
extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []() {
	SKSE::PluginVersionData v;
	v.PluginVersion(Version::MAJOR);
	v.PluginName("powerofthree's Papyrus Extender");
	v.AuthorName("powerofthree");
	v.UsesAddressLibrary(true);
	v.CompatibleVersions({ SKSE::RUNTIME_LATEST });

	return v;
}();
#else
extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = "powerofthree's Papyrus Extender";
	a_info->version = Version::MAJOR;

	if (a_skse->IsEditor()) {
		logger::critical("Loaded in editor, marking as incompatible"sv);
		return false;
	}

	const auto ver = a_skse->RuntimeVersion();
	if (ver
#	ifndef SKYRIMVR
		< SKSE::RUNTIME_1_5_39
#	else
		> SKSE::RUNTIME_VR_1_4_15_1
#	endif
	) {
		logger::critical(FMT_STRING("Unsupported runtime version {}"), ver.string());
		return false;
	}

	return true;
}
#endif

void InitializeLog()
{
	auto path = logger::log_directory();
	if (!path) {
		stl::report_and_fail("Failed to find standard logging directory"sv);
	}

	*path /= "po3_papyrusextender64.log"sv;
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::info);

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("[%H:%M:%S] [%l] %v"s);

	logger::info(FMT_STRING("{} v{}"), Version::PROJECT, Version::NAME);
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	InitializeLog();

	logger::info("Game version : {}", a_skse->RuntimeVersion().string());

	SKSE::Init(a_skse);

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
