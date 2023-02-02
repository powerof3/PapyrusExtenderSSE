#pragma once

#include "Papyrus/Util/Inventory.h"
#include "Serialization/Services.h"

namespace Papyrus::Actor
{
	inline constexpr RE::FormID SoulTrapHitArtID = 0x000531AE;

	inline bool AddBasePerk(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::BGSPerk* a_perk)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}
		if (!a_perk) {
			a_vm->TraceStack("Perk is None", a_stackID);
			return false;
		}

		return FORM::PerkManager::GetSingleton()->Add(a_actor, a_perk);
	}

	inline bool AddBaseSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::SpellItem* a_spell)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}
		if (!a_spell) {
			a_vm->TraceStack("Spell is None", a_stackID);
			return false;
		}
		if (a_actor->HasSpell(a_spell)) {
			a_vm->TraceForm(a_actor, "already has spell", a_stackID, Severity::kInfo);
			return false;
		}

		const auto actorbase = a_actor->GetActorBase();
		const auto actorEffects = actorbase ? actorbase->GetSpellList() : nullptr;

		if (actorEffects && actorEffects->AddSpell(a_spell)) {
			if (const auto combatController = a_actor->combatController; combatController && combatController->inventory) {
				combatController->inventory->dirty = true;
			}
			if (actorbase->IsPlayer() || a_spell->GetSpellType() == RE::MagicSystem::SpellType::kLeveledSpell) {
				RE::SpellsLearned::SendEvent(a_spell);
			}
			return true;
		}

		return false;
	}

	inline std::vector<RE::TESForm*> AddAllEquippedItemsToArray(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		std::vector<RE::TESForm*> result;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return result;
		}

		auto inv = a_actor->GetInventory();
		for (const auto& [item, data] : inv) {
			if (item->Is(RE::FormType::LeveledItem)) {
				continue;
			}
			const auto& [count, entry] = data;
			if (count > 0 && entry->IsWorn()) {
				result.push_back(item);
			}
		}

		return result;
	}

	inline bool ApplyPoisonToEquippedWeapon(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::AlchemyItem* a_poison,
		std::uint32_t a_count,
		bool a_leftHand)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}
		if (!a_poison) {
			a_vm->TraceStack("Poison is None", a_stackID);
			return false;
		}

		if (const auto equippedEntryData = a_actor->GetEquippedEntryData(a_leftHand); equippedEntryData) {
			if (equippedEntryData->IsPoisoned()) {
				return false;
			}
			equippedEntryData->PoisonObject(a_poison, a_count);

			return true;
		}

		return false;
	}

	inline void DecapitateActor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		a_actor->Decapitate();
	}

	inline void FreezeActor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		std::uint32_t a_type,
		bool a_enable)
	{
		using Flags = RE::CHARACTER_FLAGS;
		using BOOL_BITS = RE::Actor::BOOL_BITS;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		const auto root = a_actor->Get3D(false);
		if (!root) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return;
		}

		if (a_type == 0) {
			if (const auto charController = a_actor->GetCharController(); charController) {
				if (a_enable) {  //freeze
					charController->flags.set(Flags::kNotPushable, Flags::kNoCharacterCollisions);
					charController->flags.reset(Flags::kRecordHits, Flags::kHitFlags, Flags::kHitDamage);
				} else {  //unfreeze
					charController->flags.reset(Flags::kNotPushable, Flags::kNoCharacterCollisions);
					charController->flags.set(Flags::kRecordHits, Flags::kHitFlags, Flags::kHitDamage);
				}
				charController->SetLinearVelocityImpl(0.0f);
			}
			if (a_enable) {
				a_actor->boolBits.reset(BOOL_BITS::kProcessMe);  //disable AI
			} else {
				a_actor->boolBits.set(BOOL_BITS::kProcessMe);  //enable AI
			}
		} else if (a_type == 1) {
			if (const auto charController = a_actor->GetCharController(); charController) {
				SKSE::GetTaskInterface()->AddTask([root, charController, a_actor, a_enable]() {
					std::uint32_t filterInfo = 0;
					charController->GetCollisionFilterInfo(filterInfo);
					if (a_enable) {
						a_actor->boolBits.set(BOOL_BITS::kParalyzed);
						root->SetCollisionLayerAndGroup(RE::COL_LAYER::kDeadBip, filterInfo + 1);
						root->UpdateRigidConstraints(true);
					} else {
						a_actor->boolBits.reset(BOOL_BITS::kParalyzed);
						root->UpdateRigidConstraints(false);
						root->SetCollisionLayerAndGroup(RE::COL_LAYER::kDeadBip, filterInfo >> 16);
					}
				});
			}
		}
	}

	inline std::vector<RE::EffectSetting*> GetActiveEffects(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		bool a_inactive)
	{
		using AE = RE::ActiveEffect::Flag;

		std::vector<RE::EffectSetting*> result;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return result;
		}
