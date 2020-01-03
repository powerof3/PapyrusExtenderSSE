//credits to underthesky for original implementation, davidjcobb, aers, nukem, ryan, meh321, expired for tips and help
//modifications and extra functions by powerofthree
//modified for CommonLib

#include "po3_functions.h"
#include "po3_graphicfunctions.h"
#include "po3_offsets.h"
#include <skse64\GameReferences.h>

//--------------------------------------------------------------------------------------------

extern const SKSE::TaskInterface* g_task;

extern RE::BGSKeyword* npcKeyword;
extern RE::BGSArtObject* soulTrapArt;

//--------------------------------------------------------------------------------------------

namespace RE
{
	SoulLevel GetSoulLevel(TESSoulGem* a_soulGem, InventoryEntryData* entry)
	{
		if (a_soulGem->containedSoul != SoulLevel::kNone)
		{
			return a_soulGem->containedSoul;
		}

		if (entry->extraLists)
		{
			for (auto& list : *entry->extraLists)
			{
				auto xSoul = static_cast<ExtraSoul*>(list->GetByType(ExtraDataType::kSoul));
				if (xSoul)
				{
					return xSoul->containedSoul;
				}
			}
		}

		return SoulLevel::kNone;
	}

	bool VerifyKeywords(TESForm* form, BSScript::VMArray<BGSKeyword*>* keywords)
	{
		auto size = keywords->size();
		if (size > 0)
		{
			auto pKeywords = skyrim_cast<BGSKeywordForm*>(form);
			if (pKeywords)
			{
				for (UInt32 i = 0; i < size; i++)
				{
					auto keyword = keywords->at(i);
					if (keyword && pKeywords->HasKeyword(keyword))
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	bool VerifyAllKeywords(TESForm* form, BSScript::VMArray<BGSKeyword*>* keywords)
	{
		auto size = keywords->size();
		if (size > 0)
		{
			auto pKeywords = skyrim_cast<BGSKeywordForm*>(form);
			if (pKeywords)
			{
				bool failed = false;
				for (UInt32 i = 0; i < size; i++)
				{
					auto keyword = keywords->at(i);
					if (keyword && !pKeywords->HasKeyword(keyword))
					{
						failed = true;
						break;
					}
				}

				if (failed)
				{
					return false;
				}
			}
		}

		return true;
	}

	// navmesh related functions
	float CalcLinearDistance(const NiPoint3& a_lhs, const NiPoint3& a_rhs)
	{
		return ((a_rhs.x - a_lhs.x) * (a_rhs.x - a_lhs.x)) + ((a_rhs.y - a_lhs.y) * (a_rhs.y - a_lhs.y)) + ((a_rhs.z - a_lhs.z) * (a_rhs.z - a_lhs.z));
	}

	std::optional<NiPoint3> FindNearestVertex(TESObjectREFR* a_ref)
	{
		auto cell = a_ref->GetParentCell();

		if (!cell || !cell->navMeshes)
		{
			return std::nullopt;
		}

		auto& navMeshes = *cell->navMeshes;
		auto shortestDistance = std::numeric_limits<float>::max();
		std::optional<NiPoint3> pos = std::nullopt;

		for (auto& navMesh : navMeshes)
		{
			for (auto& vertex : navMesh->vertices)
			{
				auto linearDistance = CalcLinearDistance(a_ref->GetPosition(), vertex.location);
				if (linearDistance < shortestDistance)
				{
					shortestDistance = linearDistance;
					pos.emplace(vertex.location);
				}
			}
		}

		return pos;
	}

	void StopAllShaders_Internal(TESObjectREFR* thisRef)
	{
		auto singleton = ProcessLists::GetSingleton();

		singleton->referenceEffectsLock.Lock();
		for (auto& referenceEffect : singleton->referenceEffects)
		{
			if (!referenceEffect)
			{
				continue;
			}

			auto refHandle = thisRef->CreateRefHandle();
			if (referenceEffect->refHandle != refHandle)
			{
				continue;
			}

			referenceEffect->unk40 = 1;
		}
		singleton->referenceEffectsLock.Unlock();
	}

	void StopAllSkinAlphaShaders(TESObjectREFR* thisRef)
	{
		auto singleton = ProcessLists::GetSingleton();

		singleton->referenceEffectsLock.Lock();
		for (auto& referenceEffect : singleton->referenceEffects)
		{
			if (!referenceEffect)
			{
				continue;
			}

			auto shaderEffect = netimmerse_cast<ShaderReferenceEffect*>(referenceEffect);
			if (shaderEffect)
			{
				auto handle = thisRef->CreateRefHandle();
				if (shaderEffect->refHandle == handle)
				{
					auto shader = shaderEffect->effectShader;

					if (shader && ((shader->data.flags & EffectShaderData::Flags::kSkinOnly) != EffectShaderData::Flags::kSkinOnly) && !shader->holesTexture.textureName.empty())
					{
						shaderEffect->unk40 = 1;
					}
				}
			}
		}
		singleton->referenceEffectsLock.Unlock();
	}

	void ResetAlpha(Actor* thisActor)
	{
		auto geometry = GetHeadPartGeometry(thisActor, BGSHeadPart::HeadPartType::kFace);
		SetShaderPropertyAlpha(geometry, 1.0, true);

		SetArmorSkinAlpha(thisActor, BGSBipedObjectForm::FirstPersonFlag::kBody, 1.0);
		SetArmorSkinAlpha(thisActor, BGSBipedObjectForm::FirstPersonFlag::kHands, 1.0);
		SetArmorSkinAlpha(thisActor, BGSBipedObjectForm::FirstPersonFlag::kFeet, 1.0);
		SetArmorSkinAlpha(thisActor, BGSBipedObjectForm::FirstPersonFlag::kTail, 1.0);
		SetArmorSkinAlpha(thisActor, BGSBipedObjectForm::FirstPersonFlag::kDecapitate, 1.0);
	}

	void ResetSkinTint(Actor* thisActor, NiAVObject* root)
	{
		auto actorBase = thisActor->GetActorBase();
		if (actorBase)
		{
			auto player = PlayerCharacter::GetSingleton();
			if (thisActor == player)
			{
				auto faceNode = thisActor->GetFaceNodeSkinned();
				auto facePart = actorBase->GetCurrentHeadPartByType(BGSHeadPart::HeadPartType::kFace);

				if (faceNode && facePart)
				{
					auto singleton = FaceGen::GetSingleton();
					singleton->RegenerateHead(faceNode, facePart, actorBase);
				}
			}

			NiColorA val;
			val.red = actorBase->textureLighting.red / 255.0;
			val.green = actorBase->textureLighting.green / 255.0;
			val.blue = actorBase->textureLighting.blue / 255.0;
			NiColorA* skinColor = &val;

			if (root)
			{
				root->UpdateModelSkin(&skinColor);
			}
		}
	}

	void ResetHairTint(Actor* thisActor, NiAVObject* root)
	{
		auto actorBase = thisActor->GetActorBase();
		if (actorBase)
		{
			auto colorForm = actorBase->headData->hairColor;
			if (colorForm)
			{
				NiColorA val;
				val.red = colorForm->color.red / 255.0;
				val.green = colorForm->color.green / 255.0;
				val.blue = colorForm->color.blue / 255.0;
				NiColorA* hairColor = &val;

				if (root)
				{
					root->UpdateModelHair(&hairColor);
				}
			}
		}
	}

	template <typename V, typename A>
	void FillVMArray(std::vector<V> const& vec, BSScript::VMArray<A>& array)
	{
		auto size = vec.size();
		if (size > 0)
		{
			array.resize(size);

			for (size_t i = 0; i < size; i++)
			{
				array[i] = vec[i];
			}
		}
	}

	template <typename V, typename A>
	void NiPtr_FillVMArray(std::vector<V> const& vec, BSScript::VMArray<A>& array)
	{
		auto size = vec.size();
		if (size > 0)
		{
			array.resize(size);

			for (size_t i = 0; i < size; i++)
			{
				array[i] = vec[i].get();
			}
		}
	}

	//--------------------------------------------------------------------------------------------
	// ACTOR
	//--------------------------------------------------------------------------------------------

	void PO3_SKSEFunctions::GetHairColor(StaticFunctionTag*, Actor* thisActor, BGSColorForm* color)
	{
		if (!thisActor || !color || !thisActor->Is3DLoaded())
		{
			return;
		}

		g_task->AddTask([thisActor, color]()
		{
			auto geometry = GetHeadPartGeometry(thisActor, BGSHeadPart::HeadPartType::kHair);
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

				if (material && material->GetFeature() == BSShaderMaterial::Feature::kHairTint)
				{
					auto tintedMaterial = static_cast<BSLightingShaderMaterialHairTint*>(material);
					NiColor tintColor = tintedMaterial->tintColor;

					color->color.red = tintColor.red * 255;
					color->color.green = tintColor.green * 255;
					color->color.blue = tintColor.blue * 255;
				}
			}
		});
	}

	void PO3_SKSEFunctions::SetHairColor(StaticFunctionTag*, Actor* thisActor, BGSColorForm* color)
	{
		if (!thisActor || !color)
		{
			return;
		}

		auto root = thisActor->Get3D(0);
		if (root)
		{
			NiColorA val;
			val.red = color->color.red / 255.0;
			val.green = color->color.green / 255.0;
			val.blue = color->color.blue / 255.0;
			NiColorA* hairColor = &val;

			root->UpdateModelHair(&hairColor);

			auto data = root->GetExtraData(BSFixedString("PO3_HAIRTINT"));
			if (!data)
			{
				auto newData = NiBooleanExtraData::Create(BSFixedString("PO3_HAIRTINT"), true);
				if (newData)
				{
					root->AddExtraData(newData);
				}
			}
		}
	}

	void PO3_SKSEFunctions::GetSkinColor(StaticFunctionTag*, Actor* thisActor, BGSColorForm* color)
	{
		if (!thisActor || !color || !thisActor->Is3DLoaded())
		{
			return;
		}

		g_task->AddTask([thisActor, color]()
		{
			auto geometry = GetArmorGeometry(thisActor, BGSBipedObjectForm::FirstPersonFlag::kBody);

			if (!geometry)
			{
				geometry = GetArmorGeometry(thisActor, BGSBipedObjectForm::FirstPersonFlag::kHands);
			}

			if (!geometry)
			{
				geometry = GetArmorGeometry(thisActor, BGSBipedObjectForm::FirstPersonFlag::kFeet);
			}

			if (!geometry)
			{
				geometry = GetArmorGeometry(thisActor, BGSBipedObjectForm::FirstPersonFlag::kTail);
			}

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

					if (material && material->GetFeature() == BSShaderMaterial::Feature::kFaceGenRGBTint)
					{
						auto tintedMaterial = static_cast<BSLightingShaderMaterialFacegenTint*>(material);
						NiColor tintColor = tintedMaterial->tintColor;

						color->color.red = tintColor.red * 255;
						color->color.green = tintColor.green * 255;
						color->color.blue = tintColor.blue * 255;
					}
				}
			}
			else
			{
				auto actorBase = thisActor->GetActorBase();
				if (actorBase)
				{
					color->color.red = actorBase->textureLighting.red;
					color->color.green = actorBase->textureLighting.green;
					color->color.blue = actorBase->textureLighting.blue;
				}
			}
		});
	}

	void PO3_SKSEFunctions::SetSkinColor(StaticFunctionTag*, Actor* thisActor, BGSColorForm* color)
	{
		if (!thisActor || !color)
		{
			return;
		}

		auto root = thisActor->Get3D(0);
		if (root)
		{
			g_task->AddTask([thisActor, color, root]()
			{
				auto geometry = GetHeadPartGeometry(thisActor, BGSHeadPart::HeadPartType::kFace);
				SetShaderPropertyRGBTint(geometry);

				NiColorA val;
				val.red = color->color.red / 255.0;
				val.green = color->color.green / 255.0;
				val.blue = color->color.blue / 255.0;
				NiColorA* skinColor = &val;

				if (root)
				{
					root->UpdateModelSkin(&skinColor);
				}
			});

			auto data = root->GetExtraData(BSFixedString("PO3_SKINTINT"));
			if (!data)
			{
				auto newData = NiBooleanExtraData::Create(BSFixedString("PO3_SKINTINT"), true);
				if (newData)
				{
					root->AddExtraData(newData);
				}
			}
		}
	}

	void PO3_SKSEFunctions::MixColorWithSkinTone(StaticFunctionTag*, Actor* thisActor, BGSColorForm* color, bool manualMode, float percentage)
	{
		if (!thisActor || !color)
		{
			return;
		}

		auto root = thisActor->Get3D(0);
		if (root)
		{
			g_task->AddTask([thisActor, color, percentage, manualMode, root]()
			{
				auto actorBase = thisActor->GetActorBase();
				if (actorBase)
				{
					float skinLuminance = manualMode ? percentage : calculateLuminance(actorBase->textureLighting.red, actorBase->textureLighting.green, actorBase->textureLighting.blue);

					UInt8 colorRed = colorMix(color->color.red, actorBase->textureLighting.red, skinLuminance);
					UInt8 colorGreen = colorMix(color->color.green, actorBase->textureLighting.green, skinLuminance);
					UInt8 colorBlue = colorMix(color->color.blue, actorBase->textureLighting.blue, skinLuminance);

					NiColorA val;
					val.red = colorRed / 255.0;
					val.green = colorGreen / 255.0;
					val.blue = colorBlue / 255.0;
					NiColorA* multipliedColor = &val;

					auto geometry = GetHeadPartGeometry(thisActor, BGSHeadPart::HeadPartType::kFace);
					SetShaderPropertyRGBTint(geometry); //makes face tintable

					if (root)
					{
						root->UpdateModelSkin(&multipliedColor);
					}
				}
			});

			auto data = root->GetExtraData(BSFixedString("PO3_SKINTINT"));
			if (!data)
			{
				auto newData = NiBooleanExtraData::Create(BSFixedString("PO3_SKINTINT"), true);
				if (newData)
				{
					root->AddExtraData(newData);
				}
			}
		}
	}

	void PO3_SKSEFunctions::SetSkinAlpha(StaticFunctionTag*, Actor* thisActor, float alpha)
	{
		if (!thisActor || !thisActor->Is3DLoaded())
		{
			return;
		}

		g_task->AddTask([thisActor, alpha]()
		{
			auto geometry = GetHeadPartGeometry(thisActor, BGSHeadPart::HeadPartType::kFace);
			SetShaderPropertyAlpha(geometry, alpha, true);

			SetArmorSkinAlpha(thisActor, BGSBipedObjectForm::FirstPersonFlag::kBody, alpha);
			SetArmorSkinAlpha(thisActor, BGSBipedObjectForm::FirstPersonFlag::kHands, alpha);
			SetArmorSkinAlpha(thisActor, BGSBipedObjectForm::FirstPersonFlag::kFeet, alpha);
			SetArmorSkinAlpha(thisActor, BGSBipedObjectForm::FirstPersonFlag::kTail, alpha); //tail
			SetArmorSkinAlpha(thisActor, BGSBipedObjectForm::FirstPersonFlag::kDecapitateHead, alpha); //decap
		});

		auto root = thisActor->Get3D(0);
		if (root)
		{
			auto data = static_cast<NiFloatExtraData*>(root->GetExtraData(BSFixedString("PO3_ALPHA")));
			if (data)
			{
				if (alpha == 1.0)
				{
					root->RemoveExtraData(data);
				}
			}
			else
			{
				auto newData = NiFloatExtraData::Create(BSFixedString("PO3_ALPHA"), alpha);
				if (newData)
				{
					root->AddExtraData(newData);
				}
			}
		}
	}

	void PO3_SKSEFunctions::EquipArmorIfSkinVisible(StaticFunctionTag*, Actor* thisActor, TESObjectARMO* armorToCheck, TESObjectARMO* armorToEquip)
	{
		if (!thisActor || !armorToCheck || !armorToEquip || !thisActor->Is3DLoaded())
		{
			return;
		}

		g_task->AddTask([thisActor, armorToCheck, armorToEquip]()
		{
			for (auto& arma : armorToCheck->armature)
			{
				if (arma)
				{
					NiAVObject* armorObject = VisitArmorAddon(thisActor, armorToCheck, arma);
					if (armorObject)
					{
						if (HasShaderType(armorObject, BSShaderMaterial::Feature::kFaceGenRGBTint))
						{
							thisActor->AddWornItem(armorToEquip, 1, false, 0, 0);
							return;
						}
					}
				}
			}
		});
	}

	void PO3_SKSEFunctions::ReplaceArmorTextureSet(StaticFunctionTag*, Actor* thisActor, TESObjectARMO* thisArmor, BGSTextureSet* sourceTXST, BGSTextureSet* targetTXST, SInt32 textureType)
	{
		if (!thisActor || !thisArmor || !sourceTXST || !targetTXST || !thisActor->Is3DLoaded())
		{
			return;
		}

		std::string targetPath = sourceTXST->GetTexturePath(BSShaderTextureSet::Texture::kDiffuse);
		SanitizePath(targetPath);

		g_task->AddTask([thisActor, thisArmor, sourceTXST, targetTXST, textureType, targetPath]()
		{
			for (auto& armorAddon : thisArmor->armature)
			{
				if (armorAddon)
				{
					NiAVObject* armorObject = VisitArmorAddon(thisActor, thisArmor, armorAddon);
					if (armorObject)
					{
						bool replaced = false;

						auto node = armorObject->GetAsNiNode();
						if (node)
						{
							for (UInt32 i = 0; i < node->children.size(); i++)
							{
								auto object = node->children[i];

								if (object && ReplaceTextureSet(object->GetAsBSGeometry(), *sourceTXST, *targetTXST, textureType, targetPath))
								{
									replaced = true;
								}
							}
						}
						else
						{
							replaced = ReplaceTextureSet(armorObject->GetAsBSGeometry(), *sourceTXST, *targetTXST, textureType, targetPath);
						}

						auto root = thisActor->Get3D(0);
						if (replaced && root)
						{
							auto armorID = std::to_string(thisArmor->formID);
							std::string name = "PO3_TXST - " + armorID;

							auto data = static_cast<NiStringsExtraData*>(root->GetExtraData(BSFixedString(name.c_str())));
							if (!data)
							{
								std::vector<BSFixedString> vec;
								vec.reserve(10);
								for (auto i = BSShaderTextureSet::Texture::kDiffuse; i < BSShaderTextureSet::Texture::kTotal; i++)
								{
									vec.emplace_back(sourceTXST->GetTexturePath(i));
								}
								vec.emplace_back(armorID.c_str());

								auto newData = NiStringsExtraData::Create(BSFixedString(name.c_str()), vec.data(), vec.size());
								if (newData)
								{
									root->AddExtraData(newData);
								}
							}
						}
					}
				}
			}
		});
	}

	void PO3_SKSEFunctions::ReplaceSkinTextureSet(StaticFunctionTag*, Actor* thisActor, BGSTextureSet* maleTXST, BGSTextureSet* femaleTXST, UInt32 slotMask, SInt32 textureType)
	{
		if (!thisActor || !thisActor->Is3DLoaded())
		{
			return;
		}

		bool isFemale = false;

		auto actorBase = thisActor->GetActorBase();
		if (actorBase)
		{
			isFemale = actorBase->GetSex() == TESNPC::Sex::kFemale ? true : false;
		}

		if (isFemale)
		{
			if (!femaleTXST)
			{
				return;
			}

			SetArmorSkinTXST(thisActor, femaleTXST, static_cast<BGSBipedObjectForm::BipedObjectSlot>(slotMask), textureType);
		}
		else
		{
			if (!maleTXST)
			{
				return;
			}

			SetArmorSkinTXST(thisActor, maleTXST, static_cast<BGSBipedObjectForm::BipedObjectSlot>(slotMask), textureType);
		}
	}

	void PO3_SKSEFunctions::ReplaceFaceTextureSet(StaticFunctionTag*, Actor* thisActor, BGSTextureSet* maleTXST, BGSTextureSet* femaleTXST, SInt32 textureType)
	{
		if (!thisActor || !thisActor->Is3DLoaded())
		{
			return;
		}

		bool isFemale = false;

		auto actorBase = thisActor->GetActorBase();
		if (actorBase)
		{
			isFemale = actorBase->GetSex() == TESNPC::Sex::kFemale ? true : false;
		}

		g_task->AddTask([thisActor, maleTXST, femaleTXST, textureType, isFemale]()
		{
			auto faceGeometry = GetHeadPartGeometry(thisActor, BGSHeadPart::HeadPartType::kFace);
			if (faceGeometry)
			{
				bool replaced = false;

				std::vector<BSFixedString> vec;
				vec.reserve(9);

				if (isFemale)
				{
					if (!femaleTXST)
					{
						return;
					}

					replaced = ReplaceSkinTXST(faceGeometry, *femaleTXST, vec, textureType);
				}
				else
				{
					if (!maleTXST)
					{
						return;
					}

					replaced = ReplaceSkinTXST(faceGeometry, *maleTXST, vec, textureType);
				}

				auto root = thisActor->Get3D(0);
				if (replaced && root)
				{
					auto data = static_cast<NiStringsExtraData*>(root->GetExtraData(BSFixedString("PO3_FACETXST")));
					if (!data)
					{
						auto newData = NiStringsExtraData::Create(BSFixedString("PO3_FACETXST"), vec.data(), vec.size());
						if (newData)
						{
							root->AddExtraData(newData);
						}
					}
				}
			}
		});
	}

	BGSTextureSet* PO3_SKSEFunctions::GetHeadPartTextureSet(StaticFunctionTag*, Actor* thisActor, UInt32 type)
	{
		if (!thisActor || !thisActor->Is3DLoaded())
		{
			return nullptr;
		}

		auto actorBase = thisActor->GetActorBase();
		if (actorBase)
		{
			BGSHeadPart* headpart = actorBase->GetCurrentHeadPartByType(static_cast<BGSHeadPart::HeadPartType>(type));
			if (headpart)
			{
				return headpart->textureSet;
			}
		}

		return nullptr;
	}

	void PO3_SKSEFunctions::SetHeadPartTextureSet(StaticFunctionTag*, Actor* thisActor, BGSTextureSet* headpartTXST, UInt32 type)
	{
		if (!thisActor || !headpartTXST || !thisActor->Is3DLoaded())
		{
			return;
		}

		auto actorBase = thisActor->GetActorBase();
		if (actorBase)
		{
			BGSHeadPart* headpart = actorBase->GetCurrentHeadPartByType(static_cast<BGSHeadPart::HeadPartType>(type));
			if (headpart)
			{
				headpart->textureSet = headpartTXST;
			}
		}
	}

	void PO3_SKSEFunctions::SetHeadPartAlpha(StaticFunctionTag*, Actor* thisActor, UInt32 partType, float alpha)
	{
		if (!thisActor || !thisActor->Is3DLoaded())
		{
			return;
		}

		g_task->AddTask([thisActor, partType, alpha]()
		{
			auto geometry = GetHeadPartGeometry(thisActor, static_cast<BGSHeadPart::HeadPartType>(partType));
			if (geometry)
			{
				if (alpha == 0.0)
				{
					geometry->flags |= NiAVObject::Flag::kHidden;
					SetShaderPropertyAlpha(geometry, 0.0, false);
				}
				else
				{
					SetShaderPropertyAlpha(geometry, alpha, false);
				}
			}
		});

		auto root = thisActor->Get3D(0);
		if (root)
		{
			auto data = static_cast<NiIntegersExtraData*>(root->GetExtraData(BSFixedString("PO3_HEADPARTALPHA")));
			if (!data)
			{
				if (alpha == 0.0)
				{
					std::vector<SInt32> vec;
					vec.reserve(1);
					vec.emplace_back(partType);

					auto newData = NiIntegersExtraData::Create(BSFixedString("PO3_HEADPARTALPHA"), vec.data(), vec.size());
					if (newData)
					{
						root->AddExtraData(newData);
					}
				}
			}
			else
			{
				alpha == 0.0 ? data->InsertElement(partType) : data->RemoveElement(partType);
			}
		}
	}

	void PO3_SKSEFunctions::ToggleChildNode(StaticFunctionTag*, Actor* thisActor, BSFixedString nodeName, bool disable)
	{
		if (!thisActor)
		{
			return;
		}

		auto root = thisActor->Get3D(0);
		if (!root)
		{
			return;
		}

		auto child = root->GetObjectByName(nodeName);
		if (!child)
		{
			return;
		}

		g_task->AddTask([child, disable]()
		{
			auto node = child->GetAsNiNode();
			if (node)
			{
				for (UInt32 i = 0; i < node->children.size(); i++)
				{
					auto object = node->children[i];
					if (object)
					{
						disable == false ? object->flags &= ~NiAVObject::Flag::kHidden : object->flags |= NiAVObject::Flag::kHidden;
					}
				}

				disable == false ? node->flags &= ~NiAVObject::Flag::kHidden : node->flags |= NiAVObject::Flag::kHidden;
			}

			disable == false ? child->flags &= ~NiAVObject::Flag::kHidden : child->flags |= NiAVObject::Flag::kHidden;
		});

		auto data = static_cast<NiStringsExtraData*>(root->GetExtraData(BSFixedString("PO3_TOGGLE")));
		if (!data)
		{
			if (disable)
			{
				std::vector<BSFixedString> vec;
				vec.reserve(1);
				vec.emplace_back(nodeName);

				auto newData = NiStringsExtraData::Create("PO3_TOGGLE", vec.data(), 1);
				if (newData)
				{
					root->AddExtraData(newData);
				}
			}
		}
		else
		{
			disable == true ? data->InsertElement(nodeName.c_str()) : data->RemoveElement(nodeName.c_str());
		}
	}

	void PO3_SKSEFunctions::RemoveChildNode(StaticFunctionTag*, Actor* thisActor, BSFixedString nodeName)
	{
		if (!thisActor)
		{
			return;
		}

		auto root = thisActor->Get3D(0)->GetAsNiNode();
		if (!root)
		{
			return;
		}

		auto child = root->GetObjectByName(nodeName);
		if (child)
		{
			g_task->AddTask([root, child]()
			{
				root->RemoveChild(child);
			});
		}
	}

	bool PO3_SKSEFunctions::IsActorSoulTrapped(StaticFunctionTag*, Actor* thisActor)
	{
		if (!thisActor || !soulTrapArt)
		{
			return false;
		}

		bool isNPC = false;
		if (thisActor->HasKeywordHelper(npcKeyword))
		{
			isNPC = true;
		}

		Actor* thisCaster = nullptr;
		bool hasSoulTrap = false;

		auto activeEffects = thisActor->GetActiveEffectList();
		if (activeEffects)
		{
			for (auto& ae : *activeEffects)
			{
				if (!ae || !ae->effect || !ae->effect->baseEffect)
				{
					continue;
				}

				auto mgef = ae->effect->baseEffect;
				if (mgef)
				{
					if (mgef->data.hitEffectArt != soulTrapArt && mgef->data.archetype != EffectArchetypes::ArchetypeID::kSoulTrap) //only soultrap
					{
						continue;
					}

					thisCaster = ae->GetCasterActor();
					if (!thisCaster)
					{
						return false;
					}

					hasSoulTrap = true;
					break;
				}
			}
		}

		if (hasSoulTrap)
		{
			auto exChanges = static_cast<ExtraContainerChanges*>(thisCaster->extraData.GetByType(ExtraDataType::kContainerChanges)); //loop through caster inventory
			InventoryChanges* changes = exChanges ? exChanges->changes : nullptr;

			if (changes && changes->entryList)
			{
				for (InventoryEntryData* data : *changes->entryList)
				{
					if (!data)
					{
						continue;
					}

					auto object = data->object;

					if (!object || !object->IsSoulGem())
					{
						continue;
					}

					auto thisSoulGem = skyrim_cast<TESSoulGem*>(object); //get soulgem
					if (thisSoulGem)
					{
						SoulLevel soulGemSize = GetSoulLevel(thisSoulGem, data);
						if (soulGemSize == SoulLevel::kNone)
						{
							if (isNPC)
							{
								if (thisSoulGem->formFlags & TESSoulGem::RecordFlags::kCanHoldNPCSoul)
								{
									return true;
								}
							}
							else
							{
								UInt8 actorSoulSize = thisActor->GetSoulSize();
								if (thisSoulGem->maximumCapacity >= static_cast<SoulLevel>(actorSoulSize))
								{
									return true;
								}
							}
						}
					}
				}
			}
		}

		return false;
	}

	BSScript::VMArray<TESForm*> PO3_SKSEFunctions::AddAllEquippedItemsToArray(StaticFunctionTag*, Actor* thisActor)
	{
		BSScript::VMArray<TESForm*> result;
		std::vector<TESForm*> vec;

		if (thisActor)
		{
			auto exChanges = static_cast<ExtraContainerChanges*>(thisActor->extraData.GetByType(ExtraDataType::kContainerChanges)); //loop through caster inventory
			InventoryChanges* changes = exChanges ? exChanges->changes : nullptr;

			if (changes && changes->entryList)
			{
				for (InventoryEntryData* data : *changes->entryList)
				{
					if (data && data->extraLists)
					{
						auto object = data->object;
						if (!object)
						{
							continue;
						}

						for (auto& extraList : *data->extraLists)
						{
							if (extraList && (extraList->HasType(ExtraDataType::kWorn) || extraList->HasType(ExtraDataType::kWornLeft)))
							{
								vec.push_back(object);
							}
						}
					}
				}
			}
		}

		FillVMArray(vec, result);
		return result;
	}

	bool PO3_SKSEFunctions::ResetActor3D(StaticFunctionTag*, Actor* thisActor)
	{
		if (thisActor)
		{
			auto root = thisActor->Get3D(0);
			if (!root)
			{
				return false;
			}

			auto t = root->GetPO3ExtraData<NiStringsExtraData*, NiBooleanExtraData*, NiFloatExtraData*, NiIntegersExtraData*>();

			auto toggleData = std::get<0>(t);
			auto txstFaceData = std::get<1>(t);

			auto skinTintData = std::get<2>(t);
			auto hairTintData = std::get<3>(t);

			auto alphaData = std::get<4>(t);
			auto headpartAlphaData = std::get<5>(t);

			auto txstVec = std::get<6>(t);
			auto txstSkinVec = std::get<7>(t);

			if (!toggleData && !alphaData && !headpartAlphaData && !skinTintData && !hairTintData && !txstFaceData && txstVec.empty() && txstSkinVec.empty())
			{
				return false;
			}

			auto player = PlayerCharacter::GetSingleton();
			if (thisActor != player)
			{
				StopAllShaders_Internal(thisActor);
			}

			if (toggleData)
			{
				g_task->AddTask([thisActor, root, toggleData]()
				{
					for (UInt32 i = 0; i < toggleData->size; i++)
					{
						auto object = root->GetObjectByName(BSFixedString(toggleData->value[i]));
						if (object)
						{
							auto node = object->GetAsNiNode();
							if (node)
							{
								for (UInt32 k = 0; k < node->children.size(); k++)
								{
									auto child = node->children[k];
									if (child)
									{
										child->flags &= ~NiAVObject::Flag::kHidden;
									}
								}
								node->flags &= ~NiAVObject::Flag::kHidden;
							}
							object->flags &= ~NiAVObject::Flag::kHidden;
						}
					}

					root->RemoveExtraData(toggleData);
				});
			}
			if (alphaData)
			{
				if (thisActor != player)
				{
					thisActor->ResetInventory(false);
				}
				g_task->AddTask([thisActor, root, alphaData]()
				{
					ResetAlpha(thisActor);

					root->RemoveExtraData(alphaData);
				});
			}
			if (headpartAlphaData)
			{
				g_task->AddTask([thisActor, root, headpartAlphaData]()
				{
					for (UInt32 i = 0; i < headpartAlphaData->size; i++)
					{
						auto type = static_cast<BGSHeadPart::HeadPartType>(headpartAlphaData->value[i]);

						auto geometry = GetHeadPartGeometry(thisActor, type);
						if (geometry)
						{
							geometry->flags &= ~(NiAVObject::Flag::kHidden);
							SetShaderPropertyAlpha(geometry, 1.0, false);
						}
					}

					root->RemoveExtraData(headpartAlphaData);
				});
			}
			if (skinTintData)
			{
				g_task->AddTask([thisActor, root, skinTintData]()
				{
					ResetSkinTint(thisActor, root);

					root->RemoveExtraData(skinTintData);
				});
			}
			if (hairTintData)
			{
				g_task->AddTask([thisActor, root, hairTintData]()
				{
					ResetHairTint(thisActor, root);

					root->RemoveExtraData(hairTintData);
				});
			}
			if (txstFaceData)
			{
				g_task->AddTask([thisActor, root, txstFaceData]()
				{
					auto textureset = BSShaderTextureSet::Create();
					for (auto i = BSTextureSet::Texture::kDiffuse; i < BSTextureSet::Textures::kTotal; ++i)
					{
						if (txstFaceData->value[i] && *txstFaceData->value[i] != '\0')
						{
							textureset->SetTexturePath(i, txstFaceData->value[i]);
						}
					}

					auto headGeometry = GetHeadPartGeometry(thisActor, BGSHeadPart::HeadPartType::kFace);
					ResetTextureSet(headGeometry, textureset, true);

					root->RemoveExtraData(txstFaceData);
				});
			}
			if (!txstSkinVec.empty())
			{
				g_task->AddTask([thisActor, root, txstSkinVec]()
				{
					for (auto& txstData : txstSkinVec)
					{
						if (!txstData)
						{
							continue;
						}

						std::string slotMaskstr = txstData->value[txstData->size - 1];
						if (slotMaskstr.empty())
						{
							continue;
						}

						auto slotMask = static_cast<BGSBipedObjectForm::FirstPersonFlag>(std::stoul(slotMaskstr));

						TESObjectARMO* skinarmor = GetSkinForm(thisActor, slotMask);
						if (!skinarmor)
						{
							continue;
						}

						auto textureset = BSShaderTextureSet::Create();
						for (auto i = BSTextureSet::Texture::kDiffuse; i < BSTextureSet::Textures::kTotal; ++i)
						{
							if (txstData->value[i] && *txstData->value[i] != '\0')
							{
								textureset->SetTexturePath(i, txstData->value[i]);
							}
						}

						TESObjectARMA* foundAddon = GetArmorAddonByMask(thisActor->race, skinarmor, slotMask);
						if (foundAddon)
						{
							auto armorObject = VisitArmorAddon(thisActor, skinarmor, foundAddon);
							if (armorObject)
							{
								auto node = armorObject->GetAsNiNode();
								if (node)
								{
									for (UInt32 i = 0; i < node->children.size(); i++)
									{
										auto object = node->children[i];
										if (object)
										{
											ResetTextureSet(object->GetAsBSGeometry(), textureset, true);
										}
									}
								}
								else
								{
									ResetTextureSet(armorObject->GetAsBSGeometry(), textureset, true);
								}
							}
						}

						root->RemoveExtraData(txstData);
					}
				});
			}
			if (!txstVec.empty())
			{
				g_task->AddTask([thisActor, root, txstVec]()
				{
					for (auto& txstData : txstVec)
					{
						if (!txstData)
						{
							continue;
						}

						std::string armorID = txstData->value[txstData->size - 1];

						if (armorID.empty())
						{
							continue;
						}

						auto ID = static_cast<FormID>(std::stoul(armorID));

						TESObjectARMO* armor = GetWornFormByID(thisActor, ID);
						if (!armor)
						{
							continue;
						}

						auto textureset = BSShaderTextureSet::Create();
						for (auto i = BSTextureSet::Texture::kDiffuse; i < BSTextureSet::Textures::kTotal; ++i)
						{
							if (txstData->value[i] && *txstData->value[i] != '\0')
							{
								textureset->SetTexturePath(i, txstData->value[i]);
							}
						}

						for (auto& armorAddon : armor->armature)
						{
							if (armorAddon)
							{
								NiAVObject* armorObject = VisitArmorAddon(thisActor, armor, armorAddon);

								if (armorObject)
								{
									auto node = armorObject->GetAsNiNode();
									if (node)
									{
										for (UInt32 i = 0; i < node->children.size(); i++)
										{
											auto object = node->children[i];
											if (object)
											{
												ResetTextureSet(object->GetAsBSGeometry(), textureset, false);
											}
										}
									}
									else
									{
										ResetTextureSet(armorObject->GetAsBSGeometry(), textureset, false);
									}
								}
							}
						}

						root->RemoveExtraData(txstData);
					}
				});
			}

			return true;
		}

		return false;
	}

	void PO3_SKSEFunctions::RemoveEffectsNotOfType(StaticFunctionTag*, Actor* thisActor, UInt32 effectType)
	{
		if (thisActor)
		{
			auto root = thisActor->Get3D(0);
			if (!root)
			{
				return;
			}

			auto t = root->GetPO3ExtraData<NiStringsExtraData*, NiBooleanExtraData*, NiFloatExtraData*, NiIntegersExtraData*>();

			auto toggleData = std::get<0>(t);
			auto txstFaceData = std::get<1>(t);
			auto skinTintData = std::get<2>(t);
			auto hairTintData = std::get<3>(t);
			auto alphaData = std::get<4>(t);
			auto headpartAlphaData = std::get<5>(t);
			auto txstSkinVec = std::get<7>(t);

			auto player = PlayerCharacter::GetSingleton();

			if (effectType == 0)		//Charred/Skeletonized
			{
				if (skinTintData)
				{
					g_task->AddTask([thisActor, root, skinTintData]()
					{
						ResetSkinTint(thisActor, root);

						root->RemoveExtraData(skinTintData);
					});
				}
				if (hairTintData)
				{
					g_task->AddTask([thisActor, root, hairTintData]()
					{
						ResetHairTint(thisActor, root);

						root->RemoveExtraData(hairTintData);
					});
				}
				if (txstFaceData)
				{
					g_task->AddTask([thisActor, root, txstFaceData]()
					{
						auto textureset = BSShaderTextureSet::Create();
						for (auto i = BSTextureSet::Texture::kDiffuse; i < BSTextureSet::Textures::kTotal; ++i)
						{
							if (txstFaceData->value[i] && *txstFaceData->value[i] != '\0')
							{
								textureset->SetTexturePath(i, txstFaceData->value[i]);
							}
						}

						auto headGeometry = GetHeadPartGeometry(thisActor, BGSHeadPart::HeadPartType::kFace);
						ResetTextureSet(headGeometry, textureset, true);

						root->RemoveExtraData(txstFaceData);
					});
				}
				if (!txstSkinVec.empty())
				{
					g_task->AddTask([thisActor, root, txstSkinVec]()
					{
						for (auto& txstData : txstSkinVec)
						{
							if (!txstData)
							{
								continue;
							}

							std::string slotMaskstr = txstData->value[txstData->size - 1];
							if (slotMaskstr.empty())
							{
								continue;
							}

							auto slotMask = static_cast<BGSBipedObjectForm::FirstPersonFlag>(std::stoul(slotMaskstr));

							TESObjectARMO* skinarmor = GetSkinForm(thisActor, slotMask);
							if (!skinarmor)
							{
								continue;
							}

							auto textureset = BSShaderTextureSet::Create();
							for (auto i = BSTextureSet::Texture::kDiffuse; i < BSTextureSet::Textures::kTotal; ++i)
							{
								if (txstData->value[i] && *txstData->value[i] != '\0')
								{
									textureset->SetTexturePath(i, txstData->value[i]);
								}
							}

							TESObjectARMA* foundAddon = GetArmorAddonByMask(thisActor->race, skinarmor, slotMask);
							if (foundAddon)
							{
								auto armorObject = VisitArmorAddon(thisActor, skinarmor, foundAddon);
								if (armorObject)
								{
									auto node = armorObject->GetAsNiNode();
									if (node)
									{
										for (UInt32 i = 0; i < node->children.size(); i++)
										{
											auto object = node->children[i];
											if (object)
											{
												ResetTextureSet(object->GetAsBSGeometry(), textureset, true);
											}
										}
									}
									else
									{
										ResetTextureSet(armorObject->GetAsBSGeometry(), textureset, true);
									}
								}
							}

							root->RemoveExtraData(txstData);
						}
					});
				}
			}
			else if (effectType == 1)	//Drained
			{
				if (toggleData)
				{
					g_task->AddTask([thisActor, root, toggleData]()
					{
						for (UInt32 i = 0; i < toggleData->size; i++)
						{
							auto object = root->GetObjectByName(BSFixedString(toggleData->value[i]));
							if (object)
							{
								auto node = object->GetAsNiNode();
								if (node)
								{
									for (UInt32 k = 0; k < node->children.size(); k++)
									{
										auto child = node->children[k];
										if (child)
										{
											child->flags &= ~NiAVObject::Flag::kHidden;
										}
									}
									node->flags &= ~NiAVObject::Flag::kHidden;
								}
								object->flags &= ~NiAVObject::Flag::kHidden;
							}
						}

						root->RemoveExtraData(toggleData);
					});
				}
				if (skinTintData)
				{
					g_task->AddTask([thisActor, root, skinTintData]()
					{
						ResetSkinTint(thisActor, root);

						root->RemoveExtraData(skinTintData);
					});
				}
				if (hairTintData)
				{
					g_task->AddTask([thisActor, root, hairTintData]()
					{
						ResetHairTint(thisActor, root);

						root->RemoveExtraData(hairTintData);
					});
				}
				if (txstFaceData)
				{
					g_task->AddTask([thisActor, root, txstFaceData]()
					{
						auto textureset = BSShaderTextureSet::Create();
						for (auto i = BSTextureSet::Texture::kDiffuse; i < BSTextureSet::Textures::kTotal; ++i)
						{
							if (txstFaceData->value[i] && *txstFaceData->value[i] != '\0')
							{
								textureset->SetTexturePath(i, txstFaceData->value[i]);
							}
						}

						auto headGeometry = GetHeadPartGeometry(thisActor, BGSHeadPart::HeadPartType::kFace);
						ResetTextureSet(headGeometry, textureset, true);

						root->RemoveExtraData(txstFaceData);
					});
				}
				if (!txstSkinVec.empty())
				{
					g_task->AddTask([thisActor, root, txstSkinVec]()
					{
						for (auto& txstData : txstSkinVec)
						{
							if (!txstData)
							{
								continue;
							}

							std::string slotMaskstr = txstData->value[txstData->size - 1];
							if (slotMaskstr.empty())
							{
								continue;
							}

							auto slotMask = static_cast<BGSBipedObjectForm::FirstPersonFlag>(std::stoul(slotMaskstr));

							TESObjectARMO* skinarmor = GetSkinForm(thisActor, slotMask);
							if (!skinarmor)
							{
								continue;
							}

							auto textureset = BSShaderTextureSet::Create();
							for (auto i = BSTextureSet::Texture::kDiffuse; i < BSTextureSet::Textures::kTotal; ++i)
							{
								if (txstData->value[i] && *txstData->value[i] != '\0')
								{
									textureset->SetTexturePath(i, txstData->value[i]);
								}
							}

							TESObjectARMA* foundAddon = GetArmorAddonByMask(thisActor->race, skinarmor, slotMask);
							if (foundAddon)
							{
								auto armorObject = VisitArmorAddon(thisActor, skinarmor, foundAddon);
								if (armorObject)
								{
									auto node = armorObject->GetAsNiNode();
									if (node)
									{
										for (UInt32 i = 0; i < node->children.size(); i++)
										{
											auto object = node->children[i];
											if (object)
											{
												ResetTextureSet(object->GetAsBSGeometry(), textureset, true);
											}
										}
									}
									else
									{
										ResetTextureSet(armorObject->GetAsBSGeometry(), textureset, true);
									}
								}
							}

							root->RemoveExtraData(txstData);
						}
					});
				}
			}
			else if (effectType == 2)	//Poisoned/Frightened
			{
				if (thisActor != player)
				{
					StopAllSkinAlphaShaders(thisActor);
				}

				if (toggleData)
				{
					g_task->AddTask([thisActor, root, toggleData]()
					{
						for (UInt32 i = 0; i < toggleData->size; i++)
						{
							auto object = root->GetObjectByName(BSFixedString(toggleData->value[i]));
							if (object)
							{
								auto node = object->GetAsNiNode();
								if (node)
								{
									for (UInt32 k = 0; k < node->children.size(); k++)
									{
										auto child = node->children[k];
										if (child)
										{
											child->flags &= ~NiAVObject::Flag::kHidden;
										}
									}
									node->flags &= ~NiAVObject::Flag::kHidden;
								}
								object->flags &= ~NiAVObject::Flag::kHidden;
							}
						}

						root->RemoveExtraData(toggleData);
					});
				}
				if (alphaData)
				{
					if (thisActor != player)
					{
						thisActor->ResetInventory(false);
					}
					g_task->AddTask([thisActor, root, alphaData]()
					{
						ResetAlpha(thisActor);

						root->RemoveExtraData(alphaData);
					});
				}
				if (headpartAlphaData)
				{
					g_task->AddTask([thisActor, root, headpartAlphaData]()
					{
						for (UInt32 i = 0; i < headpartAlphaData->size; i++)
						{
							auto type = static_cast<BGSHeadPart::HeadPartType>(headpartAlphaData->value[i]);

							auto geometry = GetHeadPartGeometry(thisActor, type);
							if (geometry)
							{
								geometry->flags &= ~(NiAVObject::Flag::kHidden);
								SetShaderPropertyAlpha(geometry, 1.0, false);
							}
						}

						root->RemoveExtraData(headpartAlphaData);
					});
				}
				if (txstFaceData)
				{
					g_task->AddTask([thisActor, root, txstFaceData]()
					{
						auto textureset = BSShaderTextureSet::Create();
						for (auto i = BSTextureSet::Texture::kDiffuse; i < BSTextureSet::Textures::kTotal; ++i)
						{
							if (txstFaceData->value[i] && *txstFaceData->value[i] != '\0')
							{
								textureset->SetTexturePath(i, txstFaceData->value[i]);
							}
						}

						auto headGeometry = GetHeadPartGeometry(thisActor, BGSHeadPart::HeadPartType::kFace);
						ResetTextureSet(headGeometry, textureset, true);

						root->RemoveExtraData(txstFaceData);
					});
				}
				if (!txstSkinVec.empty())
				{
					g_task->AddTask([thisActor, root, txstSkinVec]()
					{
						for (auto& txstData : txstSkinVec)
						{
							if (!txstData)
							{
								continue;
							}

							std::string slotMaskstr = txstData->value[txstData->size - 1];
							if (slotMaskstr.empty())
							{
								continue;
							}

							auto slotMask = static_cast<BGSBipedObjectForm::FirstPersonFlag>(std::stoul(slotMaskstr));

							TESObjectARMO* skinarmor = GetSkinForm(thisActor, slotMask);
							if (!skinarmor)
							{
								continue;
							}

							auto textureset = BSShaderTextureSet::Create();
							for (auto i = BSTextureSet::Texture::kDiffuse; i < BSTextureSet::Textures::kTotal; ++i)
							{
								if (txstData->value[i] && *txstData->value[i] != '\0')
								{
									textureset->SetTexturePath(i, txstData->value[i]);
								}
							}

							TESObjectARMA* foundAddon = GetArmorAddonByMask(thisActor->race, skinarmor, slotMask);
							if (foundAddon)
							{
								auto armorObject = VisitArmorAddon(thisActor, skinarmor, foundAddon);
								if (armorObject)
								{
									auto node = armorObject->GetAsNiNode();
									if (node)
									{
										for (UInt32 i = 0; i < node->children.size(); i++)
										{
											auto object = node->children[i];
											if (object)
											{
												ResetTextureSet(object->GetAsBSGeometry(), textureset, true);
											}
										}
									}
									else
									{
										ResetTextureSet(armorObject->GetAsBSGeometry(), textureset, true);
									}
								}
							}

							root->RemoveExtraData(txstData);
						}
					});
				}
			}
			else if (effectType == 3)	//Aged
			{
				if (thisActor != player)
				{
					StopAllSkinAlphaShaders(thisActor);
				}

				if (toggleData)
				{
					g_task->AddTask([thisActor, root, toggleData]()
					{
						for (UInt32 i = 0; i < toggleData->size; i++)
						{
							auto object = root->GetObjectByName(BSFixedString(toggleData->value[i]));
							if (object)
							{
								auto node = object->GetAsNiNode();
								if (node)
								{
									for (UInt32 k = 0; k < node->children.size(); k++)
									{
										auto child = node->children[k];
										if (child)
										{
											child->flags &= ~NiAVObject::Flag::kHidden;
										}
									}
									node->flags &= ~NiAVObject::Flag::kHidden;
								}
								object->flags &= ~NiAVObject::Flag::kHidden;
							}
						}

						root->RemoveExtraData(toggleData);
					});
				}
				if (alphaData)
				{
					if (thisActor != player)
					{
						thisActor->ResetInventory(false);
					}
					g_task->AddTask([thisActor, root, alphaData]()
					{
						ResetAlpha(thisActor);

						root->RemoveExtraData(alphaData);
					});
				}
				if (headpartAlphaData)
				{
					g_task->AddTask([thisActor, root, headpartAlphaData]()
					{
						for (UInt32 i = 0; i < headpartAlphaData->size; i++)
						{
							auto type = static_cast<BGSHeadPart::HeadPartType>(headpartAlphaData->value[i]);

							auto geometry = GetHeadPartGeometry(thisActor, type);
							if (geometry)
							{
								geometry->flags &= ~(NiAVObject::Flag::kHidden);
								SetShaderPropertyAlpha(geometry, 1.0, false);
							}
						}

						root->RemoveExtraData(headpartAlphaData);
					});
				}
			}
		}
	}

	void PO3_SKSEFunctions::DecapitateActor(StaticFunctionTag*, Actor* thisActor)
	{
		if (thisActor)
		{
			thisActor->Decapitate();
		}
	}

	//gets actor's time of death
	float PO3_SKSEFunctions::GetTimeDead(StaticFunctionTag*, Actor* thisActor)
	{
		if (thisActor && thisActor->currentProcess)
		{
			float timeOfDeath = thisActor->currentProcess->deathTime;
			if (timeOfDeath > 0.0)
			{
				auto g_gameDaysPassed = Calendar::GetSingleton()->daysPassed;
				if (g_gameDaysPassed)
				{
					return floorf(g_gameDaysPassed->value * 24.0) - timeOfDeath;
				}
			}
		}

		return 0.0;
	}

	//gets actor's time of death
	float PO3_SKSEFunctions::GetTimeOfDeath(StaticFunctionTag*, Actor* thisActor)
	{
		if (thisActor && thisActor->currentProcess)
		{
			float timeOfDeath = thisActor->currentProcess->deathTime;
			if (timeOfDeath > 0.0)
			{
				return timeOfDeath / 24.0;
			}
		}

		return 0.0;
	}

	//gets actor's current package
	TESPackage* PO3_SKSEFunctions::GetRunningPackage(StaticFunctionTag*, Actor* thisActor)
	{
		return thisActor ? thisActor->GetCurrentPackageInternal() : nullptr;
	}

	bool PO3_SKSEFunctions::IsActorInWater(StaticFunctionTag*, Actor* thisActor)
	{
		return thisActor ? (thisActor->boolBits & Actor::BOOL_BITS::kInWater) != Actor::BOOL_BITS::kNone : false;
	}

	float PO3_SKSEFunctions::GetActorAlpha(StaticFunctionTag*, Actor* thisActor)
	{
		if (thisActor)
		{
			auto currentProcess = thisActor->currentProcess;
			if (currentProcess)
			{
				auto middleProcess = currentProcess->middleHigh;
				if (middleProcess)
				{
					return middleProcess->alphaMult;
				}
			}
		}

		return 1.0;
	}

	float PO3_SKSEFunctions::GetActorRefraction(StaticFunctionTag*, Actor* thisActor)
	{
		if (thisActor)
		{
			auto currentProcess = thisActor->currentProcess;
			if (currentProcess)
			{
				auto middleProcess = currentProcess->middleHigh;
				if (middleProcess)
				{
					return middleProcess->scriptRefractPower;
				}
			}
		}

		return 1.0;
	}

	void PO3_SKSEFunctions::SetActorRefraction(StaticFunctionTag*, Actor* thisActor, float refraction)
	{
		if (thisActor)
		{
			auto currentProcess = thisActor->currentProcess;
			if (currentProcess)
			{
				refraction = std::clamp(refraction, 0.0f, 1.0f);

				currentProcess->SetActorRefraction(refraction);

				float invisibility = thisActor->GetActorValueCurrent(ActorValue::kInvisibility); //invisibility

				if (invisibility < 0.0 || invisibility <= 1.0 && invisibility <= 0.0 || thisActor != PlayerCharacter::GetSingleton())
				{
					if (refraction <= 0.0)
					{
						thisActor->SetRefraction(0, refraction);
						thisActor->UpdateAlpha();
					}
					else
					{
						thisActor->SetRefraction(1, refraction);
					}
				}
				else
				{
					thisActor->SetAlpha(1.0);

					refraction = 1.0 - refraction / 100.0;
					refraction = 1.0 + (0.01 - 1.0) * ((refraction - 0.0) / (1.0 - 0.0));

					thisActor->SetRefraction(1, refraction);
				}
			}
		}
	}

	SInt32 PO3_SKSEFunctions::GetActorState(StaticFunctionTag*, Actor* thisActor)
	{
		if (thisActor)
		{
			auto flags = thisActor->flags08;

			return static_cast<UInt32>(pun_bits(flags.secondaryAnimState1, flags.secondaryAnimState2, flags.secondaryAnimState3, flags.secondaryAnimState4));
		}

		return -1;
	}

	bool PO3_SKSEFunctions::InstantKill(StaticFunctionTag*, Actor* thisActor)
	{
		return thisActor ? thisActor->InstantKill() : false;
	}

	/*void PO3_SKSEFunctions::SetShaderType(StaticFunctionTag*, Actor* thisActor, TESObjectARMO* templateArmor)
	{
		if (!thisActor || !thisActor->Is3DLoaded() || !templateArmor)
		{
			return;
		}

		g_task->AddTask([thisActor, templateArmor]()
		{
			BSGeometry * templateGeometry = GetTemplateArmorGeometry(thisActor, templateArmor, 11);

			if (!templateGeometry)
			{
				return;
			}

			auto geometry = GetHeadPartGeometry(thisActor, BGSHeadPart::HeadPartType::kFace);
			SetShaderPropertyMLP(geometry, templateGeometry);

			SetArmorSkinShaderType(thisActor, templateGeometry, BGSBipedObjectForm::FirstPersonFlag::kBody);
			SetArmorSkinShaderType(thisActor, templateGeometry, BGSBipedObjectForm::FirstPersonFlag::kHands);
			SetArmorSkinShaderType(thisActor, templateGeometry, BGSBipedObjectForm::FirstPersonFlag::kFeet);
			SetArmorSkinShaderType(thisActor, templateGeometry, BGSBipedObjectForm::FirstPersonFlag::kDecapitate); //decap
			SetArmorSkinShaderType(thisActor, templateGeometry, BGSBipedObjectForm::FirstPersonFlag::kTail); //tail
		});
	}*/

	//--------------------------------------------------------------------------------------------
	// ARRAY
	//--------------------------------------------------------------------------------------------

	bool PO3_SKSEFunctions::AddStringToArray(StaticFunctionTag*, BSFixedString thisString, BSScript::VMArray<BSFixedString> stringArray)
	{
		auto length = stringArray.size();
		if (length > 0)
		{
			BSFixedString string = nullptr;

			for (UInt32 i = 0; i < length; i++)
			{
				string = stringArray[i];
				if (string.empty())
				{
					stringArray[i] = thisString;
					return true;
				}
			}
		}

		return false;
	}

	bool PO3_SKSEFunctions::AddActorToArray(StaticFunctionTag*, Actor* thisActor, BSScript::VMArray<Actor*> actorArray)
	{
		auto length = actorArray.size();
		if (length > 0)
		{
			for (UInt32 i = 0; i < length; i++)
			{
				auto actor = actorArray.at(i);
				if (actor == nullptr)
				{
					actorArray.at(i) = thisActor;
					return true;
				}
			}
		}

		return false;
	}

	//count how many instances of string are found in an array
	UInt32 PO3_SKSEFunctions::ArrayStringCount(StaticFunctionTag*, BSFixedString thisString, BSScript::VMArray<BSFixedString> stringArray)
	{
		UInt32 count = 0;

		auto length = stringArray.size();
		if (length > 0)
		{
			BSFixedString string = nullptr;

			for (UInt32 i = 0; i < length; i++)
			{
				string = stringArray[i];
				if (string == thisString)
				{
					count++;
				}
			}
		}

		return count;
	}

	//alphabetically sorts strings inside array
	BSScript::VMArray<BSFixedString> PO3_SKSEFunctions::SortArrayString(StaticFunctionTag*, BSScript::VMArray<BSFixedString> stringArray)
	{
		BSScript::VMArray<BSFixedString> result;

		auto length = stringArray.size();
		if (length > 0)
		{
			std::string str;

			std::vector<std::string> vec;
			vec.reserve(length);

			BSFixedString string = nullptr;

			for (UInt32 i = 0; i < length; i++)
			{
				string = stringArray[i];
				if (!string.empty())
				{
					vec.emplace_back(string.c_str());
				}
			}

			std::sort(vec.begin(), vec.end());

			auto size = vec.size();
			if (size > 0)
			{
				result.resize(size);
				for (size_t i = 0; i < size; i++)
				{
					result[i] = vec[i].c_str();
				}
			}
		}

		return result;
	}

	//--------------------------------------------------------------------------------------------
	// EFFECTSHADER
	//--------------------------------------------------------------------------------------------

	//returns effect shader particle count
	float PO3_SKSEFunctions::GetEffectShaderFullParticleCount(StaticFunctionTag*, TESEffectShader* thisEffectShader)
	{
		return thisEffectShader ? thisEffectShader->data.particleShaderFullParticleBirthRatio : 0.0;
	}

	//sets effect shader particle count
	void PO3_SKSEFunctions::SetEffectShaderFullParticleCount(StaticFunctionTag*, TESEffectShader* thisEffectShader, float particleCount)
	{
		if (thisEffectShader)
		{
			thisEffectShader->data.particleShaderFullParticleBirthRatio = particleCount;
		}
	}

	//get effect shader persistant particle count
	float PO3_SKSEFunctions::GetEffectShaderPersistentParticleCount(StaticFunctionTag*, TESEffectShader* thisEffectShader)
	{
		return thisEffectShader ? thisEffectShader->data.particleShaderPersistantParticleCount : 0.0;
	}

	//set effect shader persistant particle count
	void PO3_SKSEFunctions::SetEffectShaderPersistentParticleCount(StaticFunctionTag*, TESEffectShader* thisEffectShader, float particleCount)
	{
		if (thisEffectShader)
		{
			thisEffectShader->data.particleShaderPersistantParticleCount = particleCount;
		}
	}

	bool PO3_SKSEFunctions::IsEffectShaderFlagSet(StaticFunctionTag*, TESEffectShader* thisEffectShader, UInt32 flag)
	{
		return thisEffectShader ? (static_cast<UInt32>(thisEffectShader->data.flags)& flag) == flag : false;
	}

	void PO3_SKSEFunctions::SetEffectShaderFlag(StaticFunctionTag*, TESEffectShader* thisEffectShader, UInt32 flag)
	{
		if (thisEffectShader)
		{
			thisEffectShader->data.flags |= static_cast<EffectShaderData::Flags>(flag);
		}
	}

	void PO3_SKSEFunctions::ClearEffectShaderFlag(StaticFunctionTag*, TESEffectShader* thisEffectShader, UInt32 flag)
	{
		if (thisEffectShader)
		{
			thisEffectShader->data.flags &= ~static_cast<EffectShaderData::Flags>(flag);
		}
	}

	UInt32 PO3_SKSEFunctions::GetEffectShaderTotalCount(StaticFunctionTag*, TESEffectShader* effectShader, bool active)
	{
		UInt32 count = 0;

		if (effectShader)
		{
			auto singleton = ProcessLists::GetSingleton();

			singleton->referenceEffectsLock.Lock();
			for (auto& referenceEffect : singleton->referenceEffects)
			{
				if (!referenceEffect)
				{
					continue;
				}

				auto shaderEffect = netimmerse_cast<ShaderReferenceEffect*>(referenceEffect);
				if (shaderEffect)
				{
					auto shader = shaderEffect->effectShader;
					if (shader && shader == effectShader)
					{
						if (active && shaderEffect->unk40 == 1)
						{
							continue;
						}
						count++;
					}
				}
			}
			singleton->referenceEffectsLock.Unlock();
		}

		return count;
	}

	//--------------------------------------------------------------------------------------------
	// FORM
	//--------------------------------------------------------------------------------------------

	// replaces keyword on form
	void PO3_SKSEFunctions::ReplaceKeywordOnForm(StaticFunctionTag*, TESForm* thisForm, BGSKeyword* KYWDtoRemove, BGSKeyword* KYWDtoAdd)
	{
		if (!thisForm || !KYWDtoRemove || !KYWDtoAdd)
		{
			return;
		}

		auto pKeywords = skyrim_cast<BGSKeywordForm*>(thisForm);
		if (pKeywords)
		{
			UInt32 removeIndex = 0;
			BGSKeyword* thisKYWD = nullptr;
			bool found = false;

			for (size_t i = 0; i < pKeywords->numKeywords; i++)
			{
				thisKYWD = pKeywords->keywords[i];

				if (thisKYWD)
				{
					if (thisKYWD->keyword.c_str() == KYWDtoAdd->keyword.c_str())
					{
						return;
					}

					if (thisKYWD->keyword.c_str() == KYWDtoRemove->keyword.c_str())
					{
						removeIndex = i;
						found = true;
					}
				}
			}

			if (found)
			{
				pKeywords->keywords[removeIndex] = KYWDtoAdd;
			}
		}
	}

	// adds keyword to form
	void PO3_SKSEFunctions::AddKeywordToForm(StaticFunctionTag*, TESForm* thisForm, BGSKeyword* KYWDtoAdd)
	{
		if (!thisForm || !KYWDtoAdd)
		{
			return;
		}

		auto pKeywords = skyrim_cast<BGSKeywordForm*>(thisForm);
		if (pKeywords)
		{
			auto oldData = pKeywords->keywords;

			pKeywords->keywords = calloc<BGSKeyword*>(++pKeywords->numKeywords);
			if (oldData)
			{
				for (UInt32 i = 0; i < pKeywords->numKeywords - 1; ++i)
				{
					pKeywords->keywords[i] = oldData[i];
				}

				pKeywords->keywords[pKeywords->numKeywords - 1] = KYWDtoAdd;

				free(oldData);
				oldData = nullptr;
			}
		}
	}

	//--------------------------------------------------------------------------------------------
	// GAME
	//--------------------------------------------------------------------------------------------

	bool PO3_SKSEFunctions::IsPluginFound(StaticFunctionTag*, BSFixedString name)
	{
		auto dataHandler = TESDataHandler::GetSingleton();
		const TESFile* modInfo = dataHandler->LookupModByName(name);

		if (modInfo)
		{
			return modInfo->IsLoaded();
		}

		return false;
	}

	BSScript::VMArray<TESForm*> PO3_SKSEFunctions::GetAllSpellsInMod(StaticFunctionTag*, BSFixedString modName, BSScript::VMArray<BGSKeyword*> keywords, bool isPlayable)
	{
		BSScript::VMArray<TESForm*> result;

		auto dataHandler = TESDataHandler::GetSingleton();
		const TESFile* modInfo = dataHandler->LookupModByName(modName.c_str());

		if (!modInfo || !modInfo->IsLoaded())
		{
			return result;
		}

		std::vector<TESForm*> vec;

		if (isPlayable)
		{
			SpellItem* spell = nullptr;

			for (auto& book : dataHandler->GetFormArray<TESObjectBOOK>())
			{
				if (!book || !modInfo->IsFormInMod(book->formID))
				{
					continue;
				}

				spell = book->data.teaches.spell;

				if (!spell || spell && VerifyKeywords(spell, &keywords))
				{
					continue;
				}

				vec.push_back(spell);
			}
		}
		else
		{
			for (auto& spell : dataHandler->GetFormArray<SpellItem>())
			{
				if (!spell || !modInfo->IsFormInMod(spell->formID))
				{
					continue;
				}

				if (!VerifyKeywords(spell, &keywords))
				{
					continue;
				}

				vec.push_back(spell);
			}
		}

		FillVMArray(vec, result);
		return result;
	}

	BSScript::VMArray<TESForm*> PO3_SKSEFunctions::GetAllRacesInMod(StaticFunctionTag*, BSFixedString modName, BSScript::VMArray<BGSKeyword*> keywords)
	{
		BSScript::VMArray<TESForm*> result;

		auto dataHandler = TESDataHandler::GetSingleton();
		const TESFile* modInfo = dataHandler->LookupModByName(modName.c_str());

		if (!modInfo || !modInfo->IsLoaded())
		{
			return result;
		}

		std::vector<TESForm*> vec;

		for (auto& race : dataHandler->GetFormArray<TESRace>())
		{
			if (!race || !modInfo->IsFormInMod(race->formID))
			{
				continue;
			}

			if (!VerifyKeywords(race, &keywords))
			{
				continue;
			}

			vec.push_back(race);
		}

		FillVMArray(vec, result);
		return result;
	}

	void PO3_SKSEFunctions::AddAllGameSpellsToList(StaticFunctionTag*, BGSListForm* thisList, BSScript::VMArray<BGSKeyword*> keywords, bool isPlayable)
	{
		if (!thisList)
		{
			return;
		}

		auto dataHandler = TESDataHandler::GetSingleton();

		if (isPlayable)
		{
			SpellItem* spell = nullptr;

			for (auto& book : dataHandler->GetFormArray<TESObjectBOOK>())
			{
				if (book)
				{
					spell = book->data.teaches.spell;

					if (!spell || spell && VerifyKeywords(spell, &keywords))
					{
						continue;
					}

					thisList->AddForm(spell);
				}
			}
		}
		else
		{
			for (auto& spell : dataHandler->GetFormArray<SpellItem>())
			{
				if (!spell || !VerifyKeywords(spell, &keywords))
				{
					continue;
				}

				thisList->AddForm(spell);
			}
		}
	}

	void PO3_SKSEFunctions::AddAllGameRacesToList(StaticFunctionTag*, BGSListForm* thisList, BSScript::VMArray<BGSKeyword*> keywords)
	{
		if (!thisList)
		{
			return;
		}

		auto dataHandler = TESDataHandler::GetSingleton();
		for (auto& race : dataHandler->GetFormArray<TESRace>())
		{
			if (!race || !VerifyKeywords(race, &keywords))
			{
				continue;
			}

			thisList->AddForm(race);
		}
	}

	//gets actors by AI processing level - see https://geck.bethsoft.com/index.php?title=GetActorsByProcessingLevel
	BSScript::VMArray<Actor*> PO3_SKSEFunctions::GetActorsByProcessingLevel(StaticFunctionTag*, UInt32 level)
	{
		BSScript::VMArray<Actor*> result;
		std::vector<Actor*> vec;

		auto singleton = ProcessLists::GetSingleton();
		BSTArray<ActorHandle>* arr = nullptr;

		switch (level)
		{
			case 0:
				arr = &singleton->highProcesses;
				break;
			case 1:
				arr = &singleton->middleHighProcesses;
				break;
			case 2:
				arr = &singleton->middleLowProcesses;
				break;
			case 3:
				arr = &singleton->lowProcesses;
				break;
			default:
				arr = nullptr;
				break;
		}

		NiPointer<Actor> actorPtr;
		if (arr)
		{
			for (auto& actorHandle : *arr)
			{
				actorPtr = actorHandle.get();
				if (actorPtr && actorPtr.get())
				{
					vec.push_back(actorPtr.get());
				}
			}
		}

		FillVMArray(vec, result);
		return result;
	}

	//gets amount of actors in high process
	SInt32 PO3_SKSEFunctions::GetNumActorsInHigh(StaticFunctionTag*)
	{
		return ProcessLists::GetSingleton()->numActorsInHighProcess;
	}

	//--------------------------------------------------------------------------------------------
	// LIGHT
	//--------------------------------------------------------------------------------------------

	float PO3_SKSEFunctions::GetLightRadius(StaticFunctionTag*, TESObjectLIGH* thisLight)
	{
		return thisLight ? static_cast<float>(thisLight->data.radius) : 0.0;
	}

	void PO3_SKSEFunctions::SetLightRadius(StaticFunctionTag*, TESObjectLIGH* thisLight, float radius)
	{
		if (thisLight)
		{
			thisLight->data.radius = static_cast<UInt32>(radius);
		}
	}

	float PO3_SKSEFunctions::GetLightFade(StaticFunctionTag*, TESObjectLIGH* thisLight)
	{
		return thisLight ? thisLight->fadeValue : 0.0;
	}

	void PO3_SKSEFunctions::SetLightFade(StaticFunctionTag*, TESObjectLIGH* thisLight, float fadeValue)
	{
		if (thisLight)
		{
			thisLight->fadeValue = fadeValue;
		}
	}

	BGSColorForm* PO3_SKSEFunctions::GetLightColor(StaticFunctionTag*, TESObjectLIGH* thisLight)
	{
		if (thisLight)
		{
			auto factory = IFormFactory::GetConcreteFormFactoryByType<BGSColorForm>();
			auto colorForm = factory->Create();

			if (colorForm)
			{
				colorForm->flags &= ~BGSColorForm::Flag::kPlayable;

				colorForm->color.red = thisLight->data.color.red;
				colorForm->color.green = thisLight->data.color.green;
				colorForm->color.blue = thisLight->data.color.blue;

				return colorForm;
			}
		}

		return nullptr;
	}

	void PO3_SKSEFunctions::SetLightColor(StaticFunctionTag*, TESObjectLIGH* thisLight, BGSColorForm* colorForm)
	{
		if (thisLight && colorForm)
		{
			thisLight->data.color.red = colorForm->color.red;
			thisLight->data.color.green = colorForm->color.green;
			thisLight->data.color.blue = colorForm->color.blue;
		}
	}

	UInt32 PO3_SKSEFunctions::GetLightTypeInternal(TESObjectLIGH* thisLight)
	{
		if (thisLight)
		{
			const auto flags = thisLight->data.flags;

			if ((flags & TESObjectLIGH::Data::Flag::kFlag_TypeHemiShadow) == TESObjectLIGH::Data::Flag::kFlag_TypeHemiShadow)
			{
				return 1;
			}
			if ((flags & TESObjectLIGH::Data::Flag::kFlag_TypeOmni) == TESObjectLIGH::Data::Flag::kFlag_TypeOmni)
			{
				return 2;
			}
			if ((flags & TESObjectLIGH::Data::Flag::kFlag_TypeOmniShadow) == TESObjectLIGH::Data::Flag::kFlag_TypeOmniShadow)
			{
				return 3;
			}
			if ((flags & TESObjectLIGH::Data::Flag::kFlag_TypeSpot) == TESObjectLIGH::Data::Flag::kFlag_TypeSpot)
			{
				return 4;
			}
			if ((flags & TESObjectLIGH::Data::Flag::kFlag_TypeSpotShadow) == TESObjectLIGH::Data::Flag::kFlag_TypeSpotShadow)
			{
				return 5;
			}
		}

		return 0;
	}

	UInt32 PO3_SKSEFunctions::GetLightType(StaticFunctionTag*, TESObjectLIGH* thisLight)
	{
		return GetLightTypeInternal(thisLight);
	}

	void PO3_SKSEFunctions::SetLightType(StaticFunctionTag*, TESObjectLIGH* thisLight, UInt32 lightType)
	{
		if (!thisLight)
		{
			return;
		}

		auto flags = thisLight->data.flags;

		switch (lightType)
		{
			case 1:
				flags = flags & ~(TESObjectLIGH::Data::Flag::kFlags_Type | TESObjectLIGH::Data::Flag::kFlag_TypeHemiShadow);
				break;
			case 2:
				flags = flags & ~(TESObjectLIGH::Data::Flag::kFlags_Type | TESObjectLIGH::Data::Flag::kFlag_TypeOmni);
				break;
			case 3:
				flags = flags & ~(TESObjectLIGH::Data::Flag::kFlags_Type | TESObjectLIGH::Data::Flag::kFlag_TypeOmniShadow);
				break;
			case 4:
				flags = flags & ~(TESObjectLIGH::Data::Flag::kFlags_Type | TESObjectLIGH::Data::Flag::kFlag_TypeSpot);
				break;
			case 5:
				flags = flags & ~(TESObjectLIGH::Data::Flag::kFlags_Type | TESObjectLIGH::Data::Flag::kFlag_TypeSpotShadow);
				break;
			default:
				return;
		}

		thisLight->data.flags = flags;
	}

	float PO3_SKSEFunctions::GetLightFOV(StaticFunctionTag*, TESObjectLIGH* thisLight)
	{
		return thisLight ? thisLight->data.fov : 0.0;
	}

	void PO3_SKSEFunctions::SetLightFOV(StaticFunctionTag*, TESObjectLIGH* thisLight, float FOV)
	{
		if (thisLight)
		{
			thisLight->data.fov = FOV;
		}
	}

	float PO3_SKSEFunctions::GetLightShadowDepthBias(StaticFunctionTag*, TESObjectREFR* thisLightObject)
	{
		if (!thisLightObject)
		{
			return 1.0;
		}

		auto thisLight = skyrim_cast<TESObjectLIGH*>(thisLightObject->GetBaseObject());
		if (thisLight)
		{
			auto xLightData = static_cast<ExtraLightData*>(thisLightObject->extraData.GetByType(ExtraDataType::kLightData));
			if (xLightData)
			{
				return xLightData->data.shadowDepthBias;
			}
		}

		return 1.0;
	}

	//creates extralightdata if none exists
	void PO3_SKSEFunctions::SetLightShadowDepthBias(StaticFunctionTag*, TESObjectREFR* thisLightObject, float depthBias)
	{
		if (!thisLightObject)
		{
			return;
		}

		auto thisLight = skyrim_cast<TESObjectLIGH*>(thisLightObject->GetBaseObject());
		if (thisLight)
		{
			auto xLightData = static_cast<ExtraLightData*>(thisLightObject->extraData.GetByType(ExtraDataType::kLightData));
			if (xLightData)
			{
				xLightData->data.shadowDepthBias = depthBias;
			}
			else
			{
				auto&& newLightData = ExtraLightData::ExtraLightData();
				newLightData.data.shadowDepthBias = depthBias;
				(&thisLightObject->extraData)->Add(&newLightData);
			}
		}
	}

	//--------------------------------------------------------------------------------------------
	// LOCATION
	//--------------------------------------------------------------------------------------------

	BGSLocation* PO3_SKSEFunctions::GetParentLocation(StaticFunctionTag*, BGSLocation* thisLocation)
	{
		return thisLocation ? thisLocation->parentLoc : nullptr;
	}

	void PO3_SKSEFunctions::SetParentLocation(StaticFunctionTag*, BGSLocation* thisLocation, BGSLocation* newLocation)
	{
		if (thisLocation)
		{
			thisLocation->parentLoc = newLocation;
		}
	}

	//--------------------------------------------------------------------------------------------
	// MATHS
	//--------------------------------------------------------------------------------------------

	// based on mersenne twister
	float PO3_SKSEFunctions::GenerateRandomFloat(StaticFunctionTag*, float afMin, float afMax)
	{
		std::random_device rd;
		std::mt19937 engine{ rd() };
		std::uniform_real_distribution<float> dist(afMin, afMax);

		return dist(engine);
	}

	UInt32 PO3_SKSEFunctions::GenerateRandomInt(StaticFunctionTag*, UInt32 afMin, UInt32 afMax)
	{
		std::random_device rd;
		std::mt19937 engine{ rd() };
		std::uniform_int_distribution<UInt32>dist(afMin, afMax);

		return dist(engine);
	}

	//--------------------------------------------------------------------------------------------
	// MAGICEFFECT
	//--------------------------------------------------------------------------------------------

	BSScript::VMArray<EffectSetting*> PO3_SKSEFunctions::GetAllActiveEffectsOnActor(StaticFunctionTag*, Actor* thisActor, bool showInactive)
	{
		BSScript::VMArray<EffectSetting*> result;
		std::vector<EffectSetting*> vec;

		if (thisActor)
		{
			auto activeEffects = thisActor->GetActiveEffectList();
			if (activeEffects)
			{
				for (auto& ae : *activeEffects)
				{
					if (!ae || !ae->effect || !ae->effect->baseEffect)
					{
						continue;
					}

					auto mgef = ae->effect->baseEffect;
					if (mgef)
					{
						if (!showInactive && ((ae->flags & ActiveEffect::Flag::kInactive) == ActiveEffect::Flag::kInactive || (mgef->data.flags & EffectSetting::EffectSettingData::Flag::kHideInUI) == EffectSetting::EffectSettingData::Flag::kHideInUI))
						{
							continue;
						}

						vec.push_back(mgef);
					}
				}
			}
		}

		FillVMArray(vec, result);
		return result;
	}

	// for internal use
	BSFixedString PO3_SKSEFunctions::GetEffectArchetypeInternal(EffectSetting* mgef)
	{
		BSFixedString archetype("");

		if (!mgef)
		{
			return archetype;
		}

		switch (mgef->data.archetype)
		{
			case Archetype::kValueModifier:
				archetype = "ValueMod";
				break;
			case Archetype::kScript:
				archetype = "Script";
				break;
			case Archetype::kDispel:
				archetype = "Dispel";
				break;
			case Archetype::kCureDisease:
				archetype = "CureDisease";
				break;
			case Archetype::kAbsorb:
				archetype = "Absorb";
				break;
			case Archetype::kDualValueModifier:
				archetype = "DualValueMod";
				break;
			case Archetype::kCalm:
				archetype = "Calm";
				break;
			case Archetype::kDemoralize:
				archetype = "Demoralize";
				break;
			case Archetype::kFrenzy:
				archetype = "Frenzy";
				break;
			case Archetype::kDisarm:
				archetype = "Disarm";
				break;
			case Archetype::kCommandSummoned:
				archetype = "CommandSummoned";
				break;
			case Archetype::kInvisibility:
				archetype = "Invisibility";
				break;
			case Archetype::kLight:
				archetype = "Light";
				break;
			case Archetype::kLock:
				archetype = "Lock";
				break;
			case Archetype::kOpen:
				archetype = "Open";
				break;
			case Archetype::kBoundWeapon:
				archetype = "BoundWeapon";
				break;
			case Archetype::kSummonCreature:
				archetype = "SummonCreature";
				break;
			case Archetype::kDetectLife:
				archetype = "DetectLife";
				break;
			case Archetype::kTelekinesis:
				archetype = "Telekinesis";
				break;
			case Archetype::kParalysis:
				archetype = "Paralysis";
				break;
			case Archetype::kReanimate:
				archetype = "Reanimate";
				break;
			case Archetype::kSoulTrap:
				archetype = "SoulTrap";
				break;
			case Archetype::kTurnUndead:
				archetype = "TurnUndead";
				break;
			case Archetype::kGuide:
				archetype = "Guide";
				break;
			case Archetype::kWerewolfFeed:
				archetype = "WerewolfFeed";
				break;
			case Archetype::kCureParalysis:
				archetype = "CureParalysis";
				break;
			case Archetype::kCureAddiction:
				archetype = "CureAddiction";
				break;
			case Archetype::kCurePoison:
				archetype = "CurePoison";
				break;
			case Archetype::kConcussion:
				archetype = "Concussion";
				break;
			case Archetype::kValueAndParts:
				archetype = "ValueAndParts";
				break;
			case Archetype::kAccumulateMagnitude:
				archetype = "AccumulateMagnitude";
				break;
			case Archetype::kStagger:
				archetype = "Stagger";
				break;
			case Archetype::kPeakValueModifier:
				archetype = "PeakValueMod";
				break;
			case Archetype::kCloak:
				archetype = "Cloak";
				break;
			case Archetype::kWerewolf:
				archetype = "Werewolf";
				break;
			case Archetype::kSlowTime:
				archetype = "SlowTime";
				break;
			case Archetype::kRally:
				archetype = "Rally";
				break;
			case Archetype::kEnhanceWeapon:
				archetype = "EnhanceWeapon";
				break;
			case Archetype::kSpawnHazard:
				archetype = "SpawnHazard";
				break;
			case Archetype::kEtherealize:
				archetype = "Etherealize";
				break;
			case Archetype::kBanish:
				archetype = "Banish";
				break;
			case Archetype::kDisguise:
				archetype = "Disguise";
				break;
			case Archetype::kGrabActor:
				archetype = "GrabActor";
				break;
			case Archetype::kVampireLord:
				archetype = "VampireLord";
				break;
			default:
				break;
		}

		return archetype;
	}

	bool PO3_SKSEFunctions::HasMagicEffectWithArchetype(StaticFunctionTag*, Actor* thisActor, BSFixedString archetype)
	{
		if (thisActor)
		{
			auto activeEffects = thisActor->GetActiveEffectList();
			if (activeEffects)
			{
				for (auto& ae : *activeEffects)
				{
					if (!ae || !ae->effect || !ae->effect->baseEffect)
					{
						continue;
					}

					auto mgef = ae->effect->baseEffect;
					if (GetEffectArchetypeInternal(mgef) == archetype)
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	UInt32 PO3_SKSEFunctions::GetEffectArchetypeAsInt(StaticFunctionTag*, EffectSetting* mgef)
	{
		return mgef ? static_cast<UInt32>(mgef->data.archetype) : 0;
	}

	BSFixedString PO3_SKSEFunctions::GetEffectArchetypeAsString(StaticFunctionTag*, EffectSetting* mgef)
	{
		return GetEffectArchetypeInternal(mgef);
	}

	BGSSoundDescriptorForm* PO3_SKSEFunctions::GetMagicEffectSound(StaticFunctionTag*, EffectSetting* mgef, UInt32 type)
	{
		if (mgef)
		{
			for (auto& effectSound : mgef->effectSounds)
			{
				if (effectSound.id == static_cast<MagicSystem::SoundID>(type))
				{
					return effectSound.sound;
				}
			}
		}

		return nullptr;
	}

	void PO3_SKSEFunctions::SetMagicEffectSound(StaticFunctionTag*, EffectSetting* mgef, BGSSoundDescriptorForm* mgefSound, UInt32 type)
	{
		if (mgef && mgefSound)
		{
			for (auto& effectSound : mgef->effectSounds)
			{
				if (effectSound.id == static_cast<MagicSystem::SoundID>(type))
				{
					effectSound.sound = mgefSound;
					break;
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------------
	// OBJECTREFERENCES
	//--------------------------------------------------------------------------------------------

	// [x, y, z]
	BSScript::VMArray<float> PO3_SKSEFunctions::GetPositionAsArray(StaticFunctionTag*, TESObjectREFR* ref)
	{
		BSScript::VMArray<float> pos;
		pos.resize(3);

		if (ref)
		{
			pos[0] = ref->GetPosition().x;
			pos[1] = ref->GetPosition().y;
			pos[2] = ref->GetPosition().z;
		}

		return pos;
	}

	// [angleX, angleY, angleZ]
	BSScript::VMArray<float> PO3_SKSEFunctions::GetRotationAsArray(StaticFunctionTag*, TESObjectREFR* ref)
	{
		BSScript::VMArray<float> angles;
		angles.resize(3);

		if (ref)
		{
			angles[0] = ref->GetRotation().x * (180.0 / MATH_PI);
			angles[1] = ref->GetRotation().y * (180.0 / MATH_PI);
			angles[2] = ref->GetRotation().z * (180.0 / MATH_PI);
		}

		return angles;
	}

	bool PO3_SKSEFunctions::IsLoadDoor(StaticFunctionTag*, TESObjectREFR* thisDoor)
	{
		if (thisDoor)
		{
			auto teleport = static_cast<ExtraTeleport*>(thisDoor->extraData.GetByType(ExtraDataType::kTeleport));
			if (teleport)
			{
				return true;
			}
		}

		return false;
	}

	// internal checker
	bool PO3_SKSEFunctions::CanItemBeTaken(InventoryEntryData* data, bool noEquipped, bool noFavourited, bool noQuestItem)
	{
		if (data->extraLists)
		{
			for (auto& extraList : *data->extraLists)
			{
				if (noEquipped)
				{
					if (extraList->HasType(ExtraDataType::kWorn) || extraList->HasType(ExtraDataType::kWornLeft))
					{
						return false;
					}
				}

				if (noFavourited)
				{
					if (extraList->HasType(ExtraDataType::kHotkey))
					{
						return false;
					}
				}

				if (noQuestItem)
				{
					auto aliases = static_cast<ExtraAliasInstanceArray*>(extraList->GetByType(ExtraDataType::kAliasInstanceArray));

					if (aliases)
					{
						for (auto& alias : aliases->aliasInfoArr)
						{
							TESQuest* quest = alias->quest;
							BGSBaseAlias* refAlias = alias->alias;

							if (quest && refAlias && refAlias->IsQuestObject())
							{
								return false;
							}
						}
					}
				}
			}
		}

		return true;
	}

	void PO3_SKSEFunctions::AddAllInventoryItemsToList(StaticFunctionTag*, TESObjectREFR* thisRef, BGSListForm* thisList, bool noEquipped, bool noFavourited, bool noQuestItem)
	{
		if (thisRef && thisList)
		{
			auto exChanges = static_cast<ExtraContainerChanges*>(thisRef->extraData.GetByType(ExtraDataType::kContainerChanges)); //loop through caster inventory
			InventoryChanges* changes = exChanges ? exChanges->changes : nullptr;

			if (changes && changes->entryList)
			{
				for (InventoryEntryData* data : *changes->entryList)
				{
					if (data && CanItemBeTaken(data, noEquipped, noFavourited, noQuestItem))
					{
						auto object = data->object;
						if (object)
						{
							thisList->AddForm(object);
						}
					}
				}
			}
		}
	}

	BSScript::VMArray<TESForm*> PO3_SKSEFunctions::AddAllInventoryItemsToArray(StaticFunctionTag*, TESObjectREFR* thisRef, bool noEquipped, bool noFavourited, bool noQuestItem)
	{
		BSScript::VMArray<TESForm*> result;
		std::vector<TESForm*> vec;

		if (thisRef)
		{
			auto exChanges = static_cast<ExtraContainerChanges*>(thisRef->extraData.GetByType(ExtraDataType::kContainerChanges)); //loop through caster inventory
			InventoryChanges* changes = exChanges ? exChanges->changes : nullptr;

			if (changes && changes->entryList)
			{
				for (InventoryEntryData* data : *changes->entryList)
				{
					if (data && CanItemBeTaken(data, noEquipped, noFavourited, noQuestItem))
					{
						auto object = data->object;
						if (object)
						{
							vec.push_back(object);
						}
					}
				}
			}
		}

		FillVMArray(vec, result);
		return result;
	}

	// replaces keyword on ref
	void PO3_SKSEFunctions::ReplaceKeywordOnRef(StaticFunctionTag*, TESObjectREFR* thisRef, BGSKeyword* KYWDtoRemove, BGSKeyword* KYWDtoAdd)
	{
		if (!thisRef || !KYWDtoRemove || !KYWDtoAdd)
		{
			return;
		}

		auto pKeywords = skyrim_cast<BGSKeywordForm*>(thisRef->GetBaseObject());
		if (pKeywords)
		{
			UInt32 removeIndex = 0;
			BGSKeyword* thisKYWD = nullptr;
			bool found = false;

			for (size_t i = 0; i < pKeywords->numKeywords; i++)
			{
				thisKYWD = pKeywords->keywords[i];

				if (thisKYWD)
				{
					if (thisKYWD->keyword.c_str() == KYWDtoAdd->keyword.c_str())
					{
						return;
					}

					if (thisKYWD->keyword.c_str() == KYWDtoRemove->keyword.c_str())
					{
						removeIndex = i;
						found = true;
					}
				}
			}

			if (found)
			{
				pKeywords->keywords[removeIndex] = KYWDtoAdd;
			}
		}
	}

	// adds keyword to form
	void PO3_SKSEFunctions::AddKeywordToRef(StaticFunctionTag*, TESObjectREFR* thisRef, BGSKeyword* KYWDtoAdd)
	{
		if (!thisRef || !KYWDtoAdd || thisRef->HasKeywordHelper(KYWDtoAdd))
		{
			return;
		}

		auto pKeywords = skyrim_cast<BGSKeywordForm*>(thisRef->GetBaseObject());
		if (pKeywords)
		{
			auto oldData = pKeywords->keywords;

			pKeywords->keywords = calloc<BGSKeyword*>(++pKeywords->numKeywords);
			if (oldData)
			{
				for (UInt32 i = 0; i < pKeywords->numKeywords - 1; ++i)
				{
					pKeywords->keywords[i] = oldData[i];
				}

				pKeywords->keywords[pKeywords->numKeywords - 1] = KYWDtoAdd;

				free(oldData);
				oldData = nullptr;
			}
		}
	}

	//calculates a 2D vector
	void PO3_SKSEFunctions::Apply2DHavokImpulse(StaticFunctionTag*, TESObjectREFR* source, TESObjectREFR* target, float afZ, float magnitude)
	{
		if (!source || !target || !source->Is3DLoaded() || !target->Is3DLoaded())
		{
			return;
		}

		g_task->AddTask([source, target, afZ, magnitude]()
		{
			auto vm = BSScript::Internal::VirtualMachine::GetSingleton();

			float sourceZ = source->GetRotation().z * static_cast<float>(180.0 / MATH_PI);
			float angleZ = sourceZ + GetHeadingAngle(vm, 0, source, target); //source.getanglez() + source.getheadingangle(target)

			ApplyHavokImpulse(vm, 0, target, asinf(angleZ), acosf(angleZ), afZ, magnitude);
		});
	}

	//calculates a 3D vector and takes into account the elevation between source and target.
	void PO3_SKSEFunctions::Apply3DHavokImpulse(StaticFunctionTag*, TESObjectREFR* source, TESObjectREFR* target, float magnitude)
	{
		if (!source || !target || !source->Is3DLoaded() || !target->Is3DLoaded())
		{
			return;
		}

		float dx = target->GetPosition().x - source->GetPosition().x;
		float dy = target->GetPosition().y - source->GetPosition().y;
		float dz = target->GetPosition().z - source->GetPosition().z;

		float dist = CalcLinearDistance(target->GetPosition(), source->GetPosition());

		float x = dx / dist; //x
		float y = dy / dist; //y
		float z = dz / dist; //z

		g_task->AddTask([target, x, y, z, magnitude]()
		{
			ApplyHavokImpulse(BSScript::Internal::VirtualMachine::GetSingleton(), 0, target, x, y, z, magnitude);
		});
	}

	//moves object to nearest navemesh location
	void PO3_SKSEFunctions::MoveToNearestNavmeshLocation(StaticFunctionTag*, TESObjectREFR* target)
	{
		if (!target)
		{
			return;
		}

		auto nearestVertex = FindNearestVertex(target);
		if (!nearestVertex)
		{
			return;
		}

		auto handle = target->CreateRefHandle();
		g_task->AddTask([handle, nearestVertex]()
		{
			auto ref = handle.get();
			if (!ref || !ref.get())
			{
				return;
			}

			ref->SetPosition(std::move(*nearestVertex));
		});
	}

	BSScript::VMArray<TESEffectShader*> PO3_SKSEFunctions::GetAllEffectShaders(StaticFunctionTag*, TESObjectREFR* thisRef)
	{
		BSScript::VMArray<TESEffectShader*> result;
		std::vector<TESEffectShader*> vec;

		if (thisRef)
		{
			auto singleton = ProcessLists::GetSingleton();

			singleton->referenceEffectsLock.Lock();
			for (auto& referenceEffect : singleton->referenceEffects)
			{
				if (!referenceEffect)
				{
					continue;
				}

				auto shaderEffect = netimmerse_cast<ShaderReferenceEffect*>(referenceEffect);
				if (shaderEffect)
				{
					auto handle = thisRef->CreateRefHandle();
					if (referenceEffect->refHandle == handle)
					{
						if (shaderEffect->effectShader)
						{
							vec.push_back(shaderEffect->effectShader);
						}
					}
				}
			}
			singleton->referenceEffectsLock.Unlock();
		}

		FillVMArray(vec, result);
		return result;
	}

	UInt32 PO3_SKSEFunctions::HasEffectShader(StaticFunctionTag*, TESObjectREFR* thisRef, TESEffectShader* effectShader, bool active)
	{
		UInt32 effectShaderCount = 0;

		if (thisRef && effectShader)
		{
			auto singleton = ProcessLists::GetSingleton();

			singleton->referenceEffectsLock.Lock();
			for (auto& referenceEffect : singleton->referenceEffects)
			{
				if (!referenceEffect)
				{
					continue;
				}

				auto shaderEffect = netimmerse_cast<ShaderReferenceEffect*>(referenceEffect);
				if (shaderEffect)
				{
					auto handle = thisRef->CreateRefHandle();
					if (shaderEffect->refHandle == handle)
					{
						auto shader = shaderEffect->effectShader;

						if (shader && shader == effectShader)
						{
							if (active && shaderEffect->unk40 == 1)
							{
								continue;
							}
							effectShaderCount++;
						}
					}
				}
			}
			singleton->referenceEffectsLock.Unlock();
		}

		return effectShaderCount;
	}

	BSScript::VMArray<BGSArtObject*> PO3_SKSEFunctions::GetAllArtObjects(StaticFunctionTag*, TESObjectREFR* thisRef)
	{
		BSScript::VMArray<BGSArtObject*> result;
		std::vector<BGSArtObject*> vec;

		if (thisRef)
		{
			auto singleton = ProcessLists::GetSingleton();

			singleton->referenceEffectsLock.Lock();
			for (auto& referenceEffect : singleton->referenceEffects)
			{
				if (!referenceEffect)
				{
					continue;
				}

				auto modelEffect = netimmerse_cast<ModelReferenceEffect*>(referenceEffect);
				if (modelEffect)
				{
					auto handle = thisRef->CreateRefHandle();
					if (modelEffect->refHandle == handle)
					{
						if (modelEffect->artObject)
						{
							vec.push_back(modelEffect->artObject);
						}
					}
				}
			}
			singleton->referenceEffectsLock.Unlock();
		}

		FillVMArray(vec, result);
		return result;
	}

	UInt32 PO3_SKSEFunctions::HasArtObject(StaticFunctionTag*, TESObjectREFR* thisRef, BGSArtObject* artObject, bool active)
	{
		UInt32 artObjectCount = 0;

		if (thisRef && artObject)
		{
			auto singleton = ProcessLists::GetSingleton();

			singleton->referenceEffectsLock.Lock();
			for (auto& referenceEffect : singleton->referenceEffects)
			{
				if (!referenceEffect)
				{
					continue;
				}

				auto modelEffect = netimmerse_cast<ModelReferenceEffect*>(referenceEffect);
				if (modelEffect)
				{
					auto handle = thisRef->CreateRefHandle();
					if (modelEffect->refHandle == handle)
					{
						auto art = modelEffect->artObject;

						if (art && art == artObject)
						{
							if (active && modelEffect->unk40 == 1)
							{
								continue;
							}
							artObjectCount++;
						}
					}
				}
			}
			singleton->referenceEffectsLock.Unlock();
		}

		return artObjectCount;
	}

	void PO3_SKSEFunctions::StopArtObject(StaticFunctionTag*, TESObjectREFR* thisRef, BGSArtObject* artObject)
	{
		if (thisRef)
		{
			ProcessLists::GetSingleton()->StopArtObject(thisRef, artObject);
		}
	}

	void PO3_SKSEFunctions::StopAllShaders(StaticFunctionTag*, TESObjectREFR* thisRef)
	{
		if (thisRef)
		{
			StopAllShaders_Internal(thisRef);
		}
	}

	Actor* PO3_SKSEFunctions::GetActorCause(StaticFunctionTag*, TESObjectREFR* thisRef)
	{
		if (thisRef)
		{
			auto actorCause = thisRef->GetActorCause();
			if (actorCause)
			{
				auto actorPtr = Actor::LookupByHandle(actorCause->handle);
				if (actorPtr && actorPtr.get())
				{
					return actorPtr.get();
				}
			}
		}

		return nullptr;
	}

	Actor* PO3_SKSEFunctions::GetClosestActorFromRef(StaticFunctionTag*, TESObjectREFR* thisRef, bool ignorePlayer)
	{
		if (thisRef)
		{
			auto singleton = ProcessLists::GetSingleton();

			if (ignorePlayer && singleton->numActorsInHighProcess == 0)
			{
				return nullptr;
			}

			const auto originPos = thisRef->GetPosition();
			auto shortestDistance = std::numeric_limits<float>::max();

			std::map<Actor*, float> map;
			NiPointer<Actor> actorPtr;

			for (auto& actorHandle : singleton->highProcesses)
			{
				actorPtr = actorHandle.get();

				if (actorPtr && actorPtr.get() && actorPtr.get() != thisRef)
				{
					auto distance = CalcLinearDistance(originPos, actorPtr->GetPosition());
					map.emplace(actorPtr.get(), distance);

					if (distance < shortestDistance)
					{
						shortestDistance = distance;
					}
				}
			}

			auto player = PlayerCharacter::GetSingleton();

			if (!ignorePlayer && thisRef != player)
			{
				auto distance = CalcLinearDistance(originPos, player->GetPosition());
				map.emplace(player, distance);

				if (distance < shortestDistance)
				{
					shortestDistance = distance;
				}
			}

			auto it = std::find_if(map.begin(), map.end(), [shortestDistance](const auto& mo)
			{
				return mo.second == shortestDistance;
			});

			if (it != map.end())
			{
				return it->first;
			}
		}

		return nullptr;
	}

	Actor* PO3_SKSEFunctions::GetRandomActorFromRef(StaticFunctionTag*, TESObjectREFR* thisRef, float radius, bool ignorePlayer)
	{
		if (thisRef)
		{
			auto singleton = ProcessLists::GetSingleton();

			if (ignorePlayer && singleton->numActorsInHighProcess == 0)
			{
				return nullptr;
			}

			std::vector<Actor*> vec;
			vec.reserve(singleton->numActorsInHighProcess);

			auto squaredRadius = radius * radius;
			auto originPos = thisRef->GetPosition();

			NiPointer<Actor> actorPtr;
			for (auto& actorHandle : singleton->highProcesses)
			{
				actorPtr = actorHandle.get();

				if (!actorPtr || !actorPtr.get() || actorPtr.get() == thisRef)
				{
					continue;
				}

				auto distance = CalcLinearDistance(originPos, actorPtr->GetPosition());
				if (distance > squaredRadius)
				{
					continue;
				}

				vec.emplace_back(actorPtr.get());
			}

			auto player = PlayerCharacter::GetSingleton();
			if (thisRef != player)
			{
				auto distance = CalcLinearDistance(originPos, player->GetPosition());
				if (distance <= squaredRadius)
				{
					vec.emplace_back(player);
				}
			}

			std::random_device rd;
			std::mt19937 engine{ rd() };

			std::uniform_int_distribution<UInt32>dist(0, vec.size() - 1);

			return vec.at(dist(engine));
		}

		return nullptr;
	}

	BSScript::VMArray<TESObjectREFR*> PO3_SKSEFunctions::FindAllReferencesOfType(StaticFunctionTag*, TESObjectREFR* ref, TESForm* formOrList, float radius)
	{
		BSScript::VMArray<TESObjectREFR*> result;

		if (!ref || !formOrList)
		{
			return result;
		}

		std::vector<NiPointer<TESObjectREFR>> vec;

		BGSListForm* formList = nullptr;
		if (formOrList->formType == FormType::FormList)
		{
			formList = static_cast<BGSListForm*>(formOrList);
		}

		auto refPos = ref->GetPosition();
		auto squaredRadius = radius * radius;
		auto singleton = TES::GetSingleton();

		auto cell = singleton->currentInteriorCell;
		if (cell)
		{
			cell->cellRefLock.Lock();
			vec.reserve(cell->objectList.size());
			for (auto& object : cell->objectMap)
			{
				if (object && object->GetBaseObject())
				{
					auto distance = CalcLinearDistance(refPos, object->GetPosition());
					if (distance <= squaredRadius)
					{
						auto objectBaseForm = object->GetBaseObject();

						if (formList)
						{
							for (auto& form : formList->forms)
							{
								if (form && form == objectBaseForm)
								{
									vec.emplace_back(object);
								}
							}
							for (auto& formID : *formList->scriptAddedTempForms)
							{
								auto form = TESForm::LookupByID(formID);

								if (form && form == objectBaseForm)
								{
									vec.emplace_back(object);
								}
							}
						}
						else if (formOrList == objectBaseForm)
						{
							vec.emplace_back(object);
						}
					}
				}
			}
			cell->cellRefLock.Unlock();
		}
		else
		{
			auto gridCellArray = singleton->gridCellArray;
			auto gridLength = gridCellArray->length;

			if (gridCellArray && gridLength > 0)
			{
				UInt32 x = 0;
				UInt32 y = 0;

				float YplusRadius = refPos.y + radius;
				float YminusRadius = refPos.y - radius;
				float XplusRadius = refPos.x + radius;
				float XminusRadius = refPos.x - radius;

				for (x = 0, y = 0; (x < gridLength && y < gridLength); x++, y++)
				{
					TESObjectCELL* cell = gridCellArray->GetCell(x, y);
					if (cell && cell->IsAttached())
					{
						auto cellCoords = cell->GetCoordinates();
						if (cellCoords->worldX < XplusRadius && (cellCoords->worldX + 4096.0) > XminusRadius&& cellCoords->worldY < YplusRadius && (cellCoords->worldY + 4096.0) > YminusRadius)
						{
							cell->cellRefLock.Lock();
							vec.reserve(cell->objectMap.size());
							for (auto& object : cell->objectMap)
							{
								if (object && object->GetBaseObject())
								{
									auto distance = CalcLinearDistance(refPos, object->GetPosition());
									if (distance <= squaredRadius)
									{
										auto objectBaseForm = object->GetBaseObject();

										if (formList)
										{
											for (auto& form : formList->forms)
											{
												if (form && form == objectBaseForm)
												{
													vec.emplace_back(object);
												}
											}

											for (auto& formID : *formList->scriptAddedTempForms)
											{
												auto form = TESForm::LookupByID(formID);

												if (form && form == objectBaseForm)
												{
													vec.emplace_back(object);
												}
											}
										}
										else if (formOrList == objectBaseForm)
										{
											vec.emplace_back(object);
										}
									}
								}
							}
							cell->cellRefLock.Unlock();
						}
					}
				}
			}
		}
		if (vec.empty())
		{
			auto worldSpace = singleton->worldSpace;
			if (worldSpace)
			{
				auto cell = worldSpace->GetOrCreateUnk0F8Cell();
				if (cell)
				{
					cell->cellRefLock.Lock();
					vec.reserve(cell->objectList.size());
					for (auto& object : cell->objectMap)
					{
						if (object && object->GetBaseObject())
						{
							auto distance = CalcLinearDistance(refPos, object->GetPosition());
							if (distance <= squaredRadius)
							{
								auto objectBaseForm = object->GetBaseObject();

								if (formList)
								{
									for (auto& form : formList->forms)
									{
										if (form && form == objectBaseForm)
										{
											vec.emplace_back(object);
										}
									}

									for (auto& formID : *formList->scriptAddedTempForms)
									{
										auto form = TESForm::LookupByID(formID);

										if (form && form == objectBaseForm)
										{
											vec.emplace_back(object);
										}
									}
								}
								else if (formOrList == objectBaseForm)
								{
									vec.emplace_back(object);
								}
							}
						}
					}
					cell->cellRefLock.Unlock();
				}
			}
		}

		NiPtr_FillVMArray(vec, result);
		return result;
	}

	BSScript::VMArray<TESObjectREFR*> PO3_SKSEFunctions::FindAllReferencesWithKeyword(StaticFunctionTag*, TESObjectREFR* ref, BSScript::VMArray<BGSKeyword*> keywords, float radius, bool matchAll)
	{
		BSScript::VMArray<TESObjectREFR*> result;

		if (!ref || keywords.empty())
		{
			return result;
		}

		std::vector<NiPointer<TESObjectREFR>> vec;

		NiPoint3 refPos = ref->GetPosition();
		float squaredRadius = radius * radius;
		auto singleton = TES::GetSingleton();

		auto cell = singleton->currentInteriorCell;
		if (cell)
		{
			cell->cellRefLock.Lock();
			vec.reserve(cell->objectList.size());
			for (auto& object : cell->objectList)
			{
				if (object && object->GetBaseObject())
				{
					auto distance = CalcLinearDistance(refPos, object->GetPosition());
					if (distance <= squaredRadius)
					{
						bool success = matchAll ? VerifyAllKeywords(object->GetBaseObject(), &keywords) : VerifyKeywords(object->GetBaseObject(), &keywords);
						if (success)
						{
							vec.emplace_back(object);
						}
					}
				}
			}
			cell->cellRefLock.Unlock();
		}
		else
		{
			auto gridCellArray = singleton->gridCellArray;
			auto gridLength = gridCellArray->length;

			if (gridCellArray && gridLength > 0)
			{
				UInt32 x = 0;
				UInt32 y = 0;

				float YplusRadius = refPos.y + radius;
				float YminusRadius = refPos.y - radius;
				float XplusRadius = refPos.x + radius;
				float XminusRadius = refPos.x - radius;

				for (x = 0, y = 0; (x < gridLength && y < gridLength); x++, y++)
				{
					TESObjectCELL* cell = gridCellArray->GetCell(x, y);
					if (cell && cell->IsAttached())
					{
						auto cellCoords = cell->GetCoordinates();
						if (cellCoords->worldX < XplusRadius && (cellCoords->worldX + 4096.0) > XminusRadius&& cellCoords->worldY < YplusRadius && (cellCoords->worldY + 4096.0) > YminusRadius)
						{
							cell->cellRefLock.Lock();
							vec.reserve(cell->objectMap.size());
							for (auto& object : cell->objectMap)
							{
								if (object && object->GetBaseObject())
								{
									auto distance = CalcLinearDistance(refPos, object->GetPosition());
									if (distance <= squaredRadius)
									{
										bool success = matchAll ? VerifyAllKeywords(object->GetBaseObject(), &keywords) : VerifyKeywords(object->GetBaseObject(), &keywords);
										if (success)
										{
											vec.emplace_back(object);
										}
									}
								}
							}
							cell->cellRefLock.Unlock();
						}
					}
				}
			}
		}
		if (vec.empty())
		{
			auto worldSpace = singleton->worldSpace;
			if (worldSpace)
			{
				auto cell = worldSpace->GetOrCreateUnk0F8Cell();
				if (cell)
				{
					cell->cellRefLock.Lock();
					vec.reserve(cell->objectList.size());
					for (auto& object : cell->objectList)
					{
						if (object && object->GetBaseObject())
						{
							auto distance = CalcLinearDistance(refPos, object->GetPosition());
							if (distance <= squaredRadius)
							{
								bool success = matchAll ? VerifyAllKeywords(object->GetBaseObject(), &keywords) : VerifyKeywords(object->GetBaseObject(), &keywords);
								if (success)
								{
									vec.emplace_back(object);
								}
							}
						}
					}
					cell->cellRefLock.Unlock();
				}
			}
		}

		NiPtr_FillVMArray(vec, result);
		return result;
	}

	float PO3_SKSEFunctions::GetEffectShaderDuration(StaticFunctionTag*, TESObjectREFR* thisRef, TESEffectShader* effectShader)
	{
		float time = 0.0;

		if (thisRef && effectShader)
		{
			auto singleton = ProcessLists::GetSingleton();

			singleton->referenceEffectsLock.Lock();
			for (auto& referenceEffect : singleton->referenceEffects)
			{
				if (!referenceEffect)
				{
					continue;
				}

				auto shaderEffect = netimmerse_cast<ShaderReferenceEffect*>(referenceEffect);
				if (shaderEffect)
				{
					auto handle = thisRef->CreateRefHandle();
					if (shaderEffect->refHandle == handle)
					{
						auto shader = shaderEffect->effectShader;

						if (shader && shader == effectShader)
						{
							time = shaderEffect->time;
							break;
						}
					}
				}
			}
			singleton->referenceEffectsLock.Unlock();
		}

		return time;
	}

	void PO3_SKSEFunctions::SetEffectShaderDuration(StaticFunctionTag*, TESObjectREFR* thisRef, TESEffectShader* effectShader, float time, bool absolute)
	{
		if (thisRef && effectShader)
		{
			auto singleton = ProcessLists::GetSingleton();

			singleton->referenceEffectsLock.Lock();
			for (auto& referenceEffect : singleton->referenceEffects)
			{
				if (!referenceEffect)
				{
					continue;
				}

				auto shaderEffect = netimmerse_cast<ShaderReferenceEffect*>(referenceEffect);
				if (shaderEffect)
				{
					auto handle = thisRef->CreateRefHandle();
					if (shaderEffect->refHandle == handle)
					{
						auto shader = shaderEffect->effectShader;

						if (shader && shader == effectShader)
						{
							if (!absolute)
							{
								float value = shaderEffect->time + time;
								if (value >= -1.0)
								{
									shaderEffect->time = time;
								}
							}
							else
							{
								shaderEffect->time = time;
							}
							break;
						}
					}
				}
			}
			singleton->referenceEffectsLock.Unlock();
		}
	}

	//--------------------------------------------------------------------------------------------
	// PACKAGE
	//--------------------------------------------------------------------------------------------

	SInt32 PO3_SKSEFunctions::GetPackageType(StaticFunctionTag*, TESPackage* package)
	{
		return package ? static_cast<UInt32>(package->packData.type) : -1;
	}

	//--------------------------------------------------------------------------------------------
	// PROJECTILE
	//--------------------------------------------------------------------------------------------

	float PO3_SKSEFunctions::GetProjectileSpeed(StaticFunctionTag*, BGSProjectile* thisProjectile)
	{
		return thisProjectile ? thisProjectile->data.speed : 0.0;
	}

	void PO3_SKSEFunctions::SetProjectileSpeed(StaticFunctionTag*, BGSProjectile* thisProjectile, float speed)
	{
		if (thisProjectile)
		{
			thisProjectile->data.speed = speed;
		}
	}

	float PO3_SKSEFunctions::GetProjectileRange(StaticFunctionTag*, BGSProjectile* thisProjectile)
	{
		return thisProjectile ? thisProjectile->data.range : 0.0;
	}

	void PO3_SKSEFunctions::SetProjectileRange(StaticFunctionTag*, BGSProjectile* thisProjectile, float range)
	{
		if (thisProjectile)
		{
			thisProjectile->data.range = range;
		}
	}

	float PO3_SKSEFunctions::GetProjectileGravity(StaticFunctionTag*, BGSProjectile* thisProjectile)
	{
		return thisProjectile ? thisProjectile->data.gravity : 0.0;
	}

	void PO3_SKSEFunctions::SetProjectileGravity(StaticFunctionTag*, BGSProjectile* thisProjectile, float gravity)
	{
		if (thisProjectile)
		{
			thisProjectile->data.gravity = gravity;
		}
	}

	float PO3_SKSEFunctions::GetProjectileImpactForce(StaticFunctionTag*, BGSProjectile* thisProjectile)
	{
		return thisProjectile ? thisProjectile->data.impactForce : 0.0;
	}

	void PO3_SKSEFunctions::SetProjectileImpactForce(StaticFunctionTag*, BGSProjectile* thisProjectile, float impactForce)
	{
		if (thisProjectile)
		{
			thisProjectile->data.impactForce = impactForce;
		}
	}

	UInt32 PO3_SKSEFunctions::GetProjectileType(StaticFunctionTag*, BGSProjectile* thisProjectile)
	{
		if (thisProjectile)
		{
			const auto types = thisProjectile->data.types;

			if ((types & BGSProjectile::Data::Type::kMissile) == BGSProjectile::Data::Type::kMissile) //Missile
			{
				return 1;
			}
			if ((types & BGSProjectile::Data::Type::kLobber) == BGSProjectile::Data::Type::kLobber) //Lobber (runes)
			{
				return 2;
			}
			if ((types & BGSProjectile::Data::Type::kBeam) == BGSProjectile::Data::Type::kBeam) //Beam
			{
				return 3;
			}
			if ((types & BGSProjectile::Data::Type::kFlame) == BGSProjectile::Data::Type::kFlame) //Flame
			{
				return 4;
			}
			if ((types & BGSProjectile::Data::Type::kCone) == BGSProjectile::Data::Type::kCone) //Cone
			{
				return 5;
			}
			if ((types & BGSProjectile::Data::Type::kBarrier) == BGSProjectile::Data::Type::kBarrier) //Barrier
			{
				return 6;
			}
			if ((types & BGSProjectile::Data::Type::kArrow) == BGSProjectile::Data::Type::kArrow) //Arrow
			{
				return 7;
			}
		}

		return 0;
	}

	//--------------------------------------------------------------------------------------------
	// SOUND
	//--------------------------------------------------------------------------------------------

	void PO3_SKSEFunctions::SetSoundDescriptor(StaticFunctionTag*, TESSound* thisSound, BGSSoundDescriptorForm* thisSoundDescriptor)
	{
		if (thisSound && thisSoundDescriptor)
		{
			thisSound->soundDescriptor = thisSoundDescriptor;
		}
	}

	//--------------------------------------------------------------------------------------------
	// SPELL
	//--------------------------------------------------------------------------------------------

	UInt32 PO3_SKSEFunctions::GetSpellType(StaticFunctionTag*, SpellItem* thisSpell)
	{
		return thisSpell ? static_cast<UInt32>(thisSpell->data.type) : 0;
	}

	//--------------------------------------------------------------------------------------------
	// VISUALEFFECT
	//--------------------------------------------------------------------------------------------

	BGSArtObject* PO3_SKSEFunctions::GetArtObject(StaticFunctionTag*, BGSReferenceEffect* visualEffect)
	{
		return visualEffect ? visualEffect->data.artObject : nullptr;
	}

	void PO3_SKSEFunctions::SetArtObject(StaticFunctionTag*, BGSReferenceEffect* visualEffect, BGSArtObject* art)
	{
		if (visualEffect && art)
		{
			visualEffect->data.artObject = art;
		}
	}

	UInt32 PO3_SKSEFunctions::GetArtObjectTotalCount(StaticFunctionTag*, BGSReferenceEffect* visualEffect, bool active)
	{
		UInt32 count = 0;

		if (visualEffect)
		{
			auto artObject = visualEffect->data.artObject;
			if (artObject)
			{
				auto singleton = ProcessLists::GetSingleton();

				singleton->referenceEffectsLock.Lock();
				for (auto& referenceEffect : singleton->referenceEffects)
				{
					if (!referenceEffect)
					{
						continue;
					}

					auto modelEffect = netimmerse_cast<ModelReferenceEffect*>(referenceEffect);
					if (modelEffect)
					{
						auto art = modelEffect->artObject;
						if (art && art == artObject)
						{
							if (active && modelEffect->unk40 == 1)
							{
								continue;
							}
							count++;
						}
					}
				}
				singleton->referenceEffectsLock.Unlock();
			}
		}

		return count;
	}

	//--------------------------------------------------------------------------------------------
	// WEATHER
	//--------------------------------------------------------------------------------------------

	//returns wind speed from 0-255 (how it's set up in the weather form)
	UInt32 PO3_SKSEFunctions::GetWindSpeedAsInt(StaticFunctionTag*, TESWeather* thisWeather)
	{
		return thisWeather ? thisWeather->data.windSpeed : 0;
	}

	//returns wind speed from 0.0-1.0 (how it's set up in the CK)
	float PO3_SKSEFunctions::GetWindSpeedAsFloat(StaticFunctionTag*, TESWeather* thisWeather)
	{
		return thisWeather ? static_cast<float>(thisWeather->data.windSpeed / 255.0) : 0.0;
	}

	SInt32 PO3_SKSEFunctions::GetWeatherType(StaticFunctionTag*, TESWeather* thisWeather)
	{
		TESWeather* currentWeather = nullptr;

		if (thisWeather)
		{
			currentWeather = thisWeather;
		}

		if (!currentWeather)
		{
			currentWeather = Sky::GetSingleton()->currentWeather;
		}

		if (currentWeather)
		{
			const auto flags = currentWeather->data.flags;

			if ((flags & TESWeather::Data::Flag::kWeather_Pleasant) != TESWeather::Data::Flag::kNone)
			{
				return 0;
			}
			if ((flags & TESWeather::Data::Flag::kWeather_Cloudy) != TESWeather::Data::Flag::kNone)
			{
				return 1;
			}
			if ((flags & TESWeather::Data::Flag::kWeather_Rainy) != TESWeather::Data::Flag::kNone)
			{
				return 2;
			}
			if ((flags & TESWeather::Data::Flag::kWeather_Snowy) != TESWeather::Data::Flag::kNone)
			{
				return 3;
			}
		}

		return -1;
	}

	//--------------------------------------------------------------------------------------------
	// REGISTER
	//--------------------------------------------------------------------------------------------

	// Tell the game about the new functions.
	bool PO3_SKSEFunctions::Register(BSScript::Internal::VirtualMachine* a_vm)
	{
		if (!a_vm)
		{
			_MESSAGE("couldn't get VMState");
			return false;
		}

		a_vm->RegisterFunction("GetHairColor", "PO3_SKSEFunctions", GetHairColor);
		a_vm->RegisterFunction("GetSkinColor", "PO3_SKSEFunctions", GetSkinColor);
		a_vm->RegisterFunction("SetHairColor", "PO3_SKSEFunctions", SetHairColor);
		a_vm->RegisterFunction("SetSkinColor", "PO3_SKSEFunctions", SetSkinColor);
		a_vm->RegisterFunction("MixColorWithSkinTone", "PO3_SKSEFunctions", MixColorWithSkinTone);
		a_vm->RegisterFunction("SetSkinAlpha", "PO3_SKSEFunctions", SetSkinAlpha);
		a_vm->RegisterFunction("EquipArmorIfSkinVisible", "PO3_SKSEFunctions", EquipArmorIfSkinVisible);
		a_vm->RegisterFunction("ReplaceArmorTextureSet", "PO3_SKSEFunctions", ReplaceArmorTextureSet);
		a_vm->RegisterFunction("ReplaceSkinTextureSet", "PO3_SKSEFunctions", ReplaceSkinTextureSet);
		a_vm->RegisterFunction("ReplaceFaceTextureSet", "PO3_SKSEFunctions", ReplaceFaceTextureSet);
		a_vm->RegisterFunction("GetHeadPartTextureSet", "PO3_SKSEFunctions", GetHeadPartTextureSet);
		a_vm->RegisterFunction("SetHeadPartTextureSet", "PO3_SKSEFunctions", SetHeadPartTextureSet);
		a_vm->RegisterFunction("SetHeadPartAlpha", "PO3_SKSEFunctions", SetHeadPartAlpha);
		a_vm->RegisterFunction("ToggleChildNode", "PO3_SKSEFunctions", ToggleChildNode);
		a_vm->RegisterFunction("RemoveChildNode", "PO3_SKSEFunctions", RemoveChildNode);
		a_vm->RegisterFunction("IsActorSoulTrapped", "PO3_SKSEFunctions", IsActorSoulTrapped);
		a_vm->RegisterFunction("ResetActor3D", "PO3_SKSEFunctions", ResetActor3D);
		a_vm->RegisterFunction("RemoveEffectsNotOfType", "PO3_SKSEFunctions", RemoveEffectsNotOfType);
		a_vm->RegisterFunction("DecapitateActor", "PO3_SKSEFunctions", DecapitateActor);
		a_vm->RegisterFunction("GetTimeDead", "PO3_SKSEFunctions", GetTimeDead);
		a_vm->RegisterFunction("GetTimeOfDeath", "PO3_SKSEFunctions", GetTimeOfDeath);
		a_vm->RegisterFunction("GetRunningPackage", "PO3_SKSEFunctions", GetRunningPackage);
		a_vm->RegisterFunction("IsActorInWater", "PO3_SKSEFunctions", IsActorInWater);
		a_vm->RegisterFunction("GetActorAlpha", "PO3_SKSEFunctions", GetActorAlpha);
		a_vm->RegisterFunction("GetActorRefraction", "PO3_SKSEFunctions", GetActorRefraction);
		a_vm->RegisterFunction("SetActorRefraction", "PO3_SKSEFunctions", SetActorRefraction);
		a_vm->RegisterFunction("GetActorState", "PO3_SKSEFunctions", GetActorState);
		a_vm->RegisterFunction("InstantKill", "PO3_SKSEFunctions", InstantKill);
		//a_vm->RegisterFunction("SetShaderType", "PO3_SKSEFunctions", SetShaderType);

		a_vm->RegisterFunction("AddStringToArray", "PO3_SKSEFunctions", AddStringToArray);
		a_vm->RegisterFunction("AddActorToArray", "PO3_SKSEFunctions", AddActorToArray);
		a_vm->RegisterFunction("ArrayStringCount", "PO3_SKSEFunctions", ArrayStringCount);
		a_vm->RegisterFunction("SortArrayString", "PO3_SKSEFunctions", SortArrayString);

		a_vm->RegisterFunction("GetEffectShaderFullParticleCount", "PO3_SKSEFunctions", GetEffectShaderFullParticleCount);
		a_vm->RegisterFunction("SetEffectShaderFullParticleCount", "PO3_SKSEFunctions", SetEffectShaderFullParticleCount);
		a_vm->RegisterFunction("GetEffectShaderPersistentParticleCount", "PO3_SKSEFunctions", GetEffectShaderPersistentParticleCount);
		a_vm->RegisterFunction("SetEffectShaderPersistentParticleCount", "PO3_SKSEFunctions", SetEffectShaderPersistentParticleCount);
		a_vm->RegisterFunction("IsEffectShaderFlagSet", "PO3_SKSEFunctions", IsEffectShaderFlagSet);
		a_vm->RegisterFunction("SetEffectShaderFlag", "PO3_SKSEFunctions", SetEffectShaderFlag);
		a_vm->RegisterFunction("ClearEffectShaderFlag", "PO3_SKSEFunctions", ClearEffectShaderFlag);
		a_vm->RegisterFunction("GetEffectShaderTotalCount", "PO3_SKSEFunctions", GetEffectShaderTotalCount);

		a_vm->RegisterFunction("ReplaceKeywordOnForm", "PO3_SKSEFunctions", ReplaceKeywordOnForm);
		a_vm->RegisterFunction("AddKeywordToForm", "PO3_SKSEFunctions", AddKeywordToForm);

		a_vm->RegisterFunction("IsPluginFound", "PO3_SKSEFunctions", IsPluginFound, BSScript::IVirtualMachine::FunctionFlag::kNoWait);
		a_vm->RegisterFunction("GetAllSpellsInMod", "PO3_SKSEFunctions", GetAllSpellsInMod);
		a_vm->RegisterFunction("GetAllRacesInMod", "PO3_SKSEFunctions", GetAllRacesInMod);
		a_vm->RegisterFunction("AddAllGameSpellsToList", "PO3_SKSEFunctions", AddAllGameSpellsToList);
		a_vm->RegisterFunction("AddAllGameRacesToList", "PO3_SKSEFunctions", AddAllGameRacesToList);
		a_vm->RegisterFunction("GetActorsByProcessingLevel", "PO3_SKSEFunctions", GetActorsByProcessingLevel);
		a_vm->RegisterFunction("GetNumActorsInHigh", "PO3_SKSEFunctions", GetNumActorsInHigh);

		a_vm->RegisterFunction("GetLightRadius", "PO3_SKSEFunctions", GetLightRadius);
		a_vm->RegisterFunction("SetLightRadius", "PO3_SKSEFunctions", SetLightRadius);
		a_vm->RegisterFunction("GetLightFade", "PO3_SKSEFunctions", GetLightFade);
		a_vm->RegisterFunction("SetLightFade", "PO3_SKSEFunctions", SetLightFade);
		a_vm->RegisterFunction("GetLightColor", "PO3_SKSEFunctions", GetLightColor);
		a_vm->RegisterFunction("SetLightColor", "PO3_SKSEFunctions", SetLightColor);
		a_vm->RegisterFunction("GetLightType", "PO3_SKSEFunctions", GetLightType);
		a_vm->RegisterFunction("SetLightType", "PO3_SKSEFunctions", SetLightType);
		a_vm->RegisterFunction("GetLightFOV", "PO3_SKSEFunctions", GetLightFOV);
		a_vm->RegisterFunction("SetLightFOV", "PO3_SKSEFunctions", SetLightFOV);
		a_vm->RegisterFunction("GetLightShadowDepthBias", "PO3_SKSEFunctions", GetLightShadowDepthBias);
		a_vm->RegisterFunction("SetLightShadowDepthBias", "PO3_SKSEFunctions", SetLightShadowDepthBias);

		a_vm->RegisterFunction("GetParentLocation", "PO3_SKSEFunctions", GetParentLocation);
		a_vm->RegisterFunction("SetParentLocation", "PO3_SKSEFunctions", SetParentLocation);

		a_vm->RegisterFunction("GenerateRandomFloat", "PO3_SKSEFunctions", GenerateRandomFloat, BSScript::IVirtualMachine::FunctionFlag::kNoWait);
		a_vm->RegisterFunction("GenerateRandomInt", "PO3_SKSEFunctions", GenerateRandomInt, BSScript::IVirtualMachine::FunctionFlag::kNoWait);

		a_vm->RegisterFunction("GetAllActiveEffectsOnActor", "PO3_SKSEFunctions", GetAllActiveEffectsOnActor);
		a_vm->RegisterFunction("HasMagicEffectWithArchetype", "PO3_SKSEFunctions", HasMagicEffectWithArchetype);
		a_vm->RegisterFunction("GetEffectArchetypeAsInt", "PO3_SKSEFunctions", GetEffectArchetypeAsInt);
		a_vm->RegisterFunction("GetEffectArchetypeAsString", "PO3_SKSEFunctions", GetEffectArchetypeAsString);
		a_vm->RegisterFunction("GetMagicEffectSound", "PO3_SKSEFunctions", GetMagicEffectSound);
		a_vm->RegisterFunction("SetMagicEffectSound", "PO3_SKSEFunctions", SetMagicEffectSound);

		a_vm->RegisterFunction("GetPositionAsArray", "PO3_SKSEFunctions", GetPositionAsArray);
		a_vm->RegisterFunction("GetRotationAsArray", "PO3_SKSEFunctions", GetRotationAsArray);
		a_vm->RegisterFunction("IsLoadDoor", "PO3_SKSEFunctions", IsLoadDoor);
		a_vm->RegisterFunction("AddAllInventoryItemsToList", "PO3_SKSEFunctions", AddAllInventoryItemsToList);
		a_vm->RegisterFunction("AddAllInventoryItemsToArray", "PO3_SKSEFunctions", AddAllInventoryItemsToArray);
		a_vm->RegisterFunction("AddAllEquippedItemsToArray", "PO3_SKSEFunctions", AddAllEquippedItemsToArray);
		a_vm->RegisterFunction("ReplaceKeywordOnRef", "PO3_SKSEFunctions", ReplaceKeywordOnRef);
		a_vm->RegisterFunction("AddKeywordToRef", "PO3_SKSEFunctions", AddKeywordToRef);
		a_vm->RegisterFunction("Apply2DHavokImpulse", "PO3_SKSEFunctions", Apply2DHavokImpulse);
		a_vm->RegisterFunction("Apply3DHavokImpulse", "PO3_SKSEFunctions", Apply3DHavokImpulse);
		a_vm->RegisterFunction("MoveToNearestNavmeshLocation", "PO3_SKSEFunctions", MoveToNearestNavmeshLocation);
		a_vm->RegisterFunction("GetAllEffectShaders", "PO3_SKSEFunctions", GetAllEffectShaders);
		a_vm->RegisterFunction("HasEffectShader", "PO3_SKSEFunctions", HasEffectShader);
		a_vm->RegisterFunction("GetAllArtObjects", "PO3_SKSEFunctions", GetAllArtObjects);
		a_vm->RegisterFunction("HasArtObject", "PO3_SKSEFunctions", HasArtObject);
		a_vm->RegisterFunction("StopArtObject", "PO3_SKSEFunctions", StopArtObject);
		a_vm->RegisterFunction("StopAllShaders", "PO3_SKSEFunctions", StopAllShaders);
		a_vm->RegisterFunction("GetActorCause", "PO3_SKSEFunctions", GetActorCause);
		a_vm->RegisterFunction("GetClosestActorFromRef", "PO3_SKSEFunctions", GetClosestActorFromRef);
		a_vm->RegisterFunction("GetRandomActorFromRef", "PO3_SKSEFunctions", GetRandomActorFromRef);
		a_vm->RegisterFunction("FindAllReferencesOfType", "PO3_SKSEFunctions", FindAllReferencesOfType);
		a_vm->RegisterFunction("FindAllReferencesWithKeyword", "PO3_SKSEFunctions", FindAllReferencesWithKeyword);
		a_vm->RegisterFunction("GetEffectShaderDuration", "PO3_SKSEFunctions", GetEffectShaderDuration);
		a_vm->RegisterFunction("SetEffectShaderDuration", "PO3_SKSEFunctions", SetEffectShaderDuration);

		a_vm->RegisterFunction("GetPackageType", "PO3_SKSEFunctions", GetPackageType);

		a_vm->RegisterFunction("GetProjectileSpeed", "PO3_SKSEFunctions", GetProjectileSpeed);
		a_vm->RegisterFunction("SetProjectileSpeed", "PO3_SKSEFunctions", SetProjectileSpeed);
		a_vm->RegisterFunction("GetProjectileRange", "PO3_SKSEFunctions", GetProjectileRange);
		a_vm->RegisterFunction("SetProjectileRange", "PO3_SKSEFunctions", SetProjectileRange);
		a_vm->RegisterFunction("GetProjectileGravity", "PO3_SKSEFunctions", GetProjectileGravity);
		a_vm->RegisterFunction("SetProjectileGravity", "PO3_SKSEFunctions", SetProjectileGravity);
		a_vm->RegisterFunction("GetProjectileImpactForce", "PO3_SKSEFunctions", GetProjectileImpactForce);
		a_vm->RegisterFunction("SetProjectileImpactForce", "PO3_SKSEFunctions", SetProjectileImpactForce);
		a_vm->RegisterFunction("GetProjectileType", "PO3_SKSEFunctions", GetProjectileType);

		a_vm->RegisterFunction("SetSoundDescriptor", "PO3_SKSEFunctions", SetSoundDescriptor);

		a_vm->RegisterFunction("GetSpellType", "PO3_SKSEFunctions", GetSpellType);

		a_vm->RegisterFunction("GetArtObject", "PO3_SKSEFunctions", GetArtObject);
		a_vm->RegisterFunction("SetArtObject", "PO3_SKSEFunctions", SetArtObject);
		a_vm->RegisterFunction("GetArtObjectTotalCount", "PO3_SKSEFunctions", GetArtObjectTotalCount);

		a_vm->RegisterFunction("GetWindSpeedAsInt", "PO3_SKSEFunctions", GetWindSpeedAsInt);
		a_vm->RegisterFunction("GetWindSpeedAsFloat", "PO3_SKSEFunctions", GetWindSpeedAsFloat);
		//a_vm->RegisterFunction("GetWeatherType", "PO3_SKSEFunctions", GetWeatherType);

		_MESSAGE("Registered papyrus functions");

		return true;
	}
}