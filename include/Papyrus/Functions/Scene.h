#pragma once

namespace Papyrus::Scene
{
	std::vector<RE::Actor*> GetActorsInScene(STATIC_ARGS, RE::BGSScene* a_scene);
	bool                    IsActorInScene(STATIC_ARGS, RE::BGSScene* a_scene, RE::Actor* a_actor);

	void Bind(VM& a_vm);
}
