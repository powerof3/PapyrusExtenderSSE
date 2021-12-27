#pragma once

namespace Papyrus::UI
{
	inline RE::TESObjectREFR* GetMenuContainer(RE::StaticFunctionTag*)
	{
		RE::TESObjectREFR* container = nullptr;

		const auto ui = RE::UI::GetSingleton();
		const auto menu = ui ? ui->GetMenu<RE::ContainerMenu>() : nullptr;
		if (menu) {
			const auto refHandle = menu->GetTargetRefHandle();
			RE::TESObjectREFRPtr refr;
			RE::LookupReferenceByHandle(refHandle, refr);

			container = refr.get();
		}

		return container;
	}

	inline void Bind(VM& a_vm)
	{
		BIND(GetMenuContainer);

		logger::info("Registered UI functions"sv);
	}
}
