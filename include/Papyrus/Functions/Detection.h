#pragma once

namespace Papyrus::Detection
{
	std::int32_t CanActorBeDetected(STATIC_ARGS, RE::Actor* a_actor);
	std::int32_t CanActorDetect(STATIC_ARGS, RE::Actor* a_actor);
	void         ForceActorDetection(STATIC_ARGS, RE::Actor* a_actor);
	void         ForceActorDetecting(STATIC_ARGS, RE::Actor* a_actor);
	bool         IsDetectedByAnyone(STATIC_ARGS, RE::Actor* a_actor);
	void         PreventActorDetection(STATIC_ARGS, RE::Actor* a_actor);
	void         PreventActorDetecting(STATIC_ARGS, RE::Actor* a_actor);
	void         ResetActorDetection(STATIC_ARGS, RE::Actor* a_actor);
	void         ResetActorDetecting(STATIC_ARGS, RE::Actor* a_actor);

	void Bind(VM& a_vm);
}
