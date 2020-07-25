#pragma once


namespace papyrusFurniture
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;


	std::int32_t GetFurnitureType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESFurniture* a_furniture);


	bool RegisterFuncs(VM* a_vm);
}
