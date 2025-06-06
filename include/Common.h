#pragma once

using VM = RE::BSScript::Internal::VirtualMachine;
using StackID = RE::VMStackID;
using Severity = RE::BSScript::ErrorLogger::Severity;

namespace stl
{
	inline bool read_string(SKSE::SerializationInterface* a_intfc, std::string& a_str)
	{
		std::size_t size = 0;
		if (!a_intfc->ReadRecordData(size)) {
			return false;
		}

		char* buf = new char[size];

		if (!a_intfc->ReadRecordData(buf, static_cast<std::uint32_t>(size))) {
			delete[] buf;
			return false;
		}

		buf[size - 1] = '\0';
		a_str = buf;

		delete[] buf;
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
}

namespace Papyrus
{
	inline constexpr auto script = "PO3_SKSEFunctions"sv;

#define BIND(a_method, ...) a_vm.RegisterFunction(#a_method##sv, script, a_method __VA_OPT__(, ) __VA_ARGS__)
#define BIND_EVENT(a_method, ...) a_vm.RegisterFunction(#a_method##sv, obj, a_method __VA_OPT__(, ) __VA_ARGS__)

#define STATIC_ARGS [[maybe_unused]] VM *a_vm, [[maybe_unused]] StackID a_stackID, RE::StaticFunctionTag *
}
