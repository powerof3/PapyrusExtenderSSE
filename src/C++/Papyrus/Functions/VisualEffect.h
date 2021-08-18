#pragma once

namespace Papyrus::VisualEffect
{
	inline RE::BGSArtObject* GetArtObject(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSReferenceEffect* a_vfx)
	{
		if (!a_vfx) {
			a_vm->TraceStack("VisualEffect is None", a_stackID);
			return nullptr;
		}

		return a_vfx->data.artObject;
	}

	inline std::uint32_t GetArtObjectTotalCount(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		const RE::BGSReferenceEffect* a_vfx, 
		bool a_active)
	{
		std::uint32_t count = 0;

		if (!a_vfx) {
			a_vm->TraceStack("VisualEffect is None", a_stackID);
			return count;
		}

		const auto art = a_vfx->data.artObject;
		if (!art) {
			a_vm->TraceStack("ArtObject is None", a_stackID);
			return count;
		}

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			processLists->GetModelEffects([&](RE::ModelReferenceEffect& a_modelEffect) {
				if (a_modelEffect.artObject == art && !a_active || !a_modelEffect.finished) {
					count++;
				}
				return true;
			});
		}

		return count;
	}

	inline void SetArtObject(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		RE::BGSReferenceEffect* a_vfx, 
		RE::BGSArtObject* a_art)
	{
		if (!a_vfx) {
			a_vm->TraceStack("VisualEffect is None", a_stackID);
			return;
		}
		if (!a_art) {
			a_vm->TraceStack("ArtObject is None", a_stackID);
			return;
		}

		a_vfx->data.artObject = a_art;
	}

	inline void Bind(VM& a_vm)
	{
		BIND(GetArtObject);
		BIND(GetArtObjectTotalCount);
		BIND(SetArtObject);

		logger::info("Registered visual effect functions"sv);
	}
}
