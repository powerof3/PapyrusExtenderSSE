#include "Papyrus/ObjectReference.h"

#include "Serialization/Form/Keywords.h"
#include "Util/VMErrors.h"


auto papyrusObjectReference::AddAllItemsToArray(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem) -> std::vector<RE::TESForm*>
{
	std::vector<RE::TESForm*> vec;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return vec;
	}

	auto inv = a_ref->GetInventory();
	for (auto& [item, data] : inv) {
		auto& [count, entry] = data;
		if (count > 0 && entry->CanItemBeTaken(a_noEquipped, a_noFavourited, a_noQuestItem)) {
			vec.push_back(item);
		}
	}

	return vec;
}


void papyrusObjectReference::AddAllItemsToList(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSListForm* a_list, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem)
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_list) {
		a_vm->TraceStack("Formlist is None", a_stackID, Severity::kWarning);
		return;
	}

	auto inv = a_ref->GetInventory();
	for (auto& [item, data] : inv) {
		auto& [count, entry] = data;
		if (count > 0 && entry->CanItemBeTaken(a_noEquipped, a_noFavourited, a_noQuestItem)) {
			a_list->AddForm(item);
		}
	}
}


void papyrusObjectReference::AddKeywordToRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSKeyword* a_add)
{
	using namespace Serialization;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_add) {
		a_vm->TraceStack("Keyword is None", a_stackID, Severity::kWarning);
		return;
	}

	if (const auto form = a_ref->GetBaseObject(); form) {
		Form::Keywords::GetSingleton()->PapyrusApply(form, a_add, Form::kAdd);
	}
}


void IterateOverAttachedCells(RE::TES* TES, const RE::NiPoint3& a_origin, float a_radius, std::function<bool(RE::TESObjectREFR& a_ref)> a_callback)
{
	auto cell = TES->interiorCell;
	if (cell) {
		cell->ForEachReferenceInRange(a_origin, a_radius, [&](RE::TESObjectREFR& a_ref) {
            return a_callback(a_ref);
		});
	} else {
		const auto gridCells = TES->gridCells;
		if (gridCells) {
			const auto gridLength = gridCells->length;
			if (gridLength > 0) {
				const float yPlus = a_origin.y + a_radius;
				const float yMinus = a_origin.y - a_radius;
				const float xPlus = a_origin.x + a_radius;
				const float xMinus = a_origin.x - a_radius;

			    for (std::uint32_t x = 0, y = 0; x < gridLength && y < gridLength; x++, y++) {
					cell = gridCells->GetCell(x, y);
					if (cell && cell->IsAttached()) {
						const auto cellCoords = cell->GetCoordinates();
						if (cellCoords && (cellCoords->worldX < xPlus && (cellCoords->worldX + 4096.0f) > xMinus && cellCoords->worldY < yPlus && (cellCoords->worldY + 4096.0f) > yMinus)) {
							cell->ForEachReferenceInRange(a_origin, a_radius, [&](RE::TESObjectREFR& a_ref) {
                                return a_callback(a_ref);
							});
						}
					}
				}
			}
		}
	}
}


auto papyrusObjectReference::FindAllReferencesOfFormType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_origin, std::uint32_t a_formType, float a_radius) -> std::vector<RE::TESObjectREFR*>
{
	std::vector<RE::TESObjectREFR*> vec;

	if (!a_origin) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return vec;
	}

	if (const auto TES = RE::TES::GetSingleton(); TES) {
		auto formType = static_cast<RE::FormType>(a_formType);
		const auto originPos = a_origin->GetPosition();
		const auto squaredRadius = a_radius * a_radius;

		IterateOverAttachedCells(TES, originPos, squaredRadius, [&](RE::TESObjectREFR& a_ref) {
			auto base = a_ref.GetBaseObject();
			if (formType == RE::FormType::None || base && base->Is(formType)) {
				vec.push_back(&a_ref);
			}
			return true;
		});

		if (vec.empty()) {
			auto worldSpace = TES->worldSpace;
			if (worldSpace) {
				const auto skyCell = worldSpace->GetOrCreateSkyCell();
				if (skyCell) {
					skyCell->ForEachReferenceInRange(originPos, squaredRadius, [&](RE::TESObjectREFR& a_ref) {
						auto base = a_ref.GetBaseObject();
					    if (formType == RE::FormType::None || base && base->Is(formType)) {
							vec.push_back(&a_ref);
						}
						return true;
					});
				}
			}
		}
	}

	return vec;
}


