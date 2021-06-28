#pragma once

namespace Papyrus::Cell
{
	inline RE::BGSLightingTemplate* GetLightingTemplate(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESObjectCELL* a_cell)
	{
		if (!a_cell) {
			a_vm->TraceStack("Cell is None", a_stackID);
			return nullptr;
		}

		return a_cell->lightingTemplate;
	}

	inline void SetLightingTemplate(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		RE::TESObjectCELL* a_cell, 
		RE::BGSLightingTemplate* a_lightingTemplate)
	{
		if (!a_cell) {
			a_vm->TraceStack("Cell is None", a_stackID);
			return;
		}
		if (!a_lightingTemplate) {
			a_vm->TraceStack("Lighting Template is None", a_stackID);
			return;
		}

		a_cell->lightingTemplate = a_lightingTemplate;
	}

	inline float GetCellNorthRotation(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectCELL* a_cell)
	{
		if (!a_cell) {
			a_vm->TraceStack("Cell is None", a_stackID);
			return 0.0f;
		}

		return a_cell->GetNorthRotation();
	}

	inline void SetCellNorthRotation(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		RE::TESObjectCELL* a_cell, 
		float a_angle)
	{
		if (!a_cell) {
			a_vm->TraceStack("Cell is None", a_stackID);
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

	inline void Register(VM& a_vm)
	{
		BIND(GetLightingTemplate);
		BIND(SetLightingTemplate);
		BIND(GetCellNorthRotation);
		BIND(SetCellNorthRotation);

		logger::info("Registered cell functions"sv);
	}

}
