#pragma once

#include "Papyrus/Util/Inventory.h"
#include "Serialization/Services.h"

namespace Papyrus::ObjectReference
{
	inline std::vector<RE::TESForm*> AddAllItemsToArray(STATIC_ARGS, RE::TESObjectREFR* a_ref, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem)
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
			if (count > 0 && INV::can_be_taken(entry, a_noEquipped, a_noFavourited, a_noQuestItem)) {
				result.push_back(item);
			}
		}

		return result;
	}

	inline void AddAllItemsToList(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::BGSListForm* a_list, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem)
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
			if (count > 0 && INV::can_be_taken(entry, a_noEquipped, a_noFavourited, a_noQuestItem)) {
				a_list->AddForm(item);
			}
		}
	}

	inline std::vector<RE::TESForm*> AddItemsOfTypeToArray(STATIC_ARGS, RE::TESObjectREFR* a_ref, std::uint32_t a_formType, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem)
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
			if (count > 0 && (formType == RE::FormType::None || item->Is(formType)) && INV::can_be_taken(entry, a_noEquipped, a_noFavourited, a_noQuestItem)) {
				result.push_back(item);
			}
		}

		return result;
	}

	inline void AddItemsOfTypeToList(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::BGSListForm* a_list, std::uint32_t a_formType, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem)
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
			if (count > 0 && (formType == RE::FormType::None || item->Is(formType)) && INV::can_be_taken(entry, a_noEquipped, a_noFavourited, a_noQuestItem)) {
				a_list->AddForm(item);
			}
		}
	}

	inline void AddKeywordToRef(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::BGSKeyword* a_keyword)
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

	inline void CastEx(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::TESForm* a_spell, RE::TESObjectREFR* a_target, RE::Actor* a_blameActor, std::uint32_t a_source)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}

		if (!a_spell) {
			a_vm->TraceStack("Spell is None", a_stackID);
			return;
		}

		auto magicCaster = a_ref->GetMagicCaster(static_cast<RE::MagicSystem::CastingSource>(a_source));
		if (!magicCaster) {
			magicCaster = a_ref->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
		}

		if (magicCaster) {
			magicCaster->CastSpellImmediate(a_spell->As<RE::MagicItem>(), false, a_target, 1.0f, false, 0.0f, a_blameActor);
		}
	}

	inline RE::TESForm* FindFirstItemInList(STATIC_ARGS, RE::TESObjectREFR* a_ref, const RE::BGSListForm* a_list)
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

	inline std::vector<RE::TESObjectREFR*> GetActivateChildren(STATIC_ARGS, RE::TESObjectREFR* a_ref)
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

	inline RE::BSFixedString GetActiveGamebryoAnimation(STATIC_ARGS, RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return {};
		}

		const auto root = a_ref->Get3D();
		if (!root) {
			a_vm->TraceForm(a_ref, "has no 3D", a_stackID);
			return {};
		}

		const auto controller = root->GetControllers();

		if (const auto manager = controller ? controller->AsNiControllerManager() : nullptr) {
			for (const auto& sequence : manager->sequenceArray) {
				if (sequence && sequence->Animating()) {
					return sequence->name;
				}
			}
		}

		return {};
	}

	inline std::vector<RE::ActiveEffect*> GetActiveMagicEffects(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::EffectSetting* a_mgef)
	{
		std::vector<RE::ActiveEffect*> result;

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
					result.push_back(activeEffect);
				}
			}
		}
