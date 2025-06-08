#include "Game/HookedEventHandler.h"
#include "Serialization/EventHolder.h"

namespace Event
{
	namespace BooksRead
	{
		struct Read
		{
			static bool thunk(RE::TESObjectBOOK* a_this, RE::TESObjectREFR* a_reader)
			{
				const auto result = func(a_this, a_reader);
				if (a_this && a_reader->IsPlayerRef()) {
					GameEventHolder::GetSingleton()->booksRead.QueueEvent(a_this);
				}
				return result;
			}

			static inline REL::Relocation<decltype(thunk)> func;
		};

		void Install()
		{
			REL::Relocation<std::uintptr_t> bookMenu{ RELOCATION_ID(50122, 51053), OFFSET_3(0x22D, 0x231, 0x295) };
			stl::write_thunk_call<Read>(bookMenu.address());

			logger::info("Hooked Book Read"sv);
		}
	}

	namespace MagicEffectApply
	{
		struct MagicTargetApply
		{
			static bool thunk(RE::MagicTarget* a_this, RE::MagicTarget::AddTargetData* a_data)
			{
				const bool hasAppliedEffect = func(a_this, a_data);

				if (const auto target = a_this && a_data ? a_this->GetTargetStatsObject() : nullptr; target) {
					const auto effect = a_data->effect;
					if (const auto baseEffect = effect ? effect->baseEffect : nullptr; baseEffect) {
						GameEventHolder::GetSingleton()->magicApply.QueueEvent(
							target,
							[=](const Filter::MagicEffectApply& a_filter, bool a_match) {  //capture by reference [&] bad
								return a_match == a_filter.PassesFilter(baseEffect);
							},
							a_data->caster, baseEffect, a_data->magicItem, hasAppliedEffect);
					}
				}

				return hasAppliedEffect;
			}

			static inline REL::Relocation<decltype(thunk)> func;
		};

		void Install()
		{
			REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(33742, 34526), OFFSET(0x1E8, 0x20B) };
			stl::write_thunk_call<MagicTargetApply>(target.address());

