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
		static void Resurrect(RE::Actor* a_this, bool a_resetInventory, bool a_attach3D)
		{
			_Resurrect(a_this, a_resetInventory, a_attach3D);

			OnActorResurrectRegSet::GetSingleton()->QueueEvent(a_this, a_resetInventory);
		}

		using Resurrect_t = decltype(&RE::Actor::Resurrect);  // 0AB
		static inline REL::Relocation<Resurrect_t> _Resurrect;
	};

	auto HookActorResurrect() -> bool
	{
		logger::info("Hooking Actor Resurrect"sv);

		ActorResurrect::Install();

		return true;
	}


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
						std::uint32_t unk = 0;
						auto const flags = *charController->Unk_08(&unk);
						root->UpdateRigidBodySettings(32, flags >> 16);
					}
				}

				const auto casterPtr = a_this->caster.get();
				const auto caster = casterPtr.get();
				if (caster) {
					OnActorReanimateStartRegSet::GetSingleton()->QueueEvent(zombie, caster);
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
					OnActorReanimateStopRegSet::GetSingleton()->QueueEvent(zombie, caster);
				}
			}

			_Reanimate(a_this);
		}

		using Reanimate_t = decltype(&RE::ReanimateEffect::Unk_15);  // 0AB
		static inline REL::Relocation<Reanimate_t> _Reanimate;
	};


	auto HookActorReanimate() -> bool
	{
		logger::info("Hooking Actor Reanimate"sv);

		ActorReanimateStart::Install();

		ActorReanimateStop::Install();

		return true;
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

	auto HookWeatherChange() -> bool
	{
		logger::info("Hooking Weather Change"sv);

		WeatherEvent::Install();

		return true;
	}


	class MagicEffectApply
	{
	public:
		static void Install()
		{
			auto& trampoline = SKSE::GetTrampoline();

			REL::Relocation<std::uintptr_t> MagicTargetFunc01{ REL::ID(33742) };
			_MagicTargetApply = trampoline.write_call<5>(MagicTargetFunc01.address() + 0x1E8, MagicTargetApply);
		}

	private:
		static bool MagicTargetApply(RE::MagicTarget* a_this, RE::MagicTarget::CreationData* a_data)
		{
			auto result = _MagicTargetApply(a_this, a_data);
			if (a_data) {
				auto target = a_this->GetTargetStatsObject();

				auto effect = a_data->effect;
				auto baseEffect = effect ? effect->baseEffect : nullptr;

				auto caster = a_data->caster;

				if (target && baseEffect && caster) {
					OnMagicEffectApplyRegMap::GetSingleton()->QueueEvent(target, baseEffect, caster, baseEffect, result);
				}
			}
			return result;
		}
		static inline REL::Relocation<decltype(MagicTargetApply)> _MagicTargetApply;
	};


	auto HookMagicEffectApply() -> bool
	{
		logger::info("Hooking Magic Effect Apply"sv);

		MagicEffectApply::Install();

		return true;
	}


	auto HookEvents() -> bool
	{
		logger::info("{:*^30}", "HOOKED EVENTS"sv);

		HookActorResurrect();

		HookActorReanimate();

		HookWeatherChange();

		HookMagicEffectApply();

		return true;
	}
}
