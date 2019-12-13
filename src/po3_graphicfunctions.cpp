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
					if (material->GetType() == BSShaderMaterial::Type::kFaceGen)
					{
						auto tintedMaterial = BSLightingShaderMaterialBase::CreateMaterial(BSShaderMaterial::Type::kFaceGenRGBTint);
						tintedMaterial->CopyBaseMaterial(material);
						lightingShader->SetFlags(0x0A, false);
						lightingShader->SetFlags(0x15, true);
						lightingShader->SetMaterial(tintedMaterial, true);
						lightingShader->InitializeShader(geometry);
						tintedMaterial->~BSLightingShaderMaterialBase();

						auto scrapHeap = TESMemoryManager::GetSingleton()->scrapHeap;
						scrapHeap->Free(tintedMaterial);
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
					auto type = material->GetType();
					
					if (type == BSShaderMaterial::Type::kFaceGenRGBTint || type == BSShaderMaterial::Type::kFaceGen)
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
					for (size_t i = 0; i < node->children.GetSize(); i++)
					{
						auto object = node->children.GetAt(i).get();

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

	bool ReplaceTextureSet(BSGeometry* geometry, BGSTextureSet& sourceTXST, BGSTextureSet& targetTXST, SInt32 textureType, const std::string& path)
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
				auto BSTextureType = static_cast<BSTextureSet::Texture>(textureType);
				std::string sourcePath = material->textureSet->GetTexturePath(BSTextureSet::Texture::kDiffuse);

				//making everything lowercase
				std::transform(sourcePath.begin(), sourcePath.end(), sourcePath.begin(), ::tolower);

				//CK texturesets start without "textures\" path while vanilla nifs always start with it.
				size_t data_pos = sourcePath.find(R"(data\textures\)", 0, 14);

				if (data_pos != std::string::npos)
				{
					sourcePath.erase(data_pos, 14); //removing "data\textures\"
				}
				else
				{
					size_t txt_pos = sourcePath.find(R"(textures\)", 0, 9);

					if (txt_pos != std::string::npos)
					{
						sourcePath.erase(txt_pos, 9); //removing "textures\"
					}
				}

				if (sourcePath != path)
				{
					return false;
				}

				auto newMaterial = BSLightingShaderMaterialBase::CreateMaterial(material->GetType());
				newMaterial->Copy(material);

				if (textureType == -1)
				{
					newMaterial->ReleaseTextures();
					newMaterial->SetTextureSet(&targetTXST);
				}
				else
				{
					auto newTextureSet = BSShaderTextureSet::Create();

					for (auto i = BSTextureSet::Texture::kDiffuse; i < BSTextureSet::Textures::kTotal; ++i)
					{
						newTextureSet->SetTexturePath(i, material->textureSet->GetTexturePath(i));
					}

					newTextureSet->SetTexturePath(BSTextureType, targetTXST.GetTexturePath(BSTextureType));

					newMaterial->ReleaseTextures();
					newMaterial->SetTextureSet(newTextureSet);
				}

				lightingShader->SetMaterial(newMaterial, 1);

				lightingShader->InvalidateTextures(0);
				lightingShader->InitializeShader(geometry);

				newMaterial->~BSLightingShaderMaterialBase();

				auto scrapHeap = TESMemoryManager::GetSingleton()->scrapHeap;
				scrapHeap->Free(newMaterial);

				return true;
			}
		}

		return false;
	}

	//----------------------------SET SKIN TEXTURESET--------------------------------------------

	bool ReplaceSkinTXST(BSGeometry* geometry, BGSTextureSet& TXST, std::vector<std::string>& vec, SInt32 textureType)
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
				auto type = material->GetType();

				if (type == BSShaderMaterial::Type::kFaceGenRGBTint || type == BSShaderMaterial::Type::kFaceGen)
				{
					auto BSTextureType = static_cast<BSTextureSet::Texture>(textureType);
					std::string sourcePath = material->textureSet->GetTexturePath(BSTextureSet::Texture::kDiffuse);

					if (vec.empty())
					{
						for (auto i = BSTextureSet::Texture::kDiffuse; i < BSTextureSet::Textures::kTotal; ++i)
						{
							auto path = material->textureSet->GetTexturePath(i);

							if (path && path[0] != '\0')
							{
								vec.push_back(path);
							}
							else
							{
								vec.push_back("NULL");
							}
						}
					}

					auto newMaterial = BSLightingShaderMaterialBase::CreateMaterial(type);
					newMaterial->Copy(material);

					if (textureType == -1)
					{
						newMaterial->ReleaseTextures();

						if (type == BSShaderMaterial::Type::kFaceGen)
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

						newTextureSet->SetTexturePath(BSTextureType, TXST.GetTexturePath(BSTextureType));

						newMaterial->ReleaseTextures();
						newMaterial->SetTextureSet(newTextureSet);
					}

					lightingShader->SetMaterial(newMaterial, 1);

					lightingShader->InvalidateTextures(0);
					lightingShader->InitializeShader(geometry);

					newMaterial->~BSLightingShaderMaterialBase();

					auto scrapHeap = TESMemoryManager::GetSingleton()->scrapHeap;
					scrapHeap->Free(newMaterial);

					return true;
				}
			}
		}

		return false;
	}

	void SetArmorSkinTXST(Actor* thisActor, BGSTextureSet* TXST, BGSBipedObjectForm::FirstPersonFlag slotMask, SInt32 textureType)
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
					std::vector<std::string> vec;

					auto node = armorObject->GetAsNiNode();

					if (node)
					{
						for (UInt32 i = 0; i < node->children.GetSize(); i++)
						{
							auto object = node->children.GetAt(i).get();

							if (object && ReplaceSkinTXST(object->GetAsBSGeometry(), *TXST, vec, textureType))
							{
								replaced = true;
							}
						}
					}
					else if (ReplaceSkinTXST(armorObject->GetAsBSGeometry(), *TXST, vec, textureType))
					{
						replaced = true;
					}

					if (replaced)
					{
						std::string slotMaskString = std::to_string(static_cast<UInt32>(slotMask));
						std::string name = "PO3_SKINTXST - " + slotMaskString;

						auto data = NiStringsExtraData::Create(BSFixedString(name.c_str()), 10);

						if (data)
						{
							data->value = NiAlloc<char*>(data->size);

							if (data->value && !vec.empty())
							{
								for (auto i = BSTextureSet::Texture::kDiffuse; i < BSTextureSet::Textures::kTotal; ++i)
								{
									std::string path = vec.at(i);

									if (!path.empty() && path != "NULL")
									{
										UInt32 strLength = strlen(path.c_str()) + 1;
										data->value[i] = NiAlloc<char>(strLength);
										memcpy(data->value[i], path.c_str(), sizeof(char) * strLength);
									}
								}

								UInt32 strLength = strlen(slotMaskString.c_str()) + 1;
								data->value[data->size - 1] = NiAlloc<char>(strLength);
								memcpy(data->value[data->size - 1], slotMaskString.c_str(), sizeof(char) * strLength);

								auto node = thisActor->GetNiRootNode(0);
								if (node)
								{
									node->AddExtraData(data);
								}
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
				auto type = material->GetType();

				if (type == BSShaderMaterial::Type::kFaceGen || type == BSShaderMaterial::Type::kFaceGenRGBTint)
				{
					auto newMaterial = BSLightingShaderMaterialBase::CreateMaterial(templateMaterial->GetType());

					newMaterial->Copy(templateMaterial);

					lightingShader->shaderFlags1 = templateLightingShader->shaderFlags1;
					lightingShader->shaderFlags2 = templateLightingShader->shaderFlags2;

					newMaterial->ReleaseTextures();
					newMaterial->SetTextureSet(templateMaterial->textureSet.get());

					lightingShader->SetMaterial(newMaterial, 1);

					lightingShader->InvalidateTextures(0);
					lightingShader->InitializeShader(geometry);

					newMaterial->~BSLightingShaderMaterialBase();

					auto scrapHeap = TESMemoryManager::GetSingleton()->scrapHeap;
					scrapHeap->Free(newMaterial);
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
				GetFirstShaderType(armorObject, BSShaderMaterial::Type::kFaceGenRGBTint);
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
				if (!data || !data->type || !data->type->IsArmor() || data->type->formID != id)
				{
					continue;
				}

				armor = skyrim_cast<TESObjectARMO*>(data->type);
			}
		}

		if (!armor)
		{
			auto actorBase = thisActor->GetActorBase();

			if (actorBase)
			{
				armor = thisActor->GetActorBase()->skin;
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

					//making everything lowercase
					std::transform(sourcePath.begin(), sourcePath.end(), sourcePath.begin(), ::tolower);

					std::size_t found = sourcePath.find("po3fireburns");

					if (found == std::string::npos)
					{
						return;
					}
				}
				else
				{
					auto type = material->GetType();

					if (type != BSShaderMaterial::Type::kFaceGenRGBTint && type != BSShaderMaterial::Type::kFaceGen)
					{
						return;
					}
				}

				auto newMaterial = BSLightingShaderMaterialBase::CreateMaterial(material->GetType());
				newMaterial->Copy(material);

				newMaterial->ReleaseTextures();
				newMaterial->SetTextureSet(textureset);

				lightingShader->SetMaterial(newMaterial, 1);

				lightingShader->InvalidateTextures(0);
				lightingShader->InitializeShader(geometry);

				newMaterial->~BSLightingShaderMaterialBase();

				auto scrapHeap = TESMemoryManager::GetSingleton()->scrapHeap;
				scrapHeap->Free(newMaterial);
			}
		}
	}

	//---------------------MISC FUNCTIONS---------------------------------------------------

	// Gets face node - adapted from RaceMenu
	BSGeometry* GetHeadPartGeometry(Actor* actor, BGSHeadPart::Type partType)
	{
		BSFaceGenNiNode* faceNode = actor->GetFaceGenNiNode();
		auto actorBase = actor->GetActorBase();

		if (faceNode && actorBase)
		{
			BGSHeadPart* facePart = actorBase->GetCurrentHeadPartByType(partType);
			if (facePart)
			{
				NiAVObject* headNode = faceNode->GetObjectByName(facePart->editorID);
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
		skeletonRoot[0] = actor->GetNiRootNode(0);
		skeletonRoot[1] = actor->GetNiRootNode(1);

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

	BSGeometry* GetFirstShaderType(NiAVObject* object, BSShaderMaterial::Type shaderType)
	{
		NiNode* node = object->GetAsNiNode();
		if (node)
		{
			for (UInt32 i = 0; i < node->children.GetSize(); i++)
			{
				NiAVObject* object = node->children.GetAt(i).get();

				if (object)
				{
					BSGeometry* skin = GetFirstShaderType(object, shaderType);

					if (skin)
					{
						return skin;
					}
				}
			}
		}
		else
		{
			BSGeometry* geometry = object->GetAsBSGeometry();
			if (geometry)
			{
				BSShaderProperty* shaderProperty = niptr_cast<BSShaderProperty>(geometry->states[BSGeometry::States::kEffect]);
				if (shaderProperty && shaderProperty->GetRTTI() == reinterpret_cast<const NiRTTI*>(NiRTTI_BSLightingShaderProperty))
				{
					// Find first geometry if the type is any
					if (shaderType == BSShaderMaterial::Type::kNull)
					{
						return geometry;
					}

					auto material = shaderProperty->material;
					if (material && material->GetType() == shaderType)
					{
						return geometry;
					}
				}
			}
		}

		return nullptr;
	}

	bool HasShaderType(NiAVObject* object, BSShaderMaterial::Type shaderType)
	{
		NiNode* node = object->GetAsNiNode();
		if (node)
		{
			for (UInt32 i = 0; i < node->children.GetSize(); i++)
			{
				NiAVObject* object = node->children.GetAt(i).get();

				if (object)
				{
					if (HasShaderType(object, shaderType))
					{
						return true;
					}
				}
			}
		}
		else
		{
			BSGeometry* geometry = object->GetAsBSGeometry();
			if (geometry)
			{
				BSShaderProperty* shaderProperty = niptr_cast<BSShaderProperty>(geometry->states[BSGeometry::States::kEffect]);
				if (shaderProperty && shaderProperty->GetRTTI() == reinterpret_cast<const NiRTTI*>(NiRTTI_BSLightingShaderProperty))
				{
					auto material = shaderProperty->material;
					if (material && material->GetType() == shaderType)
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	//get worn form
	TESObjectARMO* GetWornForm(Actor* thisActor, BGSBipedObjectForm::FirstPersonFlag mask)
	{
		auto exChanges = static_cast<ExtraContainerChanges*>(thisActor->extraData.GetByType(ExtraDataType::kContainerChanges)); //loop through caster inventory
		InventoryChanges* changes = exChanges ? exChanges->changes : nullptr;

		if (changes && changes->entryList)
		{
			for (auto& data : *changes->entryList)
			{
				if (!data || !data->type || !data->type->IsArmor() || !data->extraList)
				{
					continue;
				}

				for (auto& extraList : *data->extraList)
				{
					if (extraList->HasType(ExtraDataType::kWorn) || extraList->HasType(ExtraDataType::kWornLeft))
					{
						auto armor = skyrim_cast<TESObjectARMO*>(data->type);

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
	TESObjectARMO* GetSkinForm(Actor* thisActor, BGSBipedObjectForm::FirstPersonFlag mask)
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
	TESObjectARMA* GetArmorAddonByMask(TESRace* race, TESObjectARMO* armor, BGSBipedObjectForm::FirstPersonFlag mask)
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
}