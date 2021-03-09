#include "Papyrus/Actor.h"

#include "Serialization/Events.h"
#include "Serialization/Form/Perks.h"
#include "Util/GraphicsReset.h"
#include "Util/VMErrors.h"


auto papyrusActor::AddBasePerk(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSPerk* a_perk) -> bool
{
	using namespace Serialization::Form;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return false;
	}
	if (!a_perk) {
		a_vm->TraceStack("Perk is None", a_stackID, Severity::kWarning);
		return false;
	}

	return Perks::GetSingleton()->PapyrusApply(a_actor, a_perk, kAdd);
}


auto papyrusActor::AddBaseSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::SpellItem* a_spell) -> bool
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return false;
	}
	if (!a_spell) {
		a_vm->TraceStack("Spell is None", a_stackID, Severity::kWarning);
		return false;
	}
	if (a_actor->HasSpell(a_spell)) {
		a_vm->TraceStack(VMError::generic_error(a_actor, "already has spell"sv).c_str(), a_stackID, Severity::kWarning);
		return false;
	}

	auto actorbase = a_actor->GetActorBase();
	if (actorbase) {
		if (auto actorEffects = actorbase->GetOrCreateSpellList(); actorEffects) {
			if (actorEffects->AddSpell(a_spell)) {
				const auto combatController = a_actor->combatController;
				if (combatController) {
					combatController->data10->unk1C4 = 1;
				}
				if (actorbase->IsPlayer() || a_spell->GetSpellType() == RE::MagicSystem::SpellType::kLeveledSpell) {
					RE::SpellsLearned::SendEvent(a_spell);
				}
				return true;
			}
		}
	}

	return false;
}


auto papyrusActor::AddAllEquippedItemsToArray(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor) -> std::vector<RE::TESForm*>
{
	std::vector<RE::TESForm*> vec;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return vec;
	}

	auto inv = a_actor->GetInventory();
	for (auto& [item, data] : inv) {
		auto& [count, entry] = data;
		if (count > 0 && entry->GetWorn()) {
			vec.push_back(item);
		}
	}

	return vec;
}


void TintFace(RE::Actor* a_actor, const RE::NiColor& a_color)
{
	using HeadPartType = RE::BGSHeadPart::HeadPartType;

	if (auto object = a_actor->GetHeadPartObject(HeadPartType::kFace); object) {
		auto geometry = object->AsGeometry();
		if (geometry) {
			geometry->SwitchToFaceTint();
			geometry->UpdateBodyTint(a_color);
		}
	}
}


void AddOrUpdateColorData(RE::NiAVObject* a_root, const RE::BSFixedString& a_name, const RE::NiColor& a_color)
{
	auto data = a_root->GetExtraData<RE::NiIntegerExtraData>(a_name);
	if (data) {
		if (a_color != RE::NiColor(data->value)) {
			data->value = RE::NiColor::ColorToInt(a_color);
		}
	} else {
		auto newData = RE::NiIntegerExtraData::Create(a_name, RE::NiColor::ColorToInt(a_color));
		if (newData) {
			a_root->AddExtraData(newData);
		}
	}
}


void papyrusActor::BlendColorWithSkinTone(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSColorForm* a_color, std::uint32_t a_blendMode, bool a_autoCalc, float a_opacity)
{
	using BLEND_MODE = RE::NiColor::BLEND_MODE;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_color) {
		a_vm->TraceStack("Colorform is None", a_stackID, Severity::kWarning);
		return;
	}

	auto root = a_actor->Get3D(false);
	if (!root) {
		a_vm->TraceStack(VMError::no_3D(a_actor).c_str(), a_stackID, Severity::kWarning);
		return;
	}

	const auto actorbase = a_actor->GetActorBase();
	if (actorbase) {
		const float opacity = a_autoCalc ? std::clamp(a_opacity * RE::NiColor::CalcLuminance(actorbase->bodyTintColor), 0.0f, 1.0f) : a_opacity;
		auto newColor = RE::NiColor::Blend(actorbase->bodyTintColor, a_color->color, static_cast<BLEND_MODE>(a_blendMode), opacity);

		auto task = SKSE::GetTaskInterface();
		task->AddTask([a_actor, newColor, root]() {
			TintFace(a_actor, newColor);
			root->UpdateBodyTint(newColor);
			AddOrUpdateColorData(root, "PO3_SKINTINT", newColor);
		});
	}
}


void papyrusActor::DecapitateActor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return;
	}

	auto task = SKSE::GetTaskInterface();
	task->AddTask([a_actor]() {
		a_actor->Decapitate();
	});
}


void papyrusActor::EquipItem_Script(VM* a_vm, StackID a_stackID, RE::Actor* a_actor, RE::TESForm* a_form, bool a_forceEquip, bool a_silent)
{
	using func_t = decltype(&EquipItem_Script);
	REL::Relocation<func_t> func{ REL::ID(53861) };
	return func(a_vm, a_stackID, a_actor, a_form, a_forceEquip, a_silent);
}


void papyrusActor::EquipArmorIfSkinVisible(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::TESObjectARMO* a_check, RE::TESObjectARMO* a_equip)
{
	using Feature = RE::BSShaderMaterial::Feature;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_check) {
		a_vm->TraceStack("ArmorToCheck is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_equip) {
		a_vm->TraceStack("ArmorToEquip is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_actor->Is3DLoaded()) {
		a_vm->TraceStack(VMError::no_3D(a_actor).c_str(), a_stackID, Severity::kWarning);
		return;
	}

	if (auto arma = a_check->GetArmorAddon(a_actor->GetRace()); arma) {
		auto task = SKSE::GetTaskInterface();
		task->AddTask([a_vm, a_stackID, a_actor, a_equip, a_check, arma]() {
			if (auto armorObject = a_actor->VisitArmorAddon(a_check, arma); armorObject && armorObject->HasShaderType(Feature::kFaceGenRGBTint)) {
				EquipItem_Script(a_vm, a_stackID, a_actor, a_equip, false, false);
			}
		});
	}
}


void papyrusActor::FreezeActor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, std::uint32_t a_type, bool a_enable)
{
	using Flags = RE::CHARACTER_FLAGS;
	using BOOL_BITS = RE::Actor::BOOL_BITS;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return;
	}

	auto root = a_actor->Get3D(false);
	if (!root) {
		return;
	}

	if (a_type == 0) {
		if (!a_enable) {
			a_actor->boolBits.set(BOOL_BITS::kProcessMe);  //enable AI first
		}
		auto charController = a_actor->GetCharController();
		if (charController) {
			if (a_enable) { //freeze
				charController->flags.set(Flags::kNotPushable);
				charController->flags.reset(Flags::kRecordHits);
				charController->flags.reset(Flags::kHitFlags);
				charController->flags.reset(Flags::kHitDamage);
			} else { //unfreeze
				charController->flags.reset(Flags::kNotPushable);
				charController->flags.set(Flags::kRecordHits);
				charController->flags.set(Flags::kHitFlags);
				charController->flags.set(Flags::kHitDamage);
			}
			charController->SetLinearVelocityImpl(0.0);
		}
		if (a_enable) {
			a_actor->boolBits.reset(BOOL_BITS::kProcessMe); //disable AI last
		}
	} else if (a_type == 1) {
		auto charController = a_actor->GetCharController();
		if (charController) {
			auto task = SKSE::GetTaskInterface();
			task->AddTask([root, charController, a_actor, a_enable]() {
				std::uint32_t unk = 0;
				if (a_enable) {
					a_actor->boolBits.set(BOOL_BITS::kParalyzed);
					const auto flags = *(charController->Unk_08(&unk) + 1);
					root->UpdateRigidBodySettings(32, flags);
					root->SetRigidConstraints(true);
				} else {
					a_actor->boolBits.reset(BOOL_BITS::kParalyzed);
					root->SetRigidConstraints(false);
					const auto flags = *charController->Unk_08(&unk);
					root->UpdateRigidBodySettings(32, flags >> 16);
				}
			});
		}
	}
}


