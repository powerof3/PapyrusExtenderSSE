#include "Papyrus/PapyrusVisualEffect.h"


RE::BGSArtObject* papyrusVisualEffect::GetArtObject(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSReferenceEffect* a_vfx)
{
    if (!a_vfx) {
        a_vm->TraceStack("Visual Effect is None", a_stackID, Severity::kWarning);
        return nullptr;
    }
    return a_vfx->data.artObject;
}


UInt32 papyrusVisualEffect::GetArtObjectTotalCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSReferenceEffect* a_vfx, bool a_active)
{
	UInt32 count = 0;

	if (!a_vfx) {
		a_vm->TraceStack("Visual Effect is None", a_stackID, Severity::kWarning);
		return count;
	}
	auto art = a_vfx->data.artObject;
	if (!art) {
		a_vm->TraceStack("Couldn't get art object", a_stackID, Severity::kWarning);
		return count;
	}
	auto processLists = RE::ProcessLists::GetSingleton();
	if (processLists) {
		processLists->GetMagicEffects([&](RE::BSTempEffect* a_tempEffect)
		{
			auto modelEffect = a_tempEffect->As<RE::ModelReferenceEffect>();
			if (modelEffect) {
				auto modelArt = modelEffect->artObject;
				if (modelArt && modelArt == art) {
					if (!a_active || a_active && !modelEffect->finished) {
						count++;
					}
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
		a_vm->TraceStack("Visual Effect is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_art) {
		a_vm->TraceStack("Art Object is None", a_stackID, Severity::kWarning);
		return;
	}
	a_vfx->data.artObject = a_art;
}


bool papyrusVisualEffect::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		_MESSAGE("papyrusVisualEffect - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("GetArtObject", "PO3_SKSEFunctions", GetArtObject, true);

	a_vm->RegisterFunction("GetArtObjectTotalCount", "PO3_SKSEFunctions", GetArtObjectTotalCount);

	a_vm->RegisterFunction("SetArtObject", "PO3_SKSEFunctions", SetArtObject);

	return true;
}
