#pragma once


namespace papyrusVisualEffect
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;


	RE::BGSArtObject* GetArtObject(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSReferenceEffect* a_vfx);

	std::uint32_t GetArtObjectTotalCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSReferenceEffect* a_vfx, bool a_active);

	void SetArtObject(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSReferenceEffect* a_vfx, RE::BGSArtObject* a_art);


	bool RegisterFuncs(VM* a_vm);
}
