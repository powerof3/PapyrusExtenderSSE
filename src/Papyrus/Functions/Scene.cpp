#include "Papyrus/Functions/Scene.h"

namespace Papyrus::Scene
{
	std::vector<RE::Actor*> GetActorsInScene(STATIC_ARGS, RE::BGSScene* a_scene)
	{
		if (!a_scene) {
			a_vm->TraceStack("Scene is None", a_stackID);
			return {};
		}

		auto quest = a_scene->parentQuest;
		if (!quest) {
			a_vm->TraceStack("Scene has no parent quest", a_stackID);
			return {};
		}

		std::vector<RE::Actor*> actors;
		for (const auto& aliasID : a_scene->actors) {
			auto handle = quest->GetAliasedRef(aliasID);
			if (auto actor = handle.get().get()) {
				actors.push_back(actor->As<RE::Actor>());
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

		auto quest = a_scene->parentQuest;
		if (!quest) {
			a_vm->TraceStack("Scene has no parent quest", a_stackID);
			return false;
		}

		return std::ranges::find_if(a_scene->actors, [a_actor, quest](const auto& aliasID) {
			auto handle = quest->GetAliasedRef(aliasID);
			auto ref = handle.get().get();

			return ref == a_actor;
		}) != a_scene->actors.end();
	}

	void Bind(VM& a_vm)
	{
		BIND(GetActorsInScene);
		BIND(IsActorInScene);

		logger::info("Registered scene functions"sv);
	}
}
