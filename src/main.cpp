#include "Papyrus/Registration.h"
#include "Serialization/Keywords.h"
#include "Serialization/Perks.h"
#include "Version.h"


enum : std::uint32_t
{
	kSerializationVersion = 2,

	kPapyrusExtender = 'P3PE',
	kAddPerks = 'APTN',
	kRemovePerks = 'RPFN',
	kAddKeywords = 'AKTF',
	kRemoveKeywords = 'RKOF',
};


std::string DecodeTypeCode(std::uint32_t a_typeCode)
{
	constexpr std::size_t SIZE = sizeof(std::uint32_t);

	std::string sig;
	sig.resize(SIZE);
	char* iter = reinterpret_cast<char*>(&a_typeCode);
	for (std::size_t i = 0, j = SIZE - 2; i < SIZE - 1; ++i, --j) {
		sig[j] = iter[i];
	}
	return sig;
}


void SaveCallback(SKSE::SerializationInterface* a_intfc)
{
	auto perks = Serialize::Perks::GetSingleton();
	if (!perks->GetData(Serialize::kAdd).empty()) {
		if (!perks->Save(a_intfc, kAddPerks, kSerializationVersion, Serialize::kAdd)) {
			logger::critical("[%u] : Failed to save data!\n", perks->GetType(Serialize::kAdd));
			perks->Clear(Serialize::kAdd);
		}
	}
	if (!perks->GetData(Serialize::kRemove).empty()) {
		if (!perks->Save(a_intfc, kRemovePerks, kSerializationVersion, Serialize::kRemove)) {
			logger::critical("[%u] : Failed to save data!\n", perks->GetType(Serialize::kRemove));
			perks->Clear(Serialize::kRemove);
		}
	}

	auto keywords = Serialize::Keywords::GetSingleton();
	if (!keywords->GetData(Serialize::kAdd).empty()) {
		if (!keywords->Save(a_intfc, kAddKeywords, kSerializationVersion, Serialize::kAdd)) {
			logger::critical("[%u] : Failed to save data!\n", keywords->GetType(Serialize::kAdd));
			keywords->Clear(true);
		}
	}
	if (!keywords->GetData(Serialize::kRemove).empty()) {
		if (!keywords->Save(a_intfc, kRemoveKeywords, kSerializationVersion, Serialize::kRemove)) {
			logger::critical("[%u] : Failed to save data!\n", keywords->GetType(Serialize::kRemove));
			keywords->Clear(Serialize::kRemove);
		}
	}

	logger::info("Finished saving data");
}


void LoadCallback(SKSE::SerializationInterface* a_intfc)
{
	auto perks = Serialize::Perks::GetSingleton();
	perks->Clear(Serialize::kAdd);
	perks->Clear(Serialize::kRemove);

	auto keywords = Serialize::Keywords::GetSingleton();
	keywords->Clear(Serialize::kAdd);
	keywords->Clear(Serialize::kRemove);

	std::uint32_t type;
	std::uint32_t version;
	std::uint32_t length;
	while (a_intfc->GetNextRecordInfo(type, version, length)) {
		if (version != kSerializationVersion) {
			logger::critical("Loaded data is out of date! Read (%u), expected (%u) for type code (%s)", version, kSerializationVersion, DecodeTypeCode(type).c_str());
			continue;
		}

		switch (type) {
		case kAddPerks:
			if (!perks->Load(a_intfc, Serialize::kAdd)) {
				logger::critical("[%s] : Failed to load\n", perks->GetType(Serialize::kAdd));
				perks->Clear(Serialize::kAdd);
			}
			break;
		case kRemovePerks:
			if (!perks->Load(a_intfc, Serialize::kRemove)) {
				logger::critical("[%s] : Failed to load\n", perks->GetType(Serialize::kRemove));
				perks->Clear(Serialize::kRemove);
			}
			break;

		case kAddKeywords:
			if (!keywords->Load(a_intfc, Serialize::kAdd)) {
				logger::critical("[%s] : Failed to load\n", keywords->GetType(Serialize::kAdd));
				keywords->Clear(Serialize::kAdd);
			}
			break;
		case kRemoveKeywords:
			if (!keywords->Load(a_intfc, Serialize::kRemove)) {
				logger::critical("[%s] : Data not found or loaded\n", keywords->GetType(Serialize::kRemove));
				keywords->Clear(Serialize::kRemove);
			}
			break;
		default:
			logger::critical("Unrecognized record type (%s)!", DecodeTypeCode(type).c_str());
			break;
		}
	}

	logger::info("Finished loading data");
}


void OnInit(SKSE::MessagingInterface::Message* a_msg)
{
	if (a_msg->type == SKSE::MessagingInterface::kDataLoaded) {
		//...
	}
}


extern "C" DLLEXPORT bool APIENTRY SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	try {
		auto path = logger::log_directory() / "po3_papyrusextender64.log";
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
			logger::critical("Loaded in editor, marking as incompatible");
			return false;
		}

		const auto ver = a_skse->RuntimeVersion();
		if (ver < SKSE::RUNTIME_1_5_39) {
			logger::critical("Unsupported runtime version {}", ver.string());
			return false;
		}
	} catch (const std::exception& e) {
		logger::critical(e.what());
		return false;
	} catch (...) {
		logger::critical("caught unknown exception");
		return false;
	}

	return true;
}


extern "C" DLLEXPORT bool APIENTRY SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	try {
		logger::info("po3_PapyrusExtender loaded");

		if (!SKSE::Init(a_skse)) {
			return false;
		}

		/*const auto messaging = SKSE::GetMessagingInterface();
		if (!messaging->RegisterListener("SKSE", OnInit)) {
			_FATALERROR("Messaging interface registration failed!\n");
			return false;
		}*/

		Papyrus::Register();

		auto serialization = SKSE::GetSerializationInterface();
		serialization->SetUniqueID(kPapyrusExtender);
		serialization->SetSaveCallback(SaveCallback);
		serialization->SetLoadCallback(LoadCallback);
	} catch (const std::exception& e) {
		logger::critical(e.what());
		return false;
	} catch (...) {
		logger::critical("caught unknown exception");
		return false;
	}

	return true;
}


extern "C" DLLEXPORT const char* APIENTRY GetPluginVersion()
{
	return P3PE_VERSION_MAJOR_MINOR;
}