#ifndef SKYRIMVR
		const auto activeEffects = a_actor->GetActiveEffectList();
		if (!activeEffects) {
			a_vm->TraceForm(a_actor, "has no active effects", a_stackID, Severity::kInfo);
			return result;
		}

		for (const auto& activeEffect : *activeEffects) {
			if (auto mgef = activeEffect ? activeEffect->GetBaseObject() : nullptr; mgef) {
				if (!a_inactive && (activeEffect->flags.all(AE::kInactive) || activeEffect->flags.all(AE::kDispelled))) {
					continue;
				}
				result.push_back(mgef);
			}
		}
#else
		a_actor->VisitActiveEffects([&](RE::ActiveEffect* activeEffect) -> RE::BSContainer::ForEachResult {
			if (auto mgef = activeEffect ? activeEffect->GetBaseObject() : nullptr; mgef) {
				if (!a_inactive && (activeEffect->flags.all(AE::kInactive) || activeEffect->flags.all(AE::kDispelled))) {
					return RE::BSContainer::ForEachResult::kContinue;
				}
				result.push_back(mgef);
			}
			return RE::BSContainer::ForEachResult::kContinue;
		});
#endif

		return result;
	}

	inline float GetActorAlpha(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return 1.0f;
		}

		const auto process = a_actor->currentProcess;
		const auto middleProcess = process ? process->middleHigh : nullptr;

		return middleProcess ? middleProcess->alphaMult : 1.0f;
	}

	inline std::int32_t GetActorKnockState(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return -1;
		}

		return stl::to_underlying(a_actor->GetKnockState());
	}

	inline float GetActorRefraction(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return 1.0f;
		}

		const auto process = a_actor->currentProcess;
		const auto middleProcess = process ? process->middleHigh : nullptr;

		return middleProcess ? middleProcess->scriptRefractPower : 1.0f;
	}

	inline std::int32_t GetActorSoulSize(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return -1;
		}

		return stl::to_underlying(a_actor->GetSoulSize());
	}

	inline std::int32_t GetActorState(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return -1;
		}

		return stl::to_underlying(a_actor->GetLifeState());
	}

	inline float GetActorValueModifier(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::Actor* a_actor,
		std::int32_t a_modifier,
		RE::BSFixedString a_actorValue)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return 0.0f;
		}

		const auto actorValueList = RE::ActorValueList::GetSingleton();
		const auto actorValue = actorValueList ?
		                            actorValueList->LookupActorValueByName(a_actorValue) :
		                            RE::ActorValue::kNone;

		const auto modifier = static_cast<RE::ACTOR_VALUE_MODIFIER>(a_modifier);
		return actorValue != RE::ActorValue::kNone ?
		           a_actor->GetActorValueModifier(modifier, actorValue) :
		           0.0f;
	}

	inline std::uint32_t GetCriticalStage(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return 0;
		}

		return stl::to_underlying(a_actor->criticalStage.get());
	}

	inline std::vector<RE::Actor*> GetCombatAllies(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		std::vector<RE::Actor*> result;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return result;
		}

		if (const auto combatGroup = a_actor->GetCombatGroup(); combatGroup) {
			for (auto& memberData : combatGroup->members) {
				if (auto ally = memberData.memberHandle.get(); ally) {
					result.push_back(ally.get());
				}
			}
		}

		return result;
	}

	inline std::vector<RE::Actor*> GetCombatTargets(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		std::vector<RE::Actor*> result;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return result;
		}

		if (const auto combatGroup = a_actor->GetCombatGroup(); combatGroup) {
			for (auto& targetData : combatGroup->targets) {
				auto target = targetData.targetHandle.get();
				if (target) {
					result.push_back(target.get());
				}
			}
		}

		return result;
	}

	inline std::vector<RE::Actor*> GetCommandedActors(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		std::vector<RE::Actor*> result;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return result;
		}

		const auto process = a_actor->currentProcess;
		const auto middleHigh = process ? process->middleHigh : nullptr;

		if (middleHigh) {
			for (auto& commandedActorData : middleHigh->commandedActors) {
				const auto commandedActor = commandedActorData.commandedActor.get();
				if (commandedActor) {
					result.push_back(commandedActor.get());
				}
			}
		}

		return result;
	}

	inline RE::Actor* GetCommandingActor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return nullptr;
		}

		if (a_actor->IsCommandedActor()) {
			return a_actor->GetCommandingActor().get();
		}

		return nullptr;
	}

	inline RE::TESAmmo* GetEquippedAmmo(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return nullptr;
		}

		return a_actor->GetCurrentAmmo();
	}

