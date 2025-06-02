#pragma once

namespace Papyrus::Light
{
	 RE::BGSColorForm* GetLightColor(STATIC_ARGS, const RE::TESObjectLIGH* a_light);
	 float GetLightFade(STATIC_ARGS, const RE::TESObjectLIGH* a_light);
	 float GetLightFOV(STATIC_ARGS, const RE::TESObjectLIGH* a_light);
	 float GetLightRadius(STATIC_ARGS, const RE::TESObjectLIGH* a_light);
	 std::vector<std::uint32_t> GetLightRGB(STATIC_ARGS, const RE::TESObjectLIGH* a_light);
	 float GetLightShadowDepthBias(STATIC_ARGS, RE::TESObjectREFR* a_lightObject);
	 std::uint32_t GetLightType(STATIC_ARGS, const RE::TESObjectLIGH* a_light);
	 void SetLightColor(STATIC_ARGS, RE::TESObjectLIGH* a_light, RE::BGSColorForm* a_color);
	 void SetLightFade(STATIC_ARGS, RE::TESObjectLIGH* a_light, float a_fade);
	 void SetLightFOV(STATIC_ARGS, RE::TESObjectLIGH* a_light, float a_fov);
	 void SetLightRadius(STATIC_ARGS, RE::TESObjectLIGH* a_light, float a_radius);
	 void SetLightRGB(STATIC_ARGS, RE::TESObjectLIGH* a_light, std::vector<std::int32_t> a_rgb);
	 void SetLightShadowDepthBias(STATIC_ARGS, RE::TESObjectREFR* a_lightObject, float a_depthBias);
	 void SetLightType(STATIC_ARGS, RE::TESObjectLIGH* a_light, std::uint32_t a_type);

	 void Bind(VM& a_vm);
}
