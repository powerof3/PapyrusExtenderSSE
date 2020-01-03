#include "po3_graphicfunctions.h"

extern const SKSE::TaskInterface* g_task;

namespace RE
{
	//------------------------------SKIN--------------------------------------------------

	void SetShaderPropertyRGBTint(BSGeometry* geometry)
	{
		if (geometry)
		{
			auto shaderProperty = niptr_cast<BSShaderProperty>(geometry->states[BSGeometry::States::kEffect]);
			if (!shaderProperty)
			{
				return;
			}

			auto lightingShader = netimmerse_cast<BSLightingShaderProperty*>(shaderProperty);
			if (lightingShader)
			{
				auto material = lightingShader->material;
				if (material)
				{
					if (material->GetFeature() == BSShaderMaterial::Feature::kFaceGen)
					{
						auto tintedMaterial = BSLightingShaderMaterialFacegenTint::CreateFacegenTintMaterial();

						tintedMaterial->CopyBaseMaterial(material);

						lightingShader->SetFlags(0x0A, false);
						lightingShader->SetFlags(0x15, true);

						lightingShader->SetMaterial(tintedMaterial, true);
						lightingShader->InitializeShader(geometry);

						tintedMaterial->~BSLightingShaderMaterialFacegenTint();
						free(tintedMaterial);
					}
				}
			}
		}
	}

	//color mixing algorithm
	UInt8 colorMix(UInt8 a, UInt8 b, float t) //t is percentage blend
	{
		return round(sqrt((1 - t) * (b * b) + t * (a * a)));
	}

	//luminance detection algorithm
	float calculateLuminance(UInt8 R, UInt8 G, UInt8 B) //returns luminance between 0-1.0
	{
		return (0.2126 * R + 0.7152 * G + 0.0722 * B) / 255.0;
	}

	//----------------------------SET ALPHA-----------------------------------------------

	void SetShaderPropertyAlpha(BSGeometry* geometry, float alpha, bool onlySkin)
	{
		if (!geometry)
		{
			return;
		}

		auto shaderProperty = niptr_cast<BSShaderProperty>(geometry->states[BSGeometry::States::kEffect]);
		if (!shaderProperty)
		{
			return;
		}

		auto lightingShader = netimmerse_cast<BSLightingShaderProperty*>(shaderProperty);
		if (lightingShader)
		{
			auto material = lightingShader->material;
			if (material)
			{
				if (onlySkin)
				{
					auto type = material->GetFeature();

					if (type == BSShaderMaterial::Feature::kFaceGenRGBTint || type == BSShaderMaterial::Feature::kFaceGen)
					{
						material->materialAlpha = alpha;
					}
				}
				else
				{
					material->materialAlpha = alpha;
				}
			}
		}
	}

	void SetArmorSkinAlpha(Actor* thisActor, BGSBipedObjectForm::FirstPersonFlag slotMask, float alpha)
	{
		TESObjectARMO* skinarmor = GetSkinForm(thisActor, slotMask);

		if (!skinarmor)
		{
			return;
		}

		TESObjectARMA* foundAddon = GetArmorAddonByMask(thisActor->race, skinarmor, slotMask);
		if (foundAddon)
		{
			NiAVObject* armorObject = VisitArmorAddon(thisActor, skinarmor, foundAddon);
			if (armorObject)
			{
				auto node = armorObject->GetAsNiNode();
				if (node)
				{
					for (size_t i = 0; i < node->children.size(); i++)
					{
						auto object = node->children[i];

						if (object)
						{
							SetShaderPropertyAlpha(object->GetAsBSGeometry(), alpha, true);
						}
					}
				}
				else
				{
					SetShaderPropertyAlpha(armorObject->GetAsBSGeometry(), alpha, true);
				}
			}
		}
	}

	//----------------------------SET TEXTURESET--------------------------------------------