auto papyrusActor::GetActiveEffects(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, bool a_inactive) -> std::vector<RE::EffectSetting*>
{
	using MGEF = RE::EffectSetting::EffectSettingData::Flag;
	using AE = RE::ActiveEffect::Flag;

	std::vector<RE::EffectSetting*> vec;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return vec;
	}

	const auto activeEffects = a_actor->GetActiveEffectList();
	if (!activeEffects) {
		a_vm->TraceStack(VMError::generic_error(a_actor, "has no active effects"sv).c_str(), a_stackID, Severity::kInfo);
		return vec;
	}

	for (auto& activeEffect : *activeEffects) {
		if (activeEffect) {
			if (auto mgef = activeEffect->GetBaseObject(); mgef) {
				if (!a_inactive && (activeEffect->flags.all(AE::kInactive) || activeEffect->flags.all(AE::kDispelled))) {
					continue;
				}
				vec.push_back(mgef);
			}
		}
	}

	return vec;
}


auto papyrusActor::GetActorAlpha(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor) -> float
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return 1.0f;
	}

	if (const auto currentProcess = a_actor->currentProcess; currentProcess) {
		const auto middleProcess = currentProcess->middleHigh;
		if (middleProcess) {
			return middleProcess->alphaMult;
		}
	}

	return 1.0f;
}


auto papyrusActor::GetActorRefraction(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor) -> float
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return 1.0f;
	}

	if (const auto currentProcess = a_actor->currentProcess; currentProcess) {
		const auto middleProcess = currentProcess->middleHigh;
		if (middleProcess) {
			return middleProcess->scriptRefractPower;
		}
	}

	return 1.0f;
}


auto papyrusActor::GetActorSoulSize(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor) -> std::int32_t
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return -1;
	}

	return to_underlying(a_actor->GetSoulLevel());
}


auto papyrusActor::GetActorState(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor) -> std::int32_t
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return -1;
	}

	return to_underlying(a_actor->GetLifeState());
}


auto papyrusActor::GetCriticalStage(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor) -> std::uint32_t
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return 0;
	}

	return to_underlying(a_actor->criticalStage.get());
}


auto papyrusActor::GetCombatAllies(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor) -> std::vector<RE::Actor*>
{
	std::vector<RE::Actor*> vec;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return vec;
	}

	auto combatGroup = a_actor->GetCombatGroup();
	if (!combatGroup) {
		a_vm->TraceStack(VMError::generic_error(a_actor, "does not have a combat group"sv).c_str(), a_stackID, Severity::kWarning);
		return vec;
	}

	for (auto& allyData : combatGroup->allies) {
		auto allyPtr = allyData.allyHandle.get();
		auto ally = allyPtr.get();
		if (ally) {
			vec.push_back(ally);
		}
	}

	return vec;
}


auto papyrusActor::GetCombatTargets(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor) -> std::vector<RE::Actor*>
{
	std::vector<RE::Actor*> vec;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return vec;
	}

	auto combatGroup = a_actor->GetCombatGroup();
	if (!combatGroup) {
		a_vm->TraceStack(VMError::generic_error(a_actor, "does not have a combat group"sv).c_str(), a_stackID, Severity::kWarning);
		return vec;
	}

	for (auto& targetData : combatGroup->targets) {
		auto targetPtr = targetData.targetHandle.get();
		auto target = targetPtr.get();
		if (target) {
			vec.push_back(target);
		}
	}

	return vec;
}


