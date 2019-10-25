#include "po3_graphicfunctions.h"

extern const SKSE::TaskInterface* g_task;

//-----------------------SKIN--------------------------------------------------------

//Sets skin color
void SetShaderPropertyRGBTint(RE::BSGeometry* geometry)
{
	if (geometry)
	{
		auto shaderProperty  = netimmersePtr_cast<RE::BSShaderProperty>(geometry->states[RE::BSGeometry::States::kEffect]);
		if (!shaderProperty)
		{
			return;
		}

		auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(shaderProperty);
		if (lightingShader)
		{
			RE::BSLightingShaderMaterial* material = (RE::BSLightingShaderMaterial*)lightingShader->material;

			if (material)
			{
				// Convert the shaderType to support tints
				if (material->GetType() == RE::BSShaderMaterial::Type::kFaceGen)
				{
					RE::BSLightingShaderMaterialFacegenTint* tintedMaterial = (RE::BSLightingShaderMaterialFacegenTint*)RE::BSLightingShaderMaterialBase::CreateMaterial(RE::BSShaderMaterial::Type::kFaceGenRGBTint);
					tintedMaterial->CopyFrom(material);
					lightingShader->SetFlags(0x0A, false);
					lightingShader->SetFlags(0x15, true);
					lightingShader->SetMaterial(tintedMaterial, true);
					lightingShader->InitializeShader(geometry);
				}
			}
		}
	}

	return;
}

//------------------------------------------------SET ALPHA------------------------------------------------

// sets alpha of skin
void SetShaderPropertyAlpha(RE::BSGeometry* geometry, float alpha, bool onlySkin)
{
	if (!geometry)
	{
		return;
	}

	auto shaderProperty  = netimmersePtr_cast<RE::BSShaderProperty>(geometry->states[RE::BSGeometry::States::kEffect]);
	if (!shaderProperty)
	{
		return;
	}

	auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(shaderProperty);
	if (lightingShader)
	{
		RE::BSLightingShaderMaterial* material = (RE::BSLightingShaderMaterial*)lightingShader->material;

		if (material)
		{
			if (onlySkin)
			{
				if ((material->GetType() == RE::BSShaderMaterial::Type::kFaceGenRGBTint) || (material->GetType() == RE::BSShaderMaterial::Type::kFaceGen))
				{
					material->alpha = alpha;
				}
			}
			else
			{
				material->alpha = alpha;
			}
		}
	}
}

void SetArmorSkinAlpha(RE::Actor* thisActor, RE::BGSBipedObjectForm::FirstPersonFlag slotMask, float alpha)
{
	RE::TESObjectARMO* skinarmor = GetSkinForm(thisActor, slotMask);

	if (!skinarmor)
	{
		return;
	}

	RE::TESObjectARMA* foundAddon = GetArmorAddonByMask(thisActor->race, skinarmor, slotMask);

	if (foundAddon)
	{
		RE::NiAVObject* armorNode = VisitArmorAddon(thisActor, skinarmor, foundAddon);

		if (armorNode)
		{
			RE::NiNode* node = armorNode->GetAsNiNode();

			if (node)
			{
				for (size_t i = 0; i < node->children.GetSize(); i++)
				{
					RE::NiAVObject* object = node->children.GetAt(i).get();

					if (object)
					{
						SetShaderPropertyAlpha(object->GetAsBSGeometry(), alpha, true);
					}
				}
			}
			else
			{
				SetShaderPropertyAlpha(armorNode->GetAsBSGeometry(), alpha, true);
			}
		}
	}
}

