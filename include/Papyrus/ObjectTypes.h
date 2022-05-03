#pragma once

#define EXTEND(a_formType, a_className) a_vm.RegisterObjectType(static_cast<RE::VMTypeID>(a_formType), a_className)

namespace Papyrus::ObjectTypes
{
	struct detail
	{
		static bool is_registered(VM& a_vm, RE::FormType a_formType)  //no idea why this fixes script objects not being registered
		{
			RE::BSTSmartPointer<RE::BSScript::ObjectTypeInfo> classPtr;
			return a_vm.GetScriptObjectType(static_cast<RE::VMTypeID>(a_formType), classPtr) && classPtr;
		}
	};

	inline void Bind(VM& a_vm)
	{
		EXTEND(RE::FormType::FootstepSet, "FootstepSet");
		EXTEND(RE::FormType::LightingMaster, "LightingTemplate");
		EXTEND(RE::FormType::Debris, "Debris");
		EXTEND(RE::FormType::MaterialObject, "MaterialObject");

		logger::info("Registered object types"sv);

		logger::info("	FootstepSet {}",
			detail::is_registered(a_vm, RE::FormType::FootstepSet));
		logger::info("	LightingTemplate {}",
			detail::is_registered(a_vm, RE::FormType::LightingMaster));
		logger::info("	Debris {}",
			detail::is_registered(a_vm, RE::FormType::Debris));
		logger::info("	MaterialObject {}",
			detail::is_registered(a_vm, RE::FormType::MaterialObject));
	}
}
