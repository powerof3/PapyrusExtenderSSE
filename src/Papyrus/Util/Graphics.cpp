#include "Papyrus/Util/Graphics.h"

namespace GRAPHICS
{
	namespace MATERIAL
	{
		RE::MATERIAL_ID get_material(const std::string& a_mat)
		{
			const auto it = materialMap.find(a_mat);
			return it != materialMap.end() ? it->second : RE::MATERIAL_ID::kNone;
		}

		void for_each_material_type(const RE::NiPointer<RE::bhkWorldObject>& a_body, std::function<void(RE::MATERIAL_ID&)> a_visitor)
		{
			if (!a_body) {
				return;
			}

			const auto hkpBody = static_cast<RE::hkpWorldObject*>(a_body->referencedObject.get());

			if (const auto hkpShape = hkpBody ? hkpBody->GetShape() : nullptr) {
				if (hkpShape->type == RE::hkpShapeType::kMOPP) {
					const auto mopp = static_cast<const RE::hkpMoppBvTreeShape*>(hkpShape);
					const auto childShape = mopp ? mopp->child.childShape : nullptr;
					const auto compressedShape = childShape ? netimmerse_cast<RE::bhkCompressedMeshShape*>(childShape->userData) : nullptr;
					const auto shapeData = compressedShape ? compressedShape->data.get() : nullptr;

					if (shapeData) {
						for (auto& meshMaterial : shapeData->meshMaterials) {
							a_visitor(meshMaterial.materialID);
						}
					}
				} else if (const auto bhkShape = hkpShape->userData; bhkShape) {
					a_visitor(bhkShape->materialID);
				}
			}
		}
	}

	namespace RESET
	{
		void stop_all_skin_shaders(RE::TESObjectREFR* a_ref)
		{
			using Flags = RE::EffectShaderData::Flags;

			if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
				const auto handle = a_ref->CreateRefHandle();
				processLists->ForEachShaderEffect([&](RE::ShaderReferenceEffect* a_shaderEffect) {
					if (a_shaderEffect->target == handle) {
						if (const auto effectData = a_shaderEffect->effectData; effectData &&
																				effectData->data.flags.all(Flags::kSkinOnly) &&
																				!effectData->holesTexture.textureName.empty()) {
							a_shaderEffect->finished = true;
						}
					}
					return RE::BSContainer::ForEachResult::kContinue;
				});
			}
		}

		std::pair<bool, ResetData> get_data(RE::NiAVObject* a_object)
		{
			ResetData resetData;
			bool      success = false;

			if (!a_object->extra || a_object->extraDataSize == 0) {
				return { success, resetData };
			}

			auto& [toggle, tintSkin, tintHair, alphaSkin, TXSTFace, alphaHDPT, TXST, TXSTSkin, shader] = resetData;

			std::span<RE::NiExtraData*> span(a_object->extra, a_object->extraDataSize);
			for (const auto& extraData : span) {
				if (!extraData) {
					continue;
				}
				if (const auto name = extraData->GetName(); !name.empty()) {
					switch (string::const_hash(name)) {
					case string::const_hash(EXTRA::TOGGLE):
						{
							toggle = static_cast<RE::NiStringsExtraData*>(extraData);
							success = true;
						}
						break;
					case string::const_hash(EXTRA::SKIN_TINT):
						{
							tintSkin = static_cast<RE::NiIntegerExtraData*>(extraData);
							success = true;
						}
						break;
					case string::const_hash(EXTRA::HAIR_TINT):
						{
							tintHair = static_cast<RE::NiIntegerExtraData*>(extraData);
							success = true;
						}
						break;
					case string::const_hash(EXTRA::SKIN_ALPHA):
						{
							alphaSkin = static_cast<RE::NiBooleanExtraData*>(extraData);
							success = true;
						}
						break;
					case string::const_hash(EXTRA::FACE_TXST):
						{
							TXSTFace = static_cast<RE::NiStringsExtraData*>(extraData);
							success = true;
						}
						break;
					default:
						if (string::icontains(name, EXTRA::HEADPART)) {
							alphaHDPT.emplace_back(static_cast<RE::NiIntegerExtraData*>(extraData));
							success = true;

						} else if (string::icontains(name, EXTRA::TXST)) {
							TXST.emplace_back(static_cast<RE::NiStringsExtraData*>(extraData));
							success = true;

						} else if (string::icontains(name, EXTRA::SKIN_TXST)) {
							TXSTSkin.emplace_back(static_cast<RE::NiStringsExtraData*>(extraData));
							success = true;

						} else if (string::icontains(name, EXTRA::SHADER)) {
							shader.emplace_back(static_cast<RE::NiStringsExtraData*>(extraData));
							success = true;
						}
						break;
					}
				}
			}

			return { success, resetData };
		}

