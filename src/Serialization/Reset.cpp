#include "Serialization/Reset.h"


std::uint32_t constexpr const_hash(const char* input, std::uint32_t hash = 5381)
{
	return *input ?
				 const_hash(input + 1, hash * 33 + static_cast<std::uint32_t>(*input)) :
				 hash;
}


Reset::ResetData Reset::GetResetData(RE::NiAVObject* a_object)
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
		std::string name = extraData->name.c_str();
		if (name.empty()) {
			continue;
		}
		switch (const_hash(name.c_str())) {
		case const_hash("PO3_TOGGLE"):
			{
				std::get<kToggle>(resetData) = static_cast<RE::NiStringsExtraData*>(extraData);
			}
			break;
		case const_hash("PO3_SKINTINT"):
			{
				std::get<kTintSkin>(resetData) = static_cast<RE::NiIntegersExtraData*>(extraData);
			}
			break;
		case const_hash("PO3_HAIRTINT"):
			{
				std::get<kTintHair>(resetData) = static_cast<RE::NiIntegersExtraData*>(extraData);
			}
			break;
		case const_hash("PO3_ALPHA"):
			{
				std::get<kAlpha>(resetData) = static_cast<RE::NiFloatExtraData*>(extraData);
			}
			break;
		case const_hash("PO3_HEADPARTALPHA"):
			{
				std::get<kAlphaHDPT>(resetData) = static_cast<RE::NiIntegersExtraData*>(extraData);
			}
			break;
		case const_hash("PO3_FACETXST"):
			{
				std::get<kTXSTFace>(resetData) = static_cast<RE::NiStringsExtraData*>(extraData);
			}
			break;
		default:
			{
				if (name.find("PO3_TXST") != std::string::npos) {
					std::get<kTXST>(resetData).push_back(static_cast<RE::NiStringsExtraData*>(extraData));
				} else if (name.find("PO3_SKINTXST") != std::string::npos) {
					std::get<kTXSTSkin>(resetData).push_back(static_cast<RE::NiStringsExtraData*>(extraData));
				} else if (name.find("PO3_SHADER") != std::string::npos) {
					std::get<kShader>(resetData).push_back(static_cast<RE::NiStringsExtraData*>(extraData));
				}
			}
			break;
		}
	}
	return resetData;
}


void Reset::ResetToggleData(RE::NiAVObject* a_root, RE::NiStringsExtraData* a_data)
{
	auto task = SKSE::GetTaskInterface();
	task->AddTask([a_root, a_data]() {
		for (std::uint32_t i = 0; i < a_data->size; i++) {
			auto nodeName = RE::BSFixedString(a_data->value[i]);
			auto object = a_root->GetObjectByName(nodeName);
			if (object) {
				object->UpdateVisibility(false);
			}
		}
		a_root->RemoveExtraData(a_data);
	});
}


