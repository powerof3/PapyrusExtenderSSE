#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include <algorithm>
#include <map>
#include <random>
#include <set>
#include <ShlObj.h>
#include <tuple>
#include <vector>

#include "SKSE/API.h"
#include "RE/Skyrim.h"

//----------------------------------------------------------------------------------------------------

extern "C"
{
	__declspec(dllexport) float GetPluginVersion();
};

//----------------------------------------------------------------------------------------------------

namespace RE
{
	static constexpr FormID ArtSoulTrapTargetEffectsID = 0x000506D6;


	class PO3_SKSEFunctions
	{
	public:

		//--------------------------------------------------------------------------------------------
		// ACTOR
		//--------------------------------------------------------------------------------------------

		static BGSColorForm* GetHairColor(StaticFunctionTag*, Actor* a_actor);

		static void SetHairColor(StaticFunctionTag*, Actor* a_actor, BGSColorForm* a_color);

		static BGSColorForm* GetSkinColor(StaticFunctionTag*, Actor* a_actor);

		static void SetSkinColor(StaticFunctionTag*, Actor* a_actor, BGSColorForm* a_color);

		static void MixColorWithSkinTone(StaticFunctionTag*, Actor* a_actor, BGSColorForm* a_color, bool a_manual, float a_percent);

		static void SetSkinAlpha(StaticFunctionTag*, Actor* a_actor, float a_alpha);

		static void EquipArmorIfSkinVisible(StaticFunctionTag*, Actor* a_actor, TESObjectARMO* a_check, TESObjectARMO* a_equip);

		static void ReplaceArmorTextureSet(StaticFunctionTag*, Actor* a_actor, TESObjectARMO* a_armor, BGSTextureSet* a_srcTXST, BGSTextureSet* a_tgtTXST, SInt32 a_type);

		static void ReplaceSkinTextureSet(StaticFunctionTag*, Actor* a_actor, BGSTextureSet* a_maleTXST, BGSTextureSet* a_femaleTXST, UInt32 a_slot, SInt32 a_type);

		static void ReplaceFaceTextureSet(StaticFunctionTag*, Actor* a_actor, BGSTextureSet* a_maleTXST, BGSTextureSet* a_femaleTXST, SInt32 a_type);

		static BGSTextureSet* GetHeadPartTextureSet(StaticFunctionTag*, Actor* a_actor, UInt32 a_type);

		static void SetHeadPartTextureSet(StaticFunctionTag*, Actor* a_actor, BGSTextureSet* a_txst, UInt32 a_type);

		static void SetHeadPartAlpha(StaticFunctionTag*, Actor* a_actor, UInt32 a_type, float a_alpha);

		static void ToggleChildNode(StaticFunctionTag*, Actor* a_actor, BSFixedString a_nodeName, bool a_disable);

		static void RemoveChildNode(StaticFunctionTag*, Actor* a_actor, BSFixedString a_nodeName);

		static bool IsActorSoulTrapped(StaticFunctionTag*, Actor* a_actor);

		static bool ResetActor3D(StaticFunctionTag*, Actor* a_actor, BSFixedString a_folderName);

		static void RemoveEffectsNotOfType(StaticFunctionTag*, Actor* a_actor, UInt32 a_type);

		static void DecapitateActor(StaticFunctionTag*, Actor* a_actor);

		static float GetTimeDead(StaticFunctionTag*, Actor* a_actor);

		static float GetTimeOfDeath(StaticFunctionTag*, Actor* a_actor);

		static TESPackage* GetRunningPackage(StaticFunctionTag*, Actor* a_actor);

		static bool IsActorInWater(StaticFunctionTag*, Actor* a_actor);

		static float GetActorAlpha(StaticFunctionTag*, Actor* a_actor);

		static float GetActorRefraction(StaticFunctionTag*, Actor* a_actor);

		static void SetActorRefraction(StaticFunctionTag*, Actor* a_actor, float a_refraction);

		static SInt32 GetActorState(StaticFunctionTag*, Actor* a_actor);

		static bool InstantKill(StaticFunctionTag*, Actor* a_actor);

		static bool AddBasePerk(StaticFunctionTag*, Actor* a_actor, BGSPerk* a_perk);

		static bool RemoveBasePerk(StaticFunctionTag*, Actor* a_actor, BGSPerk* a_perk);

		static bool AddBaseSpell(StaticFunctionTag*, Actor* a_actor, SpellItem* a_spell);

		static bool RemoveBaseSpell(StaticFunctionTag*, Actor* a_actor, SpellItem* a_spell);

		//static void SetShaderType(StaticFunctionTag*, Actor* a_actor, TESObjectARMO* templateArmor);

