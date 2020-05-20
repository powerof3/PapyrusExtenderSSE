//credits to underthesky for original implementation, davidjcobb, aers, nukem, ryan, meh321, expired for tips and help
//modifications and extra functions by powerofthree
//modified for CommonLib

#include "po3_functions.h"
#include "po3_graphicfunctions.h"
#include "po3_resetfunctions.h"
#include "po3_serializedclasses.h"

#include <sstream>

extern const SKSE::TaskInterface* g_task;
extern const RE::BGSKeyword* g_npcKeyword;
extern const RE::BGSArtObject* g_soulTrapArt;


namespace RE
{
	//--------------------------------------------------------------------------------------------
	// ACTOR
	//--------------------------------------------------------------------------------------------

	BGSColorForm* PO3_SKSEFunctions::GetHairColor(StaticFunctionTag*, Actor* a_actor)
	{
		if (a_actor) {
			auto actorbase = a_actor->GetActorBase();
			if (actorbase) {
				auto root = a_actor->Get3D(0);
				if (root) {
					auto data = static_cast<NiIntegerExtraData*>(root->GetExtraData(BSFixedString("PO3_HAIRTINT")));
					if (data) {
						auto factory = IFormFactory::GetFormFactoryByType(FormType::ColorForm);
						auto color = static_cast<BGSColorForm*>(factory->Create());
						color->flags &= ~BGSColorForm::Flag::kPlayable;
						color->color = Color(data->value);
						return color;
					}
				}
				auto headData = actorbase->headRelatedData;
				if (headData) {
					return headData->hairColor;
				}
			}
		}
		return nullptr;
	}

	void PO3_SKSEFunctions::SetHairColor(StaticFunctionTag*, Actor* a_actor, BGSColorForm* a_color)
	{
		if (!a_actor || !a_color) {
			return;
		}

		auto root = a_actor->Get3D(0);
		if (root) {
			root->UpdateHairColor(a_color->color);
			AddOrUpdateColorData(root, BSFixedString("PO3_HAIRTINT"), a_color->color);
		}
	}


	BGSColorForm* PO3_SKSEFunctions::GetSkinColor(StaticFunctionTag*, Actor* a_actor)
	{
		if (a_actor) {
			auto actorBase = a_actor->GetActorBase();
			if (actorBase) {

				auto factory = IFormFactory::GetFormFactoryByType(FormType::ColorForm);
				auto color = static_cast<BGSColorForm*>(factory->Create());

				color->flags &= ~BGSColorForm::Flag::kPlayable;
				color->color = actorBase->bodyTintColor;

				auto root = a_actor->Get3D(0);
				if (root) {
					auto data = static_cast<NiIntegerExtraData*>(root->GetExtraData(BSFixedString("PO3_SKINTINT")));
					if (data) {
						color->color = Color(data->value);
					}
				}

				return color;
			}
		}
		return nullptr;
	}


	void PO3_SKSEFunctions::SetSkinColor(StaticFunctionTag*, Actor* a_actor, BGSColorForm* a_color)
	{
		if (!a_actor || !a_color) {
			return;
		}

		auto actorbase = a_actor->GetActorBase();
		if (actorbase) {
			auto root = a_actor->Get3D(0);
			if (root) {
				auto color = a_color->color;
				g_task->AddTask([a_actor, color, root]()
				{
					TintFace(a_actor, color);
					root->UpdateBodyTint(color);
				});
				AddOrUpdateColorData(root, BSFixedString("PO3_SKINTINT"), color);
			}
		}
	}


	void PO3_SKSEFunctions::BlendColorWithSkinTone(StaticFunctionTag*, Actor* a_actor, BGSColorForm* a_color, SInt32 a_blendMode, bool a_autoCalc, float a_opacity)
	{
		if (!a_actor || !a_color) {
			return;
		}

		auto actorbase = a_actor->GetActorBase();
		if (actorbase) {

			float opacity = a_autoCalc ? std::clamp(a_opacity * NiColor::CalcLuminance(actorbase->bodyTintColor), 0.0f, 1.0f) : a_opacity;
			auto blendMode = static_cast<NiColor::BLEND_MODE>(a_blendMode);
			auto newColor = NiColor::Blend(actorbase->bodyTintColor, a_color->color, blendMode, opacity);

			auto root = a_actor->Get3D(0);
			if (root) {
				g_task->AddTask([a_actor, newColor, root]()
				{
					TintFace(a_actor, newColor);
					root->UpdateBodyTint(newColor);
				});
				AddOrUpdateColorData(root, BSFixedString("PO3_SKINTINT"), newColor);
			}
		}
	}


	void PO3_SKSEFunctions::MixColorWithSkinTone(StaticFunctionTag*, Actor* a_actor, BGSColorForm* a_color, bool a_manual, float a_percent)
	{
		if (!a_actor || !a_color) {
			return;
		}

		auto actorbase = a_actor->GetActorBase();
		if (actorbase) {

			float skinLuminance = a_manual ? a_percent : NiColor::CalcLuminance(actorbase->bodyTintColor);
			auto newColor = NiColor::Mix(actorbase->bodyTintColor, a_color->color, skinLuminance);

			auto root = a_actor->Get3D(0);
			if (root) {
				g_task->AddTask([a_actor, newColor, root]()
				{
					TintFace(a_actor, newColor);
					root->UpdateBodyTint(newColor);
				});
				AddOrUpdateColorData(root, BSFixedString("PO3_SKINTINT"), newColor);
			}
		}
	}


	void PO3_SKSEFunctions::SetSkinAlpha(StaticFunctionTag*, Actor* a_actor, float a_alpha)
	{
		if (a_actor) {
			auto root = a_actor->Get3D(0);
			if (root) {
				g_task->AddTask([a_actor, a_alpha, root]()
				{
					root->UpdateAlpha(a_alpha, NiAVObject::ALPHA_MODE::kSkin);
				});
				auto data = static_cast<NiFloatExtraData*>(root->GetExtraData(BSFixedString("PO3_ALPHA")));
				if (data) {
					if (a_alpha == 1.0) {
						root->RemoveExtraData(data);
					}
				}
				else {
					auto newData = NiFloatExtraData::Create(BSFixedString("PO3_ALPHA"), a_alpha);
					if (newData) {
						root->AddExtraData(newData);
					}
				}
			}
		}
	}


	void PO3_SKSEFunctions::EquipArmorIfSkinVisible(StaticFunctionTag*, Actor* a_actor, TESObjectARMO* a_check, TESObjectARMO* a_equip)
	{
		if (!a_actor || !a_check || !a_equip || !a_actor->Is3DLoaded()) {
			return;
		}

		g_task->AddTask([a_actor, a_check, a_equip]()
		{
			for (auto& arma : a_check->armorAddons) {
				if (arma) {
					NiAVObject* armorObject = a_actor->VisitArmorAddon(a_check, arma);
					if (armorObject) {
						if (armorObject->HasShaderType(BSShaderMaterial::Feature::kFaceGenRGBTint)) {
							a_actor->AddWornItem(a_equip, 1, false, 0, 0);
							break;
						}
					}
				}
			}
		});
	}


	void PO3_SKSEFunctions::ReplaceArmorTextureSet(StaticFunctionTag*, Actor* a_actor, TESObjectARMO* a_armor, BGSTextureSet* a_srcTXST, BGSTextureSet* a_tgtTXST, SInt32 a_type)
	{
		if (!a_actor || !a_armor || !a_srcTXST || !a_tgtTXST || !a_actor->Is3DLoaded()) {
			return;
		}

		std::string targetPath = a_srcTXST->GetTexturePath(BSShaderTextureSet::Texture::kDiffuse);
		Util::SanitizeTexturePath(targetPath);

		g_task->AddTask([a_actor, a_armor, a_srcTXST, a_tgtTXST, a_type, targetPath]()
		{
			for (auto& armorAddon : a_armor->armorAddons) {
				if (armorAddon) {
					NiAVObject* armorObject = a_actor->VisitArmorAddon(a_armor, armorAddon);
					if (armorObject) {

						bool replaced = false;
						SetTextureSet(armorObject, *a_tgtTXST, a_type, targetPath, replaced);

						auto root = a_actor->Get3D(0);
						if (replaced && root) {
							auto armorID = std::to_string(a_armor->formID);
							std::string name = "PO3_TXST - " + armorID;

							auto data = static_cast<NiStringsExtraData*>(root->GetExtraData(BSFixedString(name.c_str())));
							if (!data) {
								std::vector<BSFixedString> vec;
								vec.reserve(9);
								for (auto i = BSShaderTextureSet::Texture::kDiffuse; i < BSShaderTextureSet::Texture::kTotal; i++) {
									vec.emplace_back(a_srcTXST->GetTexturePath(i));
								}
								vec.emplace_back(armorID.c_str());
								auto newData = NiStringsExtraData::Create(BSFixedString(name.c_str()), vec.data(), static_cast<UInt32>(vec.size()));
								if (newData) {
									root->AddExtraData(newData);
								}
							}
						}
					}
				}
			}
		});
	}


	void PO3_SKSEFunctions::ReplaceSkinTextureSet(StaticFunctionTag*, Actor* a_actor, BGSTextureSet* a_maleTXST, BGSTextureSet* a_femaleTXST, UInt32 a_slot, SInt32 a_txstType)
	{
		if (!a_actor || !a_actor->Is3DLoaded()) {
			return;
		}

		bool isFemale = false;
		auto actorBase = a_actor->GetActorBase();
		if (actorBase) {
			isFemale = actorBase->GetSex() == SEX::kFemale ? true : false;
		}

		if (isFemale) {
			if (!a_femaleTXST) {
				return;
			}
			SetArmorSkinTXST(a_actor, a_femaleTXST, static_cast<BGSBipedObjectForm::BipedObjectSlot>(a_slot), a_txstType);
		}
		else {
			if (!a_maleTXST) {
				return;
			}
			SetArmorSkinTXST(a_actor, a_maleTXST, static_cast<BGSBipedObjectForm::BipedObjectSlot>(a_slot), a_txstType);
		}
	}


	void PO3_SKSEFunctions::ReplaceFaceTextureSet(StaticFunctionTag*, Actor* a_actor, BGSTextureSet* a_maleTXST, BGSTextureSet* a_femaleTXST, SInt32 a_type)
	{
		if (!a_actor || !a_actor->Is3DLoaded()) {
			return;
		}

		bool isFemale = false;
		auto actorBase = a_actor->GetActorBase();
		if (actorBase) {
			isFemale = actorBase->GetSex() == SEX::kFemale ? true : false;
		}

		auto txst = isFemale ? a_femaleTXST : a_maleTXST;
		if (!txst) {
			return;
		}

		g_task->AddTask([a_actor, txst, a_type]()
		{
			auto faceObject = a_actor->GetHeadPartObject(BGSHeadPart::HeadPartType::kFace);
			if (faceObject) {
				std::vector<BSFixedString> vec;
				vec.reserve(BSTextureSet::Texture::kTotal);
				SetSkinTextureSet(faceObject, *txst, vec, a_type);

				auto root = a_actor->Get3D(0);
				if (!vec.empty() && root) {
					auto data = static_cast<NiStringsExtraData*>(root->GetExtraData(BSFixedString("PO3_FACETXST")));
					if (!data) {
						auto newData = NiStringsExtraData::Create(BSFixedString("PO3_FACETXST"), vec.data(), static_cast<UInt32>(vec.size()));
						if (newData) {
							root->AddExtraData(newData);
						}
					}
				}
			}
		});
	}


	BGSTextureSet* PO3_SKSEFunctions::GetHeadPartTextureSet(StaticFunctionTag*, Actor* a_actor, UInt32 a_type)
	{
		if (a_actor && a_actor->Is3DLoaded()) {
			auto actorBase = a_actor->GetActorBase();
			if (actorBase) {
				auto headpart = actorBase->GetCurrentHeadPartByType(static_cast<BGSHeadPart::HeadPartType>(a_type));
				if (headpart) {
					return headpart->textureSet;
				}
			}
		}
		return nullptr;
	}


	void PO3_SKSEFunctions::SetHeadPartTextureSet(StaticFunctionTag*, Actor* a_actor, BGSTextureSet* a_txst, UInt32 a_type)
	{
		if (!a_actor || !a_txst || !a_actor->Is3DLoaded()) {
			return;
		}
		auto actorBase = a_actor->GetActorBase();
		if (actorBase) {
			auto headpart = actorBase->GetCurrentHeadPartByType(static_cast<BGSHeadPart::HeadPartType>(a_type));
			if (headpart) {
				headpart->textureSet = a_txst;
			}
		}
	}


	void PO3_SKSEFunctions::SetHeadPartAlpha(StaticFunctionTag*, Actor* a_actor, UInt32 a_type, float a_alpha)
	{
		if (!a_actor || !a_actor->Is3DLoaded()) {
			return;
		}

		g_task->AddTask([a_actor, a_type, a_alpha]()
		{
			auto object = a_actor->GetHeadPartObject(static_cast<BGSHeadPart::HeadPartType>(a_type));
			if (object) {
				object->UpdateAlpha(a_alpha, NiAVObject::ALPHA_MODE::kAll);
			}
		});

		auto root = a_actor->Get3D(0);
		if (root) {
			auto data = static_cast<NiIntegersExtraData*>(root->GetExtraData(BSFixedString("PO3_HEADPARTALPHA")));
			if (!data) {
				if (a_alpha == 0.0) {
					std::vector<SInt32> vec;
					vec.reserve(1);
					vec.emplace_back(a_type);
					auto newData = NiIntegersExtraData::Create(BSFixedString("PO3_HEADPARTALPHA"), vec.data(), static_cast<UInt32>(vec.size()));
					if (newData) {
						root->AddExtraData(newData);
					}
				}
			}
			else {
				a_alpha == 0.0 ? data->InsertElement(a_type) : data->RemoveElement(a_type);
			}
		}
	}


