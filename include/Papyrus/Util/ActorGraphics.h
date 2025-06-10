#pragma once

namespace GRAPHICS
{
	using Feature = RE::BSShaderMaterial::Feature;
	using HeadPartType = RE::BGSHeadPart::HeadPartType;
	using States = RE::BSGeometry::States;
	using Texture = RE::BSTextureSet::Texture;
	using ShaderFlags = RE::BSShaderProperty::EShaderPropertyFlag;
	using ShaderFlags8 = RE::BSShaderProperty::EShaderPropertyFlag8;
	using VertexFlags = RE::BSGraphics::Vertex::Flags;
	using MaterialBase = RE::BSLightingShaderMaterialBase;
	using Slot = RE::BGSBipedObjectForm::BipedObjectSlot;

	inline constexpr std::array headSlots{ RE::BIPED_OBJECT::kHair, RE::BIPED_OBJECT::kLongHair, RE::BIPED_OBJECT::kCirclet };

	namespace EXTRA
	{
		template <class T, typename D>
		void Add(RE::NiAVObject* a_root, std::string_view a_type, D a_data)
		{
			if (const auto data = a_root->GetExtraData<T>(a_type); !data) {
				if (const auto newData = T::Create(a_type, a_data); newData) {
					a_root->AddExtraData(newData);
				}
			}
		}

		inline constexpr auto TOGGLE = "PO3_TOGGLE"sv;
		inline constexpr auto SKIN_TINT = "PO3_SKINTINT"sv;
		inline constexpr auto HAIR_TINT = "PO3_HAIRTINT"sv;
		inline constexpr auto SKIN_ALPHA = "PO3_SKINALPHA"sv;
		inline constexpr auto FACE_TXST = "PO3_FACETXST"sv;
		inline constexpr auto HEADPART = "PO3_HEADPART"sv;
		inline constexpr auto TXST = "PO3_TXST"sv;
		inline constexpr auto SKIN_TXST = "PO3_SKINTXST"sv;
		inline constexpr auto SHADER = "PO3_SHADER |"sv;
		inline constexpr auto ORIG_SHADER = "PO3_ORIGINALSHADER"sv;
	}

	namespace TEXTURE
	{
		void                    sanitize_path(std::string& a_path);
		RE::BSShaderTextureSet* create_textureset(char** a_value);
	}

	struct ShaderData
	{
		enum TYPE
		{
			kDiffuse = 0,
			kTextureTotal = 9,

			kFeature = kTextureTotal,
			kFlag,
			kColor,
			kColorMult,

			kShaderTotal
		};

		struct Input
		{
			Input(RE::TESObjectREFR* a_ref, const RE::BSFixedString& a_filter, std::uint32_t a_shaderType, std::int32_t a_textureType, bool a_noWeapons, bool a_noAlpha);

			void SetShaderType(RE::NiAVObject* a_root, RE::NiAVObject* a_templateRoot) const;

		private:
			void SetShaderTypeImpl(RE::NiAVObject* a_object, RE::BSGeometry* a_templateGeo, std::vector<RE::BSFixedString>& results) const;

			bool               noWeapons;
			bool               noAlphaMeshes;
			bool               isActor{};
			std::int32_t       textureType{};
			REX::Enum<Feature> feature{};
			std::string        sourcePath{};
		};

		struct Output
		{
			Output() = default;
			Output(RE::NiStringsExtraData* a_data);

			bool Reset(RE::BSGeometry* a_geometry, RE::BSLightingShaderProperty* a_shaderProp, RE::BSLightingShaderMaterialBase* a_material) const;

			RE::BSShaderTextureSet* textureSet;
			REX::Enum<Feature>      feature;
			std::uint64_t           flags;
			RE::NiColor             emissiveColor;
			float                   emissiveMult;
			bool                    hasData;
		};
	};

	struct ActorApplier
	{
		static void ToggleNode(RE::NiAVObject* a_root, RE::NiAVObject* a_node, bool a_cull);
		static void BodyTint(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiColor color);
		static void HairTint(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiColor color);
		static void ArmorTXST(RE::NiAVObject* a_object, RE::BGSTextureSet* a_txst, std::int32_t a_type, std::string_view a_tgtPath, bool& replaced);
		static void SkinTXST(RE::NiAVObject* a_object, RE::BGSTextureSet* a_txst, std::vector<RE::BSFixedString>& a_vec, std::int32_t a_type);
		static void ArmorSkinTXST(RE::Actor* a_actor, RE::BGSTextureSet* a_txst, RE::BIPED_MODEL::BipedObjectSlot a_slot, std::int32_t a_type);

	private:
		static void tint_face(RE::Actor* a_actor, const RE::NiColor& a_color);
		static void update_color_data(RE::NiAVObject* a_root, const RE::BSFixedString& a_name, const RE::NiColor& a_color);
	};

	struct ActorResetter
	{
		ActorResetter() = default;
		ActorResetter(RE::Actor* a_actor, RE::NiAVObject* a_object, const RE::BSFixedString& a_folderName = {});

		bool ResetActor3D() const;

	private:
		static void stop_all_skin_shaders(RE::TESObjectREFR* a_ref);
		static void reset_textureset(RE::NiAVObject* a_object, RE::BSShaderTextureSet* a_txst, bool a_doOnlySkin, const std::string& a_folder = {});
		static void reset_shaderdata(RE::NiAVObject* a_object, std::vector<RE::BSFixedString>& a_geometries);

		void ResetToggle() const;
		void ResetSkinAlpha() const;
		void ResetHeadPartAlpha() const;
		void ResetSkinTint() const;
		void ResetHairTint() const;
		void ResetFaceTXST() const;
		void ResetArmorTXST() const;
		void ResetSkinTXST() const;
		void ResetMaterialShader() const;

		// members
		RE::NiPointer<RE::Actor>             actor{};
		RE::NiPointer<RE::NiAVObject>        root{};
		std::string                          folderName{};
		RE::NiStringsExtraData*              toggle{};
		RE::NiIntegerExtraData*              tintSkin{};
		RE::NiIntegerExtraData*              tintHair{};
		RE::NiBooleanExtraData*              alphaSkin{};
		RE::NiStringsExtraData*              txstFace{};
		std::vector<RE::NiIntegerExtraData*> alphaHDPT{};
		std::vector<RE::NiStringsExtraData*> txst{};
		std::vector<RE::NiStringsExtraData*> txstSkin{};
		std::vector<RE::NiStringsExtraData*> shaders{};
		bool                                 hasData{ false };
	};
}
