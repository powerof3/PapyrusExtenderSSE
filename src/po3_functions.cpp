//credits to underthesky for original implementation, davidjcobb, aers, nukem, ryan, meh321, expired for tips and help
//modifications and extra functions by powerofthree
//modified for CommonLib

#include "po3_functions.h"
#include "po3_graphicfunctions.h"
#include "po3_offsets.h"

//--------------------------------------------------------------------------------------------

extern const SKSE::TaskInterface* g_task;

extern RE::BGSKeyword* keyword;
extern RE::BGSArtObject* art;

//--------------------------------------------------------------------------------------------

RE::SoulLevel GetSoulLevel(RE::TESSoulGem* a_soulGem, RE::InventoryEntryData* entry)
{
	if (a_soulGem->containedSoul != RE::SoulLevel::kNone)
	{
		return a_soulGem->containedSoul;
	}

	if (entry->extraList)
	{
		for (auto& list : *entry->extraList)
		{
			auto xSoul = static_cast<RE::ExtraSoul*>(list->GetByType(RE::ExtraDataType::kSoul));

			if (xSoul)
			{
				return static_cast<RE::SoulLevel>(xSoul->level);
			}
		}
	}

	return RE::SoulLevel::kNone;
}

bool VerifyKeywords(RE::TESForm* form, RE::BSScript::VMArray<RE::BGSKeyword*>* keywords)
{
	auto size = keywords->size();

	if (size > 0)
	{
		auto pKeywords = skyrim_cast<RE::BGSKeywordForm*>(form);

		if (pKeywords)
		{
			RE::BGSKeyword* keyword = nullptr;

			for (UInt32 i = 0; i < size; i++)
			{
				keyword = keywords->at(i);

				if (keyword && pKeywords->HasKeyword(keyword))
				{
					return true;
				}
			}
		}
	}

	return false;
}

// navmesh related functions
float CalcLinearDistance(const RE::NiPoint3& a_lhs, const RE::NiPoint3& a_rhs)
{
	return ((a_rhs.x - a_lhs.x) * (a_rhs.x - a_lhs.x)) + ((a_rhs.y - a_lhs.y) * (a_rhs.y - a_lhs.y)) + ((a_rhs.z - a_lhs.z) * (a_rhs.z - a_lhs.z));
}

std::optional<RE::NiPoint3> FindNearestVertex(const RE::TESObjectREFR* a_ref)
{
	auto cell = a_ref->GetParentCell();
	if (!cell || !cell->navMeshes)
	{
		return std::nullopt;
	}
	auto& navMeshes = *cell->navMeshes;

	auto shortestDistance = std::numeric_limits<float>::max();
	std::optional<RE::NiPoint3> pos = std::nullopt;

	for (auto& navMesh : navMeshes)
	{
		for (auto& vertex : navMesh->vertices)
		{
			auto linearDistance = CalcLinearDistance(a_ref->pos, vertex);
			if (linearDistance < shortestDistance)
			{
				shortestDistance = linearDistance;
				pos.emplace(vertex);
			}
		}
	}

	return pos;
}

// time
/*std::vector<SInt32> GetGameStartDate()
{
	std::vector<SInt32> vec;
	vec.reserve(3);

	auto singleton = RE::BSTimeManager::GetSingleton();

	RE::TESGlobal* g_gameDay = singleton->day;
	RE::TESGlobal* g_gameMonth = singleton->month;
	RE::TESGlobal* g_gameYear = singleton->year;
	RE::TESGlobal* g_daysElapsed = singleton->daysPassed;

	if (g_gameDay && g_gameMonth && g_gameYear && g_daysElapsed)
	{
		SInt32 currentDay = g_gameDay->value;
		SInt32 currentMonth = g_gameMonth->value;
		SInt32 currentYear = g_gameYear->value;
		float  daysElapsed = g_daysElapsed->value;

		auto firstYear = currentYear - static_cast<SInt32>(daysElapsed / 365);
		auto firstMonth = currentMonth - static_cast<SInt32>((fmodf(daysElapsed, 365) / singleton->DAYS_IN_MONTH[currentMonth]));
		auto dayOffset = static_cast<SInt32>(fmodf(fmodf(daysElapsed, 365), 30) / 1);

		SInt32 firstDay = currentDay - dayOffset;
		if (firstDay < 0)
		{
			firstDay += singleton->DAYS_IN_MONTH[firstMonth];
		}

		vec.push_back(firstYear);
		vec.push_back(firstMonth);
		vec.push_back(firstDay);
	}
	else
	{
		vec.push_back(201);
		vec.push_back(8);
		vec.push_back(17);
	}

	return vec;
}*/

void StopAllShaders_Internal(RE::TESObjectREFR* thisRef)
{
	auto singleton = RE::Unk141EBEAD0::GetSingleton();

	singleton->activeEffectShadersLock.Lock();
	for (auto& shaderReferenceEffect : singleton->activeEffectShaders)
	{
		if (!shaderReferenceEffect)
		{
			continue;
		}

		auto refHandle = thisRef->CreateRefHandle();

		if (shaderReferenceEffect->refHandle != refHandle)
		{
			continue;
		}

		shaderReferenceEffect->unk40 = 1;
	}
	singleton->activeEffectShadersLock.Unlock();
}

void ResetAlphaAndHead(RE::Actor* thisActor, RE::BSGeometry* geometry)
{
	g_task->AddTask([thisActor, geometry]()
	{
		auto faceNode = thisActor->GetFaceGenNiNode();

		if (faceNode)
		{
			for (UInt32 i = 0; i < faceNode->children.GetSize(); i++)
			{
				auto object = faceNode->children.GetAt(i).get();

				if (object)
				{
					object->flags &= ~0x01;
				}
			}

			faceNode->flags &= ~0x01;
		}

		SetShaderPropertyAlpha(geometry, 1.0, true);

		SetArmorSkinAlpha(thisActor, RE::BGSBipedObjectForm::FirstPersonFlag::kBody, 1.0);
		SetArmorSkinAlpha(thisActor, RE::BGSBipedObjectForm::FirstPersonFlag::kHands, 1.0);
		SetArmorSkinAlpha(thisActor, RE::BGSBipedObjectForm::FirstPersonFlag::kFeet, 1.0);
		SetArmorSkinAlpha(thisActor, RE::BGSBipedObjectForm::FirstPersonFlag::kTail, 1.0); //tail
		SetArmorSkinAlpha(thisActor, RE::BGSBipedObjectForm::FirstPersonFlag::kDecapitate, 1.0); //decap
	});
}

void ResetTint(RE::Actor* thisActor)
{
	auto thisNPC = thisActor->GetActorBase();

	if (thisNPC)
	{
		RE::NiColorA val;
		val.red = thisNPC->textureLighting.red / 255.0;
		val.green = thisNPC->textureLighting.green / 255.0;
		val.blue = thisNPC->textureLighting.blue / 255.0;
		RE::NiColorA* skinColor = &val;

		RE::NiNode* model = thisActor->GetNiRootNode(0);

		if (model)
		{
			model->UpdateModelSkin(&skinColor);
		}
	}
}

template <typename V, typename A>
void FillVMArray(std::vector<V> const& vec, RE::BSScript::VMArray<A>& array)
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

//--------------------------------------------------------------------------------------------
// ACTOR
//--------------------------------------------------------------------------------------------

void PO3_SKSEFunctions::GetHairColor(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::BGSColorForm* color)
{
	if (!thisActor || !color || !thisActor->Is3DLoaded())
	{
		return;
	}

	g_task->AddTask([thisActor, color]()
	{
		RE::BSGeometry* geometry = GetHeadPartGeometry(thisActor, RE::BGSHeadPart::Type::kHair);
		if (!geometry)
		{
			return;
		}

		auto shaderProperty = RE::niptr_cast<RE::BSShaderProperty>(geometry->states[RE::BSGeometry::States::kEffect]);

		if (!shaderProperty)
		{
			return;
		}

		auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(shaderProperty);

		if (lightingShader)
		{
			auto material = lightingShader->material;

			if (material && material->GetType() == RE::BSShaderMaterial::Type::kHairTint)
			{
				auto tintedMaterial = skyrim_cast<RE::BSLightingShaderMaterialHairTint*>(material);
				RE::NiColor tintColor = tintedMaterial->tintColor;

				color->color.red = tintColor.red * 255;
				color->color.green = tintColor.green * 255;
				color->color.blue = tintColor.blue * 255;
			}
		}
	});
}

void PO3_SKSEFunctions::SetHairColor(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::BGSColorForm* color)
{
	if (!thisActor || !thisActor->Is3DLoaded())
	{
		return;
	}

	RE::NiColorA val;
	val.red = color->color.red / 255.0;
	val.green = color->color.green / 255.0;
	val.blue = color->color.blue / 255.0;
	RE::NiColorA* hairColor = &val;

	RE::NiNode* model = thisActor->GetNiRootNode(0);

	if (model)
	{
		model->UpdateModelHair(&hairColor);
	}
}

