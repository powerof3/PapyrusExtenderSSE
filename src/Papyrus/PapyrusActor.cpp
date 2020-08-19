#include "Papyrus/PapyrusActor.h"

#include "Papyrus/PapyrusObjectReference.h"
#include "Serialization/Perks.h"
#include "Serialization/Reset.h"


bool papyrusActor::AddBasePerk(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSPerk* a_perk)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return false;
	} else if (!a_perk) {
		a_vm->TraceStack("Perk is None", a_stackID, Severity::kWarning);
		return false;
	}

	auto perks = Serialize::Perks::GetSingleton();
	return perks->PapyrusApplyPerks(a_actor, a_perk, Serialize::kAdd);
}


bool papyrusActor::AddBaseSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::SpellItem* a_spell)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return false;
	} else if (!a_spell) {
		a_vm->TraceStack("Spell is None", a_stackID, Severity::kWarning);
		return false;
	} else if (a_actor->HasSpell(a_spell)) {
		a_vm->TraceStack("Found existing spell on actor", a_stackID, Severity::kWarning);
		return false;
	}

	auto actorbase = a_actor->GetActorBase();
	if (actorbase) {
		auto combatController = a_actor->combatController;
		if (combatController) {
			combatController->data10->unk1C4 = 1;
		}
		auto actorEffects = actorbase->GetOrCreateSpellList();
		if (actorEffects && actorEffects->AddSpell(a_spell)) {
			return true;
		}
	}

	return false;
}


std::vector<RE::TESForm*> papyrusActor::AddAllEquippedItemsToArray(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
{
	std::vector<RE::TESForm*> vec;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return vec;
	}

	auto inv = a_actor->GetInventory();
	for (auto& item : inv) {
		auto& [count, entry] = item.second;
		if (entry->extraLists) {
			for (auto& list : *entry->extraLists) {
				if (list && list->GetWorn()) {
					vec.push_back(item.first);
				}
			}
		}
	}

	return vec;
}


void TintFace(RE::Actor* a_actor, const RE::NiColor& a_color)
{
	using HeadPartType = RE::BGSHeadPart::HeadPartType;

	auto object = a_actor->GetHeadPartObject(HeadPartType::kFace);
	if (object) {
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
	if (!data) {
		auto newData = RE::NiIntegerExtraData::Create(a_name, RE::NiColor::ColorToInt(a_color));
		if (newData) {
			a_root->AddExtraData(newData);
		}
	} else {
		auto color = RE::NiColor(data->value);
		if (a_color != color) {
			data->value = RE::NiColor::ColorToInt(a_color);
		}
	}
}


void papyrusActor::BlendColorWithSkinTone(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSColorForm* a_color, std::int32_t a_blendMode, bool a_autoCalc, float a_opacity)
{
	using BLEND_MODE = RE::NiColor::BLEND_MODE;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_color) {
		a_vm->TraceStack("Colorform is None", a_stackID, Severity::kWarning);
		return;
	}

	auto actorbase = a_actor->GetActorBase();
	if (actorbase) {
		auto root = a_actor->Get3D(0);
		if (!root) {
			a_vm->TraceStack("Actor has no 3D", a_stackID, Severity::kWarning);
			return;
		}

		const float opacity = a_autoCalc ? std::clamp(a_opacity * RE::NiColor::CalcLuminance(actorbase->bodyTintColor), 0.0f, 1.0f) : a_opacity;
		auto newColor = RE::NiColor::Blend(actorbase->bodyTintColor, a_color->color, static_cast<BLEND_MODE>(a_blendMode), opacity);

		auto task = SKSE::GetTaskInterface();
		task->AddTask([a_actor, newColor, root]() {
			TintFace(a_actor, newColor);
			root->UpdateBodyTint(newColor);
		});

		AddOrUpdateColorData(root, "PO3_SKINTINT", newColor);
	}
}


void papyrusActor::DecapitateActor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
	}

	a_actor->Decapitate();
}


void papyrusActor::EquipArmorIfSkinVisible(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::TESObjectARMO* a_check, RE::TESObjectARMO* a_equip)
{
	using Feature = RE::BSShaderMaterial::Feature;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_check) {
		a_vm->TraceStack("ArmorToCheck is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_equip) {
		a_vm->TraceStack("ArmorToEquip is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_actor->Is3DLoaded()) {
		a_vm->TraceStack("Actor has no 3D", a_stackID, Severity::kWarning);
		return;
	}

	auto task = SKSE::GetTaskInterface();
	task->AddTask([a_actor, a_check, a_equip]() {
		for (const auto& arma : a_check->armorAddons) {
			if (arma) {
				auto armorObject = a_actor->VisitArmorAddon(a_check, arma);
				if (armorObject && armorObject->HasShaderType(Feature::kFaceGenRGBTint)) {
					a_actor->AddWornItem(a_equip, 1, false, 0, 0);
					break;
				}
			}
		}
	});
}


void papyrusActor::FreezeActor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, bool a_enable)
{
	using Flags = RE::CHARACTER_FLAGS;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return;
	}

	if (!a_enable) {
		a_actor->boolBits |= RE::Actor::BOOL_BITS::kProcessMe;	//enable AI first
	}

	auto currentProcess = a_actor->currentProcess;
	if (currentProcess) {
		auto charController = currentProcess->GetCharController();
		if (charController) {
			if (a_enable) {
				charController->flags &= ~Flags::kRecordHits;									  //disable hit processing
				charController->SetLinearVelocityImpl(RE::hkVector4(1000.0f, 0.0f, 0.0f, 0.0f));  //set velocity to 0
			} else {
				charController->flags |= Flags::kRecordHits;  //enable hit processing
			}
		}
	}

	if (a_enable) {
		a_actor->boolBits &= ~RE::Actor::BOOL_BITS::kProcessMe;	 // disable AI last
	}
}


