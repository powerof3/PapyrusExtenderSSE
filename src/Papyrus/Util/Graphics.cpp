#include "Papyrus/Util/Graphics.h"

namespace GRAPHICS::MATERIAL
{
	RE::MATERIAL_ID get_material(const std::string& a_mat)
	{
		if (const auto mat = materialMap.find(a_mat)) {
			return *mat;
		}
		return RE::MATERIAL_ID::kNone;
	}

	std::string_view get_material(RE::MATERIAL_ID a_mat)
	{
		if (const auto mat = materialMap.find(a_mat)) {
			return *mat;
		}
		return "None"sv;
	}

	void for_each_material_type(const RE::NiPointer<RE::bhkWorldObject>& a_body, std::function<void(RE::MATERIAL_ID&)> a_visitor)
	{
		if (!a_body) {
			return;
		}

		const auto hkpBody = static_cast<RE::hkpWorldObject*>(a_body->referencedObject.get());

		if (const auto hkpShape = hkpBody ? hkpBody->GetShape() : nullptr) {
			if (hkpShape->type == RE::hkpShapeType::kMOPP) {
				const auto mopp = static_cast<const RE::hkpMoppBvTreeShape*>(hkpShape);
				const auto childShape = mopp ? mopp->child.childShape : nullptr;
				const auto compressedShape = childShape ? netimmerse_cast<RE::bhkCompressedMeshShape*>(childShape->userData) : nullptr;
				const auto shapeData = compressedShape ? compressedShape->data.get() : nullptr;

				if (shapeData) {
					for (auto& meshMaterial : shapeData->meshMaterials) {
						a_visitor(meshMaterial.materialID);
					}
				}
			} else if (const auto bhkShape = hkpShape->userData; bhkShape) {
				a_visitor(bhkShape->materialID);
			}
		}
	}
}
