#include "Papyrus/Functions/Package.h"

namespace Papyrus::Package
{
	void AddPackageIdle(STATIC_ARGS, RE::TESPackage* a_package, RE::TESIdleForm* a_idle)
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

	std::vector<RE::TESIdleForm*> GetPackageIdles(STATIC_ARGS, const RE::TESPackage* a_package)
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

	std::int32_t GetPackageType(STATIC_ARGS, const RE::TESPackage* a_package)
	{
		if (!a_package) {
			a_vm->TraceStack("Package is None", a_stackID);
			return -1;
		}

		return a_package->packData.packType.underlying();
	}

	void RemovePackageIdle(STATIC_ARGS, const RE::TESPackage* a_package, RE::TESIdleForm* a_idle)
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

	void Bind(VM& a_vm)
	{
		BIND(AddPackageIdle);
		BIND(GetPackageIdles);
		BIND(GetPackageType);
		BIND(RemovePackageIdle);

		logger::info("Registered package functions"sv);
	}
}