float papyrusActor::GetActorAlpha(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return 1.0f;
	}

	auto currentProcess = a_actor->currentProcess;
	if (currentProcess) {
		auto middleProcess = currentProcess->middleHigh;
		if (middleProcess) {
			return middleProcess->alphaMult;
		}
	}

	return 1.0f;
}


float papyrusActor::GetActorRefraction(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return 1.0f;
	}

	auto currentProcess = a_actor->currentProcess;
	if (currentProcess) {
		auto middleProcess = currentProcess->middleHigh;
		if (middleProcess) {
			return middleProcess->scriptRefractPower;
		}
	}

	return 1.0f;
}


std::int32_t papyrusActor::GetActorState(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return -1;
	}

	return to_underlying(a_actor->GetLifeState());
}


std::vector<RE::EffectSetting*> papyrusActor::GetAllActiveEffectsOnActor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, bool a_inactive)
{
	using MGEF = RE::EffectSetting::EffectSettingData::Flag;
	using AE = RE::ActiveEffect::Flag;

	std::vector<RE::EffectSetting*> vec;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return vec;
	}

	auto activeEffects = a_actor->GetActiveEffectList();
	if (!activeEffects) {
		a_vm->TraceStack("Actor has no active effects", a_stackID, Severity::kInfo);
		return vec;
	}

	for (auto& activeEffect : *activeEffects) {
		if (activeEffect) {
			auto mgef = activeEffect->GetBaseObject();
			if (mgef) {
				if (!a_inactive && (activeEffect->flags.all(AE::kInactive) || activeEffect->flags.all(AE::kDispelled) ||
									   mgef->data.flags.all(MGEF::kHideInUI))) {
					continue;
				}
				vec.push_back(mgef);
			}
		}
	}

	return vec;
}


RE::BGSColorForm* papyrusActor::GetHairColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	auto actorbase = a_actor->GetActorBase();
	if (actorbase) {
		auto root = a_actor->Get3D(0);
		if (root) {
			auto data = root->GetExtraData<RE::NiIntegerExtraData>("PO3_HAIRTINT");
			if (data) {
				auto factory = RE::IFormFactory::GetFormFactoryByType(RE::FormType::ColorForm);
				auto color = static_cast<RE::BGSColorForm*>(factory->Create());
				if (color) {
					color->flags &= ~RE::BGSColorForm::Flag::kPlayable;
					color->color = RE::Color(data->value);
					return color;
				}
			}
		} else {
			auto headData = actorbase->headRelatedData;
			if (headData) {
				return headData->hairColor;
			}
		}
	}

	return nullptr;
}


RE::BGSTextureSet* papyrusActor::GetHeadPartTextureSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, std::uint32_t a_type)
{
	using HeadPartType = RE::BGSHeadPart::HeadPartType;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	auto actorBase = a_actor->GetActorBase();
	if (actorBase) {
		auto headpart = actorBase->GetCurrentHeadPartByType(static_cast<HeadPartType>(a_type));
		if (headpart) {
			return headpart->textureSet;
		} else {
			a_vm->TraceStack("Could not find matching headpart", a_stackID, Severity::kWarning);
		}
	}

	return nullptr;
}


RE::TESPackage* papyrusActor::GetRunningPackage(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
{
	RE::TESPackage* package = nullptr;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return package;
	}

	auto currentProcess = a_actor->currentProcess;
	if (currentProcess) {
		auto middleHigh = currentProcess->middleHigh;
		if (middleHigh) {
			package = middleHigh->runOncePackage.package;
		}
		if (!package) {
			package = currentProcess->currentPackage.package;
		}
	}

	return package;
}


RE::BGSColorForm* papyrusActor::GetSkinColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	auto actorBase = a_actor->GetActorBase();
	if (actorBase) {
		auto factory = RE::IFormFactory::GetFormFactoryByType(RE::FormType::ColorForm);
		auto color = static_cast<RE::BGSColorForm*>(factory->Create());
		if (color) {
			color->flags &= ~RE::BGSColorForm::Flag::kPlayable;
			color->color = actorBase->bodyTintColor;
			auto root = a_actor->Get3D(0);
			if (root) {
				auto data = root->GetExtraData<RE::NiIntegerExtraData>("PO3_SKINTINT");
				if (data) {
					color->color = RE::Color(data->value);
				}
			}
			return color;
		}
	}

	return nullptr;
}


float papyrusActor::GetTimeDead(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	auto currentProcess = a_actor->currentProcess;
	if (currentProcess) {
		float timeOfDeath = currentProcess->deathTime;
		if (timeOfDeath > 0.0f) {
			auto calendar = RE::Calendar::GetSingleton();
			if (calendar) {
				auto g_gameDaysPassed = calendar->gameDaysPassed;
				return g_gameDaysPassed ? floorf(g_gameDaysPassed->value * 24.0f) - timeOfDeath : 0.0f;
			}
		} else {
			a_vm->TraceStack("Actor is not dead", a_stackID, Severity::kWarning);
		}
	}

	return 0.0f;
}


