#pragma once

#include "po3_functions.h"
#include "po3_graphicfunctions.h"


namespace RE
{
	UInt32 constexpr const_hash(const char* input, UInt32 hash = 5381)
	{
		return *input ?
			const_hash(input + 1, hash * 33 + static_cast<UInt32>(*input)) :
			hash;
	}


	template <class S, class I, class F>
	std::tuple<S, I, I, F, I, S, std::vector<S>, std::vector<S>, std::vector<S>> GetResetData(NiObjectNET* a_object)
	{
		std::tuple<S, I, I, F, I, S, std::vector<S>, std::vector<S>, std::vector<S>> t;

		if (!a_object || !a_object->extra || a_object->extraDataSize == 0) {
			return t;
		}
		for (UInt16 i = 0; i < a_object->extraDataSize; i++) {
			auto extraData = a_object->extra[i];
			if (extraData) {
				std::string name = extraData->GetName().c_str();
				if (!name.empty()) {
					switch (const_hash(name.c_str())) {
						case const_hash("PO3_TOGGLE"):
							std::get<0>(t) = static_cast<S>(extraData);
							break;
						case  const_hash("PO3_SKINTINT"):
							std::get<1>(t) = static_cast<I>(extraData);
							break;
						case const_hash("PO3_HAIRTINT"):
							std::get<2>(t) = static_cast<I>(extraData);
							break;
						case const_hash("PO3_ALPHA"):
							std::get<3>(t) = static_cast<F>(extraData);
							break;
						case const_hash("PO3_HEADPARTALPHA"):
							std::get<4>(t) = static_cast<I>(extraData);
							break;
						case const_hash("PO3_FACETXST"):
							std::get<5>(t) = static_cast<S>(extraData);
							break;
						default:
						{
							if (name.find("PO3_TXST") != std::string::npos) {
								std::get<6>(t).push_back(static_cast<S>(extraData));
							}
							else if (name.find("PO3_SKINTXST") != std::string::npos) {
								std::get<7>(t).push_back(static_cast<S>(extraData));
							}
							else if (name.find("PO3_SHADER") != std::string::npos) {
								std::get<8>(t).push_back(static_cast<S>(extraData));
							}
						}
						break;
					}
				}
			}
		}
		return t;
	}

	void StopAllSkinAlphaShaders(TESObjectREFR* a_ref);

	void ResetToggleData(NiAVObject* a_root, NiStringsExtraData* a_data);
	void ResetAlphaData(Actor* a_actor, NiAVObject* a_root, NiFloatExtraData* a_data);
	void ResetHeadPartAlphaData(Actor* a_actor, NiAVObject* a_root, NiIntegersExtraData* a_data);
	void ResetSkinTintData(Actor* a_actor, NiAVObject* a_root, NiIntegersExtraData* a_data);
	void ResetHairTintData(Actor* a_actor, NiAVObject* a_root, NiIntegersExtraData* a_data);
	void ResetFaceTXSTData(Actor* a_actor, NiAVObject* a_root, NiStringsExtraData* a_data);
	void ResetTXSTData(Actor* a_actor, NiAVObject* a_root, const BSFixedString& a_folderName, const std::vector<NiStringsExtraData*>& a_vec);
	void ResetSkinTXSTData(Actor* a_actor, NiAVObject* a_root, const std::vector<NiStringsExtraData*>& a_vec);
	void ResetShaderData(NiAVObject* a_root, const std::vector<NiStringsExtraData*>& a_vec);
}