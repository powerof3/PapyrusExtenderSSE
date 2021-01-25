#include "Papyrus/Registration.h"
#include "Serialization/Manager.h"
#include "Hooks/EventHook.h"

#include "Version.h"


void DumpScriptCommands()
{
	logger::info("");
	
	auto scriptCommands = RE::SCRIPT_FUNCTION::GetFirstScriptCommand();
	for (std::uint16_t i = 0; i < RE::SCRIPT_FUNCTION::Commands::kScriptCommandsEnd; ++i) {
		logger::info("{} : {} : {}", scriptCommands[i].functionName, scriptCommands[i].params ? scriptCommands[i].params->paramName : "NONE", scriptCommands[i].params ? to_underlying(scriptCommands[i].params->paramType.get()) : 0);
	}
}


void OnInit(SKSE::MessagingInterface::Message* a_msg)
{
	if (a_msg->type == SKSE::MessagingInterface::kDataLoaded) {
		
		Papyrus::Events::RegisterScriptEvents();

		Papyrus::Events::RegisterStoryEvents();

		Hook::HookEvents();

		//DumpScriptCommands();
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
		SKSE::AllocTrampoline(1 << 6);

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