	void PO3_SKSEFunctions::ToggleChildNode(StaticFunctionTag*, Actor* a_actor, BSFixedString a_nodeName, bool a_disable)
	{
		if (a_actor) {
			auto root = a_actor->Get3D(0);
			if (root) {
				auto child = root->GetObjectByName(a_nodeName);
				if (child) {
					g_task->AddTask([child, a_disable]()
					{
						child->UpdateVisibility(a_disable);
					});
					auto data = static_cast<NiStringsExtraData*>(root->GetExtraData(BSFixedString("PO3_TOGGLE")));
					if (!data) {
						if (a_disable) {
							std::vector<BSFixedString> vec;
							vec.reserve(1);
							vec.emplace_back(a_nodeName);
							auto newData = NiStringsExtraData::Create(BSFixedString("PO3_TOGGLE"), vec.data(), static_cast<UInt32>(vec.size()));
							if (newData) {
								root->AddExtraData(newData);
							}
						}
					}
					else {
						a_disable == true ? data->InsertElement(a_nodeName.c_str()) : data->RemoveElement(a_nodeName.c_str());
					}
				}
			}
		}
	}


	void PO3_SKSEFunctions::RemoveChildNode(StaticFunctionTag*, Actor* a_actor, BSFixedString a_nodeName)
	{
		if (a_actor) {
			auto root = a_actor->Get3D(0)->AsNode();
			if (root) {
				auto child = root->GetObjectByName(a_nodeName);
				if (child) {
					g_task->AddTask([root, child]()
					{
						root->DetachChild(child);
					});
				}
			}
		}
	}


	bool PO3_SKSEFunctions::IsActorSoulTrapped(StaticFunctionTag*, Actor* a_actor)
	{
		if (a_actor) {
			bool isNPC = false;
			if (a_actor->HasKeywordHelper(g_npcKeyword)) {
				isNPC = true;
			}

			bool hasSoulTrap = false;
			Actor* caster = nullptr;

			auto activeEffects = a_actor->GetActiveEffectList();
			if (activeEffects) {
				for (auto& ae : *activeEffects) {
					if (!ae || !ae->effect || !ae->effect->baseEffect) {
						continue;
					}
					auto a_mgef = ae->effect->baseEffect;
					if (a_mgef) {
						if (a_mgef->data.hitEffectArt != g_soulTrapArt && a_mgef->data.archetype != EffectArchetypes::ArchetypeID::kSoulTrap) //only soultrap
						{
							continue;
						}
						auto casterPtr = ae->GetCasterActor();
						if (!casterPtr.get()) {
							return false;
						}
						caster = casterPtr.get();
						hasSoulTrap = true;
						break;
					}
				}
			}

			if (hasSoulTrap) {
				auto changes = caster->GetInventoryChanges();
				if (changes && changes->entryList) {
					for (auto& entry : *changes->entryList) {
						if (!entry) {
							continue;
						}
						auto object = entry->object;
						if (!object || !object->IsSoulGem()) {
							continue;
						}
						auto soulGem = static_cast<TESSoulGem*>(object);
						if (soulGem) {
							auto soulGemLevel = entry->GetSoulLevel();
							if (soulGemLevel == SOUL_LEVEL::kNone) {
								if (isNPC) {
									if (soulGem->formFlags & TESSoulGem::RecordFlags::kCanHoldNPCSoul) {
										return true;
									}
								}
								else {
									auto actorSoulLevel = a_actor->GetSoulLevel();
									if (soulGem->soulCapacity >= actorSoulLevel) {
										return true;
									}
								}
							}
						}
					}
				}
			}
		}
		return false;
	}


	std::vector<TESForm*> PO3_SKSEFunctions::AddAllEquippedItemsToArray(StaticFunctionTag*, Actor* a_actor)
	{
		std::vector<TESForm*> vec;

		if (a_actor) {
			auto changes = a_actor->GetInventoryChanges();
			if (changes && changes->entryList) {
				for (auto& entry : *changes->entryList) {
					if (entry && entry->extraLists) {
						auto object = entry->object;
						if (!object) {
							continue;
						}
						for (auto& list : *entry->extraLists) {
							if (list && (list->HasType(ExtraDataType::kWorn) || list->HasType(ExtraDataType::kWornLeft))) {
								vec.push_back(object);
							}
						}
					}
				}
			}
		}

		return vec;
	}


	bool PO3_SKSEFunctions::ResetActor3D(StaticFunctionTag*, Actor* a_actor, BSFixedString a_folderName)
	{
		if (a_actor) {

			auto root = a_actor->Get3D(0);
			if (!root) {
				return false;
			}

			auto t = GetResetData<NiStringsExtraData*, NiIntegersExtraData*, NiFloatExtraData*>(root);
			auto toggleData = std::get<0>(t);
			auto skinTintData = std::get<1>(t);
			auto hairTintData = std::get<2>(t);
			auto alphaData = std::get<3>(t);
			auto headpartAlphaData = std::get<4>(t);
			auto txstFaceData = std::get<5>(t);
			auto txstVec = std::get<6>(t);
			auto txstSkinVec = std::get<7>(t);
			auto shaderVec = std::get<8>(t);

			if (!toggleData && !alphaData && !headpartAlphaData && !skinTintData && !hairTintData && !txstFaceData && txstVec.empty() && txstSkinVec.empty() && shaderVec.empty()) {
				return false;
			}

			if (!a_actor->IsPlayerRef()) {
				a_actor->StopAllShaders();
			}
			if (toggleData) {
				ResetToggleData(root, toggleData);
			}
			if (alphaData) {
				ResetAlphaData(a_actor, root, alphaData);
			}
			if (headpartAlphaData) {
				ResetHeadPartAlphaData(a_actor, root, headpartAlphaData);
			}
			if (skinTintData) {
				ResetSkinTintData(a_actor, root, skinTintData);
			}
			if (hairTintData) {
				ResetHairTintData(a_actor, root, hairTintData);
			}
			if (txstFaceData) {
				ResetFaceTXSTData(a_actor, root, txstFaceData);
			}
			if (!txstSkinVec.empty()) {
				ResetSkinTXSTData(a_actor, root, txstSkinVec);
			}
			if (!txstVec.empty()) {
				ResetTXSTData(a_actor, root, a_folderName, txstVec);
			}
			if (!shaderVec.empty()) {
				ResetShaderData(root, shaderVec);
			}

			return true;
		}

		return false;
	}


	void PO3_SKSEFunctions::RemoveEffectsNotOfType(StaticFunctionTag*, Actor* a_actor, SInt32 a_effectType)
	{
		if (a_actor) {

			auto root = a_actor->Get3D(0);
			if (!root) {
				return;
			}

			auto t = GetResetData<NiStringsExtraData*, NiIntegersExtraData*, NiFloatExtraData*>(root);

			auto toggleData = std::get<0>(t);
			auto skinTintData = std::get<1>(t);
			auto hairTintData = std::get<2>(t);
			auto alphaData = std::get<3>(t);
			auto headpartAlphaData = std::get<4>(t);
			auto txstFaceData = std::get<5>(t);
			auto txstVec = std::get<6>(t);
			auto txstSkinVec = std::get<7>(t);
			auto shaderVec = std::get<8>(t);

			auto effectType = static_cast<EFFECT_TYPES>(a_effectType);
			switch (effectType) {
				case EFFECT_TYPES::kCharred:
				{
					if (skinTintData) {
						ResetSkinTintData(a_actor, root, skinTintData);
					}
					if (hairTintData) {
						ResetHairTintData(a_actor, root, hairTintData);
					}
					if (txstFaceData) {
						ResetFaceTXSTData(a_actor, root, txstFaceData);
					}
					if (!txstSkinVec.empty()) {
						ResetSkinTXSTData(a_actor, root, txstSkinVec);
					}
				}
				break;
				case EFFECT_TYPES::kDrained:
				{
					if (toggleData) {
						ResetToggleData(root, toggleData);
					}
					if (skinTintData) {
						ResetSkinTintData(a_actor, root, skinTintData);
					}
					if (hairTintData) {
						ResetHairTintData(a_actor, root, hairTintData);
					}
					if (txstFaceData) {
						ResetFaceTXSTData(a_actor, root, txstFaceData);
					}
					if (!txstSkinVec.empty()) {
						ResetSkinTXSTData(a_actor, root, txstSkinVec);
					}
				}
				break;
				case EFFECT_TYPES::kPoisoned:
				{
					if (!a_actor->IsPlayerRef()) {
						StopAllSkinAlphaShaders(a_actor);
					}
					if (toggleData) {
						ResetToggleData(root, toggleData);
					}
					if (alphaData) {
						ResetAlphaData(a_actor, root, alphaData);
					}
					if (headpartAlphaData) {
						ResetHeadPartAlphaData(a_actor, root, headpartAlphaData);
					}
					if (txstFaceData) {
						ResetFaceTXSTData(a_actor, root, txstFaceData);
					}
					if (!txstSkinVec.empty()) {
						ResetSkinTXSTData(a_actor, root, txstSkinVec);
					}
				}
				break;
				case EFFECT_TYPES::kAged:
				{
					if (!a_actor->IsPlayerRef()) {
						StopAllSkinAlphaShaders(a_actor);
					}
					if (toggleData) {
						ResetToggleData(root, toggleData);
					}
					if (alphaData) {
						ResetAlphaData(a_actor, root, alphaData);
					}
					if (headpartAlphaData) {
						ResetHeadPartAlphaData(a_actor, root, headpartAlphaData);
					}
				}
				break;
				case EFFECT_TYPES::kCharredCreature:
				{
					if (!shaderVec.empty()) {
						ResetShaderData(root, shaderVec);
					}
				}
				break;
				default:
					break;
			}
		}
	}


	void PO3_SKSEFunctions::DecapitateActor(StaticFunctionTag*, Actor* a_actor)
	{
		if (a_actor) {
			a_actor->Decapitate();
		}
	}


	float PO3_SKSEFunctions::GetTimeDead(StaticFunctionTag*, Actor* a_actor)
	{
		if (a_actor && a_actor->currentProcess) {
			float timeOfDeath = a_actor->currentProcess->deathTime;
			if (timeOfDeath > 0.0) {
				auto calendar = Calendar::GetSingleton();
				if (calendar) {
					auto g_gameDaysPassed = calendar->gameDaysPassed;
					if (g_gameDaysPassed) {
						return floorf(g_gameDaysPassed->value * static_cast <float>(24.0)) - timeOfDeath;
					}
				}
			}
		}
		return 0.0;
	}


	float PO3_SKSEFunctions::GetTimeOfDeath(StaticFunctionTag*, Actor* a_actor)
	{
		if (a_actor && a_actor->currentProcess) {
			float timeOfDeath = a_actor->currentProcess->deathTime;
			if (timeOfDeath > 0.0) {
				return timeOfDeath / static_cast <float>(24.0);
			}
		}
		return 0.0;
	}


	TESPackage* PO3_SKSEFunctions::GetRunningPackage(StaticFunctionTag*, Actor* a_actor)
	{
		TESPackage* package = nullptr;

		if (a_actor) {
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
		}

		return package;
	}


	bool PO3_SKSEFunctions::IsActorInWater(StaticFunctionTag*, Actor* a_actor)
	{
		return a_actor ? (a_actor->boolBits & Actor::BOOL_BITS::kInWater) != Actor::BOOL_BITS::kNone : false;
	}


	float PO3_SKSEFunctions::GetActorAlpha(StaticFunctionTag*, Actor* a_actor)
	{
		if (a_actor) {
			auto currentProcess = a_actor->currentProcess;
			if (currentProcess) {
				auto middleProcess = currentProcess->middleHigh;
				if (middleProcess) {
					return middleProcess->alphaMult;
				}
			}
		}
		return 1.0;
	}


	float PO3_SKSEFunctions::GetActorRefraction(StaticFunctionTag*, Actor* a_actor)
	{
		if (a_actor) {
			auto currentProcess = a_actor->currentProcess;
			if (currentProcess) {
				auto middleProcess = currentProcess->middleHigh;
				if (middleProcess) {
					return middleProcess->scriptRefractPower;
				}
			}
		}
		return 1.0;
	}


	void PO3_SKSEFunctions::SetActorRefraction(StaticFunctionTag*, Actor* a_actor, float a_refraction)
	{
		if (a_actor) {
			auto currentProcess = a_actor->currentProcess;
			if (currentProcess) {
				a_refraction = std::clamp(a_refraction, 0.0f, 1.0f);
				currentProcess->SetActorRefraction(a_refraction);

				float invisibility = a_actor->GetActorValue(ActorValue::kInvisibility); //invisibility
				if (invisibility < 0.0 || invisibility <= 1.0 && invisibility <= 0.0 || !a_actor->IsPlayerRef()) {
					if (a_refraction <= 0.0) {
						a_actor->SetRefraction(0, a_refraction);
						a_actor->UpdateAlpha();
					}
					else {
						a_actor->SetRefraction(1, a_refraction);
					}
				}
				else {
					a_actor->SetAlpha(1.0);

					a_refraction = 1.0f - a_refraction / 100.0f;
					a_refraction = 1.0f + (0.01f - 1.0f) * ((a_refraction - 0.0f) / (1.0f - 0.0f));

					a_actor->SetRefraction(1, a_refraction);
				}
			}
		}
	}


