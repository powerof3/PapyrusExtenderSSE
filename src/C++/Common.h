#pragma once

#include "Papyrus/Util/ConditionParser.h"
#include "Papyrus/Util/Graphics.h"
#include "Papyrus/Util/Magic.h"
#include "Papyrus/Util/Script.h"

#ifdef SKYRIM_AE
#	define REL_ID(se, ae) REL::ID(ae)
#	define OFFSET(se, ae) ae
#	define OFFSET_3(se, ae, vr) ae
#elif SKYRIMVR
#	define REL_ID(se, ae) REL::ID(se)
#	define OFFSET(se, ae) se
#	define OFFSET_3(se, ae, vr) vr
#else
#	define REL_ID(se, ae) REL::ID(se)
#	define OFFSET(se, ae) se
#	define OFFSET_3(se, ae, vr) se
#endif

#define BIND(a_method, ...) a_vm.RegisterFunction(#a_method##sv, script, a_method __VA_OPT__(, ) __VA_ARGS__)
#define BIND_EVENT(a_method, ...) a_vm.RegisterFunction(#a_method##sv, obj, a_method __VA_OPT__(, ) __VA_ARGS__)

namespace stl
{
	using namespace SKSE::stl;

	inline bool read_string(SKSE::SerializationInterface* a_intfc, std::string& a_str)
	{
		std::uint32_t length = 0;
		if (!a_intfc->ReadRecordData(&length, sizeof(length))) {
			return false;
		}
		char* buf = new char[length + 1];
		if (!a_intfc->ReadRecordData(buf, length)) {
			return false;
		}
		buf[length] = '\0';
		a_str = std::string(buf);
		delete[] buf;
		return true;
	}

	inline void write_string(SKSE::SerializationInterface* a_intfc, const std::string& a_string)
	{
		auto length = static_cast<std::uint32_t>(a_string.length());
		a_intfc->WriteRecordData(&length, sizeof(length));
		a_intfc->WriteRecordData(a_string.c_str(), length);
	}

	template <class T>
	void write_thunk_call(std::uintptr_t a_src)
	{
		auto& trampoline = SKSE::GetTrampoline();
		SKSE::AllocTrampoline(14);

	    T::func = trampoline.write_call<5>(a_src, T::thunk);
	}

	template <class F, std::size_t idx, class T>
	void write_vfunc()
	{
		REL::Relocation<std::uintptr_t> vtbl{ F::VTABLE[0] };
		T::func = vtbl.write_vfunc(idx, T::thunk);
	}
}

namespace Papyrus
{
	using VM = RE::BSScript::Internal::VirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;

	inline constexpr auto script = "PO3_SKSEFunctions"sv;
}
