#pragma once

#include "po3_functions.h"

//-----------------------------------------------------------------------------------------------------------------------

namespace RE
{
	void SetShaderPropertyRGBTint(BSGeometry* geometry);

	void SetShaderPropertyAlpha(BSGeometry* geometry, float alpha, bool onlySkin);
	void SetArmorSkinAlpha(Actor* thisActor, BGSBipedObjectForm::FirstPersonFlag slotMask, float alpha);

	bool ReplaceTextureSet(BSGeometry* geometry, BGSTextureSet& sourceTXST, BGSTextureSet& targetTXST, SInt32 textureType, const std::string& path);

	bool ReplaceSkinTXST(BSGeometry* geometry, BGSTextureSet& TXST, std::vector<std::string>& vec, SInt32 textureType);
	void SetArmorSkinTXST(Actor* thisActor, BGSTextureSet* TXST, BGSBipedObjectForm::FirstPersonFlag slotMask, SInt32 textureType);

	void SetShaderPropertyType(BSGeometry* geometry, BSGeometry* templateGeometry);
	BSGeometry* GetArmorGeometry(Actor* thisActor, BGSBipedObjectForm::FirstPersonFlag slotMask);

	NiAVObject* VisitArmorAddon(Actor* actor, TESObjectARMO* armor, TESObjectARMA* arma);
	BSGeometry* GetHeadPartGeometry(Actor* thisActor, BGSHeadPart::Type partType);

	TESObjectARMO* GetWornFormByID(Actor* thisActor, FormID id);
	void ResetTextureSet(BSGeometry* geometry, BSShaderTextureSet* textureset, bool isSkin);
	void ResetNiNodeTextureSet(NiAVObject* object, BSShaderTextureSet& TXST, bool onlySkin);

	bool HasShaderType(NiAVObject* object, BSShaderMaterial::Type shaderType);
	BSGeometry* GetFirstShaderType(NiAVObject* object, BSShaderMaterial::Type shaderType);

	float calculateLuminance(UInt8 R, UInt8 G, UInt8 B);
	UInt8 colorMix(UInt8 a, UInt8 b, float t);

	TESObjectARMO* GetSkinForm(Actor* thisActor, BGSBipedObjectForm::FirstPersonFlag mask);
	TESObjectARMA* GetArmorAddonByMask(TESRace* race, TESObjectARMO* armor, BGSBipedObjectForm::FirstPersonFlag mask);
}
