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

		inline void remove_item(
			RE::TESObjectREFR* a_ref,
			RE::TESBoundObject* a_item,
			std::uint32_t a_count,
			bool a_silent,
			RE::TESObjectREFR* a_otherContainer,
			StackID a_stackID,
			VM* a_vm)
		{
			using func_t = decltype(&inv_util::remove_item);
			REL::Relocation<func_t> func{ RELOCATION_ID(56261, 56647) };
			return func(a_ref, a_item, a_count, a_silent, a_otherContainer, a_stackID, a_vm);
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
				for (auto i = 0; i < count; i++) {
					result.push_back(item);
				}
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
				for (auto i = 0; i < count; i++) {
					result.push_back(item);
				}
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
			FORM::KeywordManager::GetSingleton()->Add(form, a_keyword);
		}
	}

	inline std::vector<RE::TESObjectREFR*> FindAllReferencesOfFormType(RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_origin,
		std::uint32_t a_formType,
		float a_radius)
	{
		std::vector<RE::TESObjectREFR*> result;

		if (const auto TES = RE::TES::GetSingleton(); TES) {
			const auto formType = static_cast<RE::FormType>(a_formType);

			TES->ForEachReferenceInRange(a_origin, a_radius, [&](RE::TESObjectREFR& a_ref) {
				if (a_ref.Is3DLoaded()) {
					const auto base = a_ref.GetBaseObject();
					if (formType == RE::FormType::None || a_ref.Is(formType) || base && base->Is(formType)) {
						result.push_back(&a_ref);
					}
				}
				return RE::BSContainer::ForEachResult::kContinue;
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
				if (const auto base = b_ref.GetBaseObject(); base && b_ref.Is3DLoaded()) {
					if (list && list->HasForm(base) || a_formOrList == base) {
						result.push_back(&b_ref);
					}
				}
				return RE::BSContainer::ForEachResult::kContinue;
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
				if (b_ref.Is3DLoaded()) {
					bool success = false;
					if (list) {
						success = b_ref.HasKeywordInList(list, a_matchAll);
					} else if (keyword) {
						success = b_ref.HasKeyword(keyword);
					}
					if (success) {
						result.push_back(&b_ref);
					}
				}
				return RE::BSContainer::ForEachResult::kContinue;
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
		for (const auto& item : inv | std::views::keys) {
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

		if (const auto refChildren = a_ref->extraList.GetByType<RE::ExtraActivateRefChildren>(); refChildren) {
			for (const auto& child : refChildren->children) {
				if (child) {
					if (const auto ref = child->activateRef.get(); ref) {
						result.push_back(ref.get());
					}
				}
			}
		}

		if (const auto missingIDs = a_ref->extraList.GetByType<RE::ExtraMissingRefIDs>(); missingIDs && missingIDs->IDs && missingIDs->numIDs > 0) {
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
			processLists->ForEachModelEffect([&](const RE::ModelReferenceEffect& a_modelEffect) {
				if (a_modelEffect.target == handle) {
					if (const auto art = a_modelEffect.artObject; art) {
						result.push_back(art);
					}
				}
				return RE::BSContainer::ForEachResult::kContinue;
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
			processLists->ForEachShaderEffect([&](const RE::ShaderReferenceEffect& a_shaderEffect) {
				if (a_shaderEffect.target == handle) {
					if (const auto shader = a_shaderEffect.effectData; shader) {
						result.push_back(shader);
					}
				}
				return RE::BSContainer::ForEachResult::kContinue;
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
			processLists->ForEachShaderEffect([&](const RE::ShaderReferenceEffect& a_shaderEffect) {
				if (a_shaderEffect.target == handle && a_shaderEffect.effectData == a_effectShader) {
					time = a_shaderEffect.lifetime;
					return RE::BSContainer::ForEachResult::kStop;
				}
				return RE::BSContainer::ForEachResult::kContinue;
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
					if (const auto ref = child.refr.get(); ref) {
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
#ifndef SKYRIMVR
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
#else
		if (magicTarget) {
			magicTarget->VisitActiveEffects([&](RE::ActiveEffect* activeEffect) -> RE::BSContainer::ForEachResult {
				const auto mgef = activeEffect ? activeEffect->GetBaseObject() : nullptr;
				if (mgef && mgef == a_mgef) {
					result.push_back(activeEffect->spell);
					const auto caster = activeEffect->caster.get();
					result.push_back(caster.get());
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
		}
#endif

		return result;
	}

	inline std::vector<std::string> GetMaterialType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		std::string a_nodeName)
	{
		std::vector<std::string> result;

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
							result.emplace_back(GRAPHICS::MATERIAL::get_material(meshMaterial.materialID));
						}
					}
				} else if (const auto bhkShape = hkpShape->userData; bhkShape) {
					result.emplace_back(GRAPHICS::MATERIAL::get_material(bhkShape->materialID).data());
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
		using RNG = stl::RNG;

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

		if (const auto xAliases = a_ref->extraList.GetByType<RE::ExtraAliasInstanceArray>(); xAliases && !xAliases->aliases.empty()) {
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

	inline std::int32_t GetRefCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return -1;
		}

		return a_ref->extraList.GetCount();
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
			processLists->ForEachModelEffect([&](const RE::ModelReferenceEffect& a_modelEffect) {
				if (a_modelEffect.target == handle && a_modelEffect.artObject == a_art) {
					if (!a_active || !a_modelEffect.finished) {
						count++;
					}
				}
				return RE::BSContainer::ForEachResult::kContinue;
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
			processLists->ForEachShaderEffect([&](const RE::ShaderReferenceEffect& a_shaderEffect) {
				if (a_shaderEffect.target == handle && a_shaderEffect.effectData == a_effectShader) {
					if (!a_active || !a_shaderEffect.finished) {
						count++;
					}
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
		}

		return count;
	}

	inline bool IsCasting(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		RE::TESForm* a_form)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return false;
		}
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return false;
		}

		const auto magicItem = a_form->As<RE::MagicItem>();
		if (!magicItem) {
			a_vm->TraceStack("Form is not a magic item", a_stackID);
			return false;
		}

		if (const auto actor = a_ref->As<RE::Actor>(); actor) {
			return actor->IsCasting(magicItem);
		} else {
			const auto magicCaster = a_ref->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
			return magicCaster && magicCaster->currentSpell == magicItem;
		}
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

			return std::ranges::any_of(xAliases->aliases, [](const auto& aliasData) {
				const auto alias = aliasData ? aliasData->alias : nullptr;
				return alias && alias->IsQuestObject();
			});
		}

		return a_ref->HasQuestObject();
	}

	inline bool IsRefInWater(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return false;
		}

		return a_ref->IsPointSubmergedMoreThan(a_ref->GetPosition(), a_ref->GetParentCell(), 0.01f);
	}

	inline bool IsRefUnderwater(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return false;
		}

		return a_ref->IsPointSubmergedMoreThan(a_ref->GetPosition(), a_ref->GetParentCell(), 0.875f);
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
		if (!nearestVertex) {
			a_vm->TraceForm(a_ref, "does not have a valid navmesh position", a_stackID);
			return;
		}

		const auto handle = a_ref->CreateRefHandle();
		SKSE::GetTaskInterface()->AddTask([handle, nearestVertex]() {
			if (const auto ref = handle.get(); ref) {
				ref->SetPosition(*nearestVertex);
			}
		});
	}

	inline void RemoveAllModItems(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_modName,
		bool a_unequip)
	{
		using Slot = RE::BIPED_MODEL::BipedObjectSlot;

		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}

		const auto dataHandler = RE::TESDataHandler::GetSingleton();
		const auto modInfo = dataHandler ? dataHandler->LookupModByName(a_modName) : nullptr;

		if (!modInfo) {
			return;
		}

		auto inv = a_ref->GetInventory([&](const RE::TESBoundObject& a_object) {
			return modInfo->IsFormInMod(a_object.GetFormID());
		});

		if (inv.empty()) {
			return;
		}

		if (a_unequip) {
			if (const auto actor = a_ref->As<RE::Actor>()) {
				for (const auto& [item, data] : inv) {
					const auto& [count, entry] = data;
					if (count > 0 && entry->IsWorn()) {
						RE::ActorEquipManager::GetSingleton()->UnequipObject(actor, item);
					}
				}
			}
		} else {
			for (const auto& [item, data] : inv) {
				inv_util::remove_item(a_ref, item, data.first, false, nullptr, a_stackID, a_vm);
			}
		}
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
		return form && FORM::KeywordManager::GetSingleton()->Remove(form, a_keyword);
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

	inline void SetCollisionLayer(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		std::string a_nodeName,
		std::uint32_t a_colLayer)
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

		const auto colLayer = static_cast<RE::COL_LAYER>(a_colLayer);

		if (a_nodeName.empty()) {
			root->SetCollisionLayer(colLayer);
		} else {
			auto object = root->GetObjectByName(a_nodeName);
			if (object) {
				object->SetCollisionLayer(colLayer);
			}
		}
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
			processLists->ForEachShaderEffect([&](RE::ShaderReferenceEffect& a_shaderEffect) {
				if (a_shaderEffect.target == handle && a_shaderEffect.effectData == a_effectShader) {
					if (!a_absolute) {
						const auto value = a_shaderEffect.lifetime + a_time;
						if (value < -1.0f) {
							return RE::BSContainer::ForEachResult::kStop;
						}
					}
					a_shaderEffect.lifetime = a_time;
				}
				return RE::BSContainer::ForEachResult::kContinue;
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

		const auto teleport = a_ref->extraList.GetByType<RE::ExtraTeleport>();
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
		std::string a_newMaterialType,
		std::string a_oldMaterialType,
		std::string a_nodeName)
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

		for (const auto& [id, matString] : GRAPHICS::MATERIAL::materialMap) {
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
			processLists->ForEachModelEffect([&](RE::ModelReferenceEffect& a_modelEffect) {
				if (a_modelEffect.target == handle && a_modelEffect.artObject == a_art) {
					a_modelEffect.finished = true;
				}
				return RE::BSContainer::ForEachResult::kContinue;
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
		BIND(GetRefCount);
		BIND(GetStoredSoulSize);
		BIND(HasArtObject);
		BIND(HasEffectShader);
		BIND(HasNiExtraData);
		BIND(IsCasting);
		BIND(IsLoadDoor, true);
		BIND(IsQuestItem);
		BIND(IsRefInWater);
		BIND(IsRefUnderwater);
		BIND(IsVIP);
		BIND(MoveToNearestNavmeshLocation);
		BIND(RemoveAllModItems);
		BIND(RemoveKeywordFromRef);
		BIND(ReplaceKeywordOnRef);
		BIND(SetBaseObject);
		BIND(SetCollisionLayer);
		BIND(SetDoorDestination);
		BIND(SetEffectShaderDuration);
		BIND(SetLinkedRef);
		BIND(SetMaterialType);
		BIND(StopAllShaders);
		BIND(StopArtObject);

		logger::info("Registered object reference functions"sv);
	}
}