auto papyrusObjectReference::FindAllReferencesOfType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_origin, RE::TESForm* a_formOrList, float a_radius) -> std::vector<RE::TESObjectREFR*>
{
	std::vector<RE::TESObjectREFR*> vec;

	if (!a_origin) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return vec;
	}
	if (!a_formOrList) {
		a_vm->TraceStack("FormOrList is None", a_stackID, Severity::kWarning);
		return vec;
	}

	if (const auto TES = RE::TES::GetSingleton(); TES) {
		auto list = a_formOrList->As<RE::BGSListForm>();
		const auto originPos = a_origin->GetPosition();
		const auto squaredRadius = a_radius * a_radius;

		IterateOverAttachedCells(TES, originPos, squaredRadius, [&](RE::TESObjectREFR& a_ref) {
			if (const auto base = a_ref.GetBaseObject(); base) {
				if (list && list->HasForm(base) || a_formOrList == base) {
					vec.push_back(&a_ref);
				}
			}
			return true;
		});

		if (vec.empty()) {
			auto worldSpace = TES->worldSpace;
			if (worldSpace) {
				const auto skyCell = worldSpace->GetOrCreateSkyCell();
				if (skyCell) {
					skyCell->ForEachReferenceInRange(originPos, squaredRadius, [&](RE::TESObjectREFR& a_ref) {
						if (const auto base = a_ref.GetBaseObject(); base) {
							if (list && list->HasForm(base) || a_formOrList == base) {
								vec.push_back(&a_ref);
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


auto papyrusObjectReference::FindAllReferencesWithKeyword(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_origin, RE::TESForm* a_formOrList, float a_radius, bool a_matchAll) -> std::vector<RE::TESObjectREFR*>
{
	std::vector<RE::TESObjectREFR*> vec;

	if (!a_origin) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return vec;
	}
	if (!a_formOrList) {
		a_vm->TraceStack("FormOrList is None", a_stackID, Severity::kWarning);
		return vec;
	}

	auto keyword = a_formOrList->As<RE::BGSKeyword>();
	auto list = a_formOrList->As<RE::BGSListForm>();

	if (!keyword && !list) {
		a_vm->TraceStack("FormOrList parameter has invalid formtype", a_stackID, Severity::kWarning);
		return vec;
	}

	if (const auto TES = RE::TES::GetSingleton(); TES) {
		const auto originPos = a_origin->GetPosition();
		const auto squaredRadius = a_radius * a_radius;

		IterateOverAttachedCells(TES, originPos, squaredRadius, [&](RE::TESObjectREFR& a_ref) {
			bool success = false;
			if (list) {
				success = a_matchAll ? a_ref.HasAllKeywords(list) : a_ref.HasKeywords(list);
			} else if (keyword) {
				success = a_ref.HasKeyword(keyword);
			}
			if (success) {
				vec.push_back(&a_ref);
			}
			return true;
		});

		if (vec.empty()) {
			auto worldSpace = TES->worldSpace;
			if (worldSpace) {
				const auto skyCell = worldSpace->GetOrCreateSkyCell();
				if (skyCell) {
					skyCell->ForEachReferenceInRange(originPos, squaredRadius, [&](RE::TESObjectREFR& a_ref) {
						bool success = false;
						if (list) {
							success = a_matchAll ? a_ref.HasAllKeywords(list) : a_ref.HasKeywords(list);
						} else if (keyword) {
							success = a_ref.HasKeyword(keyword);
						}
						if (success) {
							vec.push_back(&a_ref);
						}
						return true;
					});
				}
			}
		}
	}

	return vec;
}


auto papyrusObjectReference::FindFirstItemInList(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSListForm* a_list) -> RE::TESForm*
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return nullptr;
	}
	if (!a_list) {
		a_vm->TraceStack("Formlist is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	auto inv = a_ref->GetInventory();
	for (auto& [item, data] : inv) {
		if (a_list->HasForm(item)) {
			return item;
		}
	}

	return nullptr;
}


auto papyrusObjectReference::GetActivateChildren(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref) -> std::vector<RE::TESObjectREFR*>
{
	std::vector<RE::TESObjectREFR*> vec;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return vec;
	}

	if (auto refChildren = a_ref->extraList.GetByType<RE::ExtraActivateRefChildren>(); refChildren) {
		auto& children = refChildren->children;
		if (!children.empty()) {
			for (auto& child : children) {
				if (child) {
					auto refPtr = child->activateRef.get();
					if (auto ref = refPtr.get(); ref) {
						vec.push_back(ref);
					}
				}
			}
		}
	}

	if (const auto missingIDs = a_ref->extraList.GetByType<RE::ExtraMissingRefIDs>(); missingIDs) {
		if (missingIDs->IDs) {
			stl::span<RE::ActivateParentID> span(missingIDs->IDs, missingIDs->numIDs);
			for (auto& ID : span) {
				if (auto form = RE::TESForm::LookupByID(ID.refID); form) {
					auto ref = form->AsReference();
					if (ref) {
						vec.push_back(ref);
					}
				}
			}
		}
	}

	return vec;
}


auto papyrusObjectReference::GetActorCause(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref) -> RE::Actor*
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	if (const auto actorCause = a_ref->GetActorCause(); actorCause) {
		const auto actorPtr = actorCause->actor.get();
		const auto actor = actorPtr.get();
		if (actor) {
			return actor;
		}
	}

	return nullptr;
}


auto papyrusObjectReference::GetAllArtObjects(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref) -> std::vector<RE::BGSArtObject*>
{
	std::vector<RE::BGSArtObject*> vec;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return vec;
	}

	if (auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
		auto handle = a_ref->CreateRefHandle();
		processLists->GetMagicEffects([&](RE::BSTempEffect& a_tempEffect) {
			const auto modelEffect = a_tempEffect.As<RE::ModelReferenceEffect>();
			if (modelEffect && modelEffect->target == handle && modelEffect->artObject) {
				vec.push_back(modelEffect->artObject);
			}
			return true;
		});
	}

	return vec;
}


auto papyrusObjectReference::GetAllEffectShaders(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref) -> std::vector<RE::TESEffectShader*>
{
	std::vector<RE::TESEffectShader*> vec;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return vec;
	}

	if (auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
		auto handle = a_ref->CreateRefHandle();
		processLists->GetMagicEffects([&](RE::BSTempEffect& a_tempEffect) {
			const auto shaderEffect = a_tempEffect.As<RE::ShaderReferenceEffect>();
			if (shaderEffect && shaderEffect->target == handle && shaderEffect->effectData) {
				vec.push_back(shaderEffect->effectData);
			}
			return true;
		});
	}

	return vec;
}


auto papyrusObjectReference::GetClosestActorFromRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, bool a_ignorePlayer) -> RE::Actor*
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	if (auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
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
				auto distance = originPos.GetDistance(actorPtr->GetPosition());
				map.emplace(actor, distance);
				if (distance < shortestDistance) {
					shortestDistance = distance;
				}
			}
		}

		if (!a_ignorePlayer) {
			auto player = RE::PlayerCharacter::GetSingleton();
			if (player && a_ref != player) {
				auto distance = originPos.GetDistance(player->GetPosition());
				map.emplace(player, distance);
				if (distance < shortestDistance) {
					shortestDistance = distance;
				}
			}
		}

		const auto it = std::find_if(map.begin(), map.end(), [shortestDistance](const auto& mo) {
			return SKSE::UTIL::FLOAT::approximatelyEqual(mo.second, shortestDistance);
		});
		if (it != map.end()) {
			return it->first;
		}
	}

	return nullptr;
}


