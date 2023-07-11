#pragma once

namespace Papyrus::Package
{
	inline void AddPackageIdle(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESPackage*  a_package,
		RE::TESIdleForm* a_idle)
	{
		if (!a_package) {
			a_vm->TraceStack("Package is None", a_stackID);
			return;
		}

		if (!a_idle) {
			a_vm->TraceStack("Idle is None", a_stackID);
			return;
		}

		auto& idleCollection = a_package->idleCollection;
		if (!idleCollection) {
			idleCollection = RE::BGSIdleCollection::Create();
		}
		if (idleCollection) {
			idleCollection->AddIdle(a_idle);
		}
	}

	inline std::vector<RE::TESIdleForm*> GetPackageIdles(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESPackage* a_package)
	{
		std::vector<RE::TESIdleForm*> result;

		if (!a_package) {
			a_vm->TraceStack("Package is None", a_stackID);
			return result;
		}

		const auto idleCollection = a_package->idleCollection;
		if (idleCollection && idleCollection->idles && idleCollection->idleCount > 0) {
			std::span<RE::TESIdleForm*> span(idleCollection->idles, idleCollection->idleCount);
			result.assign(span.begin(), span.end());
		}

		return result;
	}

	inline std::int32_t GetPackageType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESPackage* a_package)
	{
		if (!a_package) {
			a_vm->TraceStack("Package is None", a_stackID);
			return -1;
		}

		return stl::to_underlying(a_package->packData.packType.get());
	}

	inline void RemovePackageIdle(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		const RE::TESPackage* a_package,
		RE::TESIdleForm*      a_idle)
	{
		if (!a_package) {
			a_vm->TraceStack("Package is None", a_stackID);
			return;
		}

		if (!a_idle) {
			a_vm->TraceStack("Idle is None", a_stackID);
			return;
		}

		if (auto& idleCollection = a_package->idleCollection; idleCollection) {
			idleCollection->RemoveIdle(a_idle);
		}
	}

	inline void Bind(VM& a_vm)
	{
		BIND(AddPackageIdle);
		BIND(GetPackageIdles);
		BIND(GetPackageType);
		BIND(RemovePackageIdle);

		logger::info("Registered package functions"sv);
	}
}
