#include "Papyrus/Cell.h"


auto papyrusCell::GetLightingTemplate(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectCELL* a_cell) -> RE::BGSLightingTemplate*
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
	}
	if (!a_lightingTemplate) {
		a_vm->TraceStack("Lighting Template is None", a_stackID, Severity::kWarning);
		return;
	}

	a_cell->lightingTemplate = a_lightingTemplate;
}


auto papyrusCell::GetCellNorthRotation(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectCELL* a_cell) -> float
{
	if (!a_cell) {
		a_vm->TraceStack("Cell is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

    return a_cell->GetNorthRotation();
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
		} else {
			extraNorthMarker = new RE::ExtraNorthRotation();
			if (extraNorthMarker) {
				extraNorthMarker->northRot = a_angle;
				a_cell->extraList.Add(extraNorthMarker);
			}
		}
	} else {
		const auto worldspace = a_cell->worldSpace;
		if (worldspace) {
			worldspace->northRotation = a_angle;
		}
	}
}


auto papyrusCell::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusCell - couldn't get VMState"sv);
		return false;
	}

	auto constexpr Functions = "PO3_SKSEFunctions"sv;

	a_vm->RegisterFunction("GetLightingTemplate"sv, Functions, GetLightingTemplate);

    a_vm->RegisterFunction("SetLightingTemplate"sv, Functions, SetLightingTemplate);

    a_vm->RegisterFunction("GetCellNorthRotation"sv, Functions, GetCellNorthRotation);

    a_vm->RegisterFunction("SetCellNorthRotation"sv, Functions, SetCellNorthRotation);

	return true;
}