auto papyrusObjectReference::GetEffectShaderDuration(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESEffectShader* a_effectShader) -> float
{
	float time = 0.0f;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return time;
	}
	if (!a_effectShader) {
		a_vm->TraceStack("Effect Shader is None", a_stackID, Severity::kWarning);
		return time;
	}

	if (auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
		auto handle = a_ref->CreateRefHandle();
		processLists->GetMagicEffects([&](RE::BSTempEffect& a_tempEffect) {
			const auto shaderEffect = a_tempEffect.As<RE::ShaderReferenceEffect>();
			if (shaderEffect && shaderEffect->target == handle && shaderEffect->effectData == a_effectShader) {
				time = shaderEffect->lifetime;
				return false;
			}
			return true;
		});
	}

	return time;
}


auto papyrusObjectReference::GetDoorDestination(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_door) -> RE::TESObjectREFR*
{
	if (!a_door) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	if (const auto teleport = a_door->extraList.GetByType<RE::ExtraTeleport>(); teleport) {
		const auto teleportData = teleport->teleportData;
		if (teleportData) {
			const auto doorPtr = teleportData->linkedDoor.get();
			return doorPtr.get();
		}
	} else {
		a_vm->TraceStack(VMError::generic_error(a_door, "is not a load door"sv).c_str(), a_stackID, Severity::kWarning);
	}

	return nullptr;
}


auto papyrusObjectReference::GetLinkedChildren(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSKeyword* a_keyword) -> std::vector<RE::TESObjectREFR*>
{
	std::vector<RE::TESObjectREFR*> vec;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return vec;
	}

	if (auto data = a_ref->extraList.GetByType<RE::ExtraLinkedRefChildren>(); data) {
		for (auto& child : data->linkedChildren) {
			if (!a_keyword || a_keyword && child.keyword == a_keyword) {
				auto refPtr = child.refr.get();
				auto ref = refPtr.get();
				if (ref) {
					vec.push_back(ref);
				}
			}
		}
	}
	if (auto missingIDs = a_ref->extraList.GetByType<RE::ExtraMissingLinkedRefIDs>(); missingIDs) {
		for (auto& entry : missingIDs->entries) {
			if (!a_keyword || a_keyword && entry.keyword == a_keyword) {
				if (auto form = RE::TESForm::LookupByID(entry.linkedRefID); form) {
					auto ref = form->AsReference();
					if (ref) {
						vec.push_back(ref);
					}
				}
			}
		}
	}

	return vec;
}


auto papyrusObjectReference::GetMagicEffectSource(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::EffectSetting* a_mgef) -> std::vector<RE::TESForm*>
{
	std::vector<RE::TESForm*> vec;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return vec;
	}
	if (!a_mgef) {
		a_vm->TraceStack("Magic Effect is None", a_stackID, Severity::kWarning);
		return vec;
	}

	auto magicTarget = a_ref->GetMagicTarget();
	if (!magicTarget) {
		return vec;
	}

	const auto activeEffects = magicTarget->GetActiveEffectList();
	if (activeEffects) {
		for (auto& activeEffect : *activeEffects) {
			if (activeEffect) {
				const auto mgef = activeEffect->GetBaseObject();
				if (mgef == a_mgef) {
					vec.push_back(activeEffect->spell);
					const auto casterPtr = activeEffect->caster.get();
					const auto caster = casterPtr.get();
					if (caster) {
						vec.push_back(caster);
					}
					break;
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

	if (auto world = a_body->GetbhkWorld()) {
		RE::BSReadLockGuard locker(world->worldLock);

		auto& materialMap = papyrusObjectReference::materialMap;

		const auto hkpBody = static_cast<RE::hkpWorldObject*>(a_body->referencedObject.get());
		if (hkpBody) {
			if (const auto hkpShape = hkpBody->GetShape(); hkpShape) {
				switch (hkpShape->type) {
				case RE::hkpShapeType::kMOPP:
					{
						const auto mopp = static_cast<const RE::hkpMoppBvTreeShape*>(hkpShape);
						if (mopp) {
							if (const auto childShape = mopp->child.childShape; childShape) {
								if (const auto bhkShape = childShape->userData; bhkShape) {
									const auto compressedShape = netimmerse_cast<RE::bhkCompressedMeshShape*>(bhkShape);
									if (compressedShape) {
										if (auto data = compressedShape->data.get(); data) {
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
						if (const auto bhkShape = hkpShape->userData; bhkShape) {
							vec.emplace_back(materialMap.at(bhkShape->materialID).data());
						}
					}
					break;
				}
			}
		}
	}
}


auto papyrusObjectReference::GetMaterialType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BSFixedString a_nodeName) -> std::vector<RE::BSFixedString>
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
		const auto object = root->GetObjectByName(a_nodeName);
		if (object) {
			const auto colObject = static_cast<RE::bhkNiCollisionObject*>(object->collisionObject.get());
			if (colObject) {
				GetMaterialType_Impl(colObject->body.get(), vec);
			}
		}
	} else {
		RE::BSVisit::TraverseScenegraphCollision(root, [&](RE::NiCollisionObject* a_col) -> RE::BSVisit::BSVisitControl {
			const auto colObject = static_cast<RE::bhkNiCollisionObject*>(a_col);
			if (colObject) {
				GetMaterialType_Impl(colObject->body.get(), vec);
			}
			return RE::BSVisit::BSVisitControl::kStop;
		});
	}

	return vec;
}


auto papyrusObjectReference::GetMotionType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref) -> std::int32_t
{
	std::int32_t motionType = -1;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return motionType;
	}

	const auto root = a_ref->Get3D();
	if (!root) {
		a_vm->TraceStack(VMError::no_3D(a_ref).c_str(), a_stackID, Severity::kWarning);
		return motionType;
	}

	RE::BSVisit::TraverseScenegraphCollision(root, [&](RE::NiCollisionObject* a_col) -> RE::BSVisit::BSVisitControl {
		const auto colObject = static_cast<RE::bhkNiCollisionObject*>(a_col);
		if (colObject) {
			if (auto body = colObject->body.get(); body) {
				if (auto world = body->GetbhkWorld(); world) {
					RE::BSReadLockGuard locker(world->worldLock);

					const auto hkpRigidBody = static_cast<RE::hkpRigidBody*>(body->referencedObject.get());
					if (hkpRigidBody) {
						motionType = hkpRigidBody->motion.type.underlying();
					}
				}
			}
		}
		return motionType != -1 ? RE::BSVisit::BSVisitControl::kStop : RE::BSVisit::BSVisitControl::kContinue;
	});

	return motionType;
}


auto papyrusObjectReference::GetRandomActorFromRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, float a_radius, bool a_ignorePlayer) -> RE::Actor*
{
	using RNG = SKSE::RNG;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	if (auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
		if (a_ignorePlayer && processLists->numberHighActors == 0) {
			return nullptr;
		}

		const auto squaredRadius = a_radius * a_radius;
		const auto originPos = a_ref->GetPosition();

		std::vector<RE::Actor*> vec;
		vec.reserve(processLists->numberHighActors);

		for (auto& actorHandle : processLists->highActorHandles) {
			auto actorPtr = actorHandle.get();
			auto actor = actorPtr.get();
			if (!actor || actor == a_ref) {
				continue;
			}
			if (originPos.GetSquaredDistance(actor->GetPosition()) > squaredRadius) {
				continue;
			}
			vec.emplace_back(actor);
		}

		if (!a_ignorePlayer) {
			if (auto player = RE::PlayerCharacter::GetSingleton(); player && a_ref != player) {
				if (originPos.GetSquaredDistance(player->GetPosition()) <= squaredRadius) {
					vec.emplace_back(player);
				}
			}
		}

		if (!vec.empty()) {
			return vec[RNG::GetSingleton()->Generate<size_t>(0, vec.size() - 1)];
		}
	}

	return nullptr;
}


auto papyrusObjectReference::GetStoredSoulSize(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref) -> std::int32_t
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return -1;
	}

	return to_underlying(a_ref->extraList.GetSoulLevel());
}


