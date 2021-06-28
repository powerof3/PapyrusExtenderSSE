#include "Hooks/EventHook.h"

#include "Serialization/Events.h"

namespace Hook
{
	using namespace Serialization::HookedEvents;

	namespace ActorResurrect
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
		}
	}

	namespace ActorReanimateStart
	{
		struct ReanimateStart
		{
			static void thunk(RE::ReanimateEffect* a_this)
			{
				func(a_this);

				if (a_this->flags.none(RE::ActiveEffect::Flag::kDispelled)) {  //effect can get dispelled in original func

					const auto zombiePtr = a_this->commandedActor.get();
					auto zombie = zombiePtr.get();

					if (!zombie) {
						return;
					}

					if (zombie->boolBits.all(RE::Actor::BOOL_BITS::kParalyzed)) {
						auto root = zombie->Get3D(false);
						auto charController = zombie->GetCharController();
						if (root && charController) {
							zombie->boolBits.reset(RE::Actor::BOOL_BITS::kParalyzed);
							root->SetRigidConstraints(false);
							std::uint32_t filterInfo = 0;
							charController->GetCollisionFilterInfo(filterInfo);
							root->UpdateRigidBodySettings(32, filterInfo >> 16);
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
		}
	};

	namespace ActorReanimateStop
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
		}
	}

	namespace MagicEffectApply
	{
		struct MagicTargetApply
		{
			static bool thunk(RE::MagicTarget* a_this, RE::MagicTarget::CreationData* a_data)
			{
				auto result = func(a_this, a_data);

				auto target = a_this->GetTargetStatsObject();
				auto effect = a_data ? a_data->effect : nullptr;
				auto baseEffect = effect ? effect->baseEffect : nullptr;

				if (target && baseEffect) {
					OnMagicEffectApplyRegMap::GetSingleton()->QueueEvent(target, baseEffect, a_data->caster, baseEffect, a_data->magicItem, result);
				}

				return result;
			}
			static inline REL::Relocation<decltype(thunk)> func;
		};

		static void Install()
		{
			REL::Relocation<std::uintptr_t> target{ REL::ID(33742), 0x1E8 };
			stl::write_thunk_call<MagicTargetApply>(target.address());
		}
	}

	namespace MagicHit
	{
		struct SendHitEvent
		{
			static void thunk(RE::BSTEventSource<RE::TESHitEvent>& a_source, RE::TESHitEvent& a_event)
			{
				if (auto aggressor = a_event.cause.get(); aggressor) {
					auto target = a_event.target.get();
					auto source = RE::TESForm::LookupByID(a_event.source);
					auto projectile = RE::TESForm::LookupByID<RE::BGSProjectile>(a_event.projectile);
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
		}
	}
	
	namespace WeaponHit
	{
		namespace Actor
		{
			REL::Relocation<std::uintptr_t> target{ REL::ID(37633), 0x16A };
			
			struct SendHitEvent
			{
				static void thunk(RE::ScriptEventSourceHolder* a_holder, RE::NiPointer<RE::TESObjectREFR>& a_target, RE::NiPointer<RE::TESObjectREFR>& a_aggressor, RE::FormID a_source, RE::FormID a_projectile, RE::HitData& a_data)
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
		}
	}

	class WeatherEvent
	{
	public:
		static void Install()
		{
			REL::Relocation<std::uintptr_t> target{ REL::ID(25684), 0x416 };

			struct Patch : Xbyak::CodeGenerator
			{
				Patch(std::uintptr_t a_func, std::uintptr_t a_target)
				{
					Xbyak::Label funcLbl;
					Xbyak::Label returnLbl;

					mov(dword[rbx + 0x1DC], eax);  // original code

					call(ptr[rip + funcLbl]);  // new function
					jmp(ptr[rip + returnLbl]);

					L(funcLbl);
					dq(a_func);

					L(returnLbl);
					dq(a_target + 0x6);  // next line
				}
			};

			Patch patch(reinterpret_cast<std::uintptr_t>(SendWeatherEvent), target.address());
			patch.ready();

			auto& trampoline = SKSE::GetTrampoline();
			trampoline.write_branch<6>(
				target.address(),
				trampoline.allocate(patch));
		}

	private:
		static void SendWeatherEvent()
		{
			const auto sky = RE::Sky::GetSingleton();
			const auto currentWeather = sky ? sky->currentWeather : nullptr;
			const auto lastWeather = sky ? sky->lastWeather : nullptr;
			
			OnWeatherChangeRegSet::GetSingleton()->QueueEvent(lastWeather, currentWeather);
		}
	};
	
	bool HookEvents()
	{
		logger::info("{:*^30}", "HOOKED EVENTS"sv);

		ActorResurrect::Install();
		logger::info("Hooked Actor Resurrect"sv);

		ActorReanimateStart::Install();
		ActorReanimateStop::Install();
		logger::info("Hooked Actor Reanimate"sv);

		WeatherEvent::Install();
		logger::info("Hooked Weather Change"sv);

		MagicEffectApply::Install();
		logger::info("Hooked Magic Effect Apply"sv);

		WeaponHit::Install();
		logger::info("Hooked Weapon Hit"sv);

		MagicHit::Install();
		logger::info("Hooked Magic Hit"sv);

		return true;
	}
};
