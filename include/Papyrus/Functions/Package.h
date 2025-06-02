#pragma once

namespace Papyrus::Package
{
	void                          AddPackageIdle(STATIC_ARGS, RE::TESPackage* a_package, RE::TESIdleForm* a_idle);
	std::vector<RE::TESIdleForm*> GetPackageIdles(STATIC_ARGS, const RE::TESPackage* a_package);
	std::int32_t                  GetPackageType(STATIC_ARGS, const RE::TESPackage* a_package);
	void                          RemovePackageIdle(STATIC_ARGS, const RE::TESPackage* a_package, RE::TESIdleForm* a_idle);

	void Bind(VM& a_vm);
}