auto papyrusActor::GetDeathEffectType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, std::uint32_t a_type) -> std::vector<std::int32_t>
{
	using FLAG = RE::EffectSetting::EffectSettingData::Flag;
	using AE_FLAG = RE::ActiveEffect::Flag;
	using CAST_TYPE = RE::MagicSystem::CastingType;

	namespace FLOAT = SKSE::UTIL::FLOAT;

	std::vector<std::int32_t> vec;
	vec.resize(3, -1);

	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return vec;
	}

	const auto activeEffects = a_actor->GetActiveEffectList();
	if (!activeEffects) {
		a_vm->TraceStack(VMError::generic_error(a_actor, "has no active effects"sv).c_str(), a_stackID, Severity::kInfo);
		return vec;
	}

	using deathEffectPair = std::pair<std::uint32_t, RE::EffectSetting*>;  //[type, mgef]
	deathEffectPair effectPair = { -1, nullptr };

	using deathEffectMap = std::map<std::uint32_t, std::vector<std::pair<RE::EffectSetting*, float>>>;  // [type, [mgef, mag]]
	deathEffectMap effectMap;

	for (auto& activeEffect : *activeEffects) {
		if (activeEffect && activeEffect->flags.none(AE_FLAG::kInactive) && activeEffect->flags.none(AE_FLAG::kDispelled)) {
			auto mgef = activeEffect->GetBaseObject();
			if (mgef && mgef->data.flags.all(FLAG::kHostile) && mgef->data.flags.all(FLAG::kDetrimental)) {
				if (a_type == 0) {
					if (mgef->HasKeywordString("PO3_MagicDamageSun"sv)) {
						effectPair = { DEATH_TYPE::kSun, mgef };  //sun override
						break;
						/*} else if (mgef->data.resistVariable == RE::ActorValue::kPoisonResist && mgef->data.castingType == CAST_TYPE::kConcentration) {
						effectPair = { DEATH_TYPE::kAcid, mgef };  //acid override
						break;*/
					}
					if (mgef->HasKeywordString("MagicDamageFire"sv)) {
						effectMap[DEATH_TYPE::kFire].emplace_back(mgef, -activeEffect->magnitude);  //flipping the magnitude back to +ve
					} else if (mgef->HasKeywordString("MagicDamageFrost"sv)) {
						effectMap[DEATH_TYPE::kFrost].emplace_back(mgef, -activeEffect->magnitude);
					} else if (mgef->HasKeywordString("MagicDamageShock"sv)) {
						effectMap[DEATH_TYPE::kShock].emplace_back(mgef, -activeEffect->magnitude);
					} else if (mgef->GetArchetype() == RE::Archetype::kAbsorb) {
						effectMap[DEATH_TYPE::kDrain].emplace_back(mgef, -activeEffect->magnitude);
					}
				} else {
					if (mgef->data.resistVariable == RE::ActorValue::kPoisonResist /*&& mgef->data.castingType != CAST_TYPE::kConcentration*/) {
						effectMap[DEATH_TYPE::kPoison].emplace_back(mgef, -activeEffect->magnitude);
					} else if (mgef->GetArchetype() == RE::Archetype::kDemoralize) {
						effectMap[DEATH_TYPE::kFear].emplace_back(mgef, -activeEffect->magnitude);
					} else if (mgef->data.associatedSkill == RE::ActorValue::kAlteration && mgef->HasKeywordString("MagicParalysis"sv)) {
						effectMap[DEATH_TYPE::kAsh].emplace_back(mgef, -activeEffect->magnitude);
					}
				}
			}
		}
	}

	if (effectPair.first == DEATH_TYPE::kNone && !effectMap.empty()) {
		auto mag_cmp = [&](const auto& a_lhs, const auto& a_rhs) {
			return FLOAT::definitelyLessThan(a_lhs.second, a_rhs.second);
		};

		if (effectMap.size() == 1) {
			const auto& type = effectMap.begin()->first;
			auto [effect, effectMag] = *std::max_element(effectMap.begin()->second.begin(), effectMap.begin()->second.end(), mag_cmp);
			effectPair = { type, effect };
		} else {
			if (a_type != 0) {
				bool poison = !effectMap[DEATH_TYPE::kPoison].empty();
				bool fear = !effectMap[DEATH_TYPE::kFear].empty();
				bool ash = !effectMap[DEATH_TYPE::kAsh].empty();

				if (poison) {
					auto& poisonVec = effectMap[DEATH_TYPE::kPoison];
					auto poisonEffect = *std::max_element(poisonVec.begin(), poisonVec.end(), mag_cmp);

					effectPair = { DEATH_TYPE::kPoison, poisonEffect.first };
					if (fear) {
						effectPair.first = DEATH_TYPE::kPoisonFear;
					}
				} else if (ash) {
					auto& ashVec = effectMap[DEATH_TYPE::kAsh];
					auto ashEffect = *std::max_element(ashVec.begin(), ashVec.end(), mag_cmp);

					effectPair = { DEATH_TYPE::kAsh, ashEffect.first };
					if (fear) {
						effectPair.first = DEATH_TYPE::kAshFear;
					}
				} else if (fear) {
					auto& fearVec = effectMap[DEATH_TYPE::kFear];
					auto poisonEffect = *std::max_element(fearVec.begin(), fearVec.end(), mag_cmp);

					effectPair = { DEATH_TYPE::kFear, poisonEffect.first };
				}
			} else {
				bool fire = !effectMap[DEATH_TYPE::kFire].empty();
				bool frost = !effectMap[DEATH_TYPE::kFrost].empty();
				bool shock = !effectMap[DEATH_TYPE::kShock].empty();
				bool drain = !effectMap[DEATH_TYPE::kDrain].empty();

				if (fire) {
					auto& fireVec = effectMap[DEATH_TYPE::kFire];
					auto fireEffect = *std::max_element(fireVec.begin(), fireVec.end(), mag_cmp);

					effectPair = { DEATH_TYPE::kFire, fireEffect.first };
					if (frost) {
						effectPair.first = DEATH_TYPE::kFireFrost;
					} else if (shock) {
						effectPair.first = DEATH_TYPE::kFireShock;
					}
				} else if (drain) {
					auto& drainVec = effectMap[DEATH_TYPE::kDrain];
					auto drainEffect = *std::max_element(drainVec.begin(), drainVec.end(), mag_cmp);

					effectPair = { DEATH_TYPE::kDrain, drainEffect.first };
					if (shock) {
						effectPair.first = DEATH_TYPE::kDrainShock;
						auto& shockVec = effectMap[DEATH_TYPE::kShock];
						auto shockEffect = *std::max_element(shockVec.begin(), shockVec.end(), mag_cmp);

						if (FLOAT::definitelyLessThan(drainEffect.second, shockEffect.second)) {
							effectPair.second = shockEffect.first;
						}
					} else if (frost) {
						effectPair.first = DEATH_TYPE::kDrainFrost;
						auto& frostVec = effectMap[DEATH_TYPE::kFrost];
						auto frostEffect = *std::max_element(frostVec.begin(), frostVec.end(), mag_cmp);

						if (FLOAT::definitelyLessThan(drainEffect.second, frostEffect.second)) {
							effectPair.second = frostEffect.first;
						}
					}
				} else if (frost) {
					auto& frostVec = effectMap[DEATH_TYPE::kFrost];
					auto frostEffect = *std::max_element(frostVec.begin(), frostVec.end(), mag_cmp);

					effectPair = { DEATH_TYPE::kFrost, frostEffect.first };
					if (shock) {
						auto& shockVec = effectMap[DEATH_TYPE::kShock];
						auto shockEffect = *std::max_element(shockVec.begin(), shockVec.end(), mag_cmp);

						if (FLOAT::definitelyLessThan(frostEffect.second, shockEffect.second)) {
							effectPair = { DEATH_TYPE::kShockFrost, shockEffect.first };
						} else {
							effectPair.first = { DEATH_TYPE::kFrostShock };
						}
					}
				} else if (shock) {
					auto& shockVec = effectMap[DEATH_TYPE::kShock];
					auto shockEffect = *std::max_element(shockVec.begin(), shockVec.end(), mag_cmp);

					effectPair = { DEATH_TYPE::kShock, shockEffect.first };
				}
			}
		}
	}

	if (effectPair.first != DEATH_TYPE::kNone) {
		auto& [value, mgef] = effectPair;
		if (mgef) {
			vec[0] = value;
			vec[1] = mgef->GetMinimumSkillLevel();
			if (const auto projectile = mgef->data.projectileBase; projectile) {
				vec[2] = projectile->GetType();
			}
		}
	}

	return vec;
}


auto papyrusActor::GetHairColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor) -> RE::BGSColorForm*
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	const auto root = a_actor->Get3D(false);
	if (root) {
		if (auto data = root->GetExtraData<RE::NiIntegerExtraData>("PO3_HAIRTINT"sv); data) {
			auto factory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::BGSColorForm>();
			auto color = factory->Create();
			if (color) {
				color->flags.reset(RE::BGSColorForm::Flag::kPlayable);
				color->color = RE::Color(data->value);
				return color;
			}
		}
	}

	const auto actorbase = a_actor->GetActorBase();
	if (actorbase) {
		const auto headData = actorbase->headRelatedData;
		if (headData) {
			return headData->hairColor;
		}
	}

	return nullptr;
}


auto papyrusActor::GetHeadPartTextureSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, std::uint32_t a_type) -> RE::BGSTextureSet*
{
	using HeadPartType = RE::BGSHeadPart::HeadPartType;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	auto actorBase = a_actor->GetActorBase();
	if (!actorBase) {
		return nullptr;
	}

	const auto headpart = actorBase->GetCurrentHeadPartByType(static_cast<HeadPartType>(a_type));
	return headpart ? headpart->textureSet : nullptr;
}


auto papyrusActor::GetLocalGravityActor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor) -> float
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return 1.0f;
	}

	const auto charController = a_actor->GetCharController();
	return charController ? charController->gravity : 1.0f;
}


auto papyrusActor::GetObjectUnderFeet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor) -> RE::TESObjectREFR*
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return nullptr;
	}

	if (const auto charProxy = static_cast<RE::bhkCharProxyController*>(a_actor->GetCharController()); charProxy) {
		const auto supportBody = charProxy->supportBody.get();
		if (supportBody) {
			return supportBody->GetUserData();
		}
	}

	return nullptr;
}


auto papyrusActor::GetRunningPackage(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor) -> RE::TESPackage*
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	const auto currentProcess = a_actor->currentProcess;
	return currentProcess ? currentProcess->GetRunningPackage() : nullptr;
}


auto papyrusActor::GetSkinColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor) -> RE::BGSColorForm*
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	const auto actorBase = a_actor->GetActorBase();
	if (actorBase) {
		auto factory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::BGSColorForm>();
		auto color = factory->Create();
		if (color) {
			color->flags.reset(RE::BGSColorForm::Flag::kPlayable);
			color->color = actorBase->bodyTintColor;

			if (const auto root = a_actor->Get3D(false); root) {
				if (const auto data = root->GetExtraData<RE::NiIntegerExtraData>("PO3_SKINTINT"sv); data) {
					color->color = RE::Color(data->value);
				}
			}

			return color;
		}
	}

	return nullptr;
}


