#include "Papyrus/PapyrusObjectReference.h"

#include "Serialization/Serialize.h"


std::vector<RE::TESForm*> papyrusObjectReference::AddAllInventoryItemsToArray(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem)
{
	std::vector<RE::TESForm*> vec;
	if (!a_ref) {
		a_vm->TraceStack("Cannot get inventory from a None object", a_stackID, Severity::kWarning);
		return vec;
	}
	auto changes = a_ref->GetInventoryChanges();
	if (changes && changes->entryList) {
		for (auto& entry : *changes->entryList) {
			if (entry && entry->CanItemBeTaken(a_noEquipped, a_noFavourited, a_noQuestItem)) {
				auto object = entry->object;
				if (object) {
					vec.push_back(object);
				}
			}
		}
	}
	return vec;
}


void papyrusObjectReference::AddAllInventoryItemsToList(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSListForm* a_list, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem)
{
	if (!a_ref) {
		a_vm->TraceStack("Cannot get inventory from a None object", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_list) {
		a_vm->TraceStack("Cannot add inventory to a None formlist", a_stackID, Severity::kWarning);
		return;
	}
	auto changes = a_ref->GetInventoryChanges();
	if (changes && changes->entryList) {
		for (auto& entry : *changes->entryList) {
			if (entry && entry->CanItemBeTaken(a_noEquipped, a_noFavourited, a_noQuestItem)) {
				auto object = entry->object;
				if (object) {
					a_list->AddForm(object);
				}
			}
		}
	}
}


void papyrusObjectReference::AddKeywordToRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSKeyword* a_add)
{
	if (!a_ref) {
		a_vm->TraceStack("Cannot add keyword to a None object", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_add) {
		a_vm->TraceStack("Cannot add a None keyword", a_stackID, Severity::kWarning);
		return;
	}
	auto base = a_ref->GetBaseObject();
	if (base) {
		auto keywords = Serialize::Keywords::GetSingleton();
		if (keywords) {
			keywords->PapyrusApplyKeywords(base, a_add, Serialize::Base::kAdd);
		}
	}
}


void ForEachRefWithinDistance(const RE::TESObjectCELL* a_cell, const RE::NiPoint3& a_origin, float a_radius, std::function<bool(RE::TESObjectREFR* a_cellRef)> a_fn)
{
	a_cell->spinLock.Lock();
	for (auto& refPtr : a_cell->references) {
		auto ref = refPtr.get();
		if (ref) {
			auto distance = RE::NiPoint3::GetSquaredDistance(a_origin, ref->GetPosition());
			if (distance <= a_radius) {
				if (!a_fn(ref)) {
					break;
				}
			}
		}
	}
	a_cell->spinLock.Unlock();
}


void IterateOverAttachedCells(RE::TES* TES, const RE::NiPoint3& a_origin, float a_radius, std::function<bool(RE::TESObjectREFR* a_ref)> a_fn)
{
	auto cell = TES->currentInteriorCell;
	if (cell) {
		ForEachRefWithinDistance(cell, a_origin, a_radius, [&](RE::TESObjectREFR* ref)
		{
			if (!a_fn(ref)) {
				return false;
			}
			return true;
		});
	}
	else {
		auto gridCellArray = TES->gridCellArray;
		if (gridCellArray) {
			auto gridLength = gridCellArray->length;
			if (gridLength > 0) {
				UInt32 x = 0;
				UInt32 y = 0;
				float yPlus = a_origin.y + a_radius;
				float yMinus = a_origin.y - a_radius;
				float xPlus = a_origin.x + a_radius;
				float xMinus = a_origin.x - a_radius;

				for (x = 0, y = 0; (x < gridLength && y < gridLength); x++, y++) {
					cell = gridCellArray->GetCell(x, y);
					if (cell && cell->IsAttached()) {
						auto cellCoords = cell->GetCoordinates();
						if (cellCoords) {
							if (cellCoords->worldX < xPlus && (cellCoords->worldX + 4096.0) > xMinus && cellCoords->worldY < yPlus && (cellCoords->worldY + 4096.0) > yMinus) {
								ForEachRefWithinDistance(cell, a_origin, a_radius, [&](RE::TESObjectREFR* a_cellRef)
								{
									if (!a_fn(a_cellRef)) {
										return false;
									}
									return true;
								});
							}
						}
					}
				}
			}
		}
	}
}


std::vector<RE::TESObjectREFR*> papyrusObjectReference::FindAllReferencesOfFormType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_origin, UInt32 a_formType, float a_radius)
{
	std::vector<RE::TESObjectREFR*> vec;

	if (!a_origin) {
		a_vm->TraceStack("Cannot get references from a None object", a_stackID, Severity::kWarning);
		return vec;
	}

	auto TES = RE::TES::GetSingleton();
	if (TES) {
		auto formType = static_cast<RE::FormType>(a_formType);
		auto originPos = a_origin->GetPosition();
		auto squaredRadius = a_radius * a_radius;

		IterateOverAttachedCells(TES, originPos, squaredRadius, [&](RE::TESObjectREFR* a_ref)
		{
			if (a_ref->GetFormType() == formType) {
				vec.push_back(a_ref);
			}
			return true;
		});

		if (vec.empty()) {
			auto worldSpace = TES->worldSpace;
			if (worldSpace) {
				auto skyCell = worldSpace->GetOrCreateSkyCell();
				if (skyCell) {
					ForEachRefWithinDistance(skyCell, originPos, squaredRadius, [&](RE::TESObjectREFR* a_cellRef)
					{
						if (a_cellRef->GetFormType() == formType) {
							vec.push_back(a_cellRef);
						}
						return true;
					});
				}
			}
		}
	}

	return vec;
}