#ifdef SKYRIMVR
	// Fix missing GetEquippedArmorInSlot declared in SKSEVR but that doesn't exist in VR.
	// https://www.creationkit.com/index.php?title=Actor_Script#Special_Edition_Exclusive_Functions
	inline RE::TESObjectARMO* GetEquippedArmorInSlot(VM* a_vm, StackID a_stackID, RE::Actor* a_actor, std::int32_t a_slot)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return nullptr;
		}
		auto biped_slot = (a_slot - 30) >= 0 ? 1 << (a_slot - 30) : 0;
		auto result = a_actor->GetWornArmor(static_cast<RE::BGSBipedObjectForm::BipedObjectSlot>(biped_slot));
		logger::debug("GetEquippedArmor running on {} with slot {} returning biped_slot {} with {}", a_actor->GetDisplayFullName(), a_slot, biped_slot, result ? result->GetFullName() : "None");
		return result;
	}
#endif

	inline bool GetEquippedWeaponIsPoisoned(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		bool a_leftHand)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		const auto equippedEntryData = a_actor->GetEquippedEntryData(a_leftHand);
		return equippedEntryData && equippedEntryData->IsPoisoned();
	}

	inline RE::AlchemyItem* GetEquippedWeaponPoison(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		bool a_leftHand)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return nullptr;
		}

		const auto xPoison = INV::get_equipped_weapon_poison_data(a_actor, a_leftHand);
		return xPoison ? xPoison->poison : nullptr;
	}

	inline std::uint32_t GetEquippedWeaponPoisonCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		bool a_leftHand)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return 0;
		}

		const auto xPoison = INV::get_equipped_weapon_poison_data(a_actor, a_leftHand);
		return xPoison ? xPoison->count : 0;
	}

	inline float GetEquippedWeight(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return 0.0f;
		}

		return a_actor->GetEquippedWeight();
	}

	inline RE::Actor* GetMount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return nullptr;
		}

		RE::NiPointer<RE::Actor> mount;
		if (a_actor->GetMount(mount)) {
			return mount.get();
		}

		return nullptr;
	}

	inline float GetLocalGravityActor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return 1.0f;
		}

		const auto charController = a_actor->GetCharController();
		return charController ? charController->gravity : 1.0f;
	}

	inline RE::TESObjectREFR* GetObjectUnderFeet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return nullptr;
		}

		const auto charController = a_actor->GetCharController();
		const auto supportBody = charController ? charController->supportBody : nullptr;

		return supportBody ? supportBody->GetUserData() : nullptr;
	}

	inline bool GetOffersServices(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		return a_actor->CanOfferServices();
	}

	inline RE::Actor* GetRider(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return nullptr;
		}

		RE::NiPointer<RE::Actor> rider;
		if (a_actor->GetRider(rider)) {
			return rider.get();
		}

		return nullptr;
	}

	inline RE::TESPackage* GetRunningPackage(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return nullptr;
		}

		const auto currentProcess = a_actor->currentProcess;
		return currentProcess ? currentProcess->GetRunningPackage() : nullptr;
	}

	inline float GetTimeDead(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return 0.0f;
		}

		const auto currentProcess = a_actor->currentProcess;
		const auto timeOfDeath = currentProcess ? currentProcess->deathTime : 0.0f;

		if (timeOfDeath > 0.0f) {
			if (const auto calendar = RE::Calendar::GetSingleton(); calendar) {
				const auto g_gameDaysPassed = calendar->gameDaysPassed;
				return g_gameDaysPassed ? floorf(g_gameDaysPassed->value * 24.0f) - timeOfDeath : 0.0f;
			}
		}

		return 0.0f;
	}

	inline float GetTimeOfDeath(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return 0.0f;
		}

		const auto currentProcess = a_actor->currentProcess;
		const auto timeOfDeath = currentProcess ? currentProcess->deathTime : 0.0f;

		return timeOfDeath > 0.0f ? timeOfDeath / 24.0f : 0.0f;
	}

	inline RE::TESFaction* GetVendorFaction(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return nullptr;
		}

		return a_actor->GetVendorFaction();
	}

	inline bool HasActiveMagicEffect(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		const RE::EffectSetting* a_mgef)
	{
		using AE = RE::ActiveEffect::Flag;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}
		if (!a_mgef) {
			a_vm->TraceStack("Magic Effect is None", a_stackID);
			return false;
		}