auto papyrusActor::GetTimeDead(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor) -> float
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	const auto currentProcess = a_actor->currentProcess;
	if (currentProcess) {
		const auto timeOfDeath = currentProcess->deathTime;
		if (timeOfDeath > 0.0f) {
			const auto calendar = RE::Calendar::GetSingleton();
			if (calendar) {
				const auto g_gameDaysPassed = calendar->gameDaysPassed;
				return g_gameDaysPassed ? floorf(g_gameDaysPassed->value * 24.0f) - timeOfDeath : 0.0f;
			}
		}
	}

	return 0.0f;
}


auto papyrusActor::GetTimeOfDeath(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor) -> float
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	const auto currentProcess = a_actor->currentProcess;
	if (currentProcess) {
		const auto timeOfDeath = currentProcess->deathTime;
		if (timeOfDeath > 0.0f) {
			return timeOfDeath / 24.0f;
		}
	}

	return 0.0f;
}


auto papyrusActor::HasActiveSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::SpellItem* a_spell) -> bool
{
	using AE = RE::ActiveEffect::Flag;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return false;
	}
	if (!a_spell) {
		a_vm->TraceStack("Spell is None", a_stackID, Severity::kWarning);
		return false;
	}

	auto activeEffects = a_actor->GetActiveEffectList();
	if (!activeEffects) {
		a_vm->TraceStack(VMError::generic_error(a_actor, "has no active effects"sv).c_str(), a_stackID, Severity::kInfo);
		return false;
	}

	return std::any_of(activeEffects->begin(), activeEffects->end(), [&](auto const& ae) {
		return ae && ae->spell == a_spell && ae->flags.none(AE::kInactive) && ae->flags.none(AE::kDispelled);
	});
}


auto papyrusActor::HasDeferredKill(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor) -> bool
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return false;
	}

	const auto currentProcess = a_actor->currentProcess;
	const auto middleHighProcess = currentProcess ? currentProcess->middleHigh : nullptr;
	return middleHighProcess && middleHighProcess->inDeferredKill;
}


auto papyrusActor::HasMagicEffectWithArchetype(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BSFixedString a_archetype) -> bool
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return false;
	}
	if (a_archetype.empty()) {
		a_vm->TraceStack("Archetype is None", a_stackID, Severity::kWarning);
		return false;
	}

	auto activeEffects = a_actor->GetActiveEffectList();
	if (!activeEffects) {
		a_vm->TraceStack(VMError::generic_error(a_actor, "has no active effects"sv).c_str(), a_stackID, Severity::kInfo);
		return false;
	}

	return std::any_of(activeEffects->begin(), activeEffects->end(), [&](auto const& ae) {
		if (ae) {
			const auto mgef = ae->GetBaseObject();
			return mgef && mgef->GetArchetypeAsString() == a_archetype;
		}
		return false;
	});
}


auto papyrusActor::IsActorInWater(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor) -> bool
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return false;
	}

	const float waterLevel = a_actor->GetSubmergedWaterLevel(a_actor->GetPositionZ(), a_actor->GetParentCell());
	return waterLevel >= 0.01f;
}


auto papyrusActor::IsActorUnderwater(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor) -> bool
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return false;
	}

	const float waterLevel = a_actor->GetSubmergedWaterLevel(a_actor->GetPositionZ(), a_actor->GetParentCell());
	return waterLevel >= 0.875f;
}


auto papyrusActor::IsLimbGone(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, std::int32_t a_limbEnum) -> bool
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return false;
	}

	return a_actor->IsLimbGone(a_limbEnum);
}


auto papyrusActor::IsQuadruped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor) -> bool
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return false;
	}

	const auto charController = a_actor->GetCharController();
	return charController ? charController->flags.all(RE::CHARACTER_FLAGS::kQuadruped) : false;
}


auto papyrusActor::IsSoulTrapped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor) -> bool
{
	using Archetype = RE::EffectArchetypes::ArchetypeID;
	using Flags = RE::TESSoulGem::RecordFlags;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return false;
	}

	const auto currentProcess = a_actor->currentProcess;
	if (currentProcess) {
		const auto middleHighProcess = currentProcess->middleHigh;
		if (middleHighProcess && middleHighProcess->soulTrapped) {
			return true;
		}
	}

	bool isBeingSoulTrapped = false;

	if (auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
		auto handle = a_actor->CreateRefHandle();
		processLists->GetMagicEffects([&](RE::BSTempEffect& a_tempEffect) {
			auto modelEffect = a_tempEffect.As<RE::ModelReferenceEffect>();
			if (modelEffect && modelEffect->target.native_handle() == handle.native_handle()) {
				const auto modelArt = modelEffect->artObject;
				if (modelArt && modelArt->GetFormID() == SoulTrapHitArtID) {
					isBeingSoulTrapped = true;
					return false;
				}
			}
			return true;
		});
	}

	return isBeingSoulTrapped;
}


void papyrusActor::KillNoWait(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return;
	}

	a_actor->KillImmediate();
	a_actor->boolBits.set(RE::Actor::BOOL_BITS::kDead);
	a_actor->boolBits.set(RE::Actor::BOOL_BITS::kSetOnDeath);
}


void papyrusActor::MixColorWithSkinTone(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSColorForm* a_color, bool a_manual, float a_percent)
{
	a_vm->TraceStack("Function is deprecated, use BlendColorWithSkinTone instead", a_stackID, Severity::kError);

	if (a_actor && a_color) {
		const auto actorbase = a_actor->GetActorBase();
		if (actorbase) {
			auto root = a_actor->Get3D(false);
			if (root) {
				const float skinLuminance = a_manual ? a_percent : RE::NiColor::CalcLuminance(actorbase->bodyTintColor);
				auto newColor = RE::NiColor::Mix(actorbase->bodyTintColor, a_color->color, skinLuminance);

				auto task = SKSE::GetTaskInterface();
				task->AddTask([a_actor, newColor, root]() {
					TintFace(a_actor, newColor);
					root->UpdateBodyTint(newColor);
					AddOrUpdateColorData(root, "PO3_SKINTINT"sv, newColor);
				});
			}
		}
	}
}


auto papyrusActor::RemoveBasePerk(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSPerk* a_perk) -> bool
{
	using namespace Serialization::Form;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return false;
	}
	if (!a_perk) {
		a_vm->TraceStack("Perk is None", a_stackID, Severity::kWarning);
		return false;
	}

	return Perks::GetSingleton()->PapyrusApply(a_actor, a_perk, kRemove);
}


auto papyrusActor::RemoveBaseSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::SpellItem* a_spell) -> bool
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return false;
	}
	if (!a_spell) {
		a_vm->TraceStack("Spell is None", a_stackID, Severity::kWarning);
		return false;
	}

	if (const auto activeEffects = a_actor->GetActiveEffectList(); activeEffects) {
		for (auto& activeEffect : *activeEffects) {
			if (activeEffect && activeEffect->spell == a_spell) {
				activeEffect->Dispel(true);
			}
		}
	}

	if (const auto actorbase = a_actor->GetActorBase(); actorbase) {
		auto actorEffects = actorbase->actorEffects;
		if (actorEffects) {
			if (actorEffects->GetIndex(a_spell) == std::nullopt) {
				return false;
			}
			const auto combatController = a_actor->combatController;
			if (combatController) {
				combatController->data10->unk1C4 = 1;
			}
			a_actor->RemoveSelectedSpell(a_spell);
			return actorEffects->RemoveSpell(a_spell);
		}
	}

	return false;
}


