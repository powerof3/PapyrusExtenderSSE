#pragma once

#include "skse64_common/skse_version.h"

#include "SKSE/API.h"
#include "RE/Skyrim.h"

#include <algorithm>
#include <random>
#include <set>  
#include <vector>  
#include <cmath>
#include <ShlObj.h>

#include "po3_events.h"

//---------------------------------------------------------------------------------------------------------------------

static const UInt32 KeywordActorTypeNPCID = 0x00013794;
static const UInt32 ArtSoulTrapTargetEffectsID = 0x000506D6;

//---------------------------------------------------------------------------------------------------------------------

class PO3_SKSEFunctions
{
public:

	//--------------------------------------------------------------------------------------------
	// ACTOR
	//--------------------------------------------------------------------------------------------

	static void GetHairColor(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::BGSColorForm* color);

	static void SetHairColor(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::BGSColorForm* color);

	static void GetSkinColor(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::BGSColorForm* color);

	static void SetSkinColor(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::BGSColorForm* color);

	static void MixColorWithSkinTone(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::BGSColorForm* color, bool manualMode, float percentage);

	static void SetSkinAlpha(RE::StaticFunctionTag*, RE::Actor* thisActor, float alpha);

	static void EquipArmorIfSkinVisible(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::TESObjectARMO* armorToCheck, RE::TESObjectARMO* armorToEquip);

	static void ReplaceArmorTextureSet(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::TESObjectARMO* thisArmor, RE::BGSTextureSet* sourceTXST, RE::BGSTextureSet* targetTXST, SInt32 textureType);

	static void ReplaceSkinTextureSet(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::BGSTextureSet* maleTXST, RE::BGSTextureSet* femaleTXST, UInt32 slotMask, SInt32 textureType);

	static void ReplaceFaceTextureSet(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::BGSTextureSet* maleTXST, RE::BGSTextureSet* femaleTXST, SInt32 textureType);

	static RE::BGSTextureSet* GetHeadPartTextureSet(RE::StaticFunctionTag*, RE::Actor* thisActor, UInt32 type);

	static void SetHeadPartTextureSet(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::BGSTextureSet* headpartTXST, UInt32 type);

	static void SetHeadPartAlpha(RE::StaticFunctionTag*, RE::Actor* thisActor, UInt32 partType, float alpha);

	static void ToggleChildNode(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::BSFixedString nodeName, bool disable);

	static void RemoveChildNode(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::BSFixedString nodeName);

	static bool IsActorSoulTrapped(RE::StaticFunctionTag*, RE::Actor* thisActor);

	static bool ResetActor3D(RE::StaticFunctionTag*, RE::Actor* thisActor);

	static void DecapitateActor(RE::StaticFunctionTag*, RE::Actor* thisActor);

	static float GetTimeDead(RE::StaticFunctionTag*, RE::Actor* thisActor);

	static float GetTimeOfDeath(RE::StaticFunctionTag*, RE::Actor* thisActor);

	static RE::TESPackage* GetRunningPackage(RE::StaticFunctionTag*, RE::Actor* thisActor);

	static bool IsActorInWater(RE::StaticFunctionTag*, RE::Actor* thisActor);

	static float GetActorAlpha(RE::StaticFunctionTag*, RE::Actor* thisActor);

	static float GetActorRefraction(RE::StaticFunctionTag*, RE::Actor* thisActor);

	static void SetActorRefraction(RE::StaticFunctionTag*, RE::Actor* thisActor, float refraction);

	static SInt32 GetActorState(RE::StaticFunctionTag*, RE::Actor* thisActor);

	static bool InstantKill(RE::StaticFunctionTag*, RE::Actor* thisActor);

	//static void SetShaderType(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::TESObjectARMO* templateArmor);

	//--------------------------------------------------------------------------------------------
	// ARRAY
	//--------------------------------------------------------------------------------------------

	static bool AddStringToArray(RE::StaticFunctionTag*, RE::BSFixedString thisString, RE::BSScript::VMArray<RE::BSFixedString> stringArray);

	static bool AddActorToArray(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::BSScript::VMArray<RE::Actor*> actorArray);

