//credits to underthesky for original implementation, davidjcobb, aers, nukem, meh321, expired for tips and help
//modifications and extra functions by powerofthree
//modified for CommonLib

#include "po3_functions.h"
#include "po3_graphicfunctions.h"

//--------------------------------------------------------------------------------------------

extern const SKSETaskInterface	* g_task;

//--------------------------------------------------------------------------------------------

//MISC FUNCTIONS

//keyword function added by papyrusgamedata
bool po3_SKSEFunctions::VerifyKeywords(RE::TESForm * form, RE::BSScript::VMArray<RE::BGSKeyword*> * keywords)
{
	if (form && keywords->size > 0)
	{
		RE::BGSKeywordForm* pKeywords = skyrim_cast<RE::BGSKeywordForm*>(form);

		if (pKeywords)
		{
			RE::BGSKeyword * keyword = nullptr;

			for (size_t i = 0; i < keywords->size; i++)
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

template <typename V, typename A>
RE::BSScript::VMArray<A> ReturnVMArray(std::vector<V> const& vec, RE::BSScript::VMArray<A> const& array)
{
	auto size = vec.size();

	if (size != 0)
	{
		result.resize(size);

		for (size_t i = 0; i < size; i++)
		{
			array[i] = vec[i];
		}
	}

	return array;
}

//--------------------------------------------------------------------------------------------

//Sets hair color
void po3_SKSEFunctions::SetHairColor(Actor * thisActor, RE::BGSColorForm* color)
{
	if (thisActor && color)
	{
		RE::NiColorA val;
		val.red = color->color.red / 255.0;
		val.green = color->color.green / 255.0;
		val.blue = color->color.blue / 255.0;
		RE::NiColorA * haircolor = &val;

		NiNode * model = thisActor->GetNiRootNode(0);

		if (model)
		{
			UpdateModelHair((NiAVObject*)model, &haircolor); //sets hair color
		}
	}
}

//Sets skin color
void po3_SKSEFunctions::SetSkinColor(Actor * thisActor, RE::BGSColorForm* color)
{
	if (!thisActor || !color)
	{
		return;
	}

	g_task->AddTask(new po3_TaskSetSkinColor(thisActor, color));
}

//Multiplies existing skin color by tint
void po3_SKSEFunctions::MixColorWithSkinTone(Actor * thisActor, RE::BGSColorForm* color, bool manualMode, float percentage)
{
	if (!thisActor || !color || percentage <= 0.0)
	{
		return;
	}

	if (manualMode)
	{
		g_task->AddTask(new po3_TaskManualBlendSkinColor(thisActor, color, percentage));
	}
	else
	{
		g_task->AddTask(new po3_TaskAutoBlendSkinColor(thisActor, color));
	}

}

//Sets skin alpha
void po3_SKSEFunctions::SetSkinAlpha(Actor * thisActor, float alpha)
{
	if (thisActor)
	{
		RE::BSGeometry * headGeometry = nullptr;
		headGeometry = GetHeadPartGeometry(thisActor, BGSHeadPart::kTypeFace);

		if (headGeometry)
		{
			g_task->AddTask(new po3_TaskSetShaderPropertyAlpha(headGeometry, alpha, true));
		}

		SetArmorSkinAlpha(thisActor, BGSBipedObjectForm::kPart_Body, alpha);
		SetArmorSkinAlpha(thisActor, BGSBipedObjectForm::kPart_Hands, alpha);
		SetArmorSkinAlpha(thisActor, BGSBipedObjectForm::kPart_Feet, alpha);
		SetArmorSkinAlpha(thisActor, BGSBipedObjectForm::kPart_Unnamed21, alpha); //decap
		SetArmorSkinAlpha(thisActor, BGSBipedObjectForm::kPart_Unnamed10, alpha); //tail
	}
}

//Sets armor textureset
void po3_SKSEFunctions::ReplaceTextureSetOnArmor(Actor * thisActor, TESObjectARMO * thisArmor, BGSTextureSet * sourceTXST, BGSTextureSet * targetTXST, SInt32 textureType)
{
	if (thisActor && thisArmor)
	{
		TESObjectARMA * arma = NULL;

		for (UInt32 i = 0; i < thisArmor->armorAddons.count; i++)
		{
			if (thisArmor->armorAddons.GetNthItem(i, arma))
			{
				NiAVObject * armorNode = VisitArmorAddon(thisActor, thisArmor, arma);

				if (armorNode)
				{
					NiNode * node = armorNode->GetAsNiNode();

					if (node)
					{
						for (UInt32 i = 0; i < node->m_children.m_emptyRunStart; i++)
						{
							NiAVObject * object = node->m_children.m_data[i];

							if (object)
							{
								BSGeometry * armorGeometry = object->GetAsBSGeometry();

								if (armorGeometry)
								{
									g_task->AddTask(new po3_TaskReplaceTextureSet(armorGeometry, sourceTXST, targetTXST, textureType));
								}
							}
						}
					}
					else
					{
						BSGeometry * armorGeometry = armorNode->GetAsBSGeometry();

						if (armorGeometry)
						{
							g_task->AddTask(new po3_TaskReplaceTextureSet(armorGeometry, sourceTXST, targetTXST, textureType));
						}
					}
				}
			}
		}
	}
}

//Sets skin textureset
void po3_SKSEFunctions::ReplaceSkinTextureSet(Actor * thisActor, BGSTextureSet * maleTXST, BGSTextureSet * femaleTXST, UInt32 slotMask, SInt32 textureType)
{
	if (thisActor)
	{
		TESNPC * actorBase = skyrim_cast<TESNPC*>(thisActor->baseForm);

		bool isFemale = false;

		if (actorBase)
		{
			UInt8 gender = CALL_MEMBER_FN(actorBase, GetSex)();
			isFemale = gender == 1 ? true : false;
		}

		if (isFemale)
		{
			SetArmorSkinTXST(thisActor, femaleTXST, slotMask, textureType);
		}
		else
		{
			SetArmorSkinTXST(thisActor, maleTXST, slotMask, textureType);
		}
	}
}

//Sets face textureset
void po3_SKSEFunctions::ReplaceFaceTextureSet(Actor * thisActor, BGSTextureSet * maleTXST, BGSTextureSet * femaleTXST, SInt32 textureType)
{
	if (thisActor)
	{
		TESNPC * actorBase = skyrim_cast<TESNPC*>(thisActor->baseForm);

		bool isFemale = false;

		if (actorBase)
		{
			UInt8 gender = CALL_MEMBER_FN(actorBase, GetSex)();
			isFemale = gender == 1 ? true : false;
		}

		BSGeometry * faceGeometry = nullptr;
		faceGeometry = GetHeadPartGeometry(thisActor, BGSHeadPart::kTypeFace);

		if (isFemale)
		{
			g_task->AddTask(new po3_TaskReplaceSkinTextureSet(faceGeometry, femaleTXST, textureType));
		}
		else
		{
			g_task->AddTask(new po3_TaskReplaceSkinTextureSet(faceGeometry, maleTXST, textureType));
		}
	}
}

//Gets textureset of specified headpart
RE::BGSTextureSet* po3_SKSEFunctions::GetHeadPartTextureSet(RE::Actor* thisActor, UInt32 type)
{
	if (thisActor)
	{
		RE::TESNPC * actorBase = skyrim_cast<RE::TESNPC*>(thisActor->baseForm);

		if (actorBase)
		{
			RE::BGSHeadPart* headpart = actorBase->GetCurrentHeadPartByType(type);

			if (headpart)
			{
				return headpart->textureSet;
			}
		}
	}

	return nullptr;
}

//Sets textureset of specified headpart - not sure if this works
void po3_SKSEFunctions::SetHeadPartTextureSet(RE::Actor* thisActor, RE::BGSTextureSet* headpartTXST, UInt32 type)
{
	if (thisActor && headpartTXST)
	{
		RE::TESNPC * actorBase = skyrim_cast<RE::TESNPC*>(thisActor->baseForm);

		if (actorBase)
		{
			RE::BGSHeadPart* headpart = actorBase->GetCurrentHeadPartByType(type);

			if (headpart)
			{
				headpart->textureSet = headpartTXST;
			}
		}
	}
}

//Sets headpart alpha
void po3_SKSEFunctions::SetHeadPartAlpha(Actor * thisActor, UInt32 partType, float alpha)
{
	if (!thisActor || alpha < 0.0)
	{
		return;
	}

	BSGeometry * geometry = nullptr;
	geometry = GetHeadPartGeometry(thisActor, partType);

	if (geometry)
	{
		g_task->AddTask(new po3_TaskSetShaderPropertyAlpha(geometry, alpha, false));
	}
}

//toggle blood decals
void po3_SKSEFunctions::ToggleSkinnedDecals(RE::Actor * thisActor, bool disable)
{
	if (!thisActor)
	{
		return;
	}

	RE::NiNode * parent = thisActor->GetNiNode();

	if (parent)
	{
		RE::BSFixedString rootName("Skinned Decal Node");

		RE::NiAVObject * object = parent->GetObjectByName(rootName);

		if (object)
		{
			if (disable)
			{
				object->localTransform.scale = 0.0;
			}
			else
			{
				object->localTransform.scale = 1.0;
			}

			g_task->AddTask(new po3_TaskUpdateWorldData(object));
		}
	}
}

//remove child node from parent 3D
void po3_SKSEFunctions::RemoveChildNode(RE::Actor * thisActor, RE::BSFixedString nodeName)
{
	if (!thisActor)
	{
		return;
	}

	RE::NiNode * parent = thisActor->GetNiNode();

	if (parent)
	{
		RE::NiAVObject * object = parent->GetObjectByName(nodeName);

		if (object)
		{
			g_task->AddTask(new po3_TaskRemoveChildNode(parent, object));
		}
	}
}

//returns whether actor was soultrapped
bool po3_SKSEFunctions::IsActorSoulTrapped(RE::Actor* thisActor)
{
	//return CALL_MEMBER_FN(thisActor->processManager, IsSoulTrapped)() //can't use RE function because ST overhaul mods may bypass vanilla SoulTrap()

	auto keyword = skyrim_cast<RE::BGSKeyword*>(RE::TESForm::LookupByID(KeywordActorTypeNPCID));
	auto art = skyrim_cast<RE::BGSArtObject*>(RE::TESForm::LookupByID(ArtSoulTrapTargetEffectsID));

	if (!thisActor || !art || !keyword)
	{
		return false;
	}

	auto effects = thisActor->GetActiveEffects();

	if (!effects)
	{
		return false;
	}

	RE::Actor * thisCaster = nullptr;
	RE::EffectSetting* mgef = 0;

	for (auto& effect : *effects)
	{
		if (!effect)
		{
			continue;
		}

		mgef = effect ? effect->GetBaseObject() : 0;

		if (mgef)
		{
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

			RE::ExtraContainerChanges* exChanges = static_cast<RE::ExtraContainerChanges*>(thisCaster->extraData.GetByType(ExtraDataType::kExtraData_ContainerChanges)); //loop through caster inventory
			RE::InventoryChanges *changes = exChanges ? exChanges->changes : nullptr;

			if (changes && changes->entryList)
			{
				for (RE::InventoryEntryData *data : *changes->entryList)
				{
					RE::TESForm *item = data->GetOwner();

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
								RE::SoulLevel actorSoulSize = thisActor->GetSoulSize();

								if (thisSoulGem->maximumCapacity >= actorSoulSize)
								{
									return true;
								}
							}
						}
					}
				}
			}
		}
	}

	return false;
}

