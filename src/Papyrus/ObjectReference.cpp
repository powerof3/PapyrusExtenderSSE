#include "Papyrus/ObjectReference.h"

#include "Serialization/Form/Keywords.h"
#include "Util/VMErrors.h"


std::vector<RE::TESForm*> papyrusObjectReference::AddAllItemsToArray(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem)
{
	std::vector<RE::TESForm*> vec;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return vec;
	}

	auto inv = a_ref->GetInventory();
	for (auto& item : inv) {
		auto& [count, entry] = item.second;
		if (count > 0 && entry && entry->CanItemBeTaken(a_noEquipped, a_noFavourited, a_noQuestItem)) {
			vec.push_back(item.first);
		}
	}

	return vec;
}


void papyrusObjectReference::AddAllItemsToList(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSListForm* a_list, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem)
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_list) {
		a_vm->TraceStack("Formlist is None", a_stackID, Severity::kWarning);
		return;
	}

	auto inv = a_ref->GetInventory();
	for (auto& item : inv) {
		auto& [count, entry] = item.second;
		if (count > 0 && entry && entry->CanItemBeTaken(a_noEquipped, a_noFavourited, a_noQuestItem)) {
			a_list->AddForm(item.first);
		}
	}
}


void papyrusObjectReference::AddKeywordToRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSKeyword* a_add)
{
	using namespace Serialization;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_add) {
		a_vm->TraceStack("Keyword is None", a_stackID, Severity::kWarning);
		return;
	}

	auto form = a_ref->GetBaseObject();
	if (form) {
		auto keywords = Form::Keywords::GetSingleton();
		keywords->PapyrusApply(form, a_add, Form::kAdd);
	}
}


