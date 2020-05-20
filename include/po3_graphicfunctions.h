#pragma once

#include "po3_functions.h"


namespace RE
{
	void TintFace(Actor* a_actor, const NiColor& a_color);
	void AddOrUpdateColorData(NiAVObject* a_root, const BSFixedString& a_name, const NiColor& a_color);

	void SetTextureSet(NiAVObject* a_object, BGSTextureSet& a_txst, SInt32 a_type, const std::string& a_tgtPath, bool& replaced);
	
	void SetSkinTextureSet(NiAVObject* a_object, BGSTextureSet& a_txst, std::vector<BSFixedString>& a_vec, SInt32 a_type);
	void SetArmorSkinTXST(Actor* a_actor, BGSTextureSet* a_txst, BGSBipedObjectForm::BipedObjectSlot a_slot, SInt32 a_type);

	BSGeometry* GetSkinGeometry(Actor* a_actor, BGSBipedObjectForm::FirstPersonFlag a_slot);

	void ResetTextureSet(NiAVObject* a_object, BSShaderTextureSet& a_txst, NiAVObject::ALPHA_MODE a_type, const std::string& a_folder);

	void SetShaderType_Impl(NiAVObject* a_object, BSGeometry* a_template, const std::string& a_path, SInt32 a_textureType, std::vector<std::vector<BSFixedString>>& a_vec, bool a_noWeapons, bool a_noAlpha, bool a_isActor);

	void ResetShaderType(NiAVObject* a_object, BSShaderTextureSet& a_txst, BSShaderProperty::EShaderPropertyFlag a_flags, const NiColor& a_emissive, float a_emissiveMult, BSShaderMaterial::Feature a_changed, BSShaderMaterial::Feature a_original, const std::string& changedDiffuse);
}
