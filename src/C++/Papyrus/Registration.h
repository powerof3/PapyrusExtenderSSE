#pragma once

namespace Papyrus
{
	namespace Script
	{
		bool Register(RE::BSScript::IVirtualMachine* a_vm);
	}

	namespace Events
	{
		void RegisterScriptEvents();

		void RegisterStoryEvents();
	}
}
