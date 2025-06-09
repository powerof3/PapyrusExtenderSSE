#include "Papyrus/Functions/Light.h"

namespace Papyrus::Light
{
	RE::BGSColorForm* GetLightColor(STATIC_ARGS, [[maybe_unused]] const RE::TESObjectLIGH* a_light)
	{
		a_vm->TraceStack("Function is deprecated. Use GetLightRGB instead", a_stackID);
		return nullptr;
	}

	float GetLightFade(STATIC_ARGS, const RE::TESObjectLIGH* a_light)
	{
		if (!a_light) {
			a_vm->TraceStack("Light is None", a_stackID);
			return 0.0f;
		}

		return a_light->fade;
	}

	float GetLightFOV(STATIC_ARGS, const RE::TESObjectLIGH* a_light)
	{
		if (!a_light) {
			a_vm->TraceStack("Light is None", a_stackID);
			return 0.0f;
		}

		return a_light->data.fov;
	}

	float GetLightRadius(STATIC_ARGS, const RE::TESObjectLIGH* a_light)
	{
		if (!a_light) {
			a_vm->TraceStack("Light is None", a_stackID);
			return 0.0f;
		}

		return static_cast<float>(a_light->data.radius);  //should ideally return as std::uint32_t but messed the return
	}

	std::vector<std::uint32_t> GetLightRGB(STATIC_ARGS, const RE::TESObjectLIGH* a_light)
	{
		if (!a_light) {
			a_vm->TraceStack("Light is None", a_stackID);
			return {};
		}

		const auto& color = a_light->data.color;
		return { static_cast<std::uint32_t>(color.red), static_cast<std::uint32_t>(color.green), static_cast<std::uint32_t>(color.blue) };
	}

	float GetLightShadowDepthBias(STATIC_ARGS, RE::TESObjectREFR* a_lightObject)
	{
		if (!a_lightObject) {
			a_vm->TraceStack("Object is None", a_stackID);
			return 1.0f;
		}

		const auto base = a_lightObject->GetBaseObject();
		const auto light = base ? base->As<RE::TESObjectLIGH>() : nullptr;
		if (!light) {
			a_vm->TraceForm(a_lightObject, "is not a Light form", a_stackID);
			return 1.0f;
		}

		const auto xLightData = a_lightObject->extraList.GetByType<RE::ExtraLightData>();
		return xLightData ? xLightData->data.shadowDepthBias : 1.0f;
	}

	std::uint32_t GetLightType(STATIC_ARGS, const RE::TESObjectLIGH* a_light)
	{
		using FLAGS = RE::TES_LIGHT_FLAGS;

		if (!a_light) {
			a_vm->TraceStack("Light is None", a_stackID);
			return 0;
		}

		const auto flags = a_light->data.flags;
		if (flags.none(FLAGS::kHemiShadow) && flags.none(FLAGS::kOmniShadow) && flags.none(FLAGS::kSpotlight) && flags.none(FLAGS::kSpotShadow)) {  // Omni
			return 2;
		}
		if (flags.any(FLAGS::kHemiShadow)) {
			return 1;
		}
		if (flags.any(FLAGS::kOmniShadow)) {
			return 3;
		}
		if (flags.any(FLAGS::kSpotlight)) {
			return 4;
		}
		if (flags.any(FLAGS::kSpotShadow)) {
			return 5;
		}

		return 0;
	}

	void SetLightColor(STATIC_ARGS, RE::TESObjectLIGH* a_light, RE::BGSColorForm* a_color)
	{
		if (!a_light) {
			a_vm->TraceStack("Light is None", a_stackID);
			return;
		}
		if (!a_color) {
			a_vm->TraceStack("ColorForm is None", a_stackID);
			return;
		}

		a_light->data.color = a_color->color;
	}

	void SetLightFade(STATIC_ARGS, RE::TESObjectLIGH* a_light, float a_fade)
	{
		if (!a_light) {
			a_vm->TraceStack("Light is None", a_stackID);
			return;
		}

		a_light->fade = a_fade;
	}