	bool ReplaceTextureSet(BSGeometry* geometry, BGSTextureSet& sourceTXST, BGSTextureSet& targetTXST, SInt32 textureType, const std::string& targetPath)
	{
		if (!geometry)
		{
			return false;
		}

		auto shaderProperty = niptr_cast<BSShaderProperty>(geometry->states[BSGeometry::States::kEffect]);
		if (!shaderProperty)
		{
			return false;
		}

		auto lightingShader = netimmerse_cast<BSLightingShaderProperty*>(shaderProperty);
		if (lightingShader)
		{
			auto material = lightingShader->material;
			if (material)
			{
				std::string sourcePath = material->textureSet->GetTexturePath(BSTextureSet::Texture::kDiffuse);
				SanitizePath(sourcePath);

				if (sourcePath != targetPath)
				{
					return false;
				}

				auto newMaterial = static_cast<BSLightingShaderMaterialBase*>(material->Create());
				newMaterial->CopyMembers(material);

				if (textureType == -1)
				{
					newMaterial->ClearTextures();
					newMaterial->SetTextureSet(&targetTXST);
				}
				else
				{
					auto newTextureSet = BSShaderTextureSet::Create();
					for (auto i = BSTextureSet::Texture::kDiffuse; i < BSTextureSet::Textures::kTotal; ++i)
					{
						newTextureSet->SetTexturePath(i, material->textureSet->GetTexturePath(i));
					}

					auto BSTextureType = static_cast<BSTextureSet::Texture>(textureType);
					newTextureSet->SetTexturePath(BSTextureType, targetTXST.GetTexturePath(BSTextureType));

					newMaterial->ClearTextures();
					newMaterial->SetTextureSet(newTextureSet);
				}

				lightingShader->SetMaterial(newMaterial, 1);
				lightingShader->InvalidateTextures(0);
				lightingShader->InitializeShader(geometry);

				newMaterial->~BSLightingShaderMaterialBase();
				free(newMaterial);

				return true;
			}
		}

		return false;
	}

	//----------------------------SET SKIN TEXTURESET--------------------------------------------

	bool ReplaceSkinTXST(BSGeometry* geometry, BGSTextureSet& TXST, std::vector<BSFixedString>& vec, SInt32 textureType)
	{
		if (!geometry)
		{
			return false;
		}

		auto shaderProperty = niptr_cast<BSShaderProperty>(geometry->states[BSGeometry::States::kEffect]);
		if (!shaderProperty)
		{
			return false;
		}

		auto lightingShader = netimmerse_cast<BSLightingShaderProperty*>(shaderProperty);
		if (lightingShader)
		{
			auto material = lightingShader->material;
			if (material)
			{
				auto type = material->GetFeature();

				if (type == BSShaderMaterial::Feature::kFaceGenRGBTint || type == BSShaderMaterial::Feature::kFaceGen)
				{
					if (vec.empty())
					{
						for (auto i = BSTextureSet::Texture::kDiffuse; i < BSTextureSet::Textures::kTotal; ++i)
						{
							vec.emplace_back(material->textureSet->GetTexturePath(i));
						}
					}

					auto newMaterial = static_cast<BSLightingShaderMaterialBase*>(material->Create());
					newMaterial->CopyMembers(material);

					if (textureType == -1)
					{
						newMaterial->ClearTextures();

						if (type == BSShaderMaterial::Feature::kFaceGen)
						{
							auto newTextureSet = BSShaderTextureSet::Create();
							for (auto i = BSTextureSet::Texture::kDiffuse; i < BSTextureSet::Textures::kTotal; ++i)
							{
								newTextureSet->SetTexturePath(i, TXST.GetTexturePath(i));
							}

							newTextureSet->SetTexturePath(BSTextureSet::Texture::kMultilayer, material->textureSet->GetTexturePath(BSTextureSet::Texture::kMultilayer));
							newMaterial->SetTextureSet(newTextureSet);
						}
						else
						{
							newMaterial->SetTextureSet(&TXST);
						}
					}
					else
					{
						auto newTextureSet = BSShaderTextureSet::Create();
						for (auto i = BSTextureSet::Texture::kDiffuse; i < BSTextureSet::Textures::kTotal; ++i)
						{
							newTextureSet->SetTexturePath(i, material->textureSet->GetTexturePath(i));
						}

						auto BSTextureType = static_cast<BSTextureSet::Texture>(textureType);
						newTextureSet->SetTexturePath(BSTextureType, TXST.GetTexturePath(BSTextureType));

						newMaterial->ClearTextures();
						newMaterial->SetTextureSet(newTextureSet);
					}

					lightingShader->SetMaterial(newMaterial, 1);
					lightingShader->InvalidateTextures(0);
					lightingShader->InitializeShader(geometry);

					newMaterial->~BSLightingShaderMaterialBase();
					free(newMaterial);

					return true;
				}
			}
		}

		return false;
	}

