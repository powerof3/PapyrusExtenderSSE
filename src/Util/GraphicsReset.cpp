#include "Util/GraphicsReset.h"


namespace GraphicsReset
{
	std::uint32_t constexpr const_hash(const char* input, std::uint32_t hash = 5381)
	{
		return *input ?
					 const_hash(input + 1, hash * 33 + static_cast<std::uint32_t>(*input)) :
					 hash;
	}


	ResetData GetResetData(RE::NiAVObject* a_object)
	{
		ResetData resetData;

		if (!a_object->extra || a_object->extraDataSize == 0) {
			return resetData;
		}
		
		for (std::uint16_t i = 0; i < a_object->extraDataSize; i++) {
			auto extraData = a_object->extra[i];
			if (!extraData) {
				continue;
			}
			std::string name(extraData->name.c_str());
			if (name.empty()) {
				continue;
			}

			auto& [toggle, tintSkin, tintHair, alpha, alphaHDPT, TXSTFace, TXST, TXSTSkin, shader] = resetData;

			switch (const_hash(name.c_str())) {
			case const_hash("PO3_TOGGLE"):
				{
					toggle = static_cast<RE::NiStringsExtraData*>(extraData);
				}
				break;
			case const_hash("PO3_SKINTINT"):
				{
					tintSkin = static_cast<RE::NiIntegersExtraData*>(extraData);
				}
				break;
			case const_hash("PO3_HAIRTINT"):
				{
					tintHair = static_cast<RE::NiIntegersExtraData*>(extraData);
				}
				break;
			case const_hash("PO3_ALPHA"):
				{
					alpha = static_cast<RE::NiFloatExtraData*>(extraData);
				}
				break;
			case const_hash("PO3_HEADPARTALPHA"):
				{
					alphaHDPT = static_cast<RE::NiIntegersExtraData*>(extraData);
				}
				break;
			case const_hash("PO3_FACETXST"):
				{
					TXSTFace = static_cast<RE::NiStringsExtraData*>(extraData);
				}
				break;
			default:
				{
					if (name.find("PO3_TXST"sv) != std::string::npos) {
						TXST.emplace_back(static_cast<RE::NiStringsExtraData*>(extraData));
					} else if (name.find("PO3_SKINTXST"sv) != std::string::npos) {
						TXSTSkin.emplace_back(static_cast<RE::NiStringsExtraData*>(extraData));
					} else if (name.find("PO3_SHADER"sv) != std::string::npos) {
						shader.emplace_back(static_cast<RE::NiStringsExtraData*>(extraData));
					}
				}
				break;
			}
		}
		return resetData;
	}


	void ResetToggleData(RE::NiAVObject* a_root, RE::NiStringsExtraData* a_data)
	{
		for (std::uint32_t i = 0; i < a_data->size; i++) {
			auto object = a_root->GetObjectByName(a_data->value[i]);
			if (object) {
				object->ToggleNode(false);
			}
		}
		a_root->RemoveExtraData(a_data);
	}


	void ResetSkinTintData(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiIntegersExtraData* a_data)
	{
		using HeadPartType = RE::BGSHeadPart::HeadPartType;

		auto actorbase = a_actor->GetActorBase();
		if (actorbase) {
			auto faceNode = a_actor->GetFaceNodeSkinned();
			auto facePart = actorbase->GetCurrentHeadPartByType(HeadPartType::kFace);
			if (faceNode && facePart) {
				auto task = SKSE::GetTaskInterface();
				task->AddTask([a_actor, a_root, a_data, actorbase, faceNode, facePart]() {
					auto faceGen = RE::FaceGen::GetSingleton();
					if (faceGen) {
						faceGen->RegenerateHead(faceNode, facePart, actorbase);
					}
					a_root->UpdateBodyTint(actorbase->bodyTintColor);
					a_root->RemoveExtraData(a_data);
				});
			}
		}
	}


	void ResetHairTintData(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiIntegersExtraData* a_data)
	{
		auto actorBase = a_actor->GetActorBase();
		if (actorBase) {
			auto headData = actorBase->headRelatedData;
			if (headData) {
				auto colorForm = headData->hairColor;
				if (colorForm) {
					a_root->UpdateHairColor(colorForm->color);
					a_root->RemoveExtraData(a_data);
				}
			}
		}
	}


	void ResetAlphaData(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiFloatExtraData* a_data, const RE::BSFixedString& a_folderName)
	{
		a_root->UpdateMaterialAlpha(1.0, true);
		a_root->RemoveExtraData(a_data);
	}