	void SetLightFOV(STATIC_ARGS, RE::TESObjectLIGH* a_light, float a_fov)
	{
		if (!a_light) {
			a_vm->TraceStack("Light is None", a_stackID);
			return;
		}

		a_light->data.fov = a_fov;
	}

	void SetLightRadius(STATIC_ARGS, RE::TESObjectLIGH* a_light, float a_radius)
	{
		if (!a_light) {
			a_vm->TraceStack("Light is None", a_stackID);
			return;
		}

		a_light->data.radius = static_cast<std::uint32_t>(a_radius);
	}

	void SetLightRGB(STATIC_ARGS, RE::TESObjectLIGH* a_light, std::vector<std::int32_t> a_rgb)
	{
		if (!a_light) {
			a_vm->TraceStack("Light is None", a_stackID);
			return;
		}

		if (a_rgb.size() != 3) {
			a_vm->TraceForm(a_light, "RGB array is empty", a_stackID);
			return;
		}

		auto& color = a_light->data.color;
		for (std::uint32_t i = 0; i < 3; ++i) {
			color[i] = static_cast<std::uint8_t>(a_rgb[i]);
		}
	}

	void SetLightShadowDepthBias(STATIC_ARGS, RE::TESObjectREFR* a_lightObject, float a_depthBias)
	{
		if (!a_lightObject) {
			a_vm->TraceStack("Object is None", a_stackID);
			return;
		}

		const auto base = a_lightObject->GetBaseObject();
		const auto light = base ? base->As<RE::TESObjectLIGH>() : nullptr;
		if (!light) {
			a_vm->TraceForm(a_lightObject, "is not a Light form", a_stackID);
			return;
		}

		if (const auto xLightData = a_lightObject->extraList.GetByType<RE::ExtraLightData>(); xLightData) {
			xLightData->data.shadowDepthBias = a_depthBias;
		} else {
			if (const auto newLightData = new RE::ExtraLightData(); newLightData) {
				newLightData->data.shadowDepthBias = a_depthBias;
				a_lightObject->extraList.Add(newLightData);
			}
		}
	}

	void SetLightType(STATIC_ARGS, RE::TESObjectLIGH* a_light, std::uint32_t a_type)
	{
		using FLAGS = RE::TES_LIGHT_FLAGS;

		if (!a_light) {
			a_vm->TraceStack("Light is None", a_stackID);
			return;
		}

		auto& flags = a_light->data.flags;
		switch (a_type) {
		case 1:
			{
				flags.reset(FLAGS::kOmniShadow, FLAGS::kSpotlight, FLAGS::kSpotShadow);
				flags.set(FLAGS::kHemiShadow);
			}
			break;
		case 2:
			{
				flags.reset(FLAGS::kHemiShadow, FLAGS::kOmniShadow, FLAGS::kSpotlight, FLAGS::kSpotShadow);
			}
			break;
		case 3:
			{
				flags.reset(FLAGS::kHemiShadow, FLAGS::kSpotlight, FLAGS::kSpotShadow);
				flags.set(FLAGS::kOmniShadow);
			}
			break;
		case 4:
			{
				flags.reset(FLAGS::kHemiShadow, FLAGS::kOmniShadow, FLAGS::kSpotShadow);
				flags.set(FLAGS::kSpotlight);
			}
			break;
		case 5:
			{
				flags.reset(FLAGS::kHemiShadow, FLAGS::kOmniShadow, FLAGS::kSpotlight);
				flags.set(FLAGS::kSpotShadow);
			}
			break;
		default:
			break;
		}
	}

	void Bind(VM& a_vm)
	{
		BIND(GetLightColor);
		BIND(GetLightFade, true);
		BIND(GetLightFOV, true);
		BIND(GetLightRadius, true);
		BIND(GetLightRGB);
		BIND(GetLightShadowDepthBias);
		BIND(GetLightType);
		BIND(SetLightRadius);
		BIND(SetLightRGB);
		BIND(SetLightColor);
		BIND(SetLightFade);
		BIND(SetLightFOV);
		BIND(SetLightShadowDepthBias);
		BIND(SetLightType);

		logger::info("Registered light functions"sv);
	}
}