std::vector<RE::TESObjectREFR*> papyrusObjectReference::FindAllReferencesOfType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_origin, RE::TESForm* a_formOrList, float a_radius)
{
	std::vector<RE::TESObjectREFR*> vec;

	if (!a_origin) {
		a_vm->TraceStack("Cannot get references from a None object", a_stackID, Severity::kWarning);
		return vec;
	}
	if (!a_formOrList) {
		a_vm->TraceStack("Cannot get references of type None from ref", a_stackID, Severity::kWarning);
		return vec;
	}
	auto TES = RE::TES::GetSingleton();
	if (TES) {
		auto list = a_formOrList->As<RE::BGSListForm>();
		auto originPos = a_origin->GetPosition();
		auto squaredRadius = a_radius * a_radius;

		IterateOverAttachedCells(TES, originPos, squaredRadius, [&](RE::TESObjectREFR* a_ref)
		{
			auto base = a_ref->GetBaseObject();
			if (base) {
				if (list) {
					if (!list->forms.empty()) {
						for (auto& form : list->forms) {
							if (form && form == base) {
								vec.push_back(a_ref);
							}
						}
					}
					if (list->scriptAddedTempForms) {
						for (const auto& formID : *list->scriptAddedTempForms) {
							auto form = RE::TESForm::LookupByID(formID);
							if (form && form == base) {
								vec.push_back(a_ref);
							}
						}
					}
				}
				else if (a_formOrList == base) {
					vec.push_back(a_ref);
				}
			}
			return true;
		});

		if (vec.empty()) {
			auto worldSpace = TES->worldSpace;
			if (worldSpace) {
				auto skyCell = worldSpace->GetOrCreateSkyCell();
				if (skyCell) {
					ForEachRefWithinDistance(skyCell, originPos, squaredRadius, [&](RE::TESObjectREFR* a_cellRef)
					{
						auto base = a_cellRef->GetBaseObject();
						if (base) {
							if (list) {
								if (!list->forms.empty()) {
									for (auto& form : list->forms) {
										if (form && form == base) {
											vec.push_back(a_cellRef);
										}
									}
								}
								if (list->scriptAddedTempForms) {
									for (const auto& formID : *list->scriptAddedTempForms) {
										auto form = RE::TESForm::LookupByID(formID);
										if (form && form == base) {
											vec.push_back(a_cellRef);
										}
									}
								}
							}
							else if (a_formOrList == base) {
								vec.push_back(a_cellRef);
							}
						}
						return true;
					});
				}
			}
		}
	}

	return vec;
}