//resets RE::Actor 3d data if modified
void po3_SKSEFunctions::ResetActor3D(Actor * thisActor)
{
	if (thisActor)
	{
		BSGeometry * geometry = nullptr;
		geometry = GetHeadPartGeometry(thisActor, BGSHeadPart::kTypeFace);

		if (geometry)
		{
			UInt32 type = GetShaderPropertyModdedSkin(geometry);

			if (type == 1)
			{
				if (thisActor == *g_thePlayer)
				{
					papyrusActor::QueueNiNodeUpdate(thisActor);
				}
				else
				{
					g_task->AddTask(new po3_TaskSetShaderPropertyAlpha(geometry, 1.0, true));
				}

				SetArmorSkinAlpha(thisActor, BGSBipedObjectForm::kPart_Body, 1.0);
				SetArmorSkinAlpha(thisActor, BGSBipedObjectForm::kPart_Hands, 1.0);
				SetArmorSkinAlpha(thisActor, BGSBipedObjectForm::kPart_Feet, 1.0);
				SetArmorSkinAlpha(thisActor, BGSBipedObjectForm::kPart_Unnamed21, 1.0); //decap
				SetArmorSkinAlpha(thisActor, BGSBipedObjectForm::kPart_Unnamed10, 1.0); //tail

			}
			else if (type == 2)
			{
				if (thisActor == *g_thePlayer)
				{
					papyrusActor::QueueNiNodeUpdate(thisActor);
				}
				else
				{
					TESNPC * thisNPC = DYNAMIC_CAST(thisActor->baseForm, TESForm, TESNPC);

					if (thisNPC)
					{
						NiColorA val;
						val.r = thisNPC->color.red / 255.0;
						val.g = thisNPC->color.green / 255.0;
						val.b = thisNPC->color.blue / 255.0;
						NiColorA * skinColor = &val;

						NiNode * model = thisActor->GetNiRootNode(0);

						if (model)
						{
							UpdateModelSkin((NiAVObject*)model, &skinColor); //sets body skin color
						}
					}
				}
			}
		}
	}
}

