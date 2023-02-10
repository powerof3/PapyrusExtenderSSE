#pragma once

namespace GRAPHICS
{
	namespace ACTOR
	{
		inline constexpr std::array headSlots = { RE::BIPED_OBJECT::kHair, RE::BIPED_OBJECT::kLongHair, RE::BIPED_OBJECT::kCirclet };
	}

	namespace EXTRA
	{
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

	namespace MATERIAL
	{
		inline constexpr frozen::map<std::string_view, RE::MATERIAL_ID, 88> materialMap = {
			{ "None"sv, RE::MATERIAL_ID::kNone },
			{ "StoneBroken"sv, RE::MATERIAL_ID::kStoneBroken },
			{ "BlockBlade1Hand"sv, RE::MATERIAL_ID::kBlockBlade1Hand },
			{ "Meat"sv, RE::MATERIAL_ID::kMeat },
			{ "CarriageWheel"sv, RE::MATERIAL_ID::kCarriageWheel },
			{ "MetalLight"sv, RE::MATERIAL_ID::kMetalLight },
			{ "WoodLight"sv, RE::MATERIAL_ID::kWoodLight },
			{ "Snow"sv, RE::MATERIAL_ID::kSnow },
			{ "Gravel"sv, RE::MATERIAL_ID::kGravel },
			{ "ChainMetal"sv, RE::MATERIAL_ID::kChainMetal },
			{ "Bottle"sv, RE::MATERIAL_ID::kBottle },
			{ "Wood"sv, RE::MATERIAL_ID::kWood },
			{ "Ash"sv, RE::MATERIAL_ID::kAsh },
			{ "Skin"sv, RE::MATERIAL_ID::kSkin },
			{ "BlockBlunt"sv, RE::MATERIAL_ID::kBlockBlunt },
			{ "DLC1DeerSkin"sv, RE::MATERIAL_ID::kDLC1DeerSkin },
			{ "Insect"sv, RE::MATERIAL_ID::kInsect },
			{ "Barrel"sv, RE::MATERIAL_ID::kBarrel },
			{ "CeramicMedium"sv, RE::MATERIAL_ID::kCeramicMedium },
			{ "Basket"sv, RE::MATERIAL_ID::kBasket },
			{ "Ice"sv, RE::MATERIAL_ID::kIce },
			{ "GlassStairs"sv, RE::MATERIAL_ID::kGlassStairs },
			{ "StoneStairs"sv, RE::MATERIAL_ID::kStoneStairs },
			{ "Water"sv, RE::MATERIAL_ID::kWater },
			{ "DraugrSkeleton"sv, RE::MATERIAL_ID::kDraugrSkeleton },
			{ "Blade1Hand"sv, RE::MATERIAL_ID::kBlade1Hand },
			{ "Book"sv, RE::MATERIAL_ID::kBook },
			{ "Carpet"sv, RE::MATERIAL_ID::kCarpet },
			{ "MetalSolid"sv, RE::MATERIAL_ID::kMetalSolid },
			{ "Axe1Hand"sv, RE::MATERIAL_ID::kAxe1Hand },
			{ "BlockBlade2Hand"sv, RE::MATERIAL_ID::kBlockBlade2Hand },
			{ "OrganicLarge"sv, RE::MATERIAL_ID::kOrganicLarge },
			{ "Amulet"sv, RE::MATERIAL_ID::kAmulet },
			{ "WoodStairs"sv, RE::MATERIAL_ID::kWoodStairs },
			{ "Mud"sv, RE::MATERIAL_ID::kMud },
			{ "BoulderSmall"sv, RE::MATERIAL_ID::kBoulderSmall },
			{ "SnowStairs"sv, RE::MATERIAL_ID::kSnowStairs },
			{ "StoneHeavy"sv, RE::MATERIAL_ID::kStoneHeavy },
			{ "DragonSkeleton"sv, RE::MATERIAL_ID::kDragonSkeleton },
			{ "Trap"sv, RE::MATERIAL_ID::kTrap },
			{ "BowsStaves"sv, RE::MATERIAL_ID::kBowsStaves },
			{ "Alduin"sv, RE::MATERIAL_ID::kAlduin },
			{ "BlockBowsStaves"sv, RE::MATERIAL_ID::kBlockBowsStaves },
			{ "WoodAsStairs"sv, RE::MATERIAL_ID::kWoodAsStairs },
			{ "SteelGreatSword"sv, RE::MATERIAL_ID::kSteelGreatSword },
			{ "Grass"sv, RE::MATERIAL_ID::kGrass },
			{ "BoulderLarge"sv, RE::MATERIAL_ID::kBoulderLarge },
			{ "StoneAsStairs"sv, RE::MATERIAL_ID::kStoneAsStairs },
			{ "Blade2Hand"sv, RE::MATERIAL_ID::kBlade2Hand },
			{ "BottleSmall"sv, RE::MATERIAL_ID::kBottleSmall },
			{ "BoneActor"sv, RE::MATERIAL_ID::kBoneActor },
			{ "Sand"sv, RE::MATERIAL_ID::kSand },
			{ "MetalHeavy"sv, RE::MATERIAL_ID::kMetalHeavy },
			{ "DLC1SabreCatPelt"sv, RE::MATERIAL_ID::kDLC1SabreCatPelt },
			{ "IceForm"sv, RE::MATERIAL_ID::kIceForm },
			{ "Dragon"sv, RE::MATERIAL_ID::kDragon },
			{ "Blade1HandSmall"sv, RE::MATERIAL_ID::kBlade1HandSmall },
			{ "SkinSmall"sv, RE::MATERIAL_ID::kSkinSmall },
			{ "PotsPans"sv, RE::MATERIAL_ID::kPotsPans },
			{ "SkinSkeleton"sv, RE::MATERIAL_ID::kSkinSkeleton },
			{ "Blunt1Hand"sv, RE::MATERIAL_ID::kBlunt1Hand },
			{ "StoneStairsBroken"sv, RE::MATERIAL_ID::kStoneStairsBroken },
			{ "SkinLarge"sv, RE::MATERIAL_ID::kSkinLarge },
			{ "Organic"sv, RE::MATERIAL_ID::kOrganic },
			{ "Bone"sv, RE::MATERIAL_ID::kBone },
			{ "WoodHeavy"sv, RE::MATERIAL_ID::kWoodHeavy },
			{ "Chain"sv, RE::MATERIAL_ID::kChain },
			{ "Dirt"sv, RE::MATERIAL_ID::kDirt },
			{ "Ghost"sv, RE::MATERIAL_ID::kGhost },
			{ "SkinMetalLarge"sv, RE::MATERIAL_ID::kSkinMetalLarge },
			{ "BlockAxe"sv, RE::MATERIAL_ID::kBlockAxe },
			{ "ArmorLight"sv, RE::MATERIAL_ID::kArmorLight },
			{ "ShieldLight"sv, RE::MATERIAL_ID::kShieldLight },
			{ "Coin"sv, RE::MATERIAL_ID::kCoin },
			{ "BlockBlunt2Hand"sv, RE::MATERIAL_ID::kBlockBlunt2Hand },
			{ "ShieldHeavy"sv, RE::MATERIAL_ID::kShieldHeavy },
			{ "ArmorHeavy"sv, RE::MATERIAL_ID::kArmorHeavy },
			{ "Arrow"sv, RE::MATERIAL_ID::kArrow },
			{ "Glass"sv, RE::MATERIAL_ID::kGlass },
			{ "Stone"sv, RE::MATERIAL_ID::kStone },
			{ "WaterPuddle"sv, RE::MATERIAL_ID::kWaterPuddle },
			{ "Cloth"sv, RE::MATERIAL_ID::kCloth },
			{ "SkinMetalSmall"sv, RE::MATERIAL_ID::kSkinMetalSmall },
			{ "Ward"sv, RE::MATERIAL_ID::kWard },
			{ "Web"sv, RE::MATERIAL_ID::kWeb },
			{ "TrailerSteelSword"sv, RE::MATERIAL_ID::kTrailerSteelSword },
			{ "Blunt2Hand"sv, RE::MATERIAL_ID::kBlunt2Hand },
			{ "DLC1SwingingBridge"sv, RE::MATERIAL_ID::kDLC1SwingingBridge },
			{ "BoulderMedium"sv, RE::MATERIAL_ID::kBoulderMedium }
		};