std::vector<RE::TESObjectREFR*> papyrusObjectReference::FindAllReferencesWithKeyword(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_origin, RE::TESForm* a_formOrList, float a_radius, bool a_matchAll)
{
	std::vector<RE::TESObjectREFR*> vec;

	if (!a_origin) {
		a_vm->TraceStack("Cannot get references from a None object", a_stackID, Severity::kWarning);
		return vec;
	}
	if (!a_formOrList) {
		a_vm->TraceStack("Cannot get references of type None from ref", a_stackID, Severity::kWarning);
		return vec;
	}

	auto TES = RE::TES::GetSingleton();
	if (TES) {
		auto keyword = a_formOrList->As<RE::BGSKeyword>();
		auto list = a_formOrList->As<RE::BGSListForm>();

		if (!keyword && !list) {
			a_vm->TraceStack("FormOrList parameter is neither a keyword nor formlist", a_stackID, Severity::kWarning);
			return vec;
		}
		auto originPos = a_origin->GetPosition();
		float squaredRadius = a_radius * a_radius;

		IterateOverAttachedCells(TES, originPos, squaredRadius, [&](RE::TESObjectREFR* a_ref)
		{
			bool success = false;
			if (list) {
				success = a_matchAll ? a_ref->HasAllKeywords(list) : a_ref->HasKeywords(list);
			}
			else if (keyword) {
				success = a_ref->HasKeyword(keyword);
			}
			if (success) {
				vec.push_back(a_ref);
			}
			return true;
		});

		if (vec.empty()) {
			auto worldSpace = TES->worldSpace;
			if (worldSpace) {
				auto cell = worldSpace->GetOrCreateSkyCell();
				if (cell) {
					ForEachRefWithinDistance(cell, originPos, squaredRadius, [&](RE::TESObjectREFR* a_cellRef)
					{
						bool success = false;
						if (list) {
							success = a_matchAll ? a_cellRef->HasAllKeywords(list) : a_cellRef->HasKeywords(list);
						}
						else if (keyword) {
							success = a_cellRef->HasKeyword(keyword);
						}
						if (success) {
							vec.push_back(a_cellRef);
						}
						return true;
					});
				}
			}
		}
	}

	return vec;
}


RE::Actor* papyrusObjectReference::GetActorCause(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
{
	if (!a_ref) {
		a_vm->TraceStack("Cannot get actor cause from a None object", a_stackID, Severity::kWarning);
		return nullptr;
	}
	auto actorCause = a_ref->GetActorCause();
	if (actorCause) {
		auto actorPtr = actorCause->actor.get();
		if (actorPtr.get()) {
			return actorPtr.get();
		}
	}
	return nullptr;
}


RE::Actor* papyrusObjectReference::GetClosestActorFromRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, bool a_ignorePlayer)
{
	if (!a_ref) {
		a_vm->TraceStack("Cannot get closest actor from a None object", a_stackID, Severity::kWarning);
		return nullptr;
	}
	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		if (a_ignorePlayer && processLists->numberHighActors == 0) {
			a_vm->TraceStack("No actors are loaded", a_stackID, Severity::kWarning);
			return nullptr;
		}

		const auto originPos = a_ref->GetPosition();
		auto shortestDistance = std::numeric_limits<float>::max();
		std::map<RE::Actor*, float> map;

		for (auto& actorHandle : processLists->highActorHandles) {
			auto actorPtr = actorHandle.get();
			auto actor = actorPtr.get();
			if (actor && actor != a_ref) {
				auto distance = RE::NiPoint3::GetDistance(originPos, actorPtr->GetPosition());
				map.emplace(actor, distance);
				if (distance < shortestDistance) {
					shortestDistance = distance;
				}
			}
		}
		if (!a_ignorePlayer) {
			auto player = RE::PlayerCharacter::GetSingleton();
			if (player && a_ref != player) {
				auto distance = RE::NiPoint3::GetDistance(originPos, player->GetPosition());
				map.emplace(player, distance);
				if (distance < shortestDistance) {
					shortestDistance = distance;
				}
			}
		}
		auto it = std::find_if(map.begin(), map.end(), [shortestDistance](const auto& mo)
		{
			return mo.second == shortestDistance;
		});
		if (it != map.end()) {
			return it->first;
		}
	}
	return nullptr;
}


std::vector<RE::BGSArtObject*> papyrusObjectReference::GetAllArtObjects(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
{
	std::vector<RE::BGSArtObject*> vec;

	if (!a_ref) {
		a_vm->TraceStack("Cannot get art objects from a None object", a_stackID, Severity::kWarning);
		return vec;
	}

	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		processLists->GetMagicEffects([&](RE::BSTempEffect* a_tempEffect)
		{
			auto modelEffect = a_tempEffect->As<RE::ModelReferenceEffect>();
			if (modelEffect) {
				auto handle = a_ref->CreateRefHandle();
				if (modelEffect->target == handle) {
					if (modelEffect->artObject) {
						vec.push_back(modelEffect->artObject);
					}
				}
			}
			return true;
		});
	}

	return vec;
}


std::vector<RE::TESEffectShader*> papyrusObjectReference::GetAllEffectShaders(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
{
	std::vector<RE::TESEffectShader*> vec;

	if (!a_ref) {
		a_vm->TraceStack("Cannot get effect shaders from a None object", a_stackID, Severity::kWarning);
		return vec;
	}

	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		processLists->GetMagicEffects([&](RE::BSTempEffect* a_tempEffect)
		{
			auto shaderEffect = a_tempEffect->As<RE::ShaderReferenceEffect>();
			if (shaderEffect) {
				auto handle = a_ref->CreateRefHandle();
				if (shaderEffect->target == handle) {
					if (shaderEffect->effectData) {
						vec.push_back(shaderEffect->effectData);
					}
				}
			}
			return true;
		});
	}

	return vec;
}


