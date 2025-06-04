#pragma once

#define EXTEND(a_formType, a_className)                              \
	auto a_className##_type = static_cast<RE::VMTypeID>(a_formType); \
	a_vm.RegisterObjectType(a_className##_type, #a_className);       \
	logger::info("\t{} : {}", #a_className, detail::is_registered(a_vm, a_className##_type));

namespace Papyrus::ObjectTypes
{
	struct detail
	{
		static bool is_registered(VM& a_vm, RE::VMTypeID a_type);
	};

	void Bind(VM& a_vm);
}