		//--------------------------------------------------------------------------------------------
		// ACTORBASE
		//--------------------------------------------------------------------------------------------

		static UInt32 GetPerkCount(StaticFunctionTag*, TESNPC* a_actorBase);

		static BGSPerk* GetNthPerk(StaticFunctionTag*, TESNPC* a_actorBase, UInt32 a_index);

		//--------------------------------------------------------------------------------------------
		// ARRAY
		//--------------------------------------------------------------------------------------------

		static bool AddStringToArray(StaticFunctionTag*, BSFixedString a_string, BSScript::VMArray<BSFixedString> a_stringArray);

		static bool AddActorToArray(StaticFunctionTag*, Actor* a_actor, BSScript::VMArray<Actor*> actorArray);

		static UInt32 ArrayStringCount(StaticFunctionTag*, BSFixedString a_string, BSScript::VMArray<BSFixedString> a_stringArray);

		static BSScript::VMArray<BSFixedString> SortArrayString(StaticFunctionTag*, BSScript::VMArray<BSFixedString> a_stringArray);

		//--------------------------------------------------------------------------------------------
		// EFFECTSHADER
		//--------------------------------------------------------------------------------------------

		static float GetEffectShaderFullParticleCount(StaticFunctionTag*, TESEffectShader* a_effectShader);

		static void SetEffectShaderFullParticleCount(StaticFunctionTag*, TESEffectShader* a_effectShader, float a_particleCount);

		static float GetEffectShaderPersistentParticleCount(StaticFunctionTag*, TESEffectShader* a_effectShader);

		static void SetEffectShaderPersistentParticleCount(StaticFunctionTag*, TESEffectShader* a_effectShader, float a_particleCount);

		static bool IsEffectShaderFlagSet(StaticFunctionTag*, TESEffectShader* a_effectShader, UInt32 flag);

		static void SetEffectShaderFlag(StaticFunctionTag*, TESEffectShader* a_effectShader, UInt32 flag);

		static void ClearEffectShaderFlag(StaticFunctionTag*, TESEffectShader* a_effectShader, UInt32 flag);

		static UInt32 GetEffectShaderTotalCount(StaticFunctionTag*, TESEffectShader* a_effectShader, bool a_active);

		//--------------------------------------------------------------------------------------------
		// FORM
		//--------------------------------------------------------------------------------------------

		static void ReplaceKeywordOnForm(StaticFunctionTag*, TESForm* a_form, BGSKeyword* a_remove, BGSKeyword* a_add);

		static void AddKeywordToForm(StaticFunctionTag*, TESForm* a_form, BGSKeyword* a_add);

		static bool RemoveKeywordOnForm(StaticFunctionTag*, TESForm* a_form, BGSKeyword* a_remove);

		//--------------------------------------------------------------------------------------------
		// GAME
		//--------------------------------------------------------------------------------------------

		static bool IsPluginFound(StaticFunctionTag*, BSFixedString a_name);

		static BSScript::VMArray<TESForm*> GetAllSpellsInMod(StaticFunctionTag*, BSFixedString a_name, BSScript::VMArray<BGSKeyword*> a_keywordArray, bool a_playable);

		static BSScript::VMArray<TESForm*> GetAllRacesInMod(StaticFunctionTag*, BSFixedString a_name, BSScript::VMArray<BGSKeyword*> a_keywordArray);

		static void AddAllGameSpellsToList(StaticFunctionTag*, BGSListForm* a_list, BSScript::VMArray<BGSKeyword*> a_keywordArray, bool a_playable);

		static void AddAllGameRacesToList(StaticFunctionTag*, BGSListForm* a_list, BSScript::VMArray<BGSKeyword*> a_keywordArray);

		static BSScript::VMArray<Actor*> GetActorsByProcessingLevel(StaticFunctionTag*, UInt32 a_level);

		static SInt32 GetNumActorsInHigh(StaticFunctionTag*);

		//--------------------------------------------------------------------------------------------
		// LIGHT
		//--------------------------------------------------------------------------------------------

		static float GetLightRadius(StaticFunctionTag*, TESObjectLIGH* a_light);

		static void SetLightRadius(StaticFunctionTag*, TESObjectLIGH* a_light, float a_radius);

		static float GetLightFade(StaticFunctionTag*, TESObjectLIGH* a_light);

		static void SetLightFade(StaticFunctionTag*, TESObjectLIGH* a_light, float a_fade);

		static BGSColorForm* GetLightColor(StaticFunctionTag*, TESObjectLIGH* a_light);

