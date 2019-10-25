#include "po3_offsets.h"

//-------------------------------------------------------

void CastSpell(RE::BSScript::Internal::VirtualMachine* vm, UInt32 stackID, RE::SpellItem* spell, RE::TESObjectREFR* caster, RE::TESObjectREFR* target)
{
	using func_t = function_type_t<decltype(&CastSpell)>;
	REL::Offset<func_t*> func(po3_Offsets::CastSpell);
	return func(vm, stackID, spell, caster, target);
}

//---------------------------------------------------------

SInt32 ApplyImageSpaceModifier(RE::TESImageSpaceModifier * imagespace, float abStrength, UInt64 unk0)
{
	using func_t = function_type_t<decltype(&ApplyImageSpaceModifier)>;
	REL::Offset<func_t*> func(po3_Offsets::ApplyImageSpaceModifier);
	return func(imagespace, abStrength, unk0);
}

//---------------------------------------------------------

UInt64 ShakeCamera(float strength, RE::NiPoint3 * pos, float duration)
{
	using func_t = function_type_t<decltype(&ShakeCamera)>;
	REL::Offset<func_t*> func(po3_Offsets::ShakeCamera);
	return func(strength, pos, duration);
}

//-------------------------------------------------------

float GetHeadingAngle(RE::BSScript::Internal::VirtualMachine* vm, uint32_t stackID, RE::TESObjectREFR *source, RE::TESObjectREFR *target)
{
	using func_t = function_type_t<decltype(&GetHeadingAngle)>;
	REL::Offset<func_t*> func(po3_Offsets::GetHeadingAngle);
	return func(vm, stackID, source, target);
}

//---------------------------------------------------------

void ApplyHavokImpulse(RE::BSScript::Internal::VirtualMachine* vm, uint32_t stackID, RE::TESObjectREFR *target, float afX, float afY, float afZ, float magnitude)
{
	using func_t = function_type_t<decltype(&ApplyHavokImpulse)>;
	REL::Offset<func_t*> func(po3_Offsets::ApplyHavokImpulse);
	return func(vm, stackID, target, afX, afY, afZ, magnitude);
}

//---------------------------------------------------------

bool StartQuest(RE::BSScript::Internal::VirtualMachine* vm, uint32_t stackID, RE::TESQuest *quest)
{
	using func_t = function_type_t<decltype(&StartQuest)>;
	REL::Offset<func_t*> func(po3_Offsets::StartQuest);
	return func(vm, stackID, quest);
}

//---------------------------------------------------------

void StopQuest(RE::BSScript::Internal::VirtualMachine* vm, uint32_t stackID, RE::TESQuest *quest)
{
	using func_t = function_type_t<decltype(&StopQuest)>;
	REL::Offset<func_t*> func(po3_Offsets::StopQuest);
	return func(vm, stackID, quest);
}

//---------------------------------------------------------

RE::TESObjectREFR* GetReference(RE::BSScript::Internal::VirtualMachine* vm, uint32_t stackID, RE::BGSRefAlias * refAlias)
{
	using func_t = function_type_t<decltype(&GetReference)>;
	REL::Offset<func_t*> func(po3_Offsets::GetReference);
	return func(vm, stackID, refAlias);
}

//---------------------------------------------------------

UInt32 PlayTESSound(RE::BSScript::Internal::VirtualMachine* vm, uint32_t stackID, RE::TESSound * sound, RE::TESObjectREFR* source)
{
	using func_t = function_type_t<decltype(&PlayTESSound)>;
	REL::Offset<func_t*> func(po3_Offsets::PlayTESSound);
	return func(vm, stackID, sound, source);
}