void Reset::ResetSkinTintData(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiIntegersExtraData* a_data)
{
	using HeadPartType = RE::BGSHeadPart::HeadPartType;

	auto actorbase = a_actor->GetActorBase();
	if (actorbase) {
		auto faceNode = a_actor->GetFaceNodeSkinned();
		auto facePart = actorbase->GetCurrentHeadPartByType(HeadPartType::kFace);
		if (faceNode && facePart) {
			auto task = SKSE::GetTaskInterface();
			task->AddTask([a_root, a_data, faceNode, facePart, actorbase]() {
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


void Reset::ResetHairTintData(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiIntegersExtraData* a_data)
{
	auto actorBase = a_actor->GetActorBase();
	if (actorBase) {
		auto headData = actorBase->headRelatedData;
		if (headData) {
			auto colorForm = headData->hairColor;
			if (colorForm) {
				auto task = SKSE::GetTaskInterface();
				task->AddTask([a_root, a_data, colorForm]() {
					a_root->UpdateHairColor(colorForm->color);
					a_root->RemoveExtraData(a_data);
				});
			}
		}
	}
}


void Reset::ResetAlphaData(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiFloatExtraData* a_data)
{
	for (auto& slot : fxSlots) {
		auto armor = a_actor->GetWornArmor(slot);
		if (armor) {
			a_actor->UnequipItem(0, armor, 1, nullptr);
		}
	}
	auto task = SKSE::GetTaskInterface();
	task->AddTask([a_actor, a_root, a_data]() {
		a_root->UpdateAlpha(1.0, RE::NiAVObject::ALPHA_MODE::kSkin);
		a_root->RemoveExtraData(a_data);
	});
}


void Reset::ResetHeadPartAlphaData(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiIntegersExtraData* a_data)
{
	using HeadPartType = RE::BGSHeadPart::HeadPartType;

	auto task = SKSE::GetTaskInterface();
	task->AddTask([a_actor, a_root, a_data]() {
		for (std::uint32_t i = 0; i < a_data->size; i++) {
			auto object = a_actor->GetHeadPartObject(static_cast<HeadPartType>(a_data->value[i]));
			if (object) {
				object->UpdateAlpha(1.0, RE::NiAVObject::ALPHA_MODE::kAll);
			}
		}
		a_root->RemoveExtraData(a_data);
	});
}


void ResetTextureSet(RE::NiAVObject* a_object, RE::BSShaderTextureSet* a_txst, RE::NiAVObject::ALPHA_MODE a_type, std::string_view a_folder)
{
	RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
		using ALPHA_MODE = RE::NiAVObject::ALPHA_MODE;
		using State = RE::BSGeometry::States;
		using Feature = RE::BSShaderMaterial::Feature;
		using Texture = RE::BSTextureSet::Texture;

		auto effect = a_geometry->properties[State::kEffect].get();
		if (effect) {
			auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect);
			if (lightingShader) {
				auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
				if (material) {
					if (a_type == ALPHA_MODE::kAll) {
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


void Reset::ResetFaceTXSTData(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiStringsExtraData* a_data)
{
	using ALPHA_MODE = RE::NiAVObject::ALPHA_MODE;
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
			auto task = SKSE::GetTaskInterface();
			task->AddTask([a_root, a_data, faceObject, textureset]() {
				ResetTextureSet(faceObject, textureset, ALPHA_MODE::kSkin, std::string());
				a_root->RemoveExtraData(a_data);
			});
		}
	}
}


void Reset::ResetTXSTData(RE::Actor* a_actor, RE::NiAVObject* a_root, const RE::BSFixedString& a_folderName, const std::vector<RE::NiStringsExtraData*>& a_vec)
{
	using ALPHA_MODE = RE::NiAVObject::ALPHA_MODE;
	using Texture = RE::BSTextureSet::Texture;

	std::string folder = a_folderName.c_str();
	RE::Util::SanitizeTexturePath(folder);

	for (auto& a_data : a_vec) {
		if (a_data) {
			auto task = SKSE::GetTaskInterface();
			task->AddTask([a_actor, a_root, folder, a_data]() {
				std::string armorID = a_data->value[a_data->size - 1];
				std::uint32_t formID = 0;
				if (!armorID.empty()) {
					try {
						formID = std::stoul(armorID);
					} catch (...) {
						SKSE::log::warn("ResetTXSTData - Unable to get armor ID!");
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
									ResetTextureSet(armorObject, textureset, ALPHA_MODE::kAll, folder);
								}
							}
						}
					}
				}
				a_root->RemoveExtraData(a_data);
			});
		}
	}
}


void Reset::ResetSkinTXSTData(RE::Actor* a_actor, RE::NiAVObject* a_root, const std::vector<RE::NiStringsExtraData*>& a_vec)
{
	using Slot = RE::BGSBipedObjectForm::FirstPersonFlag;
	using ALPHA_MODE = RE::NiAVObject::ALPHA_MODE;
	using Texture = RE::BSTextureSet::Texture;


	for (auto& data : a_vec) {
		if (data) {
			auto task = SKSE::GetTaskInterface();
			task->AddTask([a_actor, a_root, a_vec, data]() {
				std::string slotMaskstr = data->value[data->size - 1];
				Slot a_slot = Slot::kNone;
				if (!slotMaskstr.empty()) {
					try {
						a_slot = static_cast<Slot>(std::stoul(slotMaskstr));
					} catch (...) {
						SKSE::log::warn("ResetSkinTXSTData - Unable to get armor slot!");
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
								ResetTextureSet(armorObject, newTextureSet, ALPHA_MODE::kSkin, std::string());
							}
						}
					}
				}
				a_root->RemoveExtraData(data);
			});
		}
	}
}


void ResetShaderData_Impl(RE::NiAVObject* a_object, Reset::ShaderData data)
{
	RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
		using State = RE::BSGeometry::States;
		using Texture = RE::BSTextureSet::Texture;
		using Flag = RE::BSShaderProperty::EShaderPropertyFlag;
		using Flag8 = RE::BSShaderProperty::EShaderPropertyFlag8;
		using VertexFlags = RE::NiSkinPartition::Vertex::Flags;
		using SHADER = Reset::SHADER_TYPE;

		bool hasNormals = (RE::NiSkinPartition::GetVertexFlags(a_geometry->vertexDesc) & VertexFlags::kNormal) == VertexFlags::kNormal;
		if (!hasNormals) {
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

		auto effect = a_geometry->properties[State::kEffect].get();
		if (effect) {
			auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect);
			if (lightingShader) {
				auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
				if (material) {
					if (material->GetFeature() == std::get<SHADER::kFeature>(data)) {
						std::string currentDiffuse = material->textureSet->GetTexturePath(Texture::kDiffuse);
						RE::Util::SanitizeTexturePath(currentDiffuse);
						if (currentDiffuse.find(std::get<SHADER::kDiffuse>(data)) == std::string::npos) {
							return RE::BSVisit::BSVisitControl::kContinue;
						}

						auto newMaterial = RE::BSLightingShaderMaterialBase::CreateMaterial(std::get<SHADER::kFeatureOrig>(data));
						if (newMaterial) {
							newMaterial->CopyBaseMembers(material);
							newMaterial->ClearTextures();
							newMaterial->OnLoadTextureSet(0, std::get<SHADER::kTexture>(data));
							lightingShader->SetMaterial(newMaterial, 1);
							lightingShader->InitializeGeometry(a_geometry);
							lightingShader->InitializeShader(a_geometry);

							lightingShader->flags = std::get<SHADER::kFlags>(data);
							lightingShader->lastRenderPassState = std::numeric_limits<std::int32_t>::max();
							if ((lightingShader->flags & Flag::kOwnEmit) == Flag::kOwnEmit) {
								auto emissive = std::get<SHADER::kEmissive>(data);
								lightingShader->emissiveColor->red = emissive.red;
								lightingShader->emissiveColor->green = emissive.green;
								lightingShader->emissiveColor->blue = emissive.blue;
							}
							lightingShader->emissiveMult = std::get<SHADER::kEmissiveMult>(data);

							bool hasVertexColors = (RE::NiSkinPartition::GetVertexFlags(a_geometry->vertexDesc) & VertexFlags::kColors) == VertexFlags::kColors;
							lightingShader->SetFlags(Flag8::kVertexColors, hasVertexColors);

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


void Reset::ResetShaderData(RE::NiAVObject* a_root, const std::vector<RE::NiStringsExtraData*>& a_vec)
{
	using Flag = RE::BSShaderProperty::EShaderPropertyFlag;
	using Feature = RE::BSShaderMaterial::Feature;
	using Texture = RE::BSTextureSet::Texture;
	using SHADER = Reset::SHADER_TYPE;

	auto task = SKSE::GetTaskInterface();
	task->AddTask([a_root, a_vec]() {
		for (auto& a_data : a_vec) {
			if (a_data) {
				auto textureset = RE::BSShaderTextureSet::Create();
				if (textureset) {
					for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
						if (a_data->value[i] && *a_data->value[i] != '\0') {
							textureset->SetTexturePath(i, a_data->value[i]);
						}
					}
					try {
						ShaderData data;
						std::get<SHADER::kTexture>(data) = textureset;
						std::get<SHADER::kFlags>(data) = static_cast<Flag>(std::stoull(a_data->value[9]));
						std::get<SHADER::kEmissive>(data) = RE::NiColor(std::stoul(a_data->value[10]));
						std::get<SHADER::kEmissiveMult>(data) = std::stof(a_data->value[11]);
						std::get<SHADER::kFeature>(data) = static_cast<Feature>(std::stoul(a_data->value[12]));
						std::get<SHADER::kFeatureOrig>(data) = static_cast<Feature>(std::stoul(a_data->value[13]));
						std::get<SHADER::kDiffuse>(data) = a_data->value[14];
						ResetShaderData_Impl(a_root, data);
					} catch (...) {
						SKSE::log::warn("ResetShaderData - unable to get original shader params!");
					}
				}
			}
			a_root->RemoveExtraData(a_data);
		}
	});
}