	void ResetHeadPartAlphaData(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiIntegersExtraData* a_data)
	{
		using HeadPartType = RE::BGSHeadPart::HeadPartType;

		for (std::uint32_t i = 0; i < a_data->size; i++) {
			auto object = a_actor->GetHeadPartObject(static_cast<HeadPartType>(a_data->value[i]));
			if (object) {
				object->UpdateMaterialAlpha(1.0f, false);
			}
		}
		a_root->RemoveExtraData(a_data);
	}


	void ResetTextureSet(RE::NiAVObject* a_object, RE::BSShaderTextureSet* a_txst, bool a_onlySkin, std::string_view a_folder)
	{
		RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
			using State = RE::BSGeometry::States;
			using Feature = RE::BSShaderMaterial::Feature;
			using Texture = RE::BSTextureSet::Texture;

			auto effect = a_geometry->properties[State::kEffect].get();
			if (effect) {
				auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect);
				if (lightingShader) {
					auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
					if (material) {
						if (!a_onlySkin) {
							if (!a_folder.empty()) {
								std::string sourcePath = material->textureSet->GetTexturePath(Texture::kDiffuse);
								RE::Util::SanitizeTexturePath(sourcePath);
								if (sourcePath.find(a_folder) == std::string::npos) {
									return RE::BSVisit::BSVisitControl::kContinue;
								}
							}
						} else {
							auto type = material->GetFeature();
							if (type != Feature::kFaceGenRGBTint && type != Feature::kFaceGen) {
								return RE::BSVisit::BSVisitControl::kContinue;
							}
						}
						auto newMaterial = static_cast<RE::BSLightingShaderMaterialBase*>(material->Create());
						if (newMaterial) {
							newMaterial->CopyMembers(material);
							newMaterial->ClearTextures();
							newMaterial->OnLoadTextureSet(0, a_txst);
							lightingShader->SetMaterial(newMaterial, 1);
							lightingShader->InitializeGeometry(a_geometry);
							lightingShader->InitializeShader(a_geometry);
							newMaterial->~BSLightingShaderMaterialBase();
							RE::free(newMaterial);
						}
					}
				}
			}
			return RE::BSVisit::BSVisitControl::kContinue;
		});
	}


	void ResetFaceTXSTData(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiStringsExtraData* a_data)
	{
		using HeadPartType = RE::BGSHeadPart::HeadPartType;
		using Texture = RE::BSTextureSet::Texture;

		auto textureset = RE::BSShaderTextureSet::Create();
		if (textureset) {
			for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
				if (a_data->value[i] && *a_data->value[i] != '\0') {
					textureset->SetTexturePath(i, a_data->value[i]);
				}
			}
			auto faceObject = a_actor->GetHeadPartObject(HeadPartType::kFace);
			if (faceObject) {
				ResetTextureSet(faceObject, textureset, true, std::string());
			}
		}
		a_root->RemoveExtraData(a_data);
	}


	void ResetTXSTData(RE::Actor* a_actor, RE::NiAVObject* a_root, const RE::BSFixedString& a_folderName, const std::vector<RE::NiStringsExtraData*>& a_vec)
	{
		using Texture = RE::BSTextureSet::Texture;

		std::string folder = a_folderName.c_str();
		RE::Util::SanitizeTexturePath(folder);

		for (auto& a_data : a_vec) {
			if (a_data) {
				std::string armorID = a_data->value[a_data->size - 1];
				std::uint32_t formID = 0;
				if (!armorID.empty()) {
					try {
						formID = std::stoul(armorID);
					} catch (...) {
						SKSE::log::warn("ResetTXSTData - Unable to get armor ID!"sv);
					}
				}
				auto armor = a_actor->GetWornArmor(formID);
				if (!armor) {
					auto actorBase = a_actor->GetActorBase();
					if (actorBase) {
						armor = actorBase->skin;
					}
				}
				if (armor) {
					auto textureset = RE::BSShaderTextureSet::Create();
					if (textureset) {
						for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
							if (a_data->value[i] && *a_data->value[i] != '\0') {
								textureset->SetTexturePath(i, a_data->value[i]);
							}
						}
						for (auto& armorAddon : armor->armorAddons) {
							if (armorAddon) {
								RE::NiAVObject* armorObject = a_actor->VisitArmorAddon(armor, armorAddon);
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
	}


	void ResetSkinTXSTData(RE::Actor* a_actor, RE::NiAVObject* a_root, const std::vector<RE::NiStringsExtraData*>& a_vec)
	{
		using Slot = RE::BGSBipedObjectForm::FirstPersonFlag;
		using Texture = RE::BSTextureSet::Texture;

		for (auto& data : a_vec) {
			if (data) {
				std::string slotMaskstr = data->value[data->size - 1];
				Slot a_slot = Slot::kNone;
				if (!slotMaskstr.empty()) {
					try {
						a_slot = static_cast<Slot>(std::stoul(slotMaskstr));
					} catch (...) {
					}
				}
				auto skinarmor = a_actor->GetSkin(a_slot);
				if (skinarmor) {
					auto foundAddon = skinarmor->GetArmorAddonByMask(a_actor->race, a_slot);
					if (foundAddon) {
						auto armorObject = a_actor->VisitArmorAddon(skinarmor, foundAddon);
						if (armorObject) {
							auto newTextureSet = RE::BSShaderTextureSet::Create();
							if (newTextureSet) {
								for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
									if (data->value[i] && *data->value[i] != '\0') {
										newTextureSet->SetTexturePath(i, data->value[i]);
									}
								}
								ResetTextureSet(armorObject, newTextureSet, true, std::string());
							}
						}
					}
				}
				a_root->RemoveExtraData(data);
			}
		}
	}


	void ResetShaderData_Impl(RE::NiAVObject* a_object, ShaderData data)
	{
		RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
			if (!a_geometry->HasVertexFlag(VertexFlags::kNormal)) {
				return RE::BSVisit::BSVisitControl::kContinue;
			}
			auto parent = a_geometry->parent;
			if (parent && parent->AsFadeNode()) {
				return RE::BSVisit::BSVisitControl::kContinue;
			}
			auto alpha = a_geometry->properties[State::kProperty].get();
			if (alpha) {
				return RE::BSVisit::BSVisitControl::kContinue;
			}

			auto& [texture, flags, emissiveColor, emissiveMult, feature, featureOrig, diffuse] = data;

			auto effect = a_geometry->properties[State::kEffect].get();
			if (effect) {
				auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect);
				if (lightingShader) {
					auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
					if (material) {
						if (material->GetFeature() == feature) {
							std::string currentDiffuse = material->textureSet->GetTexturePath(Texture::kDiffuse);
							RE::Util::SanitizeTexturePath(currentDiffuse);
							if (currentDiffuse.find(diffuse) == std::string::npos) {
								return RE::BSVisit::BSVisitControl::kContinue;
							}

							auto newMaterial = RE::BSLightingShaderMaterialBase::CreateMaterial(featureOrig);
							if (newMaterial) {
								newMaterial->CopyBaseMembers(material);
								newMaterial->ClearTextures();
								newMaterial->OnLoadTextureSet(0, texture);
								lightingShader->SetMaterial(newMaterial, 1);
								lightingShader->InitializeGeometry(a_geometry);
								lightingShader->InitializeShader(a_geometry);
								lightingShader->flags = flags;
								lightingShader->lastRenderPassState = std::numeric_limits<std::int32_t>::max();
								if (lightingShader->flags.all(Flag::kOwnEmit)) {
									lightingShader->emissiveColor->red = emissiveColor.red;
									lightingShader->emissiveColor->green = emissiveColor.green;
									lightingShader->emissiveColor->blue = emissiveColor.blue;
								}
								lightingShader->emissiveMult = emissiveMult;
								lightingShader->SetFlags(Flag8::kVertexColors, a_geometry->HasVertexFlag(VertexFlags::kColors));

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


	void ResetShaderData(RE::NiAVObject* a_root, const std::vector<RE::NiStringsExtraData*>& a_vec)
	{
		for (auto& data : a_vec) {
			if (data) {
				auto textureset = RE::BSShaderTextureSet::Create();
				if (textureset) {
					for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
						if (data->value[i] && *data->value[i] != '\0') {
							textureset->SetTexturePath(i, data->value[i]);
						}
					}
					try {
						ShaderData shaderData;
						auto& [texture, flags, emissiveColor, emissiveMult, feature, featureOrig, diffuse] = shaderData;
						texture = textureset;
						flags = static_cast<Flag>(std::stoull(data->value[9]));
						emissiveColor = RE::NiColor(std::stoul(data->value[10]));
						emissiveMult = std::stof(data->value[11]);
						feature = static_cast<Feature>(std::stoul(data->value[12]));
						featureOrig = static_cast<Feature>(std::stoul(data->value[13]));
						diffuse = data->value[14];
						ResetShaderData_Impl(a_root, shaderData);
					} catch (...) {
					}
				}
				a_root->RemoveExtraData(data);
			}
		}
	}
}