#include "Util/GraphicsReset.h"


namespace RESET
{
	auto constexpr const_hash(const char* input, const std::uint32_t hash = 5381) -> std::uint32_t
	{
		return *input ?
                   const_hash(input + 1, hash * 33 + static_cast<std::uint32_t>(*input)) :
                   hash;
	}


	auto GetData(RE::NiAVObject* a_object) -> std::pair<bool, ResetData>
	{
		ResetData resetData;

		if (!a_object->extra || a_object->extraDataSize == 0) {
			return { false, resetData };
		}

		bool success = false;

		stl::span<RE::NiExtraData*> span(a_object->extra, a_object->extraDataSize);
		for (auto& extraData : span) {
			if (extraData) {
				if (std::string name(extraData->name); !name.empty()) {
					auto& [toggle, tintSkin, tintHair, alphaSkin, TXSTFace, alphaHDPT, TXST, TXSTSkin, shader] = resetData;

					switch (const_hash(name.c_str())) {
					case const_hash("PO3_TOGGLE"):
						{
							success = true;
							toggle = static_cast<RE::NiStringsExtraData*>(extraData);
						}
						break;
					case const_hash("PO3_SKINTINT"):
						{
							success = true;
							tintSkin = static_cast<RE::NiIntegerExtraData*>(extraData);
						}
						break;
					case const_hash("PO3_HAIRTINT"):
						{
							success = true;
							tintHair = static_cast<RE::NiIntegerExtraData*>(extraData);
						}
						break;
					case const_hash("PO3_SKINALPHA"):
						{
							success = true;
							alphaSkin = static_cast<RE::NiBooleanExtraData*>(extraData);
						}
						break;
					case const_hash("PO3_FACETXST"):
						{
							success = true;
							TXSTFace = static_cast<RE::NiStringsExtraData*>(extraData);
						}
						break;
					default:
						{
							if (name.find("PO3_HEADPART"sv) != std::string::npos) {
								success = true;
								alphaHDPT.emplace_back(static_cast<RE::NiIntegerExtraData*>(extraData));

							} else if (name.find("PO3_TXST"sv) != std::string::npos) {
								success = true;
								TXST.emplace_back(static_cast<RE::NiStringsExtraData*>(extraData));

							} else if (name.find("PO3_SKINTXST"sv) != std::string::npos) {
								success = true;
								TXSTSkin.emplace_back(static_cast<RE::NiStringsExtraData*>(extraData));

							} else if (name.find("PO3_SHADER |"sv) != std::string::npos) {
								success = true;
								shader.emplace_back(static_cast<RE::NiStringsExtraData*>(extraData));
							}
						}
						break;
					}
				}
			}
		}


		return { success, resetData };
	}


