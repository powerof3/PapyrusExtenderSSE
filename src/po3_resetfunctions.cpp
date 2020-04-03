#include "po3_resetfunctions.h"

extern const SKSE::TaskInterface* g_task;

namespace RE
{
	//---------------------------------------------------------------------------------

	void StopAllSkinAlphaShaders(TESObjectREFR* a_ref)
	{
		auto singleton = ProcessLists::GetSingleton();

		singleton->magicEffectsLock.Lock();
		for (auto& tempEffect : singleton->magicEffects) {
			if (!tempEffect || !tempEffect.get()) {
				continue;
			}
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
		singleton->magicEffectsLock.Unlock();
	}

	//---------------------------------------------------------------------------------

	void ResetToggleData(Actor* a_actor, NiAVObject* a_root, NiStringsExtraData* a_data)
	{
		g_task->AddTask([a_actor, a_root, a_data]()
		{
			for (UInt32 i = 0; i < a_data->size; i++) {
				auto object = a_root->GetObjectByName(BSFixedString(a_data->value[i]));
				if (object) {
					object->UpdateVisibility(false);
				}
			}
			a_root->RemoveExtraData(a_data);
		});
	}


	void ResetAlphaData(Actor* a_actor, NiAVObject* a_root, NiFloatExtraData* a_data)
	{
		if (!a_actor->IsPlayerRef()) {
			a_actor->ResetInventory(false);
		}

		g_task->AddTask([a_actor, a_root, a_data]()
		{
			auto faceGeometry = a_actor->GetHeadPartGeometry(BGSHeadPart::HeadPartType::kFace);
			if (faceGeometry) {
				faceGeometry->SetMaterialAlpha(1.0, true);
			}
			SetArmorSkinAlpha(a_actor, BGSBipedObjectForm::FirstPersonFlag::kBody, 1.0);
			SetArmorSkinAlpha(a_actor, BGSBipedObjectForm::FirstPersonFlag::kHands, 1.0);
			SetArmorSkinAlpha(a_actor, BGSBipedObjectForm::FirstPersonFlag::kFeet, 1.0);
			SetArmorSkinAlpha(a_actor, BGSBipedObjectForm::FirstPersonFlag::kTail, 1.0);
			SetArmorSkinAlpha(a_actor, BGSBipedObjectForm::FirstPersonFlag::kDecapitate, 1.0);
			SetArmorSkinAlpha(a_actor, BGSBipedObjectForm::FirstPersonFlag::kDecapitateHead, 1.0);

			a_root->RemoveExtraData(a_data);
		});
	}


	void ResetHeadPartAlphaData(Actor* a_actor, NiAVObject* a_root, NiIntegersExtraData* a_data)
	{
		g_task->AddTask([a_actor, a_root, a_data]()
		{
			for (UInt32 i = 0; i < a_data->size; i++) {
				auto geometry = a_actor->GetHeadPartGeometry(static_cast<BGSHeadPart::HeadPartType>(a_data->value[i]));
				if (geometry) {
					geometry->SetMaterialAlpha(1.0, false);
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
				if (a_actor->IsPlayerRef()) {
					auto faceNode = a_actor->GetFaceNodeSkinned();
					auto facePart = actorBase->GetCurrentHeadPartByType(BGSHeadPart::HeadPartType::kFace);

					if (faceNode && facePart) {
						auto singleton = FaceGen::GetSingleton();
						singleton->RegenerateHead(faceNode, facePart, actorBase);
					}
				}
				NiColor color;
				color.red = actorBase->bodyTintColor.red / static_cast <float>(255.0);
				color.green = actorBase->bodyTintColor.green / static_cast <float>(255.0);
				color.blue = actorBase->bodyTintColor.blue / static_cast <float>(255.0);
				a_root->UpdateBodyTint(color);
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
				auto colorForm = actorBase->headRelatedData->hairColor;
				if (colorForm) {
					NiColor color;
					color.red = colorForm->color.red / static_cast <float>(255.0);
					color.green = colorForm->color.green / static_cast <float>(255.0);
					color.blue = colorForm->color.blue / static_cast <float>(255.0);

					a_root->UpdateHairColor(color);
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
			for (auto i = BSTextureSet::Texture::kDiffuse; i < BSTextureSet::Textures::kTotal; ++i) {
				if (a_data->value[i] && *a_data->value[i] != '\0') {
					textureset->SetTexturePath(i, a_data->value[i]);
				}
			}
			auto faceGeometry = a_actor->GetHeadPartGeometry(BGSHeadPart::HeadPartType::kFace);
			if (faceGeometry) {
				ResetTextureSet(faceGeometry, textureset, true);
			}
			a_root->RemoveExtraData(a_data);
		});
	}


	void ResetTXSTData(Actor* a_actor, NiAVObject* a_root, const BSFixedString& a_folderName, std::vector<NiStringsExtraData*>& a_vec)
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
							for (auto i = BSTextureSet::Texture::kDiffuse; i < BSTextureSet::Textures::kTotal; ++i) {
								if (a_data->value[i] && *a_data->value[i] != '\0') {
									textureset->SetTexturePath(i, a_data->value[i]);
								}
							}
							for (auto& armorAddon : armor->armorAddons) {
								if (armorAddon) {
									NiAVObject* armorObject = a_actor->VisitArmorAddon(armor, armorAddon);
									if (armorObject) {
										ResetTextureSet(armorObject, textureset, false, folder);
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


	void ResetSkinTXSTData(Actor* a_actor, NiAVObject* a_root, std::vector<NiStringsExtraData*>& a_vec)
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
							auto textureset = BSShaderTextureSet::Create();
							for (auto i = BSTextureSet::Texture::kDiffuse; i < BSTextureSet::Textures::kTotal; ++i) {
								if (a_data->value[i] && *a_data->value[i] != '\0') {
									textureset->SetTexturePath(i, a_data->value[i]);
								}
							}
							TESObjectARMA* foundAddon = skinarmor->GetArmorAddonByMask(a_actor->race, a_slot);
							if (foundAddon) {
								auto armorObject = a_actor->VisitArmorAddon(skinarmor, foundAddon);
								if (armorObject) {
									ResetTextureSet(armorObject, textureset, true);
								}
							}
						}
					}
					a_root->RemoveExtraData(a_data);
				}
			}
		});
	}
}