#pragma once


namespace extendedObjectTypes
{
	using VM = RE::BSScript::IVirtualMachine;
	
	template <class T>
	RE::VMTypeID vm_cast()
	{
		return static_cast<RE::VMTypeID>(T::FORMTYPE);
	}

	bool RegisterTypes(VM* a_vm);
}