	SInt32 PO3_SKSEFunctions::GetActorState(StaticFunctionTag*, Actor* a_actor)
	{
		if (a_actor) {
			return static_cast<SInt32>(a_actor->GetLifeState());
		}
		return -1;
	}


	bool PO3_SKSEFunctions::InstantKill(StaticFunctionTag*, Actor* a_actor)
	{
		return a_actor ? a_actor->InstantKill() : false;
	}


	bool PO3_SKSEFunctions::AddBasePerk(StaticFunctionTag*, Actor* a_actor, BGSPerk* a_perk)
	{
		if (a_actor && a_perk) {
			if (a_actor->IsPlayerRef()) {
				a_actor->AddPerk(a_perk, 0);
				return true;
			}
			auto perks = Perks::GetSingleton();
			if (perks) {
				return perks->PapyrusApplyPerks(a_actor, a_perk, Base::kAdd);
			}
		}
		return false;
	}


	bool PO3_SKSEFunctions::RemoveBasePerk(StaticFunctionTag*, Actor* a_actor, BGSPerk* a_perk)
	{
		if (a_actor && a_perk) {
			if (a_actor->IsPlayerRef()) {
				a_actor->RemovePerk(a_perk);
				return true;
			}
			auto perks = Perks::GetSingleton();
			if (perks) {
				return perks->PapyrusApplyPerks(a_actor, a_perk, Base::kRemove);
			}
		}
		return false;
	}


	bool PO3_SKSEFunctions::AddBaseSpell(StaticFunctionTag*, Actor* a_actor, SpellItem* a_spell)
	{
		if (!a_actor || !a_spell || a_actor->HasSpell(a_spell)) {
			return false;
		}
		auto actorbase = a_actor->GetActorBase();
		if (actorbase) {
			auto combatController = a_actor->combatController;
			if (combatController) {
				combatController->data10->unk1C4 = 1;
			}
			auto actorEffects = actorbase->GetOrCreateSpellList();
			if (actorEffects) {
				auto oldData = actorEffects->spells;
				actorEffects->spells = calloc<SpellItem*>(++actorEffects->numSpells);
				if (oldData) {
					for (UInt32 i = 0; i < actorEffects->numSpells - 1; i++) {
						actorEffects->spells[i] = oldData[i];
					}
					free(oldData);
					oldData = nullptr;
				}
				actorEffects->spells[actorEffects->numSpells - 1] = a_spell;
				actorbase->AddChange(TESNPC::ChangeFlags::kSpellList);
				return true;
			}
		}
		return false;
	}


	bool PO3_SKSEFunctions::RemoveBaseSpell(StaticFunctionTag*, Actor* a_actor, SpellItem* a_spell)
	{
		if (a_actor && a_spell) {
			auto actorbase = a_actor->GetActorBase();
			if (actorbase) {
				auto actorEffects = actorbase->actorEffects;
				if (actorEffects) {
					auto index = actorEffects->GetSpellIndex(a_spell);
					if (index == -1) {
						return false;
					}
					auto activeEffects = a_actor->GetActiveEffectList();
					if (activeEffects) {
						for (auto& ae : *activeEffects) {
							if (ae && ae->spell && ae->spell == a_spell) {
								ae->Dispell(true);
							}
						}
					}
					auto combatController = a_actor->combatController;
					if (combatController) {
						combatController->data10->unk1C4 = 1;
					}
					auto oldData = actorEffects->spells;
					actorEffects->spells = calloc<SpellItem*>(--actorEffects->numSpells);
					if (oldData) {
						for (UInt32 i = 0; i < actorEffects->numSpells + 1; i++) {
							if (index != i) {
								actorEffects->spells[i] = oldData[i];
							}
						}
						free(oldData);
						oldData = nullptr;
					}
					actorbase->AddChange(TESNPC::ChangeFlags::kSpellList);
					return true;
				}
			}
		}
		return false;
	}

	//--------------------------------------------------------------------------------------------
	// ACTORBASE
	//--------------------------------------------------------------------------------------------

	UInt32 PO3_SKSEFunctions::GetPerkCount(StaticFunctionTag*, TESNPC* a_actorbase)
	{
		return a_actorbase ? a_actorbase->perkCount : 0;
	}


	BGSPerk* PO3_SKSEFunctions::GetNthPerk(StaticFunctionTag*, TESNPC* a_actorbase, UInt32 a_index)
	{
		if (a_actorbase && a_index < a_actorbase->perkCount) {
			return a_actorbase->perks[a_index].perk;
		}

		return nullptr;
	}

	//--------------------------------------------------------------------------------------------
	// ARRAY
	//--------------------------------------------------------------------------------------------

	bool PO3_SKSEFunctions::AddActorToArray(StaticFunctionTag*, Actor* a_actor, reference_array<Actor*> a_actors)
	{
		for (auto& actor : a_actors) {
			if (!actor) {
				actor = a_actor;
				return true;
			}
		}
		return false;
	}


	bool PO3_SKSEFunctions::AddStringToArray(StaticFunctionTag*, BSFixedString a_string, reference_array<BSFixedString> a_strings)
	{
		for (auto& string : a_strings) {
			if (string.empty()) {
				string = a_string;
				return true;
			}
		}
		return false;
	}


	//count how many instances of string are found in an array
	UInt32 PO3_SKSEFunctions::ArrayStringCount(StaticFunctionTag*, BSFixedString a_string, std::vector<BSFixedString> a_strings)
	{
		return static_cast<UInt32>(std::count(a_strings.begin(), a_strings.end(), a_string));
	}


	//alphabetically sorts strings inside array
	std::vector<BSFixedString> PO3_SKSEFunctions::SortArrayString(StaticFunctionTag*, reference_array<BSFixedString> a_strings)
	{
		std::vector<BSFixedString> strings;

		for (auto& string : a_strings) {
			if (!string.empty()) {
				strings.push_back(string);
			}
		}
		std::sort(strings.begin(), strings.end());

		return strings;
	}


	std::vector<BSFixedString> PO3_SKSEFunctions::GetSortedActorNameArray(StaticFunctionTag*, BGSKeyword* a_keyword, bool a_invert)
	{
		std::unordered_map<std::string, size_t> nameMap;

		bool noKeyword = !a_keyword ? true : false;
		bool hasKeyword = false;

		auto processLists = ProcessLists::GetSingleton();
		if (processLists) {
			for (auto& handle : processLists->highActorHandles) {
				auto actor = handle.get();
				if (actor.get()) {
					if (!noKeyword) {
						hasKeyword = actor->HasKeyword(g_npcKeyword) ? true : false;
						if (a_invert) {
							hasKeyword = !hasKeyword;
						}
					}
					if (noKeyword || hasKeyword) {
						++nameMap[actor->GetName()];
					}
				}
			}
		}

		std::vector<BSFixedString> names;

		for (const auto& string : nameMap) {
			std::string fullName = string.second > 1 ? std::to_string(string.second) + " " + string.first + "(s)" : string.first;
			names.push_back(fullName.c_str());
		}

		std::sort(names.begin(), names.end());

		return names;
	}

	//--------------------------------------------------------------------------------------------
	// CELL
	//--------------------------------------------------------------------------------------------

	std::vector<TESObjectCELL*> PO3_SKSEFunctions::GetAttachedCells(StaticFunctionTag*)
	{
		std::vector<TESObjectCELL*> vec;

		auto TES = TES::GetSingleton();
		if (TES) {
			auto cell = TES->currentInteriorCell;
			if (cell) {
				vec.push_back(cell);
			}
			else {
				auto gridCellArray = TES->gridCellArray;
				if (gridCellArray) {
					auto gridLength = gridCellArray->length;
					if (gridLength > 0) {
						UInt32 x = 0;
						UInt32 y = 0;
						for (x = 0, y = 0; (x < gridLength && y < gridLength); x++, y++) {
							cell = gridCellArray->GetCell(x, y);
							if (cell) {
								vec.push_back(cell);
							}
						}
					}
				}
			}
			if (vec.empty()) {
				auto worldSpace = TES->worldSpace;
				if (worldSpace) {
					cell = worldSpace->GetOrCreateSkyCell();
					if (cell) {
						vec.push_back(cell);
					}
				}
			}
		}

		return vec;
	}

	//--------------------------------------------------------------------------------------------
	// EFFECTSHADER
	//--------------------------------------------------------------------------------------------

	float PO3_SKSEFunctions::GetEffectShaderFullParticleCount(StaticFunctionTag*, TESEffectShader* a_effectShader)
	{
		return a_effectShader ? a_effectShader->data.particleShaderFullParticleBirthRatio : static_cast <float>(0.0);
	}


	void PO3_SKSEFunctions::SetEffectShaderFullParticleCount(StaticFunctionTag*, TESEffectShader* a_effectShader, float a_particleCount)
	{
		if (a_effectShader) {
			a_effectShader->data.particleShaderFullParticleBirthRatio = a_particleCount;
		}
	}


	float PO3_SKSEFunctions::GetEffectShaderPersistentParticleCount(StaticFunctionTag*, TESEffectShader* a_effectShader)
	{
		return a_effectShader ? a_effectShader->data.particleShaderPersistantParticleCount : static_cast <float>(0.0);
	}


	void PO3_SKSEFunctions::SetEffectShaderPersistentParticleCount(StaticFunctionTag*, TESEffectShader* a_effectShader, float a_particleCount)
	{
		if (a_effectShader) {
			a_effectShader->data.particleShaderPersistantParticleCount = a_particleCount;
		}
	}


	bool PO3_SKSEFunctions::IsEffectShaderFlagSet(StaticFunctionTag*, TESEffectShader* a_effectShader, UInt32 a_flag)
	{
		return a_effectShader ? (static_cast<UInt32>(a_effectShader->data.flags)& a_flag) == a_flag : false;
	}


	void PO3_SKSEFunctions::SetEffectShaderFlag(StaticFunctionTag*, TESEffectShader* a_effectShader, UInt32 a_flag)
	{
		if (a_effectShader) {
			a_effectShader->data.flags |= static_cast<EffectShaderData::Flags>(a_flag);
		}
	}


	void PO3_SKSEFunctions::ClearEffectShaderFlag(StaticFunctionTag*, TESEffectShader* a_effectShader, UInt32 a_flag)
	{
		if (a_effectShader) {
			a_effectShader->data.flags &= ~static_cast<EffectShaderData::Flags>(a_flag);
		}
	}


	UInt32 PO3_SKSEFunctions::GetEffectShaderTotalCount(StaticFunctionTag*, TESEffectShader* a_effectShader, bool a_active)
	{
		UInt32 count = 0;

		if (a_effectShader) {
			auto processLists = ProcessLists::GetSingleton();
			if (processLists) {
				processLists->magicEffectsLock.Lock();
				for (auto& tempEffect : processLists->magicEffects) {
					if (tempEffect.get()) {
						auto shaderEffect = netimmerse_cast<ShaderReferenceEffect*>(tempEffect.get());
						if (shaderEffect) {
							auto effectData = shaderEffect->effectData;
							if (effectData && effectData == a_effectShader) {
								if (a_active && shaderEffect->finished == 1) {
									continue;
								}
								count++;
							}
						}
					}
				}
				processLists->magicEffectsLock.Unlock();
			}
		}

		return count;
	}

	//--------------------------------------------------------------------------------------------
	// FORM
	//--------------------------------------------------------------------------------------------

	void PO3_SKSEFunctions::ReplaceKeywordOnForm(StaticFunctionTag*, TESForm* a_form, BGSKeyword* a_remove, BGSKeyword* a_add)
	{
		if (!a_form || !a_remove || !a_add) {
			return;
		}

		auto keywordForm = a_form->As<BGSKeywordForm>();
		if (keywordForm) {
			UInt32 removeIndex = 0;
			BGSKeyword* keyword = nullptr;
			bool found = false;

			for (UInt32 i = 0; i < keywordForm->numKeywords; i++) {
				keyword = keywordForm->keywords[i];
				if (keyword) {
					if (keyword->formEditorID == a_add->formEditorID) {
						return;
					}
					if (keyword->formEditorID == a_remove->formEditorID) {
						removeIndex = i;
						found = true;
					}
				}
			}

			if (found) {
				keywordForm->keywords[removeIndex] = a_add;
			}
		}
	}


	void PO3_SKSEFunctions::AddKeywordToForm(StaticFunctionTag*, TESForm* a_form, BGSKeyword* a_add)
	{
		if (a_form && a_add) {
			auto keywords = Keywords::GetSingleton();
			if (keywords) {
				keywords->PapyrusApplyKeywords(a_form, a_add, Base::kAdd);
			}
		}
	}


	bool PO3_SKSEFunctions::RemoveKeywordOnForm(StaticFunctionTag*, TESForm* a_form, BGSKeyword* a_remove)
	{
		if (a_form && a_remove) {
			auto keywords = Keywords::GetSingleton();
			if (keywords) {
				return keywords->PapyrusApplyKeywords(a_form, a_remove, Base::kRemove);
			}
		}
		return false;
	}

	//--------------------------------------------------------------------------------------------
	// GAME
	//--------------------------------------------------------------------------------------------

	bool PO3_SKSEFunctions::IsPluginFound(StaticFunctionTag*, BSFixedString a_name)
	{
		if (!a_name.empty()) {
			auto dataHandler = TESDataHandler::GetSingleton();
			if (dataHandler) {
				const auto modInfo = dataHandler->LookupModByName(a_name.c_str());
				if (modInfo) {
					return modInfo->IsLoaded();
				}
			}
		}
		return false;
	}


