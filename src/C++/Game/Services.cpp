#include "Serialization/Services.h"

namespace DETECTION
{
	struct CalculateDetection
	{
		static std::uint8_t* thunk(RE::Actor* a_source, RE::Actor* a_target, std::int32_t& a_detectionValue, std::uint8_t& a_unk04, std::uint8_t& a_unk05, std::uint32_t& a_unk06, RE::NiPoint3& a_pos, float& a_unk08, float& a_unk09, float& a_unk10)
		{
			const auto sourceManager = SourceManager::GetSingleton();
			const auto targetManager = TargetManager::GetSingleton();

			if (a_source && sourceManager->Contains(a_source, kHide) || a_target && targetManager->Contains(a_target, kHide)) {
				a_detectionValue = -1000;
				return nullptr;
			}
			const auto result = func(a_source, a_target, a_detectionValue, a_unk04, a_unk05, a_unk06, a_pos, a_unk08, a_unk09, a_unk10);
			if (a_source && sourceManager->Contains(a_source, kAlert) || a_target && targetManager->Contains(a_target, kAlert)) {
				a_detectionValue = 1000;
			}

			return result;
		}
		static inline REL::Relocation<decltype(&thunk)> func;
	};

	void Register()
	{
		REL::Relocation<std::uintptr_t> target{ REL::ID(41659), 0x526 };
		stl::write_thunk_call<CalculateDetection>(target.address());

		logger::info("Installing detection hook");
	}
}