//decapitaRE::TES dead and living actors
void po3_SKSEFunctions::DecapitateActor(RE::Actor * thisActor)
{
	if (!thisActor)
	{
		return;
	}

	thisActor->Decapitate();
}

//add string to array.
bool po3_SKSEFunctions::AddStringToArray(RE::BSFixedString thisString, RE::BSScript::VMArray<RE::BSFixedString> stringArray)
{
	auto length = stringArray.size;

	if (length > 0)
	{
		for (size_t i = 0; i < length; i++)
		{
			RE::BSFixedString string = nullptr;
			string = stringArray[i];

			if (string.c_str() == nullptr)
			{
				stringArray.resize(length + 1);
				stringArray[length - 1] = thisString;
				return true;
			}
		}
	}

	return false;
}

//add RE::Actor to array.
bool po3_SKSEFunctions::AddActorToArray(RE::Actor* thisActor, RE::BSScript::VMArray<RE::Actor*> actorArray)
{
	auto length = actorArray.size;

	if (length > 0)
	{
		for (size_t i = 0; i < length; i++)
		{
			RE::Actor* actor = nullptr;
			actor = actorArray[i];

			if (actor == nullptr)
			{
				actorArray.resize(length + 1);
				actorArray[length - 1] = thisActor;
				return true;
			}
		}
	}

	return false;
}