void ReplaceTextureSet(RE::BSGeometry* geometry, RE::BGSTextureSet* sourceTXST, RE::BGSTextureSet* targetTXST, SInt32 textureType)
{
	if (!geometry)
	{
		return;
	}

	auto shaderProperty = netimmersePtr_cast<RE::BSShaderProperty>(geometry->states[RE::BSGeometry::States::kEffect]);
	if (!shaderProperty)
	{
		return;
	}

	auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(shaderProperty);
	if (lightingShader)
	{
		RE::BSLightingShaderMaterial* material = (RE::BSLightingShaderMaterial*)lightingShader->material;

		if (material)
		{
			std::string sourcePath = (material->textureSet)->GetTexturePath(RE::BSTextureSet::Textures::kDiffuse);
			std::string targetPath = sourceTXST->GetTexturePath(RE::BSTextureSet::Textures::kDiffuse);

			//making everything lowercase
			std::transform(sourcePath.begin(), sourcePath.end(), sourcePath.begin(), ::tolower);
			std::transform(targetPath.begin(), targetPath.end(), targetPath.begin(), ::tolower);

			//CK texturesets start without "textures\" path while vanilla nifs always start with it.	
			size_t data_pos = sourcePath.find("data\\textures\\", 0, 14);
			size_t txt_pos = sourcePath.find("textures\\", 0, 9);

			if (data_pos != std::string::npos)
			{
				sourcePath.erase(data_pos, 14); //removing "data\textures\"
			}
			else if (txt_pos != std::string::npos)
			{
				sourcePath.erase(txt_pos, 9); //removing "textures\"
			}

			if (sourcePath != targetPath)
			{
				return;
			}

			if (textureType == -1)
			{
				material->ReleaseTextures();
				material->SetTextureSet(targetTXST);
			}
			else
			{
				if ((UInt32)textureType < RE::BSTextureSet::Textures::kTotal)
				{
					RE::BSShaderTextureSet* newTextureSet = RE::BSShaderTextureSet::Create();

					for (UInt32 i = 0; i < RE::BSTextureSet::Textures::kTotal; i++)
					{
						const char* texturePath = material->textureSet->GetTexturePath((RE::BSTextureSet::Texture)i);
						newTextureSet->SetTexturePath((RE::BSTextureSet::Texture)i, texturePath);
					}

					newTextureSet->SetTexturePath((RE::BSTextureSet::Texture)textureType, targetTXST->GetTexturePath((RE::BSTextureSet::Texture)textureType));
					material->ReleaseTextures();
					material->SetTextureSet(newTextureSet);
				}
			}

			lightingShader->InvalidateTextures(0);
			lightingShader->InitializeShader(geometry);
		}
	}
}

void ReplaceSkinTXST(RE::BSGeometry* geometry, RE::BGSTextureSet* TXST, SInt32 textureType)
{
	if (!geometry)
	{
		return;
	}

	auto shaderProperty  = netimmersePtr_cast<RE::BSShaderProperty>(geometry->states[RE::BSGeometry::States::kEffect]);
	if (!shaderProperty)
	{
		return;
	}

	auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(shaderProperty);
	if (lightingShader)
	{
		RE::BSLightingShaderMaterial* material = (RE::BSLightingShaderMaterial*)lightingShader->material;

		if (material && (material->GetType() == RE::BSShaderMaterial::Type::kFaceGenRGBTint || material->GetType() == RE::BSShaderMaterial::Type::kFaceGen))
		{
			if (textureType == -1)
			{
				material->ReleaseTextures();
				material->SetTextureSet(TXST);
			}
			else
			{
				if ((UInt32)textureType < RE::BSTextureSet::Textures::kTotal)
				{
					RE::BSShaderTextureSet* newTextureSet = RE::BSShaderTextureSet::Create();

					for (UInt32 i = 0; i < RE::BSTextureSet::Textures::kTotal; i++)
					{
						const char* texturePath = material->textureSet->GetTexturePath((RE::BSTextureSet::Texture)i);
						newTextureSet->SetTexturePath((RE::BSTextureSet::Texture)i, texturePath);
					}

					newTextureSet->SetTexturePath((RE::BSTextureSet::Texture)textureType, TXST->GetTexturePath((RE::BSTextureSet::Texture)textureType));
					material->ReleaseTextures();
					material->SetTextureSet(newTextureSet);
				}
			}

			lightingShader->InvalidateTextures(0);
			lightingShader->InitializeShader(geometry);
		}
	}
}

