#include "Papyrus/Functions/Cell.h"

namespace Papyrus::Cell
{
	RE::BGSLightingTemplate* GetLightingTemplate(STATIC_ARGS, const RE::TESObjectCELL* a_cell)
	{
		if (!a_cell) {
			a_vm->TraceStack("Cell is None", a_stackID);
			return nullptr;
		}

		return a_cell->lightingTemplate;
	}

	void SetLightingTemplate(STATIC_ARGS, RE::TESObjectCELL* a_cell, RE::BGSLightingTemplate* a_lightingTemplate)
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

	float GetCellNorthRotation(STATIC_ARGS, RE::TESObjectCELL* a_cell)
	{
		if (!a_cell) {
			a_vm->TraceStack("Cell is None", a_stackID);
			return 0.0f;
		}

		return a_cell->GetNorthRotation();
	}

	void SetCellNorthRotation(STATIC_ARGS, RE::TESObjectCELL* a_cell, float a_angle)
	{
		if (!a_cell) {
			a_vm->TraceStack("Cell is None", a_stackID);
			return;
		}

		if (a_cell->IsInteriorCell()) {
			if (auto extraNorthMarker = a_cell->extraList.GetByType<RE::ExtraNorthRotation>(); extraNorthMarker) {
				extraNorthMarker->northRot = a_angle;
			} else {
				extraNorthMarker = new RE::ExtraNorthRotation();
				if (extraNorthMarker) {
					extraNorthMarker->northRot = a_angle;
					a_cell->extraList.Add(extraNorthMarker);
				}
			}
		} else {
			if (const auto worldspace = a_cell->worldSpace; worldspace) {
				worldspace->northRotation = a_angle;
			}
		}
	}

	void Bind(VM& a_vm)
	{
		std::uint32_t count = 0;

		BIND(GetLightingTemplate);
		BIND(SetLightingTemplate);
		BIND(GetCellNorthRotation);
		BIND(SetCellNorthRotation);

		logger::info("Registered {} cell functions"sv, count);
	}
}