	void SetArmorSkinTXST(Actor* thisActor, BGSTextureSet* TXST, BGSBipedObjectForm::BipedObjectSlot slotMask, SInt32 textureType)
	{
		TESObjectARMO* skinarmor = GetSkinForm(thisActor, slotMask);
		if (!skinarmor)
		{
			return;
		}

		g_task->AddTask([thisActor, skinarmor, TXST, slotMask, textureType]()
		{
			TESObjectARMA* foundAddon = GetArmorAddonByMask(thisActor->race, skinarmor, slotMask);
			if (foundAddon)
			{
				NiAVObject* armorObject = VisitArmorAddon(thisActor, skinarmor, foundAddon);
				if (armorObject)
				{
					bool replaced = false;

					std::vector<BSFixedString> vec;
					vec.reserve(10);

					auto node = armorObject->GetAsNiNode();
					if (node)
					{
						for (UInt32 i = 0; i < node->children.size(); i++)
						{
							auto object = node->children[i];

							if (object && ReplaceSkinTXST(object->GetAsBSGeometry(), *TXST, vec, textureType))
							{
								replaced = true;
							}
						}
					}
					else
					{
						replaced = ReplaceSkinTXST(armorObject->GetAsBSGeometry(), *TXST, vec, textureType);
					}

					auto root = thisActor->Get3D(0);
					if (replaced && root)
					{
						std::string slotMaskStr = std::to_string(static_cast<UInt32>(slotMask));
						std::string name = "PO3_SKINTXST - " + slotMaskStr;

						vec.emplace_back(slotMaskStr.c_str());

						auto data = static_cast<NiStringsExtraData*>(root->GetExtraData(BSFixedString(name.c_str())));
						if (!data)
						{
							auto newData = NiStringsExtraData::Create(BSFixedString(name.c_str()), vec.data(), vec.size());
							if (data)
							{
								root->AddExtraData(newData);
							}
						}
					}
				}
			}
		});
	}

	//--------------------------------------------------------------------------------------

	void SetShaderPropertyType(BSGeometry* geometry, BSGeometry* templateGeometry)
	{
		if (!geometry || !templateGeometry)
		{
			return;
		}

		auto shaderProperty = niptr_cast<BSShaderProperty>(geometry->states[BSGeometry::States::kEffect]);
		auto templateShaderProperty = niptr_cast<BSShaderProperty>(templateGeometry->states[BSGeometry::States::kEffect]);

		if (!shaderProperty || !templateShaderProperty)
		{
			return;
		}

		auto lightingShader = netimmerse_cast<BSLightingShaderProperty*>(shaderProperty);
		auto templateLightingShader = netimmerse_cast<BSLightingShaderProperty*>(shaderProperty);

		if (lightingShader && templateLightingShader)
		{
			auto material = lightingShader->material;
			auto templateMaterial = templateLightingShader->material;

			if (material && templateMaterial)
			{
				auto type = material->GetFeature();

				if (type == BSShaderMaterial::Feature::kFaceGen || type == BSShaderMaterial::Feature::kFaceGenRGBTint)
				{
					auto newMaterial = static_cast<BSLightingShaderMaterialBase*>(templateMaterial->Create());

					newMaterial->CopyMembers(templateMaterial);

					lightingShader->shaderFlags1 = templateLightingShader->shaderFlags1;
					lightingShader->shaderFlags2 = templateLightingShader->shaderFlags2;

					newMaterial->ClearTextures();
					newMaterial->SetTextureSet(templateMaterial->textureSet.get());

					lightingShader->SetMaterial(newMaterial, 1);

					lightingShader->InvalidateTextures(0);
					lightingShader->InitializeShader(geometry);

					newMaterial->~BSLightingShaderMaterialBase();
					free(newMaterial);
				}
			}
		}
	}

	BSGeometry* GetArmorGeometry(Actor* thisActor, BGSBipedObjectForm::FirstPersonFlag slotMask)
	{
		TESObjectARMO* skinarmor = GetSkinForm(thisActor, slotMask);
		if (!skinarmor)
		{
			return nullptr;
		}

		TESObjectARMA* foundAddon = GetArmorAddonByMask(thisActor->race, skinarmor, slotMask);
		if (foundAddon)
		{
			NiAVObject* armorObject = VisitArmorAddon(thisActor, skinarmor, foundAddon);
			if (armorObject)
			{
				GetFirstShaderType(armorObject, BSShaderMaterial::Feature::kFaceGenRGBTint);
			}
		}

		return nullptr;
	}

	//-------------------------RESET---------------------------------------------------------

