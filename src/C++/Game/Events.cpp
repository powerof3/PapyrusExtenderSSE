#include "Game/Events.h"
#include "Serialization/Events.h"

namespace Events
{
	namespace Script
	{
		EventResult EventHandler::ProcessEvent(const RE::TESCellFullyLoadedEvent* a_event, RE::BSTEventSource<RE::TESCellFullyLoadedEvent>*)
		{
			if (!a_event || !a_event->cell) {
				return EventResult::kContinue;
			}

			OnCellFullyLoadedRegSet::GetSingleton()->QueueEvent(a_event->cell);

			return EventResult::kContinue;
		}

		EventResult EventHandler::ProcessEvent(const RE::TESQuestStartStopEvent* a_event, RE::BSTEventSource<RE::TESQuestStartStopEvent>*)
		{
			if (!a_event) {
				return EventResult::kContinue;
			}

			if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID); quest) {
				a_event->started ?
                    OnQuestStartRegMap::GetSingleton()->QueueEvent(a_event->formID, quest) :
                    OnQuestStopRegMap::GetSingleton()->QueueEvent(a_event->formID, quest);
			}

			return EventResult::kContinue;
		}

		EventResult EventHandler::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*)
		{
			if (!a_event) {
				return EventResult::kContinue;
			}

			if (const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(a_event->formID); quest) {
				OnQuestStageRegMap::GetSingleton()->QueueEvent(a_event->formID, quest, a_event->stage);
			}

			return EventResult::kContinue;
		}

		EventResult EventHandler::ProcessEvent(const RE::TESObjectLoadedEvent* a_event, RE::BSTEventSource<RE::TESObjectLoadedEvent>*)
		{
			if (!a_event) {
				return EventResult::kContinue;
			}

			const auto object = RE::TESForm::LookupByID<RE::TESObjectREFR>(a_event->formID);
			const auto base = object ? object->GetBaseObject() : nullptr;

			if (base) {
				auto baseType = base->GetFormType();
				a_event->loaded ?
                    OnObjectLoadedRegMap::GetSingleton()->QueueEvent(baseType, object, baseType) :
                    OnObjectUnloadedRegMap::GetSingleton()->QueueEvent(baseType, object, baseType);
			}

			return EventResult::kContinue;
		}

		EventResult EventHandler::ProcessEvent(const RE::TESGrabReleaseEvent* a_event, RE::BSTEventSource<RE::TESGrabReleaseEvent>*)
		{
			if (!a_event) {
				return EventResult::kContinue;
			}

			if (const auto object = a_event->ref.get(); object) {
				a_event->grabbed ?
                    OnGrabRegSet::GetSingleton()->QueueEvent(object) :
                    OnReleaseRegSet::GetSingleton()->QueueEvent(object);
			}

			return EventResult::kContinue;
		}
	}

	namespace Story
	{
		EventResult EventHandler::ProcessEvent(const RE::ActorKill::Event* a_event, RE::BSTEventSource<RE::ActorKill::Event>*)
		{
			if (!a_event) {
				return EventResult::kContinue;
			}

			const auto victim = a_event->victim;
			const auto killer = a_event->killer;

			if (victim) {
				OnActorKillRegSet::GetSingleton()->QueueEvent(victim, killer);
			}

			return EventResult::kContinue;
		}

		EventResult EventHandler::ProcessEvent(const RE::BooksRead::Event* a_event, RE::BSTEventSource<RE::BooksRead::Event>*)
		{
			if (!a_event) {
				return EventResult::kContinue;
			}

			if (const auto book = a_event->book; book) {
				OnBooksReadRegSet::GetSingleton()->QueueEvent(book);
			}

			return EventResult::kContinue;
		}

		EventResult EventHandler::ProcessEvent(const RE::CriticalHit::Event* a_event, RE::BSTEventSource<RE::CriticalHit::Event>*)
		{
			if (!a_event) {
				return EventResult::kContinue;
			}

			const auto agressor = a_event->aggressor;
			const auto agressorActor = agressor ? agressor->As<RE::Actor>() : nullptr;

			const auto weapon = a_event->weapon;

			if (agressorActor && weapon) {
				OnCriticalHitRegSet::GetSingleton()->QueueEvent(agressorActor, weapon, a_event->sneakHit);
			}

			return EventResult::kContinue;
		}

		EventResult EventHandler::ProcessEvent(const RE::DisarmedEvent::Event* a_event, RE::BSTEventSource<RE::DisarmedEvent::Event>*)
		{
			if (!a_event) {
				return EventResult::kContinue;
			}

			const auto source = a_event->source;
			const auto target = a_event->target;

			if (source && target) {
				OnDisarmedRegSet::GetSingleton()->QueueEvent(source, target);
			}

			return EventResult::kContinue;
		}

		EventResult EventHandler::ProcessEvent(const RE::DragonSoulsGained::Event* a_event, RE::BSTEventSource<RE::DragonSoulsGained::Event>*)
		{
			if (!a_event) {
				return EventResult::kContinue;
			}

			OnDragonSoulsGainedRegSet::GetSingleton()->QueueEvent(a_event->souls);

			return EventResult::kContinue;
		}

		EventResult EventHandler::ProcessEvent(const RE::TESHarvestedEvent::ItemHarvested* a_event, RE::BSTEventSource<RE::TESHarvestedEvent::ItemHarvested>*)
		{
			if (!a_event) {
				return EventResult::kContinue;
			}

			if (const auto produce = a_event->produceItem; produce) {
				OnItemHarvestedRegSet::GetSingleton()->QueueEvent(produce);
			}

			return EventResult::kContinue;
		}

		EventResult EventHandler::ProcessEvent(const RE::LevelIncrease::Event* a_event, RE::BSTEventSource<RE::LevelIncrease::Event>*)
		{
			if (!a_event) {
				return EventResult::kContinue;
			}

			OnLevelIncreaseRegSet::GetSingleton()->QueueEvent(a_event->newLevel);

			return EventResult::kContinue;
		}

		EventResult EventHandler::ProcessEvent(const RE::LocationDiscovery::Event* a_event, RE::BSTEventSource<RE::LocationDiscovery::Event>*)
		{
			if (!a_event) {
				return EventResult::kContinue;
			}

			if (const auto data = a_event->mapMarkerData; data) {
				OnLocationDiscoveryRegSet::GetSingleton()->QueueEvent(data->locationName.fullName, a_event->worldspaceID);
			}

			return EventResult::kContinue;
		}

		EventResult EventHandler::ProcessEvent(const RE::ShoutAttack::Event* a_event, RE::BSTEventSource<RE::ShoutAttack::Event>*)
		{
			if (!a_event) {
				return EventResult::kContinue;
			}

			if (const auto shout = a_event->shout; shout) {
				OnShoutAttackRegSet::GetSingleton()->QueueEvent(shout);
			}

			return EventResult::kContinue;
		}

		EventResult EventHandler::ProcessEvent(const RE::SkillIncrease::Event* a_event, RE::BSTEventSource<RE::SkillIncrease::Event>*)
		{
			if (!a_event) {
				return EventResult::kContinue;
			}

			const auto avList = RE::ActorValueList::GetSingleton();
			const auto avInfo = avList ? avList->GetActorValue(a_event->actorValue) : nullptr;

			if (avInfo) {
				OnSkillIncreaseRegSet::GetSingleton()->QueueEvent(avInfo->enumName);
			}

			return EventResult::kContinue;
		}

		EventResult EventHandler::ProcessEvent(const RE::SoulsTrapped::Event* a_event, RE::BSTEventSource<RE::SoulsTrapped::Event>*)
		{
			if (!a_event) {
				return EventResult::kContinue;
			}

			const auto trapper = a_event->trapper;
			const auto target = a_event->target;

			if (trapper && target) {
				OnSoulsTrappedRegSet::GetSingleton()->QueueEvent(target, trapper);
			}

			return EventResult::kContinue;
		}

		EventResult EventHandler::ProcessEvent(const RE::SpellsLearned::Event* a_event, RE::BSTEventSource<RE::SpellsLearned::Event>*)
		{
			if (!a_event) {
				return EventResult::kContinue;
			}

			if (const auto spell = a_event->spell; spell) {
				OnSpellsLearnedRegSet::GetSingleton()->QueueEvent(spell);
			}

			return EventResult::kContinue;
		}
	}

	namespace Game
	{
		namespace FallLongDistance
		{
			struct CalcDoDamage
			{
				static float thunk(RE::Actor* a_this, float a_fallDistance, float a_defaultMult)
				{
					auto fallDamage = func(a_this, a_fallDistance, a_defaultMult);
					if (fallDamage > 0.0) {
						OnActorFallLongDistanceRegSet::GetSingleton()->QueueEvent(a_this, a_this, a_fallDistance, fallDamage);
					}
					return fallDamage;
				}
				static inline REL::Relocation<decltype(&thunk)> func;
			};

			inline void Install()
			{
				REL::Relocation<std::uintptr_t> target{ REL::ID(36346) };
				stl::write_thunk_call<CalcDoDamage>(target.address() + 0x35);

				logger::info("Hooked Fall Damage"sv);
			}
		}
		
		namespace Resurrect
		{
			struct Resurrect
			{
				static void thunk(RE::Character* a_this, bool a_resetInventory, bool a_attach3D)
				{
					func(a_this, a_resetInventory, a_attach3D);

					OnActorResurrectRegSet::GetSingleton()->QueueEvent(a_this, a_this, a_resetInventory);
				}
				static inline REL::Relocation<decltype(&thunk)> func;
			};

			void Install()
			{
				stl::write_vfunc<RE::Character, 0x0AB, Resurrect>();			
				
				logger::info("Hooked Actor Resurrect"sv);
			}
		}

		namespace ReanimateStart
		{
			struct ReanimateStart
			{
				static void thunk(RE::ReanimateEffect* a_this)
				{
					func(a_this);

					if (a_this->flags.none(RE::ActiveEffect::Flag::kDispelled)) {  //effect can get dispelled in original func

						const auto zombiePtr = a_this->commandedActor.get();
						const auto zombie = zombiePtr.get();
						if (!zombie) {
							return;
						}

						if (zombie->boolBits.all(RE::Actor::BOOL_BITS::kParalyzed)) {
							const auto root = zombie->Get3D(false);
							const auto charController = zombie->GetCharController();
							if (root && charController) {
								zombie->boolBits.reset(RE::Actor::BOOL_BITS::kParalyzed);

								root->UpdateRigidConstraints(false);

								std::uint32_t filterInfo = 0;
								charController->GetCollisionFilterInfo(filterInfo);

								root->SetCollisionLayerAndGroup(RE::COL_LAYER::kDeadBip, filterInfo >> 16);
							}
						}

						const auto casterPtr = a_this->caster.get();
						const auto caster = casterPtr.get();
						if (caster) {
							OnActorReanimateStartRegSet::GetSingleton()->QueueEvent(zombie, zombie, caster);
						}
					}
				}
				static inline REL::Relocation<decltype(&thunk)> func;
			};

			void Install()
			{
				stl::write_vfunc<RE::ReanimateEffect, 0x14, ReanimateStart>();
				
				logger::info("Hooked Actor Reanimate Start"sv);
			}
		};

		namespace ReanimateStop
		{
			struct ReanimateStop
			{
				static void thunk(RE::ReanimateEffect* a_this)
				{
					const auto zombiePtr = a_this->commandedActor.get();
					const auto zombie = zombiePtr.get();
					if (zombie) {
						const auto casterPtr = a_this->caster.get();
						const auto caster = casterPtr.get();
						if (caster) {
							OnActorReanimateStopRegSet::GetSingleton()->QueueEvent(zombie, zombie, caster);
						}
					}

					func(a_this);
				}
				static inline REL::Relocation<decltype(&thunk)> func;
			};

			void Install()
			{
				stl::write_vfunc<RE::ReanimateEffect, 0x15, ReanimateStop>();
				
				logger::info("Hooked Actor Reanimate Stop"sv);
			}
		}

		namespace MagicEffectApply
		{
			struct MagicTargetApply
			{
				static bool thunk(RE::MagicTarget* a_this, RE::MagicTarget::CreationData* a_data)
				{
					auto result = func(a_this, a_data);

					const auto target = a_this ? a_this->GetTargetStatsObject() : nullptr;
					if (target && a_data) {
						const auto effect = a_data->effect;
						const auto baseEffect = effect ? effect->baseEffect : nullptr;
						if (baseEffect) {
							OnMagicEffectApplyRegMap::GetSingleton()->QueueEvent(target, baseEffect, a_data->caster, baseEffect, a_data->magicItem, result);
						}
					}

					return result;
				}
				static inline REL::Relocation<decltype(thunk)> func;
			};

			static void Install()
			{
				REL::Relocation<std::uintptr_t> target{ REL::ID(33742), 0x1E8 };
				stl::write_thunk_call<MagicTargetApply>(target.address());

				logger::info("Hooked Magic Effect Apply"sv);
			}
		}

		namespace MagicHit
		{
			struct SendHitEvent
			{
				static void thunk(RE::BSTEventSource<RE::TESHitEvent>& a_source, RE::TESHitEvent& a_event)
				{
					if (const auto aggressor = a_event.cause.get(); aggressor) {
						const auto target = a_event.target.get();
						const auto source = RE::TESForm::LookupByID(a_event.source);
						const auto projectile = RE::TESForm::LookupByID<RE::BGSProjectile>(a_event.projectile);

						OnMagicHitRegSet::GetSingleton()->QueueEvent(aggressor, target, source, projectile);
					}

					func(a_source, a_event);
				}
				static inline REL::Relocation<decltype(&thunk)> func;
			};

			void Install()
			{
				REL::Relocation<std::uintptr_t> target{ REL::ID(37832), 0x1C3 };
				stl::write_thunk_call<SendHitEvent>(target.address());

				logger::info("Hooked Magic Hit"sv);
			}
		}

		namespace WeaponHit
		{
			namespace Actor
			{
				REL::Relocation<std::uintptr_t> target{ REL::ID(37633), 0x16A };

				struct SendHitEvent
				{
					static void thunk(RE::ScriptEventSourceHolder* a_holder,
						RE::NiPointer<RE::TESObjectREFR>& a_target,
						RE::NiPointer<RE::TESObjectREFR>& a_aggressor,
						RE::FormID a_source,
						RE::FormID a_projectile,
						RE::HitData& a_data)
					{
						if (const auto aggressor = a_aggressor.get(); aggressor) {
							const auto hitTarget = a_target.get();
							const auto source = RE::TESForm::LookupByID(a_source);
							const auto flags = stl::to_underlying(a_data.flags);

							OnWeaponHitRegSet::GetSingleton()->QueueEvent(aggressor, hitTarget, source, nullptr, flags);
						}

						func(a_holder, a_target, a_aggressor, a_source, a_projectile, a_data);
					}
					static inline REL::Relocation<decltype(thunk)> func;
				};
			}

			namespace Static
			{
				REL::Relocation<std::uintptr_t> target{ REL::ID(37674), 0x6C7 };

				struct SendHitEvent
				{
					static void thunk(RE::BSTEventSource<RE::TESHitEvent>& a_source, RE::TESHitEvent& a_event)
					{
						if (const auto aggressor = a_event.cause.get(); aggressor) {
							const auto hitTarget = a_event.target.get();
							const auto source = RE::TESForm::LookupByID(a_event.source);
							OnWeaponHitRegSet::GetSingleton()->QueueEvent(aggressor, hitTarget, source, nullptr, 0);
						}
						func(a_source, a_event);
					}
					static inline REL::Relocation<decltype(thunk)> func;
				};
			}

			namespace Projectile
			{
				REL::Relocation<std::uintptr_t> target{ REL::ID(43022), 0x38D };

				struct SendHitEvent
				{
					static void thunk(RE::BSTEventSource<RE::TESHitEvent>& a_source, RE::TESHitEvent& a_event)
					{
						if (const auto aggressor = a_event.cause.get(); aggressor) {
							const auto hitTarget = a_event.target.get();
							const auto source = RE::TESForm::LookupByID(a_event.source);
							const auto projectile = RE::TESForm::LookupByID<RE::BGSProjectile>(a_event.projectile);

							if (projectile && projectile->data.types.all(RE::BGSProjectileData::Type::kArrow)) {
								OnWeaponHitRegSet::GetSingleton()->QueueEvent(aggressor, hitTarget, source, projectile, 0);
							}
							OnProjectileHitRegSet::GetSingleton()->QueueEvent(aggressor, hitTarget, source, projectile);
						}
						func(a_source, a_event);
					}
					static inline REL::Relocation<decltype(thunk)> func;
				};
			}

			inline void Install()
			{
				stl::write_thunk_call<Actor::SendHitEvent>(Actor::target.address());
				stl::write_thunk_call<Static::SendHitEvent>(Static::target.address());
				stl::write_thunk_call<Projectile::SendHitEvent>(Projectile::target.address());

				logger::info("Hooked Weapon Hit"sv);
			}
		}

		//got rid of asm hook, the fact that it worked in SE without issues was a miracle
		namespace Weather
		{
			struct SetCurrentWeather
			{
				static void thunk(RE::TESRegion* a_region, RE::TESWeather* a_currentWeather)
				{
					if (currentWeather != a_currentWeather) {
						currentWeather = a_currentWeather;

						const auto sky = RE::Sky::GetSingleton();
						const auto lastWeather = sky ? sky->lastWeather : nullptr;

						OnWeatherChangeRegSet::GetSingleton()->QueueEvent(lastWeather, currentWeather);
					}

					func(a_region, a_currentWeather);
				}
				static inline REL::Relocation<decltype(&thunk)> func;

			private:
				static inline RE::TESWeather* currentWeather;
			};

			void Install()
			{
				REL::Relocation<std::uintptr_t> target{ REL::ID(25684) };
				stl::write_thunk_call<SetCurrentWeather>(target.address() + 0x44F);

				logger::info("Hooked Weather Change"sv);
			}
		}

		void Register()
		{
			logger::info("{:*^30}", "HOOKED EVENTS"sv);

			FallLongDistance::Install();
			Resurrect::Install();
			ReanimateStart::Install();
			ReanimateStop::Install();
			MagicEffectApply::Install();
			WeaponHit::Install();
			MagicHit::Install();
			Weather::Install();
		}
	}

	void Register()
	{
		Script::EventHandler::Register();
		Story::EventHandler::Register();
	}
}
