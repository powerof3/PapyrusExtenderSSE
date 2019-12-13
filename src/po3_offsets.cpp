#include "po3_offsets.h"

namespace RE
{
	//-------------------------------------------------------

	void CastSpell(BSScript::Internal::VirtualMachine* vm, UInt32 stackID, SpellItem* spell, TESObjectREFR* caster, TESObjectREFR* target)
	{
		using func_t = function_type_t<decltype(&CastSpell)>;
		REL::Offset<func_t*> func(po3_Offsets::CastSpell);
		return func(vm, stackID, spell, caster, target);
	}

	//---------------------------------------------------------

	SInt32 ApplyImageSpaceModifier(TESImageSpaceModifier* imagespace, float abStrength, UInt64 unk0)
	{
		using func_t = function_type_t<decltype(&ApplyImageSpaceModifier)>;
		REL::Offset<func_t*> func(po3_Offsets::ApplyImageSpaceModifier);
		return func(imagespace, abStrength, unk0);
	}

	//---------------------------------------------------------

	UInt64 ShakeCamera(float strength, NiPoint3* pos, float duration)
	{
		using func_t = function_type_t<decltype(&ShakeCamera)>;
		REL::Offset<func_t*> func(po3_Offsets::ShakeCamera);
		return func(strength, pos, duration);
	}

	//-------------------------------------------------------

	float GetHeadingAngle(BSScript::Internal::VirtualMachine* vm, uint32_t stackID, TESObjectREFR* source, TESObjectREFR* target)
	{
		using func_t = function_type_t<decltype(&GetHeadingAngle)>;
		REL::Offset<func_t*> func(po3_Offsets::GetHeadingAngle);
		return func(vm, stackID, source, target);
	}

	//---------------------------------------------------------

	void ApplyHavokImpulse(BSScript::Internal::VirtualMachine* vm, uint32_t stackID, TESObjectREFR* target, float afX, float afY, float afZ, float magnitude)
	{
		using func_t = function_type_t<decltype(&ApplyHavokImpulse)>;
		REL::Offset<func_t*> func(po3_Offsets::ApplyHavokImpulse);
		return func(vm, stackID, target, afX, afY, afZ, magnitude);
	}

	//---------------------------------------------------------

	bool StartQuest(BSScript::Internal::VirtualMachine* vm, uint32_t stackID, TESQuest* quest)
	{
		using func_t = function_type_t<decltype(&StartQuest)>;
		REL::Offset<func_t*> func(po3_Offsets::StartQuest);
		return func(vm, stackID, quest);
	}

	//---------------------------------------------------------

	void StopQuest(BSScript::Internal::VirtualMachine* vm, uint32_t stackID, TESQuest* quest)
	{
		using func_t = function_type_t<decltype(&StopQuest)>;
		REL::Offset<func_t*> func(po3_Offsets::StopQuest);
		return func(vm, stackID, quest);
	}

	//---------------------------------------------------------

	TESObjectREFR* GetReference(BSScript::Internal::VirtualMachine* vm, uint32_t stackID, BGSRefAlias* refAlias)
	{
		using func_t = function_type_t<decltype(&GetReference)>;
		REL::Offset<func_t*> func(po3_Offsets::GetReference);
		return func(vm, stackID, refAlias);
	}

	//---------------------------------------------------------

	UInt32 PlayTESSound(BSScript::Internal::VirtualMachine* vm, uint32_t stackID, TESSound* sound, TESObjectREFR* source)
	{
		using func_t = function_type_t<decltype(&PlayTESSound)>;
		REL::Offset<func_t*> func(po3_Offsets::PlayTESSound);
		return func(vm, stackID, sound, source);
	}
}