void IterateOverAttachedCells(RE::TES* TES, const RE::NiPoint3& a_origin, float a_radius, std::function<bool(RE::TESObjectREFR* a_ref)> a_fn)
{
	auto cell = TES->currentInteriorCell;
	if (cell) {
		cell->ForEachReferenceInRange(a_origin, a_radius, [&](RE::TESObjectREFR* ref) {
			if (!a_fn(ref)) {
				return false;
			}
			return true;
		});
	} else {
		auto gridCellArray = TES->gridCellArray;
		if (gridCellArray) {
			auto gridLength = gridCellArray->length;
			if (gridLength > 0) {
				std::uint32_t x = 0;
				std::uint32_t y = 0;
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
								cell->ForEachReferenceInRange(a_origin, a_radius, [&](RE::TESObjectREFR* a_cellRef) {
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


std::vector<RE::TESObjectREFR*> papyrusObjectReference::FindAllReferencesOfFormType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_origin, std::uint32_t a_formType, float a_radius)
{
	std::vector<RE::TESObjectREFR*> vec;

	if (!a_origin) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return vec;
	}

	auto TES = RE::TES::GetSingleton();
	if (TES) {
		auto formType = static_cast<RE::FormType>(a_formType);
		auto originPos = a_origin->GetPosition();
		auto squaredRadius = a_radius * a_radius;

		IterateOverAttachedCells(TES, originPos, squaredRadius, [&](RE::TESObjectREFR* a_ref) {
			auto form = a_ref->GetBaseObject();
			if (form && form->Is(formType)) {
				vec.push_back(a_ref);
			}
			return true;
		});

		if (vec.empty()) {
			auto worldSpace = TES->worldSpace;
			if (worldSpace) {
				auto skyCell = worldSpace->GetOrCreateSkyCell();
				if (skyCell) {
					skyCell->ForEachReferenceInRange(originPos, squaredRadius, [&](RE::TESObjectREFR* a_cellRef) {
						auto form = a_cellRef->GetBaseObject();
						if (form && form->Is(formType)) {
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
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return vec;
	} else if (!a_formOrList) {
		a_vm->TraceStack("FormOrList is None", a_stackID, Severity::kWarning);
		return vec;
	}

	auto TES = RE::TES::GetSingleton();
	if (TES) {
		auto list = a_formOrList->As<RE::BGSListForm>();
		auto originPos = a_origin->GetPosition();
		auto squaredRadius = a_radius * a_radius;

		IterateOverAttachedCells(TES, originPos, squaredRadius, [&](RE::TESObjectREFR* a_ref) {
			auto base = a_ref->GetBaseObject();
			if (base) {
				if ((list && list->HasForm(base)) || a_formOrList == base) {
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
					skyCell->ForEachReferenceInRange(originPos, squaredRadius, [&](RE::TESObjectREFR* a_cellRef) {
						auto base = a_cellRef->GetBaseObject();
						if (base) {
							if ((list && list->HasForm(base)) || a_formOrList == base) {
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
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return vec;
	} else if (!a_formOrList) {
		a_vm->TraceStack("FormOrList is None", a_stackID, Severity::kWarning);
		return vec;
	}

	auto keyword = a_formOrList->As<RE::BGSKeyword>();
	auto list = a_formOrList->As<RE::BGSListForm>();

	if (!keyword && !list) {
		a_vm->TraceStack("FormOrList parameter has invalid formtype", a_stackID, Severity::kWarning);
		return vec;
	}

	auto TES = RE::TES::GetSingleton();
	if (TES) {
		auto originPos = a_origin->GetPosition();
		float squaredRadius = a_radius * a_radius;

		IterateOverAttachedCells(TES, originPos, squaredRadius, [&](RE::TESObjectREFR* a_ref) {
			bool success = false;
			if (list) {
				success = a_matchAll ? a_ref->HasAllKeywords(list) : a_ref->HasKeywords(list);
			} else if (keyword) {
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
				auto skyCell = worldSpace->GetOrCreateSkyCell();
				if (skyCell) {
					skyCell->ForEachReferenceInRange(originPos, squaredRadius, [&](RE::TESObjectREFR* a_cellRef) {
						bool success = false;
						if (list) {
							success = a_matchAll ? a_cellRef->HasAllKeywords(list) : a_cellRef->HasKeywords(list);
						} else if (keyword) {
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


RE::TESForm* papyrusObjectReference::FindFirstItemInList(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSListForm* a_list)
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return nullptr;
	} else if (!a_list) {
		a_vm->TraceStack("Formlist is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	auto inv = a_ref->GetInventory();
	for (auto& item : inv) {
		if (a_list->HasForm(item.first)) {
			return item.first;
		}
	}

	return nullptr;
}


RE::Actor* papyrusObjectReference::GetActorCause(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
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


std::vector<RE::BGSArtObject*> papyrusObjectReference::GetAllArtObjects(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
{
	std::vector<RE::BGSArtObject*> vec;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return vec;
	}

	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		processLists->GetMagicEffects([&](RE::BSTempEffect* a_tempEffect) {
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
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return vec;
	}

	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		processLists->GetMagicEffects([&](RE::BSTempEffect* a_tempEffect) {
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


RE::Actor* papyrusObjectReference::GetClosestActorFromRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, bool a_ignorePlayer)
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		if (a_ignorePlayer && processLists->numberHighActors == 0) {
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

		auto it = std::find_if(map.begin(), map.end(), [shortestDistance](const auto& mo) {
			return mo.second == shortestDistance;
		});
		if (it != map.end()) {
			return it->first;
		}
	}

	return nullptr;
}


float papyrusObjectReference::GetEffectShaderDuration(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESEffectShader* a_effectShader)
{
	float time = 0.0f;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return time;
	} else if (!a_effectShader) {
		a_vm->TraceStack("Effect Shader is None", a_stackID, Severity::kWarning);
		return time;
	}

	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		processLists->GetMagicEffects([&](RE::BSTempEffect* a_tempEffect) {
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


RE::TESObjectREFR* papyrusObjectReference::GetDoorDestination(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_door)
{
	if (a_door) {
		auto teleport = a_door->extraList.GetByType<RE::ExtraTeleport>();
		if (teleport) {
			auto teleportData = teleport->teleportData;
			if (teleportData) {
				auto doorPtr = teleportData->linkedDoor.get();
				return doorPtr.get();
			}
		} else {
			a_vm->TraceStack(VMError::generic_error(a_door, "is not a load door").c_str(), a_stackID, Severity::kWarning);
		}
	} else {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
	}

	return nullptr;
}


std::vector<RE::TESObjectREFR*> papyrusObjectReference::GetLinkedChildren(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
{
	std::vector<RE::TESObjectREFR*> vec;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return vec;
	}

	auto data = a_ref->extraList.GetByType<RE::ExtraLinkedRefChildren>();
	if (data) {
		auto& linkedChildren = data->linkedChildren;
		if (!linkedChildren.empty()) {
			vec.reserve(linkedChildren.size());
			for (auto& child : linkedChildren) {
				auto refPtr = child.refr.get();
				auto ref = refPtr.get();
				if (ref) {
					vec.emplace_back(ref);
				}
			}
		}
	}

	return vec;
}


std::vector<RE::TESForm*> papyrusObjectReference::GetMagicEffectSource(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::EffectSetting* a_mgef)
{
	std::vector<RE::TESForm*> vec;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return vec;
	} else if (!a_mgef) {
		a_vm->TraceStack("Magic Effect is None", a_stackID, Severity::kWarning);
		return vec;
	}

	auto magicTarget = a_ref->GetMagicTarget();
	if (magicTarget) {
		auto activeEffects = magicTarget->GetActiveEffectList();
		if (activeEffects) {
			for (auto& activeEffect : *activeEffects) {
				if (activeEffect) {
					auto mgef = activeEffect->GetBaseObject();
					if (mgef && mgef == a_mgef) {
						vec.push_back(activeEffect->spell);
						auto casterPtr = activeEffect->caster.get();
						auto caster = casterPtr.get();
						if (caster) {
							vec.push_back(caster);
						}
						break;
					}
				}
			}
		}
	}

	return vec;
}


void GetMaterialType_Impl(RE::bhkWorldObject* a_body, std::vector<RE::BSFixedString>& vec)
{
	if (!a_body) {
		return;
	}

	a_body->EnterCriticalOperationRead();

	auto& materialMap = papyrusObjectReference::materialMap;

	auto hkpBody = static_cast<RE::hkpWorldObject*>(a_body->referencedObject.get());
	if (hkpBody) {
		auto hkpShape = hkpBody->GetShape();
		if (hkpShape) {
			switch (hkpShape->type) {
			case RE::hkpShapeType::kMOPP:
				{
					auto mopp = static_cast<const RE::hkpMoppBvTreeShape*>(hkpShape);
					if (mopp) {
						auto childShape = mopp->child.childShape;
						if (childShape) {
							auto bhkShape = childShape->userData;
							if (bhkShape) {
								auto compressedShape = netimmerse_cast<RE::bhkCompressedMeshShape*>(bhkShape);
								if (compressedShape) {
									auto data = compressedShape->data.get();
									if (data) {
										for (auto& meshMaterial : data->meshMaterials) {
											vec.emplace_back(materialMap.at(meshMaterial.materialID).data());
										}
									}
								}
							}
						}
					}
				}
				break;
			default:
				{
					auto bhkShape = hkpShape->userData;
					if (bhkShape) {
						vec.emplace_back(materialMap.at(bhkShape->materialID).data());
					}
				}
				break;
			}
		}
	}

	a_body->ExitCriticalOperationRead();
}


std::vector<RE::BSFixedString> papyrusObjectReference::GetMaterialType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BSFixedString a_nodeName)
{
	std::vector<RE::BSFixedString> vec;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return vec;
	}

	auto root = a_ref->Get3D();
	if (!root) {
		a_vm->TraceStack(VMError::no_3D(a_ref).c_str(), a_stackID, Severity::kWarning);
		return vec;
	}

	if (!a_nodeName.empty()) {
		auto object = root->GetObjectByName(a_nodeName);
		if (object) {
			auto colObject = static_cast<RE::bhkNiCollisionObject*>(object->collisionObject.get());
			if (colObject) {
				GetMaterialType_Impl(colObject->body.get(), vec);
			}
		}
	} else {
		RE::BSVisit::TraverseScenegraphCollision(root, [&](RE::NiCollisionObject* a_col) -> RE::BSVisit::BSVisitControl {
			auto colObject = static_cast<RE::bhkNiCollisionObject*>(a_col);
			if (colObject) {
				GetMaterialType_Impl(colObject->body.get(), vec);
			}
			return RE::BSVisit::BSVisitControl::kStop;
		});
	}

	return vec;
}


std::int32_t papyrusObjectReference::GetMotionType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
{
	std::int32_t motionType = -1;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return motionType;
	}

	auto root = a_ref->Get3D();
	if (!root) {
		a_vm->TraceStack(VMError::no_3D(a_ref).c_str(), a_stackID, Severity::kWarning);
		return motionType;
	}

	RE::BSVisit::TraverseScenegraphCollision(root, [&](RE::NiCollisionObject* a_col) -> RE::BSVisit::BSVisitControl {
		auto colObject = static_cast<RE::bhkNiCollisionObject*>(a_col);
		if (colObject) {
			auto body = colObject->body.get();
			if (body) {
				body->EnterCriticalOperationRead();
				auto hkpRigidBody = static_cast<RE::hkpRigidBody*>(body->referencedObject.get());
				if (hkpRigidBody) {
					motionType = hkpRigidBody->motion.type.underlying();
				}
				body->ExitCriticalOperationRead();
			}
		}
		return motionType != -1 ? RE::BSVisit::BSVisitControl::kStop : RE::BSVisit::BSVisitControl::kContinue;
	});

	return motionType;
}


RE::Actor* papyrusObjectReference::GetRandomActorFromRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, float a_radius, bool a_ignorePlayer)
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		if (a_ignorePlayer && processLists->numberHighActors == 0) {
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

		if (!vec.empty()) {
			auto RNG = SKSE::RNG::GetSingleton();
			auto num = RNG->GenerateRandomNumber<std::uint32_t>(0, static_cast<std::uint32_t>(vec.size() - 1));
			return vec[num];
		}
	}

	return nullptr;
}


std::uint32_t papyrusObjectReference::HasArtObject(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSArtObject* a_art, bool a_active)
{
	std::uint32_t count = 0;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return count;
	} else if (!a_art) {
		a_vm->TraceStack("Art Object is None", a_stackID, Severity::kWarning);
		return count;
	}

	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		processLists->GetMagicEffects([&](RE::BSTempEffect* a_tempEffect) {
			auto modelEffect = a_tempEffect->As<RE::ModelReferenceEffect>();
			if (modelEffect) {
				auto handle = a_ref->CreateRefHandle();
				if (modelEffect->target == handle) {
					auto modelArt = modelEffect->artObject;
					if (modelArt && modelArt == a_art) {
						if (!a_active || (a_active && !modelEffect->finished)) {
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


std::uint32_t papyrusObjectReference::HasEffectShader(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESEffectShader* a_effectShader, bool a_active)
{
	std::uint32_t count = 0;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return count;
	} else if (!a_effectShader) {
		a_vm->TraceStack("Effect Shader is None", a_stackID, Severity::kWarning);
		return count;
	}

	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		processLists->GetMagicEffects([&](RE::BSTempEffect* a_tempEffect) {
			auto shaderEffect = a_tempEffect->As<RE::ShaderReferenceEffect>();
			if (shaderEffect) {
				auto handle = a_ref->CreateRefHandle();
				if (shaderEffect->target == handle) {
					auto effectData = shaderEffect->effectData;
					if (effectData && effectData == a_effectShader) {
						if (!a_active || (a_active && !shaderEffect->finished)) {
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
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return false;
	}

	return a_door->extraList.HasType(RE::ExtraDataType::kTeleport);
}


bool papyrusObjectReference::IsQuestItem(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return false;
	}

	return a_ref->HasQuestObject();
}


bool papyrusObjectReference::IsVIP(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
{
	using FILL_TYPE = RE::BGSBaseAlias::FILL_TYPE;
	using Flags = RE::BGSBaseAlias::FLAGS;
	using TYPE = RE::QUEST_DATA::Type;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return false;
	}

	bool isVIP = false;

	auto xAlias = a_ref->extraList.GetByType<RE::ExtraAliasInstanceArray>();
	if (xAlias) {
		xAlias->lock.LockForRead();
		for (auto& aliasData : xAlias->aliases) {
			if (aliasData) {
				auto alias = aliasData->alias;
				if (alias) {
					auto quest = alias->owningQuest;
					if (quest && quest->GetType() != TYPE::kNone && quest->IsRunning()) {
						auto fillType = alias->fillType.get();
						if (alias->IsQuestObject() || alias->IsEssential() || alias->IsProtected() || fillType == FILL_TYPE::kForced || fillType == FILL_TYPE::kUniqueActor) {
							isVIP = true;
						}
					}
				}
			}
		}
		xAlias->lock.UnlockForRead();
	}

	return isVIP;
}


bool papyrusObjectReference::HasNiExtraData(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BSFixedString a_name)
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return false;
	}

	auto root = a_ref->Get3D();
	if (!root) {
		a_vm->TraceStack(VMError::no_3D(a_ref).c_str(), a_stackID, Severity::kWarning);
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
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return;
	}

	auto nearestVertex = FindNearestVertex(a_ref);
	if (!nearestVertex) {
		a_vm->TraceStack(VMError::generic_error(a_ref, "does not have a valid navmesh position").c_str(), a_stackID, Severity::kWarning);
		return;
	}

	auto handle = a_ref->CreateRefHandle();
	auto task = SKSE::GetTaskInterface();
	task->AddTask([handle, nearestVertex]() {
		auto ref = handle.get();
		if (!ref.get()) {
			return;
		}
		ref->SetPosition(std::move(*nearestVertex));
	});
}


bool papyrusObjectReference::RemoveKeywordFromRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSKeyword* a_remove)
{
	using namespace Serialization;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return false;
	} else if (!a_remove) {
		a_vm->TraceStack("Keyword is None", a_stackID, Severity::kWarning);
		return false;
	}

	auto form = a_ref->GetBaseObject();
	auto keywords = Form::Keywords::GetSingleton();
	return form ? keywords->PapyrusApply(form, a_remove, Form::kRemove) : false;
}


void papyrusObjectReference::ReplaceKeywordOnRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSKeyword* a_remove, RE::BGSKeyword* a_add)
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_remove) {
		a_vm->TraceStack("Remove keyword is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_add) {
		a_vm->TraceStack("Add keyword is None", a_stackID, Severity::kWarning);
		return;
	}

	auto base = a_ref->GetBaseObject();
	if (base) {
		auto keywordForm = base->As<RE::BGSKeywordForm>();
		if (keywordForm) {
			std::uint32_t removeIndex = 0;
			bool found = false;
			if (keywordForm->keywords) {
				for (std::uint32_t i = 0; i < keywordForm->numKeywords; i++) {
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


void ScaleObject3D_Impl(RE::bhkWorldObject* a_body, float a_scale)
{
	if (!a_body) {
		return;
	}

	a_body->EnterCriticalOperationWrite();

	auto hkpBody = static_cast<RE::hkpWorldObject*>(a_body->referencedObject.get());
	if (hkpBody) {
		auto shape = hkpBody->GetShape();
		if (shape) {
			switch (shape->type) {
			case RE::hkpShapeType::kBox:
				{
					auto boxShape = const_cast<RE::hkpBoxShape*>(static_cast<const RE::hkpBoxShape*>(shape));
					if (boxShape) {
						boxShape->SetHalfExtents(boxShape->halfExtents * RE::hkVector4(a_scale));
					}
				}
				break;
			case RE::hkpShapeType::kSphere:
				{
					auto sphereShape = const_cast<RE::hkpConvexShape*>(static_cast<const RE::hkpConvexShape*>(shape));
					if (sphereShape) {
						sphereShape->radius *= a_scale;
					}
				}
				break;
			default:
				break;
			}
		}
	}

	a_body->ExitCriticalOperationWrite();
}


void papyrusObjectReference::ScaleObject3D(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BSFixedString a_nodeName, float a_scale)
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return;
	}

	auto root = a_ref->Get3D();
	if (!root) {
		a_vm->TraceStack(VMError::no_3D(a_ref).c_str(), a_stackID, Severity::kWarning);
		return;
	}

	if (!a_nodeName.empty()) {
		auto object = root->GetObjectByName(a_nodeName);
		if (object) {
			auto task = SKSE::GetTaskInterface();
			task->AddTask([object, a_scale]() {
				object->local.scale *= a_scale;
				RE::NiUpdateData updateData = { 0.0f, RE::NiUpdateData::Flag::kNone };
				object->Update(updateData);
			});
			auto colObject = static_cast<RE::bhkNiCollisionObject*>(object->collisionObject.get());
			if (colObject) {
				ScaleObject3D_Impl(colObject->body.get(), a_scale);
			}
		}
	} else {
		auto task = SKSE::GetTaskInterface();
		task->AddTask([root, a_scale]() {
			root->local.scale *= a_scale;
			RE::NiUpdateData updateData = { 0.0f, RE::NiUpdateData::Flag::kNone };
			root->Update(updateData);
		});
		RE::BSVisit::TraverseScenegraphCollision(root, [&](RE::NiCollisionObject* a_col) -> RE::BSVisit::BSVisitControl {
			auto colObject = static_cast<RE::bhkNiCollisionObject*>(a_col);
			if (colObject) {
				ScaleObject3D_Impl(colObject->body.get(), a_scale);
			}
			return RE::BSVisit::BSVisitControl::kContinue;
		});
	}

	auto data = root->GetExtraData<RE::NiFloatExtraData>("PO3_SCALE");
	if (!data) {
		auto newData = RE::NiFloatExtraData::Create("PO3_SCALE", a_scale);
		if (newData) {
			root->AddExtraData(newData);
		}
	}
}


void papyrusObjectReference::SetEffectShaderDuration(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESEffectShader* a_effectShader, float a_time, bool a_absolute)
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_effectShader) {
		a_vm->TraceStack("Effect Shader is None", a_stackID, Severity::kWarning);
		return;
	}

	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		processLists->GetMagicEffects([&](RE::BSTempEffect* a_tempEffect) {
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
						} else {
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


bool papyrusObjectReference::SetDoorDestination(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_door, RE::TESObjectREFR* a_newDoor)
{
	if (!a_door) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return false;
	} else if (!a_newDoor) {
		a_vm->TraceStack("Destination Door is None", a_stackID, Severity::kWarning);
		return false;
	}

	auto teleport = a_door->extraList.GetByType<RE::ExtraTeleport>();
	if (teleport) {
		auto teleportData = teleport->teleportData;
		if (teleportData) {
			teleportData->linkedDoor = a_newDoor->CreateRefHandle();
			return true;
		} else {
			a_vm->TraceStack(VMError::generic_error(a_door, "- couldn't get teleport data").c_str(), a_stackID, Severity::kWarning);
			return false;
		}
	} else {
		a_vm->TraceStack(VMError::generic_error(a_door, "is not a load door").c_str(), a_stackID, Severity::kWarning);
		return false;
	}
}


void papyrusObjectReference::SetLinkedRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESObjectREFR* a_targetRef, RE::BGSKeyword* a_keyword)
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_targetRef) {
		a_vm->TraceStack("Target Object Reference is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_keyword) {
		a_vm->TraceStack("Keyword Reference is None", a_stackID, Severity::kWarning);
		return;
	}

	a_ref->extraList.SetLinkedRef(a_targetRef, a_keyword);
}


void SetMaterialType_Impl(RE::bhkWorldObject* a_body, RE::MATERIAL_ID newID, RE::MATERIAL_ID oldID)
{
	if (!a_body) {
		return;
	}

	a_body->EnterCriticalOperationWrite();

	auto hkpBody = static_cast<RE::hkpWorldObject*>(a_body->referencedObject.get());
	if (hkpBody) {
		auto hkpShape = hkpBody->GetShape();
		if (hkpShape) {
			switch (hkpShape->type) {
			case RE::hkpShapeType::kMOPP:
				{
					auto mopp = static_cast<const RE::hkpMoppBvTreeShape*>(hkpShape);
					if (mopp) {
						auto childShape = mopp->child.childShape;
						if (childShape) {
							auto bhkShape = childShape->userData;
							if (bhkShape) {
								auto compressedShape = netimmerse_cast<RE::bhkCompressedMeshShape*>(bhkShape);
								if (compressedShape) {
									auto data = compressedShape->data.get();
									if (data) {
										for (auto& meshMaterial : data->meshMaterials) {
											if (oldID != RE::MATERIAL_ID::kNone) {
												if (meshMaterial.materialID != oldID) {
													continue;
												}
											}
											meshMaterial.materialID = newID;
										}
									}
								}
							}
						}
					}
				}
				break;
			default:
				{
					auto bhkShape = hkpShape->userData;
					if (bhkShape) {
						if (oldID != RE::MATERIAL_ID::kNone) {
							if (bhkShape->materialID != oldID) {
								return;
							}
						}
						bhkShape->materialID = newID;
					}
				}
				break;
			}
		}
	}

	a_body->ExitCriticalOperationWrite();
}


void papyrusObjectReference::SetMaterialType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BSFixedString a_newMaterialType, RE::BSFixedString a_oldMaterialType, RE::BSFixedString a_nodeName)
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return;
	} else if (a_newMaterialType.empty()) {
		a_vm->TraceStack("Material type string is empty", a_stackID, Severity::kWarning);
		return;
	}

	auto root = a_ref->Get3D();
	if (!root) {
		a_vm->TraceStack(VMError::no_3D(a_ref).c_str(), a_stackID, Severity::kWarning);
		return;
	}

	auto newID = RE::MATERIAL_ID::kNone;
	auto oldID = RE::MATERIAL_ID::kNone;
	for (auto& mat : materialMap) {
		if (_stricmp(mat.second.data(), a_newMaterialType.data()) == 0) {
			newID = mat.first;
			break;
		}
		if (!a_oldMaterialType.empty()) {
			if (_stricmp(mat.second.data(), a_oldMaterialType.data()) == 0) {
				oldID = mat.first;
				break;
			}
		}
	}

	if (newID != RE::MATERIAL_ID::kNone) {
		if (!a_nodeName.empty()) {
			auto object = root->GetObjectByName(a_nodeName);
			if (object) {
				auto colObject = static_cast<RE::bhkNiCollisionObject*>(object->collisionObject.get());
				if (colObject) {
					SetMaterialType_Impl(colObject->body.get(), newID, oldID);
				}
			}
		} else {
			RE::BSVisit::TraverseScenegraphCollision(root, [&](RE::NiCollisionObject* a_col) -> RE::BSVisit::BSVisitControl {
				auto colObject = static_cast<RE::bhkNiCollisionObject*>(a_col);
				if (colObject) {
					SetMaterialType_Impl(colObject->body.get(), newID, oldID);
				}
				return RE::BSVisit::BSVisitControl::kContinue;
			});
		}
	}
}


void SetShaderType_Impl(RE::NiAVObject* a_object, RE::BSGeometry* a_template, std::string_view a_path, std::int32_t a_textureType, std::vector<std::vector<RE::BSFixedString>>& a_vec, bool a_noWeapons, bool a_noAlpha, bool a_isActor)
{
	RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
		using State = RE::BSGeometry::States;
		using Feature = RE::BSShaderMaterial::Feature;
		using Texture = RE::BSTextureSet::Texture;
		using Flag = RE::BSShaderProperty::EShaderPropertyFlag8;
		using VertexFlags = RE::NiSkinPartition::Vertex::Flags;

		bool hasNormals = (RE::NiSkinPartition::GetVertexFlags(a_geometry->vertexDesc) & VertexFlags::kNormal) == VertexFlags::kNormal;
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
					auto origFeature = material->GetFeature();
					if (origFeature == Feature::kEye) {	 //disabled until flag reset is fixed
						return RE::BSVisit::BSVisitControl::kContinue;
					}
					if (origFeature != tempMaterial->GetFeature()) {
						auto newMaterial = static_cast<RE::BSLightingShaderMaterialBase*>(tempMaterial->Create());
						if (newMaterial) {
							std::string newDiffuse = tempMaterial->textureSet->GetTexturePath(Texture::kDiffuse);
							RE::Util::SanitizeTexturePath(newDiffuse);

							std::string oldDiffuse = material->textureSet->GetTexturePath(Texture::kDiffuse);
							if (a_vec.empty() || (!a_vec.back().empty() && a_vec.back().front() != oldDiffuse.c_str())) {
								std::vector<RE::BSFixedString> resetData;
								resetData.reserve(15);

								for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
									resetData.emplace_back(material->textureSet->GetTexturePath(i));
								}
								resetData.emplace_back(std::to_string(to_underlying(*lightingShader->flags)));
								resetData.emplace_back(RE::NiColor::ColorToString(*lightingShader->emissiveColor));
								resetData.emplace_back(std::to_string(lightingShader->emissiveMult));
								resetData.emplace_back(std::to_string(to_underlying(tempMaterial->GetFeature())));
								resetData.emplace_back(std::to_string(to_underlying(origFeature)));
								resetData.emplace_back(RE::Util::GetTextureName(newDiffuse));
								RE::Util::SanitizeTexturePath(oldDiffuse);
								resetData.emplace_back(RE::Util::GetTextureName(oldDiffuse));

								a_vec.push_back(resetData);
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

							bool isSkinned = (RE::NiSkinPartition::GetVertexFlags(a_geometry->vertexDesc) & VertexFlags::kSkinned) == VertexFlags::kSkinned;
							lightingShader->SetFlags(Flag::kSkinned, isSkinned);

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


void papyrusObjectReference::SetShaderType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESObjectREFR* a_template, RE::BSFixedString a_filter, std::uint32_t a_shaderType, std::int32_t a_textureType, bool a_noWeapons, bool a_noAlpha)
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_template) {
		a_vm->TraceStack("Template is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_ref->Is3DLoaded()) {
		a_vm->TraceStack(VMError::no_3D(a_ref).c_str(), a_stackID, Severity::kWarning);
		return;
	} else if (!a_template->Is3DLoaded()) {
		a_vm->TraceStack(VMError::generic_error(a_template, "template object has no 3D").c_str(), a_stackID, Severity::kWarning);
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
		task->AddTask([root, template_root, feature, sourcePath, a_textureType, a_noWeapons, a_noAlpha, isActor]() {
			auto template_geo = template_root->GetFirstGeometryOfShaderType(feature);
			if (template_geo) {
				std::vector<std::vector<RE::BSFixedString>> vec;
				SetShaderType_Impl(root, template_geo, sourcePath, a_textureType, vec, a_noWeapons, a_noAlpha, isActor);

				if (!vec.empty()) {
					for (auto& _vec : vec) {
						std::string textureName = _vec.at(12).c_str();
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


void papyrusObjectReference::SetupBodyPartGeometry(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_bodyparts, RE::Actor* a_actor)
{
	if (!a_bodyparts) {
		a_vm->TraceStack("BodyParts is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_bodyparts->Get3D()) {
		a_vm->TraceStack(VMError::no_3D(a_bodyparts).c_str(), a_stackID, Severity::kWarning);
		return;
	}

	auto actorbase = a_actor->GetActorBase();
	auto root = a_bodyparts->Get3D()->AsFadeNode();

	if (actorbase && root) {
		auto actorRoot = a_actor->Get3D(0);
		RE::NiIntegerExtraData* data = nullptr;
		if (actorRoot) {
			data = actorRoot->GetExtraData<RE::NiIntegerExtraData>("PO3_SKINTINT");
		}

		auto color = data ? RE::NiColor(data->value) : RE::NiColor(actorbase->bodyTintColor);
		auto task = SKSE::GetTaskInterface();
		task->AddTask([root, color]() {
			root->UpdateBodyTint(color);
		});
	}
}


void papyrusObjectReference::StopAllShaders(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return;
	}

	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		processLists->StopAllShaders(a_ref);
	}
}


void papyrusObjectReference::StopArtObject(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSArtObject* a_art)
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_art) {
		a_vm->TraceStack("Art Object is None", a_stackID, Severity::kWarning);
		return;
	}

	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		processLists->GetMagicEffects([&](RE::BSTempEffect* a_tempEffect) {
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


void papyrusObjectReference::ToggleChildNode(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BSFixedString a_nodeName, bool a_disable)
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return;
	}

	auto root = a_ref->Get3D();
	if (!root) {
		a_vm->TraceStack(VMError::no_3D(a_ref).c_str(), a_stackID, Severity::kWarning);
		return;
	}

	auto object = root->GetObjectByName(a_nodeName);
	if (object) {
		auto task = SKSE::GetTaskInterface();
		task->AddTask([object, a_disable]() {
			object->UpdateVisibility(a_disable);
		});

		auto data = root->GetExtraData<RE::NiStringsExtraData>("PO3_TOGGLE");
		if (!data) {
			if (a_disable) {
				std::vector<RE::BSFixedString> vec;
				vec.push_back(a_nodeName);
				auto newData = RE::NiStringsExtraData::Create("PO3_TOGGLE", vec);
				if (newData) {
					root->AddExtraData(newData);
				}
			}
		} else {
			a_disable == true ? data->InsertElement(a_nodeName) : data->RemoveElement(a_nodeName);
		}
	}
}


void papyrusObjectReference::UpdateHitEffectArtNode(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSArtObject* a_art, RE::BSFixedString a_toNode, std::vector<float> a_translate, std::vector<float> a_rotate, float a_scale)
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_art) {
		a_vm->TraceStack("Art is None", a_stackID, Severity::kWarning);
		return;
	} else if (a_toNode.empty()) {
		a_vm->TraceStack("Node name is empty", a_stackID, Severity::kWarning);
		return;
	}

	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		processLists->GetMagicEffects([&](RE::BSTempEffect* a_tempEffect) {
			auto modelEffect = a_tempEffect->As<RE::ModelReferenceEffect>();
			if (modelEffect) {
				auto handle = a_ref->CreateRefHandle();
				if (modelEffect->target == handle && modelEffect->artObject == a_art) {
					if (modelEffect->hitEffectArtData.nodeName == a_toNode) {
						return false;
					}
					auto current3DRoot = modelEffect->hitEffectArtData.current3DRoot.get();
					auto art = modelEffect->hitEffectArtData.attachedArt.get();
					if (current3DRoot && art) {
						auto newNode = current3DRoot->GetObjectByName(a_toNode);
						if (!newNode) {
							std::string error = "Node '" + std::string(a_toNode.c_str()) + "' doesn't exist on " + VMError::to_id(a_ref);
							a_vm->TraceStack(error.c_str(), a_stackID, Severity::kWarning);
							return false;
						}
						auto task = SKSE::GetTaskInterface();
						task->AddTask([a_toNode, a_translate, a_rotate, a_scale, art, newNode, modelEffect]() {
							if (newNode->AsNode()) {
								auto attachTData = art->GetExtraData<RE::NiStringsExtraData>("AttachT");
								if (attachTData) {
									constexpr std::string_view namedNode = "NamedNode&";
									auto oldNodeStr = namedNode.data() + std::string(modelEffect->hitEffectArtData.nodeName);
									auto newNodeStr = namedNode.data() + std::string(a_toNode);
									attachTData->RemoveElement(oldNodeStr.c_str());
									attachTData->InsertElement(newNodeStr.c_str());
								}
								for (auto& nodes : art->children) {
									if (nodes.get()) {
										if (!a_translate.empty() && a_translate.size() == 3) {
											nodes->local.translate = { a_translate[0], a_translate[1], a_translate[2] };
										}
										if (!a_rotate.empty() && a_rotate.size() == 3) {
											float aX = RE::degToRad(a_rotate[0]);
											float aY = RE::degToRad(a_rotate[1]);
											float aZ = RE::degToRad(a_rotate[2]);
											nodes->local.rotate.SetEulerAnglesXYZ(aX, aY, aZ);
										}
										nodes->local.scale *= a_scale;
									}
								}
								newNode->AsNode()->AttachChild(art);
								RE::NiUpdateData data = { 0.0f, RE::NiUpdateData::Flag::kDirty };
								art->UpdateWorldData(&data);
							}
							modelEffect->hitEffectArtData.nodeName = a_toNode;
							modelEffect->UpdatePosition();
						});
					}
					return false;
				}
			}
			return true;
		});
	}
}


bool papyrusObjectReference::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("papyrusObjectReference - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("AddAllItemsToArray", "PO3_SKSEFunctions", AddAllItemsToArray);

	a_vm->RegisterFunction("AddAllItemsToList", "PO3_SKSEFunctions", AddAllItemsToList);

	a_vm->RegisterFunction("AddKeywordToRef", "PO3_SKSEFunctions", AddKeywordToRef);

	a_vm->RegisterFunction("FindAllReferencesOfFormType", "PO3_SKSEFunctions", FindAllReferencesOfFormType);

	a_vm->RegisterFunction("FindAllReferencesOfType", "PO3_SKSEFunctions", FindAllReferencesOfType);

	a_vm->RegisterFunction("FindAllReferencesWithKeyword", "PO3_SKSEFunctions", FindAllReferencesWithKeyword);

	a_vm->RegisterFunction("FindFirstItemInList", "PO3_SKSEFunctions", FindFirstItemInList);

	a_vm->RegisterFunction("GetActorCause", "PO3_SKSEFunctions", GetActorCause);

	a_vm->RegisterFunction("GetAllArtObjects", "PO3_SKSEFunctions", GetAllArtObjects);

	a_vm->RegisterFunction("GetAllEffectShaders", "PO3_SKSEFunctions", GetAllEffectShaders);

	a_vm->RegisterFunction("GetClosestActorFromRef", "PO3_SKSEFunctions", GetClosestActorFromRef);

	a_vm->RegisterFunction("GetDoorDestination", "PO3_SKSEFunctions", GetDoorDestination);

	a_vm->RegisterFunction("GetEffectShaderDuration", "PO3_SKSEFunctions", GetEffectShaderDuration);

	a_vm->RegisterFunction("GetMagicEffectSource", "PO3_SKSEFunctions", GetMagicEffectSource);

	a_vm->RegisterFunction("GetMaterialType", "PO3_SKSEFunctions", GetMaterialType);

	a_vm->RegisterFunction("GetMotionType", "PO3_SKSEFunctions", GetMotionType);

	a_vm->RegisterFunction("GetRandomActorFromRef", "PO3_SKSEFunctions", GetRandomActorFromRef);

	a_vm->RegisterFunction("HasArtObject", "PO3_SKSEFunctions", HasArtObject);

	a_vm->RegisterFunction("HasEffectShader", "PO3_SKSEFunctions", HasEffectShader);

	a_vm->RegisterFunction("HasNiExtraData", "PO3_SKSEFunctions", HasNiExtraData);

	a_vm->RegisterFunction("IsLoadDoor", "PO3_SKSEFunctions", IsLoadDoor, true);

	a_vm->RegisterFunction("IsQuestItem", "PO3_SKSEFunctions", IsQuestItem);

	a_vm->RegisterFunction("IsVIP", "PO3_SKSEFunctions", IsVIP);

	a_vm->RegisterFunction("MoveToNearestNavmeshLocation", "PO3_SKSEFunctions", MoveToNearestNavmeshLocation);

	a_vm->RegisterFunction("RemoveKeywordFromRef", "PO3_SKSEFunctions", RemoveKeywordFromRef);

	a_vm->RegisterFunction("ReplaceKeywordOnRef", "PO3_SKSEFunctions", ReplaceKeywordOnRef);

	a_vm->RegisterFunction("ScaleObject3D", "PO3_SKSEFunctions", ScaleObject3D);

	a_vm->RegisterFunction("SetDoorDestination", "PO3_SKSEFunctions", SetDoorDestination);

	a_vm->RegisterFunction("SetEffectShaderDuration", "PO3_SKSEFunctions", SetEffectShaderDuration);

	a_vm->RegisterFunction("SetLinkedRef", "PO3_SKSEFunctions", SetLinkedRef);

	a_vm->RegisterFunction("SetMaterialType", "PO3_SKSEFunctions", SetMaterialType);

	a_vm->RegisterFunction("SetShaderType", "PO3_SKSEFunctions", SetShaderType);

	a_vm->RegisterFunction("SetupBodyPartGeometry", "PO3_SKSEFunctions", SetupBodyPartGeometry);

	a_vm->RegisterFunction("StopAllShaders", "PO3_SKSEFunctions", StopAllShaders);

	a_vm->RegisterFunction("StopArtObject", "PO3_SKSEFunctions", StopArtObject);

	a_vm->RegisterFunction("ToggleChildNode", "PO3_SKSEFunctions", ToggleChildNode);

	a_vm->RegisterFunction("UpdateHitEffectArtNode", "PO3_SKSEFunctions", UpdateHitEffectArtNode);

	return true;
}