auto papyrusObjectReference::HasArtObject(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSArtObject* a_art, bool a_active) -> std::uint32_t
{
	std::uint32_t count = 0;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return count;
	}
	if (!a_art) {
		a_vm->TraceStack("Art Object is None", a_stackID, Severity::kWarning);
		return count;
	}

	if (auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
		auto handle = a_ref->CreateRefHandle();
		processLists->GetMagicEffects([&](RE::BSTempEffect& a_tempEffect) {
			const auto modelEffect = a_tempEffect.As<RE::ModelReferenceEffect>();
			if (modelEffect && modelEffect->target == handle && modelEffect->artObject == a_art) {
				if (!a_active || a_active && !modelEffect->finished) {
					count++;
				}
			}
			return true;
		});
	}

	return count;
}


auto papyrusObjectReference::HasEffectShader(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESEffectShader* a_effectShader, bool a_active) -> std::uint32_t
{
	std::uint32_t count = 0;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return count;
	}
	if (!a_effectShader) {
		a_vm->TraceStack("Effect Shader is None", a_stackID, Severity::kWarning);
		return count;
	}

	if (auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
		auto handle = a_ref->CreateRefHandle();
		processLists->GetMagicEffects([&](RE::BSTempEffect& a_tempEffect) {
			const auto shaderEffect = a_tempEffect.As<RE::ShaderReferenceEffect>();
			if (shaderEffect && shaderEffect->target == handle && shaderEffect->effectData == a_effectShader) {
				if (!a_active || a_active && !shaderEffect->finished) {
					count++;
				}
			}
			return true;
		});
	}

	return count;
}


auto papyrusObjectReference::IsLoadDoor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_door) -> bool
{
	if (!a_door) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return false;
	}

	return a_door->extraList.HasType(RE::ExtraDataType::kTeleport);
}


auto papyrusObjectReference::IsQuestItem(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref) -> bool
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return false;
	}

	return a_ref->HasQuestObject();
}


auto papyrusObjectReference::IsVIP(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref) -> bool
{
	using FILL_TYPE = RE::BGSBaseAlias::FILL_TYPE;
	using TYPE = RE::QUEST_DATA::Type;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return false;
	}

	bool isVIP = false;

	if (auto xAliases = a_ref->extraList.GetByType<RE::ExtraAliasInstanceArray>(); xAliases) {
		RE::BSReadLockGuard locker(xAliases->lock);

		for (const auto& aliasData : xAliases->aliases) {
			if (aliasData) {
				if (const auto alias = aliasData->alias; alias) {
					const auto quest = alias->owningQuest;
					if (quest && quest->GetType() != TYPE::kNone && quest->IsRunning()) {
						const auto fillType = alias->fillType.get();
						if (alias->IsQuestObject() || alias->IsEssential() || alias->IsProtected() || fillType == FILL_TYPE::kForced || fillType == FILL_TYPE::kUniqueActor) {
							isVIP = true;
							break;
						}
					}
				}
			}
		}
	}

	return isVIP;
}


auto papyrusObjectReference::HasNiExtraData(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BSFixedString a_name) -> bool
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return false;
	}

    const auto root = a_ref->Get3D();
	if (!root) {
		a_vm->TraceStack(VMError::no_3D(a_ref).c_str(), a_stackID, Severity::kWarning);
		return false;
	}

	return root->HasExtraData(a_name);
}


