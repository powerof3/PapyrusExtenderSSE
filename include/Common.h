#pragma once

using VM = RE::BSScript::Internal::VirtualMachine;
using StackID = RE::VMStackID;
using Severity = RE::BSScript::ErrorLogger::Severity;

namespace stl
{
	using namespace SKSE::stl;

	inline bool read_string(SKSE::SerializationInterface* a_intfc, std::string& a_str)
	{
		std::size_t size = 0;
		if (!a_intfc->ReadRecordData(size)) {
			return false;
		}
		a_str.reserve(size);
		if (!a_intfc->ReadRecordData(a_str.data(), static_cast<std::uint32_t>(size))) {
			return false;
		}
		return true;
	}

	inline bool write_string(SKSE::SerializationInterface* a_intfc, const std::string& a_str)
	{
		std::size_t size = a_str.length() + 1;
		return a_intfc->WriteRecordData(size) && a_intfc->WriteRecordData(a_str.data(), static_cast<std::uint32_t>(size));
	}

	inline bool read_formID(SKSE::SerializationInterface* a_intfc, RE::FormID& a_formID)
	{
		a_intfc->ReadRecordData(a_formID);
		if (a_formID != 0) {
			return a_intfc->ResolveFormID(a_formID, a_formID);
		}
		return true;
	}

	template <class T>
	void write_thunk_call(std::uintptr_t a_src)
	{
		auto& trampoline = SKSE::GetTrampoline();
		T::func = trampoline.write_call<5>(a_src, T::thunk);
	}

	template <class F, std::size_t idx, class T>
	void write_vfunc()
	{
		REL::Relocation<std::uintptr_t> vtbl{ F::VTABLE[0] };
		T::func = vtbl.write_vfunc(idx, T::thunk);
	}

	template <typename First, typename... T>
	[[nodiscard]] bool is_in(First&& first, T&&... t)
	{
		return ((first == t) || ...);
	}

	constexpr inline auto enum_range(auto first, auto last)
	{
		auto enum_range =
			std::views::iota(
				std::to_underlying(first),
				std::to_underlying(last) + 1) |
			std::views::transform([](auto enum_val) {
				return (decltype(first))enum_val;
			});

		return enum_range;
	};
}

namespace Papyrus
{
	inline constexpr auto script = "PO3_SKSEFunctions"sv;

#define BIND(a_method, ...) a_vm.RegisterFunction(#a_method##sv, script, a_method __VA_OPT__(, ) __VA_ARGS__)
#define BIND_EVENT(a_method, ...) a_vm.RegisterFunction(#a_method##sv, obj, a_method __VA_OPT__(, ) __VA_ARGS__)

#define STATIC_ARGS [[maybe_unused]] VM *a_vm, [[maybe_unused]] StackID a_stackID, RE::StaticFunctionTag *
}