void StopAllSkinAlphaShaders_Impl(RE::TESObjectREFR* a_ref)
{
	using Flags = RE::EffectShaderData::Flags;

	if (auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
		auto handle = a_ref->CreateRefHandle();
		processLists->GetMagicEffects([&](RE::BSTempEffect& a_tempEffect) {
			const auto shaderEffect = a_tempEffect.As<RE::ShaderReferenceEffect>();
			if (shaderEffect && shaderEffect->target == handle) {
				const auto effectData = shaderEffect->effectData;
				if (effectData && effectData->data.flags.all(Flags::kSkinOnly) && !effectData->holesTexture.textureName.empty()) {
					shaderEffect->finished = true;
				}
			}
			return true;
		});
	}
}


void papyrusActor::RemoveEffectsNotOfType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, std::int32_t a_type)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return;
	}

	auto root = a_actor->Get3D(false);
	if (!root) {
		a_vm->TraceStack(VMError::no_3D(a_actor).c_str(), a_stackID, Severity::kWarning);
		return;
	}

	bool result;
	RESET::ResetData resetData;

	std::tie(result, resetData) = RESET::GetData(root);
	if (!result) {
		return;
	}

	auto type = static_cast<EFFECT>(a_type);
	auto task = SKSE::GetTaskInterface();
	task->AddTask([a_actor, type, root, resetData]() {
		auto& [toggleData, skinTintData, hairTintData, alphaSkinData, txstFaceData, headpartAlphaVec, txstVec, txstSkinVec, shaderVec] = resetData;

		switch (type) {
		case EFFECT::kCharred:
			{
				RESET::HeadPartAlpha(a_actor, root, headpartAlphaVec);
				RESET::SkinTint(a_actor, root, skinTintData);
				RESET::HairTint(a_actor, root, hairTintData);
				RESET::FaceTXST(a_actor, root, txstFaceData);
				RESET::SkinTXST(a_actor, root, txstSkinVec);
			}
			break;
		case EFFECT::kDrained:
			{
				RESET::Toggle(root, toggleData);
				RESET::SkinTint(a_actor, root, skinTintData);
				RESET::HairTint(a_actor, root, hairTintData);
				RESET::FaceTXST(a_actor, root, txstFaceData);
				RESET::SkinTXST(a_actor, root, txstSkinVec);
			}
			break;
		case EFFECT::kPoisoned:
			{
				RESET::Toggle(root, toggleData);
				RESET::SkinAlpha(root, alphaSkinData);
				RESET::HeadPartAlpha(a_actor, root, headpartAlphaVec);
				RESET::FaceTXST(a_actor, root, txstFaceData);
				RESET::SkinTXST(a_actor, root, txstSkinVec);

				if (!a_actor->IsPlayerRef()) {
					StopAllSkinAlphaShaders_Impl(a_actor);
				}
			}
			break;
		case EFFECT::kAged:
			{
				RESET::Toggle(root, toggleData);
				RESET::SkinAlpha(root, alphaSkinData);
				RESET::HeadPartAlpha(a_actor, root, headpartAlphaVec);

				if (!a_actor->IsPlayerRef()) {
					StopAllSkinAlphaShaders_Impl(a_actor);
				}
			}
			break;
		case EFFECT::kCharredCreature:
			{
				RESET::MaterialShader(root, shaderVec);
			}
			break;
		default:
			break;
		}
	});
}


void SetTXST(RE::NiAVObject* a_object, RE::BGSTextureSet* a_txst, std::int32_t a_type, std::string_view a_tgtPath, bool& replaced)
{
	using Feature = RE::BSShaderMaterial::Feature;
	using Texture = RE::BSTextureSet::Texture;

	RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
		auto effect = a_geometry->properties[RE::BSGeometry::States::kEffect];
		auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect.get());
		if (lightingShader) {
			auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
			if (material) {
				if (const auto textureSet = material->textureSet.get(); textureSet) {
					std::string sourcePath(textureSet->GetTexturePath(Texture::kDiffuse));
					RE::Util::SanitizeTexturePath(sourcePath);

					if (sourcePath == a_tgtPath) {
						if (auto newMaterial = static_cast<RE::BSLightingShaderMaterialBase*>(material->Create()); newMaterial) {
							newMaterial->CopyMembers(material);
							newMaterial->ClearTextures();

							if (a_type == -1) {
								newMaterial->OnLoadTextureSet(0, a_txst);
							} else {
								if (auto newTextureSet = RE::BSShaderTextureSet::Create(); newTextureSet) {
									const auto BSTextureType = static_cast<Texture>(a_type);
									for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
										if (i != BSTextureType) {
											newTextureSet->SetTexturePath(i, textureSet->GetTexturePath(i));
										}
									}
									newTextureSet->SetTexturePath(BSTextureType, a_txst->GetTexturePath(BSTextureType));
									newMaterial->OnLoadTextureSet(0, newTextureSet);
								}
							}

							lightingShader->SetMaterial(newMaterial, true);
							//lightingShader->InitializeGeometry(a_geometry);
							lightingShader->InitializeShader(a_geometry);
							newMaterial->~BSLightingShaderMaterialBase();
							RE::free(newMaterial);

							replaced = true;
						}
					}
				}
			}
		}

		return RE::BSVisit::BSVisitControl::kContinue;
	});
}


void papyrusActor::ReplaceArmorTextureSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::TESObjectARMO* a_armor, RE::BGSTextureSet* a_srcTXST, RE::BGSTextureSet* a_tgtTXST, std::int32_t a_type)
{
	using Texture = RE::BSShaderTextureSet::Textures::Texture;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_armor) {
		a_vm->TraceStack("Armor is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_srcTXST) {
		a_vm->TraceStack("Source TextureSet is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_tgtTXST) {
		a_vm->TraceStack("Target TextureSet is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_actor->Is3DLoaded()) {
		a_vm->TraceStack(VMError::no_3D(a_actor).c_str(), a_stackID, Severity::kWarning);
		return;
	}

	std::string targetPath(a_srcTXST->GetTexturePath(Texture::kDiffuse));
	RE::Util::SanitizeTexturePath(targetPath);

	auto task = SKSE::GetTaskInterface();
	task->AddTask([a_actor, a_armor, a_srcTXST, a_tgtTXST, a_type, targetPath]() {
		bool replaced = false;

		if (const auto armorAddon = a_armor->GetArmorAddon(a_actor->GetRace()); armorAddon) {
			const auto armorObject = a_actor->VisitArmorAddon(a_armor, armorAddon);
			if (armorObject) {
				SetTXST(armorObject, a_tgtTXST, a_type, targetPath, replaced);
			}
		}

		auto root = a_actor->Get3D(false);
		if (replaced && root) {
			auto armorID = std::to_string(a_armor->formID);
			auto name = "PO3_TXST - " + armorID;

			auto data = root->GetExtraData<RE::NiStringsExtraData>(name);
			if (!data) {
				std::vector<RE::BSFixedString> vec;
				vec.reserve(Texture::kTotal);
				for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
					vec.emplace_back(a_srcTXST->GetTexturePath(i));
				}
				vec.emplace_back(armorID);
				auto newData = RE::NiStringsExtraData::Create(name, vec);
				if (newData) {
					root->AddExtraData(newData);
				}
			}
		}
	});
}


void SetSkinTXST(RE::NiAVObject* a_object, RE::BGSTextureSet* a_txst, std::vector<RE::BSFixedString>& a_vec, std::int32_t a_type)
{
	using Feature = RE::BSShaderMaterial::Feature;
	using Texture = RE::BSTextureSet::Texture;

	RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
		auto effect = a_geometry->properties[RE::BSGeometry::States::kEffect];
		auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect.get());
		if (lightingShader) {
			auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
			if (material) {
				if (const auto textureSet = material->textureSet.get(); textureSet) {
					auto const type = material->GetFeature();
					if (type == Feature::kFaceGenRGBTint || type == Feature::kFaceGen) {
						if (a_vec.empty()) {
							a_vec.reserve(Texture::kTotal);
							for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
								a_vec.emplace_back(textureSet->GetTexturePath(i));
							}
						}

						if (auto newMaterial = static_cast<RE::BSLightingShaderMaterialBase*>(material->Create()); newMaterial) {
							newMaterial->CopyMembers(material);
							newMaterial->ClearTextures();

							if (a_type == -1) {
								if (type == Feature::kFaceGen) {
									if (auto newTextureSet = RE::BSShaderTextureSet::Create(); newTextureSet) {
										for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
											if (i != Texture::kMultilayer) {
												newTextureSet->SetTexturePath(i, a_txst->GetTexturePath(i));
											}
										}
										newTextureSet->SetTexturePath(Texture::kMultilayer, textureSet->GetTexturePath(Texture::kMultilayer));
										newMaterial->OnLoadTextureSet(0, newTextureSet);
									}
								} else {
									newMaterial->OnLoadTextureSet(0, a_txst);
								}
							} else {
								if (auto newTextureSet = RE::BSShaderTextureSet::Create(); newTextureSet) {
									const auto BSTextureType = static_cast<Texture>(a_type);
									for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
										if (i != BSTextureType) {
											newTextureSet->SetTexturePath(i, material->textureSet->GetTexturePath(i));
										}
									}
									newTextureSet->SetTexturePath(BSTextureType, a_txst->GetTexturePath(BSTextureType));
									newMaterial->OnLoadTextureSet(0, newTextureSet);
								}
							}

							lightingShader->SetMaterial(newMaterial, true);
							//lightingShader->InitializeGeometry(a_geometry);
							lightingShader->InitializeShader(a_geometry);
							newMaterial->~BSLightingShaderMaterialBase();
							RE::free(newMaterial);
						}
					}
				}
			}
		}

		return RE::BSVisit::BSVisitControl::kContinue;
	});
}


