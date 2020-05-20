#include "po3_resetfunctions.h"

extern const SKSE::TaskInterface* g_task;
extern std::vector<RE::BGSBipedObjectForm::BipedObjectSlot> fxSlots;


namespace RE
{
	void StopAllSkinAlphaShaders(TESObjectREFR* a_ref)
	{
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
							if (effectData && ((effectData->data.flags & EffectShaderData::Flags::kSkinOnly) == EffectShaderData::Flags::kSkinOnly)
								&& !effectData->holesTexture.textureName.empty()) {
								shaderEffect->finished = 1;
							}
						}
					}
				}
			}
			processLists->magicEffectsLock.Unlock();
		}
	}


	void ResetToggleData(NiAVObject* a_root, NiStringsExtraData* a_data)
	{
		g_task->AddTask([a_root, a_data]()
		{
			for (UInt32 i = 0; i < a_data->size; i++) {
				auto nodeName = BSFixedString(a_data->value[i]);
				auto object = a_root->GetObjectByName(nodeName);
				if (object) {
					object->UpdateVisibility(false);
				}
			}
			a_root->RemoveExtraData(a_data);
		});
	}


	void ResetAlphaData(Actor* a_actor, NiAVObject* a_root, NiFloatExtraData* a_data)
	{
		for (auto& slot : fxSlots) {
			auto armor = a_actor->GetWornArmor(slot);
			if (armor) {
				a_actor->UnequipItem(0, armor);
			}
		}
		
		g_task->AddTask([a_actor, a_root, a_data]()
		{
			a_root->UpdateAlpha(1.0, NiAVObject::ALPHA_MODE::kSkin);
			a_root->RemoveExtraData(a_data);
		});

	}


	void ResetHeadPartAlphaData(Actor* a_actor, NiAVObject* a_root, NiIntegersExtraData* a_data)
	{
		g_task->AddTask([a_actor, a_root, a_data]()
		{
			for (UInt32 i = 0; i < a_data->size; i++) {
				auto object = a_actor->GetHeadPartObject(static_cast<BGSHeadPart::HeadPartType>(a_data->value[i]));
				if (object) {
					object->UpdateAlpha(1.0, NiAVObject::ALPHA_MODE::kAll);
				}
			}
			a_root->RemoveExtraData(a_data);
		});
	}


	void ResetSkinTintData(Actor* a_actor, NiAVObject* a_root, NiIntegersExtraData* a_data)
	{
		g_task->AddTask([a_actor, a_root, a_data]()
		{
			auto actorBase = a_actor->GetActorBase();
			if (actorBase) {
				auto faceNode = a_actor->GetFaceNodeSkinned();
				auto facePart = actorBase->GetCurrentHeadPartByType(BGSHeadPart::HeadPartType::kFace);
				if (faceNode && facePart) {
					auto faceGen = FaceGen::GetSingleton();
					if (faceGen) {
						faceGen->RegenerateHead(faceNode, facePart, actorBase);
					}
				}
				a_root->UpdateBodyTint(actorBase->bodyTintColor);
			}
			a_root->RemoveExtraData(a_data);
		});
	}


	void ResetHairTintData(Actor* a_actor, NiAVObject* a_root, NiIntegersExtraData* a_data)
	{
		g_task->AddTask([a_actor, a_root, a_data]()
		{
			auto actorBase = a_actor->GetActorBase();
			if (actorBase) {
				auto headData = actorBase->headRelatedData;
				if (headData) {
					auto colorForm = headData->hairColor;
					if (colorForm) {
						a_root->UpdateHairColor(colorForm->color);
					}
				}
			}
			a_root->RemoveExtraData(a_data);
		});
	}


	void ResetFaceTXSTData(Actor* a_actor, NiAVObject* a_root, NiStringsExtraData* a_data)
	{
		g_task->AddTask([a_actor, a_root, a_data]()
		{
			auto textureset = BSShaderTextureSet::Create();
			if (textureset) {
				for (auto i = BSTextureSet::Texture::kDiffuse; i < BSTextureSet::Textures::kTotal; ++i) {
					if (a_data->value[i] && *a_data->value[i] != '\0') {
						textureset->SetTexturePath(i, a_data->value[i]);
					}
				}
				auto faceObject = a_actor->GetHeadPartObject(BGSHeadPart::HeadPartType::kFace);
				if (faceObject && textureset) {
					ResetTextureSet(faceObject, *textureset, NiAVObject::ALPHA_MODE::kSkin, std::string());
				}
			}
			a_root->RemoveExtraData(a_data);
		});
	}


	void ResetTXSTData(Actor* a_actor, NiAVObject* a_root, const BSFixedString& a_folderName, const std::vector<NiStringsExtraData*>& a_vec)
	{
		std::string folder = a_folderName.c_str();
		Util::SanitizeTexturePath(folder);

		g_task->AddTask([a_actor, a_root, folder, a_vec]()
		{
			for (auto& a_data : a_vec) {
				if (a_data) {
					std::string armorID = a_data->value[a_data->size - 1];
					if (!armorID.empty()) {
						auto ID = static_cast<FormID>(std::stoul(armorID));
						auto armor = a_actor->GetWornArmor(ID);
						if (!armor) {
							auto actorBase = a_actor->GetActorBase();
							if (actorBase) {
								armor = actorBase->skin;
							}
						}
						if (armor) {
							auto textureset = BSShaderTextureSet::Create();
							if (textureset) {
								for (auto i = BSTextureSet::Texture::kDiffuse; i < BSTextureSet::Textures::kTotal; ++i) {
									if (a_data->value[i] && *a_data->value[i] != '\0') {
										textureset->SetTexturePath(i, a_data->value[i]);
									}
								}
								for (auto& armorAddon : armor->armorAddons) {
									if (armorAddon) {
										NiAVObject* armorObject = a_actor->VisitArmorAddon(armor, armorAddon);
										if (armorObject) {
											ResetTextureSet(armorObject, *textureset, NiAVObject::ALPHA_MODE::kAll, folder);
										}
									}
								}
							}
						}
					}
					a_root->RemoveExtraData(a_data);
				}
			}
		});
	}


	void ResetSkinTXSTData(Actor* a_actor, NiAVObject* a_root, const std::vector<NiStringsExtraData*>& a_vec)
	{
		g_task->AddTask([a_actor, a_root, a_vec]()
		{
			for (auto& a_data : a_vec) {
				if (a_data) {
					std::string slotMaskstr = a_data->value[a_data->size - 1];
					if (!slotMaskstr.empty()) {
						auto a_slot = static_cast<BGSBipedObjectForm::FirstPersonFlag>(std::stoul(slotMaskstr));
						TESObjectARMO* skinarmor = a_actor->GetSkin(a_slot);
						if (skinarmor) {
							auto newTextureSet = BSShaderTextureSet::Create();
							if (newTextureSet) {
								for (auto i = BSTextureSet::Texture::kDiffuse; i < BSTextureSet::Textures::kTotal; ++i) {
									if (a_data->value[i] && *a_data->value[i] != '\0') {
										newTextureSet->SetTexturePath(i, a_data->value[i]);
									}
								}
								auto foundAddon = skinarmor->GetArmorAddonByMask(a_actor->race, a_slot);
								if (foundAddon) {
									auto armorObject = a_actor->VisitArmorAddon(skinarmor, foundAddon);
									if (armorObject) {
										ResetTextureSet(armorObject, *newTextureSet, NiAVObject::ALPHA_MODE::kSkin, std::string());
									}
								}
							}
						}
					}
					a_root->RemoveExtraData(a_data);
				}
			}
		});
	}


	void ResetShaderData(NiAVObject* a_root, const std::vector<NiStringsExtraData*>& a_vec)
	{
		using Flag = BSShaderProperty::EShaderPropertyFlag;
		using Feature = BSShaderMaterial::Feature;

		g_task->AddTask([a_root, a_vec]()
		{
			for (auto& a_data : a_vec) {
				if (a_data) {							
					auto textureset = BSShaderTextureSet::Create();
					if (textureset) {
						for (auto i = BSTextureSet::Texture::kDiffuse; i < BSTextureSet::Textures::kTotal; ++i) {
							if (a_data->value[i] && *a_data->value[i] != '\0') {
								textureset->SetTexturePath(i, a_data->value[i]);
							}
						}
						auto flags = static_cast<Flag>(std::stoul(a_data->value[9]));
						auto emissiveColor = NiColor(std::stoul(a_data->value[10]));
						auto emissiveMult = std::stof(a_data->value[11]);
						auto changedFeature = static_cast<Feature>(std::stoul(a_data->value[12]));
						auto originalFeature = static_cast<Feature>(std::stoul(a_data->value[13]));
						std::string changedDiffuse = a_data->value[14];
										
						ResetShaderType(a_root, *textureset, flags, emissiveColor, emissiveMult, changedFeature, originalFeature, changedDiffuse);
					}
				}
				a_root->RemoveExtraData(a_data);
			}
		});
	}
}