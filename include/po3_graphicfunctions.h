#pragma once

#include "po3_functions.h"
#include "po3_offsets.h"

//-----------------------------------------------------------------------------------------------------------------------

void SetShaderPropertyRGBTint(RE::BSGeometry* geometry);
void SetShaderPropertyAlpha(RE::BSGeometry* geometry, float alpha, bool onlySkin);
void ReplaceTextureSet(RE::BSGeometry* geometry, RE::BGSTextureSet* sourceTXST, RE::BGSTextureSet* targetTXST, SInt32 textureType);
void ReplaceSkinTXST(RE::BSGeometry* geometry, RE::BGSTextureSet* TXST, SInt32 textureType);

void SetShaderPropertyMLP(RE::BSGeometry* geometry, RE::BSGeometry* templateGeometry);

RE::BSGeometry* GetArmorGeometry(RE::Actor* thisActor, RE::BGSBipedObjectForm::FirstPersonFlag slotMask, SInt32 shaderType);

RE::NiAVObject* VisitArmorAddon(RE::Actor* thisActor, RE::TESObjectARMO* thisArmor, RE::TESObjectARMA* thisArma);
RE::BSGeometry* GetHeadPartGeometry(RE::Actor* thisActor, UInt32 partType);

SInt32 GetShaderPropertyType(RE::BSGeometry* geometry);
UInt32 GetShaderPropertyModdedSkin(RE::BSGeometry* geometry);

void SetArmorSkinAlpha(RE::Actor* thisActor, RE::BGSBipedObjectForm::FirstPersonFlag slotMask, float alpha);
void SetArmorSkinTXST(RE::Actor* thisActor, RE::BGSTextureSet* TXST, RE::BGSBipedObjectForm::FirstPersonFlag slotMask, SInt32 textureType);

float calculateLuminance(UInt8 R, UInt8 G, UInt8 B);
UInt8 colorMix(UInt8 a, UInt8 b, float t);

RE::TESObjectARMO* GetSkinForm(RE::Actor* thisActor, RE::BGSBipedObjectForm::FirstPersonFlag mask);
RE::TESObjectARMA* GetArmorAddonByMask(RE::TESRace* race, RE::TESObjectARMO* armor, RE::BGSBipedObjectForm::FirstPersonFlag mask);