	std::vector<TESForm*> PO3_SKSEFunctions::GetAllSpellsInMod(StaticFunctionTag*, BSFixedString a_modName, std::vector<BGSKeyword*> a_keywords, bool a_playable)
	{
		std::vector<TESForm*> vec;

		if (!a_modName.empty()) {
			auto dataHandler = TESDataHandler::GetSingleton();
			if (dataHandler) {
				const auto modInfo = dataHandler->LookupModByName(a_modName.c_str());

				if (modInfo && modInfo->IsLoaded()) {
					if (a_playable) {
						for (auto& book : dataHandler->GetFormArray<TESObjectBOOK>()) {
							if (!book || !modInfo->IsFormInMod(book->formID)) {
								continue;
							}
							auto spell = book->data.teaches.spell;
							if (!spell || !spell->HasKeywords(a_keywords)) {
								continue;
							}
							vec.push_back(spell);
						}
					}
					else {
						for (auto& spell : dataHandler->GetFormArray<SpellItem>()) {
							if (!spell || !modInfo->IsFormInMod(spell->formID) || !spell->HasKeywords(a_keywords)) {
								continue;
							}
							vec.push_back(spell);
						}
					}
				}
			}
		}

		return vec;
	}


	std::vector<TESForm*> PO3_SKSEFunctions::GetAllRacesInMod(StaticFunctionTag*, BSFixedString a_modName, std::vector<BGSKeyword*> a_keywords)
	{
		std::vector<TESForm*> vec;

		if (!a_modName.empty()) {
			auto dataHandler = TESDataHandler::GetSingleton();
			if (dataHandler) {
				const TESFile* modInfo = dataHandler->LookupModByName(a_modName.c_str());

				if (modInfo && modInfo->IsLoaded()) {
					for (auto& race : dataHandler->GetFormArray<TESRace>()) {
						if (!race || !modInfo->IsFormInMod(race->formID) || !race->HasKeywords(a_keywords)) {
							continue;
						}
						vec.push_back(race);
					}
				}
			}
		}

		return vec;
	}


	void PO3_SKSEFunctions::AddAllGameSpellsToList(StaticFunctionTag*, BGSListForm* a_list, std::vector<BGSKeyword*> a_keywords, bool a_playable)
	{
		if (a_list) {
			auto dataHandler = TESDataHandler::GetSingleton();
			if (dataHandler) {
				if (a_playable) {
					for (auto& book : dataHandler->GetFormArray<TESObjectBOOK>()) {
						if (book) {
							auto spell = book->data.teaches.spell;
							if (!spell || !spell->HasKeywords(a_keywords)) {
								continue;
							}
							a_list->AddForm(spell);
						}
					}
				}
				else {
					for (auto& spell : dataHandler->GetFormArray<SpellItem>()) {
						if (!spell || !spell->HasKeywords(a_keywords)) {
							continue;
						}
						a_list->AddForm(spell);
					}
				}
			}
		}
	}


	void PO3_SKSEFunctions::AddAllGameRacesToList(StaticFunctionTag*, BGSListForm* a_list, std::vector<BGSKeyword*> a_keywords)
	{
		if (a_list) {
			auto dataHandler = TESDataHandler::GetSingleton();
			if (dataHandler) {
				for (auto& race : dataHandler->GetFormArray<TESRace>()) {
					if (!race || !race->HasKeywords(a_keywords)) {
						continue;
					}
					a_list->AddForm(race);
				}
			}
		}
	}


	//gets actors by AI processing level - see https://geck.bethsoft.com/index.php?title=GetActorsByProcessingLevel
	std::vector<Actor*> PO3_SKSEFunctions::GetActorsByProcessingLevel(StaticFunctionTag*, UInt32 a_level)
	{
		std::vector<Actor*> vec;

		auto processLists = ProcessLists::GetSingleton();
		if (processLists) {

			BSTArray<ActorHandle>* arr = nullptr;

			switch (a_level) {
				case 0:
					arr = &processLists->highActorHandles;
					break;
				case 1:
					arr = &processLists->middleHighActorHandles;
					break;
				case 2:
					arr = &processLists->middleLowActorHandles;
					break;
				case 3:
					arr = &processLists->lowActorHandles;
					break;
				default:
					arr = nullptr;
					break;
			}

			if (arr) {
				for (auto& actorHandle : *arr) {
					auto actorPtr = actorHandle.get();
					if (actorPtr.get()) {
						vec.push_back(actorPtr.get());
					}
				}
			}
		}

		return vec;
	}


	//gets amount of actors in high process
	SInt32 PO3_SKSEFunctions::GetNumActorsInHigh(StaticFunctionTag*)
	{
		auto processLists = ProcessLists::GetSingleton();
		if (processLists) {
			return processLists->numberHighActors;
		}
		return -1;
	}

	//--------------------------------------------------------------------------------------------
	// LIGHT
	//--------------------------------------------------------------------------------------------

	float PO3_SKSEFunctions::GetLightRadius(StaticFunctionTag*, TESObjectLIGH* a_light)
	{
		return a_light ? static_cast<float>(a_light->data.radius) : 0.0f;
	}


	void PO3_SKSEFunctions::SetLightRadius(StaticFunctionTag*, TESObjectLIGH* a_light, float a_radius)
	{
		if (a_light) {
			a_light->data.radius = static_cast<UInt32>(a_radius);
		}
	}


	float PO3_SKSEFunctions::GetLightFade(StaticFunctionTag*, TESObjectLIGH* a_light)
	{
		return a_light ? a_light->fade : static_cast <float>(0.0);
	}


	void PO3_SKSEFunctions::SetLightFade(StaticFunctionTag*, TESObjectLIGH* a_light, float a_fade)
	{
		if (a_light) {
			a_light->fade = a_fade;
		}
	}


	BGSColorForm* PO3_SKSEFunctions::GetLightColor(StaticFunctionTag*, TESObjectLIGH* a_light)
	{
		if (a_light) {
			auto factory = IFormFactory::GetFormFactoryByType(FormType::ColorForm);
			auto color = static_cast<BGSColorForm*>(factory->Create());

			if (color) {
				color->flags &= ~BGSColorForm::Flag::kPlayable;

				color->color.red = a_light->data.color.red;
				color->color.green = a_light->data.color.green;
				color->color.blue = a_light->data.color.blue;
				color->color.alpha = a_light->data.color.alpha;

				return color;
			}
		}
		return nullptr;
	}


	void PO3_SKSEFunctions::SetLightColor(StaticFunctionTag*, TESObjectLIGH* a_light, BGSColorForm* a_color)
	{
		if (a_light && a_color) {
			a_light->data.color.red = a_color->color.red;
			a_light->data.color.green = a_color->color.green;
			a_light->data.color.blue = a_color->color.blue;
		}
	}


	UInt32 PO3_SKSEFunctions::GetLightType(StaticFunctionTag*, TESObjectLIGH* a_light)
	{
		if (a_light) {
			const auto flags = a_light->data.flags;

			if ((flags & TES_LIGHT_FLAGS::kHemiShadow) == TES_LIGHT_FLAGS::kHemiShadow) {
				return 1;
			}
			if ((flags & TES_LIGHT_FLAGS::kNone) == TES_LIGHT_FLAGS::kNone) {
				return 2;
			}
			if ((flags & TES_LIGHT_FLAGS::kOmniShadow) == TES_LIGHT_FLAGS::kOmniShadow) {
				return 3;
			}
			if ((flags & TES_LIGHT_FLAGS::kSpotlight) == TES_LIGHT_FLAGS::kSpotlight) {
				return 4;
			}
			if ((flags & TES_LIGHT_FLAGS::kSpotShadow) == TES_LIGHT_FLAGS::kSpotShadow) {
				return 5;
			}
		}

		return 0;
	}


	void PO3_SKSEFunctions::SetLightType(StaticFunctionTag*, TESObjectLIGH* a_light, UInt32 a_type)
	{
		if (a_light) {
			auto flags = a_light->data.flags;
			switch (a_type) {
				case 1:
					flags = flags & ~(TES_LIGHT_FLAGS::kType | TES_LIGHT_FLAGS::kHemiShadow);
					break;
				case 2:
					flags = flags & ~(TES_LIGHT_FLAGS::kType | TES_LIGHT_FLAGS::kNone);
					break;
				case 3:
					flags = flags & ~(TES_LIGHT_FLAGS::kType | TES_LIGHT_FLAGS::kOmniShadow);
					break;
				case 4:
					flags = flags & ~(TES_LIGHT_FLAGS::kType | TES_LIGHT_FLAGS::kSpotlight);
					break;
				case 5:
					flags = flags & ~(TES_LIGHT_FLAGS::kType | TES_LIGHT_FLAGS::kSpotShadow);
					break;
				default:
					return;
			}
			a_light->data.flags = flags;
		}
	}


	float PO3_SKSEFunctions::GetLightFOV(StaticFunctionTag*, TESObjectLIGH* a_light)
	{
		return a_light ? a_light->data.fov : static_cast <float>(0.0);
	}


	void PO3_SKSEFunctions::SetLightFOV(StaticFunctionTag*, TESObjectLIGH* a_light, float a_fov)
	{
		if (a_light) {
			a_light->data.fov = a_fov;
		}
	}


	float PO3_SKSEFunctions::GetLightShadowDepthBias(StaticFunctionTag*, TESObjectREFR* a_lightObject)
	{
		if (a_lightObject) {
			auto a_light = a_lightObject->As<TESObjectLIGH>();
			if (a_light) {
				auto xLightData = static_cast<ExtraLightData*>(a_lightObject->extraList.GetByType(ExtraDataType::kLightData));
				if (xLightData) {
					return xLightData->data.shadowDepthBias;
				}
			}
		}

		return 1.0;
	}