	void Toggle(RE::NiAVObject* a_root, RE::NiStringsExtraData* a_data)
	{
		if (a_data && a_data->value) {
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
	}


	void SkinTint(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiIntegerExtraData* a_data)
	{
		if (a_data) {
			auto actorbase = a_actor->GetActorBase();
			if (actorbase) {
				const auto faceNode = a_actor->GetFaceNodeSkinned();
				const auto facePart = actorbase->GetCurrentHeadPartByType(HeadPartType::kFace);

				if (faceNode && facePart) {
					if (auto faceGen = RE::FaceGen::GetSingleton(); faceGen) {
						faceGen->RegenerateHead(faceNode, facePart, actorbase);
					}
					a_root->UpdateBodyTint(actorbase->bodyTintColor);
					a_root->RemoveExtraData(a_data);
				}
			}
		}
	}


	void HairTint(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiIntegerExtraData* a_data)
	{
		if (a_data) {
			if (const auto actorBase = a_actor->GetActorBase(); actorBase) {
				if (const auto headData = actorBase->headRelatedData; headData) {
					if (const auto colorForm = headData->hairColor; colorForm) {
						a_root->UpdateHairColor(colorForm->color);
						a_root->RemoveExtraData(a_data);
					}
				}
			}
		}
	}


	void SkinAlpha(RE::NiAVObject* a_root, RE::NiBooleanExtraData* a_data)
	{
		if (a_data) {
			a_root->UpdateMaterialAlpha(1.0f, true);
			a_root->RemoveExtraData(a_data);
		}
	}


	void HeadPartAlpha(RE::Actor* a_actor, RE::NiAVObject* a_root, const std::vector<RE::NiIntegerExtraData*>& a_data)
	{
		if (!a_data.empty()) {
			for (auto& data : a_data) {
				if (data) {
					if (auto object = a_actor->GetHeadPartObject(static_cast<HeadPartType>(data->value)); object) {
						object->UpdateMaterialAlpha(1.0f, false);
						a_root->RemoveExtraData(data);
					}
				}
			}
		}
	}


	void ResetTextureSet(RE::NiAVObject* a_object, RE::BSShaderTextureSet* a_txst, bool a_onlySkin, std::string_view a_folder)
	{
		RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
			auto effect = a_geometry->properties[RE::BSGeometry::States::kEffect];
			auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect.get());
			if (lightingShader) {
				auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
				if (material) {
					if (!a_onlySkin) {
						auto textureSet = material->textureSet.get();
						if (!a_folder.empty() && textureSet) {
							std::string sourcePath(textureSet->GetTexturePath(Texture::kDiffuse));
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

					if (auto newMaterial = static_cast<RE::BSLightingShaderMaterialBase*>(material->Create()); newMaterial) {
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
			return RE::BSVisit::BSVisitControl::kContinue;
		});
	}


	void FaceTXST(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiStringsExtraData* a_data)
	{
		if (a_data && a_data->value) {
			if (auto textureset = RE::BSShaderTextureSet::Create(); textureset) {
				for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
					if (a_data->value[i] && *a_data->value[i] != '\0') {
						textureset->SetTexturePath(i, a_data->value[i]);
					}
				}
				if (const auto faceObject = a_actor->GetHeadPartObject(HeadPartType::kFace); faceObject) {
					ResetTextureSet(faceObject, textureset, true, std::string());
					a_root->RemoveExtraData(a_data);
				}
			}
		}
	}


	void TXST(RE::Actor* a_actor, RE::NiAVObject* a_root, const RE::BSFixedString& a_folderName, const std::vector<RE::NiStringsExtraData*>& a_vec)
	{
		using namespace SKSE::UTIL::STRING;

		if (a_vec.empty()) {
			return;
		}

		std::string folder(a_folderName);
		RE::Util::SanitizeTexturePath(folder);

		for (auto& a_data : a_vec) {
			if (a_data && a_data->value) {
				std::string armorID = a_data->value[a_data->size - 1];
				RE::FormID formID = 0;
				if (!armorID.empty()) {
					try {
						formID = to_num<RE::FormID>(armorID);
					} catch (...) {
						logger::warn("ResetTXSTData - Unable to get armor ID!"sv);
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
					if (const auto armorAddon = armor->GetArmorAddon(a_actor->GetRace()); armorAddon) {
						const auto armorObject = a_actor->VisitArmorAddon(armor, armorAddon);
						if (armorObject) {
							if (auto textureset = RE::BSShaderTextureSet::Create(); textureset) {
								for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
									if (a_data->value[i] && *a_data->value[i] != '\0') {
										textureset->SetTexturePath(i, a_data->value[i]);
									}
								}
								ResetTextureSet(armorObject, textureset, false, folder);
								a_root->RemoveExtraData(a_data);
							}
						}
					}
				}
			}
		}
	}


	void SkinTXST(RE::Actor* a_actor, RE::NiAVObject* a_root, const std::vector<RE::NiStringsExtraData*>& a_vec)
	{
		using namespace SKSE::UTIL;

		if (a_vec.empty()) {
			return;
		}

		for (auto& data : a_vec) {
			if (data && data->value) {
				std::string slotMaskstr(data->value[data->size - 1]);
				auto a_slot = Slot::kNone;
				if (!slotMaskstr.empty()) {
					try {
						a_slot = STRING::to_num<Slot>(slotMaskstr);
					} catch (...) {
					}
				}
				if (auto skinarmor = a_actor->GetSkin(a_slot); skinarmor) {
					const auto skinAddon = skinarmor->GetArmorAddonByMask(a_actor->GetRace(), a_slot);
					if (skinAddon) {
						const auto armorObject = a_actor->VisitArmorAddon(skinarmor, skinAddon);
						if (armorObject) {
							auto newTextureSet = RE::BSShaderTextureSet::Create();
							if (newTextureSet) {
								for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
									if (data->value[i] && *data->value[i] != '\0') {
										newTextureSet->SetTexturePath(i, data->value[i]);
									}
								}
								ResetTextureSet(armorObject, newTextureSet, true, std::string());
								a_root->RemoveExtraData(data);
							}
						}
					}
				}
			}
		}
	}


	auto GetOriginalShaders(RE::NiStringsExtraData* a_data) -> std::pair<bool, ShaderData>
	{
		using namespace SKSE::UTIL::STRING;

		ShaderData shaderData;
		bool result = true;

		if (a_data->value) {
			auto& [textureSet, feature, flags, emissiveColor, emissiveMult] = shaderData;

			textureSet = RE::BSShaderTextureSet::Create();
			if (textureSet) {
				for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
					if (a_data->value[i] && *a_data->value[i] != '\0') {
						textureSet->SetTexturePath(i, a_data->value[i]);
					}
				}
				try {
					feature = static_cast<Feature>(std::stoul(a_data->value[9]));
					flags = std::stoull(a_data->value[10]);
					emissiveColor = RE::NiColor(std::stoul(a_data->value[11]));
					emissiveMult = to_num<float>(a_data->value[12]);
				} catch (...) {
					result = false;
				}
			} else {
				result = false;
			}
		}

		return { result, shaderData };
	}