float papyrusActor::GetTimeOfDeath(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	auto currentProcess = a_actor->currentProcess;
	if (currentProcess) {
		float timeOfDeath = currentProcess->deathTime;
		if (timeOfDeath > 0.0f) {
			return timeOfDeath / 24.0f;
		} else {
			a_vm->TraceStack("Actor is not dead", a_stackID, Severity::kWarning);
		}
	}

	return 0.0f;
}


bool papyrusActor::HasActiveSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::SpellItem* a_spell)
{
	using AE = RE::ActiveEffect::Flag;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return false;
	} else if (!a_spell) {
		a_vm->TraceStack("Spell is None", a_stackID, Severity::kWarning);
		return false;
	}

	auto activeEffects = a_actor->GetActiveEffectList();
	if (!activeEffects) {
		a_vm->TraceStack("Actor has no active effects", a_stackID, Severity::kInfo);
		return false;
	}

	for (auto& activeEffect : *activeEffects) {
		if (activeEffect && activeEffect->spell && activeEffect->spell == a_spell) {
			if (activeEffect->flags.none(AE::kInactive) && activeEffect->flags.none(AE::kDispelled)) {
				return true;
			}
		}
	}

	return false;
}


bool papyrusActor::HasMagicEffectWithArchetype(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BSFixedString a_archetype)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return false;
	} else if (a_archetype.empty()) {
		a_vm->TraceStack("Archetype is None", a_stackID, Severity::kWarning);
		return false;
	}

	auto activeEffects = a_actor->GetActiveEffectList();
	if (!activeEffects) {
		a_vm->TraceStack("Actor has no active effects", a_stackID, Severity::kInfo);
		return false;
	}

	for (auto& activeEffect : *activeEffects) {
		if (activeEffect) {
			const auto mgef = activeEffect->GetBaseObject();
			if (mgef && mgef->GetArchetypeAsString() == a_archetype) {
				return true;
			}
		}
	}

	return false;
}


bool papyrusActor::InstantKill(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return false;
	}

	return a_actor->InstantKill();
}


bool papyrusActor::IsActorInWater(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return false;
	}

	return a_actor->boolBits.all(RE::Actor::BOOL_BITS::kInWater);
}


bool papyrusActor::IsActorSoulTrapped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
{
	using Archetype = RE::EffectArchetypes::ArchetypeID;
	using Flags = RE::TESSoulGem::RecordFlags;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return false;
	}

	bool isNPC = a_actor->HasKeyword("ActorTypeNPC");
	bool hasSoulTrap = false;
	RE::Actor* caster = nullptr;

	auto activeEffects = a_actor->GetActiveEffectList();
	if (!activeEffects) {
		a_vm->TraceStack("Actor has no active effects", a_stackID, Severity::kInfo);
		return false;
	}

	for (auto& activeEffect : *activeEffects) {
		if (activeEffect) {
			auto a_mgef = activeEffect->GetBaseObject();
			if (a_mgef) {
				auto hitArt = a_mgef->data.hitEffectArt;
				if (hitArt && hitArt->formID == SoulTrapHitArtID || a_mgef->data.archetype == Archetype::kSoulTrap) {
					auto casterPtr = activeEffect->GetCasterActor();
					if (!casterPtr.get()) {
						a_vm->TraceStack("Could not get Soul Trap caster ", a_stackID, Severity::kWarning);
						return false;
					}
					caster = casterPtr.get();
					hasSoulTrap = true;
					break;
				}
			}
		}
	}

	if (hasSoulTrap && caster) {
		auto inv = caster->GetInventory([](RE::TESBoundObject& a_object) -> bool {
			return a_object.IsSoulGem();
		});

		for (auto& item : inv) {
			auto soulGem = static_cast<RE::TESSoulGem*>(item.first);
			if (soulGem) {
				auto& [count, entry] = item.second;
				if (entry->GetSoulLevel() == RE::SOUL_LEVEL::kNone) {
					return isNPC ? (soulGem->formFlags & Flags::kCanHoldNPCSoul) == Flags::kCanHoldNPCSoul : soulGem->soulCapacity >= a_actor->GetSoulLevel();
				}
			}
		}
	}

	return false;
}


bool papyrusActor::IsActorUnderwater(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return false;
	}

	return a_actor->boolFlags.all(RE::Actor::BOOL_FLAGS::kUnderwater);
}


void papyrusActor::MixColorWithSkinTone(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSColorForm* a_color, bool a_manual, float a_percent)
{
	a_vm->TraceStack("Function is deprecated, use BlendColorWithSkinTone instead", a_stackID, Severity::kError);

	if (a_actor && a_color) {
		auto actorbase = a_actor->GetActorBase();
		if (actorbase) {
			auto root = a_actor->Get3D(0);
			if (root) {
				float skinLuminance = a_manual ? a_percent : RE::NiColor::CalcLuminance(actorbase->bodyTintColor);
				auto newColor = RE::NiColor::Mix(actorbase->bodyTintColor, a_color->color, skinLuminance);

				auto task = SKSE::GetTaskInterface();
				task->AddTask([a_actor, newColor, root]() {
					TintFace(a_actor, newColor);
					root->UpdateBodyTint(newColor);
				});

				AddOrUpdateColorData(root, "PO3_SKINTINT", newColor);
			}
		}
	}
}


bool papyrusActor::RemoveBasePerk(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSPerk* a_perk)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return false;
	} else if (!a_perk) {
		a_vm->TraceStack("Perk is None", a_stackID, Severity::kWarning);
		return false;
	}

	auto perks = Serialize::Perks::GetSingleton();
	return perks->PapyrusApplyPerks(a_actor, a_perk, Serialize::kRemove);
}


