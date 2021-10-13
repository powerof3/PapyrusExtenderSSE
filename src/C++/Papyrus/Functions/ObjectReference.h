#pragma once

#include "Serialization/Services.h"

namespace Papyrus::ObjectReference
{
	namespace inv_util
	{
		inline bool can_be_taken(const std::unique_ptr<RE::InventoryEntryData>& a_entry, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem)
		{
			if (a_noEquipped && a_entry->IsWorn()) {
				return false;
			}
			if (a_noFavourited && a_entry->IsFavorited()) {
				return false;
			}
			if (a_noQuestItem && a_entry->IsQuestObject()) {
				return false;
			}
			return true;
		}
	}

	inline std::vector<RE::TESForm*> AddAllItemsToArray(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		bool a_noEquipped,
		bool a_noFavourited,
		bool a_noQuestItem)
	{
		std::vector<RE::TESForm*> result;

		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return result;
		}

		auto inv = a_ref->GetInventory();
		for (const auto& [item, data] : inv) {
			if (item->Is(RE::FormType::LeveledItem)) {
				continue;
			}
			const auto& [count, entry] = data;
			if (count > 0 && inv_util::can_be_taken(entry, a_noEquipped, a_noFavourited, a_noQuestItem)) {
				result.push_back(item);
			}
		}

