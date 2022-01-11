#pragma once

#include "Serialization/EventHolder.h"

namespace Papyrus::FEC
{
	namespace DEATH_TYPE
	{
		enum PERMANENT : std::uint32_t
		{
			kNone = static_cast<std::underlying_type_t<PERMANENT>>(-1),
			kSun = 0,
			kAcid,
			kFire,
			kFrost,
			kShock,
			kDrain,
			kFireFrost,
			kFireShock,
			kDrainFrost,
			kDrainShock,
			kFrostShock,
			kShockFrost
		};

		enum TEMPORARY : std::uint32_t
		{
			kPoison = 0,
			kFear,
			kAsh,
			kPoisonFear,
			kAshFear
		};
	}

	enum class EFFECT_TYPE : std::uint32_t
	{
		kNone = static_cast<std::underlying_type_t<EFFECT_TYPE>>(-1),
		kCharred = 0,
		kSkeletonized = kCharred,
		kDrained = 1,
		kPoisoned = 2,
		kFrightened = kPoisoned,
		kAged = 3,
		kCharredCreature = 4,
		kFrozenCreature = 5
	};
	using EFFECT = EFFECT_TYPE;

	inline constexpr auto SunKYWD{ "PO3_MagicDamageSun"sv };
	inline constexpr auto FireKYWD{ "MagicDamageFire"sv };
	inline constexpr auto FrostKYWD{ "MagicDamageFrost"sv };
	inline constexpr auto ShockKYWD{ "MagicDamageShock"sv };
	inline constexpr auto ParalysisKYWD{ "MagicParalysis"sv };