	static UInt32 ArrayStringCount(RE::StaticFunctionTag*, RE::BSFixedString thisString, RE::BSScript::VMArray<RE::BSFixedString> stringArray);

	static RE::BSScript::VMArray<RE::BSFixedString> SortArrayString(RE::StaticFunctionTag*, RE::BSScript::VMArray<RE::BSFixedString> stringArray);

	//--------------------------------------------------------------------------------------------
	// EFFECTSHADER
	//--------------------------------------------------------------------------------------------

	static float GetEffectShaderFullParticleCount(RE::StaticFunctionTag*, RE::TESEffectShader* thisEffectShader);

	static void SetEffectShaderFullParticleCount(RE::StaticFunctionTag*, RE::TESEffectShader* thisEffectShader, float particleCount);

	static float GetEffectShaderPersistentParticleCount(RE::StaticFunctionTag*, RE::TESEffectShader* thisEffectShader);

	static void SetEffectShaderPersistentParticleCount(RE::StaticFunctionTag*, RE::TESEffectShader* thisEffectShader, float particleCount);

	static bool IsEffectShaderFlagSet(RE::StaticFunctionTag*, RE::TESEffectShader* thisEffectShader, UInt32 flag);

	static void SetEffectShaderFlag(RE::StaticFunctionTag*, RE::TESEffectShader* thisEffectShader, UInt32 flag);

	static void ClearEffectShaderFlag(RE::StaticFunctionTag*, RE::TESEffectShader* thisEffectShader, UInt32 flag);

	//--------------------------------------------------------------------------------------------
	// FORM
	//--------------------------------------------------------------------------------------------

	static void ReplaceKeywordOnForm(RE::StaticFunctionTag*, RE::TESForm* thisForm, RE::BGSKeyword* KYWDtoRemove, RE::BGSKeyword* KYWDtoAdd);

	static void AddKeywordToForm(RE::StaticFunctionTag*, RE::TESForm* thisForm, RE::BGSKeyword* KYWDtoAdd);

	//--------------------------------------------------------------------------------------------
	// GAME
	//--------------------------------------------------------------------------------------------

	static bool IsPluginFound(RE::StaticFunctionTag*, RE::BSFixedString name);

	static RE::BSScript::VMArray<RE::TESForm*> GetAllSpellsInMod(RE::StaticFunctionTag*, RE::BSFixedString modName, RE::BSScript::VMArray<RE::BGSKeyword*> keywords, bool isPlayable);

	static RE::BSScript::VMArray<RE::TESForm*> GetAllRacesInMod(RE::StaticFunctionTag*, RE::BSFixedString modName, RE::BSScript::VMArray<RE::BGSKeyword*> keywords);

	static void AddAllGameSpellsToList(RE::StaticFunctionTag*, RE::BGSListForm* thisList, RE::BSScript::VMArray<RE::BGSKeyword*> keywords, bool isPlayable);

	static void AddAllGameRacesToList(RE::StaticFunctionTag*, RE::BGSListForm* thisList, RE::BSScript::VMArray<RE::BGSKeyword*> keywords);

	static RE::BSScript::VMArray<RE::Actor*> GetActorsByProcessingLevel(RE::StaticFunctionTag*, UInt32 level);

	static SInt32 GetNumActorsInHigh(RE::StaticFunctionTag*);

	//--------------------------------------------------------------------------------------------
	// LIGHT
	//--------------------------------------------------------------------------------------------

	static float GetLightRadius(RE::StaticFunctionTag*, RE::TESObjectLIGH* thisLight);

	static void SetLightRadius(RE::StaticFunctionTag*, RE::TESObjectLIGH* thisLight, float radius);

	static float GetLightFade(RE::StaticFunctionTag*, RE::TESObjectLIGH* thisLight);

	static void SetLightFade(RE::StaticFunctionTag*, RE::TESObjectLIGH* thisLight, float fadeValue);

	static RE::BGSColorForm* GetLightColor(RE::StaticFunctionTag*, RE::TESObjectLIGH* thisLight);

	static void SetLightColor(RE::StaticFunctionTag*, RE::TESObjectLIGH* thisLight, RE::BGSColorForm* colorform);

