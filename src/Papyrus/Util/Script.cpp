#include "Papyrus/Util/Script.h"

namespace SCRIPT
{
	struct detail
	{
		static bool check_script(const Papyrus::VM& a_vm, const RE::VMHandle a_handle, const RE::BSFixedString& a_scriptName)
		{
			if (!a_scriptName.empty()) {
				RE::BSTSmartPointer<RE::BSScript::Object> object;
				return a_vm.FindBoundObject(a_handle, a_scriptName.c_str(), object) && object;
			}
			RE::BSSpinLockGuard locker(a_vm.attachedScriptsLock);
			if (const auto it = a_vm.attachedScripts.find(a_handle); it != a_vm.attachedScripts.end()) {
				return std::ranges::any_of(it->second, [&](const auto& script) {
					auto typeInfo = script ? script->GetTypeInfo() : nullptr;
					return typeInfo && std::find(baseScripts.begin(), baseScripts.end(), typeInfo->name) == baseScripts.end();  // is not a base script
				});
			}
			return false;
		}
	};

	bool is_script_attached(const RE::TESForm* a_form, const RE::BSFixedString& a_scriptName)
	{
		const auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
		if (!vm) {
			return false;
		}

		const auto policy = vm->GetObjectHandlePolicy();
		if (!policy) {
			return false;
		}

		const auto check_script = [&](const RE::TESForm* b_form) {
			const auto handle = policy->GetHandleForObject(static_cast<RE::VMTypeID>(b_form->GetFormType()), b_form);
			if (handle == policy->EmptyHandle()) {
				return false;
			}
			return detail::check_script(*vm, handle, a_scriptName);
		};

		auto result = check_script(a_form);
		if (!result) {
			const auto ref = a_form->As<RE::TESObjectREFR>();
			const auto base = ref ?
			                      ref->GetObjectReference() :
			                      nullptr;
			if (base) {
				result = check_script(base);
			}
		}

		return result;
	}

	bool is_script_attached(const RE::BGSBaseAlias* a_alias, const RE::BSFixedString& a_scriptName)
	{
		const auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
		if (!vm) {
			return false;
		}

		const auto policy = vm->GetObjectHandlePolicy();
		if (!policy) {
			return false;
		}

		const auto handle = policy->GetHandleForObject(a_alias->GetVMTypeID(), a_alias);
		if (handle == policy->EmptyHandle()) {
			return false;
		}

		return detail::check_script(*vm, handle, a_scriptName);
	}

	bool is_script_attached(const RE::ActiveEffect* a_activeEffect, const RE::BSFixedString& a_scriptName)
	{
		const auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
		if (!vm) {
			return false;
		}

		const auto policy = vm->GetObjectHandlePolicy();
		if (!policy) {
			return false;
		}

		const auto handle = policy->GetHandleForObject(RE::ActiveEffect::VMTYPEID, a_activeEffect);
		if (handle == policy->EmptyHandle()) {
			return false;
		}

		return detail::check_script(*vm, handle, a_scriptName);
	}
}