	void ResetShaderData_Impl(RE::NiAVObject* a_object, std::vector<RE::BSFixedString>& a_geometries)
	{
		RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
			if (std::find(a_geometries.begin(), a_geometries.end(), a_geometry->name) == a_geometries.end()) {
				return RE::BSVisit::BSVisitControl::kContinue;
			}

			auto effect = a_geometry->properties[RE::BSGeometry::States::kEffect];
			auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect.get());
			if (lightingShader) {
				auto originalData = lightingShader->GetExtraData<RE::NiStringsExtraData>("PO3_ORIGINALSHADER"sv);
				if (!originalData) {
					return RE::BSVisit::BSVisitControl::kContinue;
				}

				auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
				if (material) {
					bool result;
					ShaderData shaderData;

					std::tie(result, shaderData) = GetOriginalShaders(originalData);
					if (!result) {
						logger::warn("unable to get original shader values for {}", a_geometry->name);
						return RE::BSVisit::BSVisitControl::kContinue;
					}

					auto& [textureSet, feature, flags, emissiveColor, emissiveMult] = shaderData;

					if (auto newMaterial = RE::BSLightingShaderMaterialBase::CreateMaterial(feature); newMaterial) {
						newMaterial->CopyBaseMembers(material);
						newMaterial->ClearTextures();
						newMaterial->OnLoadTextureSet(0, textureSet);

						lightingShader->flags = static_cast<RE::BSShaderProperty::EShaderPropertyFlag>(flags);
						lightingShader->lastRenderPassState = std::numeric_limits<std::int32_t>::max();
						if (lightingShader->flags.all(Flag::kOwnEmit)) {
							if (!lightingShader->emissiveColor) {
								lightingShader->emissiveColor = new RE::NiColor();
							}
							lightingShader->emissiveColor->red = emissiveColor.red;
							lightingShader->emissiveColor->green = emissiveColor.green;
							lightingShader->emissiveColor->blue = emissiveColor.blue;
						}
						lightingShader->emissiveMult = emissiveMult;

						lightingShader->SetMaterial(newMaterial, true);
						//lightingShader->InitializeGeometry(a_geometry);
						lightingShader->InitializeShader(a_geometry);

						newMaterial->~BSLightingShaderMaterialBase();
						RE::free(newMaterial);

						lightingShader->RemoveExtraData(originalData);
						a_geometry->RemoveExtraData(originalData);
					}
				}
			}

			return RE::BSVisit::BSVisitControl::kContinue;
		});
	}


	void MaterialShader(RE::NiAVObject* a_root, const std::vector<RE::NiStringsExtraData*>& a_vec)
	{
		using namespace SKSE::UTIL::STRING;

		if (a_vec.empty()) {
			return;
		}

		for (auto& data : a_vec) {
			if (data && data->value) {
				std::vector<RE::BSFixedString> dataVec({ data->value, data->value + data->size });
				ResetShaderData_Impl(a_root, dataVec);

				a_root->RemoveExtraData(data);
			}
		}
	}
}