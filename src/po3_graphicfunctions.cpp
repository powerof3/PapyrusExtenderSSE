#include "po3_graphicfunctions.h"

extern const SKSE::TaskInterface* g_task;

namespace RE
{
	//------------------------------SKIN--------------------------------------------------

	void MakeFaceTintable(Actor* a_actor, const NiColor& a_color)
	{
		auto geometry = a_actor->GetHeadPartGeometry(BGSHeadPart::HeadPartType::kFace);
		if (geometry) {
			geometry->UpdateFaceGenTint(a_color);
		}
		geometry = GetSkinGeometry(a_actor, BGSBipedObjectForm::FirstPersonFlag::kDecapitate);
		if (geometry) {
			geometry->UpdateFaceGenTint(a_color);
		}
	}


	void AddOrUpdateColorData(NiAVObject* a_root, const BSFixedString& a_name, const Color& a_color)
	{
		auto data = static_cast<NiIntegersExtraData*>(a_root->GetExtraData(a_name));
		if (!data) {
			std::vector<SInt32> vec;
			vec.reserve(3);
			vec.emplace_back(a_color.red);
			vec.emplace_back(a_color.green);
			vec.emplace_back(a_color.blue);
			auto newData = NiIntegersExtraData::Create(a_name, vec.data(), static_cast<UInt32>(vec.size()));
			if (newData) {
				a_root->AddExtraData(newData);
			}
		}
		else {
			Color color;
			color.red = static_cast<UInt8>(data->value[0]);
			color.green = static_cast<UInt8>(data->value[1]);
			color.blue = static_cast<UInt8>(data->value[2]);
			if (a_color != color) {
				data->value[0] = a_color.red;
				data->value[1] = a_color.green;
				data->value[2] = a_color.blue;
			}
		}
	}

	//----------------------------SET ALPHA-----------------------------------------------

	void SetArmorSkinAlpha(Actor* a_actor, BGSBipedObjectForm::FirstPersonFlag a_slot, float a_alpha)
	{
		TESObjectARMO* skinarmor = a_actor->GetSkin(a_slot);
		if (skinarmor) {
			TESObjectARMA* foundAddon = skinarmor->GetArmorAddonByMask(a_actor->race, a_slot);
			if (foundAddon) {
				NiAVObject* armorObject = a_actor->VisitArmorAddon(skinarmor, foundAddon);
				if (armorObject) {
					armorObject->UpdateMaterialAlpha(a_alpha, true);
				}
			}
		}
	}

	//----------------------------SET TEXTURESET--------------------------------------------

	void SetTextureSet(NiAVObject* a_object, BGSTextureSet* a_srcTXST, BGSTextureSet* a_tgtTXST, SInt32 a_type, const std::string& a_tgtPath, bool& replaced)
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
							newMaterial->CopyMembers(material);
							if (a_type == -1) {
								newMaterial->ClearTextures();
								newMaterial->SetTextureSet(a_tgtTXST);
							}
							else {
								auto newTextureSet = BSShaderTextureSet::Create();
								for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
									newTextureSet->SetTexturePath(i, material->textureSet->GetTexturePath(i));
								}
								auto BSTextureType = static_cast<Texture>(a_type);
								newTextureSet->SetTexturePath(BSTextureType, a_tgtTXST->GetTexturePath(BSTextureType));

								newMaterial->ClearTextures();
								newMaterial->SetTextureSet(newTextureSet);
							}
							lightingShader->SetMaterial(newMaterial, 1);
							lightingShader->InvalidateTextures(0);
							lightingShader->InitializeGeometry(a_geometry);
							lightingShader->InitializeShader(a_geometry);
							newMaterial->~BSLightingShaderMaterialBase();
							free(newMaterial);

							replaced = true;
						}
					}
				}
			}
			return BSVisit::BSVisitControl::kContinue;
		});
	}

	//----------------------------SET SKIN TEXTURESET--------------------------------------------

	void SetSkinTextureSet(NiAVObject* a_object, BGSTextureSet* a_txst, std::vector<BSFixedString>& a_vec, SInt32 a_type)
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
							newMaterial->CopyMembers(material);
							if (a_type == -1) {
								newMaterial->ClearTextures();
								if (type == Feature::kFaceGen) {
									auto newTextureSet = BSShaderTextureSet::Create();
									for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
										newTextureSet->SetTexturePath(i, a_txst->GetTexturePath(i));
									}
									newTextureSet->SetTexturePath(Texture::kMultilayer, material->textureSet->GetTexturePath(Texture::kMultilayer));
									newMaterial->SetTextureSet(newTextureSet);
								}
								else {
									newMaterial->SetTextureSet(a_txst);
								}
							}
							else {
								auto newTextureSet = BSShaderTextureSet::Create();
								for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
									newTextureSet->SetTexturePath(i, material->textureSet->GetTexturePath(i));
								}
								auto BSTextureType = static_cast<Texture>(a_type);
								newTextureSet->SetTexturePath(BSTextureType, a_txst->GetTexturePath(BSTextureType));
								newMaterial->ClearTextures();
								newMaterial->SetTextureSet(newTextureSet);
							}
							lightingShader->SetMaterial(newMaterial, 1);
							lightingShader->InvalidateTextures(0);
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
						SetSkinTextureSet(armorObject, a_txst, vec, a_type);

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

	void ResetTextureSet(NiAVObject* a_object, BSShaderTextureSet* a_txst, bool a_skin, const std::string& a_folder)
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
						if (!a_skin) {
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
						newMaterial->CopyMembers(material);

						newMaterial->ClearTextures();
						newMaterial->SetTextureSet(a_txst);

						lightingShader->SetMaterial(newMaterial, 1);
						lightingShader->InvalidateTextures(0);
						lightingShader->InitializeGeometry(a_geometry);
						lightingShader->InitializeShader(a_geometry);

						newMaterial->~BSLightingShaderMaterialBase();
						free(newMaterial);
					}
				}
			}
			return BSVisit::BSVisitControl::kContinue;
		});
	}
}