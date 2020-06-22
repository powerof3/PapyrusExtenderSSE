#pragma once

#include "main.h"


namespace papyrusActor
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;


	inline constexpr RE::FormID SoulTrapHitArtID = 0x000506D6;


	enum class EFFECT_TYPE : SInt8
	{
		kNone = -1,
		kCharred = 0,
		kSkeletonized = kCharred,
		kDrained = 1,
		kPoisoned = 2,
		kFrightened = kPoisoned,
		kAged = 3,
		kCharredCreature = 4,
		kFrozenCreature = 5
	};
	using EFFECT = EFFECT_TYPE;


	bool AddBasePerk(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSPerk* a_perk);

	bool AddBaseSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::SpellItem* a_spell);

	std::vector<RE::TESForm*> AddAllEquippedItemsToArray(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor);

	void BlendColorWithSkinTone(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSColorForm* a_color, SInt32 a_blendMode, bool a_autoCalc, float a_opacity);

	void DecapitateActor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor);

	void EquipArmorIfSkinVisible(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::TESObjectARMO* a_check, RE::TESObjectARMO* a_equip);

	float GetActorAlpha(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor);

	float GetActorRefraction(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor);

	SInt32 GetActorState(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor);

	std::vector<RE::EffectSetting*> GetAllActiveEffectsOnActor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, bool a_inactive);

	RE::BGSColorForm* GetHairColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor);

	RE::BGSTextureSet* GetHeadPartTextureSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, UInt32 a_type);

	RE::TESPackage* GetRunningPackage(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor);

	RE::BGSColorForm* GetSkinColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor);

	float GetTimeDead(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor);

	float GetTimeOfDeath(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor);

	bool HasActiveSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::SpellItem* a_spell);

	bool HasMagicEffectWithArchetype(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BSFixedString a_archetype);

	bool InstantKill(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor);

	bool IsActorInWater(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor);

	bool IsActorSoulTrapped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor);

	bool IsActorUnderwater(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor);

	void MixColorWithSkinTone(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSColorForm* a_color, bool a_manual, float a_percent);

	bool RemoveBasePerk(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSPerk* a_perk);

	bool RemoveBaseSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::SpellItem* a_spell);

	void RemoveEffectsNotOfType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, SInt32 a_type);

	void ReplaceArmorTextureSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::TESObjectARMO* a_armor, RE::BGSTextureSet* a_srcTXST, RE::BGSTextureSet* a_tgtTXST, SInt32 a_type);

	void ReplaceFaceTextureSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSTextureSet* a_maleTXST, RE::BGSTextureSet* a_femaleTXST, SInt32 a_type);

	void ReplaceSkinTextureSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSTextureSet* a_maleTXST, RE::BGSTextureSet* a_femaleTXST, UInt32 a_slot, SInt32 a_type);

	bool ResetActor3D(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BSFixedString a_folderName);

	void SetActorRefraction(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, float a_refraction);

	void SetHairColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSColorForm* a_color);

	void SetHeadPartAlpha(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, SInt32 a_type, float a_alpha);

	void SetHeadPartTextureSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSTextureSet* a_txst, SInt32 a_type);

	void SetLinearVelocity(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, float a_x, float a_y, float a_z);

	void SetLocalGravity(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, float a_value, bool a_disableGravityOnGround);

	void SetSkinAlpha(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, float a_alpha);

	void SetSkinColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSColorForm* a_color);

	void ToggleChildNode(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BSFixedString a_nodeName, bool a_disable);


	bool RegisterFuncs(VM* a_vm);
}