#else
		if (magicTarget) {
			magicTarget->VisitActiveEffects([&](RE::ActiveEffect* activeEffect) -> RE::BSContainer::ForEachResult {
				const auto mgef = activeEffect ? activeEffect->GetBaseObject() : nullptr;
				if (mgef && mgef == a_mgef) {
					result.push_back(activeEffect);
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
		}
#endif

		return result;
	}

	inline RE::Actor* GetActorCause(STATIC_ARGS, RE::TESObjectREFR* a_ref)
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

	inline std::vector<RE::BGSArtObject*> GetAllArtObjects(STATIC_ARGS, RE::TESObjectREFR* a_ref)
	{
		std::vector<RE::BGSArtObject*> result;

		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return result;
		}

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			const auto handle = a_ref->CreateRefHandle();
			processLists->ForEachModelEffect([&](const RE::ModelReferenceEffect* a_modelEffect) {
				if (a_modelEffect->target == handle) {
					if (const auto art = a_modelEffect->artObject; art) {
						result.push_back(art);
					}
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
		}

		return result;
	}

	inline std::vector<RE::TESEffectShader*> GetAllEffectShaders(STATIC_ARGS, RE::TESObjectREFR* a_ref)
	{
		std::vector<RE::TESEffectShader*> result;

		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return result;
		}

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			const auto handle = a_ref->CreateRefHandle();
			processLists->ForEachShaderEffect([&](const RE::ShaderReferenceEffect* a_shaderEffect) {
				if (a_shaderEffect->target == handle) {
					if (const auto shader = a_shaderEffect->effectData; shader) {
						result.push_back(shader);
					}
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
		}

		return result;
	}

	inline RE::Actor* GetClosestActorFromRef(STATIC_ARGS, const RE::TESObjectREFR* a_ref, bool a_ignorePlayer)
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
			auto       shortestDistance = std::numeric_limits<float>::max();

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

	inline float GetEffectShaderDuration(STATIC_ARGS, RE::TESObjectREFR* a_ref, const RE::TESEffectShader* a_effectShader)
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
			processLists->ForEachShaderEffect([&](const RE::ShaderReferenceEffect* a_shaderEffect) {
				if (a_shaderEffect->target == handle && a_shaderEffect->effectData == a_effectShader) {
					time = a_shaderEffect->lifetime;
					return RE::BSContainer::ForEachResult::kStop;
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
		}

		return time;
	}

	inline RE::TESObjectREFR* GetDoorDestination(STATIC_ARGS, RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return nullptr;
		}

		const auto xTeleport = a_ref->extraList.GetByType<RE::ExtraTeleport>();

		if (const auto teleportData = xTeleport ? xTeleport->teleportData : nullptr) {
			return teleportData->linkedDoor.get().get();
		}

		return nullptr;
	}

	inline std::vector<RE::TESObjectREFR*> GetLinkedChildren(STATIC_ARGS, RE::TESObjectREFR* a_ref, const RE::BGSKeyword* a_keyword)
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

	inline std::vector<RE::TESForm*> GetMagicEffectSource(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::EffectSetting* a_mgef)
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

	inline std::vector<std::string> GetMaterialType(STATIC_ARGS, RE::TESObjectREFR* a_ref, std::string a_nodeName)
	{
		std::vector<std::string> result{};

		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return result;
		}

		const auto root = a_ref->Get3D();
		if (!root) {
			a_vm->TraceForm(a_ref, "has no 3D", a_stackID);
			return result;
		}

		const auto cell = a_ref->GetParentCell();
		const auto world = cell ? cell->GetbhkWorld() : nullptr;

		if (world) {
			RE::BSReadLockGuard locker(world->worldLock);

			if (!a_nodeName.empty()) {
				const auto node = root->GetObjectByName(a_nodeName);
				if (const auto col = node ? static_cast<RE::bhkNiCollisionObject*>(node->collisionObject.get()) : nullptr) {
					GRAPHICS::MATERIAL::for_each_material_type(col->body, [&](RE::MATERIAL_ID& a_matID) {
						result.emplace_back(RE::MaterialIDToString(a_matID).data());
					});
				}
			} else {
				RE::BSVisit::TraverseScenegraphCollision(root, [&](const RE::bhkNiCollisionObject* a_col) -> RE::BSVisit::BSVisitControl {
					GRAPHICS::MATERIAL::for_each_material_type(a_col->body, [&](RE::MATERIAL_ID& a_matID) {
						result.emplace_back(RE::MaterialIDToString(a_matID).data());
					});
					return RE::BSVisit::BSVisitControl::kStop;
				});
			}
		}

		return result;
	}

	inline std::int32_t GetMotionType(STATIC_ARGS, RE::TESObjectREFR* a_ref)
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
				if (const auto hkpRigidBody = a_col->body ? static_cast<RE::hkpRigidBody*>(a_col->body->referencedObject.get()) : nullptr) {
					motionType = hkpRigidBody->motion.type.underlying();
				}
				return motionType != -1 ? RE::BSVisit::BSVisitControl::kStop : RE::BSVisit::BSVisitControl::kContinue;
			});
		}

		return motionType;
	}

	inline RE::Actor* GetRandomActorFromRef(STATIC_ARGS, RE::TESObjectREFR* a_ref, float a_radius, bool a_ignorePlayer)
	{
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
				return result[clib_util::RNG().generate<std::size_t>(0, result.size() - 1)];
			}
		}

		return nullptr;
	}

	inline std::vector<RE::TESForm*> GetQuestItems(STATIC_ARGS, RE::TESObjectREFR* a_ref, bool a_noEquipped, bool a_noFavourited)
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
			if (count > 0 && entry->IsQuestObject() && INV::can_be_taken(entry, a_noEquipped, a_noFavourited, false)) {
				result.push_back(item);
			}
		}

		return result;
	}

	inline std::vector<RE::BGSBaseAlias*> GetRefAliases(STATIC_ARGS, RE::TESObjectREFR* a_ref)
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

	inline std::int32_t GetRefCount(STATIC_ARGS, RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return -1;
		}

		return a_ref->extraList.GetCount();
	}

	inline std::int32_t GetStoredSoulSize(STATIC_ARGS, RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return -1;
		}

		return std::to_underlying(a_ref->extraList.GetSoulLevel());
	}

	inline std::uint32_t HasArtObject(STATIC_ARGS,
		RE::TESObjectREFR* a_ref,
		RE::BGSArtObject*  a_art,
		bool               a_active)
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
			processLists->ForEachModelEffect([&](const RE::ModelReferenceEffect* a_modelEffect) {
				if (a_modelEffect->target == handle && a_modelEffect->artObject == a_art) {
					if (!a_active || !a_modelEffect->finished) {
						count++;
					}
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
		}

		return count;
	}

	inline std::uint32_t HasEffectShader(STATIC_ARGS, RE::TESObjectREFR* a_ref, const RE::TESEffectShader* a_effectShader, bool a_active)
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
			processLists->ForEachShaderEffect([&](const RE::ShaderReferenceEffect* a_shaderEffect) {
				if (a_shaderEffect->target == handle && a_shaderEffect->effectData == a_effectShader) {
					if (!a_active || !a_shaderEffect->finished) {
						count++;
					}
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
		}

		return count;
	}

	inline bool IsCasting(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::TESForm* a_form)
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
			a_vm->TraceForm(a_form, "is not a magic item", a_stackID);
			return false;
		}

		if (const auto actor = a_ref->As<RE::Actor>(); actor) {
			return actor->IsCasting(magicItem);
		} else {
			const auto magicCaster = a_ref->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
			return magicCaster && magicCaster->currentSpell == magicItem;
		}
	}

	inline bool IsLoadDoor(STATIC_ARGS, const RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return false;
		}

		return a_ref->extraList.HasType(RE::ExtraDataType::kTeleport);
	}

	inline bool IsQuestItem(STATIC_ARGS, const RE::TESObjectREFR* a_ref)
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

	inline bool IsRefInWater(STATIC_ARGS, RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return false;
		}

		return a_ref->IsPointSubmergedMoreThan(a_ref->GetPosition(), a_ref->GetParentCell(), 0.01f);
	}

	inline bool IsRefUnderwater(STATIC_ARGS, RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return false;
		}

		return a_ref->IsPointSubmergedMoreThan(a_ref->GetPosition(), a_ref->GetParentCell(), 0.875f);
	}

	inline bool IsVIP(STATIC_ARGS, RE::TESObjectREFR* a_ref)
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

	inline bool HasNiExtraData(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::BSFixedString a_name)
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

	inline void MoveToNearestNavmeshLocation(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
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

	inline void RemoveAllModItems(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::BSFixedString a_modName, bool a_unequip)
	{
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
				INV::remove_item(a_ref, item, data.first, true, nullptr, a_stackID, a_vm);
			}
		}
	}

	inline bool RemoveKeywordFromRef(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::BGSKeyword* a_keyword)
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

	inline void ReplaceKeywordOnRef(STATIC_ARGS, RE::TESObjectREFR* a_ref, const RE::BGSKeyword* a_remove, RE::BGSKeyword* a_add)
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
			bool          found = false;

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

	inline void SetBaseObject(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::TESForm* a_base)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}
		if (!a_base) {
			a_vm->TraceStack("Base Object is None", a_stackID);
			return;
		}
		if (!a_base->IsBoundObject()) {
			a_vm->TraceForm(a_ref, "is not a Bound Object", a_stackID);
			return;
		}

		SKSE::GetTaskInterface()->AddTask([a_ref, a_base]() {
			a_ref->SetObjectReference(static_cast<RE::TESBoundObject*>(a_base));
		});
	}

	inline void SetCollisionLayer(STATIC_ARGS, RE::TESObjectREFR* a_ref, std::string a_nodeName, std::uint32_t a_colLayer)
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

		SKSE::GetTaskInterface()->AddTask([a_nodeName, root, colLayer]() {
			if (a_nodeName.empty()) {
				root->SetCollisionLayer(colLayer);
			} else if (const auto object = root->GetObjectByName(a_nodeName)) {
				object->SetCollisionLayer(colLayer);
			}
		});
	}

	inline bool SetDoorDestination(STATIC_ARGS, const RE::TESObjectREFR* a_ref, RE::TESObjectREFR* a_door)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return false;
		}
		if (!a_door) {
			a_vm->TraceStack("Destination Door is None", a_stackID);
			return false;
		}

		const auto xTeleport = a_ref->extraList.GetByType<RE::ExtraTeleport>();

		if (const auto teleportData = xTeleport ? xTeleport->teleportData : nullptr) {
			teleportData->linkedDoor = a_door->CreateRefHandle();
			return true;
		}

		return false;
	}

	inline void SetEffectShaderDuration(STATIC_ARGS, RE::TESObjectREFR* a_ref, const RE::TESEffectShader* a_effectShader, float a_time, bool a_absolute)
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
			processLists->ForEachShaderEffect([&](RE::ShaderReferenceEffect* a_shaderEffect) {
				if (a_shaderEffect->target == handle && a_shaderEffect->effectData == a_effectShader) {
					if (!a_absolute) {
						const auto value = a_shaderEffect->lifetime + a_time;
						if (value < -1.0f) {
							return RE::BSContainer::ForEachResult::kStop;
						}
					}
					a_shaderEffect->lifetime = a_time;
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
		}
	}

	inline void SetKey(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::TESKey* a_key)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}

		if (auto lock = a_ref->GetLock()) {
			lock->key = a_key;
		}
	}

	inline void SetLinkedRef(STATIC_ARGS, RE::TESObjectREFR* a_ref, [[maybe_unused]] RE::TESObjectREFR* a_targetRef, [[maybe_unused]] RE::BGSKeyword* a_keyword)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}

		a_ref->extraList.SetLinkedRef(a_targetRef, a_keyword);
	}

	inline void SetMaterialType(STATIC_ARGS, RE::TESObjectREFR* a_ref, std::string a_newMaterialType, std::string a_oldMaterialType, std::string a_nodeName)
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

		const auto newID = GRAPHICS::MATERIAL::get_material(a_newMaterialType);
		const auto oldID = GRAPHICS::MATERIAL::get_material(a_oldMaterialType);

		if (newID != RE::MATERIAL_ID::kNone) {
			const auto cell = a_ref->GetParentCell();
			const auto world = cell ? cell->GetbhkWorld() : nullptr;

			if (world) {
				RE::BSWriteLockGuard locker(world->worldLock);

				if (!a_nodeName.empty()) {
					const auto object = root->GetObjectByName(a_nodeName);
					if (const auto colObject = object ? static_cast<RE::bhkNiCollisionObject*>(object->collisionObject.get()) : nullptr) {
						GRAPHICS::MATERIAL::for_each_material_type(colObject->body, [&](RE::MATERIAL_ID& a_matID) {
							if (oldID != RE::MATERIAL_ID::kNone && a_matID != oldID) {
								return;
							}
							a_matID = newID;
						});
					}
				} else {
					RE::BSVisit::TraverseScenegraphCollision(root, [&](const RE::bhkNiCollisionObject* a_col) -> RE::BSVisit::BSVisitControl {
						GRAPHICS::MATERIAL::for_each_material_type(a_col->body, [&](RE::MATERIAL_ID& a_matID) {
							if (oldID != RE::MATERIAL_ID::kNone && a_matID != oldID) {
								return;
							}
							a_matID = newID;
						});
						return RE::BSVisit::BSVisitControl::kContinue;
					});
				}
			}
		}
	}

	inline void StopAllShaders(STATIC_ARGS, RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			processLists->StopAllMagicEffects(*a_ref);
		}
	}

	inline void StopArtObject(STATIC_ARGS, RE::TESObjectREFR* a_ref, const RE::BGSArtObject* a_art)
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
			processLists->ForEachModelEffect([&](RE::ModelReferenceEffect* a_modelEffect) {
				if (a_modelEffect->target == handle && a_modelEffect->artObject == a_art) {
					a_modelEffect->finished = true;
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
		BIND(CastEx);
		BIND(FindFirstItemInList);
		BIND(GetActivateChildren);
		BIND(GetActiveGamebryoAnimation);
		BIND(GetActiveMagicEffects);
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
		BIND(SetKey);
		BIND(SetLinkedRef);
		BIND(SetMaterialType);
		BIND(StopAllShaders);
		BIND(StopArtObject);

		logger::info("Registered object reference functions"sv);
	}
}
