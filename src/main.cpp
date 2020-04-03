#include "po3_functions.h"
#include "po3_serializedclasses.h"


const SKSE::TaskInterface* g_task = nullptr;
RE::BGSKeyword* g_npcKeyword = nullptr;
RE::BGSArtObject* g_soulTrapArt = nullptr;

float version;

//----------------------------------------------------------------------------------------

enum : UInt32
{
	kSerializationVersion = 1,

	kPapyrusExtender = 'P3PE',
	kAddPerks = 'APTN',
	kRemovePerks = 'RPFN',
	kAddKeywords = 'AKTF',
	kRemoveKeywords = 'RKOF',
};

std::string DecodeTypeCode(UInt32 a_typeCode)
{
	constexpr std::size_t SIZE = sizeof(UInt32);

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
	auto perks = RE::Perks::GetSingleton();
	if (!perks->GetData(RE::Base::kAdd).empty()) {
		if (!perks->Save(a_intfc, kAddPerks, kSerializationVersion, RE::Base::kAdd)) {
			_ERROR("[%u] : Failed to save data!\n", perks->GetType(RE::Base::kAdd));
			perks->Clear(RE::Base::kAdd);
		}
	}
	if (!perks->GetData(RE::Base::kRemove).empty()) {
		if (!perks->Save(a_intfc, kRemovePerks, kSerializationVersion, RE::Base::kRemove)) {
			_ERROR("[%u] : Failed to save data!\n", perks->GetType(RE::Base::kRemove));
			perks->Clear(RE::Base::kRemove);
		}
	}

	auto keywords = RE::Keywords::GetSingleton();
	if (!keywords->GetData(RE::Base::kAdd).empty()) {
		if (!keywords->Save(a_intfc, kAddKeywords, kSerializationVersion, RE::Base::kAdd)) {
			_ERROR("[%u] : Failed to save data!\n", keywords->GetType(RE::Base::kAdd));
			keywords->Clear(true);
		}
	}
	if (!keywords->GetData(RE::Base::kRemove).empty()) {
		if (!keywords->Save(a_intfc, kRemoveKeywords, kSerializationVersion, RE::Base::kRemove)) {
			_ERROR("[%u] : Failed to save data!\n", keywords->GetType(RE::Base::kRemove));
			keywords->Clear(RE::Base::kRemove);
		}
	}

	_MESSAGE("Finished saving data");
}

void LoadCallback(SKSE::SerializationInterface* a_intfc)
{
	auto perks = RE::Perks::GetSingleton();
	perks->Clear(RE::Base::kAdd);
	perks->Clear(RE::Base::kRemove);

	auto keywords = RE::Keywords::GetSingleton();
	keywords->Clear(RE::Base::kAdd);
	keywords->Clear(RE::Base::kRemove);

	UInt32 type;
	UInt32 version;
	UInt32 length;
	while (a_intfc->GetNextRecordInfo(type, version, length)) {
		if (version != kSerializationVersion) {
			_ERROR("Loaded data is out of date! Read (%u), expected (%u) for type code (%s)", version, kSerializationVersion, DecodeTypeCode(type).c_str());
			continue;
		}

		switch (type) {
			case kAddPerks:
				if (!perks->Load(a_intfc, RE::Base::kAdd)) {
					_ERROR("[%u] : Data not found or loaded\n", perks->GetType(RE::Base::kAdd));
					perks->Clear(RE::Base::kAdd);
				}
				break;
			case kRemovePerks:
				if (!perks->Load(a_intfc, RE::Base::kRemove)) {
					_ERROR("[%u] : Data not found or loaded\n", perks->GetType(RE::Base::kRemove));
					perks->Clear(RE::Base::kRemove);
				}
				break;

			case kAddKeywords:
				if (!keywords->Load(a_intfc, RE::Base::kAdd)) {
					_ERROR("[%u] : Data not found or loaded\n", keywords->GetType(RE::Base::kAdd));
					keywords->Clear(RE::Base::kAdd);
				}
				break;
			case kRemoveKeywords:
				if (!keywords->Load(a_intfc, RE::Base::kRemove)) {
					_ERROR("[%u] : Data not found or loaded\n", keywords->GetType(RE::Base::kRemove));
					keywords->Clear(RE::Base::kRemove);
				}
				break;

			default:
				_ERROR("Unrecognized record type (%s)!", DecodeTypeCode(type).c_str());
				break;
		}
	}

	_MESSAGE("Finished loading data");
}

//----------------------------------------------------------------------------------------

void OnInit(SKSE::MessagingInterface::Message* a_msg)
{
	if (a_msg->type == SKSE::MessagingInterface::kDataLoaded) {
		g_npcKeyword = static_cast<RE::BGSKeyword*>(RE::BGSDefaultObjectManager::GetSingleton()->GetObject(RE::DEFAULT_OBJECTS::kKeywordNPC));
		g_soulTrapArt = RE::TESForm::LookupByID<RE::BGSArtObject>(RE::ArtSoulTrapTargetEffectsID);
	}
}

//----------------------------------------------------------------------------------------

extern "C" {
	bool SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
	{
		SKSE::Logger::OpenRelative(FOLDERID_Documents, L"\\My Games\\Skyrim Special Edition\\SKSE\\po3_papyrusextender64.log");
		SKSE::Logger::SetPrintLevel(SKSE::Logger::Level::kDebugMessage);
		SKSE::Logger::SetFlushLevel(SKSE::Logger::Level::kDebugMessage);
		SKSE::Logger::UseTimeStamp(true);

		_MESSAGE("Loading po3_PapyrusExtender");

		a_info->infoVersion = SKSE::PluginInfo::kVersion;
		a_info->name = "powerofthree's PapyrusExtender for SSE";
		version = a_info->version = static_cast<float>(2.4);

		if (a_skse->IsEditor()) {
			_FATALERROR("Loaded in editor, marking as incompatible!\n");
			return false;
		}

		auto ver = a_skse->RuntimeVersion();
		if (ver <= SKSE::RUNTIME_1_5_39) {
			_FATALERROR("Unsupported runtime version %s!\n", ver.GetString().c_str());
			return false;
		}

		return true;
	}

	bool SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
	{
		_MESSAGE("po3_PapyrusExtender SSE loaded");

		if (!SKSE::Init(a_skse)) {
			return false;
		}

		g_task = SKSE::GetTaskInterface();
		if (!g_task) {
			_FATALERROR("Task interface registration failed!\n");
			return false;
		}

		const auto messaging = SKSE::GetMessagingInterface();
		if (!messaging->RegisterListener("SKSE", OnInit)) {
			_FATALERROR("Messaging interface registration failed!\n");
			return false;
		}

		const auto papyrus = SKSE::GetPapyrusInterface();
		if (!papyrus->Register(RE::PO3_SKSEFunctions::Register)) {
			_FATALERROR("Failed to register papyrus callback!\n");
			return false;
		}

		auto serialization = SKSE::GetSerializationInterface();
		serialization->SetUniqueID(kPapyrusExtender);
		serialization->SetSaveCallback(SaveCallback);
		serialization->SetLoadCallback(LoadCallback);

		return true;
	}

	__declspec(dllexport) float GetPluginVersion()
	{
		return version;
	}
};