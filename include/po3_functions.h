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
#include <tuple>

#include "po3_events.h"

//--------------------------------------------------------------------------------------------------------------------

extern "C"
{
	__declspec(dllexport) float GetPluginVersion();
};

//---------------------------------------------------------------------------------------------------------------------

static const UInt32 KeywordActorTypeNPCID = 0x00013794;
static const UInt32 ArtSoulTrapTargetEffectsID = 0x000506D6;

//---------------------------------------------------------------------------------------------------------------------

namespace RE
{
	class PO3_SKSEFunctions
	{
	public:

		//--------------------------------------------------------------------------------------------
		// ACTOR
		//--------------------------------------------------------------------------------------------

		static void GetHairColor(StaticFunctionTag*, Actor* thisActor, BGSColorForm* color);

		static void SetHairColor(StaticFunctionTag*, Actor* thisActor, BGSColorForm* color);

		static void GetSkinColor(StaticFunctionTag*, Actor* thisActor, BGSColorForm* color);

		static void SetSkinColor(StaticFunctionTag*, Actor* thisActor, BGSColorForm* color);

		static void MixColorWithSkinTone(StaticFunctionTag*, Actor* thisActor, BGSColorForm* color, bool manualMode, float percentage);

		static void SetSkinAlpha(StaticFunctionTag*, Actor* thisActor, float alpha);

		static void EquipArmorIfSkinVisible(StaticFunctionTag*, Actor* thisActor, TESObjectARMO* armorToCheck, TESObjectARMO* armorToEquip);

		static void ReplaceArmorTextureSet(StaticFunctionTag*, Actor* thisActor, TESObjectARMO* thisArmor, BGSTextureSet* sourceTXST, BGSTextureSet* targetTXST, SInt32 textureType);

		static void ReplaceSkinTextureSet(StaticFunctionTag*, Actor* thisActor, BGSTextureSet* maleTXST, BGSTextureSet* femaleTXST, UInt32 slotMask, SInt32 textureType);

		static void ReplaceFaceTextureSet(StaticFunctionTag*, Actor* thisActor, BGSTextureSet* maleTXST, BGSTextureSet* femaleTXST, SInt32 textureType);

		static BGSTextureSet* GetHeadPartTextureSet(StaticFunctionTag*, Actor* thisActor, UInt32 type);

		static void SetHeadPartTextureSet(StaticFunctionTag*, Actor* thisActor, BGSTextureSet* headpartTXST, UInt32 type);

		static void SetHeadPartAlpha(StaticFunctionTag*, Actor* thisActor, UInt32 partType, float alpha);

		static void ToggleChildNode(StaticFunctionTag*, Actor* thisActor, BSFixedString nodeName, bool disable);

		static void RemoveChildNode(StaticFunctionTag*, Actor* thisActor, BSFixedString nodeName);

		static bool IsActorSoulTrapped(StaticFunctionTag*, Actor* thisActor);

		static bool ResetActor3D(StaticFunctionTag*, Actor* thisActor);

		static void DecapitateActor(StaticFunctionTag*, Actor* thisActor);

		static float GetTimeDead(StaticFunctionTag*, Actor* thisActor);

		static float GetTimeOfDeath(StaticFunctionTag*, Actor* thisActor);

		static TESPackage* GetRunningPackage(StaticFunctionTag*, Actor* thisActor);

		static bool IsActorInWater(StaticFunctionTag*, Actor* thisActor);

		static float GetActorAlpha(StaticFunctionTag*, Actor* thisActor);

		static float GetActorRefraction(StaticFunctionTag*, Actor* thisActor);

		static void SetActorRefraction(StaticFunctionTag*, Actor* thisActor, float refraction);

		static SInt32 GetActorState(StaticFunctionTag*, Actor* thisActor);

		static bool InstantKill(StaticFunctionTag*, Actor* thisActor);

		//static void SetShaderType(StaticFunctionTag*, Actor* thisActor, TESObjectARMO* templateArmor);

		//--------------------------------------------------------------------------------------------
		// ARRAY
		//--------------------------------------------------------------------------------------------