#ifndef SKYRIMVR
		if (const auto activeEffects = a_actor->GetActiveEffectList(); activeEffects) {
#else
		const auto activeEffects = new std::vector<RE::ActiveEffect*>;
		a_actor->VisitActiveEffects([&](RE::ActiveEffect* ae) -> RE::BSContainer::ForEachResult {
			if (ae)
				activeEffects->push_back(ae);
			return RE::BSContainer::ForEachResult::kContinue;
		});
		if (activeEffects) {
#endif
			return std::ranges::any_of(*activeEffects, [&](auto const& ae) {
				return ae && ae->effect && ae->effect->baseEffect == a_mgef && ae->flags.none(AE::kInactive) && ae->flags.none(AE::kDispelled);
			});
		}
		return false;
	}

	inline bool HasActiveSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		const RE::SpellItem* a_spell)
	{
		using AE = RE::ActiveEffect::Flag;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}
		if (!a_spell) {
			a_vm->TraceStack("Spell is None", a_stackID);
			return false;
		}
#ifndef SKYRIMVR
		if (const auto activeEffects = a_actor->GetActiveEffectList(); activeEffects) {
#else
		const auto activeEffects = new std::vector<RE::ActiveEffect*>;
		a_actor->VisitActiveEffects([&](RE::ActiveEffect* ae) -> RE::BSContainer::ForEachResult {
			if (ae)
				activeEffects->push_back(ae);
			return RE::BSContainer::ForEachResult::kContinue;
		});
		if (activeEffects) {
#endif
			return std::ranges::any_of(*activeEffects, [&](auto const& ae) {
				return ae && ae->spell == a_spell && ae->flags.none(AE::kInactive) && ae->flags.none(AE::kDispelled);
			});
		}
		return false;
	}

	inline bool HasDeferredKill(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		const auto currentProcess = a_actor->currentProcess;
		const auto middleProcess = currentProcess ? currentProcess->middleHigh : nullptr;

		return middleProcess && middleProcess->inDeferredKill;
	}

	inline bool HasMagicEffectWithArchetype(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::BSFixedString a_archetype)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}
		if (a_archetype.empty()) {
			a_vm->TraceStack("Archetype is None", a_stackID);
			return false;
		}
