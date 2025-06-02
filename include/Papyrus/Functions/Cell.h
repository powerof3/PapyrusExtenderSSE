#pragma once

namespace Papyrus::Cell
{
	RE::BGSLightingTemplate* GetLightingTemplate(STATIC_ARGS, const RE::TESObjectCELL* a_cell);
	void SetLightingTemplate(STATIC_ARGS, RE::TESObjectCELL* a_cell, RE::BGSLightingTemplate* a_lightingTemplate);
	float GetCellNorthRotation(STATIC_ARGS, RE::TESObjectCELL* a_cell);
	void SetCellNorthRotation(STATIC_ARGS, RE::TESObjectCELL* a_cell, float a_angle);

	void Bind(VM& a_vm);
}