		static void SetLightColor(StaticFunctionTag*, TESObjectLIGH* a_light, BGSColorForm* a_color);

		static UInt32 GetLightType(StaticFunctionTag*, TESObjectLIGH* a_light);

		static void SetLightType(StaticFunctionTag*, TESObjectLIGH* a_light, UInt32 a_type);

		static float GetLightFOV(StaticFunctionTag*, TESObjectLIGH* a_light);

		static void SetLightFOV(StaticFunctionTag*, TESObjectLIGH* a_light, float a_fov);

		static float GetLightShadowDepthBias(StaticFunctionTag*, TESObjectREFR* a_lightObject);

		static void SetLightShadowDepthBias(StaticFunctionTag*, TESObjectREFR* a_lightObject, float a_depthBias);

		//--------------------------------------------------------------------------------------------
		// LOCATION
		//--------------------------------------------------------------------------------------------

		static BGSLocation* GetParentLocation(StaticFunctionTag*, BGSLocation* a_location);

		static void SetParentLocation(StaticFunctionTag*, BGSLocation* a_location, BGSLocation* a_newLocation);

		//--------------------------------------------------------------------------------------------
		// MATHS
		//--------------------------------------------------------------------------------------------

		static float GenerateRandomFloat(StaticFunctionTag*, float a_min, float a_max);

		static UInt32 GenerateRandomInt(StaticFunctionTag*, UInt32 a_min, UInt32 a_max);

		//--------------------------------------------------------------------------------------------
		// MAGICEFFECT
		//--------------------------------------------------------------------------------------------

		static BSScript::VMArray<EffectSetting*> GetAllActiveEffectsOnActor(StaticFunctionTag*, Actor* a_actor, bool a_inactive);

		static bool HasMagicEffectWithArchetype(StaticFunctionTag*, Actor* a_actor, BSFixedString a_archetype);

		static UInt32 GetEffectArchetypeAsInt(StaticFunctionTag*, EffectSetting* a_mgef);

		static BSFixedString GetEffectArchetypeAsString(StaticFunctionTag*, EffectSetting* a_mgef);

		static BGSSoundDescriptorForm* GetMagicEffectSound(StaticFunctionTag*, EffectSetting* a_mgef, UInt32 a_type);

		static void SetMagicEffectSound(StaticFunctionTag*, EffectSetting* a_mgef, BGSSoundDescriptorForm* a_sound, UInt32 a_type);

		//--------------------------------------------------------------------------------------------
		// OBJECTREFERENCE
		//--------------------------------------------------------------------------------------------

		static BSScript::VMArray<float> GetPositionAsArray(StaticFunctionTag*, TESObjectREFR* a_ref);

		static BSScript::VMArray<float> GetRotationAsArray(StaticFunctionTag*, TESObjectREFR* a_ref);

		static bool IsLoadDoor(StaticFunctionTag*, TESObjectREFR* a_door);

		static void AddAllInventoryItemsToList(StaticFunctionTag*, TESObjectREFR* a_ref, BGSListForm* a_list, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem);

		static BSScript::VMArray<TESForm*> AddAllInventoryItemsToArray(StaticFunctionTag*, TESObjectREFR* a_ref, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem);

		static BSScript::VMArray<TESForm*> AddAllEquippedItemsToArray(StaticFunctionTag*, Actor* a_actor);

		static void ReplaceKeywordOnRef(StaticFunctionTag*, TESObjectREFR* a_ref, BGSKeyword* a_remove, BGSKeyword* a_add);

		static void AddKeywordToRef(StaticFunctionTag*, TESObjectREFR* a_ref, BGSKeyword* a_add);

		static bool RemoveKeywordFromRef(StaticFunctionTag*, TESObjectREFR* a_ref, BGSKeyword* a_remove);

		static void Apply2DHavokImpulse(StaticFunctionTag*, TESObjectREFR* a_src, TESObjectREFR* a_tgt, float a_Z, float a_magnitude);

		static void Apply3DHavokImpulse(StaticFunctionTag*, TESObjectREFR* a_src, TESObjectREFR* a_tgt, float a_magnitude);

		static void MoveToNearestNavmeshLocation(StaticFunctionTag*, TESObjectREFR* a_ref);

		static BSScript::VMArray<TESEffectShader*>GetAllEffectShaders(StaticFunctionTag*, TESObjectREFR* a_ref);

		static UInt32 HasEffectShader(StaticFunctionTag*, TESObjectREFR* a_ref, TESEffectShader* a_effectShader, bool a_active);

		static BSScript::VMArray<BGSArtObject*> GetAllArtObjects(StaticFunctionTag*, TESObjectREFR* a_ref);