void SetArmorSkinTXST(RE::Actor* thisActor, RE::BGSTextureSet* TXST, RE::BGSBipedObjectForm::FirstPersonFlag slotMask, SInt32 textureType)
{
	RE::TESObjectARMO* skinarmor = GetSkinForm(thisActor, slotMask);

	if (!skinarmor)
	{
		return;
	}

	g_task->AddTask([thisActor, skinarmor, TXST, slotMask, textureType]()
	{
		RE::TESObjectARMA* foundAddon = GetArmorAddonByMask(thisActor->race, skinarmor, slotMask);

		if (foundAddon)
		{
			RE::NiAVObject* armorNode = VisitArmorAddon(thisActor, skinarmor, foundAddon);

			if (armorNode)
			{
				RE::NiNode* node = armorNode->GetAsNiNode();

				if (node)
				{
					for (UInt32 i = 0; i < node->children.GetSize(); i++)
					{
						RE::NiAVObject* object = node->children.GetAt(i).get();

						if (object)
						{
							ReplaceSkinTXST(object->GetAsBSGeometry(), TXST, textureType);
						}
					}
				}
				else
				{
					ReplaceSkinTXST(armorNode->GetAsBSGeometry(), TXST, textureType);
				}
			}
		}
	});
}

//--------------------------------------------------------------------------------------

void SetShaderPropertyMLP(RE::BSGeometry* geometry, RE::BSGeometry* templateGeometry)
{
	if (!geometry || !templateGeometry)
	{
		return;
	}

	auto shaderProperty = netimmersePtr_cast<RE::BSShaderProperty>(geometry->states[RE::BSGeometry::States::kEffect]);
	auto templateShaderProperty = netimmersePtr_cast<RE::BSShaderProperty>(templateGeometry->states[RE::BSGeometry::States::kEffect]);

	if (!shaderProperty || !templateShaderProperty)
	{
		return;
	}

	auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(shaderProperty);
	auto templateLightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(shaderProperty);

	if (lightingShader && templateLightingShader)
	{
		RE::BSLightingShaderMaterial* material = (RE::BSLightingShaderMaterial*)lightingShader->material;
		RE::BSLightingShaderMaterialMultiLayerParallax* templateMaterial = (RE::BSLightingShaderMaterialMultiLayerParallax*)templateLightingShader->material;

		if (material && templateMaterial && (material->GetType() == RE::BSShaderMaterial::Type::kFaceGen || material->GetType() == RE::BSShaderMaterial::Type::kFaceGenRGBTint))
		{
			RE::BSLightingShaderMaterialMultiLayerParallax* newMaterial = (RE::BSLightingShaderMaterialMultiLayerParallax*)RE::BSLightingShaderMaterialBase::CreateMaterial(RE::BSShaderMaterial::Type::kMultilayerParallax);

			newMaterial->CopyFrom(templateMaterial);

			newMaterial->fEnvmapScale = templateMaterial->fEnvmapScale;
			newMaterial->fParallaxInnerLayerUScale = templateMaterial->fParallaxInnerLayerUScale;
			newMaterial->fParallaxInnerLayerVScale = templateMaterial->fParallaxInnerLayerVScale;
			newMaterial->fParallaxLayerThickness = templateMaterial->fParallaxLayerThickness;
			newMaterial->fParallaxRefractionScale = templateMaterial->fParallaxRefractionScale;

			if (material->GetType() == RE::BSShaderMaterial::Type::kFaceGen)
			{
				lightingShader->SetFlags(0x0A, false); //disable facegen
			}
			else
			{
				lightingShader->SetFlags(0x15, false); //disable rgbtint
			}

			lightingShader->SetFlags(0x25, false); //vertex colors
			lightingShader->SetFlags(0x2F, false); //env_light fade
			lightingShader->SetFlags(0x39, false); //soft lighting

			lightingShader->SetFlags(0x38, true); //mlp
			lightingShader->SetFlags(0x3B, true); //back lighting

			RE::BSShaderTextureSet* newTextureSet = RE::BSShaderTextureSet::Create();
			for (UInt32 i = 0; i < RE::BSTextureSet::Textures::kTotal; i++)
			{
				const char* texturePath = material->textureSet->GetTexturePath((RE::BSTextureSet::Texture)i);
				newTextureSet->SetTexturePath((RE::BSTextureSet::Texture)i, texturePath);
			}

			newTextureSet->SetTexturePath(RE::BSTextureSet::Texture::kNormal, material->textureSet->GetTexturePath(RE::BSTextureSet::Texture::kNormal));

			lightingShader->SetMaterial(newMaterial, true);
			lightingShader->material->ReleaseTextures();
			lightingShader->material->SetTextureSet(newTextureSet);

			lightingShader->InvalidateTextures(0);
			lightingShader->InitializeShader(geometry);
		}
	}
}