void papyrusActor::ReplaceFaceTextureSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSTextureSet* a_maleTXST, RE::BGSTextureSet* a_femaleTXST, std::int32_t a_type)
{
	using Texture = RE::BSShaderTextureSet::Texture;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_actor->Is3DLoaded()) {
		a_vm->TraceStack(VMError::no_3D(a_actor).c_str(), a_stackID, Severity::kWarning);
		return;
	}

	const auto actorBase = a_actor->GetActorBase();
	const bool isFemale = actorBase ? actorBase->IsFemale() : false;

	auto txst = isFemale ? a_femaleTXST : a_maleTXST;
	if (!txst) {
		a_vm->TraceStack("TextureSet is None", a_stackID, Severity::kWarning);
		return;
	}

	auto task = SKSE::GetTaskInterface();
	task->AddTask([txst, a_type, a_actor]() {
		const auto faceObject = a_actor->GetHeadPartObject(RE::BGSHeadPart::HeadPartType::kFace);
		if (faceObject) {
			std::vector<RE::BSFixedString> vec;
			SetSkinTXST(faceObject, txst, vec, a_type);

			auto root = a_actor->Get3D(false);
			if (!vec.empty() && root) {
				auto data = root->GetExtraData<RE::NiStringsExtraData>("PO3_FACETXST"sv);
				if (!data) {
					auto newData = RE::NiStringsExtraData::Create("PO3_FACETXST"sv, vec);
					if (newData) {
						root->AddExtraData(newData);
					}
				}
			}
		}
	});
}


void SetArmorSkinTXST(RE::Actor* a_actor, RE::BGSTextureSet* a_txst, RE::BIPED_MODEL::BipedObjectSlot a_slot, std::int32_t a_type)
{
	auto skinArmor = a_actor->GetSkin(a_slot);
	if (!skinArmor) {
		return;
	}

	auto foundAddon = skinArmor->GetArmorAddonByMask(a_actor->GetRace(), a_slot);
	if (!foundAddon) {
		return;
	}

	auto task = SKSE::GetTaskInterface();
	task->AddTask([a_actor, a_txst, a_slot, a_type, skinArmor, foundAddon]() {
		const auto armorObject = a_actor->VisitArmorAddon(skinArmor, foundAddon);
		if (armorObject) {
			std::vector<RE::BSFixedString> vec;
			vec.reserve(10);

			SetSkinTXST(armorObject, a_txst, vec, a_type);

			auto root = a_actor->Get3D(false);
			if (!vec.empty() && root) {
				auto slotMaskStr = std::to_string(to_underlying(a_slot));
				auto name = "PO3_SKINTXST - " + slotMaskStr;
				vec.emplace_back(slotMaskStr);

				auto data = root->GetExtraData<RE::NiStringsExtraData>(name);
				if (!data) {
					auto newData = RE::NiStringsExtraData::Create(name, vec);
					if (newData) {
						root->AddExtraData(newData);
					}
				}
			}
		}
	});
}


void papyrusActor::ReplaceSkinTextureSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSTextureSet* a_maleTXST, RE::BGSTextureSet* a_femaleTXST, std::uint32_t a_slot, std::int32_t a_type)
{
	using BipedSlot = RE::BIPED_MODEL::BipedObjectSlot;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_actor->Is3DLoaded()) {
		a_vm->TraceStack(VMError::no_3D(a_actor).c_str(), a_stackID, Severity::kWarning);
		return;
	}

	const auto actorBase = a_actor->GetActorBase();
	const bool isFemale = actorBase ? actorBase->IsFemale() : false;

	if (isFemale) {
		if (!a_femaleTXST) {
			a_vm->TraceStack("Female TextureSet is None", a_stackID, Severity::kWarning);
			return;
		}
		SetArmorSkinTXST(a_actor, a_femaleTXST, static_cast<BipedSlot>(a_slot), a_type);
	} else {
		if (!a_maleTXST) {
			a_vm->TraceStack("Male TextureSet is None", a_stackID, Severity::kWarning);
			return;
		}
		SetArmorSkinTXST(a_actor, a_maleTXST, static_cast<BipedSlot>(a_slot), a_type);
	}
}


auto papyrusActor::ResetActor3D(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BSFixedString a_folderName) -> bool
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return false;
	}

	auto root = a_actor->Get3D(false);
	if (!root) {
		a_vm->TraceStack(VMError::no_3D(a_actor).c_str(), a_stackID, Severity::kWarning);
		return false;
	}

	bool result = true;
	RESET::ResetData resetData;

	std::tie(result, resetData) = RESET::GetData(root);
	if (!result) {
		return false;
	}

	auto task = SKSE::GetTaskInterface();
	task->AddTask([a_actor, a_folderName, root, resetData]() {
		auto& [toggleData, skinTintData, hairTintData, skinAlphaData, txstFaceData, headpartAlphaVec, txstVec, txstSkinVec, shaderVec] = resetData;

		RESET::Toggle(root, toggleData);

		RESET::SkinAlpha(root, skinAlphaData);

		RESET::HeadPartAlpha(a_actor, root, headpartAlphaVec);

		RESET::SkinTint(a_actor, root, skinTintData);

		RESET::HairTint(a_actor, root, hairTintData);

		RESET::FaceTXST(a_actor, root, txstFaceData);

		RESET::SkinTXST(a_actor, root, txstSkinVec);

		if (!a_folderName.empty()) {
			RESET::TXST(a_actor, root, a_folderName, txstVec);
		}

		RESET::MaterialShader(root, shaderVec);
	});


	if (auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
		if (!a_actor->IsPlayerRef()) {
			processLists->StopAllShaders(*a_actor);
		} else {
			StopAllSkinAlphaShaders_Impl(a_actor);
		}
	}

	return true;
}