		void Toggle(RE::NiAVObject* a_root, RE::NiStringsExtraData* a_data)
		{
			if (a_data && a_data->value && a_data->size > 0) {
				std::span<char*> span(a_data->value, a_data->size);
				for (const auto& string : span) {
					if (!string::is_empty(string)) {
						if (const auto object = a_root->GetObjectByName(string); object) {
							object->CullNode(false);
						}
					}
				}
				a_root->RemoveExtraData(a_data->GetName());
			}
		}

		void SkinTint(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiIntegerExtraData* a_data)
		{
			if (a_data) {
				const auto actorbase = a_actor->GetActorBase();
				const auto facePart = actorbase ? actorbase->GetCurrentHeadPartByType(HeadPartType::kFace) : nullptr;
				const auto faceNode = a_actor->GetFaceNodeSkinned();

				if (faceNode && facePart) {
					if (const auto faceGen = RE::BSFaceGenManager::GetSingleton(); faceGen) {
						faceGen->PrepareHeadPartForShaders(faceNode, facePart, actorbase);
					}
					a_root->UpdateBodyTint(actorbase->bodyTintColor);
					a_root->RemoveExtraData(a_data->GetName());
				}
			}
		}

		void HairTint(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiIntegerExtraData* a_data)
		{
			if (a_data) {
				const auto actorBase = a_actor->GetActorBase();
				const auto headData = actorBase ? actorBase->headRelatedData : nullptr;
				const auto colorForm = headData ? headData->hairColor : nullptr;

				if (colorForm) {
					a_root->UpdateHairColor(colorForm->color);

					if (const auto& biped = a_actor->GetCurrentBiped(); biped) {
						for (auto& slot : ACTOR::headSlots) {
							const auto node = biped->objects[slot].partClone;
							if (node && node->HasShaderType(RE::BSShaderMaterial::Feature::kHairTint)) {
								node->UpdateHairColor(colorForm->color);
							}
						}
					}

					a_root->RemoveExtraData(a_data->GetName());
				}
			}
		}

		void SkinAlpha(RE::NiAVObject* a_root, RE::NiBooleanExtraData* a_data)
		{
			if (a_data) {
				a_root->UpdateMaterialAlpha(1.0f, true);
				a_root->RemoveExtraData(a_data->GetName());
			}
		}

		void HeadPartAlpha(RE::Actor* a_actor, RE::NiAVObject* a_root, const std::vector<RE::NiIntegerExtraData*>& a_data)
		{
			for (auto& data : a_data) {
				if (data) {
					if (const auto object = a_actor->GetHeadPartObject(static_cast<HeadPartType>(data->value)); object) {
						object->UpdateMaterialAlpha(1.0f, false);
						a_root->RemoveExtraData(data->GetName());
					}
				}
			}
		}

