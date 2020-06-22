#pragma once

#include "main.h"


namespace Reset
{
	using Flags = RE::BSShaderProperty::EShaderPropertyFlag;
	using Feature = RE::BSShaderMaterial::Feature;
	using Slot = RE::BGSBipedObjectForm::BipedObjectSlot;
	
	inline constexpr std::array<Slot, 6> fxSlots = { Slot::kModMouth, Slot::kModChestPrimary, Slot::kModPelvisPrimary, Slot::kModLegRight, Slot::kModChestSecondary, Slot::kModArmRight };


	enum RESET_TYPE : UInt8
	{
		kToggle = 0,
		kTintSkin,
		kTintHair,
		kAlpha,
		kAlphaHDPT,
		kTXSTFace,
		kTXST,
		kTXSTSkin,
		kShader
	};
	using RESET = Reset::RESET_TYPE;
	using ResetData = std::tuple<RE::NiStringsExtraData*, RE::NiIntegersExtraData*, RE::NiIntegersExtraData*, RE::NiFloatExtraData*, RE::NiIntegersExtraData*, RE::NiStringsExtraData*, std::vector<RE::NiStringsExtraData*>, std::vector<RE::NiStringsExtraData*>, std::vector<RE::NiStringsExtraData*>>;


	enum SHADER_TYPE : UInt8
	{
		kTexture = 0,
		kFlags,
		kEmissive,
		kEmissiveMult,
		kFeature,
		kFeatureOrig,
		kDiffuse,
	};
	using ShaderData = std::tuple<RE::BSShaderTextureSet*, Flags, RE::NiColor, float, Feature, Feature, std::string>;


	inline UInt32 constexpr const_hash(const char* input, UInt32 hash = 5381)
	{
		return *input ?
			const_hash(input + 1, hash * 33 + static_cast<UInt32>(*input)) :
			hash;
	}


	ResetData GetResetData(RE::NiAVObject* a_object);
	

	void ResetToggleData(RE::NiAVObject* a_root, RE::NiStringsExtraData* a_data);

	void ResetAlphaData(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiFloatExtraData* a_data);

	void ResetHeadPartAlphaData(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiIntegersExtraData* a_data);

	void ResetSkinTintData(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiIntegersExtraData* a_data);

	void ResetHairTintData(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiIntegersExtraData* a_data);

	void ResetFaceTXSTData(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiStringsExtraData* a_data);

	void ResetTXSTData(RE::Actor* a_actor, RE::NiAVObject* a_root, const RE::BSFixedString& a_folderName, const std::vector<RE::NiStringsExtraData*>& a_vec);

	void ResetSkinTXSTData(RE::Actor* a_actor, RE::NiAVObject* a_root, const std::vector<RE::NiStringsExtraData*>& a_vec);

	void ResetShaderData(RE::NiAVObject* a_root, const std::vector<RE::NiStringsExtraData*>& a_vec);
}

