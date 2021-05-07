#include "Hooks/EventHook.h"
#include "Papyrus/Registration.h"
#include "Serialization/Manager.h"

#include "Version.h"


static auto DetectOldVersion() -> std::vector<std::string>
{
	std::vector<std::string> vec;

	const auto papyrusExtender64Handle = GetModuleHandle("po3_papyrusextender64");

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


void OnInit(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kPostLoad:
		{
			auto vec = DetectOldVersion();
			if (!vec.empty() && vec.size() == 2) {
				auto id = MessageBoxA(nullptr, vec[0].c_str(), vec[1].c_str(), MB_OKCANCEL);
				if (id == 2) {
					std::_Exit(EXIT_FAILURE);
				}
			}
		}
		break;
	case SKSE::MessagingInterface::kDataLoaded:
		{
			Papyrus::Events::RegisterScriptEvents();
			Papyrus::Events::RegisterStoryEvents();

			Hook::HookEvents();
		}
		break;
	default:
		break;
	}
}


extern "C" DLLEXPORT bool APIENTRY SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	try {
		auto path = logger::log_directory().value() / "po3_papyrusextender64.log";
		auto log = spdlog::basic_logger_mt("global log", path.string(), true);
		log->flush_on(spdlog::level::info);

		log->set_level(spdlog::level::info);

		spdlog::set_default_logger(log);
		spdlog::set_pattern("[%l] %v");

		logger::info("Papyrus Extender v{}", P3PE_VERSION_VERSTRING);

		a_info->infoVersion = SKSE::PluginInfo::kVersion;
		a_info->name = "powerofthree's Papyrus Extender";
		a_info->version = P3PE_VERSION_MAJOR;

		if (a_skse->IsEditor()) {
			logger::critical("Loaded in editor, marking as incompatible"sv);
			return false;
		}

		const auto ver = a_skse->RuntimeVersion();
		if (ver < SKSE::RUNTIME_1_5_39) {
			logger::critical("Unsupported runtime version {}"sv, ver.string());
			return false;
		}
	} catch (const std::exception& e) {
		logger::critical(e.what());
		return false;
	} catch (...) {
		logger::critical("caught unknown exception"sv);
		return false;
	}

	return true;
}


extern "C" DLLEXPORT bool APIENTRY SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	try {
		logger::info("po3_PapyrusExtender loaded"sv);

		SKSE::Init(a_skse);
		SKSE::AllocTrampoline(115);

		Papyrus::Functions::Register();

		const auto messaging = SKSE::GetMessagingInterface();
		if (!messaging->RegisterListener("SKSE", OnInit)) {
			return false;
		}

		auto serialization = SKSE::GetSerializationInterface();
		serialization->SetUniqueID(Serialization::kPapyrusExtender);
		serialization->SetSaveCallback(Serialization::SaveCallback);
		serialization->SetLoadCallback(Serialization::LoadCallback);

	} catch (const std::exception& e) {
		logger::critical(e.what());
		return false;
	} catch (...) {
		logger::critical("caught unknown exception"sv);
		return false;
	}

	return true;
}


extern "C" DLLEXPORT const char* APIENTRY GetPluginVersion()
{
	return P3PE_VERSION_MAJOR_MINOR;
}
