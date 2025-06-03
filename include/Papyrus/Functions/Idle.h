#pragma once

namespace Papyrus::Idle
{
	RE::BSFixedString GetAnimationEventName(STATIC_ARGS, RE::TESIdleForm* a_idle);
	RE::BSFixedString GetAnimationFileName(STATIC_ARGS, RE::TESIdleForm* a_idle);

	void Bind(VM& a_vm);
}
