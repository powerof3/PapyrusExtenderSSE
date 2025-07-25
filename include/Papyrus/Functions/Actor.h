#pragma once

namespace Papyrus::Actor
{
	bool                            AddBasePerk(STATIC_ARGS, RE::Actor* a_actor, RE::BGSPerk* a_perk);
	bool                            AddBaseSpell(STATIC_ARGS, RE::Actor* a_actor, RE::SpellItem* a_spell);
	std::vector<RE::TESForm*>       AddAllEquippedItemsToArray(STATIC_ARGS, RE::Actor* a_actor);
	std::vector<RE::TESForm*>       AddAllEquippedItemsBySlotToArray(STATIC_ARGS, RE::Actor* a_actor, std::vector<std::uint32_t> a_slots);
	bool                            ApplyPoisonToEquippedWeapon(STATIC_ARGS, const RE::Actor* a_actor, RE::AlchemyItem* a_poison, std::uint32_t a_count, bool a_leftHand);
	void                            DecapitateActor(STATIC_ARGS, RE::Actor* a_actor);
	bool                            DamageActorHealth(STATIC_ARGS, RE::Actor* a_actor, float a_damage, RE::Actor* a_source);
	void                            FreezeActor(STATIC_ARGS, RE::Actor* a_actor, std::uint32_t a_type, bool a_enable);
	std::vector<RE::EffectSetting*> GetActiveEffects(STATIC_ARGS, RE::Actor* a_actor, bool a_inactive);
	float                           GetActorAlpha(STATIC_ARGS, const RE::Actor* a_actor);
	std::int32_t                    GetActorKnockState(STATIC_ARGS, const RE::Actor* a_actor);
	float                           GetActorRefraction(STATIC_ARGS, const RE::Actor* a_actor);
	std::int32_t                    GetActorSoulSize(STATIC_ARGS, const RE::Actor* a_actor);
	std::int32_t                    GetActorState(STATIC_ARGS, const RE::Actor* a_actor);
	float                           GetActorValueModifier(STATIC_ARGS, const RE::Actor* a_actor, std::int32_t a_modifier, RE::BSFixedString a_actorValue);
	std::vector<RE::SpellItem*>     GetAllActorPlayableSpells(STATIC_ARGS, RE::Actor* a_actor);
	std::uint32_t                   GetCriticalStage(STATIC_ARGS, const RE::Actor* a_actor);
	std::vector<RE::Actor*>         GetCombatAllies(STATIC_ARGS, const RE::Actor* a_actor);
	std::vector<RE::Actor*>         GetCombatTargets(STATIC_ARGS, const RE::Actor* a_actor);
	std::vector<RE::Actor*>         GetCommandedActors(STATIC_ARGS, const RE::Actor* a_actor);
	RE::Actor*                      GetCommandingActor(STATIC_ARGS, const RE::Actor* a_actor);
	RE::TESAmmo*                    GetEquippedAmmo(STATIC_ARGS, const RE::Actor* a_actor);
	RE::EnchantmentItem*            GetEquippedAmmoEnchantment(STATIC_ARGS, RE::Actor* a_actor);    // SeaSparrow - New Functions
	std::vector<RE::TESQuest*>      GetAllAssociatedQuests(STATIC_ARGS, RE::Actor* a_actor);  // SeaSparrow - New Functions
#ifdef SKYRIMVR
	// Fix missing GetEquippedArmorInSlot declared in SKSEVR but that doesn't exist in VR.
	// https://www.creationkit.com/index.php?title=Actor_Script#Special_Edition_Exclusive_Functions
	RE::TESObjectARMO* GetEquippedArmorInSlot(VM* a_vm, StackID a_stackID, RE::Actor* a_actor, std::int32_t a_slot);
#endif
	bool               GetEquippedWeaponIsPoisoned(STATIC_ARGS, const RE::Actor* a_actor, bool a_leftHand);
	RE::AlchemyItem*   GetEquippedWeaponPoison(STATIC_ARGS, const RE::Actor* a_actor, bool a_leftHand);
	std::uint32_t      GetEquippedWeaponPoisonCount(STATIC_ARGS, const RE::Actor* a_actor, bool a_leftHand);
	float              GetEquippedWeight(STATIC_ARGS, RE::Actor* a_actor);
	RE::Actor*         GetMount(STATIC_ARGS, RE::Actor* a_actor);
	float              GetLocalGravityActor(STATIC_ARGS, const RE::Actor* a_actor);
	RE::TESObjectREFR* GetObjectUnderFeet(STATIC_ARGS, const RE::Actor* a_actor);
	bool               GetOffersServices(STATIC_ARGS, const RE::Actor* a_actor);
	RE::Actor*         GetRider(STATIC_ARGS, RE::Actor* a_actor);
	RE::TESPackage*    GetRunningPackage(STATIC_ARGS, const RE::Actor* a_actor);
	float              GetTimeDead(STATIC_ARGS, const RE::Actor* a_actor);
	float              GetTimeOfDeath(STATIC_ARGS, const RE::Actor* a_actor);
	RE::TESFaction*    GetVendorFaction(STATIC_ARGS, RE::Actor* a_actor);
	bool               HasActiveMagicEffect(STATIC_ARGS, RE::Actor* a_actor, const RE::EffectSetting* a_mgef);
	bool               HasActiveSpell(STATIC_ARGS, RE::Actor* a_actor, const RE::SpellItem* a_spell);
	bool               HasDeferredKill(STATIC_ARGS, const RE::Actor* a_actor);
	bool               HasMagicEffectWithArchetype(STATIC_ARGS, RE::Actor* a_actor, RE::BSFixedString a_archetype);
	bool               IsActorInWater(STATIC_ARGS, const RE::Actor* a_actor);
	bool               IsActorUnderwater(STATIC_ARGS, const RE::Actor* a_actor);
	bool               IsLimbGone(STATIC_ARGS, RE::Actor* a_actor, std::int32_t a_limbEnum);
	bool               IsPowerAttacking(STATIC_ARGS, RE::Actor* a_actor);
	bool               IsQuadruped(STATIC_ARGS, const RE::Actor* a_actor);
	bool               IsSoulTrapped(STATIC_ARGS, RE::Actor* a_actor);
	void               LaunchArrow(STATIC_ARGS, RE::Actor* a_actor, RE::TESAmmo* a_ammo, RE::TESObjectWEAP* a_weapon, RE::BSFixedString a_nodeName, std::int32_t a_source, RE::TESObjectREFR* a_target, RE::AlchemyItem* a_poison);
	void               LaunchSpell(STATIC_ARGS, RE::Actor* a_actor, RE::SpellItem* a_spell, std::uint32_t a_source);
	void               KillNoWait(STATIC_ARGS, RE::Actor* a_actor);
	void               RemoveAddedSpells(STATIC_ARGS, RE::Actor* a_actor, RE::BSFixedString a_modName, std::vector<RE::BGSKeyword*> a_keywords, bool a_matchAll);
	void               RemoveArmorOfType(STATIC_ARGS, RE::Actor* a_actor, std::uint32_t a_armorType, std::vector<std::uint32_t> a_slotsToSkip, bool a_equippedOnly);
	bool               RemoveBasePerk(STATIC_ARGS, RE::Actor* a_actor, RE::BGSPerk* a_perk);
	bool               RemoveBaseSpell(STATIC_ARGS, RE::Actor* a_actor, RE::SpellItem* a_spell);
	void               SetActorRefraction(STATIC_ARGS, RE::Actor* a_actor, float a_refraction);
	bool               SetEquippedWeaponPoison(STATIC_ARGS, RE::Actor* a_actor, RE::AlchemyItem* a_poison, bool a_leftHand);
	bool               SetEquippedWeaponPoisonCount(STATIC_ARGS, RE::Actor* a_actor, std::uint32_t a_count, bool a_leftHand);
	void               SetLinearVelocity(STATIC_ARGS, RE::Actor* a_actor, float a_x, float a_y, float a_z);
	void               SetLocalGravityActor(STATIC_ARGS, RE::Actor* a_actor, float a_value, bool a_disableGravityOnGround);
	void               SetSoulTrapped(STATIC_ARGS, RE::Actor* a_actor, bool a_trapped);
	void               UnequipAllOfType(STATIC_ARGS, RE::Actor* a_actor, std::uint32_t a_armorType, std::vector<std::uint32_t> a_slotsToSkip);

	void Bind(VM& a_vm);
}
