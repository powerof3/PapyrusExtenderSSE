#pragma once

namespace Papyrus::Light
{
	inline RE::BGSColorForm* GetLightColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESObjectLIGH* a_light)
	{
		if (!a_light) {
			a_vm->TraceStack("Light is None", a_stackID);
			return nullptr;
		}

		auto factory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::BGSColorForm>();
		auto color = factory ? factory->Create() : nullptr;

		if (color) {
			color->flags.reset(RE::BGSColorForm::Flag::kPlayable);
			color->color = a_light->data.color;
			return color;
		}

		return nullptr;
	}

	inline float GetLightFade(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESObjectLIGH* a_light)
	{
		if (!a_light) {
			a_vm->TraceStack("Light is None", a_stackID);
			return 0.0f;
		}

		return a_light->fade;
	}

	inline float GetLightFOV(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESObjectLIGH* a_light)
	{
		if (!a_light) {
			a_vm->TraceStack("Light is None", a_stackID);
			return 0.0f;
		}

		return a_light->data.fov;
	}

	inline float GetLightRadius(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESObjectLIGH* a_light)
	{
		if (!a_light) {
			a_vm->TraceStack("Light is None", a_stackID);
			return 0.0f;
		}

		return static_cast<float>(a_light->data.radius);  //should ideally return as std::uint32_t but messed the return
	}

	inline std::vector<std::uint32_t> GetLightRGB(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESObjectLIGH* a_light)
	{
		std::vector<std::uint32_t> result(3, 0);

		if (!a_light) {
			a_vm->TraceStack("Light is None", a_stackID);
			return result;
		}

		const auto color = a_light->data.color;
		for (std::uint32_t i = 0; i < 3; ++i) {
			result[i] = color[i];
		}

		return result;
	}

	inline float GetLightShadowDepthBias(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESObjectREFR* a_lightObject)
	{
		if (!a_lightObject) {
			a_vm->TraceStack("Object is None", a_stackID);
			return 1.0f;
		}

		const auto base = a_lightObject->GetBaseObject();
		const auto light = base ? base->As<RE::TESObjectLIGH>() : nullptr;
		if (!light) {
			a_vm->TraceStack("Object is not a Light form", a_stackID);
			return 1.0f;
		}

		const auto xLightData = a_lightObject->extraList.GetByType<RE::ExtraLightData>();
		return xLightData ? xLightData->data.shadowDepthBias : 1.0f;
	}

	inline std::uint32_t GetLightType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::TESObjectLIGH* a_light)
	{
		using FLAGS = RE::TES_LIGHT_FLAGS;

		if (!a_light) {
			a_vm->TraceStack("Light is None", a_stackID);
			return 0;
		}

		const auto flags = a_light->data.flags;
		if (flags.all(FLAGS::kHemiShadow)) {
			return 1;
		}
		if (flags.all(FLAGS::kNone)) {
			return 2;
		}
		if (flags.all(FLAGS::kOmniShadow)) {
			return 3;
		}
		if (flags.all(FLAGS::kSpotlight)) {
			return 4;
		}
		if (flags.all(FLAGS::kSpotShadow)) {
			return 5;
		}
		return 0;
	}

	inline void SetLightColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectLIGH* a_light,
		RE::BGSColorForm* a_color)
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

	inline void SetLightFade(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectLIGH* a_light,
		float a_fade)
	{
		if (!a_light) {
			a_vm->TraceStack("Light is None", a_stackID);
			return;
		}

		a_light->fade = a_fade;
	}

	inline void SetLightFOV(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectLIGH* a_light,
		float a_fov)
	{
		if (!a_light) {
			a_vm->TraceStack("Light is None", a_stackID);
			return;
		}

		a_light->data.fov = a_fov;
	}

	inline void SetLightRadius(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectLIGH* a_light,
		float a_radius)
	{
		if (!a_light) {
			a_vm->TraceStack("Light is None", a_stackID);
			return;
		}

		a_light->data.radius = static_cast<std::uint32_t>(a_radius);
	}

	inline void SetLightRGB(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectLIGH* a_light,
		std::vector<std::int32_t> a_rgb)
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

	inline void SetLightShadowDepthBias(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_lightObject,
		float a_depthBias)
	{
		if (!a_lightObject) {
			a_vm->TraceStack("Object is None", a_stackID);
			return;
		}

		const auto base = a_lightObject->GetBaseObject();
		const auto light = base ? base->As<RE::TESObjectLIGH>() : nullptr;
		if (!light) {
			a_vm->TraceStack("Object is not a Light form", a_stackID);
			return;
		}

		const auto xLightData = a_lightObject->extraList.GetByType<RE::ExtraLightData>();
		if (xLightData) {
			xLightData->data.shadowDepthBias = a_depthBias;
		} else {
			auto newLightData = new RE::ExtraLightData();
			if (newLightData) {
				newLightData->data.shadowDepthBias = a_depthBias;
				a_lightObject->extraList.Add(newLightData);
			}
		}
	}

	inline void SetLightType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectLIGH* a_light,
		std::uint32_t a_type)
	{
		using FLAGS = RE::TES_LIGHT_FLAGS;

		if (!a_light) {
			a_vm->TraceStack("Light is None", a_stackID);
			return;
		}

		auto& flags = a_light->data.flags;
		switch (a_type) {
		case 1:
			flags = flags & FLAGS::kType | FLAGS::kHemiShadow;
			break;
		case 2:
			flags = flags & FLAGS::kType | FLAGS::kNone;
			break;
		case 3:
			flags = flags & FLAGS::kType | FLAGS::kOmniShadow;
			break;
		case 4:
			flags = flags & FLAGS::kType | FLAGS::kSpotlight;
			break;
		case 5:
			flags = flags & FLAGS::kType | FLAGS::kSpotShadow;
			break;
		default:
			break;
		}
	}

	inline void Bind(VM& a_vm)
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
