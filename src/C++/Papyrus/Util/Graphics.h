#pragma once

namespace COLOR
{
	enum class BLEND_MODE : std::uint32_t
	{
		kDarken = 0,
		kMultiply,
		kColorBurn,
		kLinearBurn,
		kDarkerColor,

		kLighten,
		kScreen,
		kColorDodge,
		kLinearDodge,
		kLighterColor,

		kOverlay,
		kSoftLight,
		kHardLight,
		kVividLight,
		kLinearLight,
		kPinLight,
		kHardMix,

		kDifference,
		kExclusion,
		kSubtract,
		kDivide
	};

	namespace detail
	{
		//https://www.shadertoy.com/view/XdS3RW
		//ben, 2013

		//UTILITY
		inline RE::NiColor abs(const RE::NiColor& a_rhs)
		{
			return RE::NiColor(std::abs(a_rhs.red),
				std::abs(a_rhs.green),
				std::abs(a_rhs.blue));
		}

		inline RE::NiColor clamp(const RE::NiColor& a_rhs, float a_min, float a_max)
		{
			return RE::NiColor(std::clamp(a_rhs.red, a_min, a_max),
				std::clamp(a_rhs.green, a_min, a_max),
				std::clamp(a_rhs.blue, a_min, a_max));
		}

		inline RE::NiColor floor(const RE::NiColor& a_rhs)
		{
			return RE::NiColor(std::floor(a_rhs.red),
				std::floor(a_rhs.green),
				std::floor(a_rhs.blue));
		}

		inline RE::NiColor min(const RE::NiColor& a_lhs, const RE::NiColor& a_rhs)
		{
			return RE::NiColor(
				std::min(a_lhs.red, a_rhs.red),
				std::min(a_lhs.green, a_rhs.green),
				std::min(a_lhs.blue, a_rhs.blue));
		}

		inline RE::NiColor max(const RE::NiColor& a_lhs, const RE::NiColor& a_rhs)
		{
			return RE::NiColor(
				std::max(a_lhs.red, a_rhs.red),
				std::max(a_lhs.green, a_rhs.green),
				std::max(a_lhs.blue, a_rhs.blue));
		}

		// BLEND MODES

		inline RE::NiColor darken(const RE::NiColor& a_src, const RE::NiColor& a_dest)
		{
			return min(a_src, a_dest);
		}

		inline RE::NiColor multiply(const RE::NiColor& a_src, const RE::NiColor& a_dest)
		{
			return a_src * a_dest;
		}

		inline RE::NiColor colorBurn(const RE::NiColor& a_src, const RE::NiColor& a_dest)
		{
			return 1.0 - (1.0 - a_dest) / a_src;
		}

		inline RE::NiColor linearBurn(const RE::NiColor& a_src, const RE::NiColor& a_dest)
		{
			return a_src + a_dest - 1.0;
		}

		inline RE::NiColor darkerColor(const RE::NiColor& a_src, const RE::NiColor& a_dest)
		{
			return (a_src.red + a_src.green + a_src.blue < a_dest.red + a_dest.green + a_dest.blue) ? a_src : a_dest;
		}

		inline RE::NiColor lighten(const RE::NiColor& a_src, const RE::NiColor& a_dest)
		{
			return max(a_src, a_dest);
		}

		inline RE::NiColor screen(const RE::NiColor& a_src, const RE::NiColor& a_dest)
		{
			return a_src + a_dest - a_src * a_dest;
		}

		inline RE::NiColor colorDodge(const RE::NiColor& a_src, const RE::NiColor& a_dest)
		{
			return a_dest / (1.0 - a_src);
		}

		inline RE::NiColor linearDodge(const RE::NiColor& a_src, const RE::NiColor& a_dest)
		{
			return a_src + a_dest;
		}

		inline RE::NiColor lighterColor(const RE::NiColor& a_src, const RE::NiColor& a_dest)
		{
			return (a_src.red + a_src.green + a_src.blue > a_dest.red + a_dest.green + a_dest.blue) ? a_src : a_dest;
		}

		inline float overlay(float a_src, float a_dest)
		{
			return (a_dest < 0.5f) ? 2.0f * a_src * a_dest : 1.0f - 2.0f * (1.0f - a_src) * (1.0f - a_dest);
		}

		inline RE::NiColor overlay(const RE::NiColor& a_src, const RE::NiColor& a_dest)
		{
			return RE::NiColor(overlay(a_src.red, a_dest.red), overlay(a_src.green, a_dest.green), overlay(a_src.blue, a_dest.blue));
		}

		inline float softLight(float a_src, float a_dest)
		{
			return (a_src < 0.5f) ? a_dest - (1.0f - 2.0f * a_src) * a_dest * (1.0f - a_dest) : (a_dest < 0.25f) ? a_dest + (2.0f * a_src - 1.0f) * a_dest * ((16.0f * a_dest - 12.0f) * a_dest + 3.0f) :
                                                                                                                   a_dest + (2.0f * a_src - 1.0f) * (sqrt(a_dest) - a_dest);
		}