		RE::MATERIAL_ID get_material(const std::string& a_mat);

		void for_each_material_type(const RE::NiPointer<RE::bhkWorldObject>& a_body, std::function<void(RE::MATERIAL_ID&)> a_visitor);
	}

	namespace RESET
	{
		using Feature = RE::BSShaderMaterial::Feature;
		using HeadPartType = RE::BGSHeadPart::HeadPartType;
		using States = RE::BSGeometry::States;

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

		void stop_all_skin_shaders(RE::TESObjectREFR* a_ref);

		std::pair<bool, ResetData> get_data(RE::NiAVObject* a_object);

		void Toggle(RE::NiAVObject* a_root, RE::NiStringsExtraData* a_data);

		void SkinAlpha(RE::NiAVObject* a_root, RE::NiBooleanExtraData* a_data);

		void HeadPartAlpha(RE::Actor* a_actor, RE::NiAVObject* a_root, const std::vector<RE::NiIntegerExtraData*>& a_data);

		void SkinTint(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiIntegerExtraData* a_data);

		void HairTint(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiIntegerExtraData* a_data);

		void FaceTXST(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiStringsExtraData* a_data);

		void ArmorTXST(RE::Actor* a_actor, RE::NiAVObject* a_root, const RE::BSFixedString& a_folderName, const std::vector<RE::NiStringsExtraData*>& a_vec);