float papyrusObjectReference::GetEffectShaderDuration(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESEffectShader* a_effectShader)
{
	float time = 0.0f;

	if (!a_ref) {
		a_vm->TraceStack("Cannot get effectshader duration from a None object", a_stackID, Severity::kWarning);
		return time;
	}
	if (!a_effectShader) {
		a_vm->TraceStack("Cannot get effectshader duration from a None effectshader", a_stackID, Severity::kWarning);
		return time;
	}

	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		processLists->GetMagicEffects([&](RE::BSTempEffect* a_tempEffect)
		{
			auto shaderEffect = a_tempEffect->As<RE::ShaderReferenceEffect>();
			if (shaderEffect) {
				auto handle = a_ref->CreateRefHandle();
				if (shaderEffect->target == handle) {
					auto effectData = shaderEffect->effectData;
					if (effectData && effectData == a_effectShader) {
						time = shaderEffect->lifetime;
						return false;
					}
				}
			}
			return true;
		});
	}

	return time;
}


RE::Actor* papyrusObjectReference::GetRandomActorFromRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, float a_radius, bool a_ignorePlayer)
{
	if (!a_ref) {
		a_vm->TraceStack("Cannot get random actor from a None object", a_stackID, Severity::kWarning);
		return nullptr;
	}
	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		if (a_ignorePlayer && processLists->numberHighActors == 0) {
			a_vm->TraceStack("No actors are loaded", a_stackID, Severity::kWarning);
			return nullptr;
		}
		std::vector<RE::Actor*> vec;
		vec.reserve(processLists->numberHighActors);

		auto squaredRadius = a_radius * a_radius;
		auto originPos = a_ref->GetPosition();

		for (auto& actorHandle : processLists->highActorHandles) {
			auto actorPtr = actorHandle.get();
			auto actor = actorPtr.get();
			if (!actor || actor == a_ref) {
				continue;
			}
			auto distance = RE::NiPoint3::GetSquaredDistance(originPos, actor->GetPosition());
			if (distance > squaredRadius) {
				continue;
			}
			vec.emplace_back(actor);
		}
		if (!a_ignorePlayer) {
			auto player = RE::PlayerCharacter::GetSingleton();
			if (player && a_ref != player) {
				auto distance = RE::NiPoint3::GetDistance(originPos, player->GetPosition());
				if (distance <= squaredRadius) {
					vec.emplace_back(player);
				}
			}
		}
		UInt32 rand = 0;
		auto RNG = SKSE::RNG::GetSingleton();
		if (RNG) {
			rand = RNG->GenerateRandomInt(0, static_cast<UInt32>(vec.size()) - 1);
		}
		return vec.at(rand);
	}
	return nullptr;
}


UInt32 papyrusObjectReference::HasArtObject(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSArtObject* a_art, bool a_active)
{
	UInt32 count = 0;

	if (!a_ref) {
		a_vm->TraceStack("Cannot get art objects from a None object", a_stackID, Severity::kWarning);
		return count;
	}
	if (!a_art) {
		a_vm->TraceStack("Cannot check against a None art object", a_stackID, Severity::kWarning);
		return count;
	}

	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		processLists->GetMagicEffects([&](RE::BSTempEffect* a_tempEffect)
		{
			auto modelEffect = a_tempEffect->As<RE::ModelReferenceEffect>();
			if (modelEffect) {
				auto handle = a_ref->CreateRefHandle();
				if (modelEffect->target == handle) {
					auto modelArt = modelEffect->artObject;
					if (modelArt && modelArt == a_art) {
						if (!a_active || a_active && !modelEffect->finished) {
							count++;
						}
					}
				}
			}
			return true;
		});
	}

	return count;
}


UInt32 papyrusObjectReference::HasEffectShader(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESEffectShader* a_effectShader, bool a_active)
{
	UInt32 count = 0;

	if (!a_ref) {
		a_vm->TraceStack("Cannot get effectshaders from a None object", a_stackID, Severity::kWarning);
		return count;
	}
	if (!a_effectShader) {
		a_vm->TraceStack("Cannot check against a None effect shader", a_stackID, Severity::kWarning);
		return count;
	}

	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		processLists->GetMagicEffects([&](RE::BSTempEffect* a_tempEffect)
		{
			auto shaderEffect = a_tempEffect->As<RE::ShaderReferenceEffect>();
			if (shaderEffect) {
				auto handle = a_ref->CreateRefHandle();
				if (shaderEffect->target == handle) {
					auto effectData = shaderEffect->effectData;
					if (effectData && effectData == a_effectShader) {
						if (!a_active || a_active && !shaderEffect->finished) {
							count++;
						}
					}
				}
			}
			return true;
		});
	}

	return count;
}


