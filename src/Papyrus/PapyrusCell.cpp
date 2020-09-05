#include "Papyrus/PapyrusCell.h"


RE::BGSLightingTemplate* papyrusCell::GetLightingTemplate(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectCELL* a_cell)
{
	if (!a_cell) {
		a_vm->TraceStack("Cell is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	return a_cell->lightingTemplate;
}


void papyrusCell::SetLightingTemplate(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectCELL* a_cell, RE::BGSLightingTemplate* a_lightingTemplate)
{
	if (!a_cell) {
		a_vm->TraceStack("Cell is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_lightingTemplate) {
		a_vm->TraceStack("Lighting Template is None", a_stackID, Severity::kWarning);
		return;
	}

	a_cell->lightingTemplate = a_lightingTemplate;
}


bool papyrusCell::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("papyrusCell - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("GetLightingTemplate", "PO3_SKSEFunctions", GetLightingTemplate);
	a_vm->RegisterFunction("SetLightingTemplate", "PO3_SKSEFunctions", SetLightingTemplate);

	return true;
}
