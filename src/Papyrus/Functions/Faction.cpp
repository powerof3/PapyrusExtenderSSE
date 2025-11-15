#include "Papyrus/Functions/Faction.h"

namespace Papyrus::Faction
{
	RE::TESObjectREFR* GetVendorFactionContainer(STATIC_ARGS, const RE::TESFaction* a_faction)
	{
		if (!a_faction) {
			a_vm->TraceStack("Faction is None", a_stackID);
			return nullptr;
		}

		return a_faction->vendorData.merchantContainer;
	}

	//SeaSparrow - New Functions
	std::vector<RE::Actor*> GetAllActorsInFaction(STATIC_ARGS, const RE::TESFaction* a_faction)
	{
		std::vector<RE::Actor*> result;

		if (!a_faction) {
			return result;
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
					if (actorPtr) {
						if (actorPtr->IsInFaction(a_faction)) {
							result.push_back(actorPtr.get());
						}
					}
				}
			}
		}

		return result;
	}

	void Bind(VM& a_vm)
	{
		std::uint32_t count = 0;
		
		BIND(GetVendorFactionContainer, true);
		BIND(GetAllActorsInFaction);
		logger::info("Registered {} faction functions"sv, count);
	}
}
