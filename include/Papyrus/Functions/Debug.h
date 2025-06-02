#pragma once

namespace Papyrus::Debug
{
	struct detail
	{
		static bool get_god_mode_impl()
		{
			using func_t = decltype(&detail::get_god_mode_impl);
			static REL::Relocation<func_t> func{ RELOCATION_ID(39425, 40501) };
			return func();
		}
	};

	void GivePlayerSpellBook(RE::StaticFunctionTag*);
	void DumpAnimationVariables(RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BSFixedString a_prefix);
	bool GetGodMode(RE::StaticFunctionTag*);

	void Bind(VM& a_vm);
}