		inline RE::NiColor softLight(const RE::NiColor& a_src, const RE::NiColor& a_dest)
		{
			return RE::NiColor(softLight(a_src.red, a_dest.red), softLight(a_src.green, a_dest.green), softLight(a_src.blue, a_dest.blue));
		}

		inline float hardLight(float a_src, float a_dest)
		{
			return (a_src < 0.5f) ? 2.0f * a_src * a_dest : 1.0f - 2.0f * (1.0f - a_src) * (1.0f - a_dest);
		}

		inline RE::NiColor hardLight(const RE::NiColor& a_src, const RE::NiColor& a_dest)
		{
			return RE::NiColor(hardLight(a_src.red, a_dest.red), hardLight(a_src.green, a_dest.green), hardLight(a_src.blue, a_dest.blue));
		}

		inline float vividLight(float a_src, float a_dest)
		{
			return (a_src < 0.5f) ? 1.0f - (1.0f - a_dest) / (2.0f * a_src) : a_dest / (2.0f * (1.0f - a_src));
		}

		inline RE::NiColor vividLight(const RE::NiColor& a_src, const RE::NiColor& a_dest)
		{
			return RE::NiColor(vividLight(a_src.red, a_dest.red), vividLight(a_src.green, a_dest.green), vividLight(a_src.blue, a_dest.blue));
		}

		inline RE::NiColor linearLight(const RE::NiColor& a_src, const RE::NiColor& a_dest)
		{
			return 2.0 * a_src + a_dest - 1.0;
		}

		inline float pinLight(float a_src, float a_dest)
		{
			return (2.0f * a_src - 1.0f > a_dest) ? 2.0f * a_src - 1.0f : (a_src < 0.5f * a_dest) ? 2.0f * a_src :
                                                                                                    a_dest;
		}

		inline RE::NiColor pinLight(const RE::NiColor& a_src, const RE::NiColor& a_dest)
		{
			return RE::NiColor(pinLight(a_src.red, a_dest.red), pinLight(a_src.green, a_dest.green), pinLight(a_src.blue, a_dest.blue));
		}

		inline RE::NiColor hardMix(const RE::NiColor& a_src, const RE::NiColor& a_dest)
		{
			return floor(a_src + a_dest);
		}

		inline RE::NiColor difference(const RE::NiColor& a_src, const RE::NiColor& a_dest)
		{
			return abs(a_dest - a_src);
		}

		inline RE::NiColor exclusion(const RE::NiColor& a_src, const RE::NiColor& a_dest)
		{
			return a_src + a_dest - 2.0 * a_src * a_dest;
		}

		inline RE::NiColor subtract(const RE::NiColor& a_src, const RE::NiColor& a_dest)
		{
			return a_src - a_dest;
		}

		inline RE::NiColor divide(const RE::NiColor& a_src, const RE::NiColor& a_dest)
		{
			return a_src / a_dest;
		}

		inline RE::NiColor mix(const RE::NiColor& a_src, const RE::NiColor& a_dest, float a_alpha)
		{
			return a_src * (1 - a_alpha) + a_dest * a_alpha;
		}
	}

	RE::NiColor Blend(const RE::NiColor& a_src, const RE::NiColor& a_dest, BLEND_MODE a_mode, float a_alpha);

	float CalcLuminance(const RE::NiColor& a_src);