		void reset_textureset(RE::NiAVObject* a_object, RE::BSShaderTextureSet* a_txst, bool a_doOnlySkin, const std::string& a_folder)
		{
			using Texture = RE::BSTextureSet::Texture;

			RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
				const auto effect = a_geometry->properties[States::kEffect];
				const auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect.get());
				if (lightingShader) {
					const auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
					if (material) {
						if (!a_doOnlySkin) {
							if (const auto textureSet = material->textureSet; textureSet && !a_folder.empty()) {
								std::string sourcePath{ textureSet->GetTexturePath(Texture::kDiffuse) };
								if (TEXTURE::sanitize_path(sourcePath); sourcePath.find(a_folder) == std::string::npos) {
									return RE::BSVisit::BSVisitControl::kContinue;
								}
							}
						} else {
							if (const auto feature = material->GetFeature(); feature != Feature::kFaceGenRGBTint && feature != Feature::kFaceGen) {
								return RE::BSVisit::BSVisitControl::kContinue;
							}
						}

						if (const auto newMaterial = static_cast<RE::BSLightingShaderMaterialBase*>(material->Create()); newMaterial) {
							newMaterial->CopyMembers(material);

							newMaterial->ClearTextures();
							newMaterial->OnLoadTextureSet(0, a_txst);

							lightingShader->SetMaterial(newMaterial, true);

							lightingShader->SetupGeometry(a_geometry);
							lightingShader->FinishSetupGeometry(a_geometry);

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
				const auto textureset = TEXTURE::create_textureset(a_data->value);
				const auto faceObject = a_actor->GetHeadPartObject(HeadPartType::kFace);
				if (textureset && faceObject) {
					reset_textureset(faceObject, textureset, true, std::string());
					a_root->RemoveExtraData(a_data->GetName());
				}
			}
		}

		void ArmorTXST(RE::Actor* a_actor, RE::NiAVObject* a_root, const RE::BSFixedString& a_folderName, const std::vector<RE::NiStringsExtraData*>& a_vec)
		{
			if (a_vec.empty()) {
				return;
			}

			std::string folder{ a_folderName };
			TEXTURE::sanitize_path(folder);

			for (auto& data : a_vec) {
				if (data && data->value && data->size > 0) {
					RE::FormID formID = 0;
					if (std::string armorID{ data->value[data->size - 1] }; !armorID.empty()) {
						try {
							formID = string::to_num<RE::FormID>(armorID, true);
						} catch (...) {
							continue;
						}
					}
					auto armor = a_actor->GetWornArmor(formID);
					if (!armor) {
						armor = a_actor->GetSkin();
					}
					if (armor) {
						const auto textureset = TEXTURE::create_textureset(data->value);
						const auto arma = armor->GetArmorAddon(a_actor->GetRace());
						if (textureset && arma) {
							a_actor->VisitArmorAddon(armor, arma, [&](bool, RE::NiAVObject& a_obj) -> bool {
								reset_textureset(&a_obj, textureset, false, folder);
								return true;
							});
							a_root->RemoveExtraData(data->GetName());
						}
					}
				}
			}
		}

		void SkinTXST(RE::Actor* a_actor, RE::NiAVObject* a_root, const std::vector<RE::NiStringsExtraData*>& a_vec)
		{
			using Slot = RE::BGSBipedObjectForm::BipedObjectSlot;

			if (a_vec.empty()) {
				return;
			}

			for (auto& data : a_vec) {
				if (data && data->value && data->size > 0) {
					auto slot = Slot::kNone;
					if (std::string slotMaskstr{ data->value[data->size - 1] }; !slotMaskstr.empty()) {
						try {
							slot = string::to_num<Slot>(slotMaskstr);
						} catch (...) {
							continue;
						}
					}
					if (const auto skinArmor = a_actor->GetSkin(slot); skinArmor) {
						const auto textureset = TEXTURE::create_textureset(data->value);
						const auto skinArma = skinArmor->GetArmorAddonByMask(a_actor->GetRace(), slot);
						if (textureset && skinArma) {
							a_actor->VisitArmorAddon(skinArmor, skinArma, [&](bool, RE::NiAVObject& a_obj) -> bool {
								reset_textureset(&a_obj, textureset, true, std::string());
								return true;
							});
							a_root->RemoveExtraData(data->GetName());
						}
					}
				}
			}
		}

		std::pair<bool, ShaderData> get_original_shaders(RE::NiStringsExtraData* a_data)
		{
			ShaderData shaderData;
			bool       result = true;

			if (a_data && a_data->value && a_data->size > 0) {
				auto& [textureSet, feature, flags, emissiveColor, emissiveMult] = shaderData;
				if (const auto new_txst = TEXTURE::create_textureset(a_data->value); new_txst) {
					try {
						textureSet = new_txst;
						feature = string::to_num<Feature>(a_data->value[9]);
						flags = string::to_num<std::uint64_t>(a_data->value[10]);
						emissiveColor = RE::NiColor(
							string::to_num<std::uint32_t>(a_data->value[11]));
						emissiveMult = string::to_num<float>(a_data->value[12]);
					} catch (...) {
						result = false;
					}
				} else {
					result = false;
				}
			}

			return { result, shaderData };
		}

		void reset_shaderdata(RE::NiAVObject* a_object, std::vector<RE::BSFixedString>& a_geometries)
		{
			using Flag = RE::BSShaderProperty::EShaderPropertyFlag;

			RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
				if (std::ranges::find(a_geometries, a_geometry->name) == a_geometries.end()) {
					return RE::BSVisit::BSVisitControl::kContinue;
				}

				const auto effect = a_geometry->properties[States::kEffect];
				const auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect.get());

				if (lightingShader) {
					const auto originalData = lightingShader->GetExtraData<RE::NiStringsExtraData>(EXTRA::ORIG_SHADER);
					if (!originalData) {
						return RE::BSVisit::BSVisitControl::kContinue;
					}

					if (const auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material)) {
						auto [result, shaderData] = get_original_shaders(originalData);
						if (!result) {
							logger::warn("unable to get original shader values for {}", a_geometry->name.c_str());
							return RE::BSVisit::BSVisitControl::kContinue;
						}

						auto& [textureSet, feature, flags, emissiveColor, emissiveMult] = shaderData;

						if (const auto newMaterial = RE::BSLightingShaderMaterialBase::CreateMaterial(feature); newMaterial) {
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
							lightingShader->SetupGeometry(a_geometry);
							lightingShader->FinishSetupGeometry(a_geometry);

							newMaterial->~BSLightingShaderMaterialBase();
							RE::free(newMaterial);

							lightingShader->RemoveExtraData(originalData->GetName());
							a_geometry->RemoveExtraData(originalData->GetName());
						}
					}
				}

