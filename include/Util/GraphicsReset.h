#pragma once


namespace RESET
{
	using Feature = RE::BSShaderMaterial::Feature;
	using HeadPartType = RE::BGSHeadPart::HeadPartType;
	using Flag = RE::BSShaderProperty::EShaderPropertyFlag;
	using Flag8 = RE::BSShaderProperty::EShaderPropertyFlag8;
	using Slot = RE::BGSBipedObjectForm::BipedObjectSlot;
	using State = RE::BSGeometry::States;
	using Texture = RE::BSTextureSet::Texture;
	using VertexFlags = RE::NiSkinPartition::Vertex::Flags;

	using ResetData = std::tuple<
		RE::NiStringsExtraData*,
		RE::NiIntegerExtraData*,
		RE::NiIntegerExtraData*,
		RE::NiBooleanExtraData*,
		RE::NiStringsExtraData*,
		std::vector<RE::NiIntegerExtraData*>,
		std::vector<RE::NiStringsExtraData*>,
		std::vector<RE::NiStringsExtraData*>,
		std::vector<RE::NiStringsExtraData*>>;

	using ShaderData = std::tuple<RE::BSShaderTextureSet*, Feature, std::uint64_t, RE::NiColor, float>;


	std::pair<bool, ResetData> GetData(RE::NiAVObject* a_object);

	void Toggle(RE::NiAVObject* a_root, RE::NiStringsExtraData* a_data);

	void SkinAlpha(RE::NiAVObject* a_root, RE::NiBooleanExtraData* a_data);

	void HeadPartAlpha(RE::Actor* a_actor, RE::NiAVObject* a_root, const std::vector<RE::NiIntegerExtraData*>& a_data);

	void SkinTint(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiIntegerExtraData* a_data);

	void HairTint(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiIntegerExtraData* a_data);

	void FaceTXST(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiStringsExtraData* a_data);

	void TXST(RE::Actor* a_actor, RE::NiAVObject* a_root, const RE::BSFixedString& a_folderName, const std::vector<RE::NiStringsExtraData*>& a_vec);

	void SkinTXST(RE::Actor* a_actor, RE::NiAVObject* a_root, const std::vector<RE::NiStringsExtraData*>& a_vec);

	void MaterialShader(RE::NiAVObject* a_root, const std::vector<RE::NiStringsExtraData*>& a_vec);
}