RE::BSGeometry* GetArmorGeometry(RE::Actor* thisActor, RE::BGSBipedObjectForm::FirstPersonFlag slotMask, SInt32 shaderType)
{
	RE::TESObjectARMO* skinarmor = GetSkinForm(thisActor, slotMask);

	if (!skinarmor)
	{
		return nullptr;
	}

	RE::TESObjectARMA* foundAddon = GetArmorAddonByMask(thisActor->race, skinarmor, slotMask);

	if (foundAddon)
	{
		RE::NiAVObject* armorNode = VisitArmorAddon(thisActor, skinarmor, foundAddon);

		if (armorNode)
		{
			RE::NiNode* node = armorNode->GetAsNiNode();

			if (node)
			{
				for (UInt32 i = 0; i < node->children.GetSize(); i++)
				{
					RE::NiAVObject* object = node->children.GetAt(i).get();

					if (object && GetShaderPropertyType(object->GetAsBSGeometry()) == shaderType)
					{
						return object->GetAsBSGeometry();
					}
				}
			}
			else
			{
				if (GetShaderPropertyType(armorNode->GetAsBSGeometry()) == shaderType)
				{
					return armorNode->GetAsBSGeometry();
				}
			}
		}
	}

	return nullptr;
}

//---------------------MISC FUNCTIONS--------------------------------------

// Gets face node - adapted from RaceMenu
RE::BSGeometry* GetHeadPartGeometry(RE::Actor* actor, UInt32 partType)
{
	RE::BSFaceGenNiNode* faceNode = actor->GetFaceGenNiNode();
	auto actorBase = actor->GetActorBase();

	if (faceNode && actorBase)
	{
		RE::BGSHeadPart* facePart = actorBase->GetCurrentHeadPartByType((RE::BGSHeadPart::Type)partType);
		if (facePart)
		{
			RE::NiAVObject* headNode = faceNode->GetObjectByName(facePart->editorID);
			if (headNode)
			{
				RE::BSGeometry* geometry = headNode->GetAsBSGeometry();
				if (geometry)
				{
					return geometry;
				}
			}
		}
	}

	return nullptr;
}

// Gets AA node - adapted from RaceMenu
RE::NiAVObject* VisitArmorAddon(RE::Actor* thisActor, RE::TESObjectARMO* thisArmor, RE::TESObjectARMA* thisArma)
{
	char addonString[MAX_PATH];
	memset(addonString, 0, MAX_PATH);
	thisArma->GetNodeName(addonString, thisActor, thisArmor, -1);

	RE::BSFixedString rootName = "NPC Root [Root]";

	RE::NiNode* skeletonRoot[2];
	skeletonRoot[0] = thisActor->GetNiRootNode(0);
	skeletonRoot[1] = thisActor->GetNiRootNode(1);

	// Skip second skeleton, it's the same as the first
	if (skeletonRoot[1] == skeletonRoot[0])
	{
		skeletonRoot[1] = nullptr;
	}

	for (UInt32 i = 0; i <= 1; i++)
	{
		if (skeletonRoot[i])
		{
			RE::NiAVObject* root = skeletonRoot[i]->GetObjectByName(rootName);
			if (root)
			{
				RE::NiNode* rootNode = root->GetAsNiNode();
				if (rootNode)
				{
					RE::BSFixedString addonName = addonString; // Find the Armor name from the root
					RE::NiAVObject* armorNode = skeletonRoot[i]->GetObjectByName(addonName);

					if (armorNode)
					{
						return armorNode;
					}
				}
			}
		}
	}

	return nullptr;
}

SInt32 GetShaderPropertyType(RE::BSGeometry* geometry)
{
	if (!geometry)
	{
		return -1;
	}

	auto shaderProperty  = netimmersePtr_cast<RE::BSShaderProperty>(geometry->states[RE::BSGeometry::States::kEffect]);
	if (!shaderProperty)
	{
		return -1;
	}

	auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(shaderProperty);
	if (lightingShader)
	{
		RE::BSLightingShaderMaterial* material = (RE::BSLightingShaderMaterial*)lightingShader->material;

		if (material)
		{
			return (UInt32)material->GetType();
		}
	}

	return -1;
}