				return RE::BSVisit::BSVisitControl::kContinue;
			});
		}

		void MaterialShader(RE::NiAVObject* a_root, const std::vector<RE::NiStringsExtraData*>& a_vec)
		{
			if (a_vec.empty()) {
				return;
			}

			for (auto& data : a_vec) {
				if (data && data->value && data->size > 0) {
					std::vector<RE::BSFixedString> vec({ data->value, data->value + data->size });
					reset_shaderdata(a_root, vec);

					a_root->RemoveExtraData(data->GetName());
				}
			}
		}
	}

	namespace SET
	{
		void tint_face(RE::Actor* a_actor, const RE::NiColor& a_color)
		{
			const auto object = a_actor->GetHeadPartObject(RE::BGSHeadPart::HeadPartType::kFace);
			const auto geometry = object ? object->AsGeometry() : nullptr;

			if (geometry) {
				using Feature = RE::BSShaderMaterial::Feature;
				using Flag = RE::BSShaderProperty::EShaderPropertyFlag8;
				using States = RE::BSGeometry::States;

				using Material = RE::BSLightingShaderMaterialBase;

				const auto effect = geometry->properties[States::kEffect];
				const auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect.get());
				if (lightingShader) {
					const auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
					if (material && material->GetFeature() == Feature::kFaceGen) {
						const auto facegenTint = Material::CreateMaterial<RE::BSLightingShaderMaterialFacegenTint>();
						if (facegenTint) {
							facegenTint->CopyBaseMembers(material);

							lightingShader->SetFlags(Flag::kFace, false);
							lightingShader->SetFlags(Flag::kFaceGenRGBTint, true);

							lightingShader->SetMaterial(facegenTint, true);
							lightingShader->SetupGeometry(geometry);
							lightingShader->FinishSetupGeometry(geometry);

							facegenTint->~BSLightingShaderMaterialFacegenTint();
							RE::free(facegenTint);
						}
					}
				}

				geometry->UpdateBodyTint(a_color);
			}
		}

		void update_color_data(RE::NiAVObject* a_root, const RE::BSFixedString& a_name, const RE::NiColor& a_color)
		{
			if (const auto data = a_root->GetExtraData<RE::NiIntegerExtraData>(a_name); data) {
				if (a_color != RE::NiColor(data->value)) {
					data->value = a_color.ToInt();
				}
			} else {
				if (const auto newData = RE::NiIntegerExtraData::Create(a_name, a_color.ToInt()); newData) {
					a_root->AddExtraData(newData);
				}
			}
		}

		void Toggle(RE::NiAVObject* a_root, RE::NiAVObject* a_node, bool a_cull)
		{
			a_node->CullNode(a_cull);

			if (const auto data = a_root->GetExtraData<RE::NiStringsExtraData>(EXTRA::TOGGLE); data) {
				a_cull ?
					data->Insert(a_node->name) :
					data->Remove(a_node->name);
			} else if (a_cull) {
				std::vector<RE::BSFixedString> vec{ a_node->name };
				if (const auto newData = RE::NiStringsExtraData::Create(EXTRA::TOGGLE, vec); newData) {
					a_root->AddExtraData(newData);
				}
			}
		}

		void ArmorTXST(RE::NiAVObject* a_object, RE::BGSTextureSet* a_txst, std::int32_t a_type, std::string_view a_tgtPath, bool& replaced)
		{
			using Feature = RE::BSShaderMaterial::Feature;
			using Texture = RE::BSTextureSet::Texture;

			RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
				const auto effect = a_geometry->properties[RE::BSGeometry::States::kEffect];
				const auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect.get());

				const auto material = lightingShader ? static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material) : nullptr;
				const auto textureSet = material ? material->textureSet : RE::NiPointer<RE::BSTextureSet>();

				if (lightingShader && material) {
					if (textureSet) {
						std::string sourcePath{ textureSet->GetTexturePath(Texture::kDiffuse) };
						TEXTURE::sanitize_path(sourcePath);

						if (sourcePath == a_tgtPath) {
							if (const auto newMaterial = static_cast<RE::BSLightingShaderMaterialBase*>(material->Create()); newMaterial) {
								newMaterial->CopyMembers(material);
								newMaterial->ClearTextures();

								if (a_type == -1) {
									newMaterial->OnLoadTextureSet(0, a_txst);
								} else {
									if (const auto newTextureSet = RE::BSShaderTextureSet::Create(); newTextureSet) {
										const auto BSTextureType = static_cast<Texture>(a_type);
										for (const auto& type : TEXTURE::types) {
											if (type != BSTextureType) {
												newTextureSet->SetTexturePath(type, textureSet->GetTexturePath(type));
											}
										}
										newTextureSet->SetTexturePath(BSTextureType, a_txst->GetTexturePath(BSTextureType));
										newMaterial->OnLoadTextureSet(0, newTextureSet);
									}
								}

								lightingShader->SetMaterial(newMaterial, true);

								lightingShader->SetupGeometry(a_geometry);
								lightingShader->FinishSetupGeometry(a_geometry);

								newMaterial->~BSLightingShaderMaterialBase();
								RE::free(newMaterial);

								replaced = true;
							}
						}
					}
				}

				return RE::BSVisit::BSVisitControl::kContinue;
			});
		}

		void SkinTXST(RE::NiAVObject* a_object, RE::BGSTextureSet* a_txst, std::vector<RE::BSFixedString>& a_vec, std::int32_t a_type)
		{
			using Feature = RE::BSShaderMaterial::Feature;
			using Texture = RE::BSTextureSet::Texture;

			RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
				const auto effect = a_geometry->properties[RE::BSGeometry::States::kEffect];
				const auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect.get());

				const auto material = lightingShader ? static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material) : nullptr;

				if (lightingShader && material) {
					auto const feature = material->GetFeature();
					if (const auto textureSet = material->textureSet; textureSet && stl::is_in(feature, Feature::kFaceGenRGBTint, Feature::kFaceGen)) {
						if (a_vec.empty()) {
							a_vec.reserve(Texture::kTotal);
							for (auto& type : TEXTURE::types) {
								a_vec.emplace_back(textureSet->GetTexturePath(type));
							}
						}
						if (const auto newMaterial = static_cast<RE::BSLightingShaderMaterialBase*>(material->Create()); newMaterial) {
							newMaterial->CopyMembers(material);
							newMaterial->ClearTextures();

							if (a_type == -1) {
								if (feature == Feature::kFaceGen) {
									if (const auto newTextureSet = RE::BSShaderTextureSet::Create(); newTextureSet) {
										for (auto& type : TEXTURE::types) {
											if (type != Texture::kMultilayer) {
												newTextureSet->SetTexturePath(type, a_txst->GetTexturePath(type));
											}
										}
										newTextureSet->SetTexturePath(Texture::kMultilayer, textureSet->GetTexturePath(Texture::kMultilayer));
										newMaterial->OnLoadTextureSet(0, newTextureSet);
									}
								} else {
									newMaterial->OnLoadTextureSet(0, a_txst);
								}
							} else {
								if (const auto newTextureSet = RE::BSShaderTextureSet::Create(); newTextureSet) {
									const auto BSTextureType = static_cast<Texture>(a_type);
									for (const auto& type : TEXTURE::types) {
										if (type != BSTextureType) {
											newTextureSet->SetTexturePath(type, material->textureSet->GetTexturePath(type));
										}
									}
									newTextureSet->SetTexturePath(BSTextureType, a_txst->GetTexturePath(BSTextureType));
									newMaterial->OnLoadTextureSet(0, newTextureSet);
								}
							}

							lightingShader->SetMaterial(newMaterial, true);
							lightingShader->SetupGeometry(a_geometry);
							lightingShader->FinishSetupGeometry(a_geometry);

							newMaterial->~BSLightingShaderMaterialBase();
							RE::free(newMaterial);
						}
					}
				}

				return RE::BSVisit::BSVisitControl::kContinue;
			});
		}

		void ArmorSkinTXST(RE::Actor* a_actor, RE::BGSTextureSet* a_txst, RE::BIPED_MODEL::BipedObjectSlot a_slot, std::int32_t a_type)
		{
			const auto skinArmor = a_actor->GetSkin(a_slot);
			if (!skinArmor) {
				return;
			}

			const auto skinArma = skinArmor->GetArmorAddonByMask(a_actor->GetRace(), a_slot);
			if (!skinArma) {
				return;
			}

			SKSE::GetTaskInterface()->AddTask([a_actor, a_txst, a_slot, a_type, skinArmor, skinArma]() {
				std::vector<RE::BSFixedString> result;
				a_actor->VisitArmorAddon(skinArmor, skinArma, [&](bool, RE::NiAVObject& a_obj) -> bool {
					SET::SkinTXST(&a_obj, a_txst, result, a_type);
					return true;
				});

				const auto root = a_actor->Get3D(false);
				if (!result.empty() && root) {
					auto       slotMaskStr = std::to_string(std::to_underlying(a_slot));
					const auto name = "PO3_SKINTXST - " + slotMaskStr;
					result.emplace_back(slotMaskStr);

					SET::add_data_if_none<RE::NiStringsExtraData>(root, name, result);
				}
			});
		}

		void ShaderType(RE::NiAVObject* a_object, const RE::BSGeometry* a_template, const std::string_view a_path, std::int32_t a_textureType, std::vector<RE::BSFixedString>& a_vec, const std::vector<bool>& a_params)
		{
			using State = RE::BSGeometry::States;
			using Feature = RE::BSShaderMaterial::Feature;
			using Texture = RE::BSTextureSet::Texture;
			using Flags = RE::BSShaderProperty::EShaderPropertyFlag8;
			using VertexFlags = RE::BSGraphics::Vertex::Flags;

			const bool noWeap = a_params[0];
			const bool noAlpha = a_params[1];
			const bool isActor = a_params[2];

			RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
				if (!a_geometry->vertexDesc.HasFlag(VertexFlags::VF_NORMAL)) {
					return RE::BSVisit::BSVisitControl::kContinue;
				}
				if (const auto parent = a_geometry->parent; parent && parent->AsFadeNode() && noWeap && isActor) {
					return RE::BSVisit::BSVisitControl::kContinue;
				}
				if (const auto alpha = a_geometry->properties[State::kProperty]; alpha && noAlpha) {
					return RE::BSVisit::BSVisitControl::kContinue;
				}

				const auto effect = a_geometry->properties[State::kEffect];
				const auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect.get());

				const auto tempEffect = a_template->properties[State::kEffect];
				const auto tempLightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(tempEffect.get());

				if (lightingShader && tempLightingShader) {
					const auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
					const auto tempMaterial = static_cast<RE::BSLightingShaderMaterialBase*>(tempLightingShader->material);

					if (material && tempMaterial) {
						const auto textureSet = material->textureSet;
						const auto tempTextureSet = tempMaterial->textureSet;

						if (textureSet && tempTextureSet) {
							std::string sourceDiffuse{ textureSet->GetTexturePath(Texture::kDiffuse) };
							TEXTURE::sanitize_path(sourceDiffuse);
							if (!a_path.empty() && sourceDiffuse.find(a_path) == std::string::npos) {
								return RE::BSVisit::BSVisitControl::kContinue;
							}

							const auto oldFeature = material->GetFeature();
							const auto oldFlags = lightingShader->flags.get();
							const auto oldEmissiveColor = lightingShader->emissiveColor ? lightingShader->emissiveColor->ToHex() : "000000";
							const auto oldEmissiveMult = lightingShader->emissiveMult;

							const auto newFeature = tempMaterial->GetFeature();

							if (oldFeature != newFeature) {
								if (const auto data = lightingShader->GetExtraData<RE::NiStringsExtraData>(EXTRA::ORIG_SHADER); !data) {
									std::vector<RE::BSFixedString> resetData;
									resetData.reserve(12);
									for (auto& type : TEXTURE::types) {
										resetData.emplace_back(textureSet->GetTexturePath(type));  //0-8
									}
									resetData.emplace_back(std::to_string(std::to_underlying(oldFeature)));  //9
									resetData.emplace_back(std::to_string(std::to_underlying(oldFlags)));    //10
									resetData.emplace_back(oldEmissiveColor);                                //11
									resetData.emplace_back(std::to_string(oldEmissiveMult));                 //12

									const auto newData = RE::NiStringsExtraData::Create(EXTRA::ORIG_SHADER, resetData);
									if (newData && lightingShader->AddExtraData(newData)) {
										a_vec.push_back(a_geometry->name);
									}
								}

								if (const auto newMaterial = static_cast<RE::BSLightingShaderMaterialBase*>(tempMaterial->Create()); newMaterial) {
									newMaterial->CopyMembers(tempMaterial);
									newMaterial->ClearTextures();

									if (a_textureType != -1) {
										if (const auto newTextureSet = RE::BSShaderTextureSet::Create(); newTextureSet) {
											const auto BSTextureType = static_cast<Texture>(a_textureType);
											for (auto& type : TEXTURE::types) {
												if (type != BSTextureType) {
													newTextureSet->SetTexturePath(type, tempTextureSet->GetTexturePath(type));
												}
											}
											newTextureSet->SetTexturePath(BSTextureType, textureSet->GetTexturePath(BSTextureType));
											newMaterial->OnLoadTextureSet(0, newTextureSet);
										}
									}

									lightingShader->CopyMembers(tempLightingShader);
									lightingShader->SetFlags(Flags::kSkinned, a_geometry->vertexDesc.HasFlag(VertexFlags::VF_SKINNED));

									lightingShader->SetMaterial(newMaterial, true);
									lightingShader->SetupGeometry(a_geometry);
									lightingShader->FinishSetupGeometry(a_geometry);

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
	}

	namespace TEXTURE
	{
		void sanitize_path(std::string& a_path)
		{
			std::ranges::transform(a_path, a_path.begin(),
				[](char c) { return static_cast<char>(std::tolower(c)); });

			a_path = srell::regex_replace(a_path, srell::regex("/+|\\\\+"), "\\");
			a_path = srell::regex_replace(a_path, srell::regex("^\\\\+"), "");
			a_path = srell::regex_replace(a_path, srell::regex(R"(.*?[^\s]textures\\|^textures\\)", srell::regex::icase), "");
		}

		RE::BSShaderTextureSet* create_textureset(char** a_value)
		{
			if (const auto textureset = RE::BSShaderTextureSet::Create(); textureset) {
				for (const auto& type : types) {
					if (!string::is_empty(a_value[type])) {
						textureset->SetTexturePath(type, a_value[type]);
					}
				}
				return textureset;
			}
			return nullptr;
		}
	}
}
