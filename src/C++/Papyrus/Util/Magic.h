#pragma once

namespace MAGIC
{
	using Archetype = RE::EffectArchetypes::ArchetypeID;

	enum class RESULT
	{
		kSuccess = 0,
		kFailExists = 1,
		kFailParse = 2
	};

	inline constexpr auto namedNode = "NamedNode&"sv;

	inline constexpr frozen::map<Archetype, std::string_view, 46> archetypeMap = {
		{ Archetype::kValueModifier, "ValueMod"sv },
		{ Archetype::kScript, "Script"sv },
		{ Archetype::kDispel, "Dispel"sv },
		{ Archetype::kCureDisease, "CureDisease"sv },
		{ Archetype::kAbsorb, "Absorb"sv },
		{ Archetype::kDualValueModifier, "DualValueMod"sv },
		{ Archetype::kCalm, "Calm"sv },
		{ Archetype::kDemoralize, "Demoralize"sv },
		{ Archetype::kFrenzy, "Frenzy"sv },
		{ Archetype::kDisarm, "Disarm"sv },
		{ Archetype::kCommandSummoned, "CommandSummoned"sv },
		{ Archetype::kInvisibility, "Invisibility"sv },
		{ Archetype::kLight, "Light"sv },
		{ Archetype::kDarkness, "Darkness"sv },
		{ Archetype::kNightEye, "NightEye"sv },
		{ Archetype::kLock, "Lock"sv },
		{ Archetype::kOpen, "Open"sv },
		{ Archetype::kBoundWeapon, "BoundWeapon"sv },
		{ Archetype::kSummonCreature, "SummonCreature"sv },
		{ Archetype::kDetectLife, "DetectLife"sv },
		{ Archetype::kTelekinesis, "Telekinesis"sv },
		{ Archetype::kParalysis, "Paralysis"sv },
		{ Archetype::kReanimate, "Reanimate"sv },
		{ Archetype::kSoulTrap, "SoulTrap"sv },
		{ Archetype::kTurnUndead, "TurnUndead"sv },
		{ Archetype::kGuide, "Guide"sv },
		{ Archetype::kWerewolfFeed, "WerewolfFeed"sv },
		{ Archetype::kCureParalysis, "CureParalysis"sv },
		{ Archetype::kCureAddiction, "CureAddiction"sv },
		{ Archetype::kCurePoison, "CurePoison"sv },
		{ Archetype::kConcussion, "Concussion"sv },
		{ Archetype::kValueAndParts, "ValueAndParts"sv },
		{ Archetype::kAccumulateMagnitude, "AccumulateMagnitude"sv },
		{ Archetype::kStagger, "Stagger"sv },
		{ Archetype::kPeakValueModifier, "PeakValueMod"sv },
		{ Archetype::kCloak, "Cloak"sv },
		{ Archetype::kWerewolf, "Werewolf"sv },
		{ Archetype::kSlowTime, "SlowTime"sv },
		{ Archetype::kRally, "Rally"sv },
		{ Archetype::kEnhanceWeapon, "EnhanceWeapon"sv },
		{ Archetype::kSpawnHazard, "SpawnHazard"sv },
		{ Archetype::kEtherealize, "Etherealize"sv },
		{ Archetype::kBanish, "Banish"sv },
		{ Archetype::kSpawnScriptedRef, "SpawnScriptedRef"sv },
		{ Archetype::kDisguise, "Disguise"sv },
		{ Archetype::kGrabActor, "GrabActor"sv },
		{ Archetype::kVampireLord, "VampireLord"sv },
	};

	std::string_view get_archetype_as_string(Archetype a_archetype);

	void add_effect_item(RE::MagicItem* a_item, RE::MagicItem* a_copyItem, std::uint32_t a_index, float a_cost);

	RESULT add_magic_effect(RE::MagicItem* a_item, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost, const std::vector<RE::BSFixedString>& a_conditionList);

	void remove_effect_item(RE::MagicItem* a_item, RE::MagicItem* a_copyItem, std::uint32_t a_index);

	void remove_magic_effect(RE::MagicItem* a_item, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost);
}