//checks whether geometry is modified
UInt32 GetShaderPropertyModdedSkin(RE::BSGeometry* geometry)
{
	if (!geometry)
	{
		return 0;
	}

	auto shaderProperty  = netimmersePtr_cast<RE::BSShaderProperty>(geometry->states[RE::BSGeometry::States::kEffect]);
	if (!shaderProperty)
	{
		return 0;
	}

	auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(shaderProperty);
	if (lightingShader)
	{
		RE::BSLightingShaderMaterial* material = (RE::BSLightingShaderMaterial*)lightingShader->material;

		if (material)
		{
			if (material->alpha != 1.0)
			{
				return 1;
			}
			else if (material->GetType() == RE::BSShaderMaterial::Type::kFaceGenRGBTint)
			{
				return 2;
			}
		}
	}

	return 0;
}

//color mixing algorithm
UInt8 colorMix(UInt8 a, UInt8 b, float t) //t is percentage blend
{
	return sqrt((1 - t) * pow(b, 2) + t * pow(a, 2));
}

//luminance detection algorithm
float calculateLuminance(UInt8 R, UInt8 G, UInt8 B) //returns luminance between 0-1.0
{
	return (0.2126 * R + 0.7152 * G + 0.0722 * B) / 255;
}

//get worn form
RE::TESObjectARMO* GetWornForm(RE::Actor* thisActor, RE::BGSBipedObjectForm::FirstPersonFlag mask)
{
	RE::ExtraContainerChanges* exChanges = static_cast<RE::ExtraContainerChanges*>(thisActor->extraData.GetByType(RE::ExtraDataType::kContainerChanges)); //loop through caster inventory
	RE::InventoryChanges* changes = exChanges ? exChanges->changes : nullptr;
	RE::ExtraWorn* worn = nullptr;
	RE::ExtraWornLeft* wornLeft = nullptr;

	if (changes && changes->entryList)
	{
		for (auto& data : *changes->entryList)
		{
			if (!data)
			{
				continue;
			}

			if (!data->type->IsArmor())
			{
				continue;
			}

			if (!data->extraList)
			{
				continue;
			}

			for (auto& extraList : *data->extraList)
			{
				worn = static_cast<RE::ExtraWorn*>(extraList->GetByType(RE::ExtraDataType::kWorn));
				wornLeft = static_cast<RE::ExtraWornLeft*>(extraList->GetByType(RE::ExtraDataType::kWornLeft));

				if (worn || wornLeft)
				{
					auto armor = skyrim_cast<RE::TESObjectARMO*>(data->type);

					if (armor)
					{
						for (auto& armorAddon : armor->armature)
						{
							if (armorAddon->HasPartOf(mask))
							{
								return armor;
							}
						}
					}
				}
			}
		}
	}

	return nullptr;
}

//get skin form
RE::TESObjectARMO* GetSkinForm(RE::Actor* thisActor, RE::BGSBipedObjectForm::FirstPersonFlag mask)
{
	RE::TESObjectARMO* equipped = nullptr;

	if (thisActor)
	{
		equipped = GetWornForm(thisActor, mask);

		if (!equipped)
		{
			auto actorBase = thisActor->GetActorBase();

			if (actorBase)
			{
				equipped = actorBase->skin; // Check ActorBase
			}

			RE::TESRace* race = actorBase->race;

			if (!equipped && race)
			{
				equipped = race->skin; // Check Race
			}
		}
	}

	return equipped;
}

//get armor addon with mask
RE::TESObjectARMA* GetArmorAddonByMask(RE::TESRace* race, RE::TESObjectARMO* armor, RE::BGSBipedObjectForm::FirstPersonFlag mask)
{
	for (auto& currentAddon : armor->armature)
	{
		if (currentAddon && currentAddon->IsValidRace(race) && ((currentAddon->GetSlotMask() & mask) == mask))
		{
			return currentAddon;
		}
	}

	return nullptr;
}