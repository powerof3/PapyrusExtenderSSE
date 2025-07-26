#include "Papyrus/Functions/Actor.h"

#include "Papyrus/Util/Inventory.h"
#include "Serialization/Manager.h"

namespace Papyrus::Actor
{
	bool AddBasePerk(STATIC_ARGS, RE::Actor* a_actor, RE::BGSPerk* a_perk)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		if (!a_perk) {
			a_vm->TraceStack("Perk is None", a_stackID);
			return false;
		}

		return Serialization::Manager::GetSingleton()->perks.Add(a_actor, a_perk);
	}

	bool AddBaseSpell(STATIC_ARGS, RE::Actor* a_actor, RE::SpellItem* a_spell)
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

	std::vector<RE::TESForm*> AddAllEquippedItemsToArray(STATIC_ARGS, RE::Actor* a_actor)
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

	std::vector<RE::TESForm*> AddAllEquippedItemsBySlotToArray(STATIC_ARGS, RE::Actor* a_actor, std::vector<std::uint32_t> a_slots)
	{
		using Slot = RE::BIPED_MODEL::BipedObjectSlot;

		std::vector<RE::TESForm*> result;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return result;
		}

		auto inv = a_actor->GetInventory([a_slots](RE::TESBoundObject& a_object) {
			const auto armor = a_object.As<RE::TESObjectARMO>();
			if (armor) {
				if (a_slots.empty() || std::ranges::any_of(a_slots,
										   [&](const auto& slot) {
											   return armor->HasPartOf(static_cast<Slot>(slot));
										   })) {
					return true;
				}
			}
			return false;
		});

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

	bool ApplyPoisonToEquippedWeapon(STATIC_ARGS, const RE::Actor* a_actor, RE::AlchemyItem* a_poison, std::uint32_t a_count, bool a_leftHand)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}
		if (!a_poison) {
			a_vm->TraceStack("Poison is None", a_stackID);
			return false;
		}

		if (const auto equippedEntryData = a_actor->GetEquippedEntryData(a_leftHand)) {
			if (equippedEntryData->IsPoisoned()) {
				return false;
			}
			equippedEntryData->PoisonObject(a_poison, a_count);
			return true;
		}

		return false;
	}

	void DecapitateActor(STATIC_ARGS, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		if (NGDecapitationsAPI::g_API) {
			NGDecapitationsAPI::g_API->Decapitate(a_actor);
		} else {
			a_actor->Decapitate();
		}
	}

	bool DamageActorHealth(STATIC_ARGS, RE::Actor* a_actor, float a_damage, RE::Actor* a_source)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		return a_actor->DoDamage(a_damage, a_source, false);
	}

	void FreezeActor(STATIC_ARGS, RE::Actor* a_actor, std::uint32_t a_type, bool a_enable)
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
			if (const auto charController = a_actor->GetCharController()) {
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
			if (const auto charController = a_actor->GetCharController()) {
				SKSE::GetTaskInterface()->AddTask([root, charController, a_actor, a_enable]() {
					RE::CFilter filterInfo;
					charController->GetCollisionFilterInfo(filterInfo);
					if (a_enable) {
						a_actor->boolBits.set(BOOL_BITS::kParalyzed);
						root->SetCollisionLayerAndGroup(RE::COL_LAYER::kDeadBip, filterInfo.filter + 1);
						root->UpdateRigidConstraints(true);
					} else {
						a_actor->boolBits.reset(BOOL_BITS::kParalyzed);
						root->UpdateRigidConstraints(false);
						root->SetCollisionLayerAndGroup(RE::COL_LAYER::kDeadBip, filterInfo.GetSystemGroup());
					}
				});
			}
		}
	}

	std::vector<RE::EffectSetting*> GetActiveEffects(STATIC_ARGS, RE::Actor* a_actor, bool a_inactive)
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

	float GetActorAlpha(STATIC_ARGS, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return 1.0f;
		}

		const auto middleHigh = a_actor->GetMiddleHighProcess();
		return middleHigh ? middleHigh->alphaMult : 1.0f;
	}

	std::int32_t GetActorKnockState(STATIC_ARGS, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return -1;
		}

		return std::to_underlying(a_actor->GetKnockState());
	}

	float GetActorRefraction(STATIC_ARGS, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return 1.0f;
		}

		const auto middleHigh = a_actor->GetMiddleHighProcess();
		return middleHigh ? middleHigh->scriptRefractPower : 1.0f;
	}

	std::int32_t GetActorSoulSize(STATIC_ARGS, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return -1;
		}

		return std::to_underlying(a_actor->GetSoulSize());
	}

	std::int32_t GetActorState(STATIC_ARGS, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return -1;
		}

		return std::to_underlying(a_actor->GetLifeState());
	}

	float GetActorValueModifier(STATIC_ARGS, const RE::Actor* a_actor, std::int32_t a_modifier, RE::BSFixedString a_actorValue)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return 0.0f;
		}

		const auto actorValueList = RE::ActorValueList::GetSingleton();
		const auto actorValue = actorValueList ?
		                            actorValueList->LookupActorValueByName(a_actorValue.c_str()) :
		                            RE::ActorValue::kNone;

		const auto modifier = static_cast<RE::ACTOR_VALUE_MODIFIER>(a_modifier);
		return actorValue != RE::ActorValue::kNone ?
		           a_actor->GetActorValueModifier(modifier, actorValue) :
		           0.0f;
	}

	std::vector<RE::SpellItem*> GetAllActorPlayableSpells(STATIC_ARGS, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return {};
		}

		class Visitor : public RE::Actor::ForEachSpellVisitor
		{
		public:
			RE::BSContainer::ForEachResult Visit(RE::SpellItem* a_spell) override
			{
				if (a_spell->GetSpellType() == RE::MagicSystem::SpellType::kSpell) {
					spells.push_back(a_spell);
				}
				return RE::BSContainer::ForEachResult::kContinue;
			}

			std::vector<RE::SpellItem*> spells;
		} visitor;

		a_actor->VisitSpells(visitor);
		return visitor.spells;
	}

	std::uint32_t GetCriticalStage(STATIC_ARGS, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return 0;
		}

		return a_actor->criticalStage.underlying();
	}

	std::vector<RE::Actor*> GetCombatAllies(STATIC_ARGS, const RE::Actor* a_actor)
	{
		std::vector<RE::Actor*> result;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return result;
		}

		if (const auto combatGroup = a_actor->GetCombatGroup()) {
			for (auto& memberData : combatGroup->members) {
				if (auto ally = memberData.memberHandle.get(); ally) {
					result.push_back(ally.get());
				}
			}
		}

		return result;
	}

	std::vector<RE::Actor*> GetCombatTargets(STATIC_ARGS, const RE::Actor* a_actor)
	{
		std::vector<RE::Actor*> result;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return result;
		}

		if (const auto combatGroup = a_actor->GetCombatGroup()) {
			for (auto& targetData : combatGroup->targets) {
				auto target = targetData.targetHandle.get();
				if (target) {
					result.push_back(target.get());
				}
			}
		}

		return result;
	}

	std::vector<RE::Actor*> GetCommandedActors(STATIC_ARGS, const RE::Actor* a_actor)
	{
		std::vector<RE::Actor*> result;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return result;
		}

		if (const auto middleHigh = a_actor->GetMiddleHighProcess()) {
			for (auto& commandedActorData : middleHigh->commandedActors) {
				const auto commandedActor = commandedActorData.commandedActor.get();
				if (commandedActor) {
					result.push_back(commandedActor.get());
				}
			}
		}

		return result;
	}

	RE::Actor* GetCommandingActor(STATIC_ARGS, const RE::Actor* a_actor)
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

	RE::TESAmmo* GetEquippedAmmo(STATIC_ARGS, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return nullptr;
		}

		return a_actor->GetCurrentAmmo();
	}

	//SeaSparrow - New Functions
	RE::EnchantmentItem* GetEquippedAmmoEnchantment(STATIC_ARGS, RE::Actor* a_actor)
	{
		RE::EnchantmentItem* result = nullptr;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return result;
		}

		const auto middleHigh = a_actor->GetMiddleHighProcess();
		const auto bothHands = middleHigh ? middleHigh->bothHands : nullptr;

		if (bothHands && bothHands->object) {
			if (const auto ammo = bothHands->object->As<RE::TESAmmo>()) {
				const auto projectile = ammo ? ammo->data.projectile : nullptr;
				const auto explosion = projectile ? projectile->data.explosionType : nullptr;

				result = explosion ? explosion->formEnchanting : nullptr;

				if (!result) {
					if (const auto& extraLists = bothHands->extraLists) {
						for (const auto& extraList : *extraLists) {
							const auto exEnch = extraList->GetByType<RE::ExtraEnchantment>();
							if (exEnch && exEnch->enchantment) {
								result = exEnch->enchantment;
								break;
							}
						}
					}
				}

				return result;
			}
		}

		return result;
	}

	std::vector<RE::TESQuest*> GetActiveAssociatedQuests(STATIC_ARGS, RE::Actor* a_actor, bool a_allowEmptyStages)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return {};
		}