void papyrusActor::SendFECResetEvent(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, std::uint32_t a_type, bool a_reset)
{
	using namespace Serialization::FECEvents;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return;
	}

	OnFECResetRegMap::GetSingleton()->QueueEvent(a_type, a_actor, a_type, a_reset);
}


void papyrusActor::SetActorRefraction(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, float a_refraction)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return;
	}

	auto currentProcess = a_actor->currentProcess;
	if (currentProcess) {
		a_refraction = std::clamp(a_refraction, 0.0f, 1.0f);
		currentProcess->SetRefraction(a_refraction);

		const float invisibility = a_actor->GetActorValue(RE::ActorValue::kInvisibility);
		if (invisibility < 0.0f || invisibility <= 1.0f && invisibility <= 0.0f || !a_actor->IsPlayerRef()) {
			if (a_refraction <= 0.0f) {
				a_actor->SetRefraction(false, a_refraction);
				a_actor->UpdateAlpha();
			} else {
				a_actor->SetRefraction(true, a_refraction);
			}
		} else {
			a_actor->SetAlpha(1.0);

			a_refraction = 1.0f - a_refraction / 100.0f;
			a_refraction = 1.0f + (0.01f - 1.0f) * ((a_refraction - 0.0f) / (1.0f - 0.0f));

			a_actor->SetRefraction(true, a_refraction);
		}
	}
}


void papyrusActor::SetHairColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSColorForm* a_color)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_color) {
		a_vm->TraceStack("Colorform is None", a_stackID, Severity::kWarning);
		return;
	}

	auto root = a_actor->Get3D(false);
	if (!root) {
		a_vm->TraceStack(VMError::no_3D(a_actor).c_str(), a_stackID, Severity::kWarning);
		return;
	}

	auto task = SKSE::GetTaskInterface();
	task->AddTask([root, a_color]() {
		root->UpdateHairColor(a_color->color);
		AddOrUpdateColorData(root, "PO3_HAIRTINT"sv, a_color->color);
	});
}


void papyrusActor::SetHeadPartAlpha(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, std::int32_t a_type, float a_alpha)
{
	using HeadPartType = RE::BGSHeadPart::HeadPartType;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return;
	}
	if (a_type < 0 || a_type > 6) {
		a_vm->TraceStack("Invalid headpart type", a_stackID, Severity::kWarning);
		return;
	}

	auto root = a_actor->Get3D(false);
	if (!root) {
		a_vm->TraceStack(VMError::no_3D(a_actor).c_str(), a_stackID, Severity::kWarning);
		return;
	}

	auto task = SKSE::GetTaskInterface();
	task->AddTask([root, a_actor, a_alpha, a_type]() {
		if (auto object = a_actor->GetHeadPartObject(static_cast<HeadPartType>(a_type)); object) {
			object->UpdateMaterialAlpha(a_alpha, false);

			const auto name = "PO3_HEADPART - " + std::to_string(a_type);
			if (a_alpha == 1.0f) {
				root->RemoveExtraData(name);
			} else {
				if (auto data = root->GetExtraData<RE::NiIntegerExtraData>(name); !data) {
					if (auto newData = RE::NiIntegerExtraData::Create(name, a_type); newData) {
						root->AddExtraData(newData);
					}
				}
			}
		}
	});
}


void papyrusActor::SetHeadPartTextureSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSTextureSet* a_txst, std::int32_t a_type)
{
	using HeadPartType = RE::BGSHeadPart::HeadPartType;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return;
	}
	if (a_type < 0 || a_type > 6) {
		a_vm->TraceStack("Invalid headpart type", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_txst) {
		a_vm->TraceStack("Textureset is None", a_stackID, Severity::kWarning);
		return;
	}

	auto actorBase = a_actor->GetActorBase();
	if (actorBase) {
		const auto headpart = actorBase->GetCurrentHeadPartByType(static_cast<HeadPartType>(a_type));
		if (headpart) {
			headpart->textureSet = a_txst;
		} else {
			a_vm->TraceStack("Could not find matching headpart", a_stackID, Severity::kWarning);
		}
	}
}


void papyrusActor::SetLinearVelocity(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, float a_x, float a_y, float a_z)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return;
	}

	auto currentProcess = a_actor->currentProcess;
	if (currentProcess) {
		auto charProxy = static_cast<RE::bhkCharProxyController*>(currentProcess->GetCharController());
		if (charProxy) {
			charProxy->SetLinearVelocityImpl(RE::hkVector4(a_x * 0.0142875f, a_y * 0.0142875f, a_z * 0.0142875f, 0.0f));
		}
	}
}


void papyrusActor::SetLocalGravityActor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, float a_value, bool a_disableGravityOnGround)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return;
	}

	auto currentProcess = a_actor->currentProcess;
	if (currentProcess) {
		auto charProxy = static_cast<RE::bhkCharProxyController*>(currentProcess->GetCharController());
		if (charProxy) {
			charProxy->SetLinearVelocityImpl(0.0);

			a_disableGravityOnGround ? charProxy->flags.reset(RE::CHARACTER_FLAGS::kNoGravityOnGround) : charProxy->flags.set(RE::CHARACTER_FLAGS::kNoGravityOnGround);
			charProxy->gravity = a_value;
		}
	}
}


void papyrusActor::SetSkinAlpha(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, float a_alpha)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return;
	}

	auto root = a_actor->Get3D(false);
	if (!root) {
		a_vm->TraceStack(VMError::no_3D(a_actor).c_str(), a_stackID, Severity::kWarning);
		return;
	}

	auto task = SKSE::GetTaskInterface();
	task->AddTask([root, a_alpha]() {
		root->UpdateMaterialAlpha(a_alpha, true);

		if (a_alpha == 1.0f) {
			root->RemoveExtraData("PO3_SKINALPHA"sv);
		} else if (const auto data = root->GetExtraData<RE::NiBooleanExtraData>("PO3_SKINALPHA"sv); !data) {
			if (const auto newData = RE::NiBooleanExtraData::Create("PO3_SKINALPHA"sv, true); newData) {
				root->AddExtraData(newData);
			}
		}
	});
}


void papyrusActor::SetSkinColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSColorForm* a_color)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_color) {
		a_vm->TraceStack("Colorform is None", a_stackID, Severity::kWarning);
		return;
	}

	auto root = a_actor->Get3D(false);
	if (!root) {
		a_vm->TraceStack(VMError::no_3D(a_actor).c_str(), a_stackID, Severity::kWarning);
		return;
	}

	auto task = SKSE::GetTaskInterface();
	task->AddTask([a_actor, a_color, root]() {
		TintFace(a_actor, a_color->color);
		root->UpdateBodyTint(a_color->color);

		AddOrUpdateColorData(root, "PO3_SKINTINT"sv, a_color->color);
	});
}


void papyrusActor::SetSoulTrapped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, bool a_trapped)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return;
	}

	const auto process = a_actor->currentProcess;
	if (process) {
		const auto middleHigh = process->middleHigh;
		if (middleHigh) {
			middleHigh->soulTrapped = a_trapped;
		}
	}
}


