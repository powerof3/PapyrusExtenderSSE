#pragma once

#include "po3_functions.h"


namespace RE
{
	void MakeFaceTintable(Actor* a_actor, const NiColor& a_color);
	void AddOrUpdateColorData(NiAVObject* a_root, const BSFixedString& a_name, const Color& a_color);

	void SetArmorSkinAlpha(Actor* a_actor, BGSBipedObjectForm::FirstPersonFlag a_slot, float a_alpha);

	void SetTextureSet(NiAVObject* a_object, BGSTextureSet* a_srcTXST, BGSTextureSet* a_tgtTXST, SInt32 a_type, const std::string& a_tgtPath, bool& replaced);
	
	void SetSkinTextureSet(NiAVObject* a_object, BGSTextureSet* a_txst, std::vector<BSFixedString>& a_vec, SInt32 a_type);
	void SetArmorSkinTXST(Actor* a_actor, BGSTextureSet* a_txst, BGSBipedObjectForm::BipedObjectSlot a_slot, SInt32 a_type);

	BSGeometry* GetSkinGeometry(Actor* a_actor, BGSBipedObjectForm::FirstPersonFlag a_slot);

	void ResetTextureSet(NiAVObject* a_object, BSShaderTextureSet* a_txst, bool a_skin, const std::string& a_folder = std::string());
}
