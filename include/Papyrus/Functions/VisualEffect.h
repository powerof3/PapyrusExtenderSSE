#pragma once

namespace Papyrus::VisualEffect
{
	inline RE::BGSArtObject* GetArtObject(STATIC_ARGS, const RE::BGSReferenceEffect* a_vfx)
	{
		if (!a_vfx) {
			a_vm->TraceStack("VisualEffect is None", a_stackID);
			return nullptr;
		}

		return a_vfx->data.artObject;
	}

	inline std::uint32_t GetArtObjectTotalCount(STATIC_ARGS, const RE::BGSReferenceEffect* a_vfx, bool a_active)
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
			processLists->ForEachModelEffect([&](const RE::ModelReferenceEffect* a_modelEffect) {
				if (a_modelEffect->artObject == art && !a_active || !a_modelEffect->finished) {
					count++;
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
		}

		return count;
	}

	inline void SetArtObject(STATIC_ARGS, RE::BGSReferenceEffect* a_vfx, RE::BGSArtObject* a_art)
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
