#pragma once

namespace Papyrus::UI
{
	inline RE::TESObjectREFR* GetMenuContainer(RE::StaticFunctionTag*)
	{
		RE::TESObjectREFR* container = nullptr;

		const auto UI = RE::UI::GetSingleton();
		const auto menu = UI ? UI->GetMenu<RE::ContainerMenu>() : nullptr;
		if (menu) {
			const auto           refHandle = menu->GetTargetRefHandle();
			RE::TESObjectREFRPtr refr;
			RE::LookupReferenceByHandle(refHandle, refr);

			container = refr.get();
		}

		return container;
	}

	inline void HideMenu(RE::StaticFunctionTag*, RE::BSFixedString a_menuName)
	{
		if (const auto UIMsgQueue = RE::UIMessageQueue::GetSingleton(); UIMsgQueue) {
			UIMsgQueue->AddMessage(a_menuName, RE::UI_MESSAGE_TYPE::kHide, nullptr);
		}
	}

	inline bool IsShowingMenus(RE::StaticFunctionTag*)
	{
		return RE::UI::GetSingleton() ? RE::UI::GetSingleton()->IsShowingMenus() : false;
	}

	inline void ShowMenu(RE::StaticFunctionTag*, RE::BSFixedString a_menuName)
	{
		if (const auto UIMsgQueue = RE::UIMessageQueue::GetSingleton(); UIMsgQueue) {
			UIMsgQueue->AddMessage(a_menuName, RE::UI_MESSAGE_TYPE::kShow, nullptr);
		}
	}

	inline void Bind(VM& a_vm)
	{
		BIND(GetMenuContainer);
		BIND(HideMenu);
		BIND(IsShowingMenus, true);
		BIND(ShowMenu);

		logger::info("Registered UI functions"sv);
	}
}
