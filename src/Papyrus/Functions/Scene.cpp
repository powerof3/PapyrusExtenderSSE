#include "Papyrus/Functions/Scene.h"

namespace Papyrus::Scene
{
	std::vector<RE::Actor*> GetActorsInScene(STATIC_ARGS, RE::BGSScene* a_scene)
	{
		if (!a_scene) {
			a_vm->TraceStack("Scene is None", a_stackID);
			return {};
		}

		std::vector<RE::Actor*> actors;

		for (auto& formID : a_scene->actors) {
			if (auto actor = RE::TESForm::LookupByID<RE::Actor>(formID)) {
				actors.push_back(actor);
			}
		}

		return actors;
	}

	bool IsActorInScene(STATIC_ARGS, RE::BGSScene* a_scene, RE::Actor* a_actor)
	{
		if (!a_scene) {
			a_vm->TraceStack("Scene is None", a_stackID);
			return false;
		}

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		return std::ranges::find(a_scene->actors, a_actor->GetFormID()) != a_scene->actors.end();
	}
	
	void Bind(VM& a_vm)
	{
		BIND(GetActorsInScene);
		BIND(IsActorInScene);

		logger::info("Registered scene functions"sv);
	}
}