bool papyrusActor::RemoveBaseSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::SpellItem* a_spell)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return false;
	} else if (!a_spell) {
		a_vm->TraceStack("Spell is None", a_stackID, Severity::kWarning);
		return false;
	}

	auto actorbase = a_actor->GetActorBase();
	if (actorbase) {
		auto actorEffects = actorbase->actorEffects;
		if (actorEffects) {
			if (actorEffects->GetIndex(a_spell) == -1) {
				a_vm->TraceStack("Spell is not active on actor", a_stackID, Severity::kWarning);
				return false;
			}
			auto activeEffects = a_actor->GetActiveEffectList();
			if (activeEffects) {
				for (auto& activeEffect : *activeEffects) {
					if (activeEffect && activeEffect->spell && activeEffect->spell == a_spell) {
						activeEffect->Dispel(true);
					}
				}
			}
			auto combatController = a_actor->combatController;
			if (combatController) {
				combatController->data10->unk1C4 = 1;
			}
			return actorEffects->RemoveSpell(a_spell);
		}
	}

	return false;
}


void papyrusActor::RemoveEffectsNotOfType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, std::int32_t a_type)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return;
	}

	auto root = a_actor->Get3D(0);
	if (!root) {
		a_vm->TraceStack("Actor has no 3D", a_stackID, Severity::kWarning);
		return;
	}

	auto [toggleData, skinTintData, hairTintData, alphaData, headpartAlphaData, txstFaceData, txstVec, txstSkinVec, shaderVec] = Reset::GetResetData(root);

	switch (static_cast<EFFECT>(a_type)) {
	case EFFECT::kCharred:
		{
			if (skinTintData) {
				Reset::ResetSkinTintData(a_actor, root, skinTintData);
			}
			if (hairTintData) {
				Reset::ResetHairTintData(a_actor, root, hairTintData);
			}
			if (txstFaceData) {
				Reset::ResetFaceTXSTData(a_actor, root, txstFaceData);
			}
			if (!txstSkinVec.empty()) {
				Reset::ResetSkinTXSTData(a_actor, root, txstSkinVec);
			}
		}
		break;
	case EFFECT::kDrained:
		{
			if (toggleData) {
				Reset::ResetToggleData(root, toggleData);
			}
			if (skinTintData) {
				Reset::ResetSkinTintData(a_actor, root, skinTintData);
			}
			if (hairTintData) {
				Reset::ResetHairTintData(a_actor, root, hairTintData);
			}
			if (txstFaceData) {
				Reset::ResetFaceTXSTData(a_actor, root, txstFaceData);
			}
			if (!txstSkinVec.empty()) {
				Reset::ResetSkinTXSTData(a_actor, root, txstSkinVec);
			}
		}
		break;
	case EFFECT::kPoisoned:
		{
			if (!a_actor->IsPlayerRef()) {
				papyrusObjectReference::StopAllSkinAlphaShaders_Impl(a_actor);
			}
			if (toggleData) {
				Reset::ResetToggleData(root, toggleData);
			}
			if (alphaData) {
				Reset::ResetAlphaData(a_actor, root, alphaData);
			}
			if (headpartAlphaData) {
				Reset::ResetHeadPartAlphaData(a_actor, root, headpartAlphaData);
			}
			if (txstFaceData) {
				Reset::ResetFaceTXSTData(a_actor, root, txstFaceData);
			}
			if (!txstSkinVec.empty()) {
				Reset::ResetSkinTXSTData(a_actor, root, txstSkinVec);
			}
		}
		break;
	case EFFECT::kAged:
		{
			if (!a_actor->IsPlayerRef()) {
				papyrusObjectReference::StopAllSkinAlphaShaders_Impl(a_actor);
			}
			if (toggleData) {
				Reset::ResetToggleData(root, toggleData);
			}
			if (alphaData) {
				Reset::ResetAlphaData(a_actor, root, alphaData);
			}
			if (headpartAlphaData) {
				Reset::ResetHeadPartAlphaData(a_actor, root, headpartAlphaData);
			}
		}
		break;
	case EFFECT::kCharredCreature:
		{
			if (!shaderVec.empty()) {
				Reset::ResetShaderData(root, shaderVec);
			}
		}
		break;
	default:
		{
			a_vm->TraceStack("Invalid effect type", a_stackID, Severity::kWarning);
		}
		break;
	}
}