#if 0
		// Keep this ready in case someone tries to query the player. SLOW SLOW SLOW.
		else if (a_actor->IsPlayerRef()) {
			a_vm->TraceStack("Player actor is not supported", a_stackID);
			return {};
		}
#endif
		if (!a_actor->extraList.HasType(RE::ExtraDataType::kAliasInstanceArray)) {
			return {};
		}

		auto* aliasInstancesBase = a_actor->extraList.GetByType(RE::ExtraDataType::kAliasInstanceArray);
		if (!aliasInstancesBase) {
			a_vm->TraceStack("Actor has no alias instances", a_stackID);
			return {};
		}

		auto* aliasInstanceArray = skyrim_cast<RE::ExtraAliasInstanceArray*>(aliasInstancesBase);
		if (!aliasInstanceArray) {
			a_vm->TraceStack("Actor has no alias instance array", a_stackID);  // this is an error.
			return {};
		}

		std::vector<RE::TESQuest*> quests{};
		quests.reserve(aliasInstanceArray->aliases.size());
		for (const auto* instance : aliasInstanceArray->aliases) {
			auto* quest = instance ? instance->quest : nullptr;
			if (!quest) {
				continue;
			}
			if (!a_allowEmptyStages) {
				auto* wating = quest->waitingStages;
				auto* executed = quest->executedStages;
				if (!wating || !executed) {
					continue;
				} else if (wating->empty() && executed->empty()) {
					continue;
				}
			}
			if (std::ranges::find(quests, quest) == quests.end()) {
				quests.push_back(quest);
			}
		}
		return quests;
	}

	std::vector<RE::TESQuest*> GetAllAssociatedQuests(STATIC_ARGS, RE::Actor* a_actor, bool a_allowEmptyStages)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return {};
		}