		static UInt32 HasArtObject(StaticFunctionTag*, TESObjectREFR* a_ref, BGSArtObject* a_art, bool a_active);

		static void StopArtObject(StaticFunctionTag*, TESObjectREFR* a_ref, BGSArtObject* a_art);

		static void StopAllShaders(StaticFunctionTag*, TESObjectREFR* a_ref);

		static Actor* GetActorCause(StaticFunctionTag*, TESObjectREFR* a_ref);

		static Actor* GetClosestActorFromRef(StaticFunctionTag*, TESObjectREFR* a_ref, bool a_ignorePlayer);

		static Actor* GetRandomActorFromRef(StaticFunctionTag*, TESObjectREFR* a_ref, float a_radius, bool a_ignorePlayer);

		static BSScript::VMArray<TESObjectREFR*> FindAllReferencesOfType(StaticFunctionTag*, TESObjectREFR* a_ref, TESForm* a_type, float a_radius);

		static BSScript::VMArray<TESObjectREFR*> FindAllReferencesWithKeyword(StaticFunctionTag*, TESObjectREFR* a_ref, BSScript::VMArray<BGSKeyword*> a_keywordArray, float a_radius, bool a_matchAll);

		static float GetEffectShaderDuration(StaticFunctionTag*, TESObjectREFR* a_ref, TESEffectShader* a_effectShader);

		static void SetEffectShaderDuration(StaticFunctionTag*, TESObjectREFR* a_ref, TESEffectShader* a_effectShader, float a_time, bool a_absolute);

		static void SetupBodyPartGeometry(StaticFunctionTag*, TESObjectREFR* a_miscItem, Actor* a_actor);

		//--------------------------------------------------------------------------------------------
		// PACKAGE
		//--------------------------------------------------------------------------------------------

		static SInt32 GetPackageType(StaticFunctionTag*, TESPackage* a_package);

		//--------------------------------------------------------------------------------------------
		// PROJECTILE
		//--------------------------------------------------------------------------------------------

		static float GetProjectileSpeed(StaticFunctionTag*, BGSProjectile* a_projectile);

		static void SetProjectileSpeed(StaticFunctionTag*, BGSProjectile* a_projectile, float a_speed);

		static float GetProjectileRange(StaticFunctionTag*, BGSProjectile* a_projectile);

		static void SetProjectileRange(StaticFunctionTag*, BGSProjectile* a_projectile, float a_range);

		static float GetProjectileGravity(StaticFunctionTag*, BGSProjectile* a_projectile);

		static void SetProjectileGravity(StaticFunctionTag*, BGSProjectile* a_projectile, float a_gravity);

		static float GetProjectileImpactForce(StaticFunctionTag*, BGSProjectile* a_projectile);

		static void SetProjectileImpactForce(StaticFunctionTag*, BGSProjectile* a_projectile, float a_dorce);

		static UInt32 GetProjectileType(StaticFunctionTag*, BGSProjectile* a_projectile);

		//--------------------------------------------------------------------------------------------
		// SOUND
		//--------------------------------------------------------------------------------------------

		static void SetSoundDescriptor(StaticFunctionTag*, TESSound* a_sound, BGSSoundDescriptorForm* a_soundDescriptor);

		//--------------------------------------------------------------------------------------------
		// SPELL
		//--------------------------------------------------------------------------------------------

		static UInt32 GetSpellType(StaticFunctionTag*, SpellItem* a_spell);

		static bool HasActiveSpell(StaticFunctionTag*, Actor* a_actor, SpellItem* a_spell);

		//--------------------------------------------------------------------------------------------
		// VISUALEFFECT
		//--------------------------------------------------------------------------------------------

		static BGSArtObject* GetArtObject(StaticFunctionTag*, BGSReferenceEffect* a_vfx);

		static void SetArtObject(StaticFunctionTag*, BGSReferenceEffect* a_vfx, BGSArtObject* a_art);

		static UInt32 GetArtObjectTotalCount(StaticFunctionTag*, BGSReferenceEffect* a_vfx, bool a_active);

		//--------------------------------------------------------------------------------------------
		// WIND
		//--------------------------------------------------------------------------------------------

		static UInt32 GetWindSpeedAsInt(StaticFunctionTag*, TESWeather* a_weather);

		static float GetWindSpeedAsFloat(StaticFunctionTag*, TESWeather* a_weather);

		static SInt32 GetWeatherType(StaticFunctionTag*, TESWeather* a_weather);

		//---------------------------------------------------------------------------------------------

		static bool Register(BSScript::Internal::VirtualMachine* a_vm);
	};
}

