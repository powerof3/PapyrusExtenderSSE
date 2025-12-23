#include "Papyrus/Functions/UI.h"

namespace Papyrus::UI
{
	RE::TESObjectREFR* GetMenuContainer(RE::StaticFunctionTag*)
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

	void HideMenu(RE::StaticFunctionTag*, RE::BSFixedString a_menuName)
	{
		if (const auto UIMsgQueue = RE::UIMessageQueue::GetSingleton(); UIMsgQueue) {
			UIMsgQueue->AddMessage(a_menuName, RE::UI_MESSAGE_TYPE::kHide, nullptr);
		}
	}

	bool IsShowingMenus(RE::StaticFunctionTag*)
	{
		return RE::UI::GetSingleton() ? RE::UI::GetSingleton()->IsShowingMenus() : false;
	}

	void ShowBookMenu(STATIC_ARGS, RE::TESObjectBOOK* a_book)
	{
		if (!a_book) {
			a_vm->TraceStack("Book is None", a_stackID);
			return;
		}

		RE::BookMenu::OpenMenuFromBaseForm(a_book);
	}

	void ShowMenu(RE::StaticFunctionTag*, RE::BSFixedString a_menuName)
	{
		if (const auto UIMsgQueue = RE::UIMessageQueue::GetSingleton(); UIMsgQueue) {
			UIMsgQueue->AddMessage(a_menuName, RE::UI_MESSAGE_TYPE::kShow, nullptr);
		}
	}

	void ToggleOpenSleepWaitMenu(RE::StaticFunctionTag*, bool a_openSleepMenu)
	{
		RE::SleepWaitMenu::ToggleOpenMenu(a_openSleepMenu);
	}

	void Bind(VM& a_vm)
	{
		std::uint32_t count = 0;

		BIND(GetMenuContainer);
		BIND(HideMenu);
		BIND(IsShowingMenus, true);
		BIND(ShowMenu);
		BIND(ShowBookMenu);
		BIND(ToggleOpenSleepWaitMenu);

		logger::info("Registered {} UI functions"sv, count);
	}
}