	static UInt32 GetLightTypeInternal(RE::TESObjectLIGH* thisLight);

	static UInt32 GetLightType(RE::StaticFunctionTag*, RE::TESObjectLIGH* thisLight);

	static void SetLightType(RE::StaticFunctionTag*, RE::TESObjectLIGH* thisLight, UInt32 lightType);

	static float GetLightFOV(RE::StaticFunctionTag*, RE::TESObjectLIGH* thisLight);

	static void SetLightFOV(RE::StaticFunctionTag*, RE::TESObjectLIGH* thisLight, float FOV);

	static float GetLightShadowDepthBias(RE::StaticFunctionTag*, RE::TESObjectREFR* thisLightObject);

	static void SetLightShadowDepthBias(RE::StaticFunctionTag*, RE::TESObjectREFR* thisLightObject, float depthBias);

	//--------------------------------------------------------------------------------------------
	// LOCATION
	//--------------------------------------------------------------------------------------------

	static RE::BGSLocation* GetParentLocation(RE::StaticFunctionTag*, RE::BGSLocation* thisLocation);

	static void SetParentLocation(RE::StaticFunctionTag*, RE::BGSLocation* thisLocation, RE::BGSLocation* newLocation);

	//--------------------------------------------------------------------------------------------
	// MATHS
	//--------------------------------------------------------------------------------------------

	static float GenerateRandomFloat(RE::StaticFunctionTag*, float afMin, float afMax);

	static UInt32 GenerateRandomInt(RE::StaticFunctionTag*, UInt32 afMin, UInt32 afMax);

	//--------------------------------------------------------------------------------------------
	// MAGICEFFECT
	//--------------------------------------------------------------------------------------------

	static RE::BSScript::VMArray<RE::EffectSetting*> GetAllActiveEffectsOnActor(RE::StaticFunctionTag*, RE::Actor* thisActor, bool showInactive);

	static RE::BSFixedString GetEffectArchetypeInternal(RE::EffectSetting* mgef);

	static bool HasMagicEffectWithArchetype(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::BSFixedString archetype);

	static UInt32 GetEffectArchetypeAsInt(RE::StaticFunctionTag*, RE::EffectSetting* mgef);

	static RE::BSFixedString GetEffectArchetypeAsString(RE::StaticFunctionTag*, RE::EffectSetting* mgef);

	static RE::BGSSoundDescriptorForm* GetMagicEffectSound(RE::StaticFunctionTag*, RE::EffectSetting* mgef, UInt32 type);

	static void SetMagicEffectSound(RE::StaticFunctionTag*, RE::EffectSetting* mgef, RE::BGSSoundDescriptorForm* mgefSound, UInt32 type);

	//--------------------------------------------------------------------------------------------
	// OBJECTREFERENCE
	//--------------------------------------------------------------------------------------------

	static RE::BSScript::VMArray<float> GetPositionAsArray(RE::StaticFunctionTag*, RE::TESObjectREFR* ref);

	static RE::BSScript::VMArray<float> GetRotationAsArray(RE::StaticFunctionTag*, RE::TESObjectREFR* ref);

	static bool IsLoadDoor(RE::StaticFunctionTag*, RE::TESObjectREFR* thisDoor);

	static bool CanItemBeTaken(RE::InventoryEntryData* data, bool noEquipped, bool noFavourited, bool noQuestItem);

	static void AddAllInventoryItemsToList(RE::StaticFunctionTag*, RE::TESObjectREFR * thisRef, RE::BGSListForm* thisList, bool noEquipped, bool noFavourited, bool noQuestItem);

	static RE::BSScript::VMArray<RE::TESForm*> AddAllInventoryItemsToArray(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef, bool noEquipped, bool noFavourited, bool noQuestItem);

	static RE::BSScript::VMArray<RE::TESForm*> AddAllEquippedItemsToArray(RE::StaticFunctionTag*, RE::Actor* thisActor);

	static void ReplaceKeywordOnRef(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef, RE::BGSKeyword* KYWDtoRemove, RE::BGSKeyword* KYWDtoAdd);

	static void AddKeywordToRef(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef, RE::BGSKeyword* KYWDtoAdd);

