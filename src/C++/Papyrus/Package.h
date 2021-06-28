#pragma once

namespace Papyrus::Package
{
	inline std::int32_t GetPackageType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESPackage* a_package)
	{
		if (!a_package) {
			a_vm->TraceStack("Package is None", a_stackID);
			return -1;
		}

		return stl::to_underlying(a_package->packData.packType.get());
	}

	inline void Register(VM& a_vm)
	{
		BIND(GetPackageType);

		logger::info("Registered package functions"sv);
	}
}