#if 0
		// Keep this ready in case someone tries to query the player. SLOW SLOW SLOW.
		else if (a_actor->IsPlayerRef()) {
			a_vm->TraceStack("Player actor is not supported", a_stackID);
			return {};
		}
#endif
		auto* dh = RE::TESDataHandler::GetSingleton();
		if (!dh) {
			a_vm->TraceStack("Data handler is None", a_stackID);
			return {};
		}
		auto& quests = dh->GetFormArray<RE::TESQuest>();
		if (quests.empty()) {
			a_vm->TraceStack("No quests found", a_stackID);
			return {};
		}

		const auto*                base = a_actor->GetActorBase();
		std::vector<RE::TESQuest*> result{};
		for (auto* quest : quests) {
			if (!quest) {
				continue;
			}

			if (!a_allowEmptyStages) {
				auto* wating = quest->waitingStages;
				auto* executed = quest->executedStages;
				if (!wating || !executed) {
					continue;
				} else if (wating->empty() && executed->empty()) {
					continue;
				}
			}

			auto& aliases = quest->aliases;
			if (aliases.empty()) {
				continue;
			}
			bool hasActorAlias = false;
			for (auto it = aliases.begin(); !hasActorAlias && it != aliases.end(); ++it) {
				if (!*it) {
					continue;
				}
				const auto* refAlias = (*it) ? skyrim_cast<RE::BGSRefAlias*>(*it) : nullptr;
				if (!refAlias) {
					continue;
				}

				if (base && refAlias->fillType == RE::BGSBaseAlias::FILL_TYPE::kUniqueActor) {
					if (base != refAlias->fillData.uniqueActor.uniqueActor) {
						continue;
					}
				} else if (refAlias->fillType == RE::BGSBaseAlias::FILL_TYPE::kForced) {
					if (const auto handle = refAlias->fillData.forced.forcedRef; handle) {
						const auto* ref = handle.get().get();
						if (!ref || ref != a_actor) {
							continue;
						}
					} else {
						continue;
					}
				} else {
					continue;
				}

				hasActorAlias = true;
			}
			if (hasActorAlias) {
				result.push_back(quest);
			}
		}
		return result;
	}

	// Fix missing GetEquippedArmorInSlot declared in SKSEVR but that doesn't exist in VR.
	// https://www.creationkit.com/index.php?title=Actor_Script#Special_Edition_Exclusive_Functions
	RE::TESObjectARMO* GetEquippedArmorInSlot(VM* a_vm, StackID a_stackID, RE::Actor* a_actor, std::int32_t a_slot)
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

	bool GetEquippedWeaponIsPoisoned(STATIC_ARGS, const RE::Actor* a_actor, bool a_leftHand)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		const auto equippedEntryData = a_actor->GetEquippedEntryData(a_leftHand);
		return equippedEntryData && equippedEntryData->IsPoisoned();
	}

	RE::AlchemyItem* GetEquippedWeaponPoison(STATIC_ARGS, const RE::Actor* a_actor, bool a_leftHand)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return nullptr;
		}

		const auto xPoison = INV::get_equipped_weapon_poison_data(a_actor, a_leftHand);
		return xPoison ? xPoison->poison : nullptr;
	}

	std::uint32_t GetEquippedWeaponPoisonCount(STATIC_ARGS, const RE::Actor* a_actor, bool a_leftHand)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return 0;
		}

		const auto xPoison = INV::get_equipped_weapon_poison_data(a_actor, a_leftHand);
		return xPoison ? xPoison->count : 0;
	}

	float GetEquippedWeight(STATIC_ARGS, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return 0.0f;
		}

		return a_actor->GetEquippedWeight();
	}

	RE::Actor* GetMount(STATIC_ARGS, RE::Actor* a_actor)
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

	float GetLocalGravityActor(STATIC_ARGS, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return 1.0f;
		}

		const auto charController = a_actor->GetCharController();
		return charController ? charController->gravity : 1.0f;
	}

	RE::TESObjectREFR* GetObjectUnderFeet(STATIC_ARGS, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return nullptr;
		}

		const auto charController = a_actor->GetCharController();
		const auto supportBody = charController ? charController->supportBody : nullptr;

		return supportBody ? supportBody->GetUserData() : nullptr;
	}

	bool GetOffersServices(STATIC_ARGS, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		return a_actor->CanOfferServices();
	}

	RE::Actor* GetRider(STATIC_ARGS, RE::Actor* a_actor)
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

	RE::TESPackage* GetRunningPackage(STATIC_ARGS, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return nullptr;
		}

		const auto currentProcess = a_actor->currentProcess;
		return currentProcess ? currentProcess->GetRunningPackage() : nullptr;
	}

	float GetTimeDead(STATIC_ARGS, const RE::Actor* a_actor)
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

	float GetTimeOfDeath(STATIC_ARGS, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return 0.0f;
		}

		const auto currentProcess = a_actor->currentProcess;
		const auto timeOfDeath = currentProcess ? currentProcess->deathTime : 0.0f;

		return timeOfDeath > 0.0f ? timeOfDeath / 24.0f : 0.0f;
	}

	RE::TESFaction* GetVendorFaction(STATIC_ARGS, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return nullptr;
		}

		return a_actor->GetVendorFaction();
	}

	bool HasActiveMagicEffect(STATIC_ARGS, RE::Actor* a_actor, const RE::EffectSetting* a_mgef)
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

	bool HasActiveSpell(STATIC_ARGS, RE::Actor* a_actor, const RE::SpellItem* a_spell)
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

	bool HasDeferredKill(STATIC_ARGS, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		const auto middleHigh = a_actor->GetMiddleHighProcess();
		return middleHigh && middleHigh->inDeferredKill;
	}

	bool HasMagicEffectWithArchetype(STATIC_ARGS, RE::Actor* a_actor, RE::BSFixedString a_archetype)
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

	bool IsActorInWater(STATIC_ARGS, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		return a_actor->IsPointSubmergedMoreThan(a_actor->GetPosition(), a_actor->GetParentCell(), 0.01f);
	}

	bool IsActorUnderwater(STATIC_ARGS, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		return a_actor->IsPointSubmergedMoreThan(a_actor->GetPosition(), a_actor->GetParentCell(), 0.875f);
	}

	bool IsLimbGone(STATIC_ARGS, RE::Actor* a_actor, std::int32_t a_limbEnum)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		if (DismemberingFrameworkAPI::g_API || NGDecapitationsAPI::g_API) {
			RE::BSFixedString nodeName;
			switch (a_limbEnum) {
			case RE::BGSBodyPartDefs::LIMB_ENUM::kTorso:
				nodeName = "NPC COM [COM ]";
				break;
			case RE::BGSBodyPartDefs::LIMB_ENUM::kHead:
				{
					if (NGDecapitationsAPI::g_API) {
						return NGDecapitationsAPI::g_API->IsDecapitated(a_actor);
					} else {
						nodeName = "NPC Head [Head]";
					}
				}
				break;
			case RE::BGSBodyPartDefs::LIMB_ENUM::kEye:
				nodeName = "NPCEyeBone";
				break;
			case RE::BGSBodyPartDefs::LIMB_ENUM::kFlyGrab:
				nodeName = "NPC Spine1 [Spn1]";
				break;
			default:
				break;
			}
			if (DismemberingFrameworkAPI::g_API) {
				return DismemberingFrameworkAPI::g_API->IsDismemberedNode(a_actor, nodeName);
			}
		}

		return a_actor->IsLimbGone(a_limbEnum);
	}

	bool IsPowerAttacking(STATIC_ARGS, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		return a_actor->IsPowerAttacking();
	}

	bool IsQuadruped(STATIC_ARGS, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		const auto charController = a_actor->GetCharController();
		return charController && charController->flags.all(RE::CHARACTER_FLAGS::kQuadruped);
	}

	bool IsSoulTrapped(STATIC_ARGS, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		const auto middleHigh = a_actor->GetMiddleHighProcess();
		if (middleHigh && middleHigh->soulTrapped) {
			return true;
		}

		bool isBeingSoulTrapped = false;

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			const auto handle = a_actor->GetHandle();
			processLists->ForEachModelEffect([&](const RE::ModelReferenceEffect* a_modelEffect) {
				if (a_modelEffect->target == handle) {
					if (const auto modelArt = a_modelEffect->artObject; modelArt && modelArt->GetFormID() == 0x000531AE) {
						isBeingSoulTrapped = true;
						return RE::BSContainer::ForEachResult::kStop;
					}
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
		}

		return isBeingSoulTrapped;
	}

	void LaunchArrow(STATIC_ARGS, RE::Actor* a_actor, RE::TESAmmo* a_ammo, RE::TESObjectWEAP* a_weapon, RE::BSFixedString a_nodeName, std::int32_t a_source, RE::TESObjectREFR* a_target, RE::AlchemyItem* a_poison)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		if (!a_ammo) {
			a_vm->TraceStack("Ammo is None", a_stackID);
			return;
		}

		if (!a_weapon) {
			a_vm->TraceStack("Weapon is None", a_stackID);
			return;
		}

		SKSE::GetTaskInterface()->AddTask([a_actor, a_ammo, a_weapon, a_nodeName, a_source, a_target, a_poison]() {
			RE::NiAVObject* fireNode = nullptr;
			auto            root = a_actor->GetCurrent3D();
			switch (a_source) {
			case -1:
				{
					if (!a_nodeName.empty()) {
						if (root) {
							fireNode = root->GetObjectByName(a_nodeName);
						}
					} else {
						if (const auto currentProcess = a_actor->currentProcess) {
							const auto& biped = a_actor->GetCurrentBiped();
							fireNode = a_weapon->IsCrossbow() ? currentProcess->GetMagicNode(biped) : currentProcess->GetWeaponNode(biped);
						} else {
							fireNode = a_weapon->GetFireNode(root);
						}
					}
				}
				break;
			case 0:
				fireNode = root ? root->GetObjectByName(RE::FixedStrings::GetSingleton()->npcLMagicNode) : nullptr;
				break;
			case 1:
				fireNode = root ? root->GetObjectByName(RE::FixedStrings::GetSingleton()->npcRMagicNode) : nullptr;
				break;
			case 2:
				fireNode = root ? root->GetObjectByName(RE::FixedStrings::GetSingleton()->npcHeadMagicNode) : nullptr;
				break;
			default:
				break;
			}
			RE::NiPoint3                  origin;
			RE::Projectile::ProjectileRot angles{};
			if (fireNode) {
				if (a_actor->IsPlayerRef()) {
					angles.z = a_actor->GetHeading(false);

					float tiltUpAngle;
					if (a_ammo->IsBolt()) {
						tiltUpAngle = RE::INISettingCollection::GetSingleton()->GetSetting("f1PBoltTiltUpAngle:Combat")->GetFloat();
					} else {
						tiltUpAngle = RE::INISettingCollection::GetSingleton()->GetSetting(RE::PlayerCamera::GetSingleton()->IsInFirstPerson() ? "f1PArrowTiltUpAngle:Combat" : "f3PArrowTiltUpAngle:Combat")->GetFloat();
					}
					angles.x = a_actor->GetAngleX() - (RE::deg_to_rad(tiltUpAngle));

					origin = fireNode->world.translate;
					a_actor->Unk_117(origin);
				} else {
					origin = fireNode->world.translate;
					a_actor->Unk_A0(fireNode, angles.x, angles.z, origin);
				}
			} else {
				origin = a_actor->GetPosition();
				origin.z += 96.0f;

				angles.x = a_actor->GetAimAngle();
				angles.z = a_actor->GetAimHeading();
			}
			RE::ProjectileHandle       handle{};
			RE::Projectile::LaunchData launchData(a_actor, origin, angles, a_ammo, a_weapon);

			launchData.desiredTarget = a_target;
			launchData.poison = a_poison;
			launchData.enchantItem = a_weapon->formEnchanting;

			RE::Projectile::Launch(&handle, launchData);

			RE::BGSSoundDescriptorForm* sound = nullptr;
			std::uint32_t               flags = 0;
			if (a_actor->IsPlayerRef() && a_weapon->attackSound2D) {
				sound = a_weapon->attackSound2D;
				flags = 18;
			} else {
				sound = a_weapon->attackSound;
				flags = 16;
			}
			if (sound) {
				RE::BSSoundHandle soundHandle;
				RE::BSAudioManager::GetSingleton()->BuildSoundDataFromDescriptor(soundHandle, sound, flags);
				soundHandle.SetPosition(origin);
				soundHandle.Play();
			}
		});
	}

	void LaunchSpell(STATIC_ARGS, RE::Actor* a_actor, RE::SpellItem* a_spell, std::uint32_t a_source)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		if (!a_spell) {
			a_vm->TraceStack("Spell is None", a_stackID);
			return;
		}

		SKSE::GetTaskInterface()->AddTask([a_actor, a_spell, a_source]() {
			RE::ProjectileHandle handle{};
			RE::Projectile::LaunchSpell(&handle, a_actor, a_spell, static_cast<RE::MagicSystem::CastingSource>(a_source));
		});
	}

	void KillNoWait(STATIC_ARGS, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		a_actor->AddDeathItems();
		a_actor->KillImmediate();

		a_actor->boolBits.set(RE::Actor::BOOL_BITS::kDead);
		a_actor->boolBits.set(RE::Actor::BOOL_BITS::kSetOnDeath);
	}

	void RemoveAddedSpells(STATIC_ARGS, RE::Actor* a_actor, RE::BSFixedString a_modName, std::vector<RE::BGSKeyword*> a_keywords, bool a_matchAll)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		std::vector<RE::SpellItem*> spells{};

		constexpr auto has_keyword = [](RE::SpellItem* a_spell, const std::vector<RE::BGSKeyword*>& a_keywordArray, bool a_matchAll) {
			if (a_matchAll) {
				return std::ranges::all_of(a_keywordArray, [&](const auto& keyword) { return keyword && a_spell->HasKeyword(keyword); });
			}
			return std::ranges::any_of(a_keywordArray, [&](const auto& keyword) { return keyword && a_spell->HasKeyword(keyword); });
		};

		const auto modInfo = RE::TESDataHandler::GetSingleton()->LookupModByName(a_modName);

		for (auto& spell : a_actor->addedSpells | std::views::reverse) {
			if (!spell) {
				continue;
			}
			if (modInfo && !modInfo->IsFormInMod(spell->GetFormID())) {
				continue;
			}
			if (!a_keywords.empty() && !has_keyword(spell, a_keywords, a_matchAll)) {
				continue;
			}
			spells.push_back(spell);
		}

		//Papyrus RemoveSpell queues a task, while console command calls this directly.
		if (const auto taskQueue = RE::TaskQueueInterface::GetSingleton()) {
			auto actorHandle = a_actor->CreateRefHandle();
			for (const auto& spell : spells) {
				taskQueue->QueueRemoveSpell(actorHandle, spell);
			}
		}
	}

	void RemoveArmorOfType(STATIC_ARGS, RE::Actor* a_actor, std::uint32_t a_armorType, std::vector<std::uint32_t> a_slotsToSkip, bool a_equippedOnly)
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

	bool RemoveBasePerk(STATIC_ARGS, RE::Actor* a_actor, RE::BGSPerk* a_perk)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}
		if (!a_perk) {
			a_vm->TraceStack("Perk is None", a_stackID);
			return false;
		}

		return Serialization::Manager::GetSingleton()->perks.Remove(a_actor, a_perk);
	}

	bool RemoveBaseSpell(STATIC_ARGS, RE::Actor* a_actor, RE::SpellItem* a_spell)
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
			if (const auto activeEffects = a_actor->GetActiveEffectList()) {
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

	void SetActorRefraction(STATIC_ARGS, RE::Actor* a_actor, float a_refraction)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		if (const auto currentProcess = a_actor->currentProcess) {
			a_refraction = std::clamp(a_refraction, 0.0f, 1.0f);

			if (const auto middleHigh = currentProcess->middleHigh) {
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

	bool SetEquippedWeaponPoison(STATIC_ARGS, RE::Actor* a_actor, RE::AlchemyItem* a_poison, bool a_leftHand)
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

	bool SetEquippedWeaponPoisonCount(STATIC_ARGS, RE::Actor* a_actor, std::uint32_t a_count, bool a_leftHand)
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

	void SetLinearVelocity(STATIC_ARGS, RE::Actor* a_actor, float a_x, float a_y, float a_z)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		if (const auto charController = a_actor->GetCharController()) {
			charController->SetLinearVelocityImpl(RE::hkVector4(RE::deg_to_rad(a_x), RE::deg_to_rad(a_y), RE::deg_to_rad(a_z), 0.0f));
		}
	}

	void SetLocalGravityActor(STATIC_ARGS, RE::Actor* a_actor, float a_value, bool a_disableGravityOnGround)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		if (const auto charController = a_actor->GetCharController()) {
			charController->SetLinearVelocityImpl(0.0f);

			a_disableGravityOnGround ?
				charController->flags.reset(RE::CHARACTER_FLAGS::kNoGravityOnGround) :
				charController->flags.set(RE::CHARACTER_FLAGS::kNoGravityOnGround);

			charController->gravity = a_value;
		}
	}

	void SetSoulTrapped(STATIC_ARGS, RE::Actor* a_actor, bool a_trapped)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		if (const auto middleHigh = a_actor->GetMiddleHighProcess()) {
			middleHigh->soulTrapped = a_trapped;
		}
	}

	void UnequipAllOfType(STATIC_ARGS, RE::Actor* a_actor, std::uint32_t a_armorType, std::vector<std::uint32_t> a_slotsToSkip)
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

		for (const auto& [item, data] : inv) {
			const auto& [count, entry] = data;
			if (count > 0 && entry->IsWorn()) {
				RE::ActorEquipManager::GetSingleton()->UnequipObject(a_actor, item);
			}
		}
	}

	void Bind(VM& a_vm)
	{
		BIND(AddBasePerk);
		BIND(AddBaseSpell);
		BIND(AddAllEquippedItemsToArray);
		BIND(AddAllEquippedItemsBySlotToArray);
		BIND(ApplyPoisonToEquippedWeapon);
		BIND(DecapitateActor);
		BIND(DamageActorHealth);
		BIND(FreezeActor);
		BIND(GetActiveEffects);
		BIND(GetActorAlpha);
		BIND(GetActorKnockState);
		BIND(GetActorRefraction);
		BIND(GetActorSoulSize, true);
		BIND(GetActorState);
		BIND(GetActorValueModifier);
		BIND(GetAllActorPlayableSpells);
		BIND(GetCriticalStage);
		BIND(GetCombatAllies);
		BIND(GetCombatTargets);
		BIND(GetCommandedActors);
		BIND(GetCommandingActor);
		BIND(GetEquippedAmmo);
		//SeaSparrow - New Binds
		BIND(GetEquippedAmmoEnchantment);
		BIND(GetActiveAssociatedQuests);
		BIND(GetAllAssociatedQuests);
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
		BIND(IsPowerAttacking);
		BIND(HasDeferredKill);
		BIND(HasMagicEffectWithArchetype);
		BIND(IsActorInWater);
		BIND(IsActorUnderwater);
		BIND(IsLimbGone);
		BIND(IsSoulTrapped);
		BIND(KillNoWait);
		BIND(LaunchArrow);
		BIND(LaunchSpell);
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
