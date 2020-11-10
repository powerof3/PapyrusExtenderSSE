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

	bool HookActorResurrect()
	{
		logger::info("Hooking Actor Resurrect");

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

				auto zombiePtr = a_this->commandedActor.get();
				auto zombie = zombiePtr.get();

				if (!zombie) {
					return;
				}

				if (zombie->boolBits.all(RE::Actor::BOOL_BITS::kParalyzed)) {
					auto root = zombie->Get3D(0);
					auto charController = zombie->GetCharController();
					if (root && charController) {
						zombie->boolBits.reset(RE::Actor::BOOL_BITS::kParalyzed);
						root->SetRigidConstraints(false);
						std::uint32_t unk = 0;
						auto const flags = *(charController->Unk_08(&unk));
						root->UpdateRigidBodySettings(32, flags >> 16);
					}
				}

				auto casterPtr = a_this->caster.get();
				auto caster = casterPtr.get();
				if (caster) {
					OnActorReanimateStartRegSet::GetSingleton()->QueueEvent(zombie, caster);
				}
			}
		}

		using Reanimate_t = decltype(&RE::ReanimateEffect::Unk_14);	 // 0AB
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
			auto zombiePtr = a_this->commandedActor.get();
			auto zombie = zombiePtr.get();
			if (zombie) {
				auto casterPtr = a_this->caster.get();
				auto caster = casterPtr.get();
				if (caster) {
					OnActorReanimateStopRegSet::GetSingleton()->QueueEvent(zombie, caster);
				}
			}

			_Reanimate(a_this);
		}

		using Reanimate_t = decltype(&RE::ReanimateEffect::Unk_15);	 // 0AB
		static inline REL::Relocation<Reanimate_t> _Reanimate;
	};


	bool HookActorReanimate()
	{
		logger::info("Hooking Actor Reanimate");

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
					dq(a_target + 0x6);	 // next line
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
			auto sky = RE::Sky::GetSingleton();
			if (sky) {
				auto currentWeather = sky->currentWeather;
				auto lastWeather = sky->lastWeather;
				if (currentWeather && lastWeather) {
					OnWeatherChangeRegSet::GetSingleton()->QueueEvent(lastWeather, currentWeather);
				}
			}
		}
	};

	bool HookWeatherChange()
	{
		logger::info("Hooking Weather Change");

		WeatherEvent::Install();

		return true;
	}


	bool HookEvents()
	{
		logger::info("{:*^30}", "HOOKED EVENTS");

		HookActorResurrect();

		HookActorReanimate();

		HookWeatherChange();

		return true;
	}
}