bool papyrusObjectReference::IsLoadDoor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_door)
{
	if (!a_door) {
		a_vm->TraceStack("Cannot check a None door", a_stackID, Severity::kWarning);
		return false;
	}
	return a_door->extraList.HasType(RE::ExtraDataType::kTeleport);
}


bool papyrusObjectReference::HasNiExtraData(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BSFixedString a_name)
{
	if (!a_ref) {
		a_vm->TraceStack("Cannot check a None object", a_stackID, Severity::kWarning);
		return false;
	}
	auto root = a_ref->Get3D();
	if (!root) {
		a_vm->TraceStack("Object has no 3D", a_stackID, Severity::kWarning);
		return false;
	}
	return root->HasExtraData(a_name);
}


std::optional<RE::NiPoint3> FindNearestVertex(RE::TESObjectREFR* a_ref)
{
	auto cell = a_ref->GetParentCell();

	if (!cell || !cell->navMeshes) {
		return std::nullopt;
	}

	auto& navMeshes = *cell->navMeshes;
	auto shortestDistance = std::numeric_limits<float>::max();
	std::optional<RE::NiPoint3> pos = std::nullopt;

	for (auto& navMesh : navMeshes.navMeshes) {
		for (auto& vertex : navMesh->vertices) {
			auto linearDistance = RE::NiPoint3::GetDistance(a_ref->GetPosition(), vertex.location);
			if (linearDistance < shortestDistance) {
				shortestDistance = linearDistance;
				pos.emplace(vertex.location);
			}
		}
	}

	return pos;
}


void papyrusObjectReference::MoveToNearestNavmeshLocation(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
{
	if (!a_ref) {
		a_vm->TraceStack("Cannot move a None object", a_stackID, Severity::kWarning);
		return;
	}
	auto nearestVertex = FindNearestVertex(a_ref);
	if (!nearestVertex) {
		a_vm->TraceStack("Cannot find closest navmesh vertex position", a_stackID, Severity::kWarning);
		return;
	}
	auto handle = a_ref->CreateRefHandle();
	auto task = SKSE::GetTaskInterface();
	task->AddTask([handle, nearestVertex]()
	{
		auto ref = handle.get();
		if (!ref.get()) {
			return;
		}
		ref->SetPosition(std::move(*nearestVertex));
	});
}


bool papyrusObjectReference::RemoveKeywordFromRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSKeyword* a_remove)
{
	if (!a_ref) {
		a_vm->TraceStack("Cannot remove keyword from a None object", a_stackID, Severity::kWarning);
		return false;
	}
	if (!a_remove) {
		a_vm->TraceStack("Cannot remove a None keyword", a_stackID, Severity::kWarning);
		return false;
	}
	auto base = a_ref->GetBaseObject();
	if (base) {
		auto keywords = Serialize::Keywords::GetSingleton();
		if (keywords) {
			return keywords->PapyrusApplyKeywords(base, a_remove, Serialize::Base::kRemove);
		}
	}
	return false;
}


void papyrusObjectReference::ReplaceKeywordOnRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSKeyword* a_remove, RE::BGSKeyword* a_add)
{
	if (a_ref) {
		if (!a_remove) {
			a_vm->TraceStack("Remove keyword is None", a_stackID, Severity::kWarning);
			return;
		}
		if (!a_add) {
			a_vm->TraceStack("Add keyword is None", a_stackID, Severity::kWarning);
			return;
		}
		auto base = a_ref->GetBaseObject();
		if (base) {
			auto keywordForm = base->As<RE::BGSKeywordForm>();
			if (keywordForm) {
				UInt32 removeIndex = 0;
				bool found = false;
				if (keywordForm->keywords) {
					for (UInt32 i = 0; i < keywordForm->numKeywords; i++) {
						auto keyword = keywordForm->keywords[i];
						if (keyword) {
							if (keyword == a_add) {
								return;
							}
							if (keyword == a_remove) {
								removeIndex = i;
								found = true;
							}
						}
					}
				}
				if (found) {
					keywordForm->keywords[removeIndex] = a_add;
				}
			}
		}
	}
	else {
		a_vm->TraceStack("Cannot replace keywords on a None object", a_stackID, Severity::kWarning);
	}
}