//count how many instances of string are found in an array
UInt32 po3_SKSEFunctions::ArrayStringCount(RE::BSFixedString thisString, RE::BSScript::VMArray<RE::BSFixedString> stringArray)
{
	UInt32 count = 0;
	auto length = stringArray.size;

	if (length > 0)
	{
		for (size_t i = 0; i < length; i++)
		{
			RE::BSFixedString string = nullptr;
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
RE::BSScript::VMArray<RE::BSFixedString> po3_SKSEFunctions::SortArrayString(RE::BSScript::VMArray<RE::BSFixedString> stringArray)
{
	RE::BSScript::VMArray<RE::BSFixedString> result;
	std::string str;
	std::vector<std::string> tempArray;

	auto count = stringArray.size;

	if (!count)
	{
		return result;
	}

	for (size_t i = 0; i < count; i++)
	{
		RE::BSFixedString string = nullptr;
		string = stringArray.at(i);

		if (string.c_str() != nullptr)
		{
			str = string.c_str();
			tempArray.push_back(str);
		}
	}

	std::sort(tempArray.begin(), tempArray.end());

	result.resize(tempArray.size());

	for (size_t i = 0; i < tempArray.size(); i++)
	{
		RE::BSFixedString tempString = tempArray[i].c_str();
		stringArray[i] = tempString;
	}

	return result;
}

//returns effect shader particle count
float po3_SKSEFunctions::GetEffectShaderFullParticleCount(RE::TESEffectShader* thisEffectShader)
{
	return (thisEffectShader) ? (thisEffectShader->data.particleShaderFullParticleBirthRatio) : 0.0;
}

//sets effect shader particle count
void po3_SKSEFunctions::SetEffectShaderFullParticleCount(RE::TESEffectShader* thisEffectShader, float particleCount)
{
	if (thisEffectShader)
	{
		thisEffectShader->data.particleShaderFullParticleBirthRatio = particleCount;
	}
}

//get effect shader persistant particle count
float po3_SKSEFunctions::GetEffectShaderPersistentParticleCount(RE::TESEffectShader* thisEffectShader)
{
	return (thisEffectShader) ? (thisEffectShader->data.particleShaderPersistantParticleCount) : 0.0;
}

//set effect shader persistant particle count
void po3_SKSEFunctions::SetEffectShaderPersistentParticleCount(RE::TESEffectShader* thisEffectShader, float particleCount)
{
	if (thisEffectShader)
	{
		thisEffectShader->data.particleShaderPersistantParticleCount = particleCount;
	}

}

// replaces keyword on form - adapted from DienesTools
void po3_SKSEFunctions::ReplaceKeywordOnForm(RE::TESForm* thisForm, RE::BGSKeyword* KYWDtoRemove, RE::BGSKeyword* KYWDtoAdd)
{
	if (!thisForm || !KYWDtoRemove || !KYWDtoAdd)
	{
		return;
	}

	RE::BGSKeywordForm* pKeywords = skyrim_cast<RE::BGSKeywordForm*>(thisForm);

	if (pKeywords)
	{
		UInt32 removeIndex;
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

//keyword function added by papyrusgamedata
bool po3_SKSEFunctions::IsPluginFound(RE::BSFixedString name)
{
	RE::TESDataHandler* dataHandler = RE::TESDataHandler::GetSingleton();

	const RE::TESFile * modInfo = dataHandler->LookupModByName(name.c_str());

	if (modInfo)
	{
		return modInfo->IsLoaded();
	}

	return false;
}

//Returns every single spell added by a specific mod in game. Filtered by keyword array, and whether it is playable.
RE::BSScript::VMArray<RE::TESForm*> po3_SKSEFunctions::GetAllSpellsInMod(RE::BSFixedString modName, RE::BSScript::VMArray<RE::BGSKeyword*> keywords, bool isPlayable)
{
	RE::BSScript::VMArray<RE::TESForm*> result;

	RE::TESDataHandler * dataHandler = RE::TESDataHandler::GetSingleton();
	const RE::TESFile * modInfo = dataHandler->LookupModByName(modName.c_str());

	if (!modInfo || !modInfo->IsLoaded())
	{
		return result;
	}

	std::vector<RE::TESForm*> vec;
	RE::SpellItem * spell = nullptr;

	if (isPlayable)
	{
		for (auto& book : dataHandler->GetFormArray<RE::TESObjectBOOK>())
		{
			if (!book)
			{
				continue;
			}

			if (!modInfo->IsFormInMod(book->formID))
			{
				continue;
			}

			bool isSpellBook = book->data.GetSanitizedType() == RE::TESObjectBOOK::Data::Flag::kTeachesSpell;
			bool accept = false;

			if (isSpellBook)
			{
				spell = book->data.teaches.spell;
			}

			if (VerifyKeywords(spell, &keywords))
			{
				accept = true;
			}

			if (!accept)
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
			if (!spell)
			{
				continue;
			}

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

	auto size = vec.size();

	if (size != 0)
	{
		result.resize(size);

		for (size_t i = 0; i < size; i++)
		{
			result[i] = vec[i];
		}
	}

	return result;
}

//Returns every single race added by a specific mod in game. Filtered by keyword array.
RE::BSScript::VMArray<RE::TESForm*> po3_SKSEFunctions::GetAllRacesInMod(RE::BSFixedString modName, RE::BSScript::VMArray<RE::BGSKeyword*> keywords)
{
	RE::BSScript::VMArray<RE::TESForm*> result;

	RE::TESDataHandler * dataHandler = RE::TESDataHandler::GetSingleton();
	const RE::TESFile * modInfo = dataHandler->LookupModByName(modName.c_str());

	if (!modInfo || !modInfo->IsLoaded())
	{
		return result;
	}

	std::vector<RE::TESForm*> vec;

	for (auto& race : dataHandler->GetFormArray<RE::TESRace>())
	{
		if (!race)
		{
			continue;
		}

		if (!modInfo->IsFormInMod(race->formID))
		{
			continue;
		}

		bool accept = false;

		if (VerifyKeywords(race, &keywords))
		{
			accept = true;
		}

		if (!accept)
		{
			continue;
		}

		vec.push_back(race);
	}

	auto size = vec.size();

	if (size != 0)
	{
		result.resize(size);

		for (size_t i = 0; i < size; i++)
		{
			result[i] = vec[i];
		}
	}

	return result;
}

//Adds every single spell in game to a formlist. Filtered by keyword array, and whether it is playable.
void po3_SKSEFunctions::AddAllSpellsInGameToList(RE::BGSListForm * thisList, RE::BSScript::VMArray<RE::BGSKeyword*> keywords, bool isPlayable)
{
	if (!thisList)
	{
		return;
	}

	RE::TESDataHandler * dataHandler = RE::TESDataHandler::GetSingleton();

	if (isPlayable)
	{
		RE::SpellItem * spell = nullptr;

		for (auto& book : dataHandler->GetFormArray<RE::TESObjectBOOK>())
		{
			if (!book)
			{
				continue;
			}

			bool isSpellBook = book->data.GetSanitizedType() == RE::TESObjectBOOK::Data::Flag::kTeachesSpell;
			bool accept = false;

			if (isSpellBook)
			{
				spell = book->data.teaches.spell;
			}

			if (VerifyKeywords(spell, &keywords))
			{
				accept = true;
			}

			if (!accept)
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
			if (!spell)
			{
				continue;
			}

			if (!VerifyKeywords(spell, &keywords))
			{
				continue;
			}

			thisList->AddFormToList(spell);
		}
	}
}

//Adds every single race in the game to list. Filtered by keyword array.
void po3_SKSEFunctions::AddAllRacesInGameToList(RE::BGSListForm * thisList, RE::BSScript::VMArray<RE::BGSKeyword*> keywords)
{
	if (!thisList)
	{
		return;
	}

	RE::TESDataHandler * dataHandler = RE::TESDataHandler::GetSingleton();

	for (auto& race : dataHandler->GetFormArray<RE::TESRace>())
	{
		if (!race)
		{
			continue;
		}

		if (VerifyKeywords(race, &keywords))
		{
			continue;
		}

		thisList->AddFormToList(race);
	}
}

//returns light radius
float po3_SKSEFunctions::GetLightRadius(RE::TESObjectLIGH * thisLight)
{
	return (thisLight) ? float(thisLight->data.radius) : 0.0;
}

//sets light radius
void po3_SKSEFunctions::SetLightRadius(RE::TESObjectLIGH * thisLight, float radius)
{
	if (thisLight)
	{
		thisLight->data.radius = (UInt32)radius;
	}
}

//returns light fade
float po3_SKSEFunctions::GetLightFade(RE::TESObjectLIGH * thisLight)
{
	return (thisLight) ? (thisLight->fadeValue) : 0.0;
}

//sets light fade
void po3_SKSEFunctions::SetLightFade(RE::TESObjectLIGH * thisLight, float fadeValue)
{
	if (thisLight)
	{
		thisLight->fadeValue = fadeValue;
	}
}

//sets light color
void po3_SKSEFunctions::SetLightColor(RE::TESObjectLIGH * thisLight, RE::BGSColorForm * colorform)
{
	if (thisLight && colorform)
	{
		thisLight->data.color.red = colorform->color.red;
		thisLight->data.color.green = colorform->color.green;
		thisLight->data.color.blue = colorform->color.blue;
	}
}

//get light type
UInt32 po3_SKSEFunctions::GetLightTypeInternal(RE::TESObjectLIGH * thisLight)
{
	if (thisLight)
	{
		auto flags = thisLight->data.flags;

		if (flags == RE::TESObjectLIGH::Data::Flag::kShadowHemisphere)
		{
			return 1;
		}
		else if (flags == RE::TESObjectLIGH::Data::Flag::kNone)
		{
			return 2;
		}
		else if (flags == RE::TESObjectLIGH::Data::Flag::kShadowOmnidirectional)
		{
			return 3;
		}
		else if (flags == RE::TESObjectLIGH::Data::Flag::kSpotlight)
		{
			return 4;
		}
		else if (flags == RE::TESObjectLIGH::Data::Flag::kShadowSpotlight)
		{
			return 5;
		}
	}

	return 0;
}

//get light type
UInt32 po3_SKSEFunctions::GetLightType(RE::TESObjectLIGH * thisLight)
{
	return GetLightTypeInternal(thisLight);
}

//get light type 
void po3_SKSEFunctions::SetLightType(RE::TESObjectLIGH * thisLight, UInt32 lightType)
{
	if (!thisLight)
	{
		return;
	}

	auto flags = thisLight->data.flags;

	switch (lightType)
	{
	case 1:
		flags = flags & ~RE::TESObjectLIGH::Data::Flag::kType | RE::TESObjectLIGH::Data::Flag::kShadowHemisphere);
		break;
	case 2:
		flags = flags & ~RE::TESObjectLIGH::Data::Flag::kType | RE::TESObjectLIGH::Data::Flag::kNone);
		break;
	case 3:
		flags = flags & ~RE::TESObjectLIGH::Data::Flag::kType | RE::TESObjectLIGH::Data::Flag::kShadowOmnidirectional);
		break;
	case 4:
		flags = flags & ~RE::TESObjectLIGH::Data::Flag::kType | RE::TESObjectLIGH::Data::Flag::kSpotlight);
		break;
	case 5:
		flags = flags & ~RE::TESObjectLIGH::Data::Flag::kType | RE::TESObjectLIGH::Data::Flag::kShadowSpotlight);
		break;
	}

	thisLight->data.flags = flags;
}

//returns light FOV
float po3_SKSEFunctions::GetLightFOV(RE::TESObjectLIGH * thisLight)
{
	return (thisLight) ? (thisLight->data.fov) : 0.0;
}

//sets light FOV
void po3_SKSEFunctions::SetLightFOV(RE::TESObjectLIGH * thisLight, float FOV)
{
	if (thisLight)
	{
		thisLight->data.fov = FOV;
	}
}

//get shadow casting light depth bias
float po3_SKSEFunctions::GetLightShadowDepthBias(RE::TESObjectREFR * thisLightObject)
{
	if (!thisLightObject)
	{
		return 1.0;
	}

	auto thisLight = skyrim_cast<RE::TESObjectLIGH*>(thisLightObject->baseForm);

	if (thisLight)
	{
		auto xLightData = static_cast<RE::ExtraLightData*>(thisLightObject->extraData.GetByType(ExtraDataType::kExtraData_LightData));

		if (xLightData)
		{
			return xLightData->depthBias;
		}
	}

	return 1.0;
}

//set shadow casting light depth bias. creaRE::TES extralightdata if none exists
void po3_SKSEFunctions::SetLightShadowDepthBias(RE::TESObjectREFR * thisLightObject, float depthBias)
{
	if (!thisLightObject)
		return;

	auto thisLight = skyrim_cast<RE::TESObjectLIGH*>(thisLightObject->baseForm);

	if (thisLight)
	{
		auto xLightData = static_cast<RE::ExtraLightData*>(thisLightObject->extraData.GetByType(ExtraDataType::kExtraData_LightData));

		if (xLightData)
		{
			xLightData->depthBias = depthBias;
		}
		else
		{
			auto newLightData = &RE::ExtraLightData::ExtraLightData();
			newLightData->depthBias = depthBias;
			(&thisLightObject->extraData)->Add(newLightData);
		}
	}
}

// [x, y, z]
RE::BSScript::VMArray<float> po3_SKSEFunctions::GetPositionAsArray(RE::TESObjectREFR * ref)
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
RE::BSScript::VMArray<float> po3_SKSEFunctions::GetRotationAsArray(RE::TESObjectREFR * ref)
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

// generaRE::TES a random float based on mersenne twister
float po3_SKSEFunctions::GenerateRandomFloat(float afMin, float afMax)
{
	std::random_device rd{};

	std::mt19937 engine{ rd() };

	std::uniform_real_distribution<float> dist(afMin, afMax);

	return dist(engine);
}

// generaRE::TES a random int based on mersenne twister
UInt32 po3_SKSEFunctions::GenerateRandomInt(UInt32 afMin, UInt32 afMax)
{
	std::random_device rd{};

	std::mt19937 engine{ rd() };

	std::uniform_int_distribution<UInt32>dist(afMin, afMax);

	return dist(engine);
}

//retrives all activemagiceffects on player and returns an array of magiceffects
RE::BSScript::VMArray<RE::EffectSetting*> po3_SKSEFunctions::GetAllActiveEffectsOnActor(RE::Actor* thisActor, bool showInactive)
{
	RE::BSScript::VMArray<RE::EffectSetting*> result;
	std::vector<RE::EffectSetting*> vec;

	if (thisActor)
	{
		auto effects = thisActor->GetActiveEffects();

		if (!effects)
		{
			RE::EffectSetting* mgef = 0;

			for (auto& effect : *effects)
			{
				mgef = effect ? effect->GetBaseObject() : 0;

				if (mgef)
				{
					if (!showInactive && (effect->flags & RE::ActiveEffect::Flag::kInactive || mgef->data.flags & RE::EffectSetting::Data::Flag::kHideInUI))
					{
						continue;
					}

					vec.push_back(mgef);
				}
			}
		}
	}

	ReturnVMArray(vec, result);
}

// Returns magic archetype - for internal use
RE::BSFixedString po3_SKSEFunctions::GetEffectArchetypeInternal(RE::EffectSetting* mgef)
{
	RE::BSFixedString archetype = nullptr;

	if (!mgef)
	{
		return archetype;
	}

	switch (mgef->properties.archetype)
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
		archetype = "Demoralise";
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
	}

	return archetype;
}

//checks if magiceffect with archetype was found on RE::Actor
bool po3_SKSEFunctions::HasMagicEffectWithArchetype(RE::Actor* thisActor, RE::BSFixedString archetype)
{
	if (thisActor)
	{
		auto effects = thisActor->GetActiveEffects();

		if (!effects)
		{
			return false;
		}

		RE::EffectSetting* mgef = 0;
		for (auto& effect : *effects)
		{
			mgef = effect ? effect->GetBaseObject() : 0;

			if (mgef && GetEffectArchetypeInternal(mgef) == archetype)
			{
				return true;
			}
		}
	}

	return false;
}

//Returns the effect archetype of a magic effect as int, 0-46
UInt32 po3_SKSEFunctions::GetEffectArchetypeAsInt(RE::EffectSetting* mgef)
{
	return (mgef) ? (UInt32)(mgef->data.archetype) : 0;
}

// Returns magic archetype as string
RE::BSFixedString po3_SKSEFunctions::GetEffectArchetypeAsString(RE::EffectSetting* mgef)
{
	return GetEffectArchetypeInternal(mgef);
}

// Gets sound attached to magiceffect
RE::BGSSoundDescriptorForm * po3_SKSEFunctions::GetMagicEffectSound(RE::EffectSetting* mgef, UInt32 type)
{
	if (mgef)
	{
		for (auto& effectSound : mgef->sounds)
		{
			if ((UInt32)effectSound.type == type)
			{
				return effectSound.sound;
			}
		}
	}

	return nullptr;
}

// Sets sound attached to magiceffect
void po3_SKSEFunctions::SetMagicEffectSound(RE::EffectSetting* mgef, RE::BGSSoundDescriptorForm * mgefSound, UInt32 type)
{
	if (mgef && mgefSound)
	{
		for (auto& effectSound : mgef->sounds)
		{
			if ((UInt32)effectSound.type == type)
			{
				effectSound.sound = mgefSound;
				break;
			}
		}
	}

	return;
}

//is door a load door?
bool po3_SKSEFunctions::IsLoadDoor(RE::TESObjectREFR* thisDoor)
{
	if (thisDoor)
	{
		//Get the ExtraTeleport
		auto teleport = skyrim_cast<RE::ExtraTeleport*>(thisDoor->extraData.GetByType(ExtraDataType::kExtraData_Teleport));

		if (teleport)
		{
			return true;
		}
	}

	return false;
}

//checker
bool po3_SKSEFunctions::CanItemBeTaken(RE::InventoryEntryData *data, bool noEquipped, bool noFavourited, bool noQuestItem)
{
	if (data->extraList)
	{
		for (RE::BaseExtraList *extraList : *data->extraList)
		{
			if (noEquipped)
			{
				auto worn = static_cast<RE::ExtraWorn*>(extraList->GetByType(ExtraDataType::kExtraData_Worn));
				auto wornLeft = static_cast<RE::ExtraWornLeft*>(extraList->GetByType(ExtraDataType::kExtraData_WornLeft));

				if (worn || wornLeft)
				{
					return false;
				}
			}

			if (noFavourited)
			{
				auto HotKey = static_cast<RE::ExtraHotkey*>(extraList->GetByType(ExtraDataType::kExtraData_Hotkey));

				if (HotKey)
				{
					return false;
				}
			}

			if (noQuestItem)
			{
				auto aliases = static_cast<RE::ExtraAliasInstanceArray*>(extraList->GetByType(ExtraDataType::kExtraData_AliasInstanceArray));

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

//add all inventory items to formlist
void po3_SKSEFunctions::AddAllInventoryItemsToList(RE::Actor * thisActor, RE::BGSListForm * thisList, bool noEquipped, bool noFavourited, bool noQuestItem)
{
	if (thisActor && thisList)
	{
		RE::ExtraContainerChanges* exChanges = static_cast<RE::ExtraContainerChanges*>(thisActor->extraData.GetByType(ExtraDataType::kExtraData_ContainerChanges)); //loop through caster inventory
		RE::InventoryChanges *changes = exChanges ? exChanges->changes : nullptr;

		if (changes && changes->entryList)
		{
			for (RE::InventoryEntryData *data : *changes->entryList)
			{
				RE::TESForm *item = data->GetOwner();

				if (CanItemBeTaken(data, noEquipped, noFavourited, noQuestItem))
				{
					thisList->AddFormToList(item);
				}
			}
		}
	}
}

//add all inventory items to array
RE::BSScript::VMArray<RE::TESForm*> po3_SKSEFunctions::AddAllInventoryItemsToArray(RE::Actor * thisActor, bool noEquipped, bool noFavourited, bool noQuestItem)
{
	RE::BSScript::VMArray<RE::TESForm*> result;
	std::vector<RE::TESForm*> vec;

	if (thisActor)
	{
		RE::ExtraContainerChanges* exChanges = static_cast<RE::ExtraContainerChanges*>(thisActor->extraData.GetByType(ExtraDataType::kExtraData_ContainerChanges)); //loop through caster inventory
		RE::InventoryChanges *changes = exChanges ? exChanges->changes : nullptr;

		if (changes && changes->entryList)
		{
			for (RE::InventoryEntryData *data : *changes->entryList)
			{
				RE::TESForm *item = data->GetOwner();

				if (CanItemBeTaken(data, noEquipped, noFavourited, noQuestItem))
				{
					vec.push_back(item);
				}
			}
		}
	}

	ReturnVMArray(vec, result);
}

//add all equipped items to formlist
RE::BSScript::VMArray<RE::TESForm*> po3_SKSEFunctions::AddAllEquippedItemsToArray(RE::Actor * thisActor)
{
	RE::BSScript::VMArray<RE::TESForm*> result;
	std::vector<RE::TESForm*> vec;

	if (thisActor)
	{
		RE::ExtraContainerChanges* exChanges = static_cast<RE::ExtraContainerChanges*>(thisActor->extraData.GetByType(ExtraDataType::kExtraData_ContainerChanges)); //loop through caster inventory
		RE::InventoryChanges *changes = exChanges ? exChanges->changes : nullptr;

		if (changes && changes->entryList)
		{
			for (RE::InventoryEntryData *data : *changes->entryList)
			{
				if (data->extraList)
				{
					for (RE::BaseExtraList *extraList : *data->extraList)
					{
						auto worn = static_cast<RE::ExtraWorn*>(extraList->GetByType(ExtraDataType::kExtraData_Worn));
						auto wornLeft = static_cast<RE::ExtraWornLeft*>(extraList->GetByType(ExtraDataType::kExtraData_WornLeft));

						if (worn || wornLeft)
						{
							vec.push_back(data->GetOwner());
						}
					}
				}
			}
		}
	}

	ReturnVMArray(vec, result);
}

// replaces keyword on ref - adapted from DienesTools
void po3_SKSEFunctions::ReplaceKeywordOnRef(RE::TESObjectREFR* thisRef, RE::BGSKeyword* KYWDtoRemove, RE::BGSKeyword* KYWDtoAdd)
{
	if (!thisRef || !KYWDtoRemove || !KYWDtoAdd)
	{
		return;
	}

	RE::BGSKeywordForm* pKeywords = skyrim_cast<RE::BGSKeywordForm*>(thisRef);

	if (pKeywords)
	{
		UInt32 removeIndex;
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

//calculaRE::TES a 2D vector. Calls ApplyHavokImpulse internally
void po3_SKSEFunctions::Apply2DHavokImpulse(RE::TESObjectREFR * source, RE::TESObjectREFR * target, float afZ, float magnitude)
{
	if (!source || !target || source == target)
	{
		return;
	}

	if (!source->Is3DLoaded() || !target->Is3DLoaded())
	{
		return;
	}

	float sourceZ = (source->rot.z * (180.0 / MATH_PI));

	auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
	float angleZ = (sourceZ + GetHeadingAngle(vm, 0, source, target)); //source.getanglez() + source.getheadingangle(target)

	g_task->AddTask(new po3_TaskApplyHavokImpulse(target, asinf(angleZ), acosf(angleZ), afZ, magnitude));
}

//calculaRE::TES a 3D vector and takes into account the elevation between source and target. Calls ApplyHavokImpulse internally
void po3_SKSEFunctions::Apply3DHavokImpulse(RE::TESObjectREFR * source, RE::TESObjectREFR * target, float magnitude)
{
	if (!source || !target || source == target)
	{
		return;
	}

	if (!source->Is3DLoaded() || !target->Is3DLoaded())
	{
		return;
	}

	float dx = target->pos.x - source->pos.x;
	float dy = target->pos.y - source->pos.y;
	float dz = target->pos.z - source->pos.z;

	float dist = std::sqrtf((dx * dx) + (dy * dy) + (dz * dz));

	if (dist > 0.0)
	{
		float x = dx / dist; //x
		float y = dy / dist; //y
		float z = dz / dist; //z

		g_task->AddTask(new po3_TaskApplyHavokImpulse(target, x, y, z, magnitude));
	}
}

//returns projectile speed 
float po3_SKSEFunctions::GetProjectileSpeed(RE::BGSProjectile * thisProjectile)
{
	return (thisProjectile) ? (thisProjectile->data.speed) : 0.0;
}

//sets projectile speed
void po3_SKSEFunctions::SetProjectileSpeed(RE::BGSProjectile * thisProjectile, float speed)
{
	if (thisProjectile)
	{
		thisProjectile->data.speed = speed;
	}
}

//returns projectile range
float po3_SKSEFunctions::GetProjectileRange(RE::BGSProjectile * thisProjectile)
{
	return (thisProjectile) ? (thisProjectile->data.range) : 0.0;
}

//sets projectile range
void po3_SKSEFunctions::SetProjectileRange(RE::BGSProjectile * thisProjectile, float range)
{
	if (thisProjectile)
	{
		thisProjectile->data.range = range;
	}
}

//returns projectile gravity
float po3_SKSEFunctions::GetProjectileGravity(RE::BGSProjectile * thisProjectile)
{
	return (thisProjectile) ? (thisProjectile->data.gravity) : 0.0;
}

//sets projectile gravity
void po3_SKSEFunctions::SetProjectileGravity(RE::BGSProjectile * thisProjectile, float gravity)
{
	if (thisProjectile)
	{
		thisProjectile->data.gravity = gravity;
	}
}

//returns projectile impactforce
float po3_SKSEFunctions::GetProjectileImpactForce(RE::BGSProjectile * thisProjectile)
{
	return (thisProjectile) ? (thisProjectile->data.impactForce) : 0.0;
}

//sets projectile impactforce
void po3_SKSEFunctions::SetProjectileImpactForce(RE::BGSProjectile * thisProjectile, float impactForce)
{
	if (thisProjectile)
	{
		thisProjectile->data.impactForce = impactForce;
	}
}

//gets projectile type
UInt32 po3_SKSEFunctions::GetProjectileType(RE::BGSProjectile * thisProjectile)
{
	if (thisProjectile)
	{
		if (((thisProjectile->data.unk00 >> 16) & 0x7F) == 0x01) //Missile
		{
			return 1;
		}
		else if (((thisProjectile->data.unk00 >> 16) & 0x7F) == 0x02) //Lobber (runes)
		{
			return 2;
		}
		else if (((thisProjectile->data.unk00 >> 16) & 0x7F) == 0x04) //Beam
		{
			return 3;
		}
		else if (((thisProjectile->data.unk00 >> 16) & 0x7F) == 0x08) //Flame
		{
			return 4;
		}
		else if (((thisProjectile->data.unk00 >> 16) & 0x7F) == 0x10) //Cone
		{
			return 5;
		}
		else if (((thisProjectile->data.unk00 >> 16) & 0x7F) == 0x20) //Barrier
		{
			return 6;
		}
		else if (((thisProjectile->data.unk00 >> 16) & 0x7F) == 0x40) //Arrow
		{
			return 7;
		}
	}

	return 0;
}

//set sound descriptor
void po3_SKSEFunctions::SetSoundDescriptor(RE::TESSound* thisSound, RE::BGSSoundDescriptorForm * thisSoundDescriptor)
{
	if (thisSound && thisSoundDescriptor)
	{
		thisSound->descriptor = thisSoundDescriptor;
	}
}

//gets spell type
UInt32 po3_SKSEFunctions::GetSpellType(RE::SpellItem* thisSpell)
{
	return (thisSpell) ? (UInt32)thisSpell->data.type : 0;
}

//Get soul level
RE::SoulLevel po3_SKSEFunctions::GetSoulLevel(RE::TESSoulGem* a_soulGem, RE::InventoryEntryData* entry)
{
	if (a_soulGem->containedSoul != RE::SoulLevel::kNone)
	{
		return static_cast<RE::SoulLevel>(a_soulGem->containedSoul);
	}
	else
	{
		if (entry->extraList)
		{
			for (RE::BaseExtraList *list : *entry->extraList)
			{
				if (list)
				{
					auto xSoul = static_cast<RE::ExtraSoul*>(list->GetByType(ExtraDataType::kExtraData_Soul));

					if (xSoul)
					{
						return static_cast<RE::SoulLevel>(xSoul->level);
					}
				}
			}
		}
	}

	return RE::SoulLevel::kNone;
}

//returns wind speed from 0-255 (how it's set up in the weather form)
UInt32 po3_SKSEFunctions::GetWindSpeedAsInt(RE::TESWeather * thisWeather)
{
	return (thisWeather) ? (thisWeather->data.windSpeed) : 0;
}

//returns wind speed from 0.0-1.0 (how it's set up in the CK)
float po3_SKSEFunctions::GetWindSpeedAsFloat(RE::TESWeather * thisWeather)
{
	return (thisWeather) ? ((thisWeather->data.windSpeed) / 255.0) : 0.0;
}

// Tell the game about the new functions.
bool po3_SKSEFunctions::Register(RE::BSScript::Internal::VirtualMachine* a_vm)
{
	if (!a_vm)
	{
		_MESSAGE("couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("SetHairColor", "po3_SKSEFunctions", SetHairColor);
	a_vm->RegisterFunction("SetSkinColor", "po3_SKSEFunctions", SetSkinColor);
	a_vm->RegisterFunction("SetSkinAlpha", "po3_SKSEFunctions", MixColorWithSkinTone);
	a_vm->RegisterFunction("SetSkinAlpha", "po3_SKSEFunctions", SetSkinAlpha);
	a_vm->RegisterFunction("ReplaceTextureSetOnArmor", "po3_SKSEFunctions", ReplaceTextureSetOnArmor);
	a_vm->RegisterFunction("ReplaceSkinTextureSet", "po3_SKSEFunctions", ReplaceSkinTextureSet);
	a_vm->RegisterFunction("ReplaceFaceTextureSet", "po3_SKSEFunctions", ReplaceFaceTextureSet);
	a_vm->RegisterFunction("GetHeadPartTextureSet", "po3_SKSEFunctions", GetHeadPartTextureSet);
	a_vm->RegisterFunction("SetHeadPartTextureSet", "po3_SKSEFunctions", SetHeadPartTextureSet);
	a_vm->RegisterFunction("SetHeadPartAlpha", "po3_SKSEFunctions", SetHeadPartAlpha);
	a_vm->RegisterFunction("ToggleSkinnedDecals", "po3_SKSEFunctions", ToggleSkinnedDecals);
	a_vm->RegisterFunction("RemoveChildNode", "po3_SKSEFunctions", RemoveChildNode);
	a_vm->RegisterFunction("IsActorSoulTrapped", "po3_SKSEFunctions", IsActorSoulTrapped);
	a_vm->RegisterFunction("ResetActor3D", "po3_SKSEFunctions", ResetActor3D);
	a_vm->RegisterFunction("DecapitateActor", "po3_SKSEFunctions", DecapitateActor);

	a_vm->RegisterFunction("AddStringToArray", "po3_SKSEFunctions", AddStringToArray);
	a_vm->RegisterFunction("AddActorToArray", "po3_SKSEFunctions", AddActorToArray);
	a_vm->RegisterFunction("ArrayStringCount", "po3_SKSEFunctions", ArrayStringCount);
	a_vm->RegisterFunction("SortArrayString", "po3_SKSEFunctions", SortArrayString);

	a_vm->RegisterFunction("GetEffectShaderFullParticleCount", "po3_SKSEFunctions", GetEffectShaderFullParticleCount);
	a_vm->RegisterFunction("SetEffectShaderFullParticleCount", "po3_SKSEFunctions", SetEffectShaderFullParticleCount);
	a_vm->RegisterFunction("GetEffectShaderPersistentParticleCount", "po3_SKSEFunctions", GetEffectShaderPersistentParticleCount);
	a_vm->RegisterFunction("SetEffectShaderPersistentParticleCount", "po3_SKSEFunctions", SetEffectShaderPersistentParticleCount);

	a_vm->RegisterFunction("ReplaceKeywordOnForm", "po3_SKSEFunctions", ReplaceKeywordOnForm);

	a_vm->RegisterFunction("IsPluginFound", "po3_SKSEFunctions", IsPluginFound);
	a_vm->RegisterFunction("GetAllSpellsInMod", "po3_SKSEFunctions", GetAllSpellsInMod);
	a_vm->RegisterFunction("GetAllRacesInMod", "po3_SKSEFunctions", GetAllRacesInMod);
	a_vm->RegisterFunction("AddAllSpellsInGameToList", "po3_SKSEFunctions", AddAllSpellsInGameToList);
	a_vm->RegisterFunction("AddAllRacesInGameToList", "po3_SKSEFunctions", AddAllRacesInGameToList);

	a_vm->RegisterFunction("GetLightRadius", "po3_SKSEFunctions", GetLightRadius);
	a_vm->RegisterFunction("SetLightRadius", "po3_SKSEFunctions", SetLightRadius);
	a_vm->RegisterFunction("GetLightFade", "po3_SKSEFunctions", GetLightFade);
	a_vm->RegisterFunction("SetLightFade", "po3_SKSEFunctions", SetLightFade);
	a_vm->RegisterFunction("SetLightColor", "po3_SKSEFunctions", SetLightColor);
	a_vm->RegisterFunction("GetLightType)", "po3_SKSEFunctions", GetLightType);
	a_vm->RegisterFunction("SetLightType", "po3_SKSEFunctions", SetLightType);
	a_vm->RegisterFunction("GetLightFOV", "po3_SKSEFunctions", GetLightFOV);
	a_vm->RegisterFunction("SetLightFOV", "po3_SKSEFunctions", SetLightFOV);
	a_vm->RegisterFunction("GetLightShadowDepthBias", "po3_SKSEFunctions", GetLightShadowDepthBias);
	a_vm->RegisterFunction("SetLightShadowDepthBias", "po3_SKSEFunctions", SetLightShadowDepthBias);

	a_vm->RegisterFunction("GetPositionAsArray", "po3_SKSEFunctions", GetPositionAsArray);
	a_vm->RegisterFunction("GetRotationAsArray", "po3_SKSEFunctions", GetRotationAsArray);
	a_vm->RegisterFunction("GenerateRandomFloat", "po3_SKSEFunctions", GenerateRandomFloat);
	a_vm->RegisterFunction("GenerateRandomInt", "po3_SKSEFunctions", GenerateRandomInt);

	a_vm->RegisterFunction("GetAllActiveEffectsOnActor", "po3_SKSEFunctions", GetAllActiveEffectsOnActor);
	a_vm->RegisterFunction("HasMagicEffectWithArchetype", "po3_SKSEFunctions", HasMagicEffectWithArchetype);
	a_vm->RegisterFunction("GetEffectArchetypeAsInt", "po3_SKSEFunctions", GetEffectArchetypeAsInt);
	a_vm->RegisterFunction("GetEffectArchetypeAsString", "po3_SKSEFunctions", GetEffectArchetypeAsString);
	a_vm->RegisterFunction("GetMagicEffectSound", "po3_SKSEFunctions", GetMagicEffectSound);
	a_vm->RegisterFunction("SetMagicEffectSound", "po3_SKSEFunctions", SetMagicEffectSound);

	a_vm->RegisterFunction("IsLoadDoor", "po3_SKSEFunctions", IsLoadDoor);
	a_vm->RegisterFunction("AddAllInventoryItemsToList", "po3_SKSEFunctions", AddAllInventoryItemsToList);
	a_vm->RegisterFunction("AddAllInventoryItemsToArray", "po3_SKSEFunctions", AddAllInventoryItemsToArray);
	a_vm->RegisterFunction("AddAllEquippedItemsToArray", "po3_SKSEFunctions", AddAllEquippedItemsToArray);
	a_vm->RegisterFunction("ReplaceKeywordOnRef", "po3_SKSEFunctions", ReplaceKeywordOnRef);
	a_vm->RegisterFunction("Apply2DHavokImpulse", "po3_SKSEFunctions", Apply2DHavokImpulse);
	a_vm->RegisterFunction("Apply3DHavokImpulse", "po3_SKSEFunctions", Apply3DHavokImpulse);

	a_vm->RegisterFunction("GetProjectileSpeed", "po3_SKSEFunctions", GetProjectileSpeed);
	a_vm->RegisterFunction("SetProjectileSpeed", "po3_SKSEFunctions", SetProjectileSpeed);
	a_vm->RegisterFunction("GetProjectileRange", "po3_SKSEFunctions", GetProjectileRange);
	a_vm->RegisterFunction("SetProjectileRange", "po3_SKSEFunctions", SetProjectileRange);
	a_vm->RegisterFunction("GetProjectileGravity", "po3_SKSEFunctions", GetProjectileGravity);
	a_vm->RegisterFunction("SetProjectileGravity", "po3_SKSEFunctions", SetProjectileGravity);
	a_vm->RegisterFunction("GetProjectileImpactForce", "po3_SKSEFunctions", GetProjectileImpactForce);
	a_vm->RegisterFunction("SetProjectileImpactForce", "po3_SKSEFunctions", SetProjectileImpactForce);
	a_vm->RegisterFunction("GetProjectileType", "po3_SKSEFunctions", GetProjectileType);

	a_vm->RegisterFunction("SetSoundDescriptor", "po3_SKSEFunctions", SetSoundDescriptor);

	a_vm->RegisterFunction("GetSpellType", "po3_SKSEFunctions", GetSpellType);

	a_vm->RegisterFunction("GetWindSpeedAsInt", "po3_SKSEFunctions", GetWindSpeedAsInt);
	a_vm->RegisterFunction("GetWindSpeedAsFloat", "po3_SKSEFunctions", GetWindSpeedAsFloat);

	_MESSAGE("Registered papyrus functions");

	return true;
}