		static bool AddStringToArray(StaticFunctionTag*, BSFixedString thisString, BSScript::VMArray<BSFixedString> stringArray);

		static bool AddActorToArray(StaticFunctionTag*, Actor* thisActor, BSScript::VMArray<Actor*> actorArray);

		static UInt32 ArrayStringCount(StaticFunctionTag*, BSFixedString thisString, BSScript::VMArray<BSFixedString> stringArray);

		static BSScript::VMArray<BSFixedString> SortArrayString(StaticFunctionTag*, BSScript::VMArray<BSFixedString> stringArray);

		//--------------------------------------------------------------------------------------------
		// EFFECTSHADER
		//--------------------------------------------------------------------------------------------

		static float GetEffectShaderFullParticleCount(StaticFunctionTag*, TESEffectShader* thisEffectShader);

		static void SetEffectShaderFullParticleCount(StaticFunctionTag*, TESEffectShader* thisEffectShader, float particleCount);

		static float GetEffectShaderPersistentParticleCount(StaticFunctionTag*, TESEffectShader* thisEffectShader);

		static void SetEffectShaderPersistentParticleCount(StaticFunctionTag*, TESEffectShader* thisEffectShader, float particleCount);

		static bool IsEffectShaderFlagSet(StaticFunctionTag*, TESEffectShader* thisEffectShader, UInt32 flag);

		static void SetEffectShaderFlag(StaticFunctionTag*, TESEffectShader* thisEffectShader, UInt32 flag);

		static void ClearEffectShaderFlag(StaticFunctionTag*, TESEffectShader* thisEffectShader, UInt32 flag);

		//--------------------------------------------------------------------------------------------
		// FORM
		//--------------------------------------------------------------------------------------------

		static void ReplaceKeywordOnForm(StaticFunctionTag*, TESForm* thisForm, BGSKeyword* KYWDtoRemove, BGSKeyword* KYWDtoAdd);

		static void AddKeywordToForm(StaticFunctionTag*, TESForm* thisForm, BGSKeyword* KYWDtoAdd);

		//--------------------------------------------------------------------------------------------
		// GAME
		//--------------------------------------------------------------------------------------------

		static bool IsPluginFound(StaticFunctionTag*, BSFixedString name);

		static BSScript::VMArray<TESForm*> GetAllSpellsInMod(StaticFunctionTag*, BSFixedString modName, BSScript::VMArray<BGSKeyword*> keywords, bool isPlayable);

		static BSScript::VMArray<TESForm*> GetAllRacesInMod(StaticFunctionTag*, BSFixedString modName, BSScript::VMArray<BGSKeyword*> keywords);

		static void AddAllGameSpellsToList(StaticFunctionTag*, BGSListForm* thisList, BSScript::VMArray<BGSKeyword*> keywords, bool isPlayable);

		static void AddAllGameRacesToList(StaticFunctionTag*, BGSListForm* thisList, BSScript::VMArray<BGSKeyword*> keywords);

		static BSScript::VMArray<Actor*> GetActorsByProcessingLevel(StaticFunctionTag*, UInt32 level);

		static SInt32 GetNumActorsInHigh(StaticFunctionTag*);

		//--------------------------------------------------------------------------------------------
		// LIGHT
		//--------------------------------------------------------------------------------------------

		static float GetLightRadius(StaticFunctionTag*, TESObjectLIGH* thisLight);

		static void SetLightRadius(StaticFunctionTag*, TESObjectLIGH* thisLight, float radius);

		static float GetLightFade(StaticFunctionTag*, TESObjectLIGH* thisLight);

		static void SetLightFade(StaticFunctionTag*, TESObjectLIGH* thisLight, float fadeValue);

		static BGSColorForm* GetLightColor(StaticFunctionTag*, TESObjectLIGH* thisLight);

		static void SetLightColor(StaticFunctionTag*, TESObjectLIGH* thisLight, BGSColorForm* colorform);

		static UInt32 GetLightTypeInternal(TESObjectLIGH* thisLight);

