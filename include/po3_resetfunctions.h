#pragma once

#include "po3_functions.h"
#include "po3_graphicfunctions.h"


namespace RE
{
	template <class S, class F, class I>
	std::tuple<S, S, I, I, F, I, std::vector<S>, std::vector<S>> GetResetData(NiObjectNET* a_object)
	{
		std::tuple<S, S, I, I, F, I, std::vector<S>, std::vector<S>> t;

		if (!a_object->extra || a_object->extraDataSize == 0) {
			return t;
		}
		for (UInt16 i = 0; i < a_object->extraDataSize; i++) {
			auto extraData = a_object->extra[i];
			if (extraData) {
				std::string name = extraData->GetName().c_str();
				if (!name.empty()) {
					if (name == "PO3_TOGGLE") {
						std::get<0>(t) = static_cast<S>(extraData);
					}
					else if (name == "PO3_FACETXST") {
						std::get<1>(t) = static_cast<S>(extraData);
					}
					else if (name == "PO3_SKINTINT") {
						std::get<2>(t) = static_cast<I>(extraData);
					}
					else if (name == "PO3_HAIRTINT") {
						std::get<3>(t) = static_cast<I>(extraData);
					}
					else if (name == "PO3_ALPHA") {
						std::get<4>(t) = static_cast<F>(extraData);
					}
					else if (name == "PO3_HEADPARTALPHA") {
						std::get<5>(t) = static_cast<I>(extraData);
					}
					else if (name.find("PO3_TXST") != std::string::npos) {
						std::get<6>(t).push_back(static_cast<S>(extraData));
					}
					else if (name.find("PO3_SKINTXST") != std::string::npos) {
						std::get<7>(t).push_back(static_cast<S>(extraData));
					}
				}
			}
		}
		return t;
	}
	
	void StopAllSkinAlphaShaders(TESObjectREFR* a_ref);

	void ResetToggleData(Actor* a_actor, NiAVObject* a_root, NiStringsExtraData* a_data);
	void ResetAlphaData(Actor* a_actor, NiAVObject* a_root, NiFloatExtraData* a_data);
	void ResetHeadPartAlphaData(Actor* a_actor, NiAVObject* a_root, NiIntegersExtraData* a_data);
	void ResetSkinTintData(Actor* a_actor, NiAVObject* a_root, NiIntegersExtraData* a_data);
	void ResetHairTintData(Actor* a_actor, NiAVObject* a_root, NiIntegersExtraData* a_data);
	void ResetFaceTXSTData(Actor* a_actor, NiAVObject* a_root, NiStringsExtraData* a_data);
	void ResetTXSTData(Actor* a_actor, NiAVObject* a_root, const BSFixedString& a_folderName, std::vector<NiStringsExtraData*>& a_vec);
	void ResetSkinTXSTData(Actor* a_actor, NiAVObject* a_root, std::vector<NiStringsExtraData*>& a_vec);
}