void PO3_SKSEFunctions::GetSkinColor(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::BGSColorForm* color)
{
	if (!thisActor || !color || thisActor->Is3DLoaded())
	{
		return;
	}

	g_task->AddTask([thisActor, color]()
	{
		RE::BSGeometry* geometry = GetArmorGeometry(thisActor, RE::BGSBipedObjectForm::FirstPersonFlag::kBody, RE::BSShaderMaterial::Type::kFaceGenRGBTint);

		if (!geometry)
		{
			geometry = GetArmorGeometry(thisActor, RE::BGSBipedObjectForm::FirstPersonFlag::kHands, RE::BSShaderMaterial::Type::kFaceGenRGBTint);
		}

		if (!geometry)
		{
			geometry = GetArmorGeometry(thisActor, RE::BGSBipedObjectForm::FirstPersonFlag::kFeet, RE::BSShaderMaterial::Type::kFaceGenRGBTint);
		}

		if (!geometry)
		{
			geometry = GetArmorGeometry(thisActor, RE::BGSBipedObjectForm::FirstPersonFlag::kTail, RE::BSShaderMaterial::Type::kFaceGenRGBTint);
		}

		if (geometry)
		{
			auto shaderProperty = RE::niptr_cast<RE::BSShaderProperty>(geometry->states[RE::BSGeometry::States::kEffect]);

			if (!shaderProperty)
			{
				return;
			}

			auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(shaderProperty);

			if (lightingShader)
			{
				auto material = lightingShader->material;

				if (material && material->GetType() == RE::BSShaderMaterial::Type::kFaceGenRGBTint)
				{
					auto tintedMaterial = skyrim_cast<RE::BSLightingShaderMaterialFacegenTint*>(material);
					RE::NiColor tintColor = tintedMaterial->tintColor;

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

void PO3_SKSEFunctions::SetSkinColor(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::BGSColorForm* color)
{
	if (!thisActor || !thisActor->Is3DLoaded() || !color)
	{
		return;
	}

	g_task->AddTask([thisActor, color]()
	{
		RE::BSGeometry* geometry = GetHeadPartGeometry(thisActor, RE::BGSHeadPart::Type::kFace);
		SetShaderPropertyRGBTint(geometry);

		RE::NiColorA val;
		val.red = color->color.red / 255.0;
		val.green = color->color.green / 255.0;
		val.blue = color->color.blue / 255.0;
		RE::NiColorA* skinColor = &val;

		RE::NiNode* model = thisActor->GetNiRootNode(0);

		if (model)
		{
			model->UpdateModelSkin(&skinColor);
		}
	});
}

void PO3_SKSEFunctions::MixColorWithSkinTone(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::BGSColorForm* color, bool manualMode, float percentage)
{
	if (!thisActor || !thisActor->Is3DLoaded() || !color)
	{
		return;
	}

	g_task->AddTask([thisActor, color, percentage, manualMode]()
	{
		auto thisNPC = thisActor->GetActorBase();

		if (!thisNPC)
		{
			return;
		}

		RE::BSGeometry* geometry = GetHeadPartGeometry(thisActor, RE::BGSHeadPart::Type::kFace);
		SetShaderPropertyRGBTint(geometry); //makes face tintable

		float skinLuminance;

		if (manualMode)
		{
			skinLuminance = percentage;
		}
		else
		{
			skinLuminance = calculateLuminance(thisNPC->textureLighting.red, thisNPC->textureLighting.green, thisNPC->textureLighting.blue);
		}

		UInt8 colorRed = colorMix(color->color.red, thisNPC->textureLighting.red, skinLuminance);
		UInt8 colorGreen = colorMix(color->color.green, thisNPC->textureLighting.green, skinLuminance);
		UInt8 colorBlue = colorMix(color->color.blue, thisNPC->textureLighting.blue, skinLuminance);

		RE::NiColorA val;
		val.red = colorRed / 255.0;
		val.green = colorGreen / 255.0;
		val.blue = colorBlue / 255.0;
		RE::NiColorA* multipliedColor = &val;

		RE::NiNode* model = thisActor->GetNiRootNode(0);

		if (model)
		{
			model->UpdateModelSkin(&multipliedColor);
		}
	});
}

void PO3_SKSEFunctions::SetSkinAlpha(RE::StaticFunctionTag*, RE::Actor* thisActor, float alpha)
{
	if (!thisActor || !thisActor->Is3DLoaded())
	{
		return;
	}

	g_task->AddTask([thisActor, alpha]()
	{
		RE::BSGeometry* geometry = GetHeadPartGeometry(thisActor, RE::BGSHeadPart::Type::kFace);
		SetShaderPropertyAlpha(geometry, alpha, true);

		SetArmorSkinAlpha(thisActor, RE::BGSBipedObjectForm::FirstPersonFlag::kBody, alpha);
		SetArmorSkinAlpha(thisActor, RE::BGSBipedObjectForm::FirstPersonFlag::kHands, alpha);
		SetArmorSkinAlpha(thisActor, RE::BGSBipedObjectForm::FirstPersonFlag::kFeet, alpha);
		SetArmorSkinAlpha(thisActor, RE::BGSBipedObjectForm::FirstPersonFlag::kTail, alpha); //tail
		SetArmorSkinAlpha(thisActor, RE::BGSBipedObjectForm::FirstPersonFlag::kDecapitateHead, alpha); //decap
	});
}

void PO3_SKSEFunctions::EquipArmorIfSkinVisible(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::TESObjectARMO* armorToCheck, RE::TESObjectARMO* armorToEquip)
{
	if (!thisActor || !thisActor->Is3DLoaded() || !armorToCheck || !armorToEquip)
	{
		return;
	}

	g_task->AddTask([thisActor, armorToCheck, armorToEquip]()
	{
		for (auto& arma : armorToCheck->armature)
		{
			if (!arma)
			{
				continue;
			}

			RE::NiAVObject* armorNode = VisitArmorAddon(thisActor, armorToCheck, arma);

			if (!armorNode)
			{
				continue;
			}

			auto node = armorNode->GetAsNiNode();

			if (node)
			{
				for (UInt32 i = 0; i < node->children.GetSize(); i++)
				{
					auto object = node->children.GetAt(i).get();

					if (object)
					{
						if (object && GetShaderPropertyType(object->GetAsBSGeometry()) == RE::BSShaderMaterial::Type::kFaceGenRGBTint)
						{
							thisActor->EquipItem(armorToEquip, 1, false, 0, 0);
							return;
						}
					}
				}
			}
			else
			{
				if (GetShaderPropertyType(armorNode->GetAsBSGeometry()) == RE::BSShaderMaterial::Type::kFaceGenRGBTint)
				{
					thisActor->EquipItem(armorToEquip, 1, false, 0, 0);
					return;
				}
			}
		}
	});
}

void PO3_SKSEFunctions::ReplaceArmorTextureSet(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::TESObjectARMO* thisArmor, RE::BGSTextureSet* sourceTXST, RE::BGSTextureSet* targetTXST, SInt32 textureType)
{
	if (!thisActor || !thisActor->Is3DLoaded() || !thisArmor)
	{
		return;
	}

	g_task->AddTask([thisActor, thisArmor, sourceTXST, targetTXST, textureType]()
	{
		for (auto& armorAddon : thisArmor->armature)
		{
			if (armorAddon)
			{
				RE::NiAVObject* armorNode = VisitArmorAddon(thisActor, thisArmor, armorAddon);

				if (armorNode)
				{
					auto node = armorNode->GetAsNiNode();

					if (node)
					{
						for (UInt32 i = 0; i < node->children.GetSize(); i++)
						{
							auto object = node->children.GetAt(i).get();

							if (object)
							{
								ReplaceTextureSet(object->GetAsBSGeometry(), sourceTXST, targetTXST, textureType);
							}
						}
					}
					else
					{
						ReplaceTextureSet(armorNode->GetAsBSGeometry(), sourceTXST, targetTXST, textureType);
					}
				}
			}
		}
	});
}

void PO3_SKSEFunctions::ReplaceSkinTextureSet(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::BGSTextureSet* maleTXST, RE::BGSTextureSet* femaleTXST, UInt32 slotMask, SInt32 textureType)
{
	if (!thisActor || !thisActor->Is3DLoaded())
	{
		return;
	}

	bool isFemale = false;

	auto actorBase = thisActor->GetActorBase();

	if (actorBase)
	{
		RE::TESNPC::Sex gender = actorBase->GetSex();
		if (gender == RE::TESNPC::Sex::kFemale)
		{
			isFemale = true;
		}
	}

	if (isFemale)
	{
		SetArmorSkinTXST(thisActor, femaleTXST, static_cast<RE::BGSBipedObjectForm::BipedBodyTemplate::FirstPersonFlag>(slotMask), textureType);
	}
	else
	{
		SetArmorSkinTXST(thisActor, maleTXST, static_cast<RE::BGSBipedObjectForm::BipedBodyTemplate::FirstPersonFlag>(slotMask), textureType);
	}
}

void PO3_SKSEFunctions::ReplaceFaceTextureSet(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::BGSTextureSet* maleTXST, RE::BGSTextureSet* femaleTXST, SInt32 textureType)
{
	if (!thisActor || !thisActor->Is3DLoaded())
	{
		return;
	}

	bool isFemale = false;

	auto actorBase = thisActor->GetActorBase();

	if (actorBase)
	{
		RE::TESNPC::Sex gender = actorBase->GetSex();
		if (gender == RE::TESNPC::Sex::kFemale)
		{
			isFemale = true;
		}
	}

	g_task->AddTask([thisActor, maleTXST, femaleTXST, textureType, isFemale]()
	{
		RE::BSGeometry* faceGeometry = GetHeadPartGeometry(thisActor, RE::BGSHeadPart::Type::kFace);

		if (isFemale)
		{
			ReplaceSkinTXST(faceGeometry, femaleTXST, textureType);
		}
		else
		{
			ReplaceSkinTXST(faceGeometry, maleTXST, textureType);
		}
	});
}

RE::BGSTextureSet* PO3_SKSEFunctions::GetHeadPartTextureSet(RE::StaticFunctionTag*, RE::Actor* thisActor, UInt32 type)
{
	if (!thisActor || !thisActor->Is3DLoaded())
	{
		return nullptr;
	}

	RE::TESNPC* actorBase = thisActor->GetActorBase();

	if (actorBase)
	{
		RE::BGSHeadPart* headpart = actorBase->GetCurrentHeadPartByType(static_cast<RE::BGSHeadPart::Type>(type));

		if (headpart)
		{
			return headpart->textureSet;
		}
	}

	return nullptr;
}

void PO3_SKSEFunctions::SetHeadPartTextureSet(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::BGSTextureSet* headpartTXST, UInt32 type)
{
	if (!thisActor || !thisActor->Is3DLoaded() || !headpartTXST)
	{
		return;
	}

	auto actorBase = skyrim_cast<RE::TESNPC*>(thisActor->baseForm);

	if (actorBase)
	{
		RE::BGSHeadPart* headpart = actorBase->GetCurrentHeadPartByType(static_cast<RE::BGSHeadPart::Type>(type));

		if (headpart)
		{
			headpart->textureSet = headpartTXST;
		}
	}
}

void PO3_SKSEFunctions::SetHeadPartAlpha(RE::StaticFunctionTag*, RE::Actor* thisActor, UInt32 partType, float alpha)
{
	if (!thisActor || !thisActor->Is3DLoaded())
	{
		return;
	}

	g_task->AddTask([thisActor, partType, alpha]()
	{
		RE::BSGeometry* geometry = GetHeadPartGeometry(thisActor, static_cast<RE::BGSHeadPart::Type>(partType));
		SetShaderPropertyAlpha(geometry, alpha, false);
	});
}

void PO3_SKSEFunctions::ToggleChildNode(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::BSFixedString nodeName, bool disable)
{
	if (!thisActor)
	{
		return;
	}

	RE::NiNode* parent = thisActor->GetNiNode();

	if (!parent)
	{
		return;
	}

	RE::NiAVObject* child = nullptr;
	if (nodeName == "faceGenNiNodeSkinned")
	{
		child = thisActor->GetFaceGenNiNode();
	}
	else
	{
		child = parent->GetObjectByName(nodeName);
	}

	if (child)
	{
		g_task->AddTask([child, disable]()
		{
			auto node = child->GetAsNiNode();

			if (node)
			{
				for (UInt32 i = 0; i < node->children.GetSize(); i++)
				{
					auto object = node->children.GetAt(i).get();

					if (object)
					{
						if (!disable)
						{
							object->flags &= ~0x01;
						}
						else
						{
							object->flags |= 0x01;
						}
					}
				}

				if (!disable)
				{
					child->flags &= ~0x01;
				}
				else
				{
					child->flags |= 0x01;
				}
			}
			else
			{
				if (!disable)
				{
					child->flags &= ~0x01;
				}
				else
				{
					child->flags |= 0x01;
				}
			}
		});
	}
}

void PO3_SKSEFunctions::RemoveChildNode(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::BSFixedString nodeName)
{
	if (!thisActor)
	{
		return;
	}

	RE::NiNode* parent = thisActor->GetNiNode();

	if (!parent)
	{
		return;
	}

	RE::NiAVObject* child = nullptr;
	if (nodeName == "faceGenNiNodeSkinned")
	{
		child = thisActor->GetFaceGenNiNode();
	}
	else
	{
		child = parent->GetObjectByName(nodeName);
	}

	if (child)
	{
		g_task->AddTask([parent, child]()
		{
			parent->RemoveChild(child);
		});
	}
}

bool PO3_SKSEFunctions::IsActorSoulTrapped(RE::StaticFunctionTag*, RE::Actor* thisActor)
{
	//return CALL_MEMBER_FN(thisActor->processManager, IsSoulTrapped)() //can't use RE function because ST overhaul mods may bypass vanilla SoulTrap()

	if (!thisActor)
	{
		return false;
	}

	auto effects = thisActor->GetActiveEffects();

	if (!effects)
	{
		return false;
	}

	RE::Actor* thisCaster = nullptr;
	RE::EffectSetting* mgef = nullptr;

	for (auto& effect : *effects)
	{
		if (!effect)
		{
			continue;
		}

		mgef = effect->GetBaseObject();

		if (!mgef)
		{
			continue;
		}

		if (mgef->data.hitEffectArt != art && mgef->data.archetype != RE::EffectSetting::Data::Archetype::kSoulTrap) //only soultrap
		{
			continue;
		}

		//got soul-trap

		thisCaster = effect->GetCasterActor();

		if (!thisCaster)
		{
			return false;
		}

		bool isNPC = false;

		if (thisActor->HasKeyword(keyword))
		{
			isNPC = true;
		}

		auto exChanges = static_cast<RE::ExtraContainerChanges*>(thisCaster->extraData.GetByType(RE::ExtraDataType::kContainerChanges)); //loop through caster inventory
		RE::InventoryChanges* changes = exChanges ? exChanges->changes : nullptr;

		if (changes && changes->entryList)
		{
			for (RE::InventoryEntryData* data : *changes->entryList)
			{
				RE::TESForm* item = data->type;

				if (!item->IsSoulGem())
				{
					continue;
				}

				auto thisSoulGem = skyrim_cast<RE::TESSoulGem*>(item); //get soulgem

				if (thisSoulGem)
				{
					RE::SoulLevel soulGemSize = GetSoulLevel(thisSoulGem, data);

					if (soulGemSize == RE::SoulLevel::kNone)
					{
						if (isNPC)
						{
							if (thisSoulGem->flags & RE::TESSoulGem::RecordFlags::kCanHoldNPCSoul)
							{
								return true;
							}
						}
						else
						{
							UInt8 actorSoulSize = thisActor->GetSoulSize();

							if (thisSoulGem->maximumCapacity >= static_cast<RE::SoulLevel>(actorSoulSize))
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

RE::BSScript::VMArray<RE::TESForm*> PO3_SKSEFunctions::AddAllEquippedItemsToArray(RE::StaticFunctionTag*, RE::Actor* thisActor)
{
	RE::BSScript::VMArray<RE::TESForm*> result;
	std::vector<RE::TESForm*> vec;

	if (thisActor)
	{
		auto exChanges = static_cast<RE::ExtraContainerChanges*>(thisActor->extraData.GetByType(RE::ExtraDataType::kContainerChanges)); //loop through caster inventory
		RE::InventoryChanges* changes = exChanges ? exChanges->changes : nullptr;

		if (changes && changes->entryList)
		{
			for (RE::InventoryEntryData* data : *changes->entryList)
			{
				if (data->extraList)
				{
					for (RE::BaseExtraList* extraList : *data->extraList)
					{
						auto worn = static_cast<RE::ExtraWorn*>(extraList->GetByType(RE::ExtraDataType::kWorn));
						auto wornLeft = static_cast<RE::ExtraWornLeft*>(extraList->GetByType(RE::ExtraDataType::kWornLeft));

						if (worn || wornLeft)
						{
							vec.push_back(data->type);
						}
					}
				}
			}
		}
	}

	FillVMArray(vec, result);

	return result;
}

bool PO3_SKSEFunctions::ResetActor3D(RE::StaticFunctionTag*, RE::Actor* thisActor)
{
	if (thisActor && thisActor->Is3DLoaded())
	{
		RE::BSGeometry* headGeometry = GetHeadPartGeometry(thisActor, RE::BGSHeadPart::Type::kFace);
		auto type = GetShaderPropertyModdedSkin(headGeometry, false);

		if (type == 0)
		{
			auto geometry = GetArmorGeometry(thisActor, RE::BGSBipedObjectForm::FirstPersonFlag::kBody, RE::BSShaderMaterial::Type::kFaceGenRGBTint);

			type = GetShaderPropertyModdedSkin(geometry, true);
		}
		if (type == 0)
		{
			auto geometry = GetArmorGeometry(thisActor, RE::BGSBipedObjectForm::FirstPersonFlag::kHands, RE::BSShaderMaterial::Type::kFaceGenRGBTint);

			type = GetShaderPropertyModdedSkin(geometry, true);
		}
		if (type == 0)
		{
			auto geometry = GetArmorGeometry(thisActor, RE::BGSBipedObjectForm::FirstPersonFlag::kFeet, RE::BSShaderMaterial::Type::kFaceGenRGBTint);

			type = GetShaderPropertyModdedSkin(geometry, true);
		}

		if (type == 0)
		{
			return false;
		}

		auto player = RE::PlayerCharacter::GetSingleton();

		if (thisActor != player)
		{
			StopAllShaders_Internal(thisActor);
		}

		if (type == 1)
		{
			if (thisActor != player)
			{
				thisActor->ResetInventory(false);
			}
			
			ResetAlphaAndHead(thisActor, headGeometry);

			return true;
		}
		else if (type == 2)
		{
			if (thisActor == player)
			{
				if (!thisActor->IsOnMount())
				{
					thisActor->QueueNiNodeUpdate(false);
				}
			}
			else
			{
				ResetTint(thisActor);
			}

			return true;
		}
		else if (type == 3)
		{
			if (thisActor == player)
			{
				if (!thisActor->IsOnMount())
				{
					thisActor->QueueNiNodeUpdate(false);
				}
			}
			else
			{
				thisActor->ResetInventory(false);
				
				ResetTint(thisActor);
				ResetAlphaAndHead(thisActor, headGeometry);
			}

			return true;
		}
	}

	return false;
}

void PO3_SKSEFunctions::DecapitateActor(RE::StaticFunctionTag*, RE::Actor* thisActor)
{
	if (!thisActor)
	{
		return;
	}

	thisActor->Decapitate();
}

//gets actor's time of death
float PO3_SKSEFunctions::GetTimeDead(RE::StaticFunctionTag*, RE::Actor* thisActor)
{
	if (thisActor && thisActor->processManager)
	{
		float timeOfDeath = thisActor->processManager->timeOfDeath;

		if (timeOfDeath > 0.0)
		{
			auto g_gameDaysPassed = RE::BSTimeManager::GetSingleton()->daysPassed;

			if (g_gameDaysPassed)
			{
				return floorf(g_gameDaysPassed->value * 24.0) - timeOfDeath;
			}
		}
	}

	return 0.0;
}

//gets actor's time of death
float PO3_SKSEFunctions::GetTimeOfDeath(RE::StaticFunctionTag*, RE::Actor* thisActor)
{
	if (thisActor && thisActor->processManager)
	{
		float timeOfDeath = thisActor->processManager->timeOfDeath;

		if (timeOfDeath > 0.0)
		{
			return timeOfDeath / 24.0;
		}
	}

	return 0.0;
}

//gets actor's current package
RE::TESPackage* PO3_SKSEFunctions::GetRunningPackage(RE::StaticFunctionTag*, RE::Actor* thisActor)
{
	if (thisActor)
	{
		return thisActor->GetCurrentPackageInternal();
	}

	return nullptr;
}

bool PO3_SKSEFunctions::IsActorInWater(RE::StaticFunctionTag*, RE::Actor* thisActor)
{
	if (thisActor)
	{
		return (thisActor->flags1 & RE::Actor::Flag1::kIsInWater) != RE::Actor::Flag1::kNone;
	}

	return false;
}

float PO3_SKSEFunctions::GetActorAlpha(RE::StaticFunctionTag*, RE::Actor* thisActor)
{
	if (thisActor)
	{
		auto processManager = thisActor->processManager;

		if (processManager)
		{
			auto middleProcess = processManager->middleProcess;

			if (middleProcess)
			{
				return middleProcess->actorAlpha;
			}
		}
	}

	return 1.0;
}

float PO3_SKSEFunctions::GetActorRefraction(RE::StaticFunctionTag*, RE::Actor* thisActor)
{
	if (thisActor)
	{
		auto processManager = thisActor->processManager;

		if (processManager)
		{
			auto middleProcess = processManager->middleProcess;

			if (middleProcess)
			{
				return middleProcess->actorRefraction;
			}
		}
	}

	return 1.0;
}

void PO3_SKSEFunctions::SetActorRefraction(RE::StaticFunctionTag*, RE::Actor* thisActor, float refraction)
{
	if (thisActor)
	{
		auto processManager = thisActor->processManager;

		if (processManager)
		{
			processManager->SetActorRefraction(refraction);

			float invisibility = thisActor->GetActorValueCurrent(RE::ActorValue::kInvisibility); //invisibility

			if (invisibility < 0.0 || invisibility <= 1.0 && invisibility <= 0.0 || thisActor != RE::PlayerCharacter::GetSingleton())
			{
				if (refraction <= 0.0)
				{
					thisActor->UpdateRefractionProperty(0, refraction);
					thisActor->UpdateRefractionWithExtraData();
				}
				else
				{
					thisActor->UpdateRefractionProperty(1, refraction);
				}
			}
			else
			{
				thisActor->UpdateRefraction(1.0);

				refraction = 1.0 - refraction / 100.0;
				refraction = 1.0 + (0.01 - 1.0) * ((refraction - 0.0) / (1.0 - 0.0));

				thisActor->UpdateRefractionProperty(1, refraction);
			}
		}
	}
}

SInt32 PO3_SKSEFunctions::GetActorState(RE::StaticFunctionTag*, RE::Actor* thisActor)
{
	if (thisActor)
	{
		auto flags = thisActor->flags08;

		return static_cast<UInt32>(pun_bits(flags.secondaryAnimState1, flags.secondaryAnimState2, flags.secondaryAnimState3, flags.secondaryAnimState4));
	}

	return -1;
}

bool PO3_SKSEFunctions::InstantKill(RE::StaticFunctionTag*, RE::Actor* thisActor)
{
	return thisActor ? thisActor->InstantKill() : false;
}

/*void PO3_SKSEFunctions::SetShaderType(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::TESObjectARMO* templateArmor)
{
	if (!thisActor || !thisActor->Is3DLoaded() || !templateArmor)
	{
		return;
	}

	g_task->AddTask([thisActor, templateArmor]()
	{
		RE::BSGeometry * templateGeometry = GetTemplateArmorGeometry(thisActor, templateArmor, 11);

		if (!templateGeometry)
		{
			_MESSAGE("no template geometry");
			return;
		}

		RE::BSGeometry * geometry = GetHeadPartGeometry(thisActor, RE::BGSHeadPart::kTypeFace);
		SetShaderPropertyMLP(geometry, templateGeometry);

		SetArmorSkinShaderType(thisActor, templateGeometry, RE::BGSBipedObjectForm::FirstPersonFlag::kBody);
		SetArmorSkinShaderType(thisActor, templateGeometry, RE::BGSBipedObjectForm::FirstPersonFlag::kHands);
		SetArmorSkinShaderType(thisActor, templateGeometry, RE::BGSBipedObjectForm::FirstPersonFlag::kFeet);
		SetArmorSkinShaderType(thisActor, templateGeometry, RE::BGSBipedObjectForm::FirstPersonFlag::kUnnamed21); //decap
		SetArmorSkinShaderType(thisActor, templateGeometry, RE::BGSBipedObjectForm::FirstPersonFlag::kTail); //tail
	})
}*/

//--------------------------------------------------------------------------------------------
// ARRAY
//--------------------------------------------------------------------------------------------

bool PO3_SKSEFunctions::AddStringToArray(RE::StaticFunctionTag*, RE::BSFixedString thisString, RE::BSScript::VMArray<RE::BSFixedString> stringArray)
{
	auto length = stringArray.size();

	if (length > 0)
	{
		RE::BSFixedString string = nullptr;

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

bool PO3_SKSEFunctions::AddActorToArray(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::BSScript::VMArray<RE::Actor*> actorArray)
{
	auto length = actorArray.size();

	if (length > 0)
	{
		RE::Actor* actor = nullptr;

		for (UInt32 i = 0; i < length; i++)
		{
			actor = actorArray[i];

			if (actor == nullptr)
			{
				actorArray[i] = thisActor;
				return true;
			}
		}
	}

	return false;
}

//count how many instances of string are found in an array
UInt32 PO3_SKSEFunctions::ArrayStringCount(RE::StaticFunctionTag*, RE::BSFixedString thisString, RE::BSScript::VMArray<RE::BSFixedString> stringArray)
{
	UInt32 count = 0;

	auto length = stringArray.size();

	if (length > 0)
	{
		RE::BSFixedString string = nullptr;

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
RE::BSScript::VMArray<RE::BSFixedString> PO3_SKSEFunctions::SortArrayString(RE::StaticFunctionTag*, RE::BSScript::VMArray<RE::BSFixedString> stringArray)
{
	RE::BSScript::VMArray<RE::BSFixedString> result;

	auto length = stringArray.size();

	if (length > 0)
	{
		std::string str;
		std::vector<std::string> vec;

		vec.reserve(length);

		RE::BSFixedString string = nullptr;

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
float PO3_SKSEFunctions::GetEffectShaderFullParticleCount(RE::StaticFunctionTag*, RE::TESEffectShader* thisEffectShader)
{
	return thisEffectShader ? thisEffectShader->data.particleShaderFullParticleBirthRatio : 0.0;
}

//sets effect shader particle count
void PO3_SKSEFunctions::SetEffectShaderFullParticleCount(RE::StaticFunctionTag*, RE::TESEffectShader* thisEffectShader, float particleCount)
{
	if (thisEffectShader)
	{
		thisEffectShader->data.particleShaderFullParticleBirthRatio = particleCount;
	}
}

//get effect shader persistant particle count
float PO3_SKSEFunctions::GetEffectShaderPersistentParticleCount(RE::StaticFunctionTag*, RE::TESEffectShader* thisEffectShader)
{
	return thisEffectShader ? thisEffectShader->data.particleShaderPersistantParticleCount : 0.0;
}

//set effect shader persistant particle count
void PO3_SKSEFunctions::SetEffectShaderPersistentParticleCount(RE::StaticFunctionTag*, RE::TESEffectShader* thisEffectShader, float particleCount)
{
	if (thisEffectShader)
	{
		thisEffectShader->data.particleShaderPersistantParticleCount = particleCount;
	}
}

bool PO3_SKSEFunctions::IsEffectShaderFlagSet(RE::StaticFunctionTag*, RE::TESEffectShader* thisEffectShader, UInt32 flag)
{
	return thisEffectShader ? (static_cast<UInt32>(thisEffectShader->data.flags)& flag) == flag : false;
}

void PO3_SKSEFunctions::SetEffectShaderFlag(RE::StaticFunctionTag*, RE::TESEffectShader* thisEffectShader, UInt32 flag)
{
	if (thisEffectShader)
	{
		thisEffectShader->data.flags |= static_cast<RE::TESEffectShader::Data::Flag>(flag);
	}
}

void PO3_SKSEFunctions::ClearEffectShaderFlag(RE::StaticFunctionTag*, RE::TESEffectShader* thisEffectShader, UInt32 flag)
{
	if (thisEffectShader)
	{
		thisEffectShader->data.flags &= ~static_cast<RE::TESEffectShader::Data::Flag>(flag);
	}
}

//--------------------------------------------------------------------------------------------
// FORM
//--------------------------------------------------------------------------------------------

// replaces keyword on form 
void PO3_SKSEFunctions::ReplaceKeywordOnForm(RE::StaticFunctionTag*, RE::TESForm* thisForm, RE::BGSKeyword* KYWDtoRemove, RE::BGSKeyword* KYWDtoAdd)
{
	if (!thisForm || !KYWDtoRemove || !KYWDtoAdd)
	{
		return;
	}

	auto pKeywords = skyrim_cast<RE::BGSKeywordForm*>(thisForm);

	if (pKeywords)
	{
		UInt32 removeIndex = 0;
		RE::BGSKeyword* thisKYWD = nullptr;
		bool found = false;

		for (size_t i = 0; i < pKeywords->GetSize(); i++)
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
void PO3_SKSEFunctions::AddKeywordToForm(RE::StaticFunctionTag*, RE::TESForm* thisForm, RE::BGSKeyword* KYWDtoAdd)
{
	if (!thisForm || !KYWDtoAdd)
	{
		return;
	}

	auto pKeywords = skyrim_cast<RE::BGSKeywordForm*>(thisForm);

	if (pKeywords)
	{
		auto oldData = pKeywords->keywords;

		pKeywords->keywords = RE::calloc<RE::BGSKeyword*>(++pKeywords->keywordCount);
		if (oldData)
		{
			for (UInt32 i = 0; i < pKeywords->keywordCount - 1; ++i)
			{
				pKeywords->keywords[i] = oldData[i];
			}

			pKeywords->keywords[pKeywords->keywordCount - 1] = KYWDtoAdd;

			RE::free(oldData);
			oldData = nullptr;
		}
	}
}

//--------------------------------------------------------------------------------------------
// GAME
//--------------------------------------------------------------------------------------------

bool PO3_SKSEFunctions::IsPluginFound(RE::StaticFunctionTag*, RE::BSFixedString name)
{
	auto dataHandler = RE::TESDataHandler::GetSingleton();
	const RE::TESFile* modInfo = dataHandler->LookupModByName(name);

	if (modInfo)
	{
		return modInfo->IsLoaded();
	}

	return false;
}

RE::BSScript::VMArray<RE::TESForm*> PO3_SKSEFunctions::GetAllSpellsInMod(RE::StaticFunctionTag*, RE::BSFixedString modName, RE::BSScript::VMArray<RE::BGSKeyword*> keywords, bool isPlayable)
{
	RE::BSScript::VMArray<RE::TESForm*> result;

	auto dataHandler = RE::TESDataHandler::GetSingleton();
	const RE::TESFile* modInfo = dataHandler->LookupModByName(modName.c_str());

	if (!modInfo || !modInfo->IsLoaded())
	{
		return result;
	}

	std::vector<RE::TESForm*> vec;

	if (isPlayable)
	{
		RE::SpellItem* spell = nullptr;

		for (auto& book : dataHandler->GetFormArray<RE::TESObjectBOOK>())
		{
			if (!modInfo->IsFormInMod(book->formID))
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
		for (auto& spell : dataHandler->GetFormArray<RE::SpellItem>())
		{
			if (!modInfo->IsFormInMod(spell->formID))
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

RE::BSScript::VMArray<RE::TESForm*> PO3_SKSEFunctions::GetAllRacesInMod(RE::StaticFunctionTag*, RE::BSFixedString modName, RE::BSScript::VMArray<RE::BGSKeyword*> keywords)
{
	RE::BSScript::VMArray<RE::TESForm*> result;

	auto dataHandler = RE::TESDataHandler::GetSingleton();
	const RE::TESFile* modInfo = dataHandler->LookupModByName(modName.c_str());

	if (!modInfo || !modInfo->IsLoaded())
	{
		return result;
	}

	std::vector<RE::TESForm*> vec;

	for (auto& race : dataHandler->GetFormArray<RE::TESRace>())
	{
		if (!modInfo->IsFormInMod(race->formID))
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

void PO3_SKSEFunctions::AddAllGameSpellsToList(RE::StaticFunctionTag*, RE::BGSListForm* thisList, RE::BSScript::VMArray<RE::BGSKeyword*> keywords, bool isPlayable)
{
	if (!thisList)
	{
		return;
	}

	auto dataHandler = RE::TESDataHandler::GetSingleton();

	if (isPlayable)
	{
		RE::SpellItem* spell = nullptr;

		for (auto& book : dataHandler->GetFormArray<RE::TESObjectBOOK>())
		{
			spell = book->data.teaches.spell;

			if (!spell || spell && VerifyKeywords(spell, &keywords))
			{
				continue;
			}

			thisList->AddFormToList(spell);
		}
	}
	else
	{
		for (auto& spell : dataHandler->GetFormArray<RE::SpellItem>())
		{
			if (!VerifyKeywords(spell, &keywords))
			{
				continue;
			}

			thisList->AddFormToList(spell);
		}
	}
}

void PO3_SKSEFunctions::AddAllGameRacesToList(RE::StaticFunctionTag*, RE::BGSListForm* thisList, RE::BSScript::VMArray<RE::BGSKeyword*> keywords)
{
	if (!thisList)
	{
		return;
	}

	auto dataHandler = RE::TESDataHandler::GetSingleton();

	for (auto& race : dataHandler->GetFormArray<RE::TESRace>())
	{
		if (VerifyKeywords(race, &keywords))
		{
			continue;
		}

		thisList->AddFormToList(race);
	}
}

//gets actors by AI processing level - see https://geck.bethsoft.com/index.php?title=GetActorsByProcessingLevel
RE::BSScript::VMArray<RE::Actor*> PO3_SKSEFunctions::GetActorsByProcessingLevel(RE::StaticFunctionTag*, UInt32 level)
{
	RE::BSScript::VMArray<RE::Actor*> result;
	std::vector<RE::Actor*> vec;

	auto singleton = RE::Unk141EBEAD0::GetSingleton();
	RE::BSTArray<RE::RefHandle>* arr = nullptr;

	switch (level)
	{
	case 0:
		arr = &singleton->actorsHigh;
		break;
	case 1:
		arr = &singleton->actorsMiddleHigh;
		break;
	case 2:
		arr = &singleton->actorsMiddleLow;
		break;
	case 3:
		arr = &singleton->actorsLow;
		break;
	default:
		arr = nullptr;
		break;
	}

	if (arr)
	{
		RE::TESObjectREFRPtr refPtr;
		RE::Actor* actor = nullptr;

		for (auto& refHandle : *arr)
		{
			RE::TESObjectREFR::LookupByHandle(refHandle, refPtr);

			actor = RE::niptr_cast<RE::Actor>(refPtr);

			if (!actor)
			{
				continue;
			}

			vec.push_back(actor);
		}
	}

	FillVMArray(vec, result);

	return result;
}

//gets amount of actors in high process
SInt32 PO3_SKSEFunctions::GetNumActorsInHigh(RE::StaticFunctionTag*)
{
	return RE::Unk141EBEAD0::GetSingleton()->numActorsInHighProcess;
}


//--------------------------------------------------------------------------------------------
// LIGHT
//--------------------------------------------------------------------------------------------

float PO3_SKSEFunctions::GetLightRadius(RE::StaticFunctionTag*, RE::TESObjectLIGH* thisLight)
{
	return thisLight ? static_cast<float>(thisLight->data.radius) : 0.0;
}

void PO3_SKSEFunctions::SetLightRadius(RE::StaticFunctionTag*, RE::TESObjectLIGH* thisLight, float radius)
{
	if (thisLight)
	{
		thisLight->data.radius = static_cast<UInt32>(radius);
	}
}

float PO3_SKSEFunctions::GetLightFade(RE::StaticFunctionTag*, RE::TESObjectLIGH* thisLight)
{
	return thisLight ? thisLight->fadeValue : 0.0;
}

void PO3_SKSEFunctions::SetLightFade(RE::StaticFunctionTag*, RE::TESObjectLIGH* thisLight, float fadeValue)
{
	if (thisLight)
	{
		thisLight->fadeValue = fadeValue;
	}
}

RE::BGSColorForm* PO3_SKSEFunctions::GetLightColor(RE::StaticFunctionTag*, RE::TESObjectLIGH* thisLight)
{
	if (thisLight)
	{
		auto factory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::BGSColorForm>();
		auto colorForm = factory->Create();

		if (colorForm)
		{
			colorForm->flags &= ~RE::BGSColorForm::Flag::kPlayable;

			colorForm->color.red = thisLight->data.color.red;
			colorForm->color.green = thisLight->data.color.green;
			colorForm->color.blue = thisLight->data.color.blue;

			return colorForm;
		}
	}

	return nullptr;
}

void PO3_SKSEFunctions::SetLightColor(RE::StaticFunctionTag*, RE::TESObjectLIGH* thisLight, RE::BGSColorForm* colorForm)
{
	if (thisLight && colorForm)
	{
		thisLight->data.color.red = colorForm->color.red;
		thisLight->data.color.green = colorForm->color.green;
		thisLight->data.color.blue = colorForm->color.blue;
	}
}

UInt32 PO3_SKSEFunctions::GetLightTypeInternal(RE::TESObjectLIGH* thisLight)
{
	if (thisLight)
	{
		const auto flags = thisLight->data.flags;

		if ((flags & RE::TESObjectLIGH::Data::Flag::kFlag_TypeHemiShadow) == RE::TESObjectLIGH::Data::Flag::kFlag_TypeHemiShadow)
		{
			return 1;
		}
		if ((flags & RE::TESObjectLIGH::Data::Flag::kFlag_TypeOmni) == RE::TESObjectLIGH::Data::Flag::kFlag_TypeOmni)
		{
			return 2;
		}
		if ((flags & RE::TESObjectLIGH::Data::Flag::kFlag_TypeOmniShadow) == RE::TESObjectLIGH::Data::Flag::kFlag_TypeOmniShadow)
		{
			return 3;
		}
		if ((flags & RE::TESObjectLIGH::Data::Flag::kFlag_TypeSpot) == RE::TESObjectLIGH::Data::Flag::kFlag_TypeSpot)
		{
			return 4;
		}
		if ((flags & RE::TESObjectLIGH::Data::Flag::kFlag_TypeSpotShadow) == RE::TESObjectLIGH::Data::Flag::kFlag_TypeSpotShadow)
		{
			return 5;
		}
	}

	return 0;
}

UInt32 PO3_SKSEFunctions::GetLightType(RE::StaticFunctionTag*, RE::TESObjectLIGH* thisLight)
{
	return GetLightTypeInternal(thisLight);
}

void PO3_SKSEFunctions::SetLightType(RE::StaticFunctionTag*, RE::TESObjectLIGH* thisLight, UInt32 lightType)
{
	if (!thisLight)
	{
		return;
	}

	auto flags = thisLight->data.flags;

	switch (lightType)
	{
	case 1:
		flags = flags & ~(RE::TESObjectLIGH::Data::Flag::kFlags_Type | RE::TESObjectLIGH::Data::Flag::kFlag_TypeHemiShadow);
		break;
	case 2:
		flags = flags & ~(RE::TESObjectLIGH::Data::Flag::kFlags_Type | RE::TESObjectLIGH::Data::Flag::kFlag_TypeOmni);
		break;
	case 3:
		flags = flags & ~(RE::TESObjectLIGH::Data::Flag::kFlags_Type | RE::TESObjectLIGH::Data::Flag::kFlag_TypeOmniShadow);
		break;
	case 4:
		flags = flags & ~(RE::TESObjectLIGH::Data::Flag::kFlags_Type | RE::TESObjectLIGH::Data::Flag::kFlag_TypeSpot);
		break;
	case 5:
		flags = flags & ~(RE::TESObjectLIGH::Data::Flag::kFlags_Type | RE::TESObjectLIGH::Data::Flag::kFlag_TypeSpotShadow);
		break;
	default:
		return;
	}

	thisLight->data.flags = flags;
}

float PO3_SKSEFunctions::GetLightFOV(RE::StaticFunctionTag*, RE::TESObjectLIGH* thisLight)
{
	return thisLight ? thisLight->data.fov : 0.0;
}

void PO3_SKSEFunctions::SetLightFOV(RE::StaticFunctionTag*, RE::TESObjectLIGH* thisLight, float FOV)
{
	if (thisLight)
	{
		thisLight->data.fov = FOV;
	}
}

float PO3_SKSEFunctions::GetLightShadowDepthBias(RE::StaticFunctionTag*, RE::TESObjectREFR* thisLightObject)
{
	if (!thisLightObject)
	{
		return 1.0;
	}

	auto thisLight = skyrim_cast<RE::TESObjectLIGH*>(thisLightObject->baseForm);

	if (thisLight)
	{
		auto xLightData = static_cast<RE::ExtraLightData*>(thisLightObject->extraData.GetByType(RE::ExtraDataType::kLightData));

		if (xLightData)
		{
			return xLightData->depthBias;
		}
	}

	return 1.0;
}

//creates extralightdata if none exists
void PO3_SKSEFunctions::SetLightShadowDepthBias(RE::StaticFunctionTag*, RE::TESObjectREFR* thisLightObject, float depthBias)
{
	if (!thisLightObject)
		return;

	auto thisLight = skyrim_cast<RE::TESObjectLIGH*>(thisLightObject->baseForm);

	if (thisLight)
	{
		auto xLightData = static_cast<RE::ExtraLightData*>(thisLightObject->extraData.GetByType(RE::ExtraDataType::kLightData));

		if (xLightData)
		{
			xLightData->depthBias = depthBias;
		}
		else
		{
			auto&& newLightData = RE::ExtraLightData::ExtraLightData();
			newLightData.depthBias = depthBias;
			(&thisLightObject->extraData)->Add(&newLightData);
		}
	}
}

//--------------------------------------------------------------------------------------------
// LOCATION
//--------------------------------------------------------------------------------------------

RE::BGSLocation* PO3_SKSEFunctions::GetParentLocation(RE::StaticFunctionTag*, RE::BGSLocation* thisLocation)
{
	return thisLocation ? thisLocation->parentLocation : nullptr;
}

void PO3_SKSEFunctions::SetParentLocation(RE::StaticFunctionTag*, RE::BGSLocation* thisLocation, RE::BGSLocation* newLocation)
{
	if (thisLocation)
	{
		thisLocation->parentLocation = newLocation;
	}
}

//--------------------------------------------------------------------------------------------
// MATHS
//--------------------------------------------------------------------------------------------

// based on mersenne twister
float PO3_SKSEFunctions::GenerateRandomFloat(RE::StaticFunctionTag*, float afMin, float afMax)
{
	std::random_device rd;

	std::mt19937 engine{ rd() };

	std::uniform_real_distribution<float> dist(afMin, afMax);

	return dist(engine);
}

UInt32 PO3_SKSEFunctions::GenerateRandomInt(RE::StaticFunctionTag*, UInt32 afMin, UInt32 afMax)
{
	std::random_device rd;

	std::mt19937 engine{ rd() };

	std::uniform_int_distribution<UInt32>dist(afMin, afMax);

	return dist(engine);
}

//--------------------------------------------------------------------------------------------
// MAGICEFFECT
//--------------------------------------------------------------------------------------------

RE::BSScript::VMArray<RE::EffectSetting*> PO3_SKSEFunctions::GetAllActiveEffectsOnActor(RE::StaticFunctionTag*, RE::Actor* thisActor, bool showInactive)
{
	RE::BSScript::VMArray<RE::EffectSetting*> result;
	std::vector<RE::EffectSetting*> vec;

	if (thisActor)
	{
		auto effects = thisActor->GetActiveEffects();

		if (effects)
		{
			RE::EffectSetting* mgef = nullptr;

			for (auto& effect : *effects)
			{
				if (!effect)
				{
					continue;
				}

				mgef = effect->GetBaseObject();

				if (mgef)
				{
					if (!showInactive && (static_cast<UInt32>(effect->flags & RE::ActiveEffect::Flag::kInactive) || static_cast<UInt32>(mgef->data.flags & RE::EffectSetting::Data::Flag::kHideInUI)))
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
RE::BSFixedString PO3_SKSEFunctions::GetEffectArchetypeInternal(RE::EffectSetting* mgef)
{
	RE::BSFixedString archetype = nullptr;

	if (!mgef)
	{
		return archetype;
	}

	switch (static_cast<UInt32>(mgef->data.archetype))
	{
	case 0:
		archetype = "ValueMod";
		break;
	case 1:
		archetype = "Script";
		break;
	case 2:
		archetype = "Dispel";
		break;
	case 3:
		archetype = "CureDisease";
		break;
	case 4:
		archetype = "Absorb";
		break;
	case 5:
		archetype = "DualValueMod";
		break;
	case 6:
		archetype = "Calm";
		break;
	case 7:
		archetype = "Demoralize";
		break;
	case 8:
		archetype = "Frenzy";
		break;
	case 9:
		archetype = "Disarm";
		break;
	case 10:
		archetype = "CommandSummoned";
		break;
	case 11:
		archetype = "Invisibility";
		break;
	case 12:
		archetype = "Light";
		break;
	case 15:
		archetype = "Lock";
		break;
	case 16:
		archetype = "Open";
		break;
	case 17:
		archetype = "BoundWeapon";
		break;
	case 18:
		archetype = "SummonCreature";
		break;
	case 19:
		archetype = "DetectLife";
		break;
	case 20:
		archetype = "Telekinesis";
		break;
	case 21:
		archetype = "Paralysis";
		break;
	case 22:
		archetype = "Reanimate";
		break;
	case 23:
		archetype = "SoulTrap";
		break;
	case 24:
		archetype = "TurnUndead";
		break;
	case 25:
		archetype = "Guide";
		break;
	case 26:
		archetype = "WerewolfFeed";
		break;
	case 27:
		archetype = "CureParalysis";
		break;
	case 28:
		archetype = "CureAddiction";
		break;
	case 29:
		archetype = "CurePoison";
		break;
	case 30:
		archetype = "Concussion";
		break;
	case 31:
		archetype = "ValueAndParts";
		break;
	case 32:
		archetype = "AccumulateMagnitude";
		break;
	case 33:
		archetype = "Stagger";
		break;
	case 34:
		archetype = "PeakValueMod";
		break;
	case 35:
		archetype = "Cloak";
		break;
	case 36:
		archetype = "Werewolf";
		break;
	case 37:
		archetype = "SlowTime";
		break;
	case 38:
		archetype = "Rally";
		break;
	case 39:
		archetype = "EnhanceWeapon";
		break;
	case 40:
		archetype = "SpawnHazard";
		break;
	case 41:
		archetype = "Etherealize";
		break;
	case 42:
		archetype = "Banish";
		break;
	case 44:
		archetype = "Disguise";
		break;
	case 45:
		archetype = "GrabActor";
		break;
	case 46:
		archetype = "VampireLord";
		break;
	default:
		break;
	}

	return archetype;
}

bool PO3_SKSEFunctions::HasMagicEffectWithArchetype(RE::StaticFunctionTag*, RE::Actor* thisActor, RE::BSFixedString archetype)
{
	if (thisActor)
	{
		auto effects = thisActor->GetActiveEffects();

		if (!effects)
		{
			return false;
		}

		RE::EffectSetting* mgef = nullptr;

		for (auto& effect : *effects)
		{
			if (!effect)
			{
				continue;
			}

			mgef = effect->GetBaseObject();

			if (mgef && GetEffectArchetypeInternal(mgef) == archetype)
			{
				return true;
			}
		}
	}

	return false;
}

UInt32 PO3_SKSEFunctions::GetEffectArchetypeAsInt(RE::StaticFunctionTag*, RE::EffectSetting* mgef)
{
	return mgef ? static_cast<UInt32>(mgef->data.archetype) : 0;
}

RE::BSFixedString PO3_SKSEFunctions::GetEffectArchetypeAsString(RE::StaticFunctionTag*, RE::EffectSetting* mgef)
{
	return GetEffectArchetypeInternal(mgef);
}

RE::BGSSoundDescriptorForm* PO3_SKSEFunctions::GetMagicEffectSound(RE::StaticFunctionTag*, RE::EffectSetting* mgef, UInt32 type)
{
	if (mgef)
	{
		for (auto& effectSound : mgef->sounds)
		{
			if (effectSound.type == static_cast<RE::EffectSetting::SoundInfo::Type>(type))
			{
				return effectSound.sound;
			}
		}
	}

	return nullptr;
}

void PO3_SKSEFunctions::SetMagicEffectSound(RE::StaticFunctionTag*, RE::EffectSetting* mgef, RE::BGSSoundDescriptorForm* mgefSound, UInt32 type)
{
	if (mgef && mgefSound)
	{
		for (auto& effectSound : mgef->sounds)
		{
			if (effectSound.type == static_cast<RE::EffectSetting::SoundInfo::Type>(type))
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
RE::BSScript::VMArray<float> PO3_SKSEFunctions::GetPositionAsArray(RE::StaticFunctionTag*, RE::TESObjectREFR* ref)
{
	RE::BSScript::VMArray<float> pos;
	pos.resize(3);

	if (ref)
	{
		pos[0] = ref->pos.x;
		pos[1] = ref->pos.y;
		pos[2] = ref->pos.z;
	}

	return pos;
}

// [angleX, angleY, angleZ]
RE::BSScript::VMArray<float> PO3_SKSEFunctions::GetRotationAsArray(RE::StaticFunctionTag*, RE::TESObjectREFR* ref)
{
	RE::BSScript::VMArray<float> angles;
	angles.resize(3);

	if (ref)
	{
		angles[0] = ref->rot.x * (180.0 / MATH_PI);
		angles[1] = ref->rot.y * (180.0 / MATH_PI);
		angles[2] = ref->rot.z * (180.0 / MATH_PI);
	}

	return angles;
}

bool PO3_SKSEFunctions::IsLoadDoor(RE::StaticFunctionTag*, RE::TESObjectREFR* thisDoor)
{
	if (thisDoor)
	{
		//Get the RE::ExtraTeleport
		auto teleport = skyrim_cast<RE::ExtraTeleport*>(thisDoor->extraData.GetByType(RE::ExtraDataType::kTeleport));

		if (teleport)
		{
			return true;
		}
	}

	return false;
}

// internal checker
bool PO3_SKSEFunctions::CanItemBeTaken(RE::InventoryEntryData* data, bool noEquipped, bool noFavourited, bool noQuestItem)
{
	if (data->extraList)
	{
		for (auto& extraList : *data->extraList)
		{
			if (noEquipped)
			{
				auto worn = static_cast<RE::ExtraWorn*>(extraList->GetByType(RE::ExtraDataType::kWorn));
				auto wornLeft = static_cast<RE::ExtraWornLeft*>(extraList->GetByType(RE::ExtraDataType::kWornLeft));

				if (worn || wornLeft)
				{
					return false;
				}
			}

			if (noFavourited)
			{
				auto HotKey = static_cast<RE::ExtraHotkey*>(extraList->GetByType(RE::ExtraDataType::kHotkey));

				if (HotKey)
				{
					return false;
				}
			}

			if (noQuestItem)
			{
				auto aliases = static_cast<RE::ExtraAliasInstanceArray*>(extraList->GetByType(RE::ExtraDataType::kAliasInstanceArray));

				if (aliases)
				{
					for (auto& alias : aliases->aliasInfoArr)
					{
						RE::TESQuest* quest = alias->quest;
						RE::BGSBaseAlias* refAlias = alias->alias;

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

void PO3_SKSEFunctions::AddAllInventoryItemsToList(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef, RE::BGSListForm* thisList, bool noEquipped, bool noFavourited, bool noQuestItem)
{
	if (thisRef && thisList)
	{
		auto exChanges = static_cast<RE::ExtraContainerChanges*>(thisRef->extraData.GetByType(RE::ExtraDataType::kContainerChanges)); //loop through caster inventory
		RE::InventoryChanges* changes = exChanges ? exChanges->changes : nullptr;

		if (changes && changes->entryList)
		{
			for (RE::InventoryEntryData* data : *changes->entryList)
			{
				if (CanItemBeTaken(data, noEquipped, noFavourited, noQuestItem))
				{
					thisList->AddFormToList(data->type);
				}
			}
		}
	}
}

RE::BSScript::VMArray<RE::TESForm*> PO3_SKSEFunctions::AddAllInventoryItemsToArray(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef, bool noEquipped, bool noFavourited, bool noQuestItem)
{
	RE::BSScript::VMArray<RE::TESForm*> result;
	std::vector<RE::TESForm*> vec;

	if (thisRef)
	{
		auto exChanges = static_cast<RE::ExtraContainerChanges*>(thisRef->extraData.GetByType(RE::ExtraDataType::kContainerChanges)); //loop through caster inventory
		RE::InventoryChanges* changes = exChanges ? exChanges->changes : nullptr;

		if (changes && changes->entryList)
		{
			for (RE::InventoryEntryData* data : *changes->entryList)
			{
				if (CanItemBeTaken(data, noEquipped, noFavourited, noQuestItem))
				{
					vec.push_back(data->type);
				}
			}
		}
	}

	FillVMArray(vec, result);

	return result;
}

// replaces keyword on ref 
void PO3_SKSEFunctions::ReplaceKeywordOnRef(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef, RE::BGSKeyword* KYWDtoRemove, RE::BGSKeyword* KYWDtoAdd)
{
	if (!thisRef || !KYWDtoRemove || !KYWDtoAdd)
	{
		return;
	}

	auto pKeywords = skyrim_cast<RE::BGSKeywordForm*>(thisRef->baseForm);

	if (pKeywords)
	{
		UInt32 removeIndex = 0;
		RE::BGSKeyword* thisKYWD = nullptr;
		bool found = false;

		for (size_t i = 0; i < pKeywords->GetSize(); i++)
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
void PO3_SKSEFunctions::AddKeywordToRef(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef, RE::BGSKeyword* KYWDtoAdd)
{
	if (!thisRef || !KYWDtoAdd || thisRef->HasKeyword(KYWDtoAdd))
	{
		return;
	}

	auto pKeywords = skyrim_cast<RE::BGSKeywordForm*>(thisRef->baseForm);

	if (pKeywords)
	{
		auto oldData = pKeywords->keywords;

		pKeywords->keywords = RE::calloc<RE::BGSKeyword*>(++pKeywords->keywordCount);
		if (oldData)
		{
			for (UInt32 i = 0; i < pKeywords->keywordCount - 1; ++i)
			{
				pKeywords->keywords[i] = oldData[i];
			}

			pKeywords->keywords[pKeywords->keywordCount - 1] = KYWDtoAdd;

			RE::free(oldData);
			oldData = nullptr;
		}
	}
}

//calculates a 2D vector
void PO3_SKSEFunctions::Apply2DHavokImpulse(RE::StaticFunctionTag*, RE::TESObjectREFR* source, RE::TESObjectREFR* target, float afZ, float magnitude)
{
	if (!source || !source->Is3DLoaded() || !target || !target->Is3DLoaded())
	{
		return;
	}

	g_task->AddTask([source, target, afZ, magnitude]()
	{
		auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();

		float sourceZ = source->rot.z * static_cast<float>(180.0 / MATH_PI);
		float angleZ = sourceZ + GetHeadingAngle(vm, 0, source, target); //source.getanglez() + source.getheadingangle(target)

		ApplyHavokImpulse(vm, 0, target, asinf(angleZ), acosf(angleZ), afZ, magnitude);
	});
}

//calculates a 3D vector and takes into account the elevation between source and target. 
void PO3_SKSEFunctions::Apply3DHavokImpulse(RE::StaticFunctionTag*, RE::TESObjectREFR* source, RE::TESObjectREFR* target, float magnitude)
{
	if (!source || !source->Is3DLoaded() || !target || !target->Is3DLoaded())
	{
		return;
	}

	g_task->AddTask([source, target, magnitude]()
	{
		float dx = target->pos.x - source->pos.x;
		float dy = target->pos.y - source->pos.y;
		float dz = target->pos.z - source->pos.z;

		float dist = std::sqrtf((dx * dx) + (dy * dy) + (dz * dz));

		float x = dx / dist; //x
		float y = dy / dist; //y
		float z = dz / dist; //z

		ApplyHavokImpulse(RE::BSScript::Internal::VirtualMachine::GetSingleton(), 0, target, x, y, z, magnitude);
	});
}

//moves object to nearest navemesh location
void PO3_SKSEFunctions::MoveToNearestNavmeshLocation(RE::StaticFunctionTag*, RE::TESObjectREFR* target)
{
	auto nearestVertex = FindNearestVertex(target);
	if (!nearestVertex)
	{
		return;
	}

	auto handle = target->CreateRefHandle();
	g_task->AddTask([handle, nearestVertex]()
	{
		RE::TESObjectREFRPtr ref;
		RE::TESObjectREFR::LookupByHandle(handle, ref);

		if (!ref)
		{
			return;
		}

		ref->SetPosition(std::move(*nearestVertex));
	});
}

RE::BSScript::VMArray<RE::TESEffectShader*> PO3_SKSEFunctions::GetAllEffectShaders(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef)
{
	RE::BSScript::VMArray<RE::TESEffectShader*> result;
	std::vector<RE::TESEffectShader*> vec;

	if (thisRef)
	{
		auto singleton = RE::Unk141EBEAD0::GetSingleton();

		singleton->activeEffectShadersLock.Lock();
		for (auto& shaderReferenceEffect : singleton->activeEffectShaders)
		{
			if (!shaderReferenceEffect)
			{
				continue;
			}

			auto handle = thisRef->CreateRefHandle();

			if (shaderReferenceEffect->refHandle != handle)
			{
				continue;
			}

			auto shader = shaderReferenceEffect->effectShader;

			if (shader)
			{
				vec.push_back(shader);
			}
		}
		singleton->activeEffectShadersLock.Unlock();
	}

	FillVMArray(vec, result);

	return result;
}

UInt32 PO3_SKSEFunctions::HasEffectShader(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef, RE::TESEffectShader* effectShader)
{
	UInt32 effectShaderCount = 0;

	if (thisRef && effectShader)
	{
		auto singleton = RE::Unk141EBEAD0::GetSingleton();

		singleton->activeEffectShadersLock.Lock();
		for (auto& shaderReferenceEffect : singleton->activeEffectShaders)
		{
			if (!shaderReferenceEffect)
			{
				continue;
			}

			auto handle = thisRef->CreateRefHandle();

			if (shaderReferenceEffect->refHandle != handle)
			{
				continue;
			}

			auto shader = shaderReferenceEffect->effectShader;

			if (shader && shader == effectShader)
			{
				effectShaderCount++;
			}
		}
		singleton->activeEffectShadersLock.Unlock();
	}

	return effectShaderCount;
}

RE::BSScript::VMArray<RE::BGSArtObject*> PO3_SKSEFunctions::GetAllArtObjects(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef)
{
	RE::BSScript::VMArray<RE::BGSArtObject*> result;
	std::vector<RE::BGSArtObject*> vec;

	if (thisRef)
	{
		auto singleton = RE::Unk141EBEAD0::GetSingleton();

		singleton->activeEffectShadersLock.Lock();
		for (auto& shaderReferenceEffect : singleton->activeEffectShaders)
		{
			if (!shaderReferenceEffect)
			{
				continue;
			}

			auto handle = thisRef->CreateRefHandle();

			if (shaderReferenceEffect->refHandle != handle)
			{
				continue;
			}

			auto art = shaderReferenceEffect->artObject;

			if (art)
			{
				vec.push_back(art);
			}
		}
		singleton->activeEffectShadersLock.Unlock();
	}

	FillVMArray(vec, result);

	return result;
}

UInt32 PO3_SKSEFunctions::HasArtObject(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef, RE::BGSArtObject* artObject)
{
	UInt32 artObjectCount = 0;

	if (thisRef && artObject)
	{
		auto singleton = RE::Unk141EBEAD0::GetSingleton();

		singleton->activeEffectShadersLock.Lock();
		for (auto& shaderReferenceEffect : singleton->activeEffectShaders)
		{
			if (!shaderReferenceEffect)
			{
				continue;
			}

			auto handle = thisRef->CreateRefHandle();

			if (shaderReferenceEffect->refHandle != handle)
			{
				continue;
			}

			auto art = shaderReferenceEffect->artObject;

			if (art && art == artObject)
			{
				artObjectCount++;
			}
		}
		singleton->activeEffectShadersLock.Unlock();
	}

	return artObjectCount;
}

void PO3_SKSEFunctions::StopArtObject(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef, RE::BGSArtObject* artObject)
{
	if (thisRef)
	{
		RE::Unk141EBEAD0::GetSingleton()->StopArtObject(thisRef, artObject);
	}
}

void PO3_SKSEFunctions::StopAllShaders(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef)
{
	if (thisRef)
	{
		StopAllShaders_Internal(thisRef);
	}
}

RE::Actor* PO3_SKSEFunctions::GetActorCause(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef)
{
	RE::Actor* actor = nullptr;

	if (thisRef)
	{
		RE::TESObjectREFRPtr refPtr;

		RE::TESObjectREFR::LookupByHandle(*thisRef->GetActorCause(), refPtr);

		actor = RE::niptr_cast<RE::Actor>(refPtr);

		if (actor)
		{
			return actor;
		}
	}

	return actor;
}

RE::Actor* PO3_SKSEFunctions::GetClosestActorFromRef(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef, float radius)
{
	if (thisRef)
	{
		auto squaredRadius = radius * radius;
		auto shortestDistance = std::numeric_limits<float>::max();

		auto originPos = thisRef->pos;

		std::map<float, RE::Actor*> map;
		RE::TESObjectREFRPtr refPtr;

		auto singleton = RE::Unk141EBEAD0::GetSingleton();

		for (auto& refHandle : singleton->actorsHigh)
		{
			RE::TESObjectREFR::LookupByHandle(refHandle, refPtr);

			auto actor = RE::niptr_cast<RE::Actor>(refPtr);

			if (!actor || actor == thisRef)
			{
				continue;
			}

			auto distance = CalcLinearDistance(originPos, actor->pos);

			if (distance > squaredRadius)
			{
				continue;
			}

			map.try_emplace(distance, actor);

			if (distance < shortestDistance)
			{
				shortestDistance = distance;
			}
		}

		auto player = RE::PlayerCharacter::GetSingleton();

		if (thisRef != player)
		{
			auto distance = CalcLinearDistance(originPos, player->pos);

			if (distance <= squaredRadius)
			{
				map.try_emplace(distance, player);

				if (distance < shortestDistance)
				{
					shortestDistance = distance;
				}
			}
		}

		return map.find(shortestDistance)->second;
	}

	return nullptr;
}

RE::Actor* PO3_SKSEFunctions::GetRandomActorFromRef(RE::StaticFunctionTag*, RE::TESObjectREFR* thisRef, float radius)
{
	if (thisRef)
	{
		auto squaredRadius = radius * radius;

		auto originPos = thisRef->pos;

		std::vector<RE::Actor*> vec;
		RE::TESObjectREFRPtr refPtr;

		auto singleton = RE::Unk141EBEAD0::GetSingleton();

		vec.reserve(singleton->numActorsInHighProcess);

		for (auto& refHandle : singleton->actorsHigh)
		{
			RE::TESObjectREFR::LookupByHandle(refHandle, refPtr);

			auto actor = RE::niptr_cast<RE::Actor>(refPtr);

			if (!actor || actor == thisRef)
			{
				continue;
			}

			auto distance = CalcLinearDistance(originPos, actor->pos);

			if (distance > squaredRadius)
			{
				continue;
			}

			vec.emplace_back(actor);
		}

		auto player = RE::PlayerCharacter::GetSingleton();

		if (thisRef != player)
		{
			auto distance = CalcLinearDistance(originPos, player->pos);

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

//--------------------------------------------------------------------------------------------
// PACKAGE
//--------------------------------------------------------------------------------------------

SInt32 PO3_SKSEFunctions::GetPackageType(RE::StaticFunctionTag*, RE::TESPackage* package)
{
	if (package)
	{
		return static_cast<UInt32>(package->packData.type);
	}

	return -1;
}

//--------------------------------------------------------------------------------------------
// PROJECTILE
//--------------------------------------------------------------------------------------------

float PO3_SKSEFunctions::GetProjectileSpeed(RE::StaticFunctionTag*, RE::BGSProjectile* thisProjectile)
{
	return thisProjectile ? thisProjectile->data.speed : 0.0;
}

void PO3_SKSEFunctions::SetProjectileSpeed(RE::StaticFunctionTag*, RE::BGSProjectile* thisProjectile, float speed)
{
	if (thisProjectile)
	{
		thisProjectile->data.speed = speed;
	}
}

float PO3_SKSEFunctions::GetProjectileRange(RE::StaticFunctionTag*, RE::BGSProjectile* thisProjectile)
{
	return thisProjectile ? thisProjectile->data.range : 0.0;
}

void PO3_SKSEFunctions::SetProjectileRange(RE::StaticFunctionTag*, RE::BGSProjectile* thisProjectile, float range)
{
	if (thisProjectile)
	{
		thisProjectile->data.range = range;
	}
}

float PO3_SKSEFunctions::GetProjectileGravity(RE::StaticFunctionTag*, RE::BGSProjectile* thisProjectile)
{
	return thisProjectile ? thisProjectile->data.gravity : 0.0;
}

void PO3_SKSEFunctions::SetProjectileGravity(RE::StaticFunctionTag*, RE::BGSProjectile* thisProjectile, float gravity)
{
	if (thisProjectile)
	{
		thisProjectile->data.gravity = gravity;
	}
}

float PO3_SKSEFunctions::GetProjectileImpactForce(RE::StaticFunctionTag*, RE::BGSProjectile* thisProjectile)
{
	return thisProjectile ? thisProjectile->data.impactForce : 0.0;
}

void PO3_SKSEFunctions::SetProjectileImpactForce(RE::StaticFunctionTag*, RE::BGSProjectile* thisProjectile, float impactForce)
{
	if (thisProjectile)
	{
		thisProjectile->data.impactForce = impactForce;
	}
}

UInt32 PO3_SKSEFunctions::GetProjectileType(RE::StaticFunctionTag*, RE::BGSProjectile* thisProjectile)
{
	if (thisProjectile)
	{
		const auto types = thisProjectile->data.types;

		if ((types & RE::BGSProjectile::Data::Type::kMissile) == RE::BGSProjectile::Data::Type::kMissile) //Missile
		{
			return 1;
		}
		if ((types & RE::BGSProjectile::Data::Type::kLobber) == RE::BGSProjectile::Data::Type::kLobber) //Lobber (runes)
		{
			return 2;
		}
		if ((types & RE::BGSProjectile::Data::Type::kBeam) == RE::BGSProjectile::Data::Type::kBeam) //Beam
		{
			return 3;
		}
		if ((types & RE::BGSProjectile::Data::Type::kFlame) == RE::BGSProjectile::Data::Type::kFlame) //Flame
		{
			return 4;
		}
		if ((types & RE::BGSProjectile::Data::Type::kCone) == RE::BGSProjectile::Data::Type::kCone) //Cone
		{
			return 5;
		}
		if ((types & RE::BGSProjectile::Data::Type::kBarrier) == RE::BGSProjectile::Data::Type::kBarrier) //Barrier
		{
			return 6;
		}
		if ((types & RE::BGSProjectile::Data::Type::kArrow) == RE::BGSProjectile::Data::Type::kArrow) //Arrow
		{
			return 7;
		}
	}

	return 0;
}

//--------------------------------------------------------------------------------------------
// SOUND
//--------------------------------------------------------------------------------------------

void PO3_SKSEFunctions::SetSoundDescriptor(RE::StaticFunctionTag*, RE::TESSound* thisSound, RE::BGSSoundDescriptorForm* thisSoundDescriptor)
{
	if (thisSound && thisSoundDescriptor)
	{
		thisSound->soundDescriptor = thisSoundDescriptor;
	}
}

//--------------------------------------------------------------------------------------------
// SPELL
//--------------------------------------------------------------------------------------------

UInt32 PO3_SKSEFunctions::GetSpellType(RE::StaticFunctionTag*, RE::SpellItem* thisSpell)
{
	return thisSpell ? static_cast<UInt32>(thisSpell->data.type) : 0;
}

//--------------------------------------------------------------------------------------------
// VISUALEFFECT
//--------------------------------------------------------------------------------------------

RE::BGSArtObject* PO3_SKSEFunctions::GetArtObject(RE::StaticFunctionTag*, RE::BGSReferenceEffect* visualEffect)
{
	return visualEffect ? visualEffect->data.effectArt : nullptr;
}

void PO3_SKSEFunctions::SetArtObject(RE::StaticFunctionTag*, RE::BGSReferenceEffect* visualEffect, RE::BGSArtObject* art)
{
	if (visualEffect && art)
	{
		visualEffect->data.effectArt = art;
	}
}

//--------------------------------------------------------------------------------------------
// WEATHER
//--------------------------------------------------------------------------------------------

//returns wind speed from 0-255 (how it's set up in the weather form)
UInt32 PO3_SKSEFunctions::GetWindSpeedAsInt(RE::StaticFunctionTag*, RE::TESWeather* thisWeather)
{
	return thisWeather ? thisWeather->data.windSpeed : 0;
}

//returns wind speed from 0.0-1.0 (how it's set up in the CK)
float PO3_SKSEFunctions::GetWindSpeedAsFloat(RE::StaticFunctionTag*, RE::TESWeather* thisWeather)
{
	return thisWeather ? static_cast<float>(thisWeather->data.windSpeed / 255.0) : 0.0;
}

SInt32 PO3_SKSEFunctions::GetWeatherType(RE::StaticFunctionTag*, RE::TESWeather* thisWeather)
{
	RE::TESWeather* currentWeather = nullptr;

	if (thisWeather)
	{
		currentWeather = thisWeather;
	}

	if (!currentWeather)
	{
		currentWeather = RE::Sky::GetSingleton()->currentWeather;
	}

	if (currentWeather)
	{
		const auto flags = currentWeather->data.flags;

		if ((flags & RE::TESWeather::Data::Flag::kWeather_Pleasant) != RE::TESWeather::Data::Flag::kNone)
		{
			return 0;
		}
		if ((flags & RE::TESWeather::Data::Flag::kWeather_Cloudy) != RE::TESWeather::Data::Flag::kNone)
		{
			return 1;
		}
		if ((flags & RE::TESWeather::Data::Flag::kWeather_Rainy) != RE::TESWeather::Data::Flag::kNone)
		{
			return 2;
		}
		if ((flags & RE::TESWeather::Data::Flag::kWeather_Snowy) != RE::TESWeather::Data::Flag::kNone)
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
bool PO3_SKSEFunctions::Register(RE::BSScript::Internal::VirtualMachine* a_vm)
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

	a_vm->RegisterFunction("ReplaceKeywordOnForm", "PO3_SKSEFunctions", ReplaceKeywordOnForm);
	a_vm->RegisterFunction("AddKeywordToForm", "PO3_SKSEFunctions", AddKeywordToForm);

	a_vm->RegisterFunction("IsPluginFound", "PO3_SKSEFunctions", IsPluginFound, RE::BSScript::IVirtualMachine::FunctionFlag::kNoWait);
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

	a_vm->RegisterFunction("GenerateRandomFloat", "PO3_SKSEFunctions", GenerateRandomFloat, RE::BSScript::IVirtualMachine::FunctionFlag::kNoWait);
	a_vm->RegisterFunction("GenerateRandomInt", "PO3_SKSEFunctions", GenerateRandomInt, RE::BSScript::IVirtualMachine::FunctionFlag::kNoWait);

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

	a_vm->RegisterFunction("GetWindSpeedAsInt", "PO3_SKSEFunctions", GetWindSpeedAsInt);
	a_vm->RegisterFunction("GetWindSpeedAsFloat", "PO3_SKSEFunctions", GetWindSpeedAsFloat);
	//a_vm->RegisterFunction("GetWeatherType", "PO3_SKSEFunctions", GetWeatherType);

	_MESSAGE("Registered papyrus functions");

	return true;
}

