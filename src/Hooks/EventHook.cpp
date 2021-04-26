#include "Hooks/EventHook.h"

#include "Serialization/Events.h"


namespace Hook
{
	using namespace Serialization::HookedEvents;

	class ActorResurrect
	{
	public:
		static void Install()
		{
			REL::Relocation<std::uintptr_t> vtbl{ REL::ID(261397) };  //Character vtbl
			_Resurrect = vtbl.write_vfunc(0x0AB, Resurrect);
		}

	private:
		static void Resurrect(RE::Character* a_this, bool a_resetInventory, bool a_attach3D)
		{
			_Resurrect(a_this, a_resetInventory, a_attach3D);

			OnActorResurrectRegSet::GetSingleton()->QueueEvent(a_this, a_this, a_resetInventory);
		}

		using Resurrect_t = decltype(&RE::Character::Resurrect);  // 0AB
		static inline REL::Relocation<Resurrect_t> _Resurrect;
	};


	class ActorReanimateStart
	{
	public:
		static void Install()
		{
			REL::Relocation<std::uintptr_t> vtbl{ REL::ID(257879) };  //Reanimate vtbl
			_Reanimate = vtbl.write_vfunc(0x14, Reanimate);
		}

	private:
		static void Reanimate(RE::ReanimateEffect* a_this)
		{
			_Reanimate(a_this);

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

		using Reanimate_t = decltype(&RE::ReanimateEffect::Unk_14);  // 0AB
		static inline REL::Relocation<Reanimate_t> _Reanimate;
	};


	class ActorReanimateStop
	{
	public:
		static void Install()
		{
			REL::Relocation<std::uintptr_t> vtbl{ REL::ID(257879) };  //Reanimate vtbl
			_Reanimate = vtbl.write_vfunc(0x15, Reanimate);
		}

	private:
		static void Reanimate(RE::ReanimateEffect* a_this)
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

			_Reanimate(a_this);
		}

		using Reanimate_t = decltype(&RE::ReanimateEffect::Unk_15);  // 0AB
		static inline REL::Relocation<Reanimate_t> _Reanimate;
	};


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

