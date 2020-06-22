#pragma once

#include "main.h"


namespace papyrusObjectReference
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;


	std::vector<RE::TESForm*> AddAllInventoryItemsToArray(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem);
	
	void AddAllInventoryItemsToList(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSListForm* a_list, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem);
	
	void AddKeywordToRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSKeyword* a_add);
	
	std::vector<RE::TESObjectREFR*> FindAllReferencesOfFormType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_origin, UInt32 a_formType, float a_radius);

	std::vector<RE::TESObjectREFR*> FindAllReferencesOfType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESForm* a_formOrList, float a_radius);

	std::vector<RE::TESObjectREFR*> FindAllReferencesWithKeyword(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_origin, RE::TESForm* a_formOrList, float a_radius, bool a_matchAll);
	
	RE::Actor* GetActorCause(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref);
	
	RE::Actor* GetClosestActorFromRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, bool a_ignorePlayer);

	void IterateOverMagicTempEffects(std::function<bool(RE::BSTempEffect* a_tempEffect)> a_fn);
	
	std::vector<RE::BGSArtObject*> GetAllArtObjects(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref);
	
	std::vector<RE::TESEffectShader*>GetAllEffectShaders(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref);
	
	float GetEffectShaderDuration(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESEffectShader* a_effectShader);
	
	RE::Actor* GetRandomActorFromRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, float a_radius, bool a_ignorePlayer);
	
	UInt32 HasArtObject(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSArtObject* a_art, bool a_active);
	
	UInt32 HasEffectShader(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESEffectShader* a_effectShader, bool a_active);
	
	bool IsLoadDoor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_door);

	bool HasNiExtraData(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BSFixedString a_name);

	void MoveToNearestNavmeshLocation(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref);

	bool RemoveKeywordFromRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSKeyword* a_remove);

	void ReplaceKeywordOnRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSKeyword* a_remove, RE::BGSKeyword* a_add);

	void SetEffectShaderDuration(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESEffectShader* a_effectShader, float a_time, bool a_absolute);
	
	void SetShaderType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESObjectREFR* a_template, RE::BSFixedString a_filter, UInt32 a_shaderType, SInt32 a_textureType, bool a_noWeapons, bool a_noAlpha);
	
	void SetupBodyPartGeometry(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_miscItem, RE::Actor* a_actor);

	void StopAllShaders_Impl(RE::TESObjectREFR* a_ref);

	void StopAllShaders(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref);

	void StopAllSkinAlphaShaders_Impl(RE::TESObjectREFR* a_ref);
		
	void StopArtObject(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSArtObject* a_art);


	bool RegisterFuncs(VM* a_vm);
}
