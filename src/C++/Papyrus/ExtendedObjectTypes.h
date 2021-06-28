#pragma once

namespace Papyrus::ExtendedObjectTypes
{
	template <class T>
	RE::VMTypeID vm_cast()
	{
		return static_cast<RE::VMTypeID>(T::FORMTYPE);
	}

	inline void Register(VM& a_vm)
	{
		a_vm.RegisterObjectType(vm_cast<RE::BGSFootstepSet>(), "FootstepSet");
		a_vm.RegisterObjectType(vm_cast<RE::BGSLightingTemplate>(), "LightingTemplate");
		a_vm.RegisterObjectType(vm_cast<RE::BGSDebris>(), "Debris");
		a_vm.RegisterObjectType(vm_cast<RE::BGSMaterialObject>(), "MaterialObject");
	}
}
