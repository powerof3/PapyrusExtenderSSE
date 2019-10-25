#pragma once

#include "po3_functions.h"

//-----------------------------------------------------------------------------

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

void CastSpell(RE::BSScript::Internal::VirtualMachine* vm, UInt32 stackID, RE::SpellItem* spell, RE::TESObjectREFR* caster, RE::TESObjectREFR* target);
SInt32 ApplyImageSpaceModifier(RE::TESImageSpaceModifier * imagespace, float abStrength, UInt64 unk00);
UInt64 ShakeCamera(float strength, RE::NiPoint3 * pos, float duration);

float GetHeadingAngle(RE::BSScript::Internal::VirtualMachine* vm, uint32_t stackID, RE::TESObjectREFR *source, RE::TESObjectREFR *target);
void ApplyHavokImpulse(RE::BSScript::Internal::VirtualMachine* vm, uint32_t stackID, RE::TESObjectREFR *target, float afX, float afY, float afZ, float magnitude);
bool StartQuest(RE::BSScript::Internal::VirtualMachine* vm, uint32_t stackID, RE::TESQuest *quest);
void StopQuest(RE::BSScript::Internal::VirtualMachine* vm, uint32_t stackID, RE::TESQuest *quest);
RE::TESObjectREFR* GetReference(RE::BSScript::Internal::VirtualMachine* vm, uint32_t stackID, RE::BGSRefAlias * refAlias);
UInt32 PlayTESSound(RE::BSScript::Internal::VirtualMachine* vm, uint32_t stackID, RE::TESSound * sound, RE::TESObjectREFR* source);