void SetTXST(RE::NiAVObject* a_object, RE::BGSTextureSet* a_txst, std::int32_t a_type, std::string_view a_tgtPath, bool& replaced)
{
	using State = RE::BSGeometry::States;
	using Feature = RE::BSShaderMaterial::Feature;
	using Texture = RE::BSTextureSet::Texture;

	RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
		auto effect = a_geometry->properties[State::kEffect].get();
		if (effect) {
			auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect);
			if (lightingShader) {
				auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
				if (material) {
					std::string sourcePath = material->textureSet->GetTexturePath(Texture::kDiffuse);
					RE::Util::SanitizeTexturePath(sourcePath);

					if (sourcePath == a_tgtPath) {
						auto newMaterial = static_cast<RE::BSLightingShaderMaterialBase*>(material->Create());
						if (newMaterial) {
							newMaterial->CopyMembers(material);
							newMaterial->ClearTextures();

							if (a_type == -1) {
								newMaterial->OnLoadTextureSet(0, a_txst);
							} else {
								auto newTextureSet = RE::BSShaderTextureSet::Create();
								if (newTextureSet) {
									for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
										newTextureSet->SetTexturePath(i, material->textureSet->GetTexturePath(i));
									}
									auto BSTextureType = static_cast<Texture>(a_type);
									newTextureSet->SetTexturePath(BSTextureType, a_txst->GetTexturePath(BSTextureType));

									newMaterial->OnLoadTextureSet(0, newTextureSet);
								}
							}

							lightingShader->SetMaterial(newMaterial, 1);
							lightingShader->InitializeGeometry(a_geometry);
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
	} else if (!a_armor) {
		a_vm->TraceStack("Armor is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_srcTXST) {
		a_vm->TraceStack("Source textureset is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_tgtTXST) {
		a_vm->TraceStack("Target textureset is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_actor->Is3DLoaded()) {
		a_vm->TraceStack("Actor has no 3D", a_stackID, Severity::kWarning);
		return;
	}

	std::string targetPath = a_srcTXST->GetTexturePath(Texture::kDiffuse);
	RE::Util::SanitizeTexturePath(targetPath);

	auto task = SKSE::GetTaskInterface();
	task->AddTask([a_actor, a_armor, a_srcTXST, a_tgtTXST, a_type, targetPath]() {
		bool replaced = false;
		for (const auto& armorAddon : a_armor->armorAddons) {
			if (armorAddon) {
				auto armorObject = a_actor->VisitArmorAddon(a_armor, armorAddon);
				if (armorObject) {
					SetTXST(armorObject, a_tgtTXST, a_type, targetPath, replaced);
				}
			}
		}

		auto root = a_actor->Get3D(0);
		if (replaced && root) {
			auto armorID = std::to_string(a_armor->formID);
			std::string name = "PO3_TXST - " + armorID;

			auto data = root->GetExtraData<RE::NiStringsExtraData>(name.c_str());
			if (!data) {
				std::vector<RE::BSFixedString> vec;
				vec.reserve(Texture::kTotal);
				for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
					vec.emplace_back(a_srcTXST->GetTexturePath(i));
				}
				vec.emplace_back(armorID);
				auto newData = RE::NiStringsExtraData::Create(name.c_str(), vec);
				if (newData) {
					root->AddExtraData(newData);
				}
			}
		}
	});
}


void SetSkinTXST(RE::NiAVObject* a_object, RE::BGSTextureSet* a_txst, std::vector<RE::BSFixedString>& a_vec, std::int32_t a_type)
{
	using State = RE::BSGeometry::States;
	using Feature = RE::BSShaderMaterial::Feature;
	using Texture = RE::BSTextureSet::Texture;

	RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
		auto effect = a_geometry->properties[State::kEffect].get();
		if (effect) {
			auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect);
			if (lightingShader) {
				auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
				if (material) {
					auto type = material->GetFeature();
					if (type == Feature::kFaceGenRGBTint || type == Feature::kFaceGen) {
						if (a_vec.empty()) {
							for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
								a_vec.emplace_back(material->textureSet->GetTexturePath(i));
							}
						}

						auto newMaterial = static_cast<RE::BSLightingShaderMaterialBase*>(material->Create());
						if (newMaterial) {
							newMaterial->CopyMembers(material);
							newMaterial->ClearTextures();

							if (a_type == -1) {
								if (type == Feature::kFaceGen) {
									auto newTextureSet = RE::BSShaderTextureSet::Create();
									if (newTextureSet) {
										for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
											newTextureSet->SetTexturePath(i, a_txst->GetTexturePath(i));
											newTextureSet->SetTexturePath(Texture::kMultilayer, material->textureSet->GetTexturePath(Texture::kMultilayer));
											newMaterial->OnLoadTextureSet(0, newTextureSet);
										}
									}
								} else {
									newMaterial->OnLoadTextureSet(0, a_txst);
								}
							} else {
								auto newTextureSet = RE::BSShaderTextureSet::Create();
								if (newTextureSet) {
									for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
										newTextureSet->SetTexturePath(i, material->textureSet->GetTexturePath(i));
									}
									auto BSTextureType = static_cast<Texture>(a_type);
									newTextureSet->SetTexturePath(BSTextureType, a_txst->GetTexturePath(BSTextureType));

									newMaterial->OnLoadTextureSet(0, newTextureSet);
								}
							}

							lightingShader->SetMaterial(newMaterial, 1);
							lightingShader->InitializeGeometry(a_geometry);
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
	} else if (!a_actor->Is3DLoaded()) {
		a_vm->TraceStack("Actor has no 3D", a_stackID, Severity::kWarning);
		return;
	}

	auto actorBase = a_actor->GetActorBase();
	bool isFemale = actorBase ? actorBase->IsFemale() : false;

	auto txst = isFemale ? a_femaleTXST : a_maleTXST;
	if (!txst) {
		a_vm->TraceStack("Textureset is None", a_stackID, Severity::kWarning);
		return;
	}

	auto faceObject = a_actor->GetHeadPartObject(RE::BGSHeadPart::HeadPartType::kFace);
	if (faceObject) {
		auto task = SKSE::GetTaskInterface();
		task->AddTask([a_actor, a_type, faceObject, txst]() {
			std::vector<RE::BSFixedString> vec;
			vec.reserve(Texture::kTotal);
			SetSkinTXST(faceObject, txst, vec, a_type);

			auto root = a_actor->Get3D(0);
			if (!vec.empty() && root) {
				auto data = root->GetExtraData<RE::NiStringsExtraData>("PO3_FACETXST");
				if (!data) {
					auto newData = RE::NiStringsExtraData::Create("PO3_FACETXST", vec);
					if (newData) {
						root->AddExtraData(newData);
					}
				}
			}
		});
	} else {
		a_vm->TraceStack("Cannot get face headpart", a_stackID, Severity::kWarning);
	}
}


void SetArmorSkinTXST(RE::Actor* a_actor, RE::BGSTextureSet* a_txst, RE::BGSBipedObjectForm::BipedObjectSlot a_slot, std::int32_t a_type)
{
	auto skinArmor = a_actor->GetSkin(a_slot);
	if (!skinArmor) {
		return;
	}

	auto foundAddon = skinArmor->GetArmorAddonByMask(a_actor->race, a_slot);
	if (!foundAddon) {
		return;
	}

	auto armorObject = a_actor->VisitArmorAddon(skinArmor, foundAddon);
	if (armorObject) {
		auto task = SKSE::GetTaskInterface();
		task->AddTask([a_actor, a_txst, a_slot, a_type, armorObject]() {
			std::vector<RE::BSFixedString> vec;
			vec.reserve(10);
			SetSkinTXST(armorObject, a_txst, vec, a_type);

			auto root = a_actor->Get3D(0);
			if (!vec.empty() && root) {
				auto slotMaskStr = std::to_string(to_underlying(a_slot));
				std::string name = "PO3_SKINTXST - " + slotMaskStr;
				vec.emplace_back(slotMaskStr.c_str());

				auto data = root->GetExtraData<RE::NiStringsExtraData>(name.c_str());
				if (!data) {
					auto newData = RE::NiStringsExtraData::Create(name.c_str(), vec);
					if (newData) {
						root->AddExtraData(newData);
					}
				}
			}
		});
	}
}


void papyrusActor::ReplaceSkinTextureSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSTextureSet* a_maleTXST, RE::BGSTextureSet* a_femaleTXST, std::uint32_t a_slot, std::int32_t a_type)
{
	using BipedSlot = RE::BGSBipedObjectForm::BipedObjectSlot;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return;
	} else if (!a_actor->Is3DLoaded()) {
		a_vm->TraceStack("Actor has no 3D", a_stackID, Severity::kWarning);
		return;
	}

	auto actorBase = a_actor->GetActorBase();
	bool isFemale = actorBase ? actorBase->IsFemale() : false;

	if (isFemale) {
		if (!a_femaleTXST) {
			a_vm->TraceStack("Female textureset is None", a_stackID, Severity::kWarning);
			return;
		}
		SetArmorSkinTXST(a_actor, a_femaleTXST, static_cast<BipedSlot>(a_slot), a_type);
	} else {
		if (!a_maleTXST) {
			a_vm->TraceStack("Male textureset is None", a_stackID, Severity::kWarning);
			return;
		}
		SetArmorSkinTXST(a_actor, a_maleTXST, static_cast<BipedSlot>(a_slot), a_type);
	}
}


bool papyrusActor::ResetActor3D(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BSFixedString a_folderName)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return false;
	}

	auto root = a_actor->Get3D(0);
	if (!root) {
		a_vm->TraceStack("Actor has no 3D", a_stackID, Severity::kWarning);
		return false;
	}

	auto [toggleData, skinTintData, hairTintData, alphaData, headpartAlphaData, txstFaceData, txstVec, txstSkinVec, shaderVec] = Reset::GetResetData(root);

	if (!toggleData && !alphaData && !headpartAlphaData && !skinTintData && !hairTintData && !txstFaceData && txstVec.empty() && txstSkinVec.empty() && shaderVec.empty()) {
		return false;
	} else {
		if (!a_actor->IsPlayerRef()) {
			papyrusObjectReference::StopAllShaders_Impl(a_actor);
		}

		if (toggleData) {
			Reset::ResetToggleData(root, toggleData);
		}
		if (alphaData) {
			Reset::ResetAlphaData(a_actor, root, alphaData);
		}
		if (headpartAlphaData) {
			Reset::ResetHeadPartAlphaData(a_actor, root, headpartAlphaData);
		}
		if (skinTintData) {
			Reset::ResetSkinTintData(a_actor, root, skinTintData);
		}
		if (hairTintData) {
			Reset::ResetHairTintData(a_actor, root, hairTintData);
		}
		if (txstFaceData) {
			Reset::ResetFaceTXSTData(a_actor, root, txstFaceData);
		}
		if (!txstSkinVec.empty()) {
			Reset::ResetSkinTXSTData(a_actor, root, txstSkinVec);
		}
		if (!txstVec.empty() && !a_folderName.empty()) {
			Reset::ResetTXSTData(a_actor, root, a_folderName, txstVec);
		}
		if (!shaderVec.empty()) {
			Reset::ResetShaderData(root, shaderVec);
		}

		return true;
	}

	return false;
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
		currentProcess->SetActorRefraction(a_refraction);

		float invisibility = a_actor->GetActorValue(RE::ActorValue::kInvisibility);
		if (invisibility < 0.0 || invisibility <= 1.0 && invisibility <= 0.0 || !a_actor->IsPlayerRef()) {
			if (a_refraction <= 0.0) {
				a_actor->SetRefraction(0, a_refraction);
				a_actor->UpdateAlpha();
			} else {
				a_actor->SetRefraction(1, a_refraction);
			}
		} else {
			a_actor->SetAlpha(1.0);

			a_refraction = 1.0f - a_refraction / 100.0f;
			a_refraction = 1.0f + (0.01f - 1.0f) * ((a_refraction - 0.0f) / (1.0f - 0.0f));

			a_actor->SetRefraction(1, a_refraction);
		}
	}
}