		void SkinTXST(RE::Actor* a_actor, RE::NiAVObject* a_root, const std::vector<RE::NiStringsExtraData*>& a_vec);

		void MaterialShader(RE::NiAVObject* a_root, const std::vector<RE::NiStringsExtraData*>& a_vec);
	}

	namespace SET
	{
		void tint_face(RE::Actor* a_actor, const RE::NiColor& a_color);

		void update_color_data(RE::NiAVObject* a_root, const RE::BSFixedString& a_name, const RE::NiColor& a_color);

		template <class T, typename D>
		void add_data_if_none(RE::NiAVObject* a_root, std::string_view a_type, D a_data)
		{
			if (const auto data = a_root->GetExtraData<T>(a_type); !data) {
				if (const auto newData = T::Create(a_type, a_data); newData) {
					a_root->AddExtraData(newData);
				}
			}
		}

		void Toggle(RE::NiAVObject* a_root, RE::NiAVObject* a_node, bool a_cull);

		void ArmorTXST(RE::NiAVObject* a_object, RE::BGSTextureSet* a_txst, std::int32_t a_type, std::string_view a_tgtPath, bool& replaced);

		void SkinTXST(RE::NiAVObject* a_object, RE::BGSTextureSet* a_txst, std::vector<RE::BSFixedString>& a_vec, std::int32_t a_type);

		void ArmorSkinTXST(RE::Actor* a_actor, RE::BGSTextureSet* a_txst, RE::BIPED_MODEL::BipedObjectSlot a_slot, std::int32_t a_type);

		void ShaderType(RE::NiAVObject* a_object, const RE::BSGeometry* a_template, std::string_view a_path, std::int32_t a_textureType, std::vector<RE::BSFixedString>& a_vec, const std::vector<bool>& a_params);
	}

	namespace TEXTURE
	{
		using Texture = RE::BSTextureSet::Texture;

		inline constexpr std::array<Texture, 9> types{ Texture::kDiffuse,
			Texture::kNormal,
			Texture::kEnvironmentMask,
			Texture::kGlowMap,
			Texture::kHeight,
			Texture::kEnvironment,
			Texture::kMultilayer,
			Texture::kBacklightMask,
			Texture::kUnused08 };  //to iterate over enum

		void sanitize_path(std::string& a_path);

		RE::BSShaderTextureSet* create_textureset(char** a_value);
	}
}
