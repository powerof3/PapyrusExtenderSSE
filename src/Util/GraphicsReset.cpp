#include "Util/GraphicsReset.h"


namespace GraphicsReset
{
	auto constexpr const_hash(const char* input, const std::uint32_t hash = 5381) -> std::uint32_t
	{
		return *input ?
                   const_hash(input + 1, hash * 33 + static_cast<std::uint32_t>(*input)) :
                   hash;
	}


	auto GetResetData(RE::NiAVObject* a_object) -> ResetData
	{
		ResetData resetData;

		if (!a_object->extra || a_object->extraDataSize == 0) {
			return resetData;
		}

		stl::span<RE::NiExtraData*> span(a_object->extra, a_object->extraDataSize);
		for (auto& extraData : span) {
			if (extraData) {
				if (std::string name(extraData->name); !name.empty()) {
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
			}
		}

		return resetData;
	}


	void ResetToggleData(RE::NiAVObject* a_root, RE::NiStringsExtraData* a_data)
	{
		stl::span<char*> span(a_data->value, a_data->size);
		for (auto& string : span) {
			if (string && *string != '\0') {
				if (auto object = a_root->GetObjectByName(string); object) {
					object->ToggleNode(false);
				}
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
				task->AddTask([a_root, actorbase, faceNode, facePart]() {
					if (auto faceGen = RE::FaceGen::GetSingleton(); faceGen) {
						faceGen->RegenerateHead(faceNode, facePart, actorbase);
					}
					a_root->UpdateBodyTint(actorbase->bodyTintColor);
				});
			}
		}
		a_root->RemoveExtraData(a_data);
	}


	void ResetHairTintData(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiIntegersExtraData* a_data)
	{
		if (const auto actorBase = a_actor->GetActorBase(); actorBase) {
			if (const auto headData = actorBase->headRelatedData; headData) {
				if (const auto colorForm = headData->hairColor; colorForm) {
					a_root->UpdateHairColor(colorForm->color);
				}
			}
		}
		a_root->RemoveExtraData(a_data);
	}


	void ResetAlphaData(RE::NiAVObject* a_root, RE::NiFloatExtraData* a_data)
	{
		a_root->UpdateMaterialAlpha(1.0, true);
		a_root->RemoveExtraData(a_data);
	}


	void ResetHeadPartAlphaData(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiIntegersExtraData* a_data)
	{
		using HeadPartType = RE::BGSHeadPart::HeadPartType;

		stl::span<std::int32_t> span(a_data->value, a_data->size);
		for (auto& integer : span) {
			if (auto object = a_actor->GetHeadPartObject(static_cast<HeadPartType>(integer)); object) {
				object->UpdateMaterialAlpha(1.0f, false);
			}
		}
		a_root->RemoveExtraData(a_data);
	}


	void ResetTextureSet(RE::NiAVObject* a_object, RE::BSShaderTextureSet* a_txst, bool a_onlySkin, std::string_view a_folder)
	{
		RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
			auto effect = a_geometry->properties[State::kEffect].get();
			if (effect) {
				auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect);
				if (lightingShader) {
					auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
					if (material) {
						if (!a_onlySkin) {
							if (!a_folder.empty() && material->textureSet.get()) {
								std::string sourcePath(material->textureSet->GetTexturePath(Texture::kDiffuse));
								RE::Util::SanitizeTexturePath(sourcePath);
								if (sourcePath.find(a_folder) == std::string::npos) {
									return RE::BSVisit::BSVisitControl::kContinue;
								}
							}
						} else {
							const auto type = material->GetFeature();
							if (type != Feature::kFaceGenRGBTint && type != Feature::kFaceGen) {
								return RE::BSVisit::BSVisitControl::kContinue;
							}
						}

						auto newMaterial = static_cast<RE::BSLightingShaderMaterialBase*>(material->Create());
						if (newMaterial) {
							newMaterial->CopyMembers(material);
							newMaterial->ClearTextures();
							newMaterial->OnLoadTextureSet(0, a_txst);
							lightingShader->SetMaterial(newMaterial, true);
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

		if (auto textureset = RE::BSShaderTextureSet::Create(); textureset) {
			for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
				if (a_data->value[i] && *a_data->value[i] != '\0') {
					textureset->SetTexturePath(i, a_data->value[i]);
				}
			}
			if (const auto faceObject = a_actor->GetHeadPartObject(HeadPartType::kFace); faceObject) {
				ResetTextureSet(faceObject, textureset, true, std::string());
			}
		}
		a_root->RemoveExtraData(a_data);
	}


	void ResetTXSTData(RE::Actor* a_actor, RE::NiAVObject* a_root, const RE::BSFixedString& a_folderName, const std::vector<RE::NiStringsExtraData*>& a_vec)
	{
		std::string folder(a_folderName);
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
					const auto actorBase = a_actor->GetActorBase();
					if (actorBase) {
						armor = actorBase->GetSkin();
					}
				}
				if (armor) {
					if (auto textureset = RE::BSShaderTextureSet::Create(); textureset) {
						for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
							if (a_data->value[i] && *a_data->value[i] != '\0') {
								textureset->SetTexturePath(i, a_data->value[i]);
							}
						}
						if (const auto armorAddon = armor->GetArmorAddon(a_actor->GetRace()); armorAddon) {
							const auto armorObject = a_actor->VisitArmorAddon(armor, armorAddon);
							if (armorObject) {
								ResetTextureSet(armorObject, textureset, false, folder);
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
		for (auto& data : a_vec) {
			if (data) {
				std::string slotMaskstr(data->value[data->size - 1]);
				Slot a_slot = Slot::kNone;
				if (!slotMaskstr.empty()) {
					try {
						a_slot = static_cast<Slot>(std::stoul(slotMaskstr));
					} catch (...) {
					}
				}
				if (auto skinarmor = a_actor->GetSkin(a_slot); skinarmor) {
					const auto foundAddon = skinarmor->GetArmorAddonByMask(a_actor->GetRace(), a_slot);
				    if (foundAddon) {
						const auto armorObject = a_actor->VisitArmorAddon(skinarmor, foundAddon);
						if (armorObject) {
							if (auto newTextureSet = RE::BSShaderTextureSet::Create(); newTextureSet) {
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
			if (auto parent = a_geometry->parent; parent && parent->AsFadeNode()) {
				return RE::BSVisit::BSVisitControl::kContinue;
			}
			if (const auto alpha = a_geometry->properties[State::kProperty].get(); alpha) {
				return RE::BSVisit::BSVisitControl::kContinue;
			}

			auto& [texture, flags, emissiveColor, emissiveMult, feature, featureOrig, diffuse] = data;

			auto effect = a_geometry->properties[State::kEffect].get();
			if (effect) {
				auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect);
				if (lightingShader) {
					auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
					if (material && material->GetFeature() == feature && material->textureSet.get()) {
						std::string currentDiffuse(material->textureSet->GetTexturePath(Texture::kDiffuse));
						RE::Util::SanitizeTexturePath(currentDiffuse);
						if (currentDiffuse.find(diffuse) == std::string::npos) {
							return RE::BSVisit::BSVisitControl::kContinue;
						}

						auto newMaterial = RE::BSLightingShaderMaterialBase::CreateMaterial(featureOrig);
						if (newMaterial) {
							newMaterial->CopyBaseMembers(material);
							newMaterial->ClearTextures();
							newMaterial->OnLoadTextureSet(0, texture);
							lightingShader->SetMaterial(newMaterial, true);
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
			return RE::BSVisit::BSVisitControl::kContinue;
		});
	}


	void ResetShaderData(RE::NiAVObject* a_root, const std::vector<RE::NiStringsExtraData*>& a_vec)
	{
		for (auto& data : a_vec) {
			if (data) {
				if (auto textureset = RE::BSShaderTextureSet::Create(); textureset) {
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