	RE::NiColor Mix(const RE::NiColor& a_src, const RE::NiColor& a_dest, float a_percentage);
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
	using MAT = RE::MATERIAL_ID;
	inline constexpr frozen::map<MAT, std::string_view, 88> materialMap = {
		{ MAT::kNone, "None"sv },
		{ MAT::kStoneBroken, "StoneBroken"sv },
		{ MAT::kBlockBlade1Hand, "BlockBlade1Hand"sv },
		{ MAT::kMeat, "Meat"sv },
		{ MAT::kCarriageWheel, "CarriageWheel"sv },
		{ MAT::kMetalLight, "MetalLight"sv },
		{ MAT::kWoodLight, "WoodLight"sv },
		{ MAT::kSnow, "Snow"sv },
		{ MAT::kGravel, "Gravel"sv },
		{ MAT::kChainMetal, "ChainMetal"sv },
		{ MAT::kBottle, "Bottle"sv },
		{ MAT::kWood, "Wood"sv },
		{ MAT::kAsh, "Ash"sv },
		{ MAT::kSkin, "Skin"sv },
		{ MAT::kBlockBlunt, "BlockBlunt"sv },
		{ MAT::kDLC1DeerSkin, "DLC1DeerSkin"sv },
		{ MAT::kInsect, "Insect"sv },
		{ MAT::kBarrel, "Barrel"sv },
		{ MAT::kCeramicMedium, "CeramicMedium"sv },
		{ MAT::kBasket, "Basket"sv },
		{ MAT::kIce, "Ice"sv },
		{ MAT::kGlassStairs, "GlassStairs"sv },
		{ MAT::kStoneStairs, "StoneStairs"sv },
		{ MAT::kWater, "Water"sv },
		{ MAT::kDraugrSkeleton, "DraugrSkeleton"sv },
		{ MAT::kBlade1Hand, "Blade1Hand"sv },
		{ MAT::kBook, "Book"sv },
		{ MAT::kCarpet, "Carpet"sv },
		{ MAT::kMetalSolid, "MetalSolid"sv },
		{ MAT::kAxe1Hand, "Axe1Hand"sv },
		{ MAT::kBlockBlade2Hand, "BlockBlade2Hand"sv },
		{ MAT::kOrganicLarge, "OrganicLarge"sv },
		{ MAT::kAmulet, "Amulet"sv },
		{ MAT::kWoodStairs, "WoodStairs"sv },
		{ MAT::kMud, "Mud"sv },
		{ MAT::kBoulderSmall, "BoulderSmall"sv },
		{ MAT::kSnowStairs, "SnowStairs"sv },
		{ MAT::kStoneHeavy, "StoneHeavy"sv },
		{ MAT::kDragonSkeleton, "DragonSkeleton"sv },
		{ MAT::kTrap, "Trap"sv },
		{ MAT::kBowsStaves, "BowsStaves"sv },
		{ MAT::kAlduin, "Alduin"sv },
		{ MAT::kBlockBowsStaves, "BlockBowsStaves"sv },
		{ MAT::kWoodAsStairs, "WoodAsStairs"sv },
		{ MAT::kSteelGreatSword, "SteelGreatSword"sv },
		{ MAT::kGrass, "Grass"sv },
		{ MAT::kBoulderLarge, "BoulderLarge"sv },
		{ MAT::kStoneAsStairs, "StoneAsStairs"sv },
		{ MAT::kBlade2Hand, "Blade2Hand"sv },
		{ MAT::kBottleSmall, "BottleSmall"sv },
		{ MAT::kBoneActor, "BoneActor"sv },
		{ MAT::kSand, "Sand"sv },
		{ MAT::kMetalHeavy, "MetalHeavy"sv },
		{ MAT::kDLC1SabreCatPelt, "DLC1SabreCatPelt"sv },
		{ MAT::kIceForm, "IceForm"sv },
		{ MAT::kDragon, "Dragon"sv },
		{ MAT::kBlade1HandSmall, "Blade1HandSmall"sv },
		{ MAT::kSkinSmall, "SkinSmall"sv },
		{ MAT::kPotsPans, "PotsPans"sv },
		{ MAT::kSkinSkeleton, "SkinSkeleton"sv },
		{ MAT::kBlunt1Hand, "Blunt1Hand"sv },
		{ MAT::kStoneStairsBroken, "StoneStairsBroken"sv },
		{ MAT::kSkinLarge, "SkinLarge"sv },
		{ MAT::kOrganic, "Organic"sv },
		{ MAT::kBone, "Bone"sv },
		{ MAT::kWoodHeavy, "WoodHeavy"sv },
		{ MAT::kChain, "Chain"sv },
		{ MAT::kDirt, "Dirt"sv },
		{ MAT::kGhost, "Ghost"sv },
		{ MAT::kSkinMetalLarge, "SkinMetalLarge"sv },
		{ MAT::kBlockAxe, "BlockAxe"sv },
		{ MAT::kArmorLight, "ArmorLight"sv },
		{ MAT::kShieldLight, "ShieldLight"sv },
		{ MAT::kCoin, "Coin"sv },
		{ MAT::kBlockBlunt2Hand, "BlockBlunt2Hand"sv },
		{ MAT::kShieldHeavy, "ShieldHeavy"sv },
		{ MAT::kArmorHeavy, "ArmorHeavy"sv },
		{ MAT::kArrow, "Arrow"sv },
		{ MAT::kGlass, "Glass"sv },
		{ MAT::kStone, "Stone"sv },
		{ MAT::kWaterPuddle, "WaterPuddle"sv },
		{ MAT::kCloth, "Cloth"sv },
		{ MAT::kSkinMetalSmall, "SkinMetalSmall"sv },
		{ MAT::kWard, "Ward"sv },
		{ MAT::kWeb, "Web"sv },
		{ MAT::kTrailerSteelSword, "TrailerSteelSword"sv },
		{ MAT::kBlunt2Hand, "Blunt2Hand"sv },
		{ MAT::kDLC1SwingingBridge, "DLC1SwingingBridge"sv },
		{ MAT::kBoulderMedium, "BoulderMedium"sv },
	};

	std::string_view get_material(MAT a_mat);
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
			const auto newData = T::Create(a_type, a_data);
			if (newData) {
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

	inline constexpr std::array<Texture, 9> types{ Texture::kDiffuse, Texture::kNormal, Texture::kEnvironmentMask, Texture::kGlowMap, Texture::kHeight, Texture::kEnvironment, Texture::kMultilayer, Texture::kBacklightMask, Texture::kUnused08 };  //to iterate over enum

	void sanitize_path(std::string& a_path);

	RE::BSShaderTextureSet* create_textureset(char** a_value);
}