void papyrusObjectReference::SetEffectShaderDuration(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESEffectShader* a_effectShader, float a_time, bool a_absolute)
{
	if (!a_ref) {
		a_vm->TraceStack("Cannot set effectshader duration on a None object", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_effectShader) {
		a_vm->TraceStack("Cannot set None effectshader duration", a_stackID, Severity::kWarning);
		return;
	}

	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		processLists->GetMagicEffects([&](RE::BSTempEffect* a_tempEffect)
		{
			auto shaderEffect = a_tempEffect->As<RE::ShaderReferenceEffect>();
			if (shaderEffect) {
				auto handle = a_ref->CreateRefHandle();
				if (shaderEffect->target == handle) {
					auto effectData = shaderEffect->effectData;
					if (effectData && effectData == a_effectShader) {
						if (!a_absolute) {
							float value = shaderEffect->lifetime + a_time;
							if (value >= -1.0) {
								shaderEffect->lifetime = a_time;
							}
						}
						else {
							shaderEffect->lifetime = a_time;
						}
						return false;
					}
				}
			}
			return true;
		});
	}
}


void SetShaderType_Impl(RE::NiAVObject* a_object, RE::BSGeometry* a_template, std::string_view a_path, SInt32 a_textureType, std::vector<std::vector<RE::BSFixedString>>& a_vec, bool a_noWeapons, bool a_noAlpha, bool a_isActor)
{
	RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl
	{
		using State = RE::BSGeometry::States;
		using Feature = RE::BSShaderMaterial::Feature;
		using Texture = RE::BSTextureSet::Texture;
		using Flag = RE::BSShaderProperty::EShaderPropertyFlag8;
		using VertexFlags = RE::NiSkinPartition::VertexFlags;

		bool hasNormals = (RE::NiSkinPartition::GetVertexFlags(a_geometry->vertexDesc) & VertexFlags::VF_NORMAL) == VertexFlags::VF_NORMAL;
		if (!hasNormals) {
			return RE::BSVisit::BSVisitControl::kContinue;
		}
		auto alpha = a_geometry->properties[State::kProperty].get();
		if (alpha && a_noAlpha) {
			return RE::BSVisit::BSVisitControl::kContinue;
		}
		auto parent = a_geometry->parent;
		if (parent) {
			if (a_isActor && a_noWeapons && parent->AsFadeNode()) {
				return RE::BSVisit::BSVisitControl::kContinue;
			}
		}

		auto effect = a_geometry->properties[State::kEffect].get();
		auto tempEffect = a_template->properties[State::kEffect].get();

		if (effect && tempEffect) {
			auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect);
			auto tempLightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(tempEffect);

			if (lightingShader && tempLightingShader) {
				auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
				auto tempMaterial = static_cast<RE::BSLightingShaderMaterialBase*>(tempLightingShader->material);

				if (material && tempMaterial) {
					std::string sourceDiffuse = material->textureSet->GetTexturePath(Texture::kDiffuse);
					RE::Util::SanitizeTexturePath(sourceDiffuse);
					if (!a_path.empty()) {
						if (sourceDiffuse.find(a_path) == std::string::npos) {
							return RE::BSVisit::BSVisitControl::kContinue;
						}
					}

					if (material->GetFeature() != tempMaterial->GetFeature()) {
						auto newMaterial = static_cast<RE::BSLightingShaderMaterialBase*>(tempMaterial->Create());
						if (newMaterial) {
							std::string newDiffuse = tempMaterial->textureSet->GetTexturePath(Texture::kDiffuse);
							RE::Util::SanitizeTexturePath(newDiffuse);

							std::string oldDiffuse = material->textureSet->GetTexturePath(Texture::kDiffuse);
							if (a_vec.empty() || !a_vec.back().empty() && a_vec.back().front() != oldDiffuse) {
								std::vector<RE::BSFixedString> shaderData;
								shaderData.reserve(15);
								for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
									shaderData.emplace_back(material->textureSet->GetTexturePath(i));
								}
								shaderData.emplace_back(std::to_string(to_underlying(lightingShader->flags)));
								shaderData.emplace_back(RE::NiColor::ColorToString(*lightingShader->emissiveColor));
								shaderData.emplace_back(std::to_string(lightingShader->emissiveMult));
								shaderData.emplace_back(std::to_string(to_underlying(tempMaterial->GetFeature())));
								shaderData.emplace_back(std::to_string(to_underlying(material->GetFeature())));
								shaderData.emplace_back(RE::Util::GetTextureName(newDiffuse));
								RE::Util::SanitizeTexturePath(oldDiffuse);
								shaderData.emplace_back(RE::Util::GetTextureName(oldDiffuse));

								a_vec.push_back(shaderData);
							}

							newMaterial->CopyMembers(tempMaterial);
							newMaterial->ClearTextures();
							if (a_textureType != -1) {
								auto newTextureSet = RE::BSShaderTextureSet::Create();
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

							bool isSkinned = (RE::NiSkinPartition::GetVertexFlags(a_geometry->vertexDesc) & VertexFlags::VF_SKINNED) == VertexFlags::VF_SKINNED;
							isSkinned ? lightingShader->SetFlags(Flag::kSkinned, true) : lightingShader->SetFlags(Flag::kSkinned, false);

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


void papyrusObjectReference::SetShaderType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESObjectREFR* a_template, RE::BSFixedString a_filter, UInt32 a_shaderType, SInt32 a_textureType, bool a_noWeapons, bool a_noAlpha)
{
	if (!a_ref || !a_ref->Is3DLoaded()) {
		a_vm->TraceStack("Reference is None or not loaded", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_template || !a_template->Is3DLoaded()) {
		a_vm->TraceStack("Template object is None or not loaded", a_stackID, Severity::kWarning);
		return;
	}

	using Texture = RE::BSTextureSet::Texture;
	using Feature = RE::BSShaderMaterial::Feature;

	auto sourcePath = std::string();
	if (!a_filter.empty()) {
		sourcePath = a_filter.c_str();
		RE::Util::SanitizeTexturePath(sourcePath);
	}
	bool isActor = a_ref->formType == RE::FormType::ActorCharacter ? true : false;
	auto feature = static_cast<Feature>(a_shaderType);
	auto root = a_ref->Get3D()->AsNode();
	auto template_root = a_template->Get3D()->AsNode();

	if (root && template_root) {
		auto task = SKSE::GetTaskInterface();
		task->AddTask([root, template_root, sourcePath, feature, a_textureType, a_noWeapons, a_noAlpha, isActor]()
		{
			auto template_geo = template_root->GetFirstGeometryOfShaderType(feature);
			if (template_geo) {
				std::vector<std::vector<RE::BSFixedString>> vec;
				SetShaderType_Impl(root, template_geo, sourcePath, a_textureType, vec, a_noWeapons, a_noAlpha, isActor);

				if (!vec.empty()) {
					for (auto& _vec : vec) {
						std::string textureName = _vec[12].c_str();
						std::string name = "PO3_SHADER | " + textureName + " | " + _vec.back().c_str();
						auto data = root->GetExtraData<RE::NiStringsExtraData>(name.c_str());
						if (!data) {
							auto newData = RE::NiStringsExtraData::Create(name.c_str(), _vec);
							if (newData) {
								root->AddExtraData(newData);
							}
						}
					}
				}
			}
		});
	}
}


void papyrusObjectReference::SetupBodyPartGeometry(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_miscItem, RE::Actor* a_actor)
{
	if (!a_miscItem) {
		a_vm->TraceStack("BodyParts is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_miscItem->Get3D()) {
		a_vm->TraceStack("BodyParts has no 3D", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_actor) {
		a_vm->TraceStack("Reference actor is None", a_stackID, Severity::kWarning);
		return;
	}
	auto actorbase = a_actor->GetActorBase();
	auto root = a_miscItem->Get3D()->AsFadeNode();
	if (actorbase && root) {
		RE::NiIntegerExtraData* data = nullptr;
		auto actorRoot = a_actor->Get3D(0);
		if (actorRoot) {
			data = actorRoot->GetExtraData<RE::NiIntegerExtraData>("PO3_SKINTINT");
		}
		auto color = data ? RE::NiColor(data->value) : RE::NiColor(actorbase->bodyTintColor);
		auto task = SKSE::GetTaskInterface();
		task->AddTask([root, color]()
		{
			root->UpdateBodyTint(color);
		});
	}
}

void papyrusObjectReference::StopAllShaders_Impl(RE::TESObjectREFR* a_ref)
{
	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		processLists->GetMagicEffects([&](RE::BSTempEffect* a_tempEffect)
		{
			auto referenceEffect = a_tempEffect->As<RE::ReferenceEffect>();
			if (referenceEffect) {
				auto handle = a_ref->CreateRefHandle();
				if (referenceEffect->target == handle) {
					referenceEffect->finished = 1;
				}
			}
			return true;
		});
	}
}


void papyrusObjectReference::StopAllShaders(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
{
	if (!a_ref) {
		a_vm->TraceStack("Cannot stop shaders on a None object", a_stackID, Severity::kWarning);
		return;
	}
	StopAllShaders_Impl(a_ref);
}


void papyrusObjectReference::StopAllSkinAlphaShaders_Impl(RE::TESObjectREFR* a_ref)
{
	using Flags = RE::EffectShaderData::Flags;

	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		processLists->GetMagicEffects([&](RE::BSTempEffect* a_tempEffect)
		{
			auto shaderEffect = a_tempEffect->As<RE::ShaderReferenceEffect>();
			if (shaderEffect) {
				auto handle = a_ref->CreateRefHandle();
				if (shaderEffect->target == handle) {
					auto effectData = shaderEffect->effectData;
					if (effectData && ((effectData->data.flags & Flags::kSkinOnly) == Flags::kSkinOnly) && !effectData->holesTexture.textureName.empty()) {
						shaderEffect->finished = 1;
					}
				}
			}
			return true;
		});
	}
}


void papyrusObjectReference::StopArtObject(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSArtObject* a_art)
{
	if (!a_ref) {
		a_vm->TraceStack("Cannot stop art objects on a None object", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_art) {
		a_vm->TraceStack("Cannot stop None art objects", a_stackID, Severity::kWarning);
		return;
	}

	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		processLists->GetMagicEffects([&](RE::BSTempEffect* a_tempEffect)
		{
			auto modelEffect = a_tempEffect->As<RE::ModelReferenceEffect>();
			if (modelEffect) {
				auto handle = a_ref->CreateRefHandle();
				if (modelEffect->target == handle) {
					auto modelArt = modelEffect->artObject;
					if (modelArt && modelArt == a_art) {
						modelEffect->finished = 1;
					}
				}
			}
			return true;
		});
	}
}


bool papyrusObjectReference::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		_MESSAGE("papyrusObjectReference - couldn't get VMState");
		return false;
	}
	
	a_vm->RegisterFunction("AddAllInventoryItemsToArray", "PO3_SKSEFunctions", AddAllInventoryItemsToArray);

	a_vm->RegisterFunction("AddAllInventoryItemsToList", "PO3_SKSEFunctions", AddAllInventoryItemsToList);

	a_vm->RegisterFunction("AddKeywordToRef", "PO3_SKSEFunctions", AddKeywordToRef);

	a_vm->RegisterFunction("FindAllReferencesOfFormType", "PO3_SKSEFunctions", FindAllReferencesOfFormType);

	a_vm->RegisterFunction("FindAllReferencesOfType", "PO3_SKSEFunctions", FindAllReferencesOfType);

	a_vm->RegisterFunction("FindAllReferencesWithKeyword", "PO3_SKSEFunctions", FindAllReferencesWithKeyword);

	a_vm->RegisterFunction("GetActorCause", "PO3_SKSEFunctions", GetActorCause);

	a_vm->RegisterFunction("GetAllArtObjects", "PO3_SKSEFunctions", GetAllArtObjects);

	a_vm->RegisterFunction("GetAllEffectShaders", "PO3_SKSEFunctions", GetAllEffectShaders);

	a_vm->RegisterFunction("GetClosestActorFromRef", "PO3_SKSEFunctions", GetClosestActorFromRef);

	a_vm->RegisterFunction("GetEffectShaderDuration", "PO3_SKSEFunctions", GetEffectShaderDuration);

	a_vm->RegisterFunction("GetRandomActorFromRef", "PO3_SKSEFunctions", GetRandomActorFromRef);

	a_vm->RegisterFunction("HasArtObject", "PO3_SKSEFunctions", HasArtObject);

	a_vm->RegisterFunction("HasEffectShader", "PO3_SKSEFunctions", HasEffectShader);

	a_vm->RegisterFunction("HasNiExtraData", "PO3_SKSEFunctions", HasNiExtraData);

	a_vm->RegisterFunction("IsLoadDoor", "PO3_SKSEFunctions", IsLoadDoor, true);

	a_vm->RegisterFunction("MoveToNearestNavmeshLocation", "PO3_SKSEFunctions", MoveToNearestNavmeshLocation);

	a_vm->RegisterFunction("RemoveKeywordFromRef", "PO3_SKSEFunctions", RemoveKeywordFromRef);

	a_vm->RegisterFunction("ReplaceKeywordOnRef", "PO3_SKSEFunctions", ReplaceKeywordOnRef);

	a_vm->RegisterFunction("SetEffectShaderDuration", "PO3_SKSEFunctions", SetEffectShaderDuration);

	a_vm->RegisterFunction("SetShaderType", "PO3_SKSEFunctions", SetShaderType);

	a_vm->RegisterFunction("SetupBodyPartGeometry", "PO3_SKSEFunctions", SetupBodyPartGeometry);

	a_vm->RegisterFunction("StopAllShaders", "PO3_SKSEFunctions", StopAllShaders);

	a_vm->RegisterFunction("StopArtObject", "PO3_SKSEFunctions", StopArtObject);

	return true;
}
