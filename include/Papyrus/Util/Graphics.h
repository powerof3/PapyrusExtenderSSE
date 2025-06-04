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
		inline constexpr frozen::bidirectional_map<RE::MATERIAL_ID, std::string_view, 89> materialMap = {
			{ RE::MATERIAL_ID::kNone, "None"sv },
			{ RE::MATERIAL_ID::kStoneBroken, "StoneBroken"sv },
			{ RE::MATERIAL_ID::kBlockBlade1Hand, "BlockBlade1Hand"sv },
			{ RE::MATERIAL_ID::kMeat, "Meat"sv },
			{ RE::MATERIAL_ID::kCarriageWheel, "CarriageWheel"sv },
			{ RE::MATERIAL_ID::kMetalLight, "MetalLight"sv },
			{ RE::MATERIAL_ID::kWoodLight, "WoodLight"sv },
			{ RE::MATERIAL_ID::kSnow, "Snow"sv },
			{ RE::MATERIAL_ID::kGravel, "Gravel"sv },
			{ RE::MATERIAL_ID::kChainMetal, "ChainMetal"sv },
			{ RE::MATERIAL_ID::kBottle, "Bottle"sv },
			{ RE::MATERIAL_ID::kWood, "Wood"sv },
			{ RE::MATERIAL_ID::kAsh, "Ash"sv },
			{ RE::MATERIAL_ID::kSkin, "Skin"sv },
			{ RE::MATERIAL_ID::kBlockBlunt, "BlockBlunt"sv },
			{ RE::MATERIAL_ID::kDLC1DeerSkin, "DLC1DeerSkin"sv },
			{ RE::MATERIAL_ID::kInsect, "Insect"sv },
			{ RE::MATERIAL_ID::kBarrel, "Barrel"sv },
			{ RE::MATERIAL_ID::kCeramicMedium, "CeramicMedium"sv },
			{ RE::MATERIAL_ID::kBasket, "Basket"sv },
			{ RE::MATERIAL_ID::kIce, "Ice"sv },
			{ RE::MATERIAL_ID::kGlassStairs, "GlassStairs"sv },
			{ RE::MATERIAL_ID::kStoneStairs, "StoneStairs"sv },
			{ RE::MATERIAL_ID::kWater, "Water"sv },
			{ RE::MATERIAL_ID::kDraugrSkeleton, "DraugrSkeleton"sv },
			{ RE::MATERIAL_ID::kBlade1Hand, "Blade1Hand"sv },
			{ RE::MATERIAL_ID::kBook, "Book"sv },
			{ RE::MATERIAL_ID::kCarpet, "Carpet"sv },
			{ RE::MATERIAL_ID::kMetalSolid, "MetalSolid"sv },
			{ RE::MATERIAL_ID::kAxe1Hand, "Axe1Hand"sv },
			{ RE::MATERIAL_ID::kBlockBlade2Hand, "BlockBlade2Hand"sv },
			{ RE::MATERIAL_ID::kOrganicLarge, "OrganicLarge"sv },
			{ RE::MATERIAL_ID::kAmulet, "Amulet"sv },
			{ RE::MATERIAL_ID::kWoodStairs, "WoodStairs"sv },
			{ RE::MATERIAL_ID::kMud, "Mud"sv },
			{ RE::MATERIAL_ID::kBoulderSmall, "BoulderSmall"sv },
			{ RE::MATERIAL_ID::kSnowStairs, "SnowStairs"sv },
			{ RE::MATERIAL_ID::kStoneHeavy, "StoneHeavy"sv },
			{ RE::MATERIAL_ID::kDragonSkeleton, "DragonSkeleton"sv },
			{ RE::MATERIAL_ID::kTrap, "Trap"sv },
			{ RE::MATERIAL_ID::kBowsStaves, "BowsStaves"sv },
			{ RE::MATERIAL_ID::kAlduin, "Alduin"sv },
			{ RE::MATERIAL_ID::kBlockBowsStaves, "BlockBowsStaves"sv },
			{ RE::MATERIAL_ID::kWoodAsStairs, "WoodAsStairs"sv },
			{ RE::MATERIAL_ID::kSteelGreatSword, "SteelGreatSword"sv },
			{ RE::MATERIAL_ID::kGrass, "Grass"sv },
			{ RE::MATERIAL_ID::kBoulderLarge, "BoulderLarge"sv },
			{ RE::MATERIAL_ID::kStoneAsStairs, "StoneAsStairs"sv },
			{ RE::MATERIAL_ID::kBlade2Hand, "Blade2Hand"sv },
			{ RE::MATERIAL_ID::kBottleSmall, "BottleSmall"sv },
			{ RE::MATERIAL_ID::kBoneActor, "BoneActor"sv },
			{ RE::MATERIAL_ID::kSand, "Sand"sv },
			{ RE::MATERIAL_ID::kMetalHeavy, "MetalHeavy"sv },
			{ RE::MATERIAL_ID::kDLC1SabreCatPelt, "DLC1SabreCatPelt"sv },
			{ RE::MATERIAL_ID::kIceForm, "IceForm"sv },
			{ RE::MATERIAL_ID::kDragon, "Dragon"sv },
			{ RE::MATERIAL_ID::kBlade1HandSmall, "Blade1HandSmall"sv },
			{ RE::MATERIAL_ID::kSkinSmall, "SkinSmall"sv },
			{ RE::MATERIAL_ID::kPotsPans, "PotsPans"sv },
			{ RE::MATERIAL_ID::kSkinSkeleton, "SkinSkeleton"sv },
			{ RE::MATERIAL_ID::kBlunt1Hand, "Blunt1Hand"sv },
			{ RE::MATERIAL_ID::kStoneStairsBroken, "StoneStairsBroken"sv },
			{ RE::MATERIAL_ID::kSkinLarge, "SkinLarge"sv },
			{ RE::MATERIAL_ID::kOrganic, "Organic"sv },
			{ RE::MATERIAL_ID::kBone, "Bone"sv },
			{ RE::MATERIAL_ID::kWoodHeavy, "WoodHeavy"sv },
			{ RE::MATERIAL_ID::kChain, "Chain"sv },
			{ RE::MATERIAL_ID::kDirt, "Dirt"sv },
			{ RE::MATERIAL_ID::kGhost, "Ghost"sv },
			{ RE::MATERIAL_ID::kSkinMetalLarge, "SkinMetalLarge"sv },
			{ RE::MATERIAL_ID::kBlockAxe, "BlockAxe"sv },
			{ RE::MATERIAL_ID::kArmorLight, "ArmorLight"sv },
			{ RE::MATERIAL_ID::kShieldLight, "ShieldLight"sv },
			{ RE::MATERIAL_ID::kCoin, "Coin"sv },
			{ RE::MATERIAL_ID::kBlockBlunt2Hand, "BlockBlunt2Hand"sv },
			{ RE::MATERIAL_ID::kShieldHeavy, "ShieldHeavy"sv },
			{ RE::MATERIAL_ID::kArmorHeavy, "ArmorHeavy"sv },
			{ RE::MATERIAL_ID::kArrow, "Arrow"sv },
			{ RE::MATERIAL_ID::kGlass, "Glass"sv },
			{ RE::MATERIAL_ID::kStone, "Stone"sv },
			{ RE::MATERIAL_ID::kWaterPuddle, "WaterPuddle"sv },
			{ RE::MATERIAL_ID::kCloth, "Cloth"sv },
			{ RE::MATERIAL_ID::kSkinMetalSmall, "SkinMetalSmall"sv },
			{ RE::MATERIAL_ID::kWard, "Ward"sv },
			{ RE::MATERIAL_ID::kWeb, "Web"sv },
			{ RE::MATERIAL_ID::kTrailerSteelSword, "TrailerSteelSword"sv },
			{ RE::MATERIAL_ID::kBlunt2Hand, "Blunt2Hand"sv },
			{ RE::MATERIAL_ID::kDLC1SwingingBridge, "DLC1SwingingBridge"sv },
			{ RE::MATERIAL_ID::kBoulderMedium, "BoulderMedium"sv }
		};

		RE::MATERIAL_ID  get_material(const std::string& a_mat);
		std::string_view get_material(RE::MATERIAL_ID a_mat);

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

		inline constexpr std::array<Texture, 9> types{
			Texture::kDiffuse,
			Texture::kNormal,
			Texture::kEnvironmentMask,
			Texture::kGlowMap,
			Texture::kHeight,
			Texture::kEnvironment,
			Texture::kMultilayer,
			Texture::kBacklightMask,
			Texture::kUnused08
		};  //to iterate over enum

		void sanitize_path(std::string& a_path);

		RE::BSShaderTextureSet* create_textureset(char** a_value);
	}
}