		static UInt32 GetLightType(StaticFunctionTag*, TESObjectLIGH* thisLight);

		static void SetLightType(StaticFunctionTag*, TESObjectLIGH* thisLight, UInt32 lightType);

		static float GetLightFOV(StaticFunctionTag*, TESObjectLIGH* thisLight);

		static void SetLightFOV(StaticFunctionTag*, TESObjectLIGH* thisLight, float FOV);

		static float GetLightShadowDepthBias(StaticFunctionTag*, TESObjectREFR* thisLightObject);

		static void SetLightShadowDepthBias(StaticFunctionTag*, TESObjectREFR* thisLightObject, float depthBias);

		//--------------------------------------------------------------------------------------------
		// LOCATION
		//--------------------------------------------------------------------------------------------

		static BGSLocation* GetParentLocation(StaticFunctionTag*, BGSLocation* thisLocation);

		static void SetParentLocation(StaticFunctionTag*, BGSLocation* thisLocation, BGSLocation* newLocation);

		//--------------------------------------------------------------------------------------------
		// MATHS
		//--------------------------------------------------------------------------------------------

		static float GenerateRandomFloat(StaticFunctionTag*, float afMin, float afMax);

		static UInt32 GenerateRandomInt(StaticFunctionTag*, UInt32 afMin, UInt32 afMax);

		//--------------------------------------------------------------------------------------------
		// MAGICEFFECT
		//--------------------------------------------------------------------------------------------

		static BSScript::VMArray<EffectSetting*> GetAllActiveEffectsOnActor(StaticFunctionTag*, Actor* thisActor, bool showInactive);

		static BSFixedString GetEffectArchetypeInternal(EffectSetting* mgef);

		static bool HasMagicEffectWithArchetype(StaticFunctionTag*, Actor* thisActor, BSFixedString archetype);

		static UInt32 GetEffectArchetypeAsInt(StaticFunctionTag*, EffectSetting* mgef);

		static BSFixedString GetEffectArchetypeAsString(StaticFunctionTag*, EffectSetting* mgef);

		static BGSSoundDescriptorForm* GetMagicEffectSound(StaticFunctionTag*, EffectSetting* mgef, UInt32 type);

		static void SetMagicEffectSound(StaticFunctionTag*, EffectSetting* mgef, BGSSoundDescriptorForm* mgefSound, UInt32 type);

		//--------------------------------------------------------------------------------------------
		// OBJECTREFERENCE
		//--------------------------------------------------------------------------------------------

		static BSScript::VMArray<float> GetPositionAsArray(StaticFunctionTag*, TESObjectREFR* ref);

		static BSScript::VMArray<float> GetRotationAsArray(StaticFunctionTag*, TESObjectREFR* ref);

		static bool IsLoadDoor(StaticFunctionTag*, TESObjectREFR* thisDoor);

		static bool CanItemBeTaken(InventoryEntryData* data, bool noEquipped, bool noFavourited, bool noQuestItem);

		static void AddAllInventoryItemsToList(StaticFunctionTag*, TESObjectREFR* thisRef, BGSListForm* thisList, bool noEquipped, bool noFavourited, bool noQuestItem);

		static BSScript::VMArray<TESForm*> AddAllInventoryItemsToArray(StaticFunctionTag*, TESObjectREFR* thisRef, bool noEquipped, bool noFavourited, bool noQuestItem);

		static BSScript::VMArray<TESForm*> AddAllEquippedItemsToArray(StaticFunctionTag*, Actor* thisActor);

		static void ReplaceKeywordOnRef(StaticFunctionTag*, TESObjectREFR* thisRef, BGSKeyword* KYWDtoRemove, BGSKeyword* KYWDtoAdd);

		static void AddKeywordToRef(StaticFunctionTag*, TESObjectREFR* thisRef, BGSKeyword* KYWDtoAdd);

		static void Apply2DHavokImpulse(StaticFunctionTag*, TESObjectREFR* source, TESObjectREFR* target, float afZ, float magnitude);

		static void Apply3DHavokImpulse(StaticFunctionTag*, TESObjectREFR* source, TESObjectREFR* target, float magnitude);