void papyrusActor::ToggleHairWigs(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, bool a_disable)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return;
	}

	auto root = a_actor->Get3D(false);
	if (!root) {
		a_vm->TraceStack(VMError::no_3D(a_actor).c_str(), a_stackID, Severity::kWarning);
		return;
	}

	auto task = SKSE::GetTaskInterface();
	task->AddTask([root, a_actor, a_disable]() {
		if (auto biped = a_actor->GetCurrentBiped().get(); biped) {
			for (auto& slot : headSlots) {
				auto object = biped->objects[slot];
				if (object.addon) {
					auto node = object.partClone.get();
					if (node && node->HasShaderType(RE::BSShaderMaterial::Feature::kHairTint)) {
						node->ToggleNode(a_disable);

						if (auto data = root->GetExtraData<RE::NiStringsExtraData>("PO3_TOGGLE"sv); data) {
							a_disable ? data->Insert(node->name) : data->Remove(node->name);
						} else if (a_disable) {
							std::vector<RE::BSFixedString> vec;
							vec.push_back(node->name);
							if (const auto newData = RE::NiStringsExtraData::Create("PO3_TOGGLE"sv, vec); newData) {
								root->AddExtraData(newData);
							}
						}
					}
				}
			}
		}
	});
}


void papyrusActor::UnequipAllOfType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, std::uint32_t a_armorType, std::vector<std::uint32_t> a_slotsToSkip)
{
	using Slot = RE::BIPED_MODEL::BipedObjectSlot;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return;
	}

	auto inv = a_actor->GetInventory([a_armorType, a_slotsToSkip](RE::TESBoundObject& a_object) {
		auto armor = a_object.As<RE::TESObjectARMO>();
		if (armor && armor->bipedModelData.armorType.underlying() == a_armorType) {
			if (a_slotsToSkip.empty() || std::none_of(a_slotsToSkip.begin(), a_slotsToSkip.end(),
											 [&](const auto& slot) {
												 return armor->HasPartOf(static_cast<Slot>(slot));
											 })) {
				return true;
			}
		}
		return false;
	});

	if (const auto equipManager = RE::ActorEquipManager::GetSingleton(); equipManager) {
		for (auto& [item, data] : inv) {
			auto& [count, entry] = data;
			if (count > 0 && entry->GetWorn()) {
				equipManager->UnequipObject(a_actor, item);
			}
		}
	}
}


auto papyrusActor::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusActor - couldn't get VMState"sv);
		return false;
	}

	auto constexpr Functions = "PO3_SKSEFunctions"sv;

	a_vm->RegisterFunction("AddBasePerk"sv, Functions, AddBasePerk);

	a_vm->RegisterFunction("AddBaseSpell"sv, Functions, AddBaseSpell);

	a_vm->RegisterFunction("AddAllEquippedItemsToArray"sv, Functions, AddAllEquippedItemsToArray);

	a_vm->RegisterFunction("BlendColorWithSkinTone"sv, Functions, BlendColorWithSkinTone);

	a_vm->RegisterFunction("DecapitateActor"sv, Functions, DecapitateActor);

	a_vm->RegisterFunction("EquipArmorIfSkinVisible"sv, Functions, EquipArmorIfSkinVisible);

	a_vm->RegisterFunction("FreezeActor"sv, Functions, FreezeActor);

	a_vm->RegisterFunction("GetActiveEffects"sv, Functions, GetActiveEffects);

	a_vm->RegisterFunction("GetActorAlpha"sv, Functions, GetActorAlpha);

	a_vm->RegisterFunction("GetActorRefraction"sv, Functions, GetActorRefraction);

	a_vm->RegisterFunction("GetActorState"sv, Functions, GetActorState, true);

	a_vm->RegisterFunction("GetActorSoulSize"sv, Functions, GetActorSoulSize, true);

	a_vm->RegisterFunction("GetCriticalStage"sv, Functions, GetCriticalStage, true);

	a_vm->RegisterFunction("GetCombatAllies"sv, Functions, GetCombatAllies);

	a_vm->RegisterFunction("GetCombatTargets"sv, Functions, GetCombatTargets);

	a_vm->RegisterFunction("GetDeathEffectType"sv, Functions, GetDeathEffectType);

	a_vm->RegisterFunction("GetHairColor"sv, Functions, GetHairColor);

	a_vm->RegisterFunction("GetHeadPartTextureSet"sv, Functions, GetHeadPartTextureSet);

	a_vm->RegisterFunction("GetLocalGravityActor"sv, Functions, GetLocalGravityActor);

	a_vm->RegisterFunction("GetObjectUnderFeet"sv, Functions, GetObjectUnderFeet);

	a_vm->RegisterFunction("GetRunningPackage"sv, Functions, GetRunningPackage);

	a_vm->RegisterFunction("GetSkinColor"sv, Functions, GetSkinColor);

	a_vm->RegisterFunction("GetTimeDead"sv, Functions, GetTimeDead);

	a_vm->RegisterFunction("GetTimeOfDeath"sv, Functions, GetTimeOfDeath);

	a_vm->RegisterFunction("HasActiveSpell"sv, Functions, HasActiveSpell);

	a_vm->RegisterFunction("IsQuadruped"sv, Functions, IsQuadruped, true);

	a_vm->RegisterFunction("HasDeferredKill"sv, Functions, HasDeferredKill);

	a_vm->RegisterFunction("HasMagicEffectWithArchetype"sv, Functions, HasMagicEffectWithArchetype);

	a_vm->RegisterFunction("IsActorInWater"sv, Functions, IsActorInWater, true);

	a_vm->RegisterFunction("IsActorUnderwater"sv, Functions, IsActorUnderwater, true);

	a_vm->RegisterFunction("IsLimbGone"sv, Functions, IsLimbGone);

	a_vm->RegisterFunction("IsSoulTrapped"sv, Functions, IsSoulTrapped);

	a_vm->RegisterFunction("KillNoWait"sv, Functions, KillNoWait);

	a_vm->RegisterFunction("MixColorWithSkinTone"sv, Functions, MixColorWithSkinTone);

	a_vm->RegisterFunction("RemoveBasePerk"sv, Functions, RemoveBasePerk);

	a_vm->RegisterFunction("RemoveBaseSpell"sv, Functions, RemoveBaseSpell);

	a_vm->RegisterFunction("RemoveEffectsNotOfType"sv, Functions, RemoveEffectsNotOfType);

	a_vm->RegisterFunction("ReplaceArmorTextureSet"sv, Functions, ReplaceArmorTextureSet);

	a_vm->RegisterFunction("ReplaceFaceTextureSet"sv, Functions, ReplaceFaceTextureSet);

	a_vm->RegisterFunction("ReplaceSkinTextureSet"sv, Functions, ReplaceSkinTextureSet);

	a_vm->RegisterFunction("ResetActor3D"sv, Functions, ResetActor3D);

	a_vm->RegisterFunction("SendFECResetEvent"sv, Functions, SendFECResetEvent);

	a_vm->RegisterFunction("SetActorRefraction"sv, Functions, SetActorRefraction);

	a_vm->RegisterFunction("SetHairColor"sv, Functions, SetHairColor);

	a_vm->RegisterFunction("SetHeadPartAlpha"sv, Functions, SetHeadPartAlpha);

	a_vm->RegisterFunction("SetHeadPartTextureSet"sv, Functions, SetHeadPartTextureSet);

	a_vm->RegisterFunction("SetLinearVelocity"sv, Functions, SetLinearVelocity);

	a_vm->RegisterFunction("SetLocalGravityActor"sv, Functions, SetLocalGravityActor);

	a_vm->RegisterFunction("SetSkinAlpha"sv, Functions, SetSkinAlpha);

	a_vm->RegisterFunction("SetSkinColor"sv, Functions, SetSkinColor);

	a_vm->RegisterFunction("SetSoulTrapped"sv, Functions, SetSoulTrapped);

	a_vm->RegisterFunction("ToggleHairWigs"sv, Functions, ToggleHairWigs);

	a_vm->RegisterFunction("UnequipAllOfType"sv, Functions, UnequipAllOfType);

	return true;
}
