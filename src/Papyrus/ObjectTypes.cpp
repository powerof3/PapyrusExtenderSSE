#include "Papyrus/ObjectTypes.h"

namespace Papyrus::ObjectTypes
{
	bool detail::is_registered(VM& a_vm, RE::VMTypeID a_type)  //no idea why this fixes script objects not being registered
	{
		RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo> classPtr;
		return a_vm.GetScriptObjectType(a_type, classPtr) && classPtr;
	}

	void Bind(VM& a_vm)
	{
		logger::info("Registered object types"sv);

		EXTEND(RE::FormType::FootstepSet, FootstepSet);
		EXTEND(RE::FormType::LightingMaster, LightingTemplate);
		EXTEND(RE::FormType::Debris, Debris);
		EXTEND(RE::FormType::MaterialObject, MaterialObject);
	}
}