#ifndef SKYRIMVR
		if (const auto activeEffects = a_actor->GetActiveEffectList(); activeEffects) {
#else
		const auto activeEffects = new std::vector<RE::ActiveEffect*>;
		a_actor->VisitActiveEffects([&](RE::ActiveEffect* ae) -> RE::BSContainer::ForEachResult {
			activeEffects->push_back(ae);
			return RE::BSContainer::ForEachResult::kContinue;
		});
		if (activeEffects) {
#endif
			return std::ranges::any_of(*activeEffects, [&](auto const& ae) {
				const auto mgef = ae ? ae->GetBaseObject() : nullptr;
				return mgef && RE::EffectArchetypeToString(mgef->GetArchetype()) == a_archetype;
			});
		}
		return false;
	}

	inline bool IsActorInWater(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		return a_actor->IsPointSubmergedMoreThan(a_actor->GetPosition(), a_actor->GetParentCell(), 0.01f);
	}

	inline bool IsActorUnderwater(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		return a_actor->IsPointSubmergedMoreThan(a_actor->GetPosition(), a_actor->GetParentCell(), 0.875f);
	}

	inline bool IsLimbGone(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		std::int32_t a_limbEnum)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		return a_actor->IsLimbGone(a_limbEnum);
	}

	inline bool IsQuadruped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		const auto charController = a_actor->GetCharController();
		return charController && charController->flags.all(RE::CHARACTER_FLAGS::kQuadruped);
	}

	inline bool IsSoulTrapped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		using Archetype = RE::EffectArchetypes::ArchetypeID;
		using Flags = RE::TESSoulGem::RecordFlags;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		const auto currentProcess = a_actor->currentProcess;
		const auto middleProcess = currentProcess ? currentProcess->middleHigh : nullptr;
		if (middleProcess && middleProcess->soulTrapped) {
			return true;
		}

		bool isBeingSoulTrapped = false;

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			const auto handle = a_actor->GetHandle();
			processLists->ForEachModelEffect([&](const RE::ModelReferenceEffect& a_modelEffect) {
				if (a_modelEffect.target == handle) {
					if (const auto modelArt = a_modelEffect.artObject; modelArt && modelArt->GetFormID() == SoulTrapHitArtID) {
						isBeingSoulTrapped = true;
						return RE::BSContainer::ForEachResult::kStop;
					}
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
		}

		return isBeingSoulTrapped;
	}

	inline void KillNoWait(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		a_actor->KillImmediate();
		a_actor->boolBits.set(RE::Actor::BOOL_BITS::kDead);
		a_actor->boolBits.set(RE::Actor::BOOL_BITS::kSetOnDeath);
	}

	inline void RemoveAddedSpells(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::BSFixedString a_modName,
		std::vector<RE::BGSKeyword*> a_keywords,
		bool a_matchAll)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		std::vector<RE::SpellItem*> spells;

		auto dataHandler = RE::TESDataHandler::GetSingleton();
		auto mod = dataHandler ? dataHandler->LookupModByName(a_modName) : nullptr;

		constexpr auto has_keyword = [](RE::SpellItem* a_spell, const std::vector<RE::BGSKeyword*>& a_keywordArray, bool a_matchAll) {
			if (a_matchAll) {
				return std::ranges::all_of(a_keywordArray, [&](const auto& keyword) { return keyword && a_spell->HasKeyword(keyword); });
			}
			return std::ranges::any_of(a_keywordArray, [&](const auto& keyword) { return keyword && a_spell->HasKeyword(keyword); });
		};

		for (auto& spell : a_actor->addedSpells | std::views::reverse) {
			if (!spell) {
				continue;
			}
			if (mod && !mod->IsFormInMod(spell->GetFormID())) {
				continue;
			}
			if (!a_keywords.empty() && !has_keyword(spell, a_keywords, a_matchAll)) {
				continue;
			}
			spells.push_back(spell);
		}

		//Papyrus RemoveSpell queues a task, while console command calls this directly.
		auto taskQueue = RE::TaskQueueInterface::GetSingleton();
		if (taskQueue) {
			auto actorHandle = a_actor->CreateRefHandle();
			for (const auto& spell : spells) {
				taskQueue->QueueRemoveSpell(actorHandle, spell);
			}
		}
	}

	inline void RemoveArmorOfType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		std::uint32_t a_armorType,
		std::vector<std::uint32_t> a_slotsToSkip,
		bool a_equippedOnly)
	{
		using Slot = RE::BIPED_MODEL::BipedObjectSlot;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		const auto armorType = static_cast<RE::BGSBipedObjectForm::ArmorType>(a_armorType);

		auto inv = a_actor->GetInventory([armorType, a_slotsToSkip](RE::TESBoundObject& a_object) {
			const auto armor = a_object.As<RE::TESObjectARMO>();
			if (armor && armor->GetArmorType() == armorType) {
				if (a_slotsToSkip.empty() || std::ranges::none_of(a_slotsToSkip,
												 [&](const auto& slot) {
													 return armor->HasPartOf(static_cast<Slot>(slot));
												 })) {
					return true;
				}
			}
			return false;
		});

		for (auto& [item, data] : inv) {
			const auto& [count, entry] = data;
			if (!entry->IsQuestObject() && (!a_equippedOnly || entry->IsWorn())) {
				INV::remove_item(a_actor, item, count, true, nullptr, a_stackID, a_vm);
			}
		}
	}

	inline bool RemoveBasePerk(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::BGSPerk* a_perk)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}
		if (!a_perk) {
			a_vm->TraceStack("Perk is None", a_stackID);
			return false;
		}

		return FORM::PerkManager::GetSingleton()->Remove(a_actor, a_perk);
	}

	inline bool RemoveBaseSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::SpellItem* a_spell)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}
		if (!a_spell) {
			a_vm->TraceStack("Spell is None", a_stackID);
			return false;
		}

		const auto actorbase = a_actor->GetActorBase();
		const auto actorEffects = actorbase ? actorbase->GetSpellList() : nullptr;

		if (actorEffects && actorEffects->GetIndex(a_spell)) {
#ifndef SKYRIMVR
			const auto activeEffects = a_actor->GetActiveEffectList();
			if (activeEffects) {
				for (const auto& activeEffect : *activeEffects) {
					if (activeEffect && activeEffect->spell == a_spell) {
						activeEffect->Dispel(true);
					}
				}
			}
#else
			a_actor->VisitActiveEffects([&](RE::ActiveEffect* activeEffect) -> RE::BSContainer::ForEachResult {
				if (activeEffect && activeEffect->spell == a_spell) {
					activeEffect->Dispel(true);
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
#endif

			const auto combatController = a_actor->combatController;
			if (combatController && combatController->inventory) {
				combatController->inventory->dirty = true;
			}

			a_actor->DeselectSpell(a_spell);
			actorEffects->RemoveSpell(a_spell);

			actorbase->AddChange(RE::TESNPC::ChangeFlags::kSpellList);

			return true;
		}

		return false;
	}

	inline void SetActorRefraction(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		float a_refraction)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		const auto currentProcess = a_actor->currentProcess;
		if (currentProcess) {
			a_refraction = std::clamp(a_refraction, 0.0f, 1.0f);

			const auto middleHigh = currentProcess->middleHigh;
			if (middleHigh) {
				middleHigh->scriptRefractPower = a_refraction;
			}

			const float invisibility = a_actor->GetActorValue(RE::ActorValue::kInvisibility);
			if (invisibility < 0.0f || invisibility <= 1.0f && invisibility <= 0.0f || !a_actor->IsPlayerRef()) {
				if (a_refraction <= 0.0f) {
					a_actor->SetRefraction(false, a_refraction);
					a_actor->UpdateAlpha();
				} else {
					a_actor->SetRefraction(true, a_refraction);
				}
			} else {
				a_actor->SetAlpha(1.0);

				a_refraction = 1.0f - a_refraction / 100.0f;
				a_refraction = 1.0f + (0.01f - 1.0f) * ((a_refraction - 0.0f) / (1.0f - 0.0f));

				a_actor->SetRefraction(true, a_refraction);
			}
		}
	}

	inline bool SetEquippedWeaponPoison(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::AlchemyItem* a_poison,
		bool a_leftHand)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		if (const auto xPoison = INV::get_equipped_weapon_poison_data(a_actor, a_leftHand); xPoison) {
			xPoison->poison = a_poison;

			return true;
		}

		return false;
	}

	inline bool SetEquippedWeaponPoisonCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		std::uint32_t a_count,
		bool a_leftHand)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		if (const auto xPoison = INV::get_equipped_weapon_poison_data(a_actor, a_leftHand); xPoison) {
			xPoison->count = a_count;

			return true;
		}

		return false;
	}

	inline void SetLinearVelocity(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		float a_x,
		float a_y,
		float a_z)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		if (const auto charController = a_actor->GetCharController(); charController) {
			charController->SetLinearVelocityImpl(RE::hkVector4(RE::deg_to_rad(a_x), RE::deg_to_rad(a_y), RE::deg_to_rad(a_z), 0.0f));
		}
	}

	inline void SetLocalGravityActor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		float a_value,
		bool a_disableGravityOnGround)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		if (const auto charController = a_actor->GetCharController(); charController) {
			charController->SetLinearVelocityImpl(0.0f);

			a_disableGravityOnGround ?
				charController->flags.reset(RE::CHARACTER_FLAGS::kNoGravityOnGround) :
				charController->flags.set(RE::CHARACTER_FLAGS::kNoGravityOnGround);

			charController->gravity = a_value;
		}
	}

	inline void SetSoulTrapped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		bool a_trapped)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		const auto process = a_actor->currentProcess;
		const auto middleProcess = process ? process->middleHigh : nullptr;

		if (middleProcess) {
			middleProcess->soulTrapped = a_trapped;
		}
	}

	inline void UnequipAllOfType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		std::uint32_t a_armorType,
		std::vector<std::uint32_t> a_slotsToSkip)
	{
		using Slot = RE::BIPED_MODEL::BipedObjectSlot;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		const auto armorType = static_cast<RE::BGSBipedObjectForm::ArmorType>(a_armorType);

		auto inv = a_actor->GetInventory([armorType, a_slotsToSkip](RE::TESBoundObject& a_object) {
			const auto armor = a_object.As<RE::TESObjectARMO>();
			if (armor && armor->GetArmorType() == armorType) {
				if (a_slotsToSkip.empty() || std::ranges::none_of(a_slotsToSkip,
												 [&](const auto& slot) {
													 return armor->HasPartOf(static_cast<Slot>(slot));
												 })) {
					return true;
				}
			}
			return false;
		});

		for (auto& [item, data] : inv) {
			const auto& [count, entry] = data;
			if (count > 0 && entry->IsWorn()) {
				RE::ActorEquipManager::GetSingleton()->UnequipObject(a_actor, item);
			}
		}
	}

	inline void Bind(VM& a_vm)
	{
		BIND(AddBasePerk);
		BIND(AddBaseSpell);
		BIND(AddAllEquippedItemsToArray);
		BIND(ApplyPoisonToEquippedWeapon);
		BIND(DecapitateActor);
		BIND(FreezeActor);
		BIND(GetActiveEffects);
		BIND(GetActorAlpha);
		BIND(GetActorKnockState);
		BIND(GetActorRefraction);
		BIND(GetActorSoulSize, true);
		BIND(GetActorState);
		BIND(GetActorValueModifier);
		BIND(GetCriticalStage);
		BIND(GetCombatAllies);
		BIND(GetCombatTargets);
		BIND(GetCommandedActors);
		BIND(GetCommandingActor);
		BIND(GetEquippedAmmo);
#ifdef SKYRIMVR
		a_vm.RegisterFunction("GetEquippedArmorInSlot"sv, "Actor", GetEquippedArmorInSlot);
		logger::info("Patching missing Actor.GetEquippedArmorInSlot in VR");
#endif
		BIND(GetEquippedWeaponIsPoisoned);
		BIND(GetEquippedWeaponPoison);
		BIND(GetEquippedWeaponPoisonCount);
		BIND(GetEquippedWeight);
		BIND(GetMount);
		BIND(GetLocalGravityActor);
		BIND(GetObjectUnderFeet);
		BIND(GetOffersServices);
		BIND(GetRider);
		BIND(GetRunningPackage);
		BIND(GetTimeDead);
		BIND(GetTimeOfDeath);
		BIND(GetVendorFaction);
		BIND(HasActiveMagicEffect);
		BIND(HasActiveSpell);
		BIND(IsQuadruped, true);
		BIND(HasDeferredKill);
		BIND(HasMagicEffectWithArchetype);
		BIND(IsActorInWater);
		BIND(IsActorUnderwater);
		BIND(IsLimbGone);
		BIND(IsSoulTrapped);
		BIND(KillNoWait);
		BIND(RemoveAddedSpells);
		BIND(RemoveArmorOfType);
		BIND(RemoveBasePerk);
		BIND(RemoveBaseSpell);
		BIND(SetActorRefraction);
		BIND(SetEquippedWeaponPoison);
		BIND(SetEquippedWeaponPoisonCount);
		BIND(SetLinearVelocity);
		BIND(SetLocalGravityActor);
		BIND(SetSoulTrapped);
		BIND(UnequipAllOfType);

		logger::info("Registered actor functions"sv);
	}
}
