#pragma once

namespace Papyrus::Idle
{
	inline RE::BSFixedString GetAnimationEventName(STATIC_ARGS, RE::TESIdleForm* a_idle)
	{
		if (!a_idle) {
			a_vm->TraceStack("Idle is None", a_stackID);
			return ""sv;
		}

		return a_idle->animEventName;
	}

	inline RE::BSFixedString GetAnimationFileName(STATIC_ARGS, RE::TESIdleForm* a_idle)
	{
		if (!a_idle) {
			a_vm->TraceStack("Idle is None", a_stackID);
			return ""sv;
		}

		return a_idle->animFileName;
	}

	inline void Bind(VM& a_vm)
	{
		BIND(GetAnimationEventName);
		BIND(GetAnimationFileName);

		logger::info("Registered idle functions"sv);
	}
}
