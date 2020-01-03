#pragma once

#include "po3_functions.h"

//-----------------------------------------------------------------------------------------------------------------------

namespace RE
{
	void SetShaderPropertyRGBTint(BSGeometry* geometry);

	void SetShaderPropertyAlpha(BSGeometry* geometry, float alpha, bool onlySkin);
	void SetArmorSkinAlpha(Actor* thisActor, BGSBipedObjectForm::BipedObjectSlot slotMask, float alpha);

	bool ReplaceTextureSet(BSGeometry* geometry, BGSTextureSet& sourceTXST, BGSTextureSet& targetTXST, SInt32 textureType, const std::string& texturePath);

	bool ReplaceSkinTXST(BSGeometry* geometry, BGSTextureSet& TXST, std::vector<BSFixedString>& vec, SInt32 textureType);
	void SetArmorSkinTXST(Actor* thisActor, BGSTextureSet* TXST, BGSBipedObjectForm::BipedObjectSlot slotMask, SInt32 textureType);

	void SetShaderPropertyType(BSGeometry* geometry, BSGeometry* templateGeometry);
	BSGeometry* GetArmorGeometry(Actor* thisActor, BGSBipedObjectForm::BipedObjectSlot slotMask);

	NiAVObject* VisitArmorAddon(Actor* actor, TESObjectARMO* armor, TESObjectARMA* arma);
	BSGeometry* GetHeadPartGeometry(Actor* thisActor, BGSHeadPart::HeadPartType partType);

	TESObjectARMO* GetWornFormByID(Actor* thisActor, FormID id);
	void ResetTextureSet(BSGeometry* geometry, BSShaderTextureSet* textureset, bool isSkin);

	bool HasShaderType(NiAVObject* object, BSShaderMaterial::Feature shaderType);
	BSGeometry* GetFirstShaderType(NiAVObject* object, BSShaderMaterial::Feature shaderType);

	float calculateLuminance(UInt8 R, UInt8 G, UInt8 B);
	UInt8 colorMix(UInt8 a, UInt8 b, float t);

	TESObjectARMO* GetSkinForm(Actor* thisActor, BGSBipedObjectForm::BipedObjectSlot mask);
	TESObjectARMA* GetArmorAddonByMask(TESRace* race, TESObjectARMO* armor, BGSBipedObjectForm::BipedObjectSlot mask);

	void SanitizePath(std::string& path);
}
