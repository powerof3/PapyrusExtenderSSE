#include "Papyrus/Cell.h"


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


float papyrusCell::GetCellNorthRotation(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectCELL* a_cell)
{
	if (!a_cell) {
		a_vm->TraceStack("Cell is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	if (a_cell->IsInteriorCell()) {
		auto extraNorthMarker = a_cell->extraList.GetByType<RE::ExtraNorthRotation>();
		if (extraNorthMarker) {
			return extraNorthMarker->northRot;
		}
	} else {
		auto worldspace = a_cell->worldSpace;
		if (worldspace) {
			return worldspace->northRotation;
		}
	}

	return 0.0f;
}


void papyrusCell::SetCellNorthRotation(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectCELL* a_cell, float a_angle)
{
	if (!a_cell) {
		a_vm->TraceStack("Cell is None", a_stackID, Severity::kWarning);
		return;
	}

	if (a_cell->IsInteriorCell()) {
		auto extraNorthMarker = a_cell->extraList.GetByType<RE::ExtraNorthRotation>();
		if (extraNorthMarker) {
			extraNorthMarker->northRot = a_angle;
		} 
		else {
			extraNorthMarker = new RE::ExtraNorthRotation();
			if (extraNorthMarker) {
				a_cell->extraList.Add(extraNorthMarker);
				extraNorthMarker->northRot = a_angle;
			}
		}
	} else {
		auto worldspace = a_cell->worldSpace;
		if (worldspace) {
			worldspace->northRotation = a_angle;
		}
	}
}



bool papyrusCell::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("papyrusCell - couldn't get VMState"sv);
		return false;
	}

	a_vm->RegisterFunction("GetLightingTemplate", "PO3_SKSEFunctions", GetLightingTemplate);
	a_vm->RegisterFunction("SetLightingTemplate", "PO3_SKSEFunctions", SetLightingTemplate);
	a_vm->RegisterFunction("GetCellNorthRotation", "PO3_SKSEFunctions", GetCellNorthRotation);
	a_vm->RegisterFunction("SetCellNorthRotation", "PO3_SKSEFunctions", SetCellNorthRotation);

	return true;
}