auto FindNearestVertex(RE::TESObjectREFR* a_ref) -> std::optional<RE::NiPoint3>
{
	const auto cell = a_ref->GetParentCell();
	if (!cell || !cell->navMeshes) {
		return std::nullopt;
	}

	auto& navMeshes = *cell->navMeshes;
	auto shortestDistance = std::numeric_limits<float>::max();
	std::optional<RE::NiPoint3> pos = std::nullopt;

	for (auto& navMesh : navMeshes.navMeshes) {
		for (auto& vertex : navMesh->vertices) {
			const auto linearDistance = a_ref->GetPosition().GetDistance(vertex.location);
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
		a_vm->TraceStack(VMError::generic_error(a_ref, "does not have a valid navmesh position"sv).c_str(), a_stackID, Severity::kWarning);
		return;
	}

	auto handle = a_ref->CreateRefHandle();
	auto task = SKSE::GetTaskInterface();
	task->AddTask([handle, nearestVertex]() {
		const auto refPtr = handle.get();
		auto ref = refPtr.get();
		if (!ref) {
			return;
		}
		ref->SetPosition(*nearestVertex);
	});
}


auto papyrusObjectReference::RemoveKeywordFromRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSKeyword* a_remove) -> bool
{
	using namespace Serialization;

	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return false;
	}
	if (!a_remove) {
		a_vm->TraceStack("Keyword is None", a_stackID, Severity::kWarning);
		return false;
	}

	const auto form = a_ref->GetBaseObject();
	return form ? Form::Keywords::GetSingleton()->PapyrusApply(form, a_remove, Form::kRemove) : false;
}


void papyrusObjectReference::ReplaceKeywordOnRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSKeyword* a_remove, RE::BGSKeyword* a_add)
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_remove) {
		a_vm->TraceStack("Remove keyword is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_add) {
		a_vm->TraceStack("Add keyword is None", a_stackID, Severity::kWarning);
		return;
	}

	auto base = a_ref->GetBaseObject();
	if (!base) {
		return;
	}

	if (const auto keywordForm = base->As<RE::BGSKeywordForm>(); keywordForm) {
		std::uint32_t removeIndex = 0;
		bool found = false;

		if (keywordForm->keywords) {
			for (std::uint32_t i = 0; i < keywordForm->numKeywords; i++) {
				const auto keyword = keywordForm->keywords[i];
				if (keyword) {
					if (keyword == a_add) {
						return;
					}
					if (keyword == a_remove) {
						removeIndex = i;
						found = true;
						break;
					}
				}
			}
		}
		if (found) {
			keywordForm->keywords[removeIndex] = a_add;
		}
	}
}


void ScaleObject3D_Impl(RE::bhkWorldObject* a_body, float a_scale)
{
	if (!a_body) {
		return;
	}

	if (auto world = a_body->GetbhkWorld(); world) {
		RE::BSWriteLockGuard locker(world->worldLock);

		const auto hkpBody = static_cast<RE::hkpWorldObject*>(a_body->referencedObject.get());
		if (hkpBody) {
			if (const auto shape = hkpBody->GetShape(); shape) {
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
	}
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
			const auto node = object->AsNode();
			if (node) {
				RE::BSVisit::TraverseScenegraphCollision(node, [&](RE::NiCollisionObject* a_col) -> RE::BSVisit::BSVisitControl {
					const auto colObject = static_cast<RE::bhkNiCollisionObject*>(a_col);
					if (colObject) {
						ScaleObject3D_Impl(colObject->body.get(), a_scale);
					}
					return RE::BSVisit::BSVisitControl::kContinue;
				});
			} else {
				const auto colObject = static_cast<RE::bhkNiCollisionObject*>(object->collisionObject.get());
				if (colObject) {
					ScaleObject3D_Impl(colObject->body.get(), a_scale);
				}
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
			const auto colObject = static_cast<RE::bhkNiCollisionObject*>(a_col);
			if (colObject) {
				ScaleObject3D_Impl(colObject->body.get(), a_scale);
			}
			return RE::BSVisit::BSVisitControl::kContinue;
		});
	}
}


void papyrusObjectReference::SetEffectShaderDuration(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESEffectShader* a_effectShader, float a_time, bool a_absolute)
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_effectShader) {
		a_vm->TraceStack("Effect Shader is None", a_stackID, Severity::kWarning);
		return;
	}

	if (auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
		auto handle = a_ref->CreateRefHandle();
		processLists->GetMagicEffects([&](RE::BSTempEffect& a_tempEffect) {
			const auto shaderEffect = a_tempEffect.As<RE::ShaderReferenceEffect>();
			if (shaderEffect && shaderEffect->target == handle && shaderEffect->effectData == a_effectShader) {
				if (!a_absolute) {
					const float value = shaderEffect->lifetime + a_time;
					if (value >= -1.0f) {
						shaderEffect->lifetime = a_time;
					}
				} else {
					shaderEffect->lifetime = a_time;
				}
				return false;
			}
			return true;
		});
	}
}


auto papyrusObjectReference::SetDoorDestination(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_door, RE::TESObjectREFR* a_newDoor) -> bool
{
	if (!a_door) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return false;
	}
	if (!a_newDoor) {
		a_vm->TraceStack("Destination Door is None", a_stackID, Severity::kWarning);
		return false;
	}

	if (const auto teleport = a_door->extraList.GetByType<RE::ExtraTeleport>(); teleport) {
		auto teleportData = teleport->teleportData;
		if (teleportData) {
			teleportData->linkedDoor = a_newDoor->CreateRefHandle();
			return true;
		}
		a_vm->TraceStack(VMError::generic_error(a_door, "- couldn't get teleport data"sv).c_str(), a_stackID, Severity::kWarning);
		return false;
	}
	a_vm->TraceStack(VMError::generic_error(a_door, "is not a load door"sv).c_str(), a_stackID, Severity::kWarning);
	return false;
}


void papyrusObjectReference::SetLinkedRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESObjectREFR* a_targetRef, RE::BGSKeyword* a_keyword)
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return;
	}

	a_ref->extraList.SetLinkedRef(a_targetRef, a_keyword);
}


