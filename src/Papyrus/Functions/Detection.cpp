#include "Papyrus/Functions/Detection.h"

#include "Serialization/Services.h"

namespace Papyrus::Detection
{
	std::int32_t CanActorBeDetected(STATIC_ARGS, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return -1;
		}

		const auto targetManager = DETECTION::TargetManager::GetSingleton();
		if (targetManager->Contains(a_actor, DETECTION::kHide)) {
			return 0;
		}
		if (targetManager->Contains(a_actor, DETECTION::kAlert)) {
			return 2;
		}
		return 1;
	}

	std::int32_t CanActorDetect(STATIC_ARGS, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return -1;
		}

		const auto sourceManager = DETECTION::SourceManager::GetSingleton();
		if (sourceManager->Contains(a_actor, DETECTION::kHide)) {
			return 0;
		}
		if (sourceManager->Contains(a_actor, DETECTION::kAlert)) {
			return 2;
		}
		return 1;
	}

	void ForceActorDetection(STATIC_ARGS, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		DETECTION::TargetManager::GetSingleton()->Add(a_actor, DETECTION::kAlert);
	}

	void ForceActorDetecting(STATIC_ARGS, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		DETECTION::SourceManager::GetSingleton()->Add(a_actor, DETECTION::kAlert);
	}

	bool IsDetectedByAnyone(STATIC_ARGS, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		if (a_actor->currentProcess) {
			if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
				for (auto& targetHandle : processLists->highActorHandles) {
					if (const auto target = targetHandle.get(); target && target->currentProcess) {
						if (const auto base = target->GetActorBase(); base && !base->AffectsStealthMeter()) {
							continue;
						}
						if (target->RequestDetectionLevel(a_actor) > 0) {
							return true;
						}
					}
				}
			}
		}

		return false;
	}

	void PreventActorDetection(STATIC_ARGS, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		DETECTION::TargetManager::GetSingleton()->Add(a_actor, DETECTION::kHide);
	}

	void PreventActorDetecting(STATIC_ARGS, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		DETECTION::SourceManager::GetSingleton()->Add(a_actor, DETECTION::kHide);
	}

	void ResetActorDetection(STATIC_ARGS, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		DETECTION::TargetManager::GetSingleton()->Remove(a_actor);
	}

	void ResetActorDetecting(STATIC_ARGS, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		DETECTION::SourceManager::GetSingleton()->Remove(a_actor);
	}

	void Bind(VM& a_vm)
	{
		BIND(CanActorBeDetected);
		BIND(CanActorDetect);
		BIND(ForceActorDetection);
		BIND(ForceActorDetecting);
		BIND(IsDetectedByAnyone);
		BIND(PreventActorDetection);
		BIND(PreventActorDetecting);
		BIND(ResetActorDetection);
		BIND(ResetActorDetecting);

		logger::info("Registered detection functions"sv);
	}
}