	TESObjectARMO* GetWornFormByID(Actor* thisActor, FormID id)
	{
		TESObjectARMO* armor = nullptr;

		auto exChanges = static_cast<ExtraContainerChanges*>(thisActor->extraData.GetByType(ExtraDataType::kContainerChanges));
		InventoryChanges* changes = exChanges ? exChanges->changes : nullptr;

		if (changes && changes->entryList)
		{
			for (auto& data : *changes->entryList)
			{
				if (!data)
				{
					continue;
				}

				auto object = data->object;

				if (!object || !object->IsArmor() || object->formID != id)
				{
					continue;
				}

				armor = static_cast<TESObjectARMO*>(object);
				break;
			}
		}

		if (!armor)
		{
			auto actorBase = thisActor->GetActorBase();
			if (actorBase)
			{
				armor = actorBase->skin;
			}
		}

		return armor;
	}

	void ResetTextureSet(BSGeometry* geometry, BSShaderTextureSet* textureset, bool isSkin)
	{
		if (!geometry)
		{
			return;
		}

		auto shaderProperty = niptr_cast<BSShaderProperty>(geometry->states[BSGeometry::States::kEffect]);
		if (!shaderProperty)
		{
			return;
		}

		auto lightingShader = netimmerse_cast<BSLightingShaderProperty*>(shaderProperty);
		if (lightingShader)
		{
			auto material = lightingShader->material;
			if (material)
			{
				if (!isSkin)
				{
					std::string sourcePath = material->textureSet->GetTexturePath(BSTextureSet::Texture::kDiffuse);
					std::transform(sourcePath.begin(), sourcePath.end(), sourcePath.begin(), ::tolower);

					std::size_t found = sourcePath.find("po3fireburns");

					if (found == std::string::npos)
					{
						return;
					}
				}
				else
				{
					auto type = material->GetFeature();

					if (type != BSShaderMaterial::Feature::kFaceGenRGBTint && type != BSShaderMaterial::Feature::kFaceGen)
					{
						return;
					}
				}

				auto newMaterial = static_cast<BSLightingShaderMaterialBase*>(material->Create());
				newMaterial->CopyMembers(material);

				newMaterial->ClearTextures();
				newMaterial->SetTextureSet(textureset);

				lightingShader->SetMaterial(newMaterial, 1);
				lightingShader->InvalidateTextures(0);
				lightingShader->InitializeShader(geometry);

				newMaterial->~BSLightingShaderMaterialBase();
				free(newMaterial);
			}
		}
	}

	//---------------------MISC FUNCTIONS---------------------------------------------------