	inline std::vector<std::int32_t> GetDeathEffectType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		std::uint32_t a_type)
	{
		using FLAG = RE::EffectSetting::EffectSettingData::Flag;
		using AE_FLAG = RE::ActiveEffect::Flag;
		using CAST_TYPE = RE::MagicSystem::CastingType;
		using Archetype = RE::EffectArchetypes::ArchetypeID;

		std::vector<std::int32_t> vec;
		vec.resize(3, -1);

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return vec;
		}

		const auto activeEffects = a_actor->GetActiveEffectList();
		if (!activeEffects) {
			a_vm->TraceForm(a_actor, "has no active effects", a_stackID, Severity::kInfo);
			return vec;
		}

		using deathEffectPair = std::pair<std::uint32_t, RE::EffectSetting*>;  //[type, mgef]
		deathEffectPair effectPair = { -1, nullptr };

		using deathEffectMap = std::map<std::uint32_t, std::vector<std::pair<RE::EffectSetting*, float>>>;  // [type, [mgef, mag]]
		deathEffectMap effectMap;

		for (const auto& activeEffect : *activeEffects) {
			const auto mgef = activeEffect ? activeEffect->GetBaseObject() : nullptr;
			if (mgef && mgef->data.flags.all(FLAG::kHostile) && mgef->data.flags.all(FLAG::kDetrimental)) {
				if (a_type == 0) {
					if (mgef->HasKeyword(SunKYWD)) {
						effectPair = { DEATH_TYPE::kSun, mgef };  //sun override
						break;
						/*} else if (mgef->data.resistVariable == RE::ActorValue::kPoisonResist && mgef->data.castingType == CAST_TYPE::kConcentration) {
						effectPair = { DEATH_TYPE::kAcid, mgef };  //acid override
						break;*/
					}
					if (mgef->HasKeyword(FireKYWD)) {
						effectMap[DEATH_TYPE::kFire].emplace_back(mgef, -activeEffect->magnitude);  //flipping the magnitude back to +ve
					} else if (mgef->HasKeyword(FrostKYWD)) {
						effectMap[DEATH_TYPE::kFrost].emplace_back(mgef, -activeEffect->magnitude);
					} else if (mgef->HasKeyword(ShockKYWD)) {
						effectMap[DEATH_TYPE::kShock].emplace_back(mgef, -activeEffect->magnitude);
					} else if (mgef->GetArchetype() == Archetype::kAbsorb) {
						effectMap[DEATH_TYPE::kDrain].emplace_back(mgef, -activeEffect->magnitude);
					}
				} else {
					if (mgef->data.resistVariable == RE::ActorValue::kPoisonResist /*&& mgef->data.castingType != CAST_TYPE::kConcentration*/) {
						effectMap[DEATH_TYPE::kPoison].emplace_back(mgef, -activeEffect->magnitude);
					} else if (mgef->GetArchetype() == Archetype::kDemoralize) {
						effectMap[DEATH_TYPE::kFear].emplace_back(mgef, -activeEffect->magnitude);
					} else if (mgef->data.associatedSkill == RE::ActorValue::kAlteration && mgef->HasKeyword(ParalysisKYWD)) {
						effectMap[DEATH_TYPE::kAsh].emplace_back(mgef, -activeEffect->magnitude);
					}
				}
			}
		}

		if (effectPair.first == DEATH_TYPE::kNone && !effectMap.empty()) {
			constexpr auto mag_cmp = [](const auto& a_lhs, const auto& a_rhs) {
				return numeric::definitely_less_than(a_lhs.second, a_rhs.second);
			};

			if (effectMap.size() == 1) {
				const auto& type = effectMap.begin()->first;
				auto [effect, effectMag] = *std::ranges::max_element(effectMap.begin()->second, mag_cmp);
				effectPair = { type, effect };
			} else {
				if (a_type != 0) {
					bool poison = !effectMap[DEATH_TYPE::kPoison].empty();
					bool fear = !effectMap[DEATH_TYPE::kFear].empty();
					bool ash = !effectMap[DEATH_TYPE::kAsh].empty();

					if (poison) {
						auto& poisonVec = effectMap[DEATH_TYPE::kPoison];
						auto poisonEffect = *std::ranges::max_element(poisonVec, mag_cmp);

						effectPair = { DEATH_TYPE::kPoison, poisonEffect.first };
						if (fear) {
							effectPair.first = DEATH_TYPE::kPoisonFear;
						}
					} else if (ash) {
						auto& ashVec = effectMap[DEATH_TYPE::kAsh];
						auto ashEffect = *std::ranges::max_element(ashVec, mag_cmp);

						effectPair = { DEATH_TYPE::kAsh, ashEffect.first };
						if (fear) {
							effectPair.first = DEATH_TYPE::kAshFear;
						}
					} else if (fear) {
						auto& fearVec = effectMap[DEATH_TYPE::kFear];
						auto poisonEffect = *std::ranges::max_element(fearVec, mag_cmp);

						effectPair = { DEATH_TYPE::kFear, poisonEffect.first };
					}
				} else {
					bool fire = !effectMap[DEATH_TYPE::kFire].empty();
					bool frost = !effectMap[DEATH_TYPE::kFrost].empty();
					bool shock = !effectMap[DEATH_TYPE::kShock].empty();
					bool drain = !effectMap[DEATH_TYPE::kDrain].empty();

					if (fire) {
						auto& fireVec = effectMap[DEATH_TYPE::kFire];
						auto fireEffect = *std::ranges::max_element(fireVec, mag_cmp);

						effectPair = { DEATH_TYPE::kFire, fireEffect.first };
						if (frost) {
							effectPair.first = DEATH_TYPE::kFireFrost;
						} else if (shock) {
							effectPair.first = DEATH_TYPE::kFireShock;
						}
					} else if (drain) {
						auto& drainVec = effectMap[DEATH_TYPE::kDrain];
						auto drainEffect = *std::ranges::max_element(drainVec, mag_cmp);

						effectPair = { DEATH_TYPE::kDrain, drainEffect.first };
						if (shock) {
							effectPair.first = DEATH_TYPE::kDrainShock;
							auto& shockVec = effectMap[DEATH_TYPE::kShock];
							auto shockEffect = *std::ranges::max_element(shockVec, mag_cmp);

							if (numeric::definitely_less_than(drainEffect.second, shockEffect.second)) {
								effectPair.second = shockEffect.first;
							}
						} else if (frost) {
							effectPair.first = DEATH_TYPE::kDrainFrost;
							auto& frostVec = effectMap[DEATH_TYPE::kFrost];
							auto frostEffect = *std::ranges::max_element(frostVec, mag_cmp);

							if (numeric::definitely_less_than(drainEffect.second, frostEffect.second)) {
								effectPair.second = frostEffect.first;
							}
						}
					} else if (frost) {
						auto& frostVec = effectMap[DEATH_TYPE::kFrost];
						auto frostEffect = *std::ranges::max_element(frostVec, mag_cmp);

						effectPair = { DEATH_TYPE::kFrost, frostEffect.first };
						if (shock) {
							auto& shockVec = effectMap[DEATH_TYPE::kShock];
							auto shockEffect = *std::ranges::max_element(shockVec, mag_cmp);

							if (numeric::definitely_less_than(frostEffect.second, shockEffect.second)) {
								effectPair = { DEATH_TYPE::kShockFrost, shockEffect.first };
							} else {
								effectPair.first = { DEATH_TYPE::kFrostShock };
							}
						}
					} else if (shock) {
						auto& shockVec = effectMap[DEATH_TYPE::kShock];
						auto shockEffect = *std::ranges::max_element(shockVec, mag_cmp);

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

	inline void RemoveEffectsNotOfType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		std::int32_t a_type)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		const auto root = a_actor->Get3D(false);
		if (!root) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID, Severity::kInfo);
			return;
		}

		bool result = false;
		RESET::ResetData resetData{};

		std::tie(result, resetData) = RESET::get_data(root);
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
						RESET::stop_all_skin_shaders(a_actor);
					}
				}
				break;
			case EFFECT::kAged:
				{
					RESET::Toggle(root, toggleData);
					RESET::SkinAlpha(root, alphaSkinData);
					RESET::HeadPartAlpha(a_actor, root, headpartAlphaVec);

					if (!a_actor->IsPlayerRef()) {
						RESET::stop_all_skin_shaders(a_actor);
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

	inline void RegisterForFECReset(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::ActiveEffect* a_activeEffect,
		std::uint32_t a_type)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->FECreset;
		regs.Register(a_activeEffect, a_type);
	}

	inline void SendFECResetEvent(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::Actor* a_actor,
		std::uint32_t a_type,
		bool a_reset)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		Event::GameEventHolder::GetSingleton()->FECreset.QueueEvent(a_type, a_actor, a_type, a_reset);
	}

	inline void UnregisterForFECReset(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::ActiveEffect* a_activeEffect,
		std::uint32_t a_type)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->FECreset;
		regs.Unregister(a_activeEffect, a_type);
	}

	inline void UnregisterForAllFECResets(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::ActiveEffect* a_activeEffect)
	{
		if (!a_activeEffect) {
			a_vm->TraceStack("Active Effect is None", a_stackID);
			return;
		}

		auto& regs = Event::GameEventHolder::GetSingleton()->FECreset;
		regs.UnregisterAll(a_activeEffect);
	}

	inline void Bind(VM& a_vm)
	{
		auto const obj = "PO3_Events_AME"sv;

		BIND(GetDeathEffectType);
		BIND(RemoveEffectsNotOfType);
		BIND(SendFECResetEvent, true);

		BIND_EVENT(RegisterForFECReset, true);
		BIND_EVENT(UnregisterForFECReset, true);
		BIND_EVENT(UnregisterForAllFECResets, true);

		logger::info("Registered FEC functions"sv);
	}
}