		return result;
	}

	inline void AddAllItemsToList(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		RE::BGSListForm* a_list,
		bool a_noEquipped,
		bool a_noFavourited,
		bool a_noQuestItem)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}
		if (!a_list) {
			a_vm->TraceStack("Formlist is None", a_stackID);
			return;
		}

		auto inv = a_ref->GetInventory();
		for (const auto& [item, data] : inv) {
			if (item->Is(RE::FormType::LeveledItem)) {
				continue;
			}
			const auto& [count, entry] = data;
			if (count > 0 && inv_util::can_be_taken(entry, a_noEquipped, a_noFavourited, a_noQuestItem)) {
				a_list->AddForm(item);
			}
		}
	}

	inline std::vector<RE::TESForm*> AddItemsOfTypeToArray(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		std::uint32_t a_formType,
		bool a_noEquipped,
		bool a_noFavourited,
		bool a_noQuestItem)
	{
		std::vector<RE::TESForm*> result;

		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return result;
		}

		const auto formType = static_cast<RE::FormType>(a_formType);

		auto inv = a_ref->GetInventory();
		for (const auto& [item, data] : inv) {
			if (item->Is(RE::FormType::LeveledItem)) {
				continue;
			}
			const auto& [count, entry] = data;
			if (count > 0 && (formType == RE::FormType::None || item->Is(formType)) && inv_util::can_be_taken(entry, a_noEquipped, a_noFavourited, a_noQuestItem)) {
				result.push_back(item);
			}
		}

		return result;
	}

	inline void AddItemsOfTypeToList(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		RE::BGSListForm* a_list,
		std::uint32_t a_formType,
		bool a_noEquipped,
		bool a_noFavourited,
		bool a_noQuestItem)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}
		if (!a_list) {
			a_vm->TraceStack("Formlist is None", a_stackID);
			return;
		}

		const auto formType = static_cast<RE::FormType>(a_formType);

		auto inv = a_ref->GetInventory();
		for (const auto& [item, data] : inv) {
			if (item->Is(RE::FormType::LeveledItem)) {
				continue;
			}
			const auto& [count, entry] = data;
			if (count > 0 && (formType == RE::FormType::None || item->Is(formType)) && inv_util::can_be_taken(entry, a_noEquipped, a_noFavourited, a_noQuestItem)) {
				a_list->AddForm(item);
			}
		}
	}

	inline void AddKeywordToRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		RE::BGSKeyword* a_keyword)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}
		if (!a_keyword) {
			a_vm->TraceStack("Keyword is None", a_stackID);
			return;
		}

		if (const auto form = a_ref->GetObjectReference(); form) {
			KeywordManager::GetSingleton()->Add(form, a_keyword);
		}
	}

	inline void ApplyMaterialShader(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		RE::BGSMaterialObject* a_shader,
		float a_materialThresholdAngle)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}

		if (!a_shader) {
			a_vm->TraceStack("Material Object is None", a_stackID);
			return;
		}

		const auto root = a_ref->Get3D();
		if (!root) {
			a_vm->TraceForm(a_ref, "has no 3D", a_stackID);
			return;
		}

		const auto projectedUVParams = RE::NiColorA(
			a_shader->directionalData.falloffScale,
			a_shader->directionalData.falloffBias,
			1.0f / a_shader->directionalData.noiseUVScale,
			std::cosf(a_materialThresholdAngle * 0.017453292f));

		root->UpdateMaterialShader(
			projectedUVParams,
			a_shader->directionalData.singlePassColor,
			a_shader->directionalData.flags.any(RE::BSMaterialObject::DIRECTIONAL_DATA::Flag::kSnow));
	}

	//TO_DO
	/*inline bool AttachModel(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_path,
		RE::BSFixedString a_nodeName,
		std::vector<float> a_translate,
		std::vector<float> a_rotate,
		float a_scale)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return false;
		}

		const auto root = a_ref->Get3D();
		if (!root) {
			a_vm->TraceForm(a_ref, "has no 3D", a_stackID, Severity::kInfo);
			return false;
		}
	}*/

	inline std::vector<RE::TESObjectREFR*> FindAllReferencesOfFormType(RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_origin,
		std::uint32_t a_formType,
		float a_radius)
	{
		std::vector<RE::TESObjectREFR*> result;

		if (const auto TES = RE::TES::GetSingleton(); TES) {
			const auto formType = static_cast<RE::FormType>(a_formType);

			TES->ForEachReferenceInRange(a_origin, a_radius, [&](RE::TESObjectREFR& a_ref) {
				const auto base = a_ref.GetBaseObject();
				if (formType == RE::FormType::None || a_ref.Is(formType) || base && base->Is(formType)) {
					result.push_back(&a_ref);
				}
				return true;
			});
		}

		return result;
	}

	inline std::vector<RE::TESObjectREFR*> FindAllReferencesOfType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		[[maybe_unused]] RE::TESObjectREFR* a_ref,
		const RE::TESForm* a_formOrList,
		float a_radius)
	{
		std::vector<RE::TESObjectREFR*> result;

		if (!a_formOrList) {
			a_vm->TraceStack("FormOrList is None", a_stackID);
			return result;
		}

		if (const auto TES = RE::TES::GetSingleton(); TES) {
			const auto list = a_formOrList->As<RE::BGSListForm>();

			TES->ForEachReferenceInRange(a_ref, a_radius, [&](RE::TESObjectREFR& b_ref) {
				if (const auto base = b_ref.GetBaseObject(); base) {
					if (list && list->HasForm(base) || a_formOrList == base) {
						result.push_back(&b_ref);
					}
				}
				return true;
			});
		}

		return result;
	}

	inline std::vector<RE::TESObjectREFR*> FindAllReferencesWithKeyword(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		RE::TESForm* a_formOrList,
		float a_radius,
		bool a_matchAll)
	{
		std::vector<RE::TESObjectREFR*> result;

		if (!a_formOrList) {
			a_vm->TraceStack("FormOrList is None", a_stackID);
			return result;
		}

		if (const auto TES = RE::TES::GetSingleton(); TES) {
			const auto keyword = a_formOrList->As<RE::BGSKeyword>();
			const auto list = a_formOrList->As<RE::BGSListForm>();

			if (!keyword && !list) {
				a_vm->TraceStack("FormOrList parameter has invalid formtype", a_stackID);
				return result;
			}

			TES->ForEachReferenceInRange(a_ref, a_radius, [&](RE::TESObjectREFR& b_ref) {
				bool success = false;
				if (list) {
					success = b_ref.HasKeywords(list, a_matchAll);
				} else if (keyword) {
					success = b_ref.HasKeyword(keyword);
				}
				if (success) {
					result.push_back(&b_ref);
				}
				return true;
			});
		}

		return result;
	}

	inline RE::TESForm* FindFirstItemInList(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		const RE::BGSListForm* a_list)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return nullptr;
		}
		if (!a_list) {
			a_vm->TraceStack("Formlist is None", a_stackID);
			return nullptr;
		}

		auto inv = a_ref->GetInventory();
		for (const auto& [item, data] : inv) {
			if (a_list->HasForm(item)) {
				return item;
			}
		}

		return nullptr;
	}

	inline std::vector<RE::TESObjectREFR*> GetActivateChildren(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{
		std::vector<RE::TESObjectREFR*> result;

		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return result;
		}

		const auto refChildren = a_ref->extraList.GetByType<RE::ExtraActivateRefChildren>();
		if (refChildren) {
			auto& children = refChildren->children;
			for (const auto& child : children) {
				if (child) {
					const auto ref = child->activateRef.get();
					if (ref) {
						result.push_back(ref.get());
					}
				}
			}
		}

		const auto missingIDs = a_ref->extraList.GetByType<RE::ExtraMissingRefIDs>();
		if (missingIDs && missingIDs->IDs && missingIDs->numIDs > 0) {
			std::span<RE::ActivateParentID> span(missingIDs->IDs, missingIDs->numIDs);
			for (const auto& ID : span) {
				const auto form = RE::TESForm::LookupByID(ID.refID);
				const auto ref = form ? form->AsReference() : nullptr;
				if (ref) {
					result.push_back(ref);
				}
			}
		}

		return result;
	}

	inline RE::BSFixedString GetActiveGamebryoAnimation(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return RE::BSFixedString();
		}

		const auto root = a_ref->Get3D();
		if (!root) {
			a_vm->TraceForm(a_ref, "has no 3D", a_stackID);
			return RE::BSFixedString();
		}

		const auto controller = root->GetControllers();
		const auto manager = controller ? controller->AsNiControllerManager() : nullptr;

		if (manager) {
			for (const auto& sequence : manager->sequenceArray) {
				if (sequence && sequence->Animating()) {
					return sequence->name;
				}
			}
		}

		return RE::BSFixedString();
	}

	inline RE::Actor* GetActorCause(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return nullptr;
		}

		if (const auto actorCause = a_ref->GetActorCause(); actorCause) {
			const auto actorPtr = actorCause->actor.get();
			return actorPtr.get();
		}

		return nullptr;
	}

	inline std::vector<RE::BGSArtObject*> GetAllArtObjects(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{
		std::vector<RE::BGSArtObject*> result;

		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return result;
		}

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			const auto handle = a_ref->CreateRefHandle();
			processLists->GetModelEffects([&](const RE::ModelReferenceEffect& a_modelEffect) {
				if (a_modelEffect.target == handle) {
					if (const auto art = a_modelEffect.artObject; art) {
						result.push_back(art);
					}
				}
				return true;
			});
		}

		return result;
	}

	inline std::vector<RE::TESEffectShader*> GetAllEffectShaders(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{
		std::vector<RE::TESEffectShader*> result;

		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return result;
		}

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			const auto handle = a_ref->CreateRefHandle();
			processLists->GetShaderEffects([&](const RE::ShaderReferenceEffect& a_shaderEffect) {
				if (a_shaderEffect.target == handle) {
					if (const auto shader = a_shaderEffect.effectData; shader) {
						result.push_back(shader);
					}
				}
				return true;
			});
		}

		return result;
	}

	inline RE::Actor* GetClosestActorFromRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::TESObjectREFR* a_ref,
		bool a_ignorePlayer)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return nullptr;
		}

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			if (a_ignorePlayer && processLists->numberHighActors == 0) {
				return nullptr;
			}

			const auto originPos = a_ref->GetPosition();
			auto shortestDistance = std::numeric_limits<float>::max();

			std::map<RE::Actor*, float> map;

			const auto get_shortest_distance = [&](RE::Actor* a_actor) {
				auto distance = originPos.GetDistance(a_actor->GetPosition());
				map.emplace(a_actor, distance);
				if (distance < shortestDistance) {
					shortestDistance = distance;
				}
			};

			for (const auto& actorHandle : processLists->highActorHandles) {
				const auto actorPtr = actorHandle.get();
				const auto actor = actorPtr.get();
				if (actor && actor != a_ref) {
					get_shortest_distance(actor);
				}
			}

			if (!a_ignorePlayer) {
				if (const auto player = RE::PlayerCharacter::GetSingleton(); player && a_ref != player) {
					get_shortest_distance(player);
				}
			}

			const auto it = std::ranges::find_if(map, [shortestDistance](const auto& mo) {
				return numeric::approximately_equal(mo.second, shortestDistance);
			});
			if (it != map.end()) {
				return it->first;
			}
		}

		return nullptr;
	}

	inline float GetEffectShaderDuration(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		const RE::TESEffectShader* a_effectShader)
	{
		float time = 0.0f;

		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return time;
		}
		if (!a_effectShader) {
			a_vm->TraceStack("Effect Shader is None", a_stackID);
			return time;
		}

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			const auto handle = a_ref->CreateRefHandle();
			processLists->GetShaderEffects([&](const RE::ShaderReferenceEffect& a_shaderEffect) {
				if (a_shaderEffect.target == handle && a_shaderEffect.effectData == a_effectShader) {
					time = a_shaderEffect.lifetime;
					return false;
				}
				return true;
			});
		}

		return time;
	}

	inline RE::TESObjectREFR* GetDoorDestination(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return nullptr;
		}

		const auto teleport = a_ref->extraList.GetByType<RE::ExtraTeleport>();
		const auto teleportData = teleport ? teleport->teleportData : nullptr;

		if (teleportData) {
			const auto doorPtr = teleportData->linkedDoor.get();
			return doorPtr.get();
		}

		return nullptr;
	}

	inline std::vector<RE::TESObjectREFR*> GetLinkedChildren(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		const RE::BGSKeyword* a_keyword)
	{
		std::vector<RE::TESObjectREFR*> result;

		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return result;
		}

		if (const auto data = a_ref->extraList.GetByType<RE::ExtraLinkedRefChildren>(); data) {
			for (const auto& child : data->linkedChildren) {
				if (!a_keyword || child.keyword == a_keyword) {
					const auto ref = child.refr.get();
					if (ref) {
						result.push_back(ref.get());
					}
				}
			}
		}
		if (const auto missingIDs = a_ref->extraList.GetByType<RE::ExtraMissingLinkedRefIDs>(); missingIDs) {
			for (const auto& entry : missingIDs->entries) {
				if (!a_keyword || entry.keyword == a_keyword) {
					const auto form = RE::TESForm::LookupByID(entry.linkedRefID);
					const auto ref = form ? form->AsReference() : nullptr;
					if (ref) {
						result.push_back(ref);
					}
				}
			}
		}

		return result;
	}

	inline std::vector<RE::TESForm*> GetMagicEffectSource(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		RE::EffectSetting* a_mgef)
	{
		std::vector<RE::TESForm*> result;

		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return result;
		}
		if (!a_mgef) {
			a_vm->TraceStack("Magic Effect is None", a_stackID);
			return result;
		}

		const auto magicTarget = a_ref->GetMagicTarget();
		const auto activeEffects = magicTarget ? magicTarget->GetActiveEffectList() : nullptr;

		if (activeEffects) {
			for (const auto& activeEffect : *activeEffects) {
				const auto mgef = activeEffect ? activeEffect->GetBaseObject() : nullptr;
				if (mgef && mgef == a_mgef) {
					result.push_back(activeEffect->spell);
					const auto caster = activeEffect->caster.get();
					result.push_back(caster.get());
				}
			}
		}

		return result;
	}

	inline std::vector<RE::BSFixedString> GetMaterialType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName)
	{
		std::vector<RE::BSFixedString> result;

		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return result;
		}

		const auto root = a_ref->Get3D();
		if (!root) {
			a_vm->TraceForm(a_ref, "has no 3D", a_stackID);
			return result;
		}

		const auto get_material_type = [&](RE::bhkWorldObject* a_body) {
			if (!a_body) {
				return;
			}

			const auto hkpBody = static_cast<RE::hkpWorldObject*>(a_body->referencedObject.get());
			const auto hkpShape = hkpBody ? hkpBody->GetShape() : nullptr;

			if (hkpShape) {
				if (hkpShape->type == RE::hkpShapeType::kMOPP) {
					const auto mopp = static_cast<const RE::hkpMoppBvTreeShape*>(hkpShape);
					const auto childShape = mopp ? mopp->child.childShape : nullptr;
					const auto compressedShape = childShape ? netimmerse_cast<RE::bhkCompressedMeshShape*>(childShape->userData) : nullptr;
					const auto shapeData = compressedShape ? compressedShape->data.get() : nullptr;

					if (shapeData) {
						for (const auto& meshMaterial : shapeData->meshMaterials) {
							result.emplace_back(MATERIAL::get_material(meshMaterial.materialID));
						}
					}
				} else if (const auto bhkShape = hkpShape->userData; bhkShape) {
					result.emplace_back(MATERIAL::get_material(bhkShape->materialID).data());
				}
			}
		};

		const auto cell = a_ref->GetParentCell();
		const auto world = cell ? cell->GetbhkWorld() : nullptr;

		if (world) {
			RE::BSReadLockGuard locker(world->worldLock);

			if (!a_nodeName.empty()) {
				const auto node = root->GetObjectByName(a_nodeName);
				const auto col = node ? static_cast<RE::bhkNiCollisionObject*>(node->collisionObject.get()) : nullptr;
				if (col) {
					get_material_type(col->body.get());
				}
			} else {
				RE::BSVisit::TraverseScenegraphCollision(root, [&](const RE::bhkNiCollisionObject* a_col) -> RE::BSVisit::BSVisitControl {
					get_material_type(a_col->body.get());

					return RE::BSVisit::BSVisitControl::kStop;
				});
			}
		}

		return result;
	}

	inline std::int32_t GetMotionType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{
		std::int32_t motionType = -1;

		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return motionType;
		}

		const auto root = a_ref->Get3D();
		if (!root) {
			a_vm->TraceForm(a_ref, "has no 3D", a_stackID);
			return motionType;
		}

		const auto cell = a_ref->GetParentCell();
		const auto world = cell ? cell->GetbhkWorld() : nullptr;

		if (world) {
			RE::BSReadLockGuard locker(world->worldLock);

			RE::BSVisit::TraverseScenegraphCollision(root, [&](const RE::bhkNiCollisionObject* a_col) -> RE::BSVisit::BSVisitControl {
				const auto body = a_col->body.get();
				const auto hkpRigidBody = body ? static_cast<RE::hkpRigidBody*>(body->referencedObject.get()) : nullptr;
				if (hkpRigidBody) {
					motionType = hkpRigidBody->motion.type.underlying();
				}
				return motionType != -1 ? RE::BSVisit::BSVisitControl::kStop : RE::BSVisit::BSVisitControl::kContinue;
			});
		}

		return motionType;
	}

	inline RE::Actor* GetRandomActorFromRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		float a_radius,
		bool a_ignorePlayer)
	{
		using RNG = SKSE::stl::RNG;

		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return nullptr;
		}

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			if (a_ignorePlayer && processLists->numberHighActors == 0) {
				return nullptr;
			}

			const auto squaredRadius = a_radius * a_radius;
			const auto originPos = a_ref->GetPosition();

			std::vector<RE::Actor*> result;
			result.reserve(processLists->numberHighActors);

			const auto get_actor_within_radius = [&](RE::Actor* a_actor) {
				if (a_actor && a_actor != a_ref && originPos.GetSquaredDistance(a_actor->GetPosition()) <= squaredRadius) {
					result.emplace_back(a_actor);
				}
			};

			for (auto& actorHandle : processLists->highActorHandles) {
				const auto actor = actorHandle.get();
				get_actor_within_radius(actor.get());
			}

			if (!a_ignorePlayer) {
				get_actor_within_radius(RE::PlayerCharacter::GetSingleton());
			}

			if (!result.empty()) {
				return result[RNG::GetSingleton()->Generate<size_t>(0, result.size() - 1)];
			}
		}

		return nullptr;
	}

	inline std::vector<RE::TESForm*> GetQuestItems(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		bool a_noEquipped,
		bool a_noFavourited)
	{
		std::vector<RE::TESForm*> result;

		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return result;
		}

		auto inv = a_ref->GetInventory();
		for (const auto& [item, data] : inv) {
			if (item->Is(RE::FormType::LeveledItem)) {
				continue;
			}
			const auto& [count, entry] = data;
			if (count > 0 && entry->IsQuestObject() && inv_util::can_be_taken(entry, a_noEquipped, a_noFavourited, false)) {
				result.push_back(item);
			}
		}

		return result;
	}

	inline std::vector<RE::BGSBaseAlias*> GetRefAliases(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{
		std::vector<RE::BGSBaseAlias*> result;

		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return result;
		}

		const auto xAliases = a_ref->extraList.GetByType<RE::ExtraAliasInstanceArray>();
		if (xAliases && !xAliases->aliases.empty()) {
			RE::BSReadLockGuard locker(xAliases->lock);
			result.reserve(xAliases->aliases.size());
			for (const auto& aliasData : xAliases->aliases) {
				if (aliasData && aliasData->alias) {
					result.emplace_back(const_cast<RE::BGSBaseAlias*>(aliasData->alias));
				}
			}
		}

		return result;
	}

	inline std::int32_t GetStoredSoulSize(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return -1;
		}

		return stl::to_underlying(a_ref->extraList.GetSoulLevel());
	}

	inline std::uint32_t HasArtObject(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		RE::BGSArtObject* a_art,
		bool a_active)
	{
		std::uint32_t count = 0;

		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return count;
		}
		if (!a_art) {
			a_vm->TraceStack("Art Object is None", a_stackID);
			return count;
		}

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			const auto handle = a_ref->CreateRefHandle();
			processLists->GetModelEffects([&](const RE::ModelReferenceEffect& a_modelEffect) {
				if (a_modelEffect.target == handle && a_modelEffect.artObject == a_art) {
					if (!a_active || !a_modelEffect.finished) {
						count++;
					}
				}
				return true;
			});
		}

		return count;
	}

	inline std::uint32_t HasEffectShader(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		const RE::TESEffectShader* a_effectShader,
		bool a_active)
	{
		std::uint32_t count = 0;

		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return count;
		}
		if (!a_effectShader) {
			a_vm->TraceStack("Effect Shader is None", a_stackID);
			return count;
		}

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			const auto handle = a_ref->CreateRefHandle();
			processLists->GetShaderEffects([&](const RE::ShaderReferenceEffect& a_shaderEffect) {
				if (a_shaderEffect.target == handle && a_shaderEffect.effectData == a_effectShader) {
					if (!a_active || !a_shaderEffect.finished) {
						count++;
					}
				}
				return true;
			});
		}

		return count;
	}

	inline bool IsLoadDoor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return false;
		}

		return a_ref->extraList.HasType(RE::ExtraDataType::kTeleport);
	}

	inline bool IsQuestItem(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return false;
		}

		if (const auto xAliases = a_ref->extraList.GetByType<RE::ExtraAliasInstanceArray>(); xAliases) {
			RE::BSReadLockGuard locker(xAliases->lock);

			for (const auto& aliasData : xAliases->aliases) {
				const auto alias = aliasData ? aliasData->alias : nullptr;

				if (alias && alias->IsQuestObject()) {
					return true;
				}
			}

			return false;
		} else {
			return a_ref->HasQuestObject();
		}
	}

	inline bool IsVIP(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{
		using FILL_TYPE = RE::BGSBaseAlias::FILL_TYPE;
		using TYPE = RE::QUEST_DATA::Type;

		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return false;
		}

		if (const auto xAliases = a_ref->extraList.GetByType<RE::ExtraAliasInstanceArray>(); xAliases) {
			RE::BSReadLockGuard locker(xAliases->lock);

			for (const auto& aliasData : xAliases->aliases) {
				const auto alias = aliasData ? aliasData->alias : nullptr;
				const auto quest = alias ? alias->owningQuest : nullptr;

				if (quest && quest->GetType() != TYPE::kNone && quest->IsRunning()) {
					const auto fillType = alias->fillType.get();
					if (alias->IsQuestObject() || alias->IsEssential() || alias->IsProtected() || fillType == FILL_TYPE::kForced || fillType == FILL_TYPE::kUniqueActor) {
						return true;
					}
				}
			}
		}

		return false;
	}

	inline bool HasNiExtraData(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_name)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return false;
		}

		const auto root = a_ref->Get3D();
		if (!root) {
			a_vm->TraceForm(a_ref, "has no 3D", a_stackID);
			return false;
		}

		return root->HasExtraData(a_name);
	}

	inline void MoveToNearestNavmeshLocation(VM* a_vm, StackID a_stackID,
		RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}

		const auto find_nearest_vertex = [&]() {
			std::optional<RE::NiPoint3> pos = std::nullopt;

			const auto cell = a_ref->GetParentCell();
			if (!cell || !cell->navMeshes) {
				return pos;
			}

			auto& [navMeshes] = *cell->navMeshes;
			auto shortestDistance = std::numeric_limits<float>::max();

			for (const auto& navMesh : navMeshes) {
				for (auto& [location] : navMesh->vertices) {
					const auto linearDistance = a_ref->GetPosition().GetDistance(location);
					if (linearDistance < shortestDistance) {
						shortestDistance = linearDistance;
						pos.emplace(location);
					}
				}
			}

			return pos;
		};

		const auto nearestVertex = find_nearest_vertex();
		if (!nearestVertex.has_value()) {
			a_vm->TraceForm(a_ref, "does not have a valid navmesh position", a_stackID);
			return;
		}

		const auto handle = a_ref->CreateRefHandle();
		auto task = SKSE::GetTaskInterface();
		task->AddTask([handle, nearestVertex]() {
			const auto ref = handle.get();
			if (ref) {
				ref->SetPosition(*nearestVertex);
			}
		});
	}

	inline bool RemoveKeywordFromRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		RE::BGSKeyword* a_keyword)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return false;
		}
		if (!a_keyword) {
			a_vm->TraceStack("Keyword is None", a_stackID);
			return false;
		}

		const auto form = a_ref->GetObjectReference();
		return form && KeywordManager::GetSingleton()->Remove(form, a_keyword);
	}

	inline void PlayDebugShader(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		std::vector<float> a_rgba)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}

		if (a_rgba.size() != 4) {
			a_vm->TraceStack("RGBA array is not valid", a_stackID);
			return;
		}

		const auto root = a_ref->Get3D();
		if (!root) {
			a_vm->TraceForm(a_ref, "has no 3D", a_stackID);
			return;
		}

		RE::NiColorA color{ a_rgba[0], a_rgba[1], a_rgba[2], a_rgba[3] };

		auto task = SKSE::GetTaskInterface();
		task->AddTask([root, color]() {
			root->TintScenegraph(color);
		});
	}

	inline void ReplaceKeywordOnRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		const RE::BGSKeyword* a_remove,
		RE::BGSKeyword* a_add)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}
		if (!a_remove) {
			a_vm->TraceStack("Remove keyword is None", a_stackID);
			return;
		}
		if (!a_add) {
			a_vm->TraceStack("Add keyword is None", a_stackID);
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
					if (const auto keyword = keywordForm->keywords[i]; keyword) {
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

	inline void ScaleObject3D(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		float a_scale)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}

		const auto root = a_ref->Get3D();
		if (!root) {
			a_vm->TraceForm(a_ref, "has no 3D", a_stackID);
			return;
		}

		const auto scale_collision = [&](RE::bhkWorldObject* a_body) {
			if (!a_body) {
				return;
			}

			const auto hkpBody = static_cast<RE::hkpWorldObject*>(a_body->referencedObject.get());
			const auto hkpShape = hkpBody ? hkpBody->GetShape() : nullptr;

			if (hkpShape) {
				switch (hkpShape->type) {
				case RE::hkpShapeType::kBox:
					{
						const auto boxShape = const_cast<RE::hkpBoxShape*>(static_cast<const RE::hkpBoxShape*>(hkpShape));
						if (boxShape) {
							boxShape->halfExtents = boxShape->halfExtents * RE::hkVector4(a_scale);
						}
					}
					break;
				case RE::hkpShapeType::kSphere:
					{
						const auto sphereShape = const_cast<RE::hkpSphereShape*>(static_cast<const RE::hkpSphereShape*>(hkpShape));
						if (sphereShape) {
							sphereShape->radius *= a_scale;
						}
					}
					break;
				case RE::hkpShapeType::kCapsule:
					{
						const auto capsuleShape = const_cast<RE::hkpCapsuleShape*>(static_cast<const RE::hkpCapsuleShape*>(hkpShape));
						if (capsuleShape) {
							const float radius = capsuleShape->radius * a_scale;

							capsuleShape->radius = radius;
							capsuleShape->vertexA.quad.m128_f32[3] = radius;
							capsuleShape->vertexB.quad.m128_f32[3] = radius;
						}
					}
					break;
				default:
					break;
				}
			}
		};

		if (!a_nodeName.empty()) {
			if (const auto object = root->GetObjectByName(a_nodeName); object) {
				auto task = SKSE::GetTaskInterface();
				task->AddTask([object, a_scale]() {
					object->local.scale *= a_scale;

					RE::NiUpdateData updateData{ 0.0f, RE::NiUpdateData::Flag::kNone };
					object->Update(updateData);
				});

				const auto cell = a_ref->GetParentCell();
				const auto world = cell ? cell->GetbhkWorld() : nullptr;

				if (world) {
					RE::BSWriteLockGuard locker(world->worldLock);

					if (const auto node = object->AsNode(); node) {
						RE::BSVisit::TraverseScenegraphCollision(node, [&](const RE::bhkNiCollisionObject* a_col) -> RE::BSVisit::BSVisitControl {
							scale_collision(a_col->body.get());

							return RE::BSVisit::BSVisitControl::kContinue;
						});
					} else {
						if (const auto col = static_cast<RE::bhkNiCollisionObject*>(object->collisionObject.get()); col) {
							scale_collision(col->body.get());
						}
					}
				}
			}
		} else {
			const auto task = SKSE::GetTaskInterface();
			task->AddTask([root, a_scale]() {
				root->local.scale *= a_scale;

				RE::NiUpdateData updateData{ 0.0f, RE::NiUpdateData::Flag::kNone };
				root->Update(updateData);
			});

			const auto cell = a_ref->GetParentCell();
			const auto world = cell ? cell->GetbhkWorld() : nullptr;

			if (world) {
				RE::BSWriteLockGuard locker(world->worldLock);

				RE::BSVisit::TraverseScenegraphCollision(root, [&](const RE::bhkNiCollisionObject* a_col) -> RE::BSVisit::BSVisitControl {
					scale_collision(a_col->body.get());

					return RE::BSVisit::BSVisitControl::kContinue;
				});
			}
		}
	}

	inline void SetBaseObject(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		RE::TESForm* a_base)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}
		if (!a_base) {
			a_vm->TraceStack("Base Form is None", a_stackID);
			return;
		}
		if (!a_base->IsBoundObject()) {
			a_vm->TraceForm(a_ref, "Base Form is not a Bound Object", a_stackID);
			return;
		}

		a_ref->SetObjectReference(static_cast<RE::TESBoundObject*>(a_base));
	}

	inline void SetEffectShaderDuration(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		const RE::TESEffectShader* a_effectShader,
		float a_time,
		bool a_absolute)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}
		if (!a_effectShader) {
			a_vm->TraceStack("Effect Shader is None", a_stackID);
			return;
		}

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			const auto handle = a_ref->CreateRefHandle();
			processLists->GetShaderEffects([&](RE::ShaderReferenceEffect& a_shaderEffect) {
				if (a_shaderEffect.target == handle && a_shaderEffect.effectData == a_effectShader) {
					if (!a_absolute) {
						const auto value = a_shaderEffect.lifetime + a_time;
						if (value < -1.0f) {
							return false;
						}
					}
					a_shaderEffect.lifetime = a_time;
				}
				return true;
			});
		}
	}

	inline bool SetDoorDestination(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::TESObjectREFR* a_ref,
		RE::TESObjectREFR* a_door)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return false;
		}
		if (!a_door) {
			a_vm->TraceStack("Destination Door is None", a_stackID);
			return false;
		}

		const auto teleport = a_door->extraList.GetByType<RE::ExtraTeleport>();
		const auto teleportData = teleport ? teleport->teleportData : nullptr;

		if (teleportData) {
			teleportData->linkedDoor = a_door->CreateRefHandle();
			return true;
		}

		return false;
	}

	inline void SetLinkedRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		[[maybe_unused]] RE::TESObjectREFR* a_targetRef,
		[[maybe_unused]] RE::BGSKeyword* a_keyword)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}

		a_ref->extraList.SetLinkedRef(a_targetRef, a_keyword);
	}

	inline void SetMaterialType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_newMaterialType,
		RE::BSFixedString a_oldMaterialType,
		RE::BSFixedString a_nodeName)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}

		if (a_newMaterialType.empty()) {
			a_vm->TraceStack("Material type string is empty", a_stackID);
			return;
		}

		const auto root = a_ref->Get3D();
		if (!root) {
			a_vm->TraceForm(a_ref, "has no 3D", a_stackID);
			return;
		}

		auto newID = RE::MATERIAL_ID::kNone;
		auto oldID = RE::MATERIAL_ID::kNone;

		for (const auto& [id, matString] : MATERIAL::materialMap) {
			if (string::icontains(matString, a_newMaterialType)) {
				newID = id;
				break;
			}
			if (!a_oldMaterialType.empty() && string::icontains(matString, a_oldMaterialType)) {
				oldID = id;
				break;
			}
		}

		const auto set_material_type = [&](RE::bhkWorldObject* a_body) {
			if (!a_body) {
				return;
			}

			const auto hkpBody = static_cast<RE::hkpWorldObject*>(a_body->referencedObject.get());
			const auto hkpShape = hkpBody ? hkpBody->GetShape() : nullptr;

			if (hkpShape) {
				if (hkpShape->type == RE::hkpShapeType::kMOPP) {
					const auto mopp = static_cast<const RE::hkpMoppBvTreeShape*>(hkpShape);
					const auto childShape = mopp ? mopp->child.childShape : nullptr;
					const auto compressedShape = childShape ? netimmerse_cast<RE::bhkCompressedMeshShape*>(childShape->userData) : nullptr;
					const auto shapeData = compressedShape ? compressedShape->data.get() : nullptr;

					if (shapeData) {
						for (auto& meshMaterial : shapeData->meshMaterials) {
							if (oldID != RE::MATERIAL_ID::kNone && meshMaterial.materialID != oldID) {
								continue;
							}
							meshMaterial.materialID = newID;
						}
					}
				} else if (const auto bhkShape = hkpShape->userData; bhkShape) {
					if (oldID != RE::MATERIAL_ID::kNone && bhkShape->materialID != oldID) {
						return;
					}
					bhkShape->materialID = newID;
				}
			}
		};

		if (newID != RE::MATERIAL_ID::kNone) {
			const auto cell = a_ref->GetParentCell();
			const auto world = cell ? cell->GetbhkWorld() : nullptr;

			if (world) {
				RE::BSWriteLockGuard locker(world->worldLock);

				if (!a_nodeName.empty()) {
					const auto object = root->GetObjectByName(a_nodeName);
					const auto colObject = object ? static_cast<RE::bhkNiCollisionObject*>(object->collisionObject.get()) : nullptr;
					if (colObject) {
						set_material_type(colObject->body.get());
					}
				} else {
					RE::BSVisit::TraverseScenegraphCollision(root, [&](const RE::bhkNiCollisionObject* a_col) -> RE::BSVisit::BSVisitControl {
						set_material_type(a_col->body.get());

						return RE::BSVisit::BSVisitControl::kContinue;
					});
				}
			}
		}
	}

	inline void SetShaderType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		RE::TESObjectREFR* a_template,
		RE::BSFixedString a_filter,
		std::uint32_t a_shaderType,
		std::int32_t a_textureType,
		bool a_noWeapons,
		bool a_noAlpha)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}
		if (!a_template) {
			a_vm->TraceStack("Template is None", a_stackID);
			return;
		}
		if (!a_ref->Is3DLoaded()) {
			a_vm->TraceForm(a_ref, "has no 3D", a_stackID);
			return;
		}
		if (!a_template->Is3DLoaded()) {
			a_vm->TraceForm(a_template, "template object has no 3D", a_stackID);
			return;
		}

		using Texture = RE::BSTextureSet::Texture;
		using Feature = RE::BSShaderMaterial::Feature;

		auto sourcePath{ std::string() };
		if (!a_filter.empty()) {
			sourcePath = a_filter.c_str();
			TEXTURE::sanitize_path(sourcePath);
		}

		const std::vector<bool> params{ a_noWeapons, a_noAlpha, a_ref->Is(RE::FormType::ActorCharacter) };

		const auto root = a_ref->Get3D()->AsNode();
		const auto template_root = a_template->Get3D()->AsNode();
		const auto feature = static_cast<Feature>(a_shaderType);

		if (root && template_root) {
			auto task = SKSE::GetTaskInterface();
			task->AddTask([root, template_root, feature, sourcePath, a_textureType, params]() {
				if (const auto template_geo = template_root->GetFirstGeometryOfShaderType(feature); template_geo) {
					std::vector<RE::BSFixedString> result;
					SET::ShaderType(root, template_geo, sourcePath, a_textureType, result, params);

					if (!result.empty()) {
						std::string name{ "PO3_SHADER | "sv };
						name.append(std::to_string(stl::to_underlying(feature)));

						SET::add_data_if_none<RE::NiStringsExtraData>(root, name, result);
					}
				}
			});
		}
	}

	inline void SetupBodyPartGeometry(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_bodyparts,
		RE::Actor* a_actor)
	{
		if (!a_bodyparts) {
			a_vm->TraceStack("BodyParts is None", a_stackID);
			return;
		}
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		const auto root = a_bodyparts->Get3D();
		if (!root) {
			a_vm->TraceForm(a_bodyparts, "has no 3D", a_stackID);
			return;
		}

		const auto actorbase = a_actor->GetActorBase();
		const auto rootNode = root->AsFadeNode();

		if (actorbase && rootNode) {
			const auto actorRoot = a_actor->Get3D(false);
			const auto data = actorRoot ? actorRoot->GetExtraData<RE::NiIntegerExtraData>(EXTRA::SKIN_TINT) : nullptr;

			RE::NiColor color{ data ? data->value : actorbase->bodyTintColor };

			auto task = SKSE::GetTaskInterface();
			task->AddTask([root, color]() {
				root->UpdateBodyTint(color);
			});
		}
	}

	inline void StopAllShaders(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			processLists->StopAllMagicEffects(*a_ref);
		}
	}

	inline void StopArtObject(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		const RE::BGSArtObject* a_art)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}
		if (!a_art) {
			a_vm->TraceStack("Art Object is None", a_stackID);
			return;
		}

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			const auto handle = a_ref->CreateRefHandle();
			processLists->GetModelEffects([&](RE::ModelReferenceEffect& a_modelEffect) {
				if (a_modelEffect.target == handle && a_modelEffect.artObject == a_art) {
					a_modelEffect.finished = true;
				}
				return true;
			});
		}
	}

	inline void ToggleChildNode(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_nodeName,
		bool a_disable)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}

		const auto root = a_ref->Get3D();
		if (!root) {
			a_vm->TraceForm(a_ref, "has no 3D", a_stackID);
			return;
		}

		auto task = SKSE::GetTaskInterface();
		task->AddTask([root, a_nodeName, a_disable]() {
			if (const auto object = root->GetObjectByName(a_nodeName); object) {
				SET::Toggle(root, object, a_disable);
			}
		});
	}

	inline void UpdateHitEffectArtNode(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		const RE::BGSArtObject* a_art,
		RE::BSFixedString a_toNode,
		std::vector<float> a_translate,
		std::vector<float> a_rotate,
		float a_scale)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}
		if (!a_art) {
			a_vm->TraceStack("Art is None", a_stackID);
			return;
		}
		if (a_translate.size() != 3 || a_rotate.size() != 3) {
			a_vm->TraceForm(a_ref, "invalid size", a_stackID);
			return;
		}

		RE::ModelReferenceEffect* hitEffect = nullptr;

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			const auto handle = a_ref->CreateRefHandle();
			processLists->GetModelEffects([&](RE::ModelReferenceEffect& a_modelEffect) {
				if (a_modelEffect.target == handle && a_modelEffect.artObject == a_art) {
					hitEffect = &a_modelEffect;
					return false;
				}
				return true;
			});
		}

		const auto art = hitEffect ? hitEffect->hitEffectArtData.attachedArt.get() : nullptr;

		if (art) {
			RE::NiMatrix3 rotate{};
			const RE::NiPoint3 rot{ RE::deg_to_rad(a_rotate[0]), RE::deg_to_rad(a_rotate[1]), RE::deg_to_rad(a_rotate[2]) };
			rotate.SetEulerAnglesXYZ(rot);

			RE::NiTransform transform;
			transform.translate = { a_translate[0], a_translate[1], a_translate[2] };
			transform.rotate = rotate;
			transform.scale = a_scale;

			const auto task = SKSE::GetTaskInterface();
			task->AddTask([a_toNode, art, hitEffect, transform]() {
				if (!a_toNode.empty() && hitEffect->hitEffectArtData.nodeName != a_toNode) {
					const auto root = hitEffect->hitEffectArtData.current3DRoot.get();
					const auto newObj = root ? root->GetObjectByName(a_toNode) : nullptr;
					const auto newNode = newObj ? newObj->AsNode() : nullptr;

					if (newNode) {
						if (const auto attachTData = art->GetExtraData<RE::NiStringsExtraData>("AttachT"sv); attachTData) {
							const std::string namedNode(MAGIC::namedNode);
							const auto oldNodeStr{ namedNode + hitEffect->hitEffectArtData.nodeName.c_str() };
							const auto newNodeStr{ namedNode + a_toNode.c_str() };
							attachTData->Replace(oldNodeStr, newNodeStr);
						}
						newNode->AttachChild(art);

						RE::NiUpdateData data{ RE::GetDurationOfApplicationRunTime() * 0.001f, RE::NiUpdateData::Flag::kDirty };
						art->UpdateWorldData(&data);

						hitEffect->hitEffectArtData.nodeName = a_toNode;
					}
				}

				for (const auto& nodesPtr : art->children) {
					if (const auto nodes = nodesPtr.get(); nodes) {
						nodes->local = transform;
					}
				}
			});
		}
	}

	inline void Bind(VM& a_vm)
	{
		BIND(AddAllItemsToArray);
		BIND(AddAllItemsToList);
		BIND(AddItemsOfTypeToArray);
		BIND(AddItemsOfTypeToList);
		BIND(AddKeywordToRef);
		BIND(ApplyMaterialShader);
		BIND(FindAllReferencesOfFormType);
		BIND(FindAllReferencesOfType);
		BIND(FindAllReferencesWithKeyword);
		BIND(FindFirstItemInList);
		BIND(GetActivateChildren);
		BIND(GetActiveGamebryoAnimation);
		BIND(GetActorCause);
		BIND(GetAllArtObjects);
		BIND(GetAllEffectShaders);
		BIND(GetClosestActorFromRef);
		BIND(GetDoorDestination);
		BIND(GetEffectShaderDuration);
		BIND(GetLinkedChildren);
		BIND(GetMagicEffectSource);
		BIND(GetMaterialType);
		BIND(GetMotionType);
		BIND(GetRandomActorFromRef);
		BIND(GetQuestItems);
		BIND(GetRefAliases);
		BIND(GetStoredSoulSize);
		BIND(HasArtObject);
		BIND(HasEffectShader);
		BIND(HasNiExtraData);
		BIND(IsLoadDoor, true);
		BIND(IsQuestItem);
		BIND(IsVIP);
		BIND(MoveToNearestNavmeshLocation);
		BIND(RemoveKeywordFromRef);
		BIND(ReplaceKeywordOnRef);
		BIND(PlayDebugShader);
		BIND(ScaleObject3D);
		BIND(SetBaseObject);
		BIND(SetDoorDestination);
		BIND(SetEffectShaderDuration);
		BIND(SetLinkedRef);
		BIND(SetMaterialType);
		BIND(SetShaderType);
		BIND(SetupBodyPartGeometry);
		BIND(StopAllShaders);
		BIND(StopArtObject);
		BIND(ToggleChildNode);
		BIND(UpdateHitEffectArtNode);

		logger::info("Registered object reference functions"sv);
	}
}
