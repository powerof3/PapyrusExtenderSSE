#pragma once

namespace Papyrus::Faction
{
	inline RE::TESObjectREFR* GetVendorFactionContainer(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESFaction* a_faction)
	{
		if (!a_faction) {
			a_vm->TraceStack("Faction is None", a_stackID);
			return nullptr;
		}

		return a_faction->vendorData.merchantContainer;
	}

	//SeaSparrow - New Functions
	inline std::vector<RE::Actor*> GetAllActorsInFaction(VM*, StackID, RE::StaticFunctionTag*, RE::TESFaction* a_kFaction)
	{
		std::vector<RE::Actor*> response;

		if (!a_kFaction) {
			return response;
		}

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			RE::BSTArray<RE::ActorHandle>* highActorHandles = &processLists->highActorHandles;
			RE::BSTArray<RE::ActorHandle>* middleHighActorHandles = &processLists->middleHighActorHandles;
			RE::BSTArray<RE::ActorHandle>* middleLowActorHandles = &processLists->middleLowActorHandles;
			RE::BSTArray<RE::ActorHandle>* lowActorHandles = &processLists->lowActorHandles;

			std::vector<RE::BSTArray<RE::ActorHandle>*> allActors;
			allActors.push_back(highActorHandles);
			allActors.push_back(middleHighActorHandles);
			allActors.push_back(middleLowActorHandles);
			allActors.push_back(lowActorHandles);

			for (auto array : allActors) {
				for (auto& actorHandle : *array) {
					auto actorPtr = actorHandle.get();

					if (auto actor = actorPtr.get(); actor) {
						if (actor->IsInFaction(a_kFaction)) {
							response.push_back(actor);
						}
					}
				}
			}
		}
		return response;
	}

	inline void Bind(VM& a_vm)
	{
		BIND(GetVendorFactionContainer, true);
		BIND(GetAllActorsInFaction);
		logger::info("Registered faction functions"sv);
	}
}