		static void MoveToNearestNavmeshLocation(StaticFunctionTag*, TESObjectREFR* target);

		static BSScript::VMArray<TESEffectShader*>GetAllEffectShaders(StaticFunctionTag*, TESObjectREFR* thisRef);

		static UInt32 HasEffectShader(StaticFunctionTag*, TESObjectREFR* thisRef, TESEffectShader* effectShader);

		static BSScript::VMArray<BGSArtObject*> GetAllArtObjects(StaticFunctionTag*, TESObjectREFR* thisRef);

		static UInt32 HasArtObject(StaticFunctionTag*, TESObjectREFR* thisRef, BGSArtObject* artObject);

		static void StopArtObject(StaticFunctionTag*, TESObjectREFR* thisRef, BGSArtObject* artObject);

		static void PO3_SKSEFunctions::StopAllShaders(StaticFunctionTag*, TESObjectREFR* thisRef);

		static Actor* GetActorCause(StaticFunctionTag*, TESObjectREFR* thisRef);

		static Actor* GetClosestActorFromRef(StaticFunctionTag*, TESObjectREFR* thisRef, bool ignorePlayer);

		static Actor* GetRandomActorFromRef(StaticFunctionTag*, TESObjectREFR* thisRef, float radius, bool ignorePlayer);

		//--------------------------------------------------------------------------------------------
		// PACKAGE
		//--------------------------------------------------------------------------------------------

		static SInt32 GetPackageType(StaticFunctionTag*, TESPackage* package);

		//--------------------------------------------------------------------------------------------
		// PROJECTILE
		//--------------------------------------------------------------------------------------------

		static float GetProjectileSpeed(StaticFunctionTag*, BGSProjectile* thisProjectile);

		static void SetProjectileSpeed(StaticFunctionTag*, BGSProjectile* thisProjectile, float speed);

		static float GetProjectileRange(StaticFunctionTag*, BGSProjectile* thisProjectile);

		static void SetProjectileRange(StaticFunctionTag*, BGSProjectile* thisProjectile, float range);

		static float GetProjectileGravity(StaticFunctionTag*, BGSProjectile* thisProjectile);

		static void SetProjectileGravity(StaticFunctionTag*, BGSProjectile* thisProjectile, float gravity);

		static float GetProjectileImpactForce(StaticFunctionTag*, BGSProjectile* thisProjectile);

		static void SetProjectileImpactForce(StaticFunctionTag*, BGSProjectile* thisProjectile, float impactForce);

		static UInt32 GetProjectileType(StaticFunctionTag*, BGSProjectile* thisProjectile);

		//--------------------------------------------------------------------------------------------
		// SOUND
		//--------------------------------------------------------------------------------------------

		static void SetSoundDescriptor(StaticFunctionTag*, TESSound* thisSound, BGSSoundDescriptorForm* thisSoundDescriptor);

		//--------------------------------------------------------------------------------------------
		// SPELL
		//--------------------------------------------------------------------------------------------

		static UInt32 GetSpellType(StaticFunctionTag*, SpellItem* thisSpell);

		//--------------------------------------------------------------------------------------------
		// VISUALEFFECT
		//--------------------------------------------------------------------------------------------

		static BGSArtObject* GetArtObject(StaticFunctionTag*, BGSReferenceEffect* visualEffect);

		static void SetArtObject(StaticFunctionTag*, BGSReferenceEffect* visualEffect, BGSArtObject* art);

		//--------------------------------------------------------------------------------------------
		// WIND
		//--------------------------------------------------------------------------------------------

		static UInt32 GetWindSpeedAsInt(StaticFunctionTag*, TESWeather* thisWeather);

		static float GetWindSpeedAsFloat(StaticFunctionTag*, TESWeather* thisWeather);

		static SInt32 GetWeatherType(StaticFunctionTag*, TESWeather* thisWeather);

		//---------------------------------------------------------------------------------------------

		static bool Register(BSScript::Internal::VirtualMachine* a_vm);
	};
};
