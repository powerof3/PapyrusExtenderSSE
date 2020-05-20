#include "po3_graphicfunctions.h"

extern const SKSE::TaskInterface* g_task;

namespace RE
{
	//------------------------------SKIN--------------------------------------------------

	void TintFace(Actor* a_actor, const NiColor& a_color)
	{
		auto object = a_actor->GetHeadPartObject(BGSHeadPart::HeadPartType::kFace);
		if (object) {
			auto geometry = object->AsGeometry();
			if (geometry) {
				geometry->MakeFaceGenTintable();
				geometry->UpdateBodyTint(a_color);
			}
		}
	}


	void AddOrUpdateColorData(NiAVObject* a_root, const BSFixedString& a_name, const NiColor& a_color)
	{
		auto data = static_cast<NiIntegerExtraData*>(a_root->GetExtraData(a_name));
		if (!data) {
			auto newData = NiIntegerExtraData::Create(a_name, NiColor::ColorToInt(a_color));
			if (newData) {
				a_root->AddExtraData(newData);
			}
		}
		else {
			auto color = NiColor(data->value);
			if (a_color != color) {
				data->value = NiColor::ColorToInt(a_color);
			}
		}
	}

	//----------------------------SET TEXTURESET--------------------------------------------

	void SetTextureSet(NiAVObject* a_object, BGSTextureSet& a_txst, SInt32 a_type, const std::string& a_tgtPath, bool& replaced)
	{
		BSVisit::TraverseScenegraphGeometries(a_object, [&](BSGeometry* a_geometry) -> BSVisit::BSVisitControl
		{
			using State = BSGeometry::States;
			using Feature = BSShaderMaterial::Feature;
			using Texture = BSTextureSet::Texture;

			auto effect = a_geometry->properties[State::kEffect].get();
			if (effect) {
				auto lightingShader = netimmerse_cast<BSLightingShaderProperty*>(effect);
				if (lightingShader) {
					auto material = static_cast<BSLightingShaderMaterialBase*>(lightingShader->material);
					if (material) {
						std::string sourcePath = material->textureSet->GetTexturePath(Texture::kDiffuse);
						Util::SanitizeTexturePath(sourcePath);
						if (sourcePath == a_tgtPath) {
							auto newMaterial = static_cast<BSLightingShaderMaterialBase*>(material->Create());
							if (newMaterial) {
								newMaterial->CopyMembers(material);
								newMaterial->ClearTextures();
								if (a_type == -1) {
									newMaterial->OnLoadTextureSet(0, &a_txst);
								}
								else {
									auto newTextureSet = BSShaderTextureSet::Create();
									if (newTextureSet) {
										for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
											newTextureSet->SetTexturePath(i, material->textureSet->GetTexturePath(i));
										}
										auto BSTextureType = static_cast<Texture>(a_type);
										newTextureSet->SetTexturePath(BSTextureType, a_txst.GetTexturePath(BSTextureType));

										newMaterial->OnLoadTextureSet(0, newTextureSet);
									}
								}
								lightingShader->SetMaterial(newMaterial, 1);
								lightingShader->InitializeGeometry(a_geometry);
								lightingShader->InitializeShader(a_geometry);
								newMaterial->~BSLightingShaderMaterialBase();
								free(newMaterial);

								replaced = true;
							}
						}
					}
				}
			}
			return BSVisit::BSVisitControl::kContinue;
		});
	}

	//----------------------------SET SKIN TEXTURESET--------------------------------------------

	void SetSkinTextureSet(NiAVObject* a_object, BGSTextureSet& a_txst, std::vector<BSFixedString>& a_vec, SInt32 a_type)
	{
		BSVisit::TraverseScenegraphGeometries(a_object, [&](BSGeometry* a_geometry) -> BSVisit::BSVisitControl
		{
			using State = BSGeometry::States;
			using Feature = BSShaderMaterial::Feature;
			using Texture = BSTextureSet::Texture;

			auto effect = a_geometry->properties[State::kEffect].get();
			if (effect) {
				auto lightingShader = netimmerse_cast<BSLightingShaderProperty*>(effect);
				if (lightingShader) {
					auto material = static_cast<BSLightingShaderMaterialBase*>(lightingShader->material);
					if (material) {
						auto type = material->GetFeature();
						if (type == Feature::kFaceGenRGBTint || type == Feature::kFaceGen) {
							if (a_vec.empty()) {
								for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
									a_vec.emplace_back(material->textureSet->GetTexturePath(i));
								}
							}
							auto newMaterial = static_cast<BSLightingShaderMaterialBase*>(material->Create());
							if (newMaterial) {
								newMaterial->CopyMembers(material);
								newMaterial->ClearTextures();
								if (a_type == -1) {
									if (type == Feature::kFaceGen) {
										auto newTextureSet = BSShaderTextureSet::Create();
										if (newTextureSet) {
											for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
												newTextureSet->SetTexturePath(i, a_txst.GetTexturePath(i));
												newTextureSet->SetTexturePath(Texture::kMultilayer, material->textureSet->GetTexturePath(Texture::kMultilayer));
												newMaterial->OnLoadTextureSet(0, newTextureSet);
											}
										}
									}
									else {
										newMaterial->OnLoadTextureSet(0, &a_txst);
									}
								}
								else {
									auto newTextureSet = BSShaderTextureSet::Create();
									if (newTextureSet) {
										for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
											newTextureSet->SetTexturePath(i, material->textureSet->GetTexturePath(i));
										}
										auto BSTextureType = static_cast<Texture>(a_type);
										newTextureSet->SetTexturePath(BSTextureType, a_txst.GetTexturePath(BSTextureType));
										
										newMaterial->OnLoadTextureSet(0, newTextureSet);
									}
								}
								lightingShader->SetMaterial(newMaterial, 1);
								lightingShader->InitializeGeometry(a_geometry);
								lightingShader->InitializeShader(a_geometry);
								newMaterial->~BSLightingShaderMaterialBase();
								free(newMaterial);
							}
						}
					}
				}
			}
			return BSVisit::BSVisitControl::kContinue;
		});
	}


	void SetArmorSkinTXST(Actor* a_actor, BGSTextureSet* a_txst, BGSBipedObjectForm::BipedObjectSlot a_slot, SInt32 a_type)
	{
		TESObjectARMO* skinarmor = a_actor->GetSkin(a_slot);
		if (skinarmor) {
			g_task->AddTask([a_actor, skinarmor, a_txst, a_slot, a_type]()
			{
				TESObjectARMA* foundAddon = skinarmor->GetArmorAddonByMask(a_actor->race, a_slot);
				if (foundAddon) {
					NiAVObject* armorObject = a_actor->VisitArmorAddon(skinarmor, foundAddon);
					if (armorObject) {
						std::vector<BSFixedString> vec;
						vec.reserve(10);
						SetSkinTextureSet(armorObject, *a_txst, vec, a_type);

						auto root = a_actor->Get3D(0);
						if (!vec.empty() && root) {
							std::string slotMaskStr = std::to_string(static_cast<UInt32>(a_slot));
							std::string name = "PO3_SKINTXST - " + slotMaskStr;
							vec.emplace_back(slotMaskStr.c_str());
							auto data = static_cast<NiStringsExtraData*>(root->GetExtraData(BSFixedString(name.c_str())));
							if (!data) {
								auto newData = NiStringsExtraData::Create(BSFixedString(name.c_str()), vec.data(), static_cast<UInt32>(vec.size()));
								if (data) {
									root->AddExtraData(newData);
								}
							}
						}
					}
				}
			});
		}
	}

	//--------------------------------------------------------------------------------------

	BSGeometry* GetSkinGeometry(Actor* a_actor, BGSBipedObjectForm::FirstPersonFlag a_slot)
	{
		TESObjectARMO* skinarmor = a_actor->GetSkin(a_slot);
		if (!skinarmor) {
			return nullptr;
		}

		TESObjectARMA* foundAddon = skinarmor->GetArmorAddonByMask(a_actor->race, a_slot);
		if (foundAddon) {
			NiAVObject* armorObject = a_actor->VisitArmorAddon(skinarmor, foundAddon);
			if (armorObject) {
				armorObject->GetFirstGeometryOfShaderType(BSShaderMaterial::Feature::kFaceGenRGBTint);
				if (!armorObject) {
					armorObject->GetFirstGeometryOfShaderType(BSShaderMaterial::Feature::kFaceGen);
				}
			}
		}

		return nullptr;
	}

	//-------------------------RESET---------------------------------------------------------

	void ResetTextureSet(NiAVObject* a_object, BSShaderTextureSet& a_txst, NiAVObject::ALPHA_MODE a_type, const std::string& a_folder)
	{
		BSVisit::TraverseScenegraphGeometries(a_object, [&](BSGeometry* a_geometry) -> BSVisit::BSVisitControl
		{
			using State = BSGeometry::States;
			using Feature = BSShaderMaterial::Feature;
			using Texture = BSTextureSet::Texture;

			auto effect = a_geometry->properties[State::kEffect].get();
			if (effect) {
				auto lightingShader = netimmerse_cast<BSLightingShaderProperty*>(effect);
				if (lightingShader) {
					auto material = static_cast<BSLightingShaderMaterialBase*>(lightingShader->material);
					if (material) {
						if (a_type == NiAVObject::ALPHA_MODE::kAll) {
							if (!a_folder.empty()) {
								std::string sourcePath = material->textureSet->GetTexturePath(Texture::kDiffuse);
								Util::SanitizeTexturePath(sourcePath);
								std::size_t found = sourcePath.find(a_folder);
								if (found == std::string::npos) {
									return BSVisit::BSVisitControl::kContinue;
								}
							}
						}
						else {
							auto type = material->GetFeature();
							if (type != Feature::kFaceGenRGBTint && type != Feature::kFaceGen) {
								return BSVisit::BSVisitControl::kContinue;
							}
						}
						auto newMaterial = static_cast<BSLightingShaderMaterialBase*>(material->Create());
						if (newMaterial) {
							newMaterial->CopyMembers(material);
							newMaterial->ClearTextures();
							newMaterial->OnLoadTextureSet(0, &a_txst);
							lightingShader->SetMaterial(newMaterial, 1);
							lightingShader->InitializeGeometry(a_geometry);
							lightingShader->InitializeShader(a_geometry);
							newMaterial->~BSLightingShaderMaterialBase();
							free(newMaterial);
						}
					}
				}
			}
			return BSVisit::BSVisitControl::kContinue;
		});
	}

	//-------------------------SET SHADER TYPE---------------------------------------------------------

	void SetShaderType_Impl(NiAVObject* a_object, BSGeometry* a_template, const std::string& a_path, SInt32 a_textureType, std::vector<std::vector<BSFixedString>>& a_vec, bool a_noWeapons, bool a_noAlpha, bool a_isActor)
	{
		BSVisit::TraverseScenegraphGeometries(a_object, [&](BSGeometry* a_geometry) -> BSVisit::BSVisitControl
		{
			using State = BSGeometry::States;
			using Feature = BSShaderMaterial::Feature;
			using Texture = BSTextureSet::Texture;
			using Flag = BSShaderProperty::EShaderPropertyFlag8;
			using VertexFlags = NiSkinPartition::VertexFlags;

			if (a_template) {
				bool hasNormals = (NiSkinPartition::GetVertexFlags(a_geometry->vertexDesc) & VertexFlags::VF_NORMAL) == VertexFlags::VF_NORMAL;
				if (!hasNormals) {
					return BSVisit::BSVisitControl::kContinue;
				}

				auto alpha = a_geometry->properties[State::kProperty].get();
				if (alpha && a_noAlpha) {
					return BSVisit::BSVisitControl::kContinue;
				}
				
				auto parent = a_geometry->parent;
				if (parent) {
					if (a_isActor && a_noWeapons && parent->AsFadeNode()) {
						return BSVisit::BSVisitControl::kContinue;
					}
				}

				auto effect = a_geometry->properties[State::kEffect].get();
				auto tempEffect = a_template->properties[State::kEffect].get();

				if (effect && tempEffect) {
					auto lightingShader = netimmerse_cast<BSLightingShaderProperty*>(effect);
					auto tempLightingShader = netimmerse_cast<BSLightingShaderProperty*>(tempEffect);

					if (lightingShader && tempLightingShader) {
						auto material = static_cast<BSLightingShaderMaterialBase*>(lightingShader->material);
						auto tempMaterial = static_cast<BSLightingShaderMaterialBase*>(tempLightingShader->material);

						if (material && tempMaterial) {

							std::string sourceDiffuse = material->textureSet->GetTexturePath(Texture::kDiffuse);
							Util::SanitizeTexturePath(sourceDiffuse);
							if (!a_path.empty()) {
								std::size_t found = sourceDiffuse.find(a_path);
								if (found == std::string::npos) {
									return BSVisit::BSVisitControl::kContinue;
								}
							}
							
							if (material->GetFeature() != tempMaterial->GetFeature()) {
								auto newMaterial = static_cast<BSLightingShaderMaterialBase*>(tempMaterial->Create());
								if (newMaterial) {									
									std::string newDiffuse = tempMaterial->textureSet->GetTexturePath(Texture::kDiffuse);
									Util::SanitizeTexturePath(newDiffuse);

									std::string oldDiffuse = material->textureSet->GetTexturePath(Texture::kDiffuse);

									if (a_vec.empty() || !a_vec.back().empty() && a_vec.back().front() != oldDiffuse.c_str()) {
										std::vector<BSFixedString> shaderData;
										shaderData.reserve(15);
										for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
											shaderData.emplace_back(material->textureSet->GetTexturePath(i));
										}
										shaderData.emplace_back(std::to_string(static_cast<UInt32>(lightingShader->flags))); 
										shaderData.emplace_back(NiColor::ColorToString(*lightingShader->emissiveColor));
										shaderData.emplace_back(std::to_string(lightingShader->emissiveMult));
										shaderData.emplace_back(std::to_string(static_cast<UInt32>(tempMaterial->GetFeature())));
										shaderData.emplace_back(std::to_string(static_cast<UInt32>(material->GetFeature())));
										shaderData.emplace_back(Util::GetTextureName(newDiffuse));
										Util::SanitizeTexturePath(oldDiffuse);
										shaderData.emplace_back(Util::GetTextureName(oldDiffuse));
										
										a_vec.push_back(shaderData);
									}

									newMaterial->CopyMembers(tempMaterial);
									newMaterial->ClearTextures();
									if (a_textureType != -1) {
										auto newTextureSet = BSShaderTextureSet::Create();
										if (newTextureSet) {
											for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
												newTextureSet->SetTexturePath(i, tempMaterial->textureSet->GetTexturePath(i));
											}
											auto BSTextureType = static_cast<Texture>(a_textureType);
											newTextureSet->SetTexturePath(BSTextureType, material->textureSet->GetTexturePath(BSTextureType));
											
											newMaterial->OnLoadTextureSet(0, newTextureSet);
										}
									}
									
									lightingShader->SetMaterial(newMaterial, 1);
									lightingShader->InitializeGeometry(a_geometry);
									lightingShader->InitializeShader(a_geometry);
									lightingShader->CopyMembers(tempLightingShader);

									bool isSkinned = (NiSkinPartition::GetVertexFlags(a_geometry->vertexDesc) & VertexFlags::VF_SKINNED) == VertexFlags::VF_SKINNED;
									if (isSkinned) {
										lightingShader->SetFlags(Flag::kSkinned, true);
									}
									else {
										lightingShader->SetFlags(Flag::kSkinned, false);
									}
									
									newMaterial->~BSLightingShaderMaterialBase();
									free(newMaterial);
								}
							}
						}
					}
				}
			}
			return BSVisit::BSVisitControl::kContinue;
		});
	}


	void ResetShaderType(NiAVObject* a_object, BSShaderTextureSet& a_txst, BSShaderProperty::EShaderPropertyFlag a_flags, const NiColor& a_emissive, float a_emissiveMult, BSShaderMaterial::Feature a_changed, BSShaderMaterial::Feature a_original, const std::string& a_changedDiffuse)
	{
		BSVisit::TraverseScenegraphGeometries(a_object, [&](BSGeometry* a_geometry) -> BSVisit::BSVisitControl
		{
			using State = BSGeometry::States;
			using Feature = BSShaderMaterial::Feature;
			using Texture = BSTextureSet::Texture;
			using Flag = BSShaderProperty::EShaderPropertyFlag;
			using VertexFlags = NiSkinPartition::VertexFlags;

			bool hasNormals = (NiSkinPartition::GetVertexFlags(a_geometry->vertexDesc) & VertexFlags::VF_NORMAL) == VertexFlags::VF_NORMAL;
			if (!hasNormals) {
				return BSVisit::BSVisitControl::kContinue;
			}
			
			auto parent = a_geometry->parent;
			if (parent && parent->AsFadeNode()) {
				return BSVisit::BSVisitControl::kContinue;
			}
			
			auto alpha = a_geometry->properties[State::kProperty].get();
			if (alpha) {
				return BSVisit::BSVisitControl::kContinue;
			}
			
			auto effect = a_geometry->properties[State::kEffect].get();
			if (effect) {
				auto lightingShader = netimmerse_cast<BSLightingShaderProperty*>(effect);
				if (lightingShader) {
					auto material = static_cast<BSLightingShaderMaterialBase*>(lightingShader->material);
					if (material) {
						if (material->GetFeature() == a_changed) {
							std::string currentDiffuse = material->textureSet->GetTexturePath(Texture::kDiffuse);
							Util::SanitizeTexturePath(currentDiffuse);
							std::size_t found = currentDiffuse.find(a_changedDiffuse);
							if (found == std::string::npos) {
								return BSVisit::BSVisitControl::kContinue;
							}
							auto newMaterial = BSLightingShaderMaterialBase::CreateMaterial(a_original);
							if (newMaterial) {
								newMaterial->CopyBaseMembers(material);
								newMaterial->ClearTextures();
								newMaterial->OnLoadTextureSet(0, &a_txst);															
								lightingShader->SetMaterial(newMaterial, 1);
								lightingShader->InitializeGeometry(a_geometry);
								lightingShader->InitializeShader(a_geometry);
																
								lightingShader->flags = a_flags;
								lightingShader->lastRenderPassState = std::numeric_limits<SInt32>::max();
								if ((lightingShader->flags & Flag::kOwnEmit) == Flag::kOwnEmit) {
									lightingShader->emissiveColor->red = a_emissive.red;
									lightingShader->emissiveColor->green = a_emissive.green;
									lightingShader->emissiveColor->blue = a_emissive.blue;
								}
								lightingShader->emissiveMult = a_emissiveMult;

								newMaterial->~BSLightingShaderMaterialBase();
								free(newMaterial);
							}
						}
					}
				}
			}
			
			return BSVisit::BSVisitControl::kContinue;
		});
	}
}