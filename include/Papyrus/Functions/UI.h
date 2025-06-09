#pragma once

namespace Papyrus::UI
{
	RE::TESObjectREFR* GetMenuContainer(RE::StaticFunctionTag*);
	void               HideMenu(RE::StaticFunctionTag*, RE::BSFixedString a_menuName);
	bool               IsShowingMenus(RE::StaticFunctionTag*);
	void               ShowBookMenu(STATIC_ARGS, RE::TESObjectBOOK* a_book);
	void               ShowMenu(RE::StaticFunctionTag*, RE::BSFixedString a_menuName);
	void               ToggleOpenSleepWaitMenu(RE::StaticFunctionTag*, bool a_openSleepMenu);

	void Bind(VM& a_vm);
}
