#include "Papyrus/Util/Magic.h"

std::string_view RE::EffectArchetypeToString(EffectArchetype a_archetype) noexcept
{
	switch (a_archetype) {
	case EffectArchetype::kValueModifier:
		return "ValueMod";
	case EffectArchetype::kScript:
		return "Script";
	case EffectArchetype::kDispel:
		return "Dispel";
	case EffectArchetype::kCureDisease:
		return "CureDisease";
	case EffectArchetype::kAbsorb:
		return "Absorb";
	case EffectArchetype::kDualValueModifier:
		return "DualValueMod";
	case EffectArchetype::kCalm:
		return "Calm";
	case EffectArchetype::kDemoralize:
		return "Demoralize";
	case EffectArchetype::kFrenzy:
		return "Frenzy";
	case EffectArchetype::kDisarm:
		return "Disarm";
	case EffectArchetype::kCommandSummoned:
		return "CommandSummoned";
	case EffectArchetype::kInvisibility:
		return "Invisibility";
	case EffectArchetype::kLight:
		return "Light";
	case EffectArchetype::kDarkness:
		return "Darkness";
	case EffectArchetype::kNightEye:
		return "NightEye";
	case EffectArchetype::kLock:
		return "Lock";
	case EffectArchetype::kOpen:
		return "Open";
	case EffectArchetype::kBoundWeapon:
		return "BoundWeapon";
	case EffectArchetype::kSummonCreature:
		return "SummonCreature";
	case EffectArchetype::kDetectLife:
		return "DetectLife";
	case EffectArchetype::kTelekinesis:
		return "Telekinesis";
	case EffectArchetype::kParalysis:
		return "Paralysis";
	case EffectArchetype::kReanimate:
		return "Reanimate";
	case EffectArchetype::kSoulTrap:
		return "SoulTrap";
	case EffectArchetype::kTurnUndead:
		return "TurnUndead";
	case EffectArchetype::kGuide:
		return "Guide";
	case EffectArchetype::kWerewolfFeed:
		return "WerewolfFeed";
	case EffectArchetype::kCureParalysis:
		return "CureParalysis";
	case EffectArchetype::kCureAddiction:
		return "CureAddiction";
	case EffectArchetype::kCurePoison:
		return "CurePoison";
	case EffectArchetype::kConcussion:
		return "Concussion";
	case EffectArchetype::kValueAndParts:
		return "ValueAndParts";
	case EffectArchetype::kAccumulateMagnitude:
		return "AccumulateMagnitude";
	case EffectArchetype::kStagger:
		return "Stagger";
	case EffectArchetype::kPeakValueModifier:
		return "PeakValueMod";
	case EffectArchetype::kCloak:
		return "Cloak";
	case EffectArchetype::kWerewolf:
		return "Werewolf";
	case EffectArchetype::kSlowTime:
		return "SlowTime";
	case EffectArchetype::kRally:
		return "Rally";
	case EffectArchetype::kEnhanceWeapon:
		return "EnhanceWeapon";
	case EffectArchetype::kSpawnHazard:
		return "SpawnHazard";
	case EffectArchetype::kEtherealize:
		return "Etherealize";
	case EffectArchetype::kBanish:
		return "Banish";
	case EffectArchetype::kSpawnScriptedRef:
		return "SpawnScriptedRef";
	case EffectArchetype::kDisguise:
		return "Disguise";
	case EffectArchetype::kGrabActor:
		return "GrabActor";
	case EffectArchetype::kVampireLord:
		return "VampireLord";
	default:
		return "None";
	}
}

void MAGIC::RemoveMagicEffect(VM* a_vm, StackID a_stackID, RE::MagicItem* a_spell, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost)
{
	if (!a_mgef) {
		a_vm->TraceStack("MagicEffect is None", a_stackID);
		return;
	}

	MGEFData data(a_mgef, a_mag, a_area, a_dur, a_cost);
	if (Manager::GetSingleton()->mgefs.Remove(a_spell, data)) {
		a_vm->TraceForm(a_spell, "Failed to remove magic effect", a_stackID);
	}
}

void MAGIC::RemoveEffectItem(VM* a_vm, StackID a_stackID, RE::MagicItem* a_spell, RE::MagicItem* a_copySpell, std::uint32_t a_index, float a_cost)
{
	if (!a_spell) {
		a_vm->TraceStack("Spell is None", a_stackID);
		return;
	}
	if (!a_copySpell) {
		a_vm->TraceStack("Copy Spell is None", a_stackID);
		return;
	}
	if (a_index > a_copySpell->effects.size()) {
		a_vm->TraceStack("Copy Spell index exceeds effect list size", a_stackID);
		return;
	}

	EffectData data(a_copySpell, a_index, a_cost);
	if (Manager::GetSingleton()->effectItems.Remove(a_spell, data)) {
		a_vm->TraceForm(a_spell, "Failed to remove magic effect", a_stackID);
	}
}

void MAGIC::SetMagicEffect(VM* a_vm, StackID a_stackID, RE::MagicItem* a_spell, RE::EffectSetting* a_magicEffect, std::uint32_t a_index)
{
	if (!a_magicEffect) {
		a_vm->TraceStack("MagicEffect is None", a_stackID);
		return;
	}

	if (a_index > a_spell->effects.size()) {
		a_vm->TraceForm(a_spell, "Index exceeds effect list size", a_stackID);
		return;
	}

	if (auto effectItem = a_spell->effects[a_index]) {
		effectItem->baseEffect = a_magicEffect;
	}
}
