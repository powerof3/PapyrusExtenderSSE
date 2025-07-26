#pragma once

namespace Papyrus::ObjectReference
{
	bool                              ActorInRangeHasEffect(STATIC_ARGS, RE::TESObjectREFR* a_ref, float a_radius, RE::EffectSetting* a_effect, bool a_ignorePlayer);
	std::vector<RE::TESForm*>         AddAllItemsToArray(STATIC_ARGS, RE::TESObjectREFR* a_ref, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem);
	void                              AddAllItemsToList(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::BGSListForm* a_list, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem);
	std::vector<RE::TESForm*>         AddItemsOfTypeToArray(STATIC_ARGS, RE::TESObjectREFR* a_ref, std::uint32_t a_formType, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem);
	void                              AddItemsOfTypeToList(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::BGSListForm* a_list, std::uint32_t a_formType, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem);
	void                              AddKeywordToRef(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::BGSKeyword* a_keyword);
	void                              CastEx(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::TESForm* a_spell, RE::TESObjectREFR* a_target, RE::Actor* a_blameActor, std::uint32_t a_source);
	RE::TESForm*                      FindFirstItemInList(STATIC_ARGS, RE::TESObjectREFR* a_ref, const RE::BGSListForm* a_list);
	std::vector<RE::TESQuest*>        GetActiveAssociatedQuests(STATIC_ARGS, RE::TESObjectREFR* a_ref, bool a_allowEmptyStages = true);  // SeaSparrow - New Functions
	std::vector<RE::TESObjectREFR*>   GetActivateChildren(STATIC_ARGS, RE::TESObjectREFR* a_ref);
	RE::BSFixedString                 GetActiveGamebryoAnimation(STATIC_ARGS, RE::TESObjectREFR* a_ref);
	std::vector<RE::ActiveEffect*>    GetActiveMagicEffects(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::EffectSetting* a_mgef);
	RE::Actor*                        GetActorCause(STATIC_ARGS, RE::TESObjectREFR* a_ref);
	std::vector<RE::BGSArtObject*>    GetAllArtObjects(STATIC_ARGS, RE::TESObjectREFR* a_ref);
	std::vector<RE::TESQuest*>        GetAllAssociatedQuests(STATIC_ARGS, RE::TESObjectREFR* a_ref, bool a_allowEmptyStages = true);  // SeaSparrow - New Functions
	std::vector<RE::TESEffectShader*> GetAllEffectShaders(STATIC_ARGS, RE::TESObjectREFR* a_ref);
	RE::Actor*                        GetClosestActorFromRef(STATIC_ARGS, const RE::TESObjectREFR* a_ref, bool a_ignorePlayer);
	float                             GetEffectShaderDuration(STATIC_ARGS, RE::TESObjectREFR* a_ref, const RE::TESEffectShader* a_effectShader);
	RE::TESObjectREFR*                GetDoorDestination(STATIC_ARGS, RE::TESObjectREFR* a_ref);
	std::vector<RE::TESObjectREFR*>   GetLinkedChildren(STATIC_ARGS, RE::TESObjectREFR* a_ref, const RE::BGSKeyword* a_keyword);
	std::vector<RE::TESForm*>         GetMagicEffectSource(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::EffectSetting* a_mgef);
	std::vector<std::string>          GetMaterialType(STATIC_ARGS, RE::TESObjectREFR* a_ref, std::string a_nodeName);
	std::int32_t                      GetMotionType(STATIC_ARGS, RE::TESObjectREFR* a_ref);
	std::int32_t                      GetNumActorsWithEffectInRange(STATIC_ARGS, RE::TESObjectREFR* a_ref, float a_radius, RE::EffectSetting* a_effect, bool a_ignorePlayer);
	RE::Actor*                        GetRandomActorFromRef(STATIC_ARGS, RE::TESObjectREFR* a_ref, float a_radius, bool a_ignorePlayer);
	std::vector<RE::TESForm*>         GetQuestItems(STATIC_ARGS, RE::TESObjectREFR* a_ref, bool a_noEquipped, bool a_noFavourited);
	std::vector<RE::BGSBaseAlias*>    GetRefAliases(STATIC_ARGS, RE::TESObjectREFR* a_ref);
	std::int32_t                      GetRefCount(STATIC_ARGS, RE::TESObjectREFR* a_ref);
	std::int32_t                      GetStoredSoulSize(STATIC_ARGS, RE::TESObjectREFR* a_ref);
	std::uint32_t                     HasArtObject(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::BGSArtObject* a_art, bool a_active);
	std::uint32_t                     HasEffectShader(STATIC_ARGS, RE::TESObjectREFR* a_ref, const RE::TESEffectShader* a_effectShader, bool a_active);
	bool                              IsCasting(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::TESForm* a_form);
	bool                              IsLoadDoor(STATIC_ARGS, const RE::TESObjectREFR* a_ref);
	bool                              IsQuestItem(STATIC_ARGS, const RE::TESObjectREFR* a_ref);
	bool                              IsRefInWater(STATIC_ARGS, RE::TESObjectREFR* a_ref);
	bool                              IsRefNodeInWater(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::BSFixedString a_nodeName);
	bool                              IsRefUnderwater(STATIC_ARGS, RE::TESObjectREFR* a_ref);
	bool                              IsVIP(STATIC_ARGS, RE::TESObjectREFR* a_ref);
	bool                              HasNiExtraData(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::BSFixedString a_name);
	void                              MoveToNearestNavmeshLocation(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref);
	void                              RemoveAllModItems(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::BSFixedString a_modName, bool a_unequip);
	bool                              RemoveKeywordFromRef(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::BGSKeyword* a_keyword);
	void                              RemoveListFromContainer(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::BGSListForm* a_formList, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem, RE::TESObjectREFR* a_destination);
	void                              ReplaceKeywordOnRef(STATIC_ARGS, RE::TESObjectREFR* a_ref, const RE::BGSKeyword* a_remove, RE::BGSKeyword* a_add);
	void                              SetBaseObject(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::TESForm* a_base);
	void                              SetCollisionLayer(STATIC_ARGS, RE::TESObjectREFR* a_ref, std::string a_nodeName, std::uint32_t a_colLayer);
	bool                              SetDoorDestination(STATIC_ARGS, const RE::TESObjectREFR* a_ref, RE::TESObjectREFR* a_door);
	void                              SetEffectShaderDuration(STATIC_ARGS, RE::TESObjectREFR* a_ref, const RE::TESEffectShader* a_effectShader, float a_time, bool a_absolute);
	void                              SetKey(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::TESKey* a_key);
	void                              SetLinkedRef(STATIC_ARGS, RE::TESObjectREFR* a_ref, [[maybe_unused]] RE::TESObjectREFR* a_targetRef, [[maybe_unused]] RE::BGSKeyword* a_keyword);
	void                              SetMaterialType(STATIC_ARGS, RE::TESObjectREFR* a_ref, std::string a_newMaterialType, std::string a_oldMaterialType, std::string a_nodeName);
	void                              StopAllShaders(STATIC_ARGS, RE::TESObjectREFR* a_ref);
	void                              StopArtObject(STATIC_ARGS, RE::TESObjectREFR* a_ref, const RE::BGSArtObject* a_art);

	void Bind(VM& a_vm);
}
