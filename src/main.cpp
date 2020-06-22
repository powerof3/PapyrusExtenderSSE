#include "main.h"

#include "Papyrus/Registration.h"
#include "Serialization/Serialize.h"


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
	auto perks = Serialize::Perks::GetSingleton();
	if (!perks->GetData(Serialize::Base::kAdd).empty()) {
		if (!perks->Save(a_intfc, kAddPerks, kSerializationVersion, Serialize::Base::kAdd)) {
			_ERROR("[%u] : Failed to save data!\n", perks->GetType(Serialize::Base::kAdd));
			perks->Clear(Serialize::Base::kAdd);
		}
	}
	if (!perks->GetData(Serialize::Base::kRemove).empty()) {
		if (!perks->Save(a_intfc, kRemovePerks, kSerializationVersion, Serialize::Base::kRemove)) {
			_ERROR("[%u] : Failed to save data!\n", perks->GetType(Serialize::Base::kRemove));
			perks->Clear(Serialize::Base::kRemove);
		}
	}

	auto keywords = Serialize::Keywords::GetSingleton();
	if (!keywords->GetData(Serialize::Base::kAdd).empty()) {
		if (!keywords->Save(a_intfc, kAddKeywords, kSerializationVersion, Serialize::Base::kAdd)) {
			_ERROR("[%u] : Failed to save data!\n", keywords->GetType(Serialize::Base::kAdd));
			keywords->Clear(true);
		}
	}
	if (!keywords->GetData(Serialize::Base::kRemove).empty()) {
		if (!keywords->Save(a_intfc, kRemoveKeywords, kSerializationVersion, Serialize::Base::kRemove)) {
			_ERROR("[%u] : Failed to save data!\n", keywords->GetType(Serialize::Base::kRemove));
			keywords->Clear(Serialize::Base::kRemove);
		}
	}

	_MESSAGE("Finished saving data");
}


void LoadCallback(SKSE::SerializationInterface* a_intfc)
{
	auto perks = Serialize::Perks::GetSingleton();
	perks->Clear(Serialize::Base::kAdd);
	perks->Clear(Serialize::Base::kRemove);

	auto keywords = Serialize::Keywords::GetSingleton();
	keywords->Clear(Serialize::Base::kAdd);
	keywords->Clear(Serialize::Base::kRemove);

	UInt32 type;
	UInt32 serialized_ver;
	UInt32 length;
	while (a_intfc->GetNextRecordInfo(type, serialized_ver, length)) {
		if (serialized_ver != kSerializationVersion) {
			_ERROR("Loaded data is out of date! Read (%u), expected (%u) for type code (%s)", serialized_ver, kSerializationVersion, DecodeTypeCode(type).c_str());
			continue;
		}

		switch (type) {
			case kAddPerks:
				if (!perks->Load(a_intfc, Serialize::Base::kAdd)) {
					_ERROR("[%u] : Data not found or loaded\n", perks->GetType(Serialize::Base::kAdd));
					perks->Clear(Serialize::Base::kAdd);
				}
				break;
			case kRemovePerks:
				if (!perks->Load(a_intfc, Serialize::Base::kRemove)) {
					_ERROR("[%u] : Data not found or loaded\n", perks->GetType(Serialize::Base::kRemove));
					perks->Clear(Serialize::Base::kRemove);
				}
				break;

			case kAddKeywords:
				if (!keywords->Load(a_intfc, Serialize::Base::kAdd)) {
					_ERROR("[%u] : Data not found or loaded\n", keywords->GetType(Serialize::Base::kAdd));
					keywords->Clear(Serialize::Base::kAdd);
				}
				break;
			case kRemoveKeywords:
				if (!keywords->Load(a_intfc, Serialize::Base::kRemove)) {
					_ERROR("[%u] : Data not found or loaded\n", keywords->GetType(Serialize::Base::kRemove));
					keywords->Clear(Serialize::Base::kRemove);
				}
				break;

			default:
				_ERROR("Unrecognized record type (%s)!", DecodeTypeCode(type).c_str());
				break;
		}
	}

	_MESSAGE("Finished loading data");
}


void OnInit(SKSE::MessagingInterface::Message* a_msg)
{
	if (a_msg->type == SKSE::MessagingInterface::kDataLoaded) {
		return;
	}
}


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
		a_info->version = Version::version;

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

		const auto messaging = SKSE::GetMessagingInterface();
		if (!messaging->RegisterListener("SKSE", OnInit)) {
			_FATALERROR("Messaging interface registration failed!\n");
			return false;
		}

		Papyrus::Register();

		auto serialization = SKSE::GetSerializationInterface();
		serialization->SetUniqueID(kPapyrusExtender);
		serialization->SetSaveCallback(SaveCallback);
		serialization->SetLoadCallback(LoadCallback);

		return true;
	}


	__declspec(dllexport) float GetPluginVersion()
	{
		return Version::version;
	}
};