	static void Apply2DHavokImpulse(RE::StaticFunctionTag*, RE::TESObjectREFR* source, RE::TESObjectREFR* target, float afZ, float magnitude);

	static void Apply3DHavokImpulse(RE::StaticFunctionTag*, RE::TESObjectREFR* source, RE::TESObjectREFR* target, float magnitude);

	static void MoveToNearestNavmeshLocation(RE::StaticFunctionTag*, RE::TESObjectREFR* target);

	static RE::BSScript::VMArray<RE::TESEffectShader*>GetAllEffectShaders(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef);

	static UInt32 HasEffectShader(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef, RE::TESEffectShader* effectShader);

	static RE::BSScript::VMArray<RE::BGSArtObject*> GetAllArtObjects(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef);

	static UInt32 HasArtObject(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef, RE::BGSArtObject* artObject);

	static void StopArtObject(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef, RE::BGSArtObject* artObject);

	static void PO3_SKSEFunctions::StopAllShaders(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef);

	static RE::Actor* GetActorCause(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef);

	static RE::Actor* GetClosestActorFromRef(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef, float radius);

	static RE::Actor* GetRandomActorFromRef(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef, float radius);

	//--------------------------------------------------------------------------------------------
	// PACKAGE
	//--------------------------------------------------------------------------------------------

	static SInt32 GetPackageType(RE::StaticFunctionTag*, RE::TESPackage* package);

	//--------------------------------------------------------------------------------------------
	// PROJECTILE
	//--------------------------------------------------------------------------------------------

	static float GetProjectileSpeed(RE::StaticFunctionTag*, RE::BGSProjectile* thisProjectile);

	static void SetProjectileSpeed(RE::StaticFunctionTag*, RE::BGSProjectile* thisProjectile, float speed);

	static float GetProjectileRange(RE::StaticFunctionTag*, RE::BGSProjectile* thisProjectile);

	static void SetProjectileRange(RE::StaticFunctionTag*, RE::BGSProjectile* thisProjectile, float range);

	static float GetProjectileGravity(RE::StaticFunctionTag*, RE::BGSProjectile* thisProjectile);

	static void SetProjectileGravity(RE::StaticFunctionTag*, RE::BGSProjectile* thisProjectile, float gravity);

	static float GetProjectileImpactForce(RE::StaticFunctionTag*, RE::BGSProjectile* thisProjectile);

	static void SetProjectileImpactForce(RE::StaticFunctionTag*, RE::BGSProjectile* thisProjectile, float impactForce);

	static UInt32 GetProjectileType(RE::StaticFunctionTag*, RE::BGSProjectile* thisProjectile);

	//--------------------------------------------------------------------------------------------
	// SOUND
	//--------------------------------------------------------------------------------------------

	static void SetSoundDescriptor(RE::StaticFunctionTag*, RE::TESSound* thisSound, RE::BGSSoundDescriptorForm* thisSoundDescriptor);

	//--------------------------------------------------------------------------------------------
	// SPELL
	//--------------------------------------------------------------------------------------------
	
	static UInt32 GetSpellType(RE::StaticFunctionTag*, RE::SpellItem* thisSpell);

	//--------------------------------------------------------------------------------------------
	// VISUALEFFECT
	//--------------------------------------------------------------------------------------------

	static RE::BGSArtObject* GetArtObject(RE::StaticFunctionTag*, RE::BGSReferenceEffect* visualEffect);

	static void SetArtObject(RE::StaticFunctionTag*, RE::BGSReferenceEffect* visualEffect, RE::BGSArtObject* art);

	//--------------------------------------------------------------------------------------------
	// WIND
	//--------------------------------------------------------------------------------------------

	static UInt32 GetWindSpeedAsInt(RE::StaticFunctionTag*, RE::TESWeather* thisWeather);

	static float GetWindSpeedAsFloat(RE::StaticFunctionTag*, RE::TESWeather* thisWeather);

	static SInt32 GetWeatherType(RE::StaticFunctionTag*, RE::TESWeather* thisWeather);

	//---------------------------------------------------------------------------------------------

	static bool Register(RE::BSScript::Internal::VirtualMachine* a_vm);
};

