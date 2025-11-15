#include "Papyrus/Functions/Detection.h"

#include "Serialization/Manager.h"

namespace Papyrus::Detection
{
	using namespace Serialization;

	std::int32_t CanActorBeDetected(STATIC_ARGS, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return -1;
		}

		const auto& targetManager = Serialization::Manager::GetSingleton()->detectionTargets;
		if (targetManager.Contains(a_actor, DETECTION::kHide)) {
			return 0;
		}
		if (targetManager.Contains(a_actor, DETECTION::kAlert)) {
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

		const auto& sourceManager = Serialization::Manager::GetSingleton()->detectionSources;
		if (sourceManager.Contains(a_actor, DETECTION::kHide)) {
			return 0;
		}
		if (sourceManager.Contains(a_actor, DETECTION::kAlert)) {
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

		Serialization::Manager::GetSingleton()->detectionTargets.Add(a_actor, DETECTION::kAlert);
	}

	void ForceActorDetecting(STATIC_ARGS, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		Serialization::Manager::GetSingleton()->detectionSources.Add(a_actor, DETECTION::kAlert);
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

		Serialization::Manager::GetSingleton()->detectionTargets.Add(a_actor, DETECTION::kHide);
	}

	void PreventActorDetecting(STATIC_ARGS, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		Serialization::Manager::GetSingleton()->detectionSources.Add(a_actor, DETECTION::kHide);
	}

	void ResetActorDetection(STATIC_ARGS, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		Serialization::Manager::GetSingleton()->detectionTargets.Remove(a_actor);
	}

	void ResetActorDetecting(STATIC_ARGS, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		Serialization::Manager::GetSingleton()->detectionSources.Remove(a_actor);
	}

	void Bind(VM& a_vm)
	{
		std::uint32_t count = 0;
		
		BIND(CanActorBeDetected);
		BIND(CanActorDetect);
		BIND(ForceActorDetection);
		BIND(ForceActorDetecting);
		BIND(IsDetectedByAnyone);
		BIND(PreventActorDetection);
		BIND(PreventActorDetecting);
		BIND(ResetActorDetection);
		BIND(ResetActorDetecting);

		logger::info("Registered {} detection functions"sv, count);
	}
}
