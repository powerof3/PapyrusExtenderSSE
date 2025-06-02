#pragma once

namespace Papyrus::VisualEffect
{
	RE::BGSArtObject* GetArtObject(STATIC_ARGS, const RE::BGSReferenceEffect* a_vfx);
	std::uint32_t     GetArtObjectTotalCount(STATIC_ARGS, const RE::BGSReferenceEffect* a_vfx, bool a_active);
	void              SetArtObject(STATIC_ARGS, RE::BGSReferenceEffect* a_vfx, RE::BGSArtObject* a_art);

	void Bind(VM& a_vm);
}