void papyrusActor::SetHairColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSColorForm* a_color)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return;
	} else if (!a_color) {
		a_vm->TraceStack("Colorform is None", a_stackID, Severity::kWarning);
		return;
	}

	auto root = a_actor->Get3D(0);
	if (root) {
		auto task = SKSE::GetTaskInterface();
		task->AddTask([a_color, root]() {
			root->UpdateHairColor(a_color->color);
		});

		AddOrUpdateColorData(root, "PO3_HAIRTINT", a_color->color);
	} else {
		a_vm->TraceStack("Actor has no 3D", a_stackID, Severity::kWarning);
	}
}


void papyrusActor::SetHeadPartAlpha(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, std::int32_t a_type, float a_alpha)
{
	using HeadPartType = RE::BGSHeadPart::HeadPartType;
	using ALPHA_MODE = RE::NiAVObject::ALPHA_MODE;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return;
	} else if (a_type < 0 || a_type > 6) {
		a_vm->TraceStack("Invalid headpart type", a_stackID, Severity::kWarning);
		return;
	}

	auto root = a_actor->Get3D(0);
	if (!root) {
		a_vm->TraceStack("Actor has no 3D", a_stackID, Severity::kWarning);
		return;
	}

	auto object = a_actor->GetHeadPartObject(static_cast<HeadPartType>(a_type));
	if (object) {
		auto task = SKSE::GetTaskInterface();
		task->AddTask([object, a_alpha]() {
			object->UpdateAlpha(a_alpha, ALPHA_MODE::kAll);
		});

		auto data = root->GetExtraData<RE::NiIntegersExtraData>("PO3_HEADPARTALPHA");
		if (!data) {
			if (a_alpha == 0.0f) {
				std::vector<std::int32_t> vec;
				vec.push_back(a_type);
				auto newData = RE::NiIntegersExtraData::Create("PO3_HEADPARTALPHA", vec);
				if (newData) {
					root->AddExtraData(newData);
				}
			}
		} else {
			a_alpha == 0.0 ? data->InsertElement(a_type) : data->RemoveElement(a_type);
		}
	} else {
		a_vm->TraceStack("Could not find matching headpart object", a_stackID, Severity::kWarning);
	}
}


