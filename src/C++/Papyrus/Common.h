#pragma once

namespace Papyrus
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;

	inline constexpr auto Functions{ "PO3_SKSEFunctions"sv };

#define BIND(a_method, ...) a_vm.RegisterFunction(#a_method##sv, Functions, a_method __VA_OPT__(, ) __VA_ARGS__)

#define BIND_EVENT(a_method, ...) a_vm.RegisterFunction(#a_method##sv, obj, a_method __VA_OPT__(, ) __VA_ARGS__)
}
