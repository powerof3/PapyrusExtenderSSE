#pragma once

#include "po3_functions.h"

//-----------------------------------------------------------------------------

namespace RE
{
	namespace po3_Offsets
	{
		constexpr std::uintptr_t CastSpell = 0x00980930;				// 1_5_80
		constexpr std::uintptr_t ShowMessageBox = 0x008AB180;			// 1_5_80
		constexpr std::uintptr_t ApplyImageSpaceModifier = 0x0024C860;	// 1_5_80
		constexpr std::uintptr_t ShakeCamera = 0x004F5420;              // 1_5_80

		constexpr std::uintptr_t GetHeadingAngle = 0x009035B0;          // 1_5_80
		constexpr std::uintptr_t ApplyHavokImpulse = 0x00908260;        // 1_5_80
		constexpr std::uintptr_t StartQuest = 0x00915E00;               // 1_5_80
		constexpr std::uintptr_t StopQuest = 0x009158D0;                // 1_5_80
		constexpr std::uintptr_t GetReference = 0x008EC3C0;             // 1_5_80
		constexpr std::uintptr_t PlayTESSound = 0x009172B0;             // 1_5_80
	}

	void CastSpell(BSScript::Internal::VirtualMachine* vm, UInt32 stackID, SpellItem* spell, TESObjectREFR* caster, TESObjectREFR* target);
	SInt32 ApplyImageSpaceModifier(TESImageSpaceModifier* imagespace, float abStrength, UInt64 unk00);
	UInt64 ShakeCamera(float strength, NiPoint3* pos, float duration);

	float GetHeadingAngle(BSScript::Internal::VirtualMachine* vm, uint32_t stackID, TESObjectREFR* source, TESObjectREFR* target);
	void ApplyHavokImpulse(BSScript::Internal::VirtualMachine* vm, uint32_t stackID, TESObjectREFR* target, float afX, float afY, float afZ, float magnitude);
	bool StartQuest(BSScript::Internal::VirtualMachine* vm, uint32_t stackID, TESQuest* quest);
	void StopQuest(BSScript::Internal::VirtualMachine* vm, uint32_t stackID, TESQuest* quest);
	TESObjectREFR* GetReference(BSScript::Internal::VirtualMachine* vm, uint32_t stackID, BGSRefAlias* refAlias);
	UInt32 PlayTESSound(BSScript::Internal::VirtualMachine* vm, uint32_t stackID, TESSound* sound, TESObjectREFR* source);
}