	// Gets face node - adapted from RaceMenu
	BSGeometry* GetHeadPartGeometry(Actor* actor, BGSHeadPart::HeadPartType partType)
	{
		BSFaceGenNiNode* faceNode = actor->GetFaceNodeSkinned();
		auto actorBase = actor->GetActorBase();

		if (faceNode && actorBase)
		{
			BGSHeadPart* facePart = actorBase->GetCurrentHeadPartByType(partType);
			if (facePart)
			{
				NiAVObject* headNode = faceNode->GetObjectByName(facePart->formEditorID);
				if (headNode)
				{
					BSGeometry* geometry = headNode->GetAsBSGeometry();
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
	NiAVObject* VisitArmorAddon(Actor* actor, TESObjectARMO* armor, TESObjectARMA* arma)
	{
		char addonString[MAX_PATH];
		memset(addonString, 0, MAX_PATH);
		arma->GetNodeName(addonString, actor, armor, -1);

		BSFixedString rootName("NPC Root [Root]");

		NiNode* skeletonRoot[2];
		skeletonRoot[0] = actor->Get3D(0)->GetAsNiNode();
		skeletonRoot[1] = actor->Get3D(1)->GetAsNiNode();

		// Skip second skeleton, it's the same as the first
		if (skeletonRoot[1] == skeletonRoot[0])
		{
			skeletonRoot[1] = nullptr;
		}

		for (UInt32 i = 0; i <= 1; i++)
		{
			if (skeletonRoot[i])
			{
				NiAVObject* root = skeletonRoot[i]->GetObjectByName(rootName);
				if (root)
				{
					NiNode* rootNode = root->GetAsNiNode();
					if (rootNode)
					{
						BSFixedString addonName(addonString); // Find the Armor name from the root
						NiAVObject* armorObject = skeletonRoot[i]->GetObjectByName(addonName);

						if (armorObject)
						{
							return armorObject;
						}
					}
				}
			}
		}

		return nullptr;
	}

	BSGeometry* GetFirstShaderType(NiAVObject* object, BSShaderMaterial::Feature shaderType)
	{
		NiNode* node = object->GetAsNiNode();
		if (node)
		{
			for (UInt32 i = 0; i < node->children.size(); i++)
			{
				auto child = node->children[i];
				if (child)
				{
					BSGeometry* skin = GetFirstShaderType(child.get(), shaderType);

					if (skin)
					{
						return skin;
					}
				}
			}
		}
		else
		{
			auto geometry = object->GetAsBSGeometry();
			if (geometry)
			{
				auto shaderProperty = niptr_cast<BSShaderProperty>(geometry->states[BSGeometry::States::kEffect]);
				if (shaderProperty)
				{
					auto lightingShader = netimmerse_cast<BSLightingShaderProperty*>(shaderProperty);
					if (lightingShader)
					{
						// Find first geometry if the type is any
						if (shaderType == BSShaderMaterial::Feature::kNone)
						{
							return geometry;
						}

						auto material = lightingShader->material;

						if (material && material->GetFeature() == shaderType)
						{
							return geometry;
						}
					}
				}
			}
		}

		return nullptr;
	}

	bool HasShaderType(NiAVObject* object, BSShaderMaterial::Feature shaderType)
	{
		NiNode* node = object->GetAsNiNode();
		if (node)
		{
			for (UInt32 i = 0; i < node->children.size(); i++)
			{
				auto child = node->children[i];
				if (child)
				{
					if (HasShaderType(child.get(), shaderType))
					{
						return true;
					}
				}
			}
		}
		else
		{
			auto geometry = object->GetAsBSGeometry();
			if (geometry)
			{
				auto shaderProperty = niptr_cast<BSShaderProperty>(geometry->states[BSGeometry::States::kEffect]);
				if (shaderProperty)
				{
					auto lightingShader = netimmerse_cast<BSLightingShaderProperty*>(shaderProperty);
					if (lightingShader)
					{
						auto material = lightingShader->material;

						if (material && material->GetFeature() == shaderType)
						{
							return true;
						}
					}
				}
			}
		}

		return false;
	}

	//get worn form
	TESObjectARMO* GetWornForm(Actor* thisActor, BGSBipedObjectForm::BipedObjectSlot mask)
	{
		auto exChanges = static_cast<ExtraContainerChanges*>(thisActor->extraData.GetByType(ExtraDataType::kContainerChanges)); //loop through caster inventory
		InventoryChanges* changes = exChanges ? exChanges->changes : nullptr;

		if (changes && changes->entryList)
		{
			for (auto& data : *changes->entryList)
			{
				if (!data || !data->extraLists)
				{
					continue;
				}

				auto object = data->object;

				if (!object || !object->IsArmor())
				{
					continue;
				}

				for (auto& extraList : *data->extraLists)
				{
					if (extraList->HasType(ExtraDataType::kWorn) || extraList->HasType(ExtraDataType::kWornLeft))
					{
						auto armor = static_cast<TESObjectARMO*>(data->object);
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
	TESObjectARMO* GetSkinForm(Actor* thisActor, BGSBipedObjectForm::BipedObjectSlot mask)
	{
		TESObjectARMO* equipped = nullptr;
		if (thisActor)
		{
			equipped = GetWornForm(thisActor, mask);
			if (!equipped)
			{
				auto actorBase = thisActor->GetActorBase();
				if (actorBase)
				{
					equipped = actorBase->skin; // Check ActorBase

					TESRace* race = actorBase->race;
					if (!equipped && race)
					{
						equipped = race->skin; // Check Race
					}
				}
			}
		}

		return equipped;
	}

	//get armor addon with mask
	TESObjectARMA* GetArmorAddonByMask(TESRace* race, TESObjectARMO* armor, BGSBipedObjectForm::BipedObjectSlot mask)
	{
		for (auto& currentAddon : armor->armature)
		{
			if (currentAddon && currentAddon->IsValidRace(race) && (currentAddon->GetSlotMask() & mask) == mask)
			{
				return currentAddon;
			}
		}

		return nullptr;
	}

	void SanitizePath(std::string& path)
	{
		std::transform(path.begin(), path.end(), path.begin(), ::tolower);

		path = std::regex_replace(path, std::regex("/+|\\\\+"), "\\"); // Replace multiple slashes or forward slashes with one backslash
		path = std::regex_replace(path, std::regex("^\\\\+"), ""); // Remove all backslashes from the front
		path = std::regex_replace(path, std::regex(R"(.*?[^\s]textures\\|^textures\\)", std::regex_constants::icase), ""); // Remove everything before and including the textures path root
	}
}