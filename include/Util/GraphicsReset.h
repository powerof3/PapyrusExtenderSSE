#pragma once


namespace GraphicsReset
{
	using Feature = RE::BSShaderMaterial::Feature;
	using Flag = RE::BSShaderProperty::EShaderPropertyFlag;
	using Flag8 = RE::BSShaderProperty::EShaderPropertyFlag8;
	using Slot = RE::BGSBipedObjectForm::BipedObjectSlot;
	using State = RE::BSGeometry::States;
	using Texture = RE::BSTextureSet::Texture;		
	using VertexFlags = RE::NiSkinPartition::Vertex::Flags;

	enum TYPE : std::uint32_t
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
	using ResetData = std::tuple<RE::NiStringsExtraData*, RE::NiIntegersExtraData*, RE::NiIntegersExtraData*, RE::NiFloatExtraData*, RE::NiIntegersExtraData*, RE::NiStringsExtraData*, std::vector<RE::NiStringsExtraData*>, std::vector<RE::NiStringsExtraData*>, std::vector<RE::NiStringsExtraData*>>;

	enum SHADER_TYPE : std::uint32_t
	{
		kTexture = 0,
		kFlags,
		kEmissive,
		kEmissiveMult,
		kFeature,
		kFeatureOrig,
		kDiffuse,
	};
	using ShaderData = std::tuple<RE::BSShaderTextureSet*, Flag, RE::NiColor, float, Feature, Feature, std::string>;


	inline constexpr frozen::set<Slot, 6> fxSlots = 
	{ Slot::kModMouth, Slot::kModChestPrimary, Slot::kModPelvisPrimary, Slot::kModLegRight, Slot::kModChestSecondary, Slot::kModArmRight };


	ResetData GetResetData(RE::NiAVObject* a_object);

	void ResetToggleData(RE::NiAVObject* a_root, RE::NiStringsExtraData* a_data);

	void ResetAlphaData(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiFloatExtraData* a_data, const RE::BSFixedString& a_folderName);

	void ResetHeadPartAlphaData(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiIntegersExtraData* a_data);

	void ResetSkinTintData(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiIntegersExtraData* a_data);

	void ResetHairTintData(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiIntegersExtraData* a_data);

	void ResetFaceTXSTData(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiStringsExtraData* a_data);

	void ResetTXSTData(RE::Actor* a_actor, RE::NiAVObject* a_root, const RE::BSFixedString& a_folderName, const std::vector<RE::NiStringsExtraData*>& a_vec);

	void ResetSkinTXSTData(RE::Actor* a_actor, RE::NiAVObject* a_root, const std::vector<RE::NiStringsExtraData*>& a_vec);

	void ResetShaderData(RE::NiAVObject* a_root, const std::vector<RE::NiStringsExtraData*>& a_vec);
}