void papyrusActor::SetHeadPartTextureSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSTextureSet* a_txst, std::int32_t a_type)
{
	using HeadPartType = RE::BGSHeadPart::HeadPartType;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return;
	} else if (!a_txst) {
		a_vm->TraceStack("Textureset is None", a_stackID, Severity::kWarning);
		return;
	} else if (a_type < 0 || a_type > 6) {
		a_vm->TraceStack("Invalid headpart type", a_stackID, Severity::kWarning);
		return;
	}

	auto actorBase = a_actor->GetActorBase();
	if (actorBase) {
		auto headpart = actorBase->GetCurrentHeadPartByType(static_cast<HeadPartType>(a_type));
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

	auto controller = a_actor->GetCharController();
	if (controller) {
		auto proxy = static_cast<RE::bhkCharProxyController*>(controller);
		if (proxy) {
			RE::hkVector4 vector(a_x, a_y, a_z, 0.0f);
			proxy->SetLinearVelocityImpl(vector);
		}
	}
}


void papyrusActor::SetLocalGravity(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, float a_value, bool a_disableGravityOnGround)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return;
	}

	auto controller = a_actor->GetCharController();
	if (controller) {
		auto proxy = static_cast<RE::bhkCharProxyController*>(controller);
		if (proxy) {
			a_disableGravityOnGround ? proxy->flags &= ~RE::CHARACTER_FLAGS::kNoGravityOnGround : proxy->flags |= RE::CHARACTER_FLAGS::kNoGravityOnGround;
			proxy->gravity = a_value;
		}
	}
}


void papyrusActor::SetSkinAlpha(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, float a_alpha)
{
	using ALPHA_MODE = RE::NiAVObject::ALPHA_MODE;

	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return;
	}

	auto root = a_actor->Get3D(0);
	if (root) {
		auto task = SKSE::GetTaskInterface();
		task->AddTask([a_actor, a_alpha, root]() {
			root->UpdateAlpha(a_alpha, ALPHA_MODE::kSkin);
		});

		auto data = root->GetExtraData<RE::NiFloatExtraData>("PO3_ALPHA");
		if (data) {
			if (a_alpha == 1.0) {
				root->RemoveExtraData(data);
			}
		} else {
			auto newData = RE::NiFloatExtraData::Create("PO3_ALPHA", a_alpha);
			if (newData) {
				root->AddExtraData(newData);
			}
		}
	} else {
		a_vm->TraceStack("Actor has no 3D", a_stackID, Severity::kWarning);
	}
}