	void PO3_SKSEFunctions::SetLightShadowDepthBias(StaticFunctionTag*, TESObjectREFR* a_lightObject, float a_depthBias)
	{
		if (a_lightObject) {
			auto a_light = a_lightObject->As<TESObjectLIGH>();
			if (a_light) {
				auto xLightData = static_cast<ExtraLightData*>(a_lightObject->extraList.GetByType(ExtraDataType::kLightData));
				if (xLightData) {
					xLightData->data.shadowDepthBias = a_depthBias;
				}
				else {
					auto&& newLightData = ExtraLightData::ExtraLightData();
					newLightData.data.shadowDepthBias = a_depthBias;
					(&a_lightObject->extraList)->Add(&newLightData);
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------------
	// LOCATION
	//--------------------------------------------------------------------------------------------

	BGSLocation* PO3_SKSEFunctions::GetParentLocation(StaticFunctionTag*, BGSLocation* a_location)
	{
		return a_location ? a_location->parentLoc : nullptr;
	}


	void PO3_SKSEFunctions::SetParentLocation(StaticFunctionTag*, BGSLocation* a_location, BGSLocation* a_newLocation)
	{
		if (a_location) {
			a_location->parentLoc = a_newLocation;
		}
	}

	//--------------------------------------------------------------------------------------------
	// MATHS
	//--------------------------------------------------------------------------------------------

	// based on mersenne twister
	float PO3_SKSEFunctions::GenerateRandomFloat(StaticFunctionTag*, float a_min, float a_max)
	{
		auto random = Random::GetSingleton();
		if (random) {
			return random->operator()(a_min, a_max);
		}
		return a_max;
	}


	UInt32 PO3_SKSEFunctions::GenerateRandomInt(StaticFunctionTag*, UInt32 a_min, UInt32 a_max)
	{
		auto random = Random::GetSingleton();
		if (random) {
			return random->operator()(a_min, a_max);
		}
		return a_max;
	}

	//--------------------------------------------------------------------------------------------
	// MAGICEFFECT
	//--------------------------------------------------------------------------------------------

	std::vector<EffectSetting*> PO3_SKSEFunctions::GetAllActiveEffectsOnActor(StaticFunctionTag*, Actor* a_actor, bool a_inactive)
	{
		using MGEF = EffectSetting::EffectSettingData::Flag;
		using AE = ActiveEffect::Flag;

		std::vector<EffectSetting*> vec;

		if (a_actor) {
			auto activeEffects = a_actor->GetActiveEffectList();
			if (activeEffects) {
				for (auto& ae : *activeEffects) {
					if (ae) {
						auto mgef = ae->GetBaseObject();
						if (mgef) {
							if (!a_inactive && ((ae->flags & AE::kInactive) == AE::kInactive || (ae->flags & AE::kDispelled) == AE::kDispelled ||
								(mgef->data.flags & MGEF::kHideInUI) == MGEF::kHideInUI)) {
								continue;
							}
							vec.push_back(mgef);
						}
					}
				}
			}
		}

		return vec;
	}


	bool PO3_SKSEFunctions::HasMagicEffectWithArchetype(StaticFunctionTag*, Actor* a_actor, BSFixedString a_archetype)
	{
		if (a_actor && !a_archetype.empty()) {
			auto activeEffects = a_actor->GetActiveEffectList();
			if (activeEffects) {
				for (auto& ae : *activeEffects) {
					if (ae) {
						auto mgef = ae->GetBaseObject();
						if (mgef && mgef->GetArchetypeAsString() == a_archetype) {
							return true;
						}
					}
				}
			}
		}
		return false;
	}


	UInt32 PO3_SKSEFunctions::GetEffectArchetypeAsInt(StaticFunctionTag*, EffectSetting* a_mgef)
	{
		return a_mgef ? static_cast<UInt32>(a_mgef->data.archetype) : 0;
	}


	BSFixedString PO3_SKSEFunctions::GetEffectArchetypeAsString(StaticFunctionTag*, EffectSetting* a_mgef)
	{
		return a_mgef ? a_mgef->GetArchetypeAsString() : BSFixedString("");
	}


	BGSSoundDescriptorForm* PO3_SKSEFunctions::GetMagicEffectSound(StaticFunctionTag*, EffectSetting* a_mgef, UInt32 a_type)
	{
		if (a_mgef) {
			for (auto& effectSound : a_mgef->effectSounds) {
				if (effectSound.id == static_cast<MagicSystem::SoundID>(a_type)) {
					return effectSound.sound;
				}
			}
		}

		return nullptr;
	}


	void PO3_SKSEFunctions::SetMagicEffectSound(StaticFunctionTag*, EffectSetting* a_mgef, BGSSoundDescriptorForm* a_sound, UInt32 a_type)
	{
		if (a_mgef && a_sound) {
			for (auto& effectSound : a_mgef->effectSounds) {
				if (effectSound.id == static_cast<MagicSystem::SoundID>(a_type)) {
					effectSound.sound = a_sound;
					break;
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------------
	// OBJECTREFERENCES
	//--------------------------------------------------------------------------------------------

	// [x, y, z]
	std::vector<float> PO3_SKSEFunctions::GetPositionAsArray(StaticFunctionTag*, TESObjectREFR* a_ref)
	{
		std::vector<float> pos;
		pos.resize(3);

		if (a_ref) {
			pos[0] = a_ref->GetPositionX();
			pos[1] = a_ref->GetPositionY();
			pos[2] = a_ref->GetPositionZ();
		}

		return pos;
	}


	// [angleX, angleY, angleZ]
	std::vector<float> PO3_SKSEFunctions::GetRotationAsArray(StaticFunctionTag*, TESObjectREFR* a_ref)
	{
		std::vector<float> angles;
		angles.resize(3);

		if (a_ref) {
			angles[0] = a_ref->GetRotationX() * static_cast<float>(180.0f / M_PI);
			angles[1] = a_ref->GetRotationY() * static_cast<float>(180.0f / M_PI);
			angles[2] = a_ref->GetRotationZ() * static_cast<float>(180.0f / M_PI);
		}

		return angles;
	}


	bool PO3_SKSEFunctions::IsLoadDoor(StaticFunctionTag*, TESObjectREFR* a_door)
	{
		if (a_door) {
			if (a_door->extraList.HasType(ExtraDataType::kTeleport)) {
				return true;
			}
		}
		return false;
	}


	void PO3_SKSEFunctions::AddAllInventoryItemsToList(StaticFunctionTag*, TESObjectREFR* a_ref, BGSListForm* list, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem)
	{
		if (a_ref && list) {
			auto changes = a_ref->GetInventoryChanges();

			if (changes && changes->entryList) {
				for (auto& entry : *changes->entryList) {
					if (entry && entry->CanItemBeTaken(a_noEquipped, a_noFavourited, a_noQuestItem)) {
						auto object = entry->object;
						if (object) {
							list->AddForm(object);
						}
					}
				}
			}
		}
	}

	std::vector<TESForm*> PO3_SKSEFunctions::AddAllInventoryItemsToArray(StaticFunctionTag*, TESObjectREFR* a_ref, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem)
	{
		std::vector<TESForm*> vec;

		if (a_ref) {
			auto changes = a_ref->GetInventoryChanges();
			if (changes && changes->entryList) {
				for (auto& entry : *changes->entryList) {
					if (entry && entry->CanItemBeTaken(a_noEquipped, a_noFavourited, a_noQuestItem)) {
						auto object = entry->object;
						if (object) {
							vec.push_back(object);
						}
					}
				}
			}
		}

		return vec;
	}

	// replaces keyword on ref
	void PO3_SKSEFunctions::ReplaceKeywordOnRef(StaticFunctionTag*, TESObjectREFR* a_ref, BGSKeyword* a_remove, BGSKeyword* a_add)
	{
		if (!a_ref || !a_remove || !a_add) {
			return;
		}

		auto base = a_ref->GetBaseObject();
		if (base) {
			auto keywordForm = base->As<BGSKeywordForm>();
			if (keywordForm) {
				UInt32 removeIndex = 0;
				BGSKeyword* keyword = nullptr;
				bool found = false;

				for (UInt32 i = 0; i < keywordForm->numKeywords; i++) {
					keyword = keywordForm->keywords[i];
					if (keyword) {
						if (keyword->formEditorID == a_add->formEditorID) {
							return;
						}
						if (keyword->formEditorID == a_remove->formEditorID) {
							removeIndex = i;
							found = true;
						}
					}
				}
				if (found) {
					keywordForm->keywords[removeIndex] = a_add;
				}
			}
		}
	}

	void PO3_SKSEFunctions::AddKeywordToRef(StaticFunctionTag*, TESObjectREFR* a_ref, BGSKeyword* a_add)
	{
		if (a_ref && a_add) {
			auto base = a_ref->GetBaseObject();
			if (base) {
				auto keywords = Keywords::GetSingleton();
				if (keywords) {
					keywords->PapyrusApplyKeywords(base, a_add, Base::kAdd);
				}
			}
		}
	}

	bool PO3_SKSEFunctions::RemoveKeywordFromRef(StaticFunctionTag*, TESObjectREFR* a_ref, BGSKeyword* a_remove)
	{
		if (a_ref && a_remove) {
			auto base = a_ref->GetBaseObject();
			if (base) {
				auto keywords = Keywords::GetSingleton();
				if (keywords) {
					return keywords->PapyrusApplyKeywords(base, a_remove, Base::kRemove);
				}
			}
		}
		return false;
	}

	//calculates a 2D vector
	void PO3_SKSEFunctions::Apply2DHavokImpulse(StaticFunctionTag*, TESObjectREFR* a_src, TESObjectREFR* a_tgt, float a_Z, float a_magnitude)
	{
		if (!a_src || !a_tgt || !a_src->Is3DLoaded() || !a_tgt->Is3DLoaded()) {
			return;
		}

		float sourceZ = a_src->GetRotationZ() * static_cast <float>(180 / M_PI);
		float angleZ = sourceZ + a_src->GetHeadingAngle(a_tgt);

		auto vm = BSScript::Internal::VirtualMachine::GetSingleton();
		if (vm) {
			g_task->AddTask([vm, a_tgt, angleZ, a_Z, a_magnitude]()
			{
				Papyrus::ApplyHavokImpulse(vm, 0, a_tgt, asinf(angleZ), acosf(angleZ), a_Z, a_magnitude);
			});
		}

	}

	//calculates a 3D vector and takes into account the elevation between a_src and a_tgt.
	void PO3_SKSEFunctions::Apply3DHavokImpulse(StaticFunctionTag*, TESObjectREFR* a_src, TESObjectREFR* a_tgt, float a_magnitude)
	{
		if (!a_src || !a_tgt || !a_src->Is3DLoaded() || !a_tgt->Is3DLoaded()) {
			return;
		}

		float dx = a_tgt->GetPositionX() - a_src->GetPositionX();
		float dy = a_tgt->GetPositionY() - a_src->GetPositionY();
		float dz = a_tgt->GetPositionZ() - a_src->GetPositionZ();

		float dist = NiPoint3::GetDistance(a_tgt->GetPosition(), a_src->GetPosition());

		float x = dx / dist; //x
		float y = dy / dist; //y
		float z = dz / dist; //z


		auto vm = BSScript::Internal::VirtualMachine::GetSingleton();
		if (vm) {
			g_task->AddTask([vm, a_tgt, x, y, z, a_magnitude]()
			{
				Papyrus::ApplyHavokImpulse(vm, 0, a_tgt, x, y, z, a_magnitude);
			});
		}
	}


	std::optional<NiPoint3> FindNearestVertex(TESObjectREFR* a_ref)
	{
		auto cell = a_ref->GetParentCell();

		if (!cell || !cell->navMeshes) {
			return std::nullopt;
		}

		auto& navMeshes = *cell->navMeshes;
		auto shortestDistance = std::numeric_limits<float>::max();
		std::optional<NiPoint3> pos = std::nullopt;

		for (auto& navMesh : navMeshes.navMeshes) {
			for (auto& vertex : navMesh->vertices) {
				auto linearDistance = NiPoint3::GetDistance(a_ref->GetPosition(), vertex.location);
				if (linearDistance < shortestDistance) {
					shortestDistance = linearDistance;
					pos.emplace(vertex.location);
				}
			}
		}

		return pos;
	}


	void PO3_SKSEFunctions::MoveToNearestNavmeshLocation(StaticFunctionTag*, TESObjectREFR* a_ref)
	{
		if (!a_ref) {
			return;
		}
		auto nearestVertex = FindNearestVertex(a_ref);
		if (!nearestVertex) {
			return;
		}
		auto handle = a_ref->CreateRefHandle();
		g_task->AddTask([handle, nearestVertex]()
		{
			auto ref = handle.get();
			if (!ref.get()) {
				return;
			}
			ref->SetPosition(std::move(*nearestVertex));
		});
	}


	std::vector<TESEffectShader*> PO3_SKSEFunctions::GetAllEffectShaders(StaticFunctionTag*, TESObjectREFR* a_ref)
	{
		std::vector<TESEffectShader*> vec;

		if (a_ref) {
			auto processLists = ProcessLists::GetSingleton();
			if (processLists) {
				processLists->magicEffectsLock.Lock();
				for (auto& tempEffect : processLists->magicEffects) {
					if (tempEffect.get()) {
						auto shaderEffect = netimmerse_cast<ShaderReferenceEffect*>(tempEffect.get());
						if (shaderEffect) {
							auto handle = a_ref->CreateRefHandle();
							if (shaderEffect->target == handle) {
								if (shaderEffect->effectData) {
									vec.push_back(shaderEffect->effectData);
								}
							}
						}
					}
				}
				processLists->magicEffectsLock.Unlock();
			}
		}

		return vec;
	}


	UInt32 PO3_SKSEFunctions::HasEffectShader(StaticFunctionTag*, TESObjectREFR* a_ref, TESEffectShader* a_effectShader, bool a_active)
	{
		UInt32 effectShaderCount = 0;

		if (a_ref && a_effectShader) {
			auto processLists = ProcessLists::GetSingleton();
			if (processLists) {
				processLists->magicEffectsLock.Lock();
				for (auto& tempEffect : processLists->magicEffects) {
					if (tempEffect.get()) {
						auto shaderEffect = netimmerse_cast<ShaderReferenceEffect*>(tempEffect.get());
						if (shaderEffect) {
							auto handle = a_ref->CreateRefHandle();
							if (shaderEffect->target == handle) {
								auto effectData = shaderEffect->effectData;
								if (effectData && effectData == a_effectShader) {
									if (a_active && shaderEffect->finished == 1) {
										continue;
									}
									effectShaderCount++;
								}
							}
						}
					}
				}
				processLists->magicEffectsLock.Unlock();
			}
		}
		return effectShaderCount;
	}


	std::vector<BGSArtObject*> PO3_SKSEFunctions::GetAllArtObjects(StaticFunctionTag*, TESObjectREFR* a_ref)
	{
		std::vector<BGSArtObject*> vec;

		if (a_ref) {
			auto processLists = ProcessLists::GetSingleton();
			if (processLists) {
				processLists->magicEffectsLock.Lock();
				for (auto& tempEffect : processLists->magicEffects) {
					if (tempEffect.get()) {
						auto modelEffect = netimmerse_cast<ModelReferenceEffect*>(tempEffect.get());
						if (modelEffect) {
							auto handle = a_ref->CreateRefHandle();
							if (modelEffect->target == handle) {
								if (modelEffect->artObject) {
									vec.push_back(modelEffect->artObject);
								}
							}
						}
					}
				}
				processLists->magicEffectsLock.Unlock();
			}
		}

		return vec;
	}


	UInt32 PO3_SKSEFunctions::HasArtObject(StaticFunctionTag*, TESObjectREFR* a_ref, BGSArtObject* a_artObject, bool a_active)
	{
		UInt32 artObjectCount = 0;

		if (a_ref && a_artObject) {
			auto processLists = ProcessLists::GetSingleton();
			if (processLists) {
				processLists->magicEffectsLock.Lock();
				for (auto& tempEffect : processLists->magicEffects) {
					if (tempEffect.get()) {
						auto modelEffect = netimmerse_cast<ModelReferenceEffect*>(tempEffect.get());
						if (modelEffect) {
							auto handle = a_ref->CreateRefHandle();
							if (modelEffect->target == handle) {
								auto modelArt = modelEffect->artObject;
								if (modelArt && modelArt == a_artObject) {
									if (a_active && modelEffect->finished == 1) {
										continue;
									}
									artObjectCount++;
								}
							}
						}
					}
				}
				processLists->magicEffectsLock.Unlock();
			}
		}

		return artObjectCount;
	}


	void PO3_SKSEFunctions::StopArtObject(StaticFunctionTag*, TESObjectREFR* a_ref, BGSArtObject* a_artObject)
	{
		if (a_ref && a_artObject) {
			auto processLists = ProcessLists::GetSingleton();
			if (processLists) {
				processLists->magicEffectsLock.Lock();
				for (auto& tempEffect : processLists->magicEffects) {
					if (tempEffect.get()) {
						auto modelEffect = netimmerse_cast<ModelReferenceEffect*>(tempEffect.get());
						if (modelEffect) {
							auto handle = a_ref->CreateRefHandle();
							if (modelEffect->target == handle) {
								auto modelArt = modelEffect->artObject;
								if (modelArt && modelArt == a_artObject) {
									modelEffect->finished = 1;
								}
							}
						}
					}
				}
				processLists->magicEffectsLock.Unlock();
			}
		}
	}


	void PO3_SKSEFunctions::StopAllShaders(StaticFunctionTag*, TESObjectREFR* a_ref)
	{
		if (a_ref) {
			a_ref->StopAllShaders();
		}
	}


	Actor* PO3_SKSEFunctions::GetActorCause(StaticFunctionTag*, TESObjectREFR* a_ref)
	{
		if (a_ref) {
			auto actorCause = a_ref->GetActorCause();
			if (actorCause) {
				auto actorPtr = actorCause->actor.get();
				if (actorPtr.get()) {
					return actorPtr.get();
				}
			}
		}

		return nullptr;
	}


	Actor* PO3_SKSEFunctions::GetClosestActorFromRef(StaticFunctionTag*, TESObjectREFR* a_ref, bool a_ignorePlayer)
	{
		if (a_ref) {
			auto processLists = ProcessLists::GetSingleton();
			if (processLists) {
				if (a_ignorePlayer && processLists->numberHighActors == 0) {
					return nullptr;
				}

				const auto originPos = a_ref->GetPosition();
				auto shortestDistance = std::numeric_limits<float>::max();
				std::map<Actor*, float> map;

				for (auto& actorHandle : processLists->highActorHandles) {
					auto actorPtr = actorHandle.get();
					auto actor = actorPtr.get();
					if (actor && actor != a_ref) {
						auto distance = NiPoint3::GetDistance(originPos, actorPtr->GetPosition());
						map.emplace(actor, distance);

						if (distance < shortestDistance) {
							shortestDistance = distance;
						}
					}
				}
				if (!a_ignorePlayer) {
					auto player = PlayerCharacter::GetSingleton();
					if (player && a_ref != player) {
						auto distance = NiPoint3::GetDistance(originPos, player->GetPosition());
						map.emplace(player, distance);
						if (distance < shortestDistance) {
							shortestDistance = distance;
						}
					}
				}
				auto it = std::find_if(map.begin(), map.end(), [shortestDistance](const auto& mo)
				{
					return mo.second == shortestDistance;
				});
				if (it != map.end()) {
					return it->first;
				}
			}
		}
		return nullptr;
	}


	Actor* PO3_SKSEFunctions::GetRandomActorFromRef(StaticFunctionTag*, TESObjectREFR* a_ref, float a_radius, bool a_ignorePlayer)
	{
		if (a_ref) {
			auto processLists = ProcessLists::GetSingleton();
			if (processLists) {
				if (a_ignorePlayer && processLists->numberHighActors == 0) {
					return nullptr;
				}

				std::vector<Actor*> vec;
				vec.reserve(processLists->numberHighActors);

				auto squaredRadius = a_radius * a_radius;
				auto originPos = a_ref->GetPosition();

				for (auto& actorHandle : processLists->highActorHandles) {
					auto actorPtr = actorHandle.get();
					auto actor = actorPtr.get();
					if (!actor || actor == a_ref) {
						continue;
					}
					auto distance = NiPoint3::GetSquaredDistance(originPos, actor->GetPosition());
					if (distance > squaredRadius) {
						continue;
					}
					vec.emplace_back(actor);
				}
				if (!a_ignorePlayer) {
					auto player = PlayerCharacter::GetSingleton();
					if (player && a_ref != player) {
						auto distance = NiPoint3::GetDistance(originPos, player->GetPosition());
						if (distance <= squaredRadius) {
							vec.emplace_back(player);
						}
					}
				}
				UInt32 rand = 0;
				auto random = Random::GetSingleton();
				if (random) {
					rand = random->operator()(0, static_cast<UInt32>(vec.size()) - 1);
				}
				return vec.at(rand);
			}
		}
		return nullptr;
	}

	//;

	void FindAllRefsOfType_Impl(std::vector<TESObjectREFR*>& a_vec, TESObjectCELL* a_cell, TESForm* a_form, BGSListForm* a_list, const NiPoint3& a_pos, float a_radius)
	{
		a_cell->spinLock.Lock();

		for (auto& refPtr : a_cell->references) {
			auto ref = refPtr.get();
			if (ref) {
				auto distance = NiPoint3::GetSquaredDistance(a_pos, ref->GetPosition());
				if (distance <= a_radius) {
					auto base = ref->GetBaseObject();
					if (base) {
						if (a_list) {
							if (!a_list->forms.empty()) {
								for (auto& form : a_list->forms) {
									if (form && form == base) {
										a_vec.push_back(ref);
									}
								}
							}
							if (a_list->scriptAddedTempForms) {
								for (const auto& formID : *a_list->scriptAddedTempForms) {
									auto form = TESForm::LookupByID(formID);
									if (form && form == base) {
										a_vec.push_back(ref);
									}
								}
							}
						}
						else if (a_form == base) {
							a_vec.push_back(ref);
						}
					}
				}
			}
		}

		a_cell->spinLock.Unlock();
	}


	std::vector<TESObjectREFR*> PO3_SKSEFunctions::FindAllReferencesOfType(StaticFunctionTag*, TESObjectREFR* a_ref, TESForm* a_formOrList, float a_radius)
	{
		std::vector<TESObjectREFR*> vec;

		if (a_ref && a_formOrList) {
			auto TES = TES::GetSingleton();
			if (TES) {
				auto formList = a_formOrList->As<BGSListForm>();

				auto refPos = a_ref->GetPosition();
				auto squaredRadius = a_radius * a_radius;

				auto cell = TES->currentInteriorCell;
				if (cell) {
					FindAllRefsOfType_Impl(vec, cell, a_formOrList, formList, refPos, squaredRadius);
				}
				else {
					auto gridCellArray = TES->gridCellArray;
					if (gridCellArray) {
						auto gridLength = gridCellArray->length;
						if (gridLength > 0) {
							UInt32 x = 0;
							UInt32 y = 0;
							float yPlus = refPos.y + a_radius;
							float yMinus = refPos.y - a_radius;
							float xPlus = refPos.x + a_radius;
							float xMinus = refPos.x - a_radius;

							for (x = 0, y = 0; (x < gridLength && y < gridLength); x++, y++) {
								cell = gridCellArray->GetCell(x, y);
								if (cell && cell->IsAttached()) {
									auto cellCoords = cell->GetCoordinates();
									if (cellCoords) {
										if (cellCoords->worldX < xPlus && (cellCoords->worldX + 4096.0) > xMinus&& cellCoords->worldY < yPlus && (cellCoords->worldY + 4096.0) > yMinus) {
											FindAllRefsOfType_Impl(vec, cell, a_formOrList, formList, refPos, squaredRadius);
										}
									}
								}
							}
						}
					}
				}
				if (vec.empty()) {
					auto worldSpace = TES->worldSpace;
					if (worldSpace) {
						cell = worldSpace->GetOrCreateSkyCell();
						if (cell) {
							FindAllRefsOfType_Impl(vec, cell, a_formOrList, formList, refPos, squaredRadius);
						}
					}
				}
			}
		}

		return vec;
	}

	//;

	void FindAllRefsWithKYWD_Impl(std::vector<TESObjectREFR*>& a_vec, TESObjectCELL* a_cell, BGSKeyword* a_KYWD, BGSListForm* a_list, const NiPoint3& a_pos, float a_radius, bool a_matchAll)
	{
		a_cell->spinLock.Lock();

		for (auto& refPtr : a_cell->references) {
			auto ref = refPtr.get();
			if (ref) {
				auto distance = NiPoint3::GetSquaredDistance(a_pos, ref->GetPosition());
				if (distance <= a_radius) {
					bool success = false;
					if (a_list) {
						success = a_matchAll ? ref->HasAllKeywords(a_list) : ref->HasKeywords(a_list);
					}
					else if (a_KYWD) {
						success = ref->HasKeyword(a_KYWD);
					}
					if (success) {
						a_vec.push_back(ref);
					}
				}
			}
		}

		a_cell->spinLock.Unlock();
	}


	std::vector<TESObjectREFR*> PO3_SKSEFunctions::FindAllReferencesWithKeyword(StaticFunctionTag*, TESObjectREFR* a_ref, TESForm* a_keywordOrList, float a_radius, bool a_matchAll)
	{
		std::vector<TESObjectREFR*> vec;

		if (a_ref && a_keywordOrList) {
			auto TES = TES::GetSingleton();
			if (TES) {
				auto keyword = a_keywordOrList->As<BGSKeyword>();
				auto formList = a_keywordOrList->As<BGSListForm>();

				if (!keyword && !formList) {
					return vec;
				}

				NiPoint3 refPos = a_ref->GetPosition();
				float squaredRadius = a_radius * a_radius;

				auto cell = TES->currentInteriorCell;
				if (cell) {
					FindAllRefsWithKYWD_Impl(vec, cell, keyword, formList, refPos, squaredRadius, a_matchAll);
				}
				else {
					auto gridCellArray = TES->gridCellArray;
					if (gridCellArray) {
						auto gridLength = gridCellArray->length;
						if (gridLength > 0) {
							UInt32 x = 0;
							UInt32 y = 0;
							float yPlus = refPos.y + a_radius;
							float yMinus = refPos.y - a_radius;
							float xPlus = refPos.x + a_radius;
							float xMinus = refPos.x - a_radius;

							for (x = 0, y = 0; (x < gridLength && y < gridLength); x++, y++) {
								cell = gridCellArray->GetCell(x, y);
								if (cell && cell->IsAttached()) {
									auto cellCoords = cell->GetCoordinates();
									if (cellCoords) {
										if (cellCoords->worldX < xPlus && (cellCoords->worldX + 4096.0) > xMinus&& cellCoords->worldY < yPlus && (cellCoords->worldY + 4096.0) > yMinus) {
											FindAllRefsWithKYWD_Impl(vec, cell, keyword, formList, refPos, squaredRadius, a_matchAll);
										}
									}
								}
							}
						}
					}
				}
				if (vec.empty()) {
					auto worldSpace = TES->worldSpace;
					if (worldSpace) {
						cell = worldSpace->GetOrCreateSkyCell();
						if (cell) {
							FindAllRefsWithKYWD_Impl(vec, cell, keyword, formList, refPos, squaredRadius, a_matchAll);
						}
					}
				}
			}
		}

		return vec;
	}


	float PO3_SKSEFunctions::GetEffectShaderDuration(StaticFunctionTag*, TESObjectREFR* a_ref, TESEffectShader* a_effectShader)
	{
		float time = 0.0;

		if (a_ref && a_effectShader) {
			auto processLists = ProcessLists::GetSingleton();
			if (processLists) {
				processLists->magicEffectsLock.Lock();
				for (auto& tempEffect : processLists->magicEffects) {
					if (tempEffect.get()) {
						auto shaderEffect = netimmerse_cast<ShaderReferenceEffect*>(tempEffect.get());
						if (shaderEffect) {
							auto handle = a_ref->CreateRefHandle();
							if (shaderEffect->target == handle) {
								auto effectData = shaderEffect->effectData;
								if (effectData && effectData == a_effectShader) {
									time = shaderEffect->lifetime;
									break;
								}
							}
						}
					}
				}
				processLists->magicEffectsLock.Unlock();
			}
		}

		return time;
	}


	void PO3_SKSEFunctions::SetEffectShaderDuration(StaticFunctionTag*, TESObjectREFR* a_ref, TESEffectShader* a_effectShader, float a_time, bool a_absolute)
	{
		if (a_ref && a_effectShader) {
			auto processLists = ProcessLists::GetSingleton();
			if (processLists) {
				processLists->magicEffectsLock.Lock();
				for (auto& tempEffect : processLists->magicEffects) {
					if (tempEffect.get()) {
						auto shaderEffect = netimmerse_cast<ShaderReferenceEffect*>(tempEffect.get());
						if (shaderEffect) {
							auto handle = a_ref->CreateRefHandle();
							if (shaderEffect->target == handle) {
								auto effectData = shaderEffect->effectData;
								if (effectData && effectData == a_effectShader) {
									if (!a_absolute) {
										float value = shaderEffect->lifetime + a_time;
										if (value >= -1.0) {
											shaderEffect->lifetime = a_time;
										}
									}
									else {
										shaderEffect->lifetime = a_time;
									}
									break;
								}
							}
						}
					}
				}
				processLists->magicEffectsLock.Unlock();
			}
		}
	}


	void PO3_SKSEFunctions::SetupBodyPartGeometry(StaticFunctionTag*, TESObjectREFR* a_miscItem, Actor* a_actor)
	{
		if (a_actor && a_miscItem && a_miscItem->Get3D()) {
			auto actorbase = a_actor->GetActorBase();
			auto root = a_miscItem->Get3D()->AsFadeNode();
			if (actorbase && root) {
				NiIntegerExtraData* data = nullptr;
				auto actorRoot = a_actor->Get3D(0);
				if (actorRoot) {
					data = static_cast<NiIntegerExtraData*>(actorRoot->GetExtraData(BSFixedString("PO3_SKINTINT")));
				}
				NiColor color = data ? NiColor(data->value) : actorbase->bodyTintColor;
				g_task->AddTask([root, color]()
				{
					root->UpdateBodyTint(color);
				});
			}
		}
	}


	void PO3_SKSEFunctions::SetShaderType(StaticFunctionTag*, TESObjectREFR* a_ref, TESObjectREFR* a_template, BSFixedString a_filter, UInt32 a_shaderType, SInt32 a_textureType, bool a_noWeapons, bool a_noAlpha)
	{
		if (a_ref && a_template && a_ref->Is3DLoaded() && a_template->Is3DLoaded()) {

			using Texture = BSTextureSet::Texture;
			using Feature = BSShaderMaterial::Feature;

			auto sourcePath = std::string();
			if (!a_filter.empty()) {
				sourcePath = a_filter.c_str();
				Util::SanitizeTexturePath(sourcePath);
			}

			bool isActor = a_ref->formType == FormType::ActorCharacter ? true : false;

			auto feature = static_cast<BSShaderMaterial::Feature>(a_shaderType);
			auto root = a_ref->Get3D()->AsNode();
			auto template_root = a_template->Get3D()->AsNode();

			if (root && template_root) {

				g_task->AddTask([root, template_root, sourcePath, feature, a_textureType, a_noWeapons, a_noAlpha, isActor]()
				{
					auto template_geo = template_root->GetFirstGeometryOfShaderType(feature);
					if (template_geo) {
						std::vector<std::vector<BSFixedString>> vec;
						SetShaderType_Impl(root, template_geo, sourcePath, a_textureType, vec, a_noWeapons, a_noAlpha, isActor);

						if (!vec.empty()) {
							for (auto& _vec : vec) {
								std::string name = "PO3_SHADER | " + std::string(_vec[12].c_str()) + " | " + std::string(_vec.back().c_str());
								auto data = static_cast<NiStringsExtraData*>(root->GetExtraData(BSFixedString(name.c_str())));
								if (!data) {
									auto newData = NiStringsExtraData::Create(BSFixedString(name.c_str()), _vec.data(), static_cast<UInt32>(_vec.size()));
									if (newData) {
										root->AddExtraData(newData);
									}
								}
							}
						}
					}
				});
			}
		}
	}


	bool PO3_SKSEFunctions::HasNiExtraData(StaticFunctionTag*, TESObjectREFR* a_ref, BSFixedString a_name)
	{
		if (a_ref) {
			auto root = a_ref->Get3D();
			if (root && root->HasExtraData(a_name)) {
				return true;
			}
		}

		return false;
	}

	//--------------------------------------------------------------------------------------------
	// PACKAGE
	//--------------------------------------------------------------------------------------------

	SInt32 PO3_SKSEFunctions::GetPackageType(StaticFunctionTag*, TESPackage* a_package)
	{
		return a_package ? static_cast<UInt32>(a_package->packData.packType) : -1;
	}

	//--------------------------------------------------------------------------------------------
	// PROJECTILE
	//--------------------------------------------------------------------------------------------

	float PO3_SKSEFunctions::GetProjectileSpeed(StaticFunctionTag*, BGSProjectile* a_projectile)
	{
		return a_projectile ? a_projectile->data.speed : 0.0f;
	}


	void PO3_SKSEFunctions::SetProjectileSpeed(StaticFunctionTag*, BGSProjectile* a_projectile, float a_speed)
	{
		if (a_projectile) {
			a_projectile->data.speed = a_speed;
		}
	}


	float PO3_SKSEFunctions::GetProjectileRange(StaticFunctionTag*, BGSProjectile* a_projectile)
	{
		return a_projectile ? a_projectile->data.range : 0.0f;
	}


	void PO3_SKSEFunctions::SetProjectileRange(StaticFunctionTag*, BGSProjectile* a_projectile, float a_range)
	{
		if (a_projectile) {
			a_projectile->data.range = a_range;
		}
	}


	float PO3_SKSEFunctions::GetProjectileGravity(StaticFunctionTag*, BGSProjectile* a_projectile)
	{
		return a_projectile ? a_projectile->data.gravity : 0.0f;
	}


	void PO3_SKSEFunctions::SetProjectileGravity(StaticFunctionTag*, BGSProjectile* a_projectile, float a_gravity)
	{
		if (a_projectile) {
			a_projectile->data.gravity = a_gravity;
		}
	}


	float PO3_SKSEFunctions::GetProjectileImpactForce(StaticFunctionTag*, BGSProjectile* a_projectile)
	{
		return a_projectile ? a_projectile->data.force : 0.0f;
	}


	void PO3_SKSEFunctions::SetProjectileImpactForce(StaticFunctionTag*, BGSProjectile* a_projectile, float a_force)
	{
		if (a_projectile) {
			a_projectile->data.force = a_force;
		}
	}


	UInt32 PO3_SKSEFunctions::GetProjectileType(StaticFunctionTag*, BGSProjectile* a_projectile)
	{
		if (a_projectile) {
			const auto types = a_projectile->data.types;

			if ((types & BGSProjectileData::Type::kMissile) == BGSProjectileData::Type::kMissile) //Missile
			{
				return 1;
			}
			if ((types & BGSProjectileData::Type::kGrenade) == BGSProjectileData::Type::kGrenade) //Lobber (runes)
			{
				return 2;
			}
			if ((types & BGSProjectileData::Type::kBeam) == BGSProjectileData::Type::kBeam) //Beam
			{
				return 3;
			}
			if ((types & BGSProjectileData::Type::kFlamethrower) == BGSProjectileData::Type::kFlamethrower) //Flame
			{
				return 4;
			}
			if ((types & BGSProjectileData::Type::kCone) == BGSProjectileData::Type::kCone) //Cone
			{
				return 5;
			}
			if ((types & BGSProjectileData::Type::kBarrier) == BGSProjectileData::Type::kBarrier) //Barrier
			{
				return 6;
			}
			if ((types & BGSProjectileData::Type::kArrow) == BGSProjectileData::Type::kArrow) //Arrow
			{
				return 7;
			}
		}

		return 0;
	}

	//--------------------------------------------------------------------------------------------
	// SOUND
	//--------------------------------------------------------------------------------------------

	void PO3_SKSEFunctions::SetSoundDescriptor(StaticFunctionTag*, TESSound* a_sound, BGSSoundDescriptorForm* a_soundDescriptor)
	{
		if (a_sound && a_soundDescriptor) {
			a_sound->descriptor = a_soundDescriptor;
		}
	}

	//--------------------------------------------------------------------------------------------
	// SPELL
	//--------------------------------------------------------------------------------------------

	UInt32 PO3_SKSEFunctions::GetSpellType(StaticFunctionTag*, SpellItem* a_spell)
	{
		return a_spell ? static_cast<UInt32>(a_spell->data.spellType) : 0;
	}


	bool PO3_SKSEFunctions::HasActiveSpell(StaticFunctionTag*, Actor* a_actor, SpellItem* a_spell)
	{
		using AE = ActiveEffect::Flag;

		if (a_actor && a_spell) {
			auto activeEffects = a_actor->GetActiveEffectList();
			if (activeEffects) {
				for (auto& ae : *activeEffects) {
					if (ae && ae->spell == a_spell) {
						if ((ae->flags & AE::kInactive) != AE::kInactive && (ae->flags & AE::kDispelled) != AE::kDispelled) {
							return true;
						}
					}
				}
			}
		}
		return false;
	}

	//--------------------------------------------------------------------------------------------
	// STRING
	//--------------------------------------------------------------------------------------------

	SInt32 PO3_SKSEFunctions::StringToInt(StaticFunctionTag*, BSFixedString a_string)
	{
		SInt32 value = -1;

		if (!a_string.empty()) {
			try {
				value = std::stoul(a_string.c_str(), nullptr, 0);
			}
			catch (const std::out_of_range & e) {
				value = -1;
			}
		}

		return value;
	}


	BSFixedString PO3_SKSEFunctions::IntToString(StaticFunctionTag*, UInt32 a_int, bool a_hex)
	{
		if (a_hex) {
			std::stringstream stream;
			stream << "0x" << std::hex << a_int;
			return BSFixedString(stream.str());
		}
		return std::to_string(a_int).c_str();
	}

	//--------------------------------------------------------------------------------------------
	// VISUALEFFECT
	//--------------------------------------------------------------------------------------------

	BGSArtObject* PO3_SKSEFunctions::GetArtObject(StaticFunctionTag*, BGSReferenceEffect* a_vfx)
	{
		return a_vfx ? a_vfx->data.artObject : nullptr;
	}


	void PO3_SKSEFunctions::SetArtObject(StaticFunctionTag*, BGSReferenceEffect* a_vfx, BGSArtObject* a_art)
	{
		if (a_vfx && a_art) {
			a_vfx->data.artObject = a_art;
		}
	}


	UInt32 PO3_SKSEFunctions::GetArtObjectTotalCount(StaticFunctionTag*, BGSReferenceEffect* a_vfx, bool a_active)
	{
		UInt32 count = 0;

		if (a_vfx) {
			auto visualArt = a_vfx->data.artObject;
			if (visualArt) {
				auto processLists = ProcessLists::GetSingleton();
				if (processLists) {
					processLists->magicEffectsLock.Lock();
					for (auto& tempEffect : processLists->magicEffects) {
						if (tempEffect.get()) {
							auto modelEffect = netimmerse_cast<ModelReferenceEffect*>(tempEffect.get());
							if (modelEffect) {
								auto modelArt = modelEffect->artObject;
								if (modelArt && modelArt == visualArt) {
									if (a_active && modelEffect->finished == 1) {
										continue;
									}
									count++;
								}
							}
						}
					}
					processLists->magicEffectsLock.Unlock();
				}
			}
		}

		return count;
	}

	//--------------------------------------------------------------------------------------------
	// WEATHER
	//--------------------------------------------------------------------------------------------

	//returns wind speed from 0-255 (how it's set up in the weather form)
	UInt32 PO3_SKSEFunctions::GetWindSpeedAsInt(StaticFunctionTag*, TESWeather* a_weather)
	{
		return a_weather ? a_weather->data.windSpeed : 0;
	}


	//returns wind speed from 0.0-1.0 (how it's set up in the CK)
	float PO3_SKSEFunctions::GetWindSpeedAsFloat(StaticFunctionTag*, TESWeather* a_weather)
	{
		return a_weather ? a_weather->data.windSpeed / 255.0f : 0.0f;
	}


	SInt32 PO3_SKSEFunctions::GetWeatherType(StaticFunctionTag*, TESWeather* a_weather)
	{
		TESWeather* currentWeather = nullptr;

		if (a_weather) {
			currentWeather = a_weather;
		}
		if (!currentWeather) {
			currentWeather = Sky::GetSingleton()->currentWeather;
		}
		if (currentWeather) {
			const auto flags = currentWeather->data.flags;

			if ((flags & TESWeather::WeatherDataFlag::kPleasant) != TESWeather::WeatherDataFlag::kNone) {
				return 0;
			}
			if ((flags & TESWeather::WeatherDataFlag::kCloudy) != TESWeather::WeatherDataFlag::kNone) {
				return 1;
			}
			if ((flags & TESWeather::WeatherDataFlag::kRainy) != TESWeather::WeatherDataFlag::kNone) {
				return 2;
			}
			if ((flags & TESWeather::WeatherDataFlag::kSnow) != TESWeather::WeatherDataFlag::kNone) {
				return 3;
			}
		}

		return -1;
	}

	//--------------------------------------------------------------------------------------------
	// REGISTER
	//--------------------------------------------------------------------------------------------

	// Tell the game about the new functions.
	bool PO3_SKSEFunctions::Register(BSScript::Internal::VirtualMachine* a_vm)
	{
		if (!a_vm) {
			_MESSAGE("couldn't get VMState");
			return false;
		}

		a_vm->RegisterFunction("GetHairColor", "PO3_SKSEFunctions", GetHairColor);
		a_vm->RegisterFunction("GetSkinColor", "PO3_SKSEFunctions", GetSkinColor);
		a_vm->RegisterFunction("SetHairColor", "PO3_SKSEFunctions", SetHairColor);
		a_vm->RegisterFunction("SetSkinColor", "PO3_SKSEFunctions", SetSkinColor);
		a_vm->RegisterFunction("BlendColorWithSkinTone", "PO3_SKSEFunctions", BlendColorWithSkinTone);
		a_vm->RegisterFunction("MixColorWithSkinTone", "PO3_SKSEFunctions", MixColorWithSkinTone);
		a_vm->RegisterFunction("SetSkinAlpha", "PO3_SKSEFunctions", SetSkinAlpha);
		a_vm->RegisterFunction("EquipArmorIfSkinVisible", "PO3_SKSEFunctions", EquipArmorIfSkinVisible);
		a_vm->RegisterFunction("ReplaceArmorTextureSet", "PO3_SKSEFunctions", ReplaceArmorTextureSet);
		a_vm->RegisterFunction("ReplaceSkinTextureSet", "PO3_SKSEFunctions", ReplaceSkinTextureSet);
		a_vm->RegisterFunction("ReplaceFaceTextureSet", "PO3_SKSEFunctions", ReplaceFaceTextureSet);
		a_vm->RegisterFunction("GetHeadPartTextureSet", "PO3_SKSEFunctions", GetHeadPartTextureSet);
		a_vm->RegisterFunction("SetHeadPartTextureSet", "PO3_SKSEFunctions", SetHeadPartTextureSet);
		a_vm->RegisterFunction("SetHeadPartAlpha", "PO3_SKSEFunctions", SetHeadPartAlpha);
		a_vm->RegisterFunction("ToggleChildNode", "PO3_SKSEFunctions", ToggleChildNode);
		a_vm->RegisterFunction("RemoveChildNode", "PO3_SKSEFunctions", RemoveChildNode);
		a_vm->RegisterFunction("IsActorSoulTrapped", "PO3_SKSEFunctions", IsActorSoulTrapped);
		a_vm->RegisterFunction("ResetActor3D", "PO3_SKSEFunctions", ResetActor3D);
		a_vm->RegisterFunction("RemoveEffectsNotOfType", "PO3_SKSEFunctions", RemoveEffectsNotOfType);
		a_vm->RegisterFunction("DecapitateActor", "PO3_SKSEFunctions", DecapitateActor);
		a_vm->RegisterFunction("GetTimeDead", "PO3_SKSEFunctions", GetTimeDead);
		a_vm->RegisterFunction("GetTimeOfDeath", "PO3_SKSEFunctions", GetTimeOfDeath);
		a_vm->RegisterFunction("GetRunningPackage", "PO3_SKSEFunctions", GetRunningPackage);
		a_vm->RegisterFunction("IsActorInWater", "PO3_SKSEFunctions", IsActorInWater);
		a_vm->RegisterFunction("GetActorAlpha", "PO3_SKSEFunctions", GetActorAlpha);
		a_vm->RegisterFunction("GetActorRefraction", "PO3_SKSEFunctions", GetActorRefraction);
		a_vm->RegisterFunction("SetActorRefraction", "PO3_SKSEFunctions", SetActorRefraction);
		a_vm->RegisterFunction("GetActorState", "PO3_SKSEFunctions", GetActorState);
		a_vm->RegisterFunction("InstantKill", "PO3_SKSEFunctions", InstantKill);
		a_vm->RegisterFunction("AddBasePerk", "PO3_SKSEFunctions", AddBasePerk);
		a_vm->RegisterFunction("RemoveBasePerk", "PO3_SKSEFunctions", RemoveBasePerk);

		a_vm->RegisterFunction("AddBaseSpell", "PO3_SKSEFunctions", AddBaseSpell);
		a_vm->RegisterFunction("RemoveBaseSpell", "PO3_SKSEFunctions", RemoveBaseSpell);
		a_vm->RegisterFunction("GetPerkCount", "PO3_SKSEFunctions", GetPerkCount);
		a_vm->RegisterFunction("GetNthPerk", "PO3_SKSEFunctions", GetNthPerk);

		a_vm->RegisterFunction("AddActorToArray", "PO3_SKSEFunctions", AddActorToArray);
		a_vm->RegisterFunction("AddStringToArray", "PO3_SKSEFunctions", AddStringToArray);
		a_vm->RegisterFunction("ArrayStringCount", "PO3_SKSEFunctions", ArrayStringCount);
		a_vm->RegisterFunction("SortArrayString", "PO3_SKSEFunctions", SortArrayString);
		a_vm->RegisterFunction("GetSortedActorNameArray", "PO3_SKSEFunctions", GetSortedActorNameArray);

		a_vm->RegisterFunction("GetAttachedCells", "PO3_SKSEFunctions", GetAttachedCells);

		a_vm->RegisterFunction("GetEffectShaderFullParticleCount", "PO3_SKSEFunctions", GetEffectShaderFullParticleCount);
		a_vm->RegisterFunction("SetEffectShaderFullParticleCount", "PO3_SKSEFunctions", SetEffectShaderFullParticleCount);
		a_vm->RegisterFunction("GetEffectShaderPersistentParticleCount", "PO3_SKSEFunctions", GetEffectShaderPersistentParticleCount);
		a_vm->RegisterFunction("SetEffectShaderPersistentParticleCount", "PO3_SKSEFunctions", SetEffectShaderPersistentParticleCount);
		a_vm->RegisterFunction("IsEffectShaderFlagSet", "PO3_SKSEFunctions", IsEffectShaderFlagSet);
		a_vm->RegisterFunction("SetEffectShaderFlag", "PO3_SKSEFunctions", SetEffectShaderFlag);
		a_vm->RegisterFunction("ClearEffectShaderFlag", "PO3_SKSEFunctions", ClearEffectShaderFlag);
		a_vm->RegisterFunction("GetEffectShaderTotalCount", "PO3_SKSEFunctions", GetEffectShaderTotalCount);

		a_vm->RegisterFunction("ReplaceKeywordOnForm", "PO3_SKSEFunctions", ReplaceKeywordOnForm);
		a_vm->RegisterFunction("AddKeywordToForm", "PO3_SKSEFunctions", AddKeywordToForm);
		a_vm->RegisterFunction("RemoveKeywordOnForm", "PO3_SKSEFunctions", RemoveKeywordOnForm);

		a_vm->RegisterFunction("IsPluginFound", "PO3_SKSEFunctions", IsPluginFound, true);
		a_vm->RegisterFunction("GetAllSpellsInMod", "PO3_SKSEFunctions", GetAllSpellsInMod);
		a_vm->RegisterFunction("GetAllRacesInMod", "PO3_SKSEFunctions", GetAllRacesInMod);
		a_vm->RegisterFunction("AddAllGameSpellsToList", "PO3_SKSEFunctions", AddAllGameSpellsToList);
		a_vm->RegisterFunction("AddAllGameRacesToList", "PO3_SKSEFunctions", AddAllGameRacesToList);
		a_vm->RegisterFunction("GetActorsByProcessingLevel", "PO3_SKSEFunctions", GetActorsByProcessingLevel);
		a_vm->RegisterFunction("GetNumActorsInHigh", "PO3_SKSEFunctions", GetNumActorsInHigh);

		a_vm->RegisterFunction("GetLightRadius", "PO3_SKSEFunctions", GetLightRadius);
		a_vm->RegisterFunction("SetLightRadius", "PO3_SKSEFunctions", SetLightRadius);
		a_vm->RegisterFunction("GetLightFade", "PO3_SKSEFunctions", GetLightFade);
		a_vm->RegisterFunction("SetLightFade", "PO3_SKSEFunctions", SetLightFade);
		a_vm->RegisterFunction("GetLightColor", "PO3_SKSEFunctions", GetLightColor);
		a_vm->RegisterFunction("SetLightColor", "PO3_SKSEFunctions", SetLightColor);
		a_vm->RegisterFunction("GetLightType", "PO3_SKSEFunctions", GetLightType);
		a_vm->RegisterFunction("SetLightType", "PO3_SKSEFunctions", SetLightType);
		a_vm->RegisterFunction("GetLightFOV", "PO3_SKSEFunctions", GetLightFOV);
		a_vm->RegisterFunction("SetLightFOV", "PO3_SKSEFunctions", SetLightFOV);
		a_vm->RegisterFunction("GetLightShadowDepthBias", "PO3_SKSEFunctions", GetLightShadowDepthBias);
		a_vm->RegisterFunction("SetLightShadowDepthBias", "PO3_SKSEFunctions", SetLightShadowDepthBias);

		a_vm->RegisterFunction("GetParentLocation", "PO3_SKSEFunctions", GetParentLocation);
		a_vm->RegisterFunction("SetParentLocation", "PO3_SKSEFunctions", SetParentLocation);

		a_vm->RegisterFunction("GenerateRandomFloat", "PO3_SKSEFunctions", GenerateRandomFloat, true);
		a_vm->RegisterFunction("GenerateRandomInt", "PO3_SKSEFunctions", GenerateRandomInt, true);

		a_vm->RegisterFunction("GetAllActiveEffectsOnActor", "PO3_SKSEFunctions", GetAllActiveEffectsOnActor);
		a_vm->RegisterFunction("HasMagicEffectWithArchetype", "PO3_SKSEFunctions", HasMagicEffectWithArchetype);
		a_vm->RegisterFunction("GetEffectArchetypeAsInt", "PO3_SKSEFunctions", GetEffectArchetypeAsInt);
		a_vm->RegisterFunction("GetEffectArchetypeAsString", "PO3_SKSEFunctions", GetEffectArchetypeAsString);
		a_vm->RegisterFunction("GetMagicEffectSound", "PO3_SKSEFunctions", GetMagicEffectSound);
		a_vm->RegisterFunction("SetMagicEffectSound", "PO3_SKSEFunctions", SetMagicEffectSound);

		a_vm->RegisterFunction("GetPositionAsArray", "PO3_SKSEFunctions", GetPositionAsArray);
		a_vm->RegisterFunction("GetRotationAsArray", "PO3_SKSEFunctions", GetRotationAsArray);
		a_vm->RegisterFunction("IsLoadDoor", "PO3_SKSEFunctions", IsLoadDoor);
		a_vm->RegisterFunction("AddAllInventoryItemsToList", "PO3_SKSEFunctions", AddAllInventoryItemsToList);
		a_vm->RegisterFunction("AddAllInventoryItemsToArray", "PO3_SKSEFunctions", AddAllInventoryItemsToArray);
		a_vm->RegisterFunction("AddAllEquippedItemsToArray", "PO3_SKSEFunctions", AddAllEquippedItemsToArray);
		a_vm->RegisterFunction("ReplaceKeywordOnRef", "PO3_SKSEFunctions", ReplaceKeywordOnRef);
		a_vm->RegisterFunction("AddKeywordToRef", "PO3_SKSEFunctions", AddKeywordToRef);
		a_vm->RegisterFunction("RemoveKeywordFromRef", "PO3_SKSEFunctions", RemoveKeywordFromRef);
		a_vm->RegisterFunction("Apply2DHavokImpulse", "PO3_SKSEFunctions", Apply2DHavokImpulse);
		a_vm->RegisterFunction("Apply3DHavokImpulse", "PO3_SKSEFunctions", Apply3DHavokImpulse);
		a_vm->RegisterFunction("MoveToNearestNavmeshLocation", "PO3_SKSEFunctions", MoveToNearestNavmeshLocation);
		a_vm->RegisterFunction("GetAllEffectShaders", "PO3_SKSEFunctions", GetAllEffectShaders);
		a_vm->RegisterFunction("HasEffectShader", "PO3_SKSEFunctions", HasEffectShader);
		a_vm->RegisterFunction("GetAllArtObjects", "PO3_SKSEFunctions", GetAllArtObjects);
		a_vm->RegisterFunction("HasArtObject", "PO3_SKSEFunctions", HasArtObject);
		a_vm->RegisterFunction("StopArtObject", "PO3_SKSEFunctions", StopArtObject);
		a_vm->RegisterFunction("StopAllShaders", "PO3_SKSEFunctions", StopAllShaders);
		a_vm->RegisterFunction("GetActorCause", "PO3_SKSEFunctions", GetActorCause);
		a_vm->RegisterFunction("GetClosestActorFromRef", "PO3_SKSEFunctions", GetClosestActorFromRef);
		a_vm->RegisterFunction("GetRandomActorFromRef", "PO3_SKSEFunctions", GetRandomActorFromRef);
		a_vm->RegisterFunction("FindAllReferencesOfType", "PO3_SKSEFunctions", FindAllReferencesOfType);
		a_vm->RegisterFunction("FindAllReferencesWithKeyword", "PO3_SKSEFunctions", FindAllReferencesWithKeyword);
		a_vm->RegisterFunction("GetEffectShaderDuration", "PO3_SKSEFunctions", GetEffectShaderDuration);
		a_vm->RegisterFunction("SetEffectShaderDuration", "PO3_SKSEFunctions", SetEffectShaderDuration);
		a_vm->RegisterFunction("SetupBodyPartGeometry", "PO3_SKSEFunctions", SetupBodyPartGeometry);
		a_vm->RegisterFunction("SetShaderType", "PO3_SKSEFunctions", SetShaderType);
		a_vm->RegisterFunction("HasNiExtraData", "PO3_SKSEFunctions", HasNiExtraData);

		a_vm->RegisterFunction("GetPackageType", "PO3_SKSEFunctions", GetPackageType);

		a_vm->RegisterFunction("GetProjectileSpeed", "PO3_SKSEFunctions", GetProjectileSpeed);
		a_vm->RegisterFunction("SetProjectileSpeed", "PO3_SKSEFunctions", SetProjectileSpeed);
		a_vm->RegisterFunction("GetProjectileRange", "PO3_SKSEFunctions", GetProjectileRange);
		a_vm->RegisterFunction("SetProjectileRange", "PO3_SKSEFunctions", SetProjectileRange);
		a_vm->RegisterFunction("GetProjectileGravity", "PO3_SKSEFunctions", GetProjectileGravity);
		a_vm->RegisterFunction("SetProjectileGravity", "PO3_SKSEFunctions", SetProjectileGravity);
		a_vm->RegisterFunction("GetProjectileImpactForce", "PO3_SKSEFunctions", GetProjectileImpactForce);
		a_vm->RegisterFunction("SetProjectileImpactForce", "PO3_SKSEFunctions", SetProjectileImpactForce);
		a_vm->RegisterFunction("GetProjectileType", "PO3_SKSEFunctions", GetProjectileType);

		a_vm->RegisterFunction("SetSoundDescriptor", "PO3_SKSEFunctions", SetSoundDescriptor);

		a_vm->RegisterFunction("GetSpellType", "PO3_SKSEFunctions", GetSpellType);
		a_vm->RegisterFunction("HasActiveSpell", "PO3_SKSEFunctions", HasActiveSpell);

		a_vm->RegisterFunction("StringToInt", "PO3_SKSEFunctions", StringToInt);
		a_vm->RegisterFunction("IntToString", "PO3_SKSEFunctions", IntToString);

		a_vm->RegisterFunction("GetArtObject", "PO3_SKSEFunctions", GetArtObject);
		a_vm->RegisterFunction("SetArtObject", "PO3_SKSEFunctions", SetArtObject);
		a_vm->RegisterFunction("GetArtObjectTotalCount", "PO3_SKSEFunctions", GetArtObjectTotalCount);

		a_vm->RegisterFunction("GetWindSpeedAsInt", "PO3_SKSEFunctions", GetWindSpeedAsInt);
		a_vm->RegisterFunction("GetWindSpeedAsFloat", "PO3_SKSEFunctions", GetWindSpeedAsFloat);
		//a_vm->RegisterFunction("GetWeatherType", "PO3_SKSEFunctions", GetWeatherType);

		_MESSAGE("Registered papyrus functions");

		return true;
	}
}