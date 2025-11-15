#include "Papyrus/Functions/Idle.h"

namespace Papyrus::Idle
{
	RE::BSFixedString GetAnimationEventName(STATIC_ARGS, RE::TESIdleForm* a_idle)
	{
		if (!a_idle) {
			a_vm->TraceStack("Idle is None", a_stackID);
			return ""sv;
		}

		return a_idle->animEventName;
	}

	RE::BSFixedString GetAnimationFileName(STATIC_ARGS, RE::TESIdleForm* a_idle)
	{
		if (!a_idle) {
			a_vm->TraceStack("Idle is None", a_stackID);
			return ""sv;
		}

		return a_idle->animFileName;
	}

	void Bind(VM& a_vm)
	{
		std::uint32_t count = 0;
		
		BIND(GetAnimationEventName);
		BIND(GetAnimationFileName);

		logger::info("Registered {} idle functions"sv, count);
	}
}