void papyrusActor::SetSkinColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSColorForm* a_color)
{
	if (!a_actor) {
		a_vm->TraceStack("Actor is None ", a_stackID, Severity::kWarning);
		return;
	} else if (!a_color) {
		a_vm->TraceStack("Colorform is None", a_stackID, Severity::kWarning);
		return;
	}

	auto actorbase = a_actor->GetActorBase();
	if (actorbase) {
		auto root = a_actor->Get3D(0);
		if (!root) {
			a_vm->TraceStack("Actor has no 3D", a_stackID, Severity::kWarning);
			return;
		}

		auto task = SKSE::GetTaskInterface();
		task->AddTask([a_actor, a_color, root]() {
			TintFace(a_actor, a_color->color);
			root->UpdateBodyTint(a_color->color);
		});

		AddOrUpdateColorData(root, "PO3_SKINTINT", a_color->color);
	}
}


bool papyrusActor::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("papyrusActor - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("AddBasePerk", "PO3_SKSEFunctions", AddBasePerk);

	a_vm->RegisterFunction("AddBaseSpell", "PO3_SKSEFunctions", AddBaseSpell);

	a_vm->RegisterFunction("AddAllEquippedItemsToArray", "PO3_SKSEFunctions", AddAllEquippedItemsToArray);

	a_vm->RegisterFunction("BlendColorWithSkinTone", "PO3_SKSEFunctions", BlendColorWithSkinTone);

	a_vm->RegisterFunction("DecapitateActor", "PO3_SKSEFunctions", DecapitateActor);

	a_vm->RegisterFunction("EquipArmorIfSkinVisible", "PO3_SKSEFunctions", EquipArmorIfSkinVisible);

	a_vm->RegisterFunction("FreezeActor", "PO3_SKSEFunctions", FreezeActor);

	a_vm->RegisterFunction("GetActorAlpha", "PO3_SKSEFunctions", GetActorAlpha);

	a_vm->RegisterFunction("GetActorRefraction", "PO3_SKSEFunctions", GetActorRefraction);

	a_vm->RegisterFunction("GetActorState", "PO3_SKSEFunctions", GetActorState, true);

	a_vm->RegisterFunction("GetAllActiveEffectsOnActor", "PO3_SKSEFunctions", GetAllActiveEffectsOnActor);

	a_vm->RegisterFunction("GetHairColor", "PO3_SKSEFunctions", GetHairColor);

	a_vm->RegisterFunction("GetHeadPartTextureSet", "PO3_SKSEFunctions", GetHeadPartTextureSet);

	a_vm->RegisterFunction("GetRunningPackage", "PO3_SKSEFunctions", GetRunningPackage);

	a_vm->RegisterFunction("GetSkinColor", "PO3_SKSEFunctions", GetSkinColor);

	a_vm->RegisterFunction("GetTimeDead", "PO3_SKSEFunctions", GetTimeDead);

	a_vm->RegisterFunction("GetTimeOfDeath", "PO3_SKSEFunctions", GetTimeOfDeath);

	a_vm->RegisterFunction("HasActiveSpell", "PO3_SKSEFunctions", HasActiveSpell);

	a_vm->RegisterFunction("HasMagicEffectWithArchetype", "PO3_SKSEFunctions", HasMagicEffectWithArchetype);

	a_vm->RegisterFunction("InstantKill", "PO3_SKSEFunctions", InstantKill);

	a_vm->RegisterFunction("IsActorInWater", "PO3_SKSEFunctions", IsActorInWater, true);

	a_vm->RegisterFunction("IsActorSoulTrapped", "PO3_SKSEFunctions", IsActorSoulTrapped);

	a_vm->RegisterFunction("IsActorUnderwater", "PO3_SKSEFunctions", IsActorUnderwater, true);

	a_vm->RegisterFunction("MixColorWithSkinTone", "PO3_SKSEFunctions", MixColorWithSkinTone);

	a_vm->RegisterFunction("RemoveBasePerk", "PO3_SKSEFunctions", RemoveBasePerk);

	a_vm->RegisterFunction("RemoveBaseSpell", "PO3_SKSEFunctions", RemoveBaseSpell);

	a_vm->RegisterFunction("RemoveEffectsNotOfType", "PO3_SKSEFunctions", RemoveEffectsNotOfType);

	a_vm->RegisterFunction("ReplaceArmorTextureSet", "PO3_SKSEFunctions", ReplaceArmorTextureSet);

	a_vm->RegisterFunction("ReplaceFaceTextureSet", "PO3_SKSEFunctions", ReplaceFaceTextureSet);

	a_vm->RegisterFunction("ReplaceSkinTextureSet", "PO3_SKSEFunctions", ReplaceSkinTextureSet);

	a_vm->RegisterFunction("ResetActor3D", "PO3_SKSEFunctions", ResetActor3D);

	a_vm->RegisterFunction("SetActorRefraction", "PO3_SKSEFunctions", SetActorRefraction);

	a_vm->RegisterFunction("SetHairColor", "PO3_SKSEFunctions", SetHairColor);

	a_vm->RegisterFunction("SetHeadPartAlpha", "PO3_SKSEFunctions", SetHeadPartAlpha);

	a_vm->RegisterFunction("SetHeadPartTextureSet", "PO3_SKSEFunctions", SetHeadPartTextureSet);

	a_vm->RegisterFunction("SetLinearVelocity", "PO3_SKSEFunctions", SetLinearVelocity);

	a_vm->RegisterFunction("SetLocalGravity", "PO3_SKSEFunctions", SetLocalGravity);

	a_vm->RegisterFunction("SetSkinAlpha", "PO3_SKSEFunctions", SetSkinAlpha);

	a_vm->RegisterFunction("SetSkinColor", "PO3_SKSEFunctions", SetSkinColor);

	return true;
}