			Patch patch(unrestricted_cast<std::uintptr_t>(SendWeatherEvent), target.address());
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
			if (sky) {
				const auto currentWeather = sky->currentWeather;
				const auto lastWeather = sky->lastWeather;
				if (currentWeather && lastWeather) {
					OnWeatherChangeRegSet::GetSingleton()->QueueEvent(lastWeather, currentWeather);
				}
			}
		}
	};


	class MagicEffectApply
	{
	public:
		static void Install()
		{
			auto& trampoline = SKSE::GetTrampoline();

			REL::Relocation<std::uintptr_t> ApplyMagicEffect{ REL::ID(33742) };
			_MagicTargetApply = trampoline.write_call<5>(ApplyMagicEffect.address() + 0x1E8, MagicTargetApply);
		}

	private:
		static bool MagicTargetApply(RE::MagicTarget* a_this, RE::MagicTarget::CreationData* a_data)
		{
			auto result = _MagicTargetApply(a_this, a_data);

			auto target = a_this->GetTargetStatsObject();
			auto effect = a_data ? a_data->effect : nullptr;
			auto baseEffect = effect ? effect->baseEffect : nullptr;

			if (target && baseEffect) {
				OnMagicEffectApplyRegMap::GetSingleton()->QueueEvent(target, baseEffect, a_data->caster, baseEffect, a_data->magicItem, result);
			}

			return result;
		}
		static inline REL::Relocation<decltype(MagicTargetApply)> _MagicTargetApply;
	};


	class WeaponHit
	{
	public:
		static void Install()
		{
			auto& trampoline = SKSE::GetTrampoline();

			REL::Relocation<std::uintptr_t> ProcessHitData{ REL::ID(37633) };
			_SendHitEvent = trampoline.write_call<5>(ProcessHitData.address() + 0x16A, SendHitEvent);  // ACTOR

			REL::Relocation<std::uintptr_t> CalculateHitTargetForWeaponSwing{ REL::ID(37674) };
			_SendHitEvent_Impl = trampoline.write_call<5>(CalculateHitTargetForWeaponSwing.address() + 0x6C7, SendHitEvent_Impl);  // STATIC

			REL::Relocation<std::uintptr_t> OnProjectileHit{ REL::ID(43022) };
			_SendHitEvent_Projectile = trampoline.write_call<5>(OnProjectileHit.address() + 0x38D, SendHitEvent_Projectile);  // ARROW
		}

	private:
		static void SendHitEvent(RE::ScriptEventSourceHolder* a_holder, RE::NiPointer<RE::TESObjectREFR>& a_target, RE::NiPointer<RE::TESObjectREFR>& a_aggressor, RE::FormID a_source, RE::FormID a_projectile, RE::HitData& a_data)
		{
			if (auto aggressor = a_aggressor.get(); aggressor) {
				auto target = a_target.get();
				auto source = RE::TESForm::LookupByID(a_source);
				auto flags = to_underlying(a_data.flags);
				OnWeaponHitRegSet::GetSingleton()->QueueEvent(aggressor, target, source, nullptr, flags);
			}

			_SendHitEvent(a_holder, a_target, a_aggressor, a_source, a_projectile, a_data);
		}
		static inline REL::Relocation<decltype(SendHitEvent)> _SendHitEvent;


		static void SendHitEvent_Impl(RE::BSTEventSource<RE::TESHitEvent>& a_source, RE::TESHitEvent& a_event)
		{
			if (auto aggressor = a_event.cause.get(); aggressor) {
				auto target = a_event.target.get();
				auto source = RE::TESForm::LookupByID(a_event.source);
				OnWeaponHitRegSet::GetSingleton()->QueueEvent(aggressor, target, source, nullptr, 0);
			}
			_SendHitEvent_Impl(a_source, a_event);
		}
		static inline REL::Relocation<decltype(SendHitEvent_Impl)> _SendHitEvent_Impl;


		static void SendHitEvent_Projectile(RE::BSTEventSource<RE::TESHitEvent>& a_source, RE::TESHitEvent& a_event)
		{
			if (auto aggressor = a_event.cause.get(); aggressor) {
				auto target = a_event.target.get();
				auto source = RE::TESForm::LookupByID(a_event.source);				
				auto projectile = RE::TESForm::LookupByID<RE::BGSProjectile>(a_event.projectile);
				
				if (projectile && projectile->data.types.all(RE::BGSProjectileData::Type::kArrow)) {
					OnWeaponHitRegSet::GetSingleton()->QueueEvent(aggressor, target, source, projectile, 0);
				}				
				OnProjectileHitRegSet::GetSingleton()->QueueEvent(aggressor, target, source, projectile);
			}
			_SendHitEvent_Projectile(a_source, a_event);
		}
		static inline REL::Relocation<decltype(SendHitEvent_Projectile)> _SendHitEvent_Projectile;
	};


	class MagicHit
	{
	public:
		static void Install()
		{
			auto& trampoline = SKSE::GetTrampoline();

			REL::Relocation<std::uintptr_t> MagicTargetFunc01{ REL::ID(37832) };
			_SendHitEvent_Impl = trampoline.write_call<5>(MagicTargetFunc01.address() + 0x1C3, SendHitEvent_Impl);  // ACTOR
		}

	private:
		static void SendHitEvent_Impl(RE::BSTEventSource<RE::TESHitEvent>& a_source, RE::TESHitEvent& a_event)
		{
			if (auto aggressor = a_event.cause.get(); aggressor) {
				auto target = a_event.target.get();
				auto source = RE::TESForm::LookupByID(a_event.source);
				auto projectile = RE::TESForm::LookupByID<RE::BGSProjectile>(a_event.projectile);
				OnMagicHitRegSet::GetSingleton()->QueueEvent(aggressor, target, source, projectile);
			}
			_SendHitEvent_Impl(a_source, a_event);
		}
		static inline REL::Relocation<decltype(SendHitEvent_Impl)> _SendHitEvent_Impl;
	};


	auto HookEvents() -> bool
	{
		logger::info("{:*^30}", "HOOKED EVENTS"sv);

		logger::info("Hooking Actor Resurrect"sv);
		ActorResurrect::Install();

		logger::info("Hooking Actor Reanimate"sv);
		ActorReanimateStart::Install();
		ActorReanimateStop::Install();

		logger::info("Hooking Weather Change"sv);
		WeatherEvent::Install();

		logger::info("Hooking Magic Effect Apply"sv);
		MagicEffectApply::Install();

		logger::info("Hooking Weapon Hit"sv);
		WeaponHit::Install();

		logger::info("Hooking Magic Hit"sv);
		MagicHit::Install();

		return true;
	}
}