			logger::info("Hooked Magic Effect Apply"sv);
		}
	}

	namespace Hit
	{
		namespace Magic
		{
			struct SendHitEvent
			{
				static void thunk(RE::BSTEventSource<RE::TESHitEvent>& a_source, RE::TESHitEvent& a_event)
				{
					using HitFlag = RE::TESHitEvent::Flag;

					const auto aggressor = a_event.cause.get();
					const auto target = a_event.target.get();
					const auto source = RE::TESForm::LookupByID(a_event.source);
					const auto projectile = RE::TESForm::LookupByID<RE::BGSProjectile>(a_event.projectile);

					if (target) {
						const auto powerAttack = a_event.flags.any(RE::TESHitEvent::Flag::kPowerAttack);
						const auto sneakAttack = a_event.flags.any(RE::TESHitEvent::Flag::kSneakAttack);
						const auto bashAttack = a_event.flags.any(RE::TESHitEvent::Flag::kBashAttack);
						const auto hitBlocked = a_event.flags.any(RE::TESHitEvent::Flag::kHitBlocked);

						GameEventHolder::GetSingleton()->onHit.QueueEvent(
							target,
							[=](const Filter::Hit& a_filter, bool a_match) {
								return a_match == a_filter.PassesFilter(aggressor, source, projectile, powerAttack, sneakAttack, bashAttack, hitBlocked);
							},
							aggressor, source, projectile, powerAttack, sneakAttack, bashAttack, hitBlocked);
					}

					if (aggressor) {
						GameEventHolder::GetSingleton()->magicHit.QueueEvent(aggressor, target, source, projectile);  // NOLINT(readability-suspicious-call-argument)
					}

					func(a_source, a_event);
				}

				static inline REL::Relocation<decltype(thunk)> func;
			};

			void Install()
			{
				REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(37832, 38786), OFFSET(0x1C3, 0x29B) };
				stl::write_thunk_call<SendHitEvent>(target.address());

				logger::info("Hooked Magic Hit"sv);
			}
		}

		namespace Weapon
		{
			namespace Actor
			{
				REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(37633, 38586), OFFSET(0x16A, 0xFA) };

				struct SendHitEvent
				{
#ifdef SKYRIM_AE
					// SendHitEventStruct was inlined
					static void thunk(RE::AIProcess* a_targetProcess, RE::HitData& a_data)
					{
						func(a_targetProcess, a_data);

						const auto hitTarget = a_targetProcess ? a_targetProcess->GetUserData() : nullptr;

						const auto aggressor = a_data.aggressor.get();
						const auto source = a_data.weapon ? a_data.weapon : RE::TESForm::LookupByID<RE::TESObjectWEAP>(0x1F4);  //unarmed

						if (hitTarget) {
							const auto powerAttack = a_data.flags.any(RE::HitData::Flag::kPowerAttack);
							const auto sneakAttack = a_data.flags.any(RE::HitData::Flag::kSneakAttack);
							const auto bashAttack = a_data.flags.any(RE::HitData::Flag::kBash, RE::HitData::Flag::kTimedBash);
							const auto hitBlocked = a_data.flags.any(RE::HitData::Flag::kBlocked, RE::HitData::Flag::kBlockWithWeapon);

							GameEventHolder::GetSingleton()->onHit.QueueEvent(
								hitTarget,
								[=](const Filter::Hit& a_filter, bool a_match) {
									return a_match == a_filter.PassesFilter(aggressor.get(), source, nullptr, powerAttack, sneakAttack, bashAttack, hitBlocked);
								},
								aggressor.get(), source, nullptr, powerAttack, sneakAttack, bashAttack, hitBlocked);
						}

						if (aggressor && hitTarget) {
							GameEventHolder::GetSingleton()->weaponHit.QueueEvent(aggressor.get(), hitTarget, source, nullptr, a_data.flags.underlying());
						}
					}
#else
					static void thunk(RE::ScriptEventSourceHolder* a_holder,
						RE::NiPointer<RE::TESObjectREFR>&          a_target,
						RE::NiPointer<RE::TESObjectREFR>&          a_aggressor,
						RE::FormID                                 a_source,
						RE::FormID                                 a_projectile,
						RE::HitData&                               a_data)
					{
						const auto aggressor = a_aggressor.get();
						const auto hitTarget = a_target.get();
						const auto source = RE::TESForm::LookupByID(a_source);

						if (hitTarget) {
							const auto powerAttack = a_data.flags.any(RE::HitData::Flag::kPowerAttack);
							const auto sneakAttack = a_data.flags.any(RE::HitData::Flag::kSneakAttack);
							const auto bashAttack = a_data.flags.any(RE::HitData::Flag::kBash, RE::HitData::Flag::kTimedBash);
							const auto hitBlocked = a_data.flags.any(RE::HitData::Flag::kBlocked, RE::HitData::Flag::kBlockWithWeapon);

							GameEventHolder::GetSingleton()->onHit.QueueEvent(
								hitTarget,
								[=](const Filter::Hit& a_filter, bool a_match) {
									return a_match == a_filter.PassesFilter(aggressor, source, nullptr, powerAttack, sneakAttack, bashAttack, hitBlocked);
								},
								aggressor, source, nullptr, powerAttack, sneakAttack, bashAttack, hitBlocked);
						}

						if (aggressor) {
							GameEventHolder::GetSingleton()->weaponHit.QueueEvent(aggressor, hitTarget, source, nullptr, a_data.flags.underlying());
						}

						func(a_holder, a_target, a_aggressor, a_source, a_projectile, a_data);
					}
#endif
					static inline REL::Relocation<decltype(thunk)> func;
				};
			}

			namespace Static
			{
				REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(37674, 38628), OFFSET(0x6C7, 0x785) };

				struct SendHitEvent
				{
					static void thunk(RE::BSTEventSource<RE::TESHitEvent>& a_source, RE::TESHitEvent& a_event)
					{
						const auto aggressor = a_event.cause.get();
						const auto hitTarget = a_event.target.get();
						const auto source = RE::TESForm::LookupByID(a_event.source);

						if (hitTarget) {
							const auto powerAttack = a_event.flags.any(RE::TESHitEvent::Flag::kPowerAttack);
							const auto sneakAttack = a_event.flags.any(RE::TESHitEvent::Flag::kSneakAttack);
							const auto bashAttack = a_event.flags.any(RE::TESHitEvent::Flag::kBashAttack);
							const auto hitBlocked = a_event.flags.any(RE::TESHitEvent::Flag::kHitBlocked);

							GameEventHolder::GetSingleton()->onHit.QueueEvent(
								hitTarget,
								[=](const Filter::Hit& a_filter, bool a_match) {
									return a_match == a_filter.PassesFilter(aggressor, source, nullptr, powerAttack, sneakAttack, bashAttack, hitBlocked);
								},
								aggressor, source, nullptr, powerAttack, sneakAttack, bashAttack, hitBlocked);
						}

						if (aggressor) {
							GameEventHolder::GetSingleton()->weaponHit.QueueEvent(aggressor, hitTarget, source, nullptr, 0);  // NOLINT(readability-suspicious-call-argument)
						}

						func(a_source, a_event);
					}

					static inline REL::Relocation<decltype(thunk)> func;
				};
			}

			namespace Projectile
			{
				REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(43022, 44213) };

				struct SpawnCollisionEffects
				{
					static void thunk(RE::Projectile* a_this, RE::TESObjectREFR* a_collidee, const RE::NiPoint3& a_contactPoint, const RE::NiPoint3& a_contactNormal, RE::MATERIAL_ID a_material, bool a_rotateToProjectileDirection)
					{
						auto effectNotSpawned = a_this->flags.none(RE::Projectile::Flags::kAddedVisualEffectOnGround);
						func(a_this, a_collidee, a_contactPoint, a_contactNormal, a_material, a_rotateToProjectileDirection);
						if (effectNotSpawned && a_collidee) {
							RE::BGSProjectile* projectile = a_this->GetProjectileBase();

							RE::ActorPtr aggressor{};
							if (const auto actorCause = a_this->GetActorCause()) {
								aggressor = actorCause->actor.get();
							}

							RE::TESObjectREFRPtr hitTarget{};
							if (auto handle = a_collidee->CreateRefHandle()) {
								hitTarget = handle.get();
							}

							RE::TESForm* source{};
							if (a_this->weaponSource) {
								source = a_this->weaponSource;
							} else if (a_this->spell) {
								source = a_this->spell;
							}

							if (hitTarget) {
								constexpr auto powerAttack = false;
								const auto     sneakAttack = aggressor && aggressor->IsSneaking();  // Magic Sneak Attacks
								constexpr auto bashAttack = false;
								constexpr auto hitBlocked = false;

								GameEventHolder::GetSingleton()->onHit.QueueEvent(
									hitTarget.get(),
									[=](const Filter::Hit& a_filter, bool a_match) {
										return a_match == a_filter.PassesFilter(aggressor.get(), source, projectile, powerAttack, sneakAttack, bashAttack, hitBlocked);
									},
									aggressor.get(), source, projectile, powerAttack, sneakAttack, bashAttack, hitBlocked);
							}

							if (aggressor) {
								if (projectile && projectile->IsArrow()) {
									if (!hitTarget || !hitTarget->IsActor()) {  // actor version takes care of arrow hits
										GameEventHolder::GetSingleton()->weaponHit.QueueEvent(aggressor.get(), hitTarget.get(), source, projectile, 0);
									}
								}
								GameEventHolder::GetSingleton()->projectileHit.QueueEvent(aggressor.get(), hitTarget.get(), source, projectile);
							}
						}
					}

					static inline REL::Relocation<decltype(thunk)> func;
				};
			}

			void Install()
			{
				stl::write_thunk_call<Actor::SendHitEvent>(Actor::target.address());
				stl::write_thunk_call<Static::SendHitEvent>(Static::target.address());
				stl::hook_function_prologue<Projectile::SpawnCollisionEffects, OFFSET(6, 7)>(Projectile::target.address());

				logger::info("Hooked Weapon Hit"sv);
			}
		}
	}

	namespace FallLongDistance
	{
		struct CalcDoDamage
		{
			static float thunk(RE::Actor* a_this, float a_fallDistance, float a_defaultMult)
			{
				const auto fallDamage = func(a_this, a_fallDistance, a_defaultMult);
				if (fallDamage > 0.0f) {
					GameEventHolder::GetSingleton()->actorFallLongDistance.QueueEvent(a_this, a_fallDistance, fallDamage);
				}
				return fallDamage;
			}

			static inline REL::Relocation<decltype(thunk)> func;
		};

		void Install()
		{
			REL::Relocation<std::uintptr_t> take_ragdoll_damage{ RELOCATION_ID(36346, 37336), 0x35 };
			stl::write_thunk_call<CalcDoDamage>(take_ragdoll_damage.address());

			REL::Relocation<std::uintptr_t> process_movefinish_event{ RELOCATION_ID(36973, 37998), OFFSET(0xAE, 0xAB) };
			stl::write_thunk_call<CalcDoDamage>(process_movefinish_event.address());

			logger::info("Hooked Fall Damage"sv);
		}
	}

	namespace FastTravel
	{
		template <class T>
		RE::TESObjectREFR* GetMapMarkerFromObject(RE::TESObjectREFR* a_refr)
		{
			if (const auto marker = a_refr ? a_refr->extraList.GetByType<RE::ExtraMapMarker>() : nullptr) {
				if (const auto mapData = marker->mapData) {
					logger::debug("Found candidate map marker {} {}", mapData->locationName.GetFullName(), mapData->flags.any(RE::MapMarkerData::Flag::kCanTravelTo));
					logger::debug("Found mapmarker match for {} target {} {} ({:X})", typeid(T).name(), mapData->locationName.GetFullName(), mapData->flags.any(RE::MapMarkerData::Flag::kCanTravelTo), a_refr->GetFormID());
					return a_refr;
				}
			}
			return nullptr;
		}

		RE::TESObjectREFR* GetMapMarkerObject(const RE::NiPointer<RE::TESObjectREFR>& a_refr)
		{
			return GetMapMarkerFromObject<RE::TESObjectREFR>(a_refr.get());
		}

		RE::TESObjectREFR* GetMapMarkerObject(const RE::FormID a_formID)
		{
			auto* refr = RE::TESForm::LookupByID<RE::TESObjectREFR>(a_formID);
			return GetMapMarkerFromObject<RE::FormID>(refr);
		}

		RE::TESObjectREFR* GetMapMarkerObject(const char* a_name)
		{
			const auto& mapMarkers = RE::PlayerCharacter::GetSingleton()->currentMapMarkers;

			for (auto& mapMarker : mapMarkers) {
				const auto refr = mapMarker.get().get();
				const auto marker = refr ? refr->extraList.GetByType<RE::ExtraMapMarker>() : nullptr;
				if (marker && marker->mapData && string::iequals(marker->mapData->locationName.GetFullName(), a_name)) {
					return GetMapMarkerFromObject<const char*>(refr);
				}
			}
			return nullptr;
		}

		struct ChangeFastTravelTarget
		{
			static void thunk(RE::FastTravelConfirmCallback* a_this, RE::IMessageBoxCallback::Message a_message)
			{
				if (a_message == RE::IMessageBoxCallback::Message::kUnk1) {
					const auto refr = GetMapMarkerObject(a_this->mapMenu->mapMarker.get());
					GameEventHolder::GetSingleton()->fastTravelConfirmed.QueueEvent(refr);

					const auto xMapMarker = refr ? refr->extraList.GetByType<RE::ExtraMapMarker>() : nullptr;
					const auto name = xMapMarker && xMapMarker->mapData ? xMapMarker->mapData->locationName.GetFullName() : "Unknown";
					const auto formID = refr ? refr->GetFormID() : 0;
					logger::debug("Found Fast Travel Confirmed target to {} ({:x})", name, formID);
				}

				if (disableFastTravel) {
					logger::debug("Fast Travel is disabled; cancelling trip");

					func(a_this, RE::IMessageBoxCallback::Message::kUnk0);
					RE::UIMessageQueue::GetSingleton()->AddMessage(RE::MapMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
					return;
				}

				const auto start = std::chrono::steady_clock::now();
				if (!newDestination && defaultTimeout > 0.0f) {
					logger::debug("Waiting for newDestination for {:.2f} seconds", defaultTimeout);
				}
				while (defaultTimeout > 0.0f) {
					std::chrono::duration<float> elapsed_seconds = std::chrono::steady_clock::now() - start;
					if (newDestination) {
						logger::debug("newDestination received after {:.2f} seconds", elapsed_seconds.count());
						break;
					}
					if (elapsed_seconds.count() > defaultTimeout) {
						logger::debug("newDestination not received after {:.2f} seconds; proceeding", elapsed_seconds.count());
						break;
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}

				if (a_this->mapMenu && newDestination) {
					a_this->mapMenu->mapMarker.reset();
					a_this->mapMenu->mapMarker = RE::ObjectRefHandle(newDestination);

					const auto refr = GetMapMarkerObject(a_this->mapMenu->mapMarker.get());
					const auto xMapMarker = refr ? refr->extraList.GetByType<RE::ExtraMapMarker>() : nullptr;
					const auto name = xMapMarker && xMapMarker->mapData ? xMapMarker->mapData->locationName.GetFullName() : "Unknown";
					const auto formID = refr ? refr->GetFormID() : 0;
					logger::debug("Changed Fast Travel target to {} ({:X})", name, formID);
				}

				func(a_this, a_message);
			}

			static inline REL::Relocation<decltype(thunk)> func;

			static inline RE::TESObjectREFR* newDestination{ nullptr };
			static inline float              defaultTimeout{ 0.0f };
			static inline bool               disableFastTravel{ false };
		};

		struct GetFastTravelTarget
		{
			// BSString::SPrintF
			static int thunk(RE::BSString* a_buffer, const char* a_template, const char* a_target)
			{
				if (a_target) {
					const auto refr = GetMapMarkerObject(a_target);
					const auto formID = refr ? refr->GetFormID() : 0;
					logger::info("Found Fast Travel Prompt target to {} ({:X})", a_target, formID);

					ChangeFastTravelTarget::newDestination = nullptr;
					GameEventHolder::GetSingleton()->fastTravelPrompt.QueueEvent(GetMapMarkerObject(a_target));
				}
				return func(a_buffer, a_template, a_target);
			}

			static inline REL::Relocation<decltype(thunk)> func;
		};

		bool SetFastTravelDisabled(const bool a_disable)
		{
			if (ChangeFastTravelTarget::disableFastTravel != a_disable) {
				logger::debug("Set Fast Travel Disabled {} -> {}", ChangeFastTravelTarget::disableFastTravel, a_disable);
				ChangeFastTravelTarget::disableFastTravel = a_disable;
			}
			return ChangeFastTravelTarget::disableFastTravel;
		}

		float SetFastTravelWaitTimeout(const float a_timeout)
		{
			if (ChangeFastTravelTarget::defaultTimeout != a_timeout) {
				logger::debug("Set Fast Travel Wait Timeout {:.2f} -> {:.2f}", ChangeFastTravelTarget::defaultTimeout, a_timeout);

				ChangeFastTravelTarget::defaultTimeout = a_timeout;
			}
			return ChangeFastTravelTarget::defaultTimeout;
		}

		bool SetFastTravelTarget(RE::TESObjectREFR* a_refr)
		{
			ChangeFastTravelTarget::newDestination = a_refr;

			if (const auto newDestination = ChangeFastTravelTarget::newDestination) {
				if (const auto mapmarker = newDestination->extraList.GetByType<RE::ExtraMapMarker>(); mapmarker && mapmarker->mapData) {
					const auto name = mapmarker->mapData->locationName.GetFullName();
					logger::debug("Set new Fast Travel target {}", name);
				}
			} else {
				logger::debug("Cleared Fast Travel target");
			}

			return ChangeFastTravelTarget::newDestination != nullptr;
		}

		bool SetFastTravelTarget(const char* a_name)
		{
			return SetFastTravelTarget(GetMapMarkerObject(a_name));
		}

		bool SetFastTravelTarget(const RE::FormID a_formID)
		{
			return SetFastTravelTarget(GetMapMarkerObject(a_formID));
		}

#ifdef SKYRIMVR
		struct FastTravelEndEvent
		{
			static void thunk(RE::AIProcess* a_process)
			{
				func(a_process);
				GameEventHolder::GetSingleton()->fastTravelEnd.QueueEvent(afTravelGameTimeHours);
			}

			static inline REL::Relocation<decltype(thunk)> func;

			static inline float afTravelGameTimeHours{ 0.0f };
		};

		struct Calendar__Update  // use this hook to calculate afTravelGameTimeHours
		{
			static void thunk(RE::Calendar* a_calendar, float a_seconds)
			{
				/*SSE function for calculating afTravelGameTimeHours occurs around this call
				Calendar__FUN_1405a6230(g_Calendar,gameDaysPassedPreTravel,(longlong)plVar18,uVar22,uVar23,pPVar27);
				gameDaysPassedPostTravel = Calendar::GetGameDaysPassed(g_Calendar); // 35408 , 0x1405adbb0 VR
				afTravelGameTimeHours = (gameDaysPassedPostTravel - gameDaysPassedPreTravel)* 24.0 ;
				*/
				const auto GameDaysPassedPreTravel = a_calendar->gameDaysPassed->value;

				func(a_calendar, a_seconds);  // travel function will modify calendar

				const auto gameDaysPassedPostTravel = a_calendar->gameDaysPassed;
				const auto result = gameDaysPassedPostTravel ? (gameDaysPassedPostTravel->value - GameDaysPassedPreTravel) * 24.0f : 0.0f;
				FastTravelEndEvent::afTravelGameTimeHours = result;
			}

			static inline REL::Relocation<decltype(thunk)> func;
		};

#endif
		void Install()
		{
			stl::write_vfunc<RE::FastTravelConfirmCallback, 0x1, ChangeFastTravelTarget>();

			REL::Relocation<std::uintptr_t> map_click{ RELOCATION_ID(52208, 53095), OFFSET_3(0x342, 0x3A6, 0x3D9) };  // BSString::unknown has potential target as string as param 3
			stl::write_thunk_call<GetFastTravelTarget>(map_click.address());

			logger::info("Hooked Fast Travel Start"sv);

#ifdef SKYRIMVR  // replicate Event OnPlayerFastTravelEnd(float afTravelGameTimeHours)

			REL::Relocation<std::uintptr_t> FastTravelEnd_event{ REL::ID(39373), 0xA22 };  //last function call before SSE call FUN_140663690(ActorProcess *param_1). Should trigger right on exit
			stl::write_thunk_call<FastTravelEndEvent>(FastTravelEnd_event.address());

			REL::Relocation<std::uintptr_t> calculateTravelTime{ REL::ID(39373), 0x29F };  // hook Calendar__FUN_1405a6230 to calculate travel time. SSE calculates around this function
			stl::write_thunk_call<Calendar__Update>(calculateTravelTime.address());

			logger::info("Hooked Fast Travel End for VR"sv);
#endif
		}
	}

	namespace ItemCrafted
	{
		struct StoryItemCraft
		{
			RE::ObjectRefHandle objectHandle;  // 00
			RE::BGSLocation*    location;      // 08
			RE::TESForm*        form;          // 10
		};

		static_assert(sizeof(StoryItemCraft) == 0x18);

		struct StoryCraftItem
		{
			static StoryItemCraft* thunk(StoryItemCraft* a_event, RE::TESObjectREFR* a_refr, RE::BGSLocation* a_loc, RE::TESForm* a_form)
			{
				GameEventHolder::GetSingleton()->itemCrafted.QueueEvent(a_refr, a_loc, a_form);

				return func(a_event, a_refr, a_loc, a_form);
			}

			static inline REL::Relocation<decltype(thunk)> func;
		};

		void Install()
		{
			REL::Relocation<std::uintptr_t> smithing{ RELOCATION_ID(50477, 51370), OFFSET(0x17D, 0x1B3) };
			stl::write_thunk_call<StoryCraftItem>(smithing.address());

			REL::Relocation<std::uintptr_t> tempering{ RELOCATION_ID(50476, 51369), OFFSET(0x11E, 0x227) };
			stl::write_thunk_call<StoryCraftItem>(tempering.address());

			REL::Relocation<std::uintptr_t> enchanting{ RELOCATION_ID(50450, 51355), OFFSET(0x2FC, 0x2FA) };
			stl::write_thunk_call<StoryCraftItem>(enchanting.address());

			REL::Relocation<std::uintptr_t> alchemy{ RELOCATION_ID(50449, 51354), OFFSET(0x29E, 0x296) };
			stl::write_thunk_call<StoryCraftItem>(alchemy.address());

			logger::info("Hooked Item Crafted"sv);
		}
	}

	namespace ObjectPoisoned
	{
		struct PoisonObject
		{
			static void thunk(RE::InventoryEntryData* a_this, RE::AlchemyItem* a_poison, std::uint32_t a_count)
			{
				func(a_this, a_poison, a_count);

				if (auto object = a_this->GetObject()) {
					GameEventHolder::GetSingleton()->objectPoisoned.QueueEvent(object, a_poison, a_count);
				}
			}

			static inline REL::Relocation<decltype(thunk)> func;
		};

		void Install()
		{
			REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(15786, 16024) };
			stl::hook_function_prologue<PoisonObject, 5>(target.address());

			logger::info("Hooked Poison Object"sv);
		}
	}

	namespace Reanimate
	{
		struct Start
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

					const auto caster = a_this->caster.get();
					if (caster) {
						GameEventHolder::GetSingleton()->actorReanimateStart.QueueEvent(zombie, caster.get());
					}
				}
			}

			static inline REL::Relocation<decltype(thunk)> func;
		};

		struct Stop
		{
			static void thunk(RE::ReanimateEffect* a_this)
			{
				const auto zombiePtr = a_this->commandedActor.get();
				const auto caster = a_this->caster.get();
				if (zombiePtr && caster) {
					const auto zombie = zombiePtr.get();
					GameEventHolder::GetSingleton()->actorReanimateStop.QueueEvent(zombie, caster.get());
				}

				func(a_this);
			}

			static inline REL::Relocation<decltype(thunk)> func;
		};

		void Install()
		{
			stl::write_vfunc<RE::ReanimateEffect, 0x14, Start>();
			stl::write_vfunc<RE::ReanimateEffect, 0x15, Stop>();

			logger::info("Hooked Actor Reanimate Start"sv);
			logger::info("Hooked Actor Reanimate Stop"sv);
		}
	}

	namespace Resurrect
	{
		struct Resurrect
		{
			static void thunk(RE::Character* a_this, bool a_resetInventory, bool a_attach3D)
			{
				func(a_this, a_resetInventory, a_attach3D);

				GameEventHolder::GetSingleton()->actorResurrect.QueueEvent(a_this, a_resetInventory);
			}

			static inline REL::Relocation<decltype(thunk)> func;
		};

		void Install()
		{
			stl::write_vfunc<RE::Character,
#ifndef SKYRIMVR
				0x0AB,
#else
				0x0AD,
#endif
				Resurrect>();

			logger::info("Hooked Actor Resurrect"sv);
		}
	}

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

					GameEventHolder::GetSingleton()->weatherChange.QueueEvent(lastWeather, currentWeather);
				}

				func(a_region, a_currentWeather);
			}

			static inline REL::Relocation<decltype(thunk)> func;

		private:
			static inline RE::TESWeather* currentWeather{ nullptr };
		};

		void Install()
		{
			REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(25684, 26231), OFFSET(0x44F, 0x46C) };
			stl::write_thunk_call<SetCurrentWeather>(target.address());

			logger::info("Hooked Weather Change"sv);
		}
	}

	void RegisterHookEvents()
	{
		logger::info("{:*^30}", "HOOKED EVENTS"sv);

		BooksRead::Install();
		FallLongDistance::Install();
		ItemCrafted::Install();
		ObjectPoisoned::Install();
		Resurrect::Install();
		Reanimate::Install();
		Weather::Install();

		MagicEffectApply::Install();
		Hit::Magic::Install();
		Hit::Weapon::Install();

		FastTravel::Install();
	}
}