void SetMaterialType_Impl(RE::bhkWorldObject* a_body, RE::MATERIAL_ID newID, RE::MATERIAL_ID oldID)
{
	if (!a_body) {
		return;
	}

	if (auto world = a_body->GetbhkWorld(); world) {
		RE::BSWriteLockGuard locker(world->worldLock);

		const auto hkpBody = static_cast<RE::hkpWorldObject*>(a_body->referencedObject.get());
		if (hkpBody) {
			if (const auto hkpShape = hkpBody->GetShape(); hkpShape) {
				switch (hkpShape->type) {
				case RE::hkpShapeType::kMOPP:
					{
						const auto mopp = static_cast<const RE::hkpMoppBvTreeShape*>(hkpShape);
						if (mopp) {
							if (const auto childShape = mopp->child.childShape; childShape) {
								if (const auto bhkShape = childShape->userData; bhkShape) {
									const auto compressedShape = netimmerse_cast<RE::bhkCompressedMeshShape*>(bhkShape);
									if (compressedShape) {
										if (auto data = compressedShape->data.get(); data) {
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
	}
}


void papyrusObjectReference::SetMaterialType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BSFixedString a_newMaterialType, RE::BSFixedString a_oldMaterialType, RE::BSFixedString a_nodeName)
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return;
	}
	if (a_newMaterialType.empty()) {
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
	for (const auto& [ID, matString] : materialMap) {
		if (_stricmp(matString.data(), a_newMaterialType.data()) == 0) {
			newID = ID;
			break;
		}
		if (!a_oldMaterialType.empty()) {
			if (_stricmp(matString.data(), a_oldMaterialType.data()) == 0) {
				oldID = ID;
				break;
			}
		}
	}

	if (newID != RE::MATERIAL_ID::kNone) {
		if (!a_nodeName.empty()) {
			const auto object = root->GetObjectByName(a_nodeName);
			if (object) {
				const auto colObject = static_cast<RE::bhkNiCollisionObject*>(object->collisionObject.get());
				if (colObject) {
					SetMaterialType_Impl(colObject->body.get(), newID, oldID);
				}
			}
		} else {
			RE::BSVisit::TraverseScenegraphCollision(root, [&](RE::NiCollisionObject* a_col) -> RE::BSVisit::BSVisitControl {
				const auto colObject = static_cast<RE::bhkNiCollisionObject*>(a_col);
				if (colObject) {
					SetMaterialType_Impl(colObject->body.get(), newID, oldID);
				}
				return RE::BSVisit::BSVisitControl::kContinue;
			});
		}
	}
}


void SetShaderType_Impl(RE::NiAVObject* a_object, RE::BSGeometry* a_template, std::string_view a_path, std::int32_t a_textureType, std::vector<RE::BSFixedString>& a_vec, bool a_noWeapons, bool a_noAlpha, bool a_isActor)
{
	RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
		using State = RE::BSGeometry::States;
		using Feature = RE::BSShaderMaterial::Feature;
		using Texture = RE::BSTextureSet::Texture;
		using Flags = RE::BSShaderProperty::EShaderPropertyFlag8;
		using VertexFlags = RE::NiSkinPartition::Vertex::Flags;

		const bool hasNormals = a_geometry->HasVertexFlag(VertexFlags::kNormal);
		if (!hasNormals) {
			return RE::BSVisit::BSVisitControl::kContinue;
		}
		const auto alpha = a_geometry->properties[State::kProperty].get();
		if (alpha && a_noAlpha) {
			return RE::BSVisit::BSVisitControl::kContinue;
		}
		auto parent = a_geometry->parent;
		if (parent && a_isActor && a_noWeapons && parent->AsFadeNode()) {
			return RE::BSVisit::BSVisitControl::kContinue;
		}

		auto effect = a_geometry->properties[State::kEffect];
		auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect.get());

		auto tempEffect = a_template->properties[State::kEffect];
		auto tempLightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(tempEffect.get());

		if (lightingShader && tempLightingShader) {
			auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
			auto tempMaterial = static_cast<RE::BSLightingShaderMaterialBase*>(tempLightingShader->material);

			if (material && tempMaterial) {
				const auto textureSet = material->textureSet.get();
				const auto tempTextureSet = tempMaterial->textureSet.get();

				if (textureSet && tempTextureSet) {
					std::string sourceDiffuse(textureSet->GetTexturePath(Texture::kDiffuse));
					RE::Util::SanitizeTexturePath(sourceDiffuse);
					if (!a_path.empty() && sourceDiffuse.find(a_path) == std::string::npos) {
						return RE::BSVisit::BSVisitControl::kContinue;
					}

					const auto oldFeature = material->GetFeature();
					const auto oldFlags = lightingShader->flags.get();
					const auto oldEmissiveColor = lightingShader->emissiveColor ? RE::NiColor::ColorToString(*lightingShader->emissiveColor) : "000000";
					const auto oldEmissiveMult = lightingShader->emissiveMult;

					const auto newFeature = tempMaterial->GetFeature();

					if (oldFeature != newFeature) {

						if (auto data = lightingShader->GetExtraData<RE::NiStringsExtraData>("PO3_ORIGINALSHADER"sv); !data) {
							std::vector<RE::BSFixedString> resetData;
							resetData.reserve(12);
							for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
								resetData.emplace_back(textureSet->GetTexturePath(i)); //0-8
							}
							resetData.emplace_back(std::to_string(to_underlying(oldFeature))); //9
							resetData.emplace_back(std::to_string(to_underlying(oldFlags))); //10
							resetData.emplace_back(oldEmissiveColor); //11
							resetData.emplace_back(std::to_string(oldEmissiveMult)); //12

							auto newData = RE::NiStringsExtraData::Create("PO3_ORIGINALSHADER"sv, resetData);
							if (newData && lightingShader->AddExtraData(newData)) {
							    a_vec.push_back(a_geometry->name);
							}
						}

					    if (auto newMaterial = static_cast<RE::BSLightingShaderMaterialBase*>(tempMaterial->Create()); newMaterial) {
							
					        newMaterial->CopyMembers(tempMaterial);
							newMaterial->ClearTextures();

							if (a_textureType != -1) {
								if (auto newTextureSet = RE::BSShaderTextureSet::Create(); newTextureSet) {
									const auto BSTextureType = static_cast<Texture>(a_textureType);
									for (auto i = Texture::kDiffuse; i < Texture::kTotal; ++i) {
										if (i != BSTextureType) {
											newTextureSet->SetTexturePath(i, tempTextureSet->GetTexturePath(i));
										}
									}
									newTextureSet->SetTexturePath(BSTextureType, textureSet->GetTexturePath(BSTextureType));
									newMaterial->OnLoadTextureSet(0, newTextureSet);
								}
							}

							lightingShader->CopyMembers(tempLightingShader);
							lightingShader->SetFlags(Flags::kSkinned, a_geometry->HasVertexFlag(VertexFlags::kSkinned));

							lightingShader->SetMaterial(newMaterial, true);
					        lightingShader->InitializeShader(a_geometry);

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
	}
	if (!a_template) {
		a_vm->TraceStack("Template is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_ref->Is3DLoaded()) {
		a_vm->TraceStack(VMError::no_3D(a_ref).c_str(), a_stackID, Severity::kWarning);
		return;
	}
	if (!a_template->Is3DLoaded()) {
		a_vm->TraceStack(VMError::generic_error(a_template, "template object has no 3D"sv).c_str(), a_stackID, Severity::kWarning);
		return;
	}

	using Texture = RE::BSTextureSet::Texture;
	using Feature = RE::BSShaderMaterial::Feature;

	auto sourcePath = std::string();
	if (!a_filter.empty()) {
		sourcePath = a_filter.c_str();
		RE::Util::SanitizeTexturePath(sourcePath);
	}

	auto isActor = a_ref->Is(RE::FormType::ActorCharacter);
	auto feature = static_cast<Feature>(a_shaderType);
	auto root = a_ref->Get3D()->AsNode();
	auto template_root = a_template->Get3D()->AsNode();

	if (root && template_root) {
		auto task = SKSE::GetTaskInterface();
		task->AddTask([root, template_root, feature, sourcePath, a_textureType, a_noWeapons, a_noAlpha, isActor]() {
			const auto template_geo = template_root->GetFirstGeometryOfShaderType(feature);
			if (template_geo) {
				std::vector<RE::BSFixedString> vec;
				SetShaderType_Impl(root, template_geo, sourcePath, a_textureType, vec, a_noWeapons, a_noAlpha, isActor);

				if (!vec.empty()) {
				    auto name = std::string("PO3_SHADER | "sv);
					name.append(std::to_string(to_underlying(feature)));

					if (auto data = root->GetExtraData<RE::NiStringsExtraData>(name); !data) {
						auto newData = RE::NiStringsExtraData::Create(name, vec);
						if (newData) {
							root->AddExtraData(newData);
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
	}
	if (!a_actor) {
		a_vm->TraceStack("Actor is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_bodyparts->Get3D()) {
		a_vm->TraceStack(VMError::no_3D(a_bodyparts).c_str(), a_stackID, Severity::kWarning);
		return;
	}

	const auto actorbase = a_actor->GetActorBase();
	auto root = a_bodyparts->Get3D()->AsFadeNode();

	if (actorbase && root) {
		RE::NiIntegerExtraData* data = nullptr;
		if (const auto actorRoot = a_actor->Get3D(false); actorRoot) {
			data = actorRoot->GetExtraData<RE::NiIntegerExtraData>("PO3_SKINTINT"sv);
		}

		RE::NiColor color = data ? data->value : actorbase->bodyTintColor;
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
		processLists->StopAllShaders(*a_ref);
	}
}


void papyrusObjectReference::StopArtObject(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSArtObject* a_art)
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_art) {
		a_vm->TraceStack("Art Object is None", a_stackID, Severity::kWarning);
		return;
	}

	if (auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
		auto handle = a_ref->CreateRefHandle();
		processLists->GetMagicEffects([&](RE::BSTempEffect& a_tempEffect) {
			const auto modelEffect = a_tempEffect.As<RE::ModelReferenceEffect>();
			if (modelEffect && modelEffect->target == handle && modelEffect->artObject == a_art) {
				modelEffect->finished = true;
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

	auto task = SKSE::GetTaskInterface();
	task->AddTask([root, a_nodeName, a_disable]() {
		auto object = root->GetObjectByName(a_nodeName);
		if (object) {
			object->ToggleNode(a_disable);

			if (auto data = root->GetExtraData<RE::NiStringsExtraData>("PO3_TOGGLE"sv); data) {
				a_disable ? data->Insert(a_nodeName) : data->Remove(a_nodeName);
			} else if (a_disable) {
				std::vector<RE::BSFixedString> vec;
				vec.push_back(a_nodeName);
				if (const auto newData = RE::NiStringsExtraData::Create("PO3_TOGGLE"sv, vec); newData) {
					root->AddExtraData(newData);
				}
			}
		}
	});
}


void papyrusObjectReference::UpdateHitEffectArtNode(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSArtObject* a_art, RE::BSFixedString a_toNode, std::vector<float> a_translate, std::vector<float> a_rotate, float a_scale)
{
	if (!a_ref) {
		a_vm->TraceStack("Object Reference is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_art) {
		a_vm->TraceStack("Art is None", a_stackID, Severity::kWarning);
		return;
	}
	if (a_toNode.empty()) {
		a_vm->TraceStack("Node name is empty", a_stackID, Severity::kWarning);
		return;
	}

	if (auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
		auto handle = a_ref->CreateRefHandle();
		processLists->GetMagicEffects([&](RE::BSTempEffect& a_tempEffect) {
			auto modelEffect = a_tempEffect.As<RE::ModelReferenceEffect>();
			if (modelEffect && modelEffect->target == handle && modelEffect->artObject == a_art) {
				if (modelEffect->hitEffectArtData.nodeName == a_toNode) {
					return false;
				}
				auto current3DRoot = modelEffect->hitEffectArtData.current3DRoot.get();
				auto art = modelEffect->hitEffectArtData.attachedArt.get();
				if (current3DRoot && art) {
					auto newObject = current3DRoot->GetObjectByName(a_toNode);
					if (!newObject) {
						const std::string error = "Node '" + std::string(a_toNode.c_str()) + "' doesn't exist on " + VMError::to_id(a_ref);
						a_vm->TraceStack(error.c_str(), a_stackID, Severity::kWarning);
						return false;
					}
					auto task = SKSE::GetTaskInterface();
					task->AddTask([a_toNode, a_translate, a_rotate, a_scale, art, newObject, modelEffect]() {
						if (auto newNode = newObject->AsNode(); newNode) {
							auto attachTData = art->GetExtraData<RE::NiStringsExtraData>("AttachT"sv);
							if (attachTData) {
								constexpr auto namedNode = "NamedNode&"sv;
								const auto oldNodeStr = namedNode.data() + std::string(modelEffect->hitEffectArtData.nodeName);
								const auto newNodeStr = namedNode.data() + std::string(a_toNode);
								attachTData->Remove(oldNodeStr);
								attachTData->Insert(newNodeStr);
							}
							for (auto& nodesPtr : art->children) {
								if (auto nodes = nodesPtr.get(); nodes) {
									if (!a_translate.empty() && a_translate.size() == 3) {
										nodes->local.translate = { a_translate[0], a_translate[1], a_translate[2] };
									}
									if (!a_rotate.empty() && a_rotate.size() == 3) {
										const auto aX = RE::degToRad(a_rotate[0]);
										const auto aY = RE::degToRad(a_rotate[1]);
										const auto aZ = RE::degToRad(a_rotate[2]);
										nodes->local.rotate.SetEulerAnglesXYZ(aX, aY, aZ);
									}
									nodes->local.scale *= a_scale;
								}
							}
							newNode->AttachChild(art);
							RE::NiUpdateData data = { 0.0f, RE::NiUpdateData::Flag::kDirty };
							art->UpdateWorldData(&data);
						}
						modelEffect->hitEffectArtData.nodeName = a_toNode;
						modelEffect->UpdatePosition();
					});
				}
				return false;
			}
			return true;
		});
	}
}


auto papyrusObjectReference::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusObjectReference - couldn't get VMState"sv);
		return false;
	}

	auto constexpr Functions = "PO3_SKSEFunctions"sv;

	a_vm->RegisterFunction("AddAllItemsToArray"sv, Functions, AddAllItemsToArray);

	a_vm->RegisterFunction("AddAllItemsToList"sv, Functions, AddAllItemsToList);

	a_vm->RegisterFunction("AddKeywordToRef"sv, Functions, AddKeywordToRef);

	a_vm->RegisterFunction("FindAllReferencesOfFormType"sv, Functions, FindAllReferencesOfFormType);

	a_vm->RegisterFunction("FindAllReferencesOfType"sv, Functions, FindAllReferencesOfType);

	a_vm->RegisterFunction("FindAllReferencesWithKeyword"sv, Functions, FindAllReferencesWithKeyword);

	a_vm->RegisterFunction("FindFirstItemInList"sv, Functions, FindFirstItemInList);

	a_vm->RegisterFunction("GetActivateChildren"sv, Functions, GetActivateChildren);

	a_vm->RegisterFunction("GetActorCause"sv, Functions, GetActorCause);

	a_vm->RegisterFunction("GetAllArtObjects"sv, Functions, GetAllArtObjects);

	a_vm->RegisterFunction("GetAllEffectShaders"sv, Functions, GetAllEffectShaders);

	a_vm->RegisterFunction("GetClosestActorFromRef"sv, Functions, GetClosestActorFromRef);

	a_vm->RegisterFunction("GetDoorDestination"sv, Functions, GetDoorDestination);

	a_vm->RegisterFunction("GetEffectShaderDuration"sv, Functions, GetEffectShaderDuration);

	a_vm->RegisterFunction("GetLinkedChildren"sv, Functions, GetLinkedChildren);

	a_vm->RegisterFunction("GetMagicEffectSource"sv, Functions, GetMagicEffectSource);

	a_vm->RegisterFunction("GetMaterialType"sv, Functions, GetMaterialType);

	a_vm->RegisterFunction("GetMotionType"sv, Functions, GetMotionType);

	a_vm->RegisterFunction("GetRandomActorFromRef"sv, Functions, GetRandomActorFromRef);

	a_vm->RegisterFunction("GetStoredSoulSize"sv, Functions, GetStoredSoulSize);

	a_vm->RegisterFunction("HasArtObject"sv, Functions, HasArtObject);

	a_vm->RegisterFunction("HasEffectShader"sv, Functions, HasEffectShader);

	a_vm->RegisterFunction("HasNiExtraData"sv, Functions, HasNiExtraData);

	a_vm->RegisterFunction("IsLoadDoor"sv, Functions, IsLoadDoor, true);

	a_vm->RegisterFunction("IsQuestItem"sv, Functions, IsQuestItem);

	a_vm->RegisterFunction("IsVIP"sv, Functions, IsVIP);

	a_vm->RegisterFunction("MoveToNearestNavmeshLocation"sv, Functions, MoveToNearestNavmeshLocation);

	a_vm->RegisterFunction("RemoveKeywordFromRef"sv, Functions, RemoveKeywordFromRef);

	a_vm->RegisterFunction("ReplaceKeywordOnRef"sv, Functions, ReplaceKeywordOnRef);

	a_vm->RegisterFunction("ScaleObject3D"sv, Functions, ScaleObject3D);

	a_vm->RegisterFunction("SetDoorDestination"sv, Functions, SetDoorDestination);

	a_vm->RegisterFunction("SetEffectShaderDuration"sv, Functions, SetEffectShaderDuration);

	a_vm->RegisterFunction("SetLinkedRef"sv, Functions, SetLinkedRef);

	a_vm->RegisterFunction("SetMaterialType"sv, Functions, SetMaterialType);

	a_vm->RegisterFunction("SetShaderType"sv, Functions, SetShaderType);

	a_vm->RegisterFunction("SetupBodyPartGeometry"sv, Functions, SetupBodyPartGeometry);

	a_vm->RegisterFunction("StopAllShaders"sv, Functions, StopAllShaders);

	a_vm->RegisterFunction("StopArtObject"sv, Functions, StopArtObject);

	a_vm->RegisterFunction("ToggleChildNode"sv, Functions, ToggleChildNode);

	a_vm->RegisterFunction("UpdateHitEffectArtNode"sv, Functions, UpdateHitEffectArtNode);

	return true;
}
