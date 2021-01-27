#include "Papyrus/Light.h"


auto papyrusLight::GetLightColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectLIGH* a_light) -> RE::BGSColorForm*
{
	if (!a_light) {
		a_vm->TraceStack("Light is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	auto factory = RE::IFormFactory::GetFormFactoryByType(RE::FormType::ColorForm);
	if (factory) {
		auto color = static_cast<RE::BGSColorForm*>(factory->Create());
		if (color) {
			color->flags.reset(RE::BGSColorForm::Flag::kPlayable);
			color->color = a_light->data.color;
			return color;
		}
	}

	return nullptr;
}


auto papyrusLight::GetLightFade(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectLIGH* a_light) -> float
{
	if (!a_light) {
		a_vm->TraceStack("Light is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_light->fade;
}


auto papyrusLight::GetLightFOV(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectLIGH* a_light) -> float
{
	if (!a_light) {
		a_vm->TraceStack("Light is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_light->data.fov;
}


auto papyrusLight::GetLightRadius(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectLIGH* a_light) -> float
{
	if (!a_light) {
		a_vm->TraceStack("Light is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return static_cast<float>(a_light->data.radius);  //should ideally return as std::uint32_t but messed the return
}


auto papyrusLight::GetLightRGB(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectLIGH* a_light) -> std::vector<std::uint32_t>
{
	std::vector<std::uint32_t> vec(3, 0);

	if (!a_light) {
		a_vm->TraceStack("Light is None", a_stackID, Severity::kWarning);
		return vec;
	}

	auto color = a_light->data.color;
	for (std::size_t i = 0; i < 3; ++i) {
		vec[i] = color[i];
	}

	return vec;
}


auto papyrusLight::GetLightShadowDepthBias(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_lightObject) -> float
{
	if (!a_lightObject) {
		a_vm->TraceStack("Object is None", a_stackID, Severity::kWarning);
		return 1.0f;
	}

	const auto a_light = a_lightObject->As<RE::TESObjectLIGH>();
	if (!a_light) {
		a_vm->TraceStack("Object is not a Light form", a_stackID, Severity::kWarning);
		return 1.0f;
	}

	const auto xLightData = a_lightObject->extraList.GetByType<RE::ExtraLightData>();
	return xLightData ? xLightData->data.shadowDepthBias : 1.0f;
}


auto papyrusLight::GetLightType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectLIGH* a_light) -> std::uint32_t
{
	using FLAGS = RE::TES_LIGHT_FLAGS;

	if (!a_light) {
		a_vm->TraceStack("Light is None", a_stackID, Severity::kWarning);
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


void papyrusLight::SetLightColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectLIGH* a_light, RE::BGSColorForm* a_color)
{
	if (!a_light) {
		a_vm->TraceStack("Light is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_color) {
		a_vm->TraceStack("ColorForm is None", a_stackID, Severity::kWarning);
		return;
	}

	a_light->data.color = a_color->color;
}


void papyrusLight::SetLightFade(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectLIGH* a_light, float a_fade)
{
	if (!a_light) {
		a_vm->TraceStack("Light is None", a_stackID, Severity::kWarning);
		return;
	}

	a_light->fade = a_fade;
}


void papyrusLight::SetLightFOV(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectLIGH* a_light, float a_fov)
{
	if (!a_light) {
		a_vm->TraceStack("Light is None", a_stackID, Severity::kWarning);
		return;
	}

	a_light->data.fov = a_fov;
}


void papyrusLight::SetLightRadius(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectLIGH* a_light, float a_radius)
{
	if (!a_light) {
		a_vm->TraceStack("Light is None", a_stackID, Severity::kWarning);
		return;
	}

	a_light->data.radius = static_cast<std::uint32_t>(a_radius);
}


void papyrusLight::SetLightRGB(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectLIGH* a_light, std::vector<std::int32_t> a_rgb)
{
	if (!a_light) {
		a_vm->TraceStack("Light is None", a_stackID, Severity::kWarning);
		return;
	}
	if (a_rgb.empty()) {
		a_vm->TraceStack("RGB array is empty", a_stackID, Severity::kWarning);
		return;
	}

	auto& color = a_light->data.color;
	for (std::size_t i = 0; i < 3; ++i) {
		color[i] = static_cast<std::uint8_t>(a_rgb[i]);
	}
}


void papyrusLight::SetLightShadowDepthBias(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_lightObject, float a_depthBias)
{
	if (!a_lightObject) {
		a_vm->TraceStack("Object is None", a_stackID, Severity::kWarning);
		return;
	}

	const auto a_light = a_lightObject->As<RE::TESObjectLIGH>();
	if (!a_light) {
		a_vm->TraceStack("Object is not a Light form", a_stackID, Severity::kWarning);
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


void papyrusLight::SetLightType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectLIGH* a_light, std::uint32_t a_type)
{
	using FLAGS = RE::TES_LIGHT_FLAGS;

	if (!a_light) {
		a_vm->TraceStack("Light is None", a_stackID, Severity::kWarning);
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


auto papyrusLight::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusLight - couldn't get VMState"sv);
		return false;
	}

	auto constexpr Functions = "PO3_SKSEFunctions"sv;

	a_vm->RegisterFunction("GetLightColor"sv, Functions, GetLightColor);

	a_vm->RegisterFunction("GetLightFade"sv, Functions, GetLightFade, true);

	a_vm->RegisterFunction("GetLightFOV"sv, Functions, GetLightFOV, true);

	a_vm->RegisterFunction("GetLightRadius"sv, Functions, GetLightRadius, true);

	a_vm->RegisterFunction("GetLightRGB"sv, Functions, GetLightRGB);

	a_vm->RegisterFunction("GetLightShadowDepthBias"sv, Functions, GetLightShadowDepthBias);

	a_vm->RegisterFunction("GetLightType"sv, Functions, GetLightType);

	a_vm->RegisterFunction("SetLightRadius"sv, Functions, SetLightRadius);

	a_vm->RegisterFunction("SetLightRGB"sv, Functions, SetLightRGB);

	a_vm->RegisterFunction("SetLightColor"sv, Functions, SetLightColor);

	a_vm->RegisterFunction("SetLightFade"sv, Functions, SetLightFade);

	a_vm->RegisterFunction("SetLightFOV"sv, Functions, SetLightFOV);

	a_vm->RegisterFunction("SetLightShadowDepthBias"sv, Functions, SetLightShadowDepthBias);

	a_vm->RegisterFunction("SetLightType"sv, Functions, SetLightType);

	return true;
}
