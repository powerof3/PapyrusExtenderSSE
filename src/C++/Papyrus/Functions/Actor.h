#pragma once

#include "Serialization/Services.h"

namespace Papyrus::Actor
{
	using Biped = RE::BIPED_OBJECT;

	inline constexpr RE::FormID SoulTrapHitArtID = 0x000531AE;
	inline constexpr std::array<Biped, 3> headSlots = { Biped::kHair, Biped::kLongHair, Biped::kCirclet };

	inline bool AddBasePerk(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::BGSPerk* a_perk)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}
		if (!a_perk) {
			a_vm->TraceStack("Perk is None", a_stackID);
			return false;
		}

		return FORM::PerkManager::GetSingleton()->Add(a_actor, a_perk);
	}

	inline bool AddBaseSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::SpellItem* a_spell)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}
		if (!a_spell) {
			a_vm->TraceStack("Spell is None", a_stackID);
			return false;
		}
		if (a_actor->HasSpell(a_spell)) {
			a_vm->TraceForm(a_actor, "already has spell", a_stackID, Severity::kInfo);
			return false;
		}

		const auto actorbase = a_actor->GetActorBase();
		const auto actorEffects = actorbase ? actorbase->GetSpellList() : nullptr;

		if (actorEffects && actorEffects->AddSpell(a_spell)) {
			if (const auto combatController = a_actor->combatController; combatController && combatController->inventoryController) {
				combatController->inventoryController->unk1C4 = 1;
			}
			if (actorbase->IsPlayer() || a_spell->GetSpellType() == RE::MagicSystem::SpellType::kLeveledSpell) {
				RE::SpellsLearned::SendEvent(a_spell);
			}
			return true;
		}

		return false;
	}

	inline std::vector<RE::TESForm*> AddAllEquippedItemsToArray(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		std::vector<RE::TESForm*> result;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return result;
		}

		auto inv = a_actor->GetInventory();
		for (const auto& [item, data] : inv) {
			if (item->Is(RE::FormType::LeveledItem)) {
				continue;
			}
			const auto& [count, entry] = data;
			if (count > 0 && entry->IsWorn()) {
				result.push_back(item);
			}
		}

		return result;
	}

	inline void BlendColorWithSkinTone(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::BGSColorForm* a_color,
		std::uint32_t a_blendMode,
		bool a_autoCalc,
		float a_opacity)
	{
		using BLEND_MODE = COLOR::BLEND_MODE;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}
		if (!a_color) {
			a_vm->TraceStack("Colorform is None", a_stackID);
			return;
		}

		auto root = a_actor->Get3D(false);
		if (!root) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return;
		}

		const auto actorbase = a_actor->GetActorBase();
		if (actorbase) {
			const float opacity = a_autoCalc ? std::clamp(a_opacity * COLOR::CalcLuminance(actorbase->bodyTintColor), 0.0f, 1.0f) : a_opacity;
			auto newColor = COLOR::Blend(actorbase->bodyTintColor, a_color->color, static_cast<BLEND_MODE>(a_blendMode), opacity);

			auto task = SKSE::GetTaskInterface();
			task->AddTask([a_actor, newColor, root]() {
				SET::tint_face(a_actor, newColor);

				root->UpdateBodyTint(newColor);

				SET::update_color_data(root, EXTRA::SKIN_TINT, newColor);
			});
		}
	}

	inline std::int32_t CanActorBeDetected(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return -1;
		}

		const auto targetManager = DETECTION::TargetManager::GetSingleton();
		if (targetManager->Contains(a_actor, DETECTION::kHide)) {
			return 0;
		}
		if (targetManager->Contains(a_actor, DETECTION::kAlert)) {
			return 2;
		}
		return 1;
	}

	inline std::int32_t CanActorDetect(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return -1;
		}

		const auto sourceManager = DETECTION::SourceManager::GetSingleton();
		if (sourceManager->Contains(a_actor, DETECTION::kHide)) {
			return 0;
		}
		if (sourceManager->Contains(a_actor, DETECTION::kAlert)) {
			return 2;
		}
		return 1;
	}

	inline void DecapitateActor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		a_actor->Decapitate();
	}

	inline bool HasSkin(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::TESObjectARMO* a_check)
	{
		bool result = false;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return result;
		}
		if (!a_check) {
			return result;
		}
		if (!a_actor->Is3DLoaded()) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return result;
		}

		if (const auto arma = a_check->GetArmorAddon(a_actor->GetRace()); arma) {
			a_actor->VisitArmorAddon(a_check, arma, [&](bool, RE::NiAVObject& a_obj) -> void {
				if (a_obj.HasShaderType(RE::BSShaderMaterial::Feature::kFaceGenRGBTint)) {
					result = true;
				}
			});
		}

		return result;
	}

	inline void FreezeActor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		std::uint32_t a_type,
		bool a_enable)
	{
		using Flags = RE::CHARACTER_FLAGS;
		using BOOL_BITS = RE::Actor::BOOL_BITS;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		const auto root = a_actor->Get3D(false);
		if (!root) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return;
		}

		if (a_type == 0) {
			if (!a_enable) {
				a_actor->boolBits.set(BOOL_BITS::kProcessMe);  //enable AI first
			}
			if (const auto charController = a_actor->GetCharController(); charController) {
				if (a_enable) {  //freeze
					charController->flags.set(Flags::kNotPushable);
					charController->flags.reset(Flags::kRecordHits);
					charController->flags.reset(Flags::kHitFlags);
					charController->flags.reset(Flags::kHitDamage);
				} else {  //unfreeze
					charController->flags.reset(Flags::kNotPushable);
					charController->flags.set(Flags::kRecordHits);
					charController->flags.set(Flags::kHitFlags);
					charController->flags.set(Flags::kHitDamage);
				}
				charController->SetLinearVelocityImpl(0.0);
			}
			if (a_enable) {
				a_actor->boolBits.reset(BOOL_BITS::kProcessMe);  //disable AI last
			}
		} else if (a_type == 1) {
			if (const auto charController = a_actor->GetCharController(); charController) {
				auto task = SKSE::GetTaskInterface();
				task->AddTask([root, charController, a_actor, a_enable]() {
					std::uint32_t filterInfo = 0;
					charController->GetCollisionFilterInfo(filterInfo);
					if (a_enable) {
						a_actor->boolBits.set(BOOL_BITS::kParalyzed);
						root->UpdateRigidBodySettings(32, filterInfo + 1);
						root->SetRigidConstraints(true);
					} else {
						a_actor->boolBits.reset(BOOL_BITS::kParalyzed);
						root->SetRigidConstraints(false);
						root->UpdateRigidBodySettings(32, filterInfo >> 16);
					}
				});
			}
		}
	}

	inline void ForceActorDetection(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		DETECTION::TargetManager::GetSingleton()->Add(a_actor, DETECTION::kAlert);
	}

	inline void ForceActorDetecting(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		DETECTION::SourceManager::GetSingleton()->Add(a_actor, DETECTION::kAlert);
	}

	inline std::vector<RE::EffectSetting*> GetActiveEffects(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		bool a_inactive)
	{
		using AE = RE::ActiveEffect::Flag;

		std::vector<RE::EffectSetting*> result;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return result;
		}

		const auto activeEffects = a_actor->GetActiveEffectList();
		if (!activeEffects) {
			a_vm->TraceForm(a_actor, "has no active effects", a_stackID, Severity::kInfo);
			return result;
		}

		for (const auto& activeEffect : *activeEffects) {
			if (auto mgef = activeEffect ? activeEffect->GetBaseObject() : nullptr; mgef) {
				if (!a_inactive && (activeEffect->flags.all(AE::kInactive) || activeEffect->flags.all(AE::kDispelled))) {
					continue;
				}
				result.push_back(mgef);
			}
		}

		return result;
	}

	inline float GetActorAlpha(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return 1.0f;
		}

		const auto process = a_actor->currentProcess;
		const auto middleProcess = process ? process->middleHigh : nullptr;

		return middleProcess ? middleProcess->alphaMult : 1.0f;
	}

	inline float GetActorRefraction(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return 1.0f;
		}

		const auto process = a_actor->currentProcess;
		const auto middleProcess = process ? process->middleHigh : nullptr;

		return middleProcess ? middleProcess->scriptRefractPower : 1.0f;
	}

	inline std::int32_t GetActorSoulSize(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return -1;
		}

		return stl::to_underlying(a_actor->GetSoulSize());
	}

	inline std::int32_t GetActorState(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return -1;
		}

		return stl::to_underlying(a_actor->GetLifeState());
	}

	inline float GetActorValueModifier(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::Actor* a_actor,
		std::int32_t a_modifier,
		RE::BSFixedString a_actorValue)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return 0.0f;
		}

		const auto actorValueList = RE::ActorValueList::GetSingleton();
		const auto actorValue = actorValueList ?
                                    actorValueList->LookupActorValueByName(a_actorValue) :
                                    RE::ActorValue::kNone;

		const auto modifier = static_cast<RE::ACTOR_VALUE_MODIFIER>(a_modifier);
		return actorValue != RE::ActorValue::kNone ?
                   a_actor->GetActorValueModifier(modifier, actorValue) :
                   0.0f;
	}

	inline std::uint32_t GetCriticalStage(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return 0;
		}

		return stl::to_underlying(a_actor->criticalStage.get());
	}

	inline std::vector<RE::Actor*> GetCombatAllies(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		std::vector<RE::Actor*> result;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return result;
		}

		const auto combatGroup = a_actor->GetCombatGroup();
		if (combatGroup) {
			for (auto& memberData : combatGroup->members) {
				auto ally = memberData.handle.get();
				if (ally) {
					result.push_back(ally.get());
				}
			}
		}

		return result;
	}

	inline std::vector<RE::Actor*> GetCombatTargets(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		std::vector<RE::Actor*> result;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return result;
		}

		const auto combatGroup = a_actor->GetCombatGroup();
		if (combatGroup) {
			for (auto& targetData : combatGroup->targets) {
				auto target = targetData.targetHandle.get();
				if (target) {
					result.push_back(target.get());
				}
			}
		}

		return result;
	}

	inline std::vector<RE::Actor*> GetCommandedActors(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		std::vector<RE::Actor*> result;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return result;
		}

		const auto process = a_actor->currentProcess;
		const auto middleHigh = process ? process->middleHigh : nullptr;

		if (middleHigh) {
			for (auto& commandedActorData : middleHigh->commandedActors) {
				const auto commandedActor = commandedActorData.commandedActor.get();
				if (commandedActor) {
					result.push_back(commandedActor.get());
				}
			}
		}

		return result;
	}

	inline RE::Actor* GetCommandingActor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return nullptr;
		}

		if (a_actor->IsCommandedActor()) {
			const auto commanderPtr = a_actor->GetCommandingActor().get();
			return commanderPtr.get();
		}

		return nullptr;
	}

	inline RE::BGSColorForm* GetHairColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return nullptr;
		}

		const auto root = a_actor->Get3D(false);
		const auto data = root ? root->GetExtraData<RE::NiIntegerExtraData>(EXTRA::HAIR_TINT) : nullptr;

		if (data) {
			auto factory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::BGSColorForm>();
			auto color = factory ? factory->Create() : nullptr;
			if (color) {
				color->color = RE::Color(data->value);
				return color;
			}
		}

		const auto actorbase = a_actor->GetActorBase();
		const auto headData = actorbase ? actorbase->headRelatedData : nullptr;
		return headData ? headData->hairColor : nullptr;
	}

	inline RE::BGSTextureSet* GetHeadPartTextureSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		std::uint32_t a_type)
	{
		using HeadPartType = RE::BGSHeadPart::HeadPartType;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return nullptr;
		}

		const auto actorBase = a_actor->GetActorBase();
		const auto headpart = actorBase ? actorBase->GetCurrentHeadPartByType(static_cast<HeadPartType>(a_type)) : nullptr;

		return headpart ? headpart->textureSet : nullptr;
	}

	inline float GetLocalGravityActor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return 1.0f;
		}

		const auto charController = a_actor->GetCharController();
		return charController ? charController->gravity : 1.0f;
	}

	inline RE::TESObjectREFR* GetObjectUnderFeet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return nullptr;
		}

		const auto charController = a_actor->GetCharController();
		const auto supportBody = charController ? charController->supportBody : nullptr;

		return supportBody ? supportBody->GetUserData() : nullptr;
	}

	inline RE::TESPackage* GetRunningPackage(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return nullptr;
		}

		const auto currentProcess = a_actor->currentProcess;
		return currentProcess ? currentProcess->GetRunningPackage() : nullptr;
	}

	inline RE::BGSColorForm* GetSkinColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return nullptr;
		}

		const auto actorBase = a_actor->GetActorBase();
		const auto factory = actorBase ? RE::IFormFactory::GetConcreteFormFactoryByType<RE::BGSColorForm>() : nullptr;
		const auto color = factory ? factory->Create() : nullptr;

		if (color) {
			const auto root = a_actor->Get3D(false);
			const auto data = root ? root->GetExtraData<RE::NiIntegerExtraData>(EXTRA::SKIN_TINT) : nullptr;
			if (data) {
				color->color = RE::Color(data->value);
			} else {
				color->color = actorBase->bodyTintColor;
			}

			return color;
		}

		return nullptr;
	}

	inline float GetTimeDead(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return 0.0f;
		}

		const auto currentProcess = a_actor->currentProcess;
		const auto timeOfDeath = currentProcess ? currentProcess->deathTime : 0.0f;

		if (timeOfDeath > 0.0f) {
			if (const auto calendar = RE::Calendar::GetSingleton(); calendar) {
				const auto g_gameDaysPassed = calendar->gameDaysPassed;
				return g_gameDaysPassed ? floorf(g_gameDaysPassed->value * 24.0f) - timeOfDeath : 0.0f;
			}
		}

		return 0.0f;
	}

	inline float GetTimeOfDeath(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return 0.0f;
		}

		const auto currentProcess = a_actor->currentProcess;
		const auto timeOfDeath = currentProcess ? currentProcess->deathTime : 0.0f;

		return timeOfDeath > 0.0f ? timeOfDeath / 24.0f : 0.0f;
	}

	inline bool HasActiveSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		const RE::SpellItem* a_spell)
	{
		using AE = RE::ActiveEffect::Flag;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}
		if (!a_spell) {
			a_vm->TraceStack("Spell is None", a_stackID);
			return false;
		}

		if (const auto activeEffects = a_actor->GetActiveEffectList(); activeEffects) {
			return std::ranges::any_of(*activeEffects, [&](auto const& ae) {
				return ae && ae->spell == a_spell && ae->flags.none(AE::kInactive) && ae->flags.none(AE::kDispelled);
			});
		}

		return false;
	}

	inline bool HasDeferredKill(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		const auto currentProcess = a_actor->currentProcess;
		const auto middleProcess = currentProcess ? currentProcess->middleHigh : nullptr;

		return middleProcess && middleProcess->inDeferredKill;
	}

	inline bool HasMagicEffectWithArchetype(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::BSFixedString a_archetype)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}
		if (a_archetype.empty()) {
			a_vm->TraceStack("Archetype is None", a_stackID);
			return false;
		}

		if (const auto activeEffects = a_actor->GetActiveEffectList(); activeEffects) {
			return std::ranges::any_of(*activeEffects, [&](auto const& ae) {
				const auto mgef = ae ? ae->GetBaseObject() : nullptr;
				return mgef && MAGIC::get_archetype_as_string(mgef->GetArchetype()) == a_archetype;
			});
		}

		return false;
	}

	inline bool IsActorInWater(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		const float waterLevel = a_actor->GetSubmergedWaterLevel(a_actor->GetPositionZ(), a_actor->GetParentCell());
		return waterLevel >= 0.01f;
	}

	inline bool IsActorUnderwater(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		const float waterLevel = a_actor->GetSubmergedWaterLevel(a_actor->GetPositionZ(), a_actor->GetParentCell());
		return waterLevel >= 0.875f;
	}

	inline bool IsLimbGone(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		std::int32_t a_limbEnum)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		return a_actor->IsLimbGone(a_limbEnum);
	}

	inline bool IsQuadruped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		const auto charController = a_actor->GetCharController();
		return charController && charController->flags.all(RE::CHARACTER_FLAGS::kQuadruped);
	}

	inline bool IsSoulTrapped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		using Archetype = RE::EffectArchetypes::ArchetypeID;
		using Flags = RE::TESSoulGem::RecordFlags;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		const auto currentProcess = a_actor->currentProcess;
		const auto middleProcess = currentProcess ? currentProcess->middleHigh : nullptr;
		if (middleProcess && middleProcess->soulTrapped) {
			return true;
		}

		bool isBeingSoulTrapped = false;

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			const auto handle = a_actor->GetHandle();
			processLists->GetModelEffects([&](const RE::ModelReferenceEffect& a_modelEffect) {
				if (a_modelEffect.target == handle) {
					if (const auto modelArt = a_modelEffect.artObject; modelArt && modelArt->GetFormID() == SoulTrapHitArtID) {
						isBeingSoulTrapped = true;
						return false;
					}
				}
				return true;
			});
		}

		return isBeingSoulTrapped;
	}

	inline void KillNoWait(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		a_actor->KillImmediate();
		a_actor->boolBits.set(RE::Actor::BOOL_BITS::kDead);
		a_actor->boolBits.set(RE::Actor::BOOL_BITS::kSetOnDeath);
	}

	inline void MixColorWithSkinTone(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::BGSColorForm* a_color,
		bool a_manual,
		float a_percent)
	{
		a_vm->TraceStack("Function is deprecated, use BlendColorWithSkinTone instead", a_stackID);

		if (a_actor && a_color) {
			const auto actorbase = a_actor->GetActorBase();
			const auto root = a_actor->Get3D(false);
			if (actorbase && root) {
				const float skinLuminance = a_manual ? a_percent : COLOR::CalcLuminance(actorbase->bodyTintColor);
				auto newColor = COLOR::Mix(actorbase->bodyTintColor, a_color->color, skinLuminance);

				auto task = SKSE::GetTaskInterface();
				task->AddTask([a_actor, newColor, root]() {
					SET::tint_face(a_actor, newColor);

					root->UpdateBodyTint(newColor);

					SET::update_color_data(root, EXTRA::SKIN_TINT, newColor);
				});
			}
		}
	}

	inline void PreventActorDetection(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		DETECTION::TargetManager::GetSingleton()->Add(a_actor, DETECTION::kHide);
	}

	inline void PreventActorDetecting(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		DETECTION::SourceManager::GetSingleton()->Add(a_actor, DETECTION::kHide);
	}

	inline void RemoveAddedSpells(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::BSFixedString a_modName,
		std::vector<RE::BGSKeyword*> a_keywords,
		bool a_matchAll)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		std::vector<RE::SpellItem*> spells;

		auto dataHandler = RE::TESDataHandler::GetSingleton();
		auto mod = dataHandler ? dataHandler->LookupModByName(a_modName) : nullptr;

		constexpr auto has_keyword = [](RE::SpellItem* a_spell, const std::vector<RE::BGSKeyword*>& a_keywords, bool a_matchAll) {
			if (a_matchAll) {
				return std::ranges::all_of(a_keywords, [&](const auto& keyword) { return keyword && a_spell->HasKeyword(keyword); });
			} else {
				return std::ranges::any_of(a_keywords, [&](const auto& keyword) { return keyword && a_spell->HasKeyword(keyword); });
			}
		};

		for (auto& spell : a_actor->addedSpells | std::ranges::views::reverse) {
			if (!spell) {
				continue;
			}
			if (mod && !mod->IsFormInMod(spell->GetFormID())) {
				continue;
			}
			if (!a_keywords.empty() && !has_keyword(spell, a_keywords, a_matchAll)) {
				continue;
			}
			spells.push_back(spell);
		}

		//Papyrus RemoveSpell queues a task, while console command calls this directly. 
		auto taskQueue = RE::TaskQueueInterface::GetSingleton();
		if (taskQueue) {
			auto actorHandle = a_actor->CreateRefHandle();
			for (auto& spell : spells) {
				taskQueue->QueueRemoveSpellTask(actorHandle, spell);
			}
		}
	}

	inline bool RemoveBasePerk(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::BGSPerk* a_perk)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}
		if (!a_perk) {
			a_vm->TraceStack("Perk is None", a_stackID);
			return false;
		}

		return FORM::PerkManager::GetSingleton()->Remove(a_actor, a_perk);
	}

	inline bool RemoveBaseSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::SpellItem* a_spell)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}
		if (!a_spell) {
			a_vm->TraceStack("Spell is None", a_stackID);
			return false;
		}

		if (const auto activeEffects = a_actor->GetActiveEffectList(); activeEffects) {
			for (const auto& activeEffect : *activeEffects) {
				if (activeEffect && activeEffect->spell == a_spell) {
					activeEffect->Dispel(true);
				}
			}
		}

		const auto actorbase = a_actor->GetActorBase();
		const auto actorEffects = actorbase ? actorbase->actorEffects : nullptr;

		if (actorEffects && actorEffects->GetIndex(a_spell).has_value()) {
			if (const auto combatController = a_actor->combatController; combatController && combatController->inventoryController) {
				combatController->inventoryController->unk1C4 = 1;
			}
			a_actor->RemoveSelectedSpell(a_spell);

			return actorEffects->RemoveSpell(a_spell);
		}

		return false;
	}

	inline void ReplaceArmorTextureSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::TESObjectARMO* a_armor,
		RE::BGSTextureSet* a_srcTXST,
		RE::BGSTextureSet* a_tgtTXST,
		std::int32_t a_type)
	{
		using Texture = RE::BSShaderTextureSet::Textures::Texture;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}
		if (!a_armor) {
			a_vm->TraceStack("Armor is None", a_stackID);
			return;
		}
		if (!a_srcTXST) {
			a_vm->TraceStack("Source TextureSet is None", a_stackID);
			return;
		}
		if (!a_tgtTXST) {
			a_vm->TraceStack("Target TextureSet is None", a_stackID);
			return;
		}
		if (!a_actor->Is3DLoaded()) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return;
		}

		std::string targetPath{ a_srcTXST->GetTexturePath(Texture::kDiffuse) };
		TEXTURE::sanitize_path(targetPath);

		auto task = SKSE::GetTaskInterface();
		task->AddTask([a_actor, a_armor, a_srcTXST, a_tgtTXST, a_type, targetPath]() {
			bool replaced = false;

			if (const auto arma = a_armor->GetArmorAddon(a_actor->GetRace()); arma) {
				a_actor->VisitArmorAddon(a_armor, arma, [&](bool, RE::NiAVObject& a_obj) -> bool {
					SET::ArmorTXST(&a_obj, a_tgtTXST, a_type, targetPath, replaced);
					return true;
				});
			}

			const auto root = a_actor->Get3D(false);
			if (replaced && root) {
				auto armorID = std::to_string(a_armor->formID);
				const auto name = "PO3_TXST - " + armorID;

				const auto data = root->GetExtraData<RE::NiStringsExtraData>(name);
				if (!data) {
					std::vector<RE::BSFixedString> result;
					result.reserve(Texture::kTotal);
					for (const auto& type : TEXTURE::types) {
						result.emplace_back(a_srcTXST->GetTexturePath(type));
					}
					result.emplace_back(armorID);
					const auto newData = RE::NiStringsExtraData::Create(name, result);
					if (newData) {
						root->AddExtraData(newData);
					}
				}
			}
		});
	}

	inline void ReplaceFaceTextureSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::BGSTextureSet* a_maleTXST,
		RE::BGSTextureSet* a_femaleTXST,
		std::int32_t a_type)
	{
		using Texture = RE::BSShaderTextureSet::Texture;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}
		if (!a_actor->Is3DLoaded()) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return;
		}

		const auto actorBase = a_actor->GetActorBase();
		const bool isFemale = actorBase && actorBase->IsFemale();

		if (const auto txst = isFemale ? a_femaleTXST : a_maleTXST; txst) {
			auto task = SKSE::GetTaskInterface();
			task->AddTask([txst, a_type, a_actor]() {
				if (const auto faceObject = a_actor->GetHeadPartObject(RE::BGSHeadPart::HeadPartType::kFace); faceObject) {
					std::vector<RE::BSFixedString> result;
					SET::SkinTXST(faceObject, txst, result, a_type);

					const auto root = a_actor->Get3D(false);
					if (!result.empty() && root) {
						SET::add_data_if_none<RE::NiStringsExtraData>(root, EXTRA::FACE_TXST, result);
					}
				}
			});
		}
	}

	inline void ReplaceSkinTextureSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::BGSTextureSet* a_maleTXST,
		RE::BGSTextureSet* a_femaleTXST,
		std::uint32_t a_slot,
		std::int32_t a_type)
	{
		using BipedSlot = RE::BIPED_MODEL::BipedObjectSlot;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}
		if (!a_actor->Is3DLoaded()) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return;
		}

		const auto actorBase = a_actor->GetActorBase();
		const bool isFemale = actorBase && actorBase->IsFemale();

		if (isFemale && !a_femaleTXST) {
			a_vm->TraceStack("Female TextureSet is None", a_stackID);
			return;
		}
		if (!isFemale && !a_maleTXST) {
			a_vm->TraceStack("Male TextureSet is None", a_stackID);
			return;
		}

		SET::ArmorSkinTXST(a_actor,
			isFemale ? a_femaleTXST : a_maleTXST,
			static_cast<BipedSlot>(a_slot), a_type);
	}

	inline bool ResetActor3D(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::BSFixedString a_folderName)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		const auto root = a_actor->Get3D(false);
		if (!root) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return false;
		}

		bool result = true;
		RESET::ResetData resetData;

		std::tie(result, resetData) = RESET::get_data(root);
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
				RESET::ArmorTXST(a_actor, root, a_folderName, txstVec);
			}

			RESET::MaterialShader(root, shaderVec);
		});

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			if (!a_actor->IsPlayerRef()) {
				processLists->StopAllMagicEffects(*a_actor);
			} else {
				RESET::stop_all_skin_shaders(a_actor);
			}
		}

		return true;
	}

	inline void ResetActorDetection(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		DETECTION::TargetManager::GetSingleton()->Remove(a_actor);
	}

	inline void ResetActorDetecting(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		DETECTION::SourceManager::GetSingleton()->Remove(a_actor);
	}

	inline void SetActorRefraction(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		float a_refraction)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		const auto currentProcess = a_actor->currentProcess;
		if (currentProcess) {
			a_refraction = std::clamp(a_refraction, 0.0f, 1.0f);

			const auto middleHigh = currentProcess->middleHigh;
			if (middleHigh) {
				middleHigh->scriptRefractPower = a_refraction;
			}

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

	inline void SetHairColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::BGSColorForm* a_color)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}
		if (!a_color) {
			a_vm->TraceStack("Colorform is None", a_stackID);
			return;
		}

		const auto root = a_actor->Get3D(false);
		if (!root) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return;
		}

		auto task = SKSE::GetTaskInterface();
		task->AddTask([root, a_color]() {
			root->UpdateHairColor(a_color->color);
			SET::update_color_data(root, EXTRA::HAIR_TINT, a_color->color);
		});
	}

	inline void SetHeadPartAlpha(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		std::int32_t a_type,
		float a_alpha)
	{
		using HeadPartType = RE::BGSHeadPart::HeadPartType;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}
		if (a_type < 0 || a_type > 6) {
			a_vm->TraceStack("Invalid headpart type", a_stackID);
			return;
		}

		const auto root = a_actor->Get3D(false);
		if (!root) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return;
		}

		auto task = SKSE::GetTaskInterface();
		task->AddTask([root, a_actor, a_alpha, a_type]() {
			if (const auto object = a_actor->GetHeadPartObject(static_cast<HeadPartType>(a_type)); object) {
				object->UpdateMaterialAlpha(a_alpha, false);

				const auto name{ "PO3_HEADPART - " + std::to_string(a_type) };
				if (a_alpha == 1.0f) {
					root->RemoveExtraData(name);
				} else {
					SET::add_data_if_none<RE::NiIntegerExtraData>(root, name, a_type);
				}
			}
		});
	}

	inline void SetHeadPartTextureSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::BGSTextureSet* a_txst,
		std::int32_t a_type)
	{
		using HeadPartType = RE::BGSHeadPart::HeadPartType;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}
		if (!a_txst) {
			a_vm->TraceStack("Textureset is None", a_stackID);
			return;
		}
		if (a_type < 0 || a_type > 6) {
			a_vm->TraceStack("Invalid headpart type", a_stackID);
			return;
		}

		const auto actorBase = a_actor->GetActorBase();
		const auto headpart = actorBase ? actorBase->GetCurrentHeadPartByType(static_cast<HeadPartType>(a_type)) : nullptr;

		if (headpart) {
			headpart->textureSet = a_txst;
		} else {
			a_vm->TraceStack("Could not find matching headpart", a_stackID);
		}
	}

	inline void SetLinearVelocity(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		float a_x,
		float a_y,
		float a_z)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		if (const auto charController = a_actor->GetCharController(); charController) {
			charController->SetLinearVelocityImpl(RE::hkVector4(a_x * 0.0142875f, a_y * 0.0142875f, a_z * 0.0142875f, 0.0f));
		}
	}

	inline void SetLocalGravityActor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		float a_value,
		bool a_disableGravityOnGround)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		if (const auto charController = a_actor->GetCharController(); charController) {
			charController->SetLinearVelocityImpl(0.0f);

			a_disableGravityOnGround ?
                charController->flags.reset(RE::CHARACTER_FLAGS::kNoGravityOnGround) :
                charController->flags.set(RE::CHARACTER_FLAGS::kNoGravityOnGround);

			charController->gravity = a_value;
		}
	}

	inline void SetSkinAlpha(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		float a_alpha)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		const auto root = a_actor->Get3D(false);
		if (!root) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return;
		}

		auto task = SKSE::GetTaskInterface();
		task->AddTask([root, a_alpha]() {
			root->UpdateMaterialAlpha(a_alpha, true);

			if (a_alpha == 1.0f) {
				root->RemoveExtraData(EXTRA::SKIN_ALPHA);
			} else {
				SET::add_data_if_none<RE::NiBooleanExtraData>(root, EXTRA::SKIN_ALPHA, true);
			}
		});
	}

	inline void SetSkinColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::BGSColorForm* a_color)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}
		if (!a_color) {
			a_vm->TraceStack("Colorform is None", a_stackID);
			return;
		}

		const auto root = a_actor->Get3D(false);
		if (!root) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return;
		}

		auto task = SKSE::GetTaskInterface();
		task->AddTask([a_actor, a_color, root]() {
			SET::tint_face(a_actor, a_color->color);

			root->UpdateBodyTint(a_color->color);

			SET::update_color_data(root, EXTRA::SKIN_TINT, a_color->color);
		});
	}

	inline void SetSoulTrapped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		bool a_trapped)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		const auto process = a_actor->currentProcess;
		const auto middleProcess = process ? process->middleHigh : nullptr;

		if (middleProcess) {
			middleProcess->soulTrapped = a_trapped;
		}
	}

	inline void ToggleHairWigs(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		bool a_disable)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		const auto root = a_actor->Get3D(false);
		if (!root) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return;
		}

		const auto task = SKSE::GetTaskInterface();
		task->AddTask([a_actor, root, a_disable]() {
			if (const auto biped = a_actor->GetCurrentBiped().get(); biped) {
				for (auto& slot : headSlots) {
					const auto node = biped->objects[slot].partClone.get();
					if (node && node->HasShaderType(RE::BSShaderMaterial::Feature::kHairTint)) {
						SET::Toggle(root, node, a_disable);
					}
				}
			}
		});
	}

	inline void UnequipAllOfType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		std::uint32_t a_armorType,
		std::vector<std::uint32_t> a_slotsToSkip)
	{
		using Slot = RE::BIPED_MODEL::BipedObjectSlot;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		const auto armorType = static_cast<RE::BGSBipedObjectForm::ArmorType>(a_armorType);

		auto inv = a_actor->GetInventory([armorType, a_slotsToSkip](RE::TESBoundObject& a_object) {
			const auto armor = a_object.As<RE::TESObjectARMO>();
			if (armor && armor->GetArmorType() == armorType) {
				if (a_slotsToSkip.empty() || std::ranges::none_of(a_slotsToSkip,
												 [&](const auto& slot) {
													 return armor->HasPartOf(static_cast<Slot>(slot));
												 })) {
					return true;
				}
			}
			return false;
		});

		if (const auto equipManager = RE::ActorEquipManager::GetSingleton(); equipManager) {
			std::for_each(
				inv.begin(),
				inv.end(),
				[&](auto& invData) {
					const auto& [item, data] = invData;
					const auto& [count, entry] = data;
					if (count > 0 && entry->IsWorn()) {
						equipManager->UnequipObject(a_actor, item);
					}
				});
		}
	}

	inline void Bind(VM& a_vm)
	{
		BIND(AddBasePerk);
		BIND(AddBaseSpell);
		BIND(AddAllEquippedItemsToArray);
		BIND(BlendColorWithSkinTone);
		BIND(CanActorBeDetected);
		BIND(CanActorDetect);
		BIND(DecapitateActor);
		BIND(HasSkin);
		BIND(FreezeActor);
		BIND(ForceActorDetection);
		BIND(ForceActorDetecting);
		BIND(GetActiveEffects);
		BIND(GetActorAlpha);
		BIND(GetActorRefraction);
		BIND(GetActorSoulSize, true);
		BIND(GetActorState);
		BIND(GetActorValueModifier);
		BIND(GetCriticalStage);
		BIND(GetCombatAllies);
		BIND(GetCombatTargets);
		BIND(GetCommandedActors);
		BIND(GetCommandingActor);
		BIND(GetHairColor);
		BIND(GetHeadPartTextureSet);
		BIND(GetLocalGravityActor);
		BIND(GetObjectUnderFeet);
		BIND(GetRunningPackage);
		BIND(GetSkinColor);
		BIND(GetTimeDead);
		BIND(GetTimeOfDeath);
		BIND(HasActiveSpell);
		BIND(IsQuadruped, true);
		BIND(HasDeferredKill);
		BIND(HasMagicEffectWithArchetype);
		BIND(IsActorInWater, true);
		BIND(IsActorUnderwater, true);
		BIND(IsLimbGone);
		BIND(IsSoulTrapped);
		BIND(KillNoWait);
		BIND(MixColorWithSkinTone);
		BIND(PreventActorDetection);
		BIND(PreventActorDetecting);
		BIND(RemoveAddedSpells);
		BIND(RemoveBasePerk);
		BIND(RemoveBaseSpell);
		BIND(ReplaceArmorTextureSet);
		BIND(ReplaceFaceTextureSet);
		BIND(ReplaceSkinTextureSet);
		BIND(ResetActor3D);
		BIND(ResetActorDetection);
		BIND(ResetActorDetecting);
		BIND(SetActorRefraction);
		BIND(SetHairColor);
		BIND(SetHeadPartAlpha);
		BIND(SetHeadPartTextureSet);
		BIND(SetLinearVelocity);
		BIND(SetLocalGravityActor);
		BIND(SetSkinAlpha);
		BIND(SetSkinColor);
		BIND(SetSoulTrapped);
		BIND(ToggleHairWigs);
		BIND(UnequipAllOfType);

		logger::info("Registered actor functions"sv);
	}
}
