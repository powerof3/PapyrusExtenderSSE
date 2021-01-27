#include "Papyrus/VisualEffect.h"


auto papyrusVisualEffect::GetArtObject(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSReferenceEffect* a_vfx) -> RE::BGSArtObject*
{
	if (!a_vfx) {
		a_vm->TraceStack("VisualEffect is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	return a_vfx->data.artObject;
}


auto papyrusVisualEffect::GetArtObjectTotalCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSReferenceEffect* a_vfx, bool a_active) -> std::uint32_t
{
	std::uint32_t count = 0;

	if (!a_vfx) {
		a_vm->TraceStack("VisualEffect is None", a_stackID, Severity::kWarning);
		return count;
	}

	auto art = a_vfx->data.artObject;
	if (!art) {
		a_vm->TraceStack("ArtObject is None", a_stackID, Severity::kWarning);
		return count;
	}

	if (auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
		processLists->GetMagicEffects([&](RE::BSTempEffect& a_tempEffect) {
			const auto modelEffect = a_tempEffect.As<RE::ModelReferenceEffect>();
			if (modelEffect && modelEffect->artObject == art) {
				if (!a_active || a_active && !modelEffect->finished) {
					count++;
				}
			}
			return true;
		});
	}

	return count;
}


void papyrusVisualEffect::SetArtObject(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSReferenceEffect* a_vfx, RE::BGSArtObject* a_art)
{
	if (!a_vfx) {
		a_vm->TraceStack("VisualEffect is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_art) {
		a_vm->TraceStack("ArtObject is None", a_stackID, Severity::kWarning);
		return;
	}

	a_vfx->data.artObject = a_art;
}


auto papyrusVisualEffect::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusVisualEffect - couldn't get VMState"sv);
		return false;
	}

	auto constexpr Functions = "PO3_SKSEFunctions"sv;

	a_vm->RegisterFunction("GetArtObject"sv, Functions, GetArtObject, true);

	a_vm->RegisterFunction("GetArtObjectTotalCount"sv, Functions, GetArtObjectTotalCount);

	a_vm->RegisterFunction("SetArtObject"sv, Functions, SetArtObject);

	return true;
}
