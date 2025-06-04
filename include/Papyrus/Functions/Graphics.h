#pragma once

namespace Papyrus::Graphics
{
	void ApplyMaterialShader(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::BGSMaterialObject* a_shader, float a_materialThresholdAngle);

	//TO_DO
	/* bool AttachModel(STATIC_ARGS,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_path,
		RE::BSFixedString a_nodeName,
		std::vector<float> a_translate,
		std::vector<float> a_rotate,
		float a_scale)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return false;
		}

		const auto root = a_ref->Get3D();
		if (!root) {
			a_vm->TraceForm(a_ref, "has no 3D", a_stackID, Severity::kInfo);
			return false;
		}
	}*/

	void               BlendColorWithSkinTone(STATIC_ARGS, RE::Actor* a_actor, RE::BGSColorForm* a_color, std::uint32_t a_blendMode, bool a_autoCalc, float a_opacity);
	bool               HasSkin(STATIC_ARGS, RE::Actor* a_actor, RE::TESObjectARMO* a_check);
	RE::BGSColorForm*  GetHairColor(STATIC_ARGS, const RE::Actor* a_actor);
	RE::BGSTextureSet* GetHeadPartTextureSet(STATIC_ARGS, RE::Actor* a_actor, std::uint32_t a_type);
	RE::BGSColorForm*  GetSkinColor(STATIC_ARGS, RE::Actor* a_actor);
	void               MixColorWithSkinTone(STATIC_ARGS, RE::Actor* a_actor, RE::BGSColorForm* a_color, bool a_manual, float a_percent);
	void               PlayDebugShader(STATIC_ARGS, RE::TESObjectREFR* a_ref, std::vector<float> a_rgba);
	void               ReplaceArmorTextureSet(STATIC_ARGS, RE::Actor* a_actor, RE::TESObjectARMO* a_armor, RE::BGSTextureSet* a_srcTXST, RE::BGSTextureSet* a_tgtTXST, std::int32_t a_type);
	void               ReplaceFaceTextureSet(STATIC_ARGS, RE::Actor* a_actor, RE::BGSTextureSet* a_maleTXST, RE::BGSTextureSet* a_femaleTXST, std::int32_t a_type);
	void               ReplaceSkinTextureSet(STATIC_ARGS, RE::Actor* a_actor, RE::BGSTextureSet* a_maleTXST, RE::BGSTextureSet* a_femaleTXST, std::uint32_t a_slot, std::int32_t a_type);
	bool               ResetActor3D(STATIC_ARGS, RE::Actor* a_actor, RE::BSFixedString a_folderName);
	void               ScaleObject3D(STATIC_ARGS, RE::TESObjectREFR* a_ref, std::string a_nodeName, float a_scale);
	void               SetHairColor(STATIC_ARGS, RE::Actor* a_actor, RE::BGSColorForm* a_color);
	void               SetHeadPartAlpha(STATIC_ARGS, RE::Actor* a_actor, std::int32_t a_type, float a_alpha);
	void               SetHeadPartTextureSet(STATIC_ARGS, RE::Actor* a_actor, RE::BGSTextureSet* a_txst, std::int32_t a_type);
	void               SetShaderType(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::TESObjectREFR* a_template, RE::BSFixedString a_filter, std::uint32_t a_shaderType, std::int32_t a_textureType, bool a_noWeapons, bool a_noAlpha);
	void               SetupBodyPartGeometry(STATIC_ARGS, RE::TESObjectREFR* a_bodyparts, RE::Actor* a_actor);
	void               SetSkinAlpha(STATIC_ARGS, RE::Actor* a_actor, float a_alpha);
	void               SetSkinColor(STATIC_ARGS, RE::Actor* a_actor, RE::BGSColorForm* a_color);
	void               ToggleChildNode(STATIC_ARGS, RE::TESObjectREFR* a_ref, std::string a_nodeName, bool a_disable);
	void               ToggleHairWigs(STATIC_ARGS, RE::Actor* a_actor, bool a_disable);
	void               UpdateHitEffectArtNode(STATIC_ARGS, RE::TESObjectREFR* a_ref, const RE::BGSArtObject* a_art, RE::BSFixedString a_toNode, std::vector<float> a_translate, std::vector<float> a_rotate, float a_scale);

	void Bind(VM& a_vm);
}
