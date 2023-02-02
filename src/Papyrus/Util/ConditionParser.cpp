#include "Papyrus/Util/ConditionParser.h"

namespace CONDITION
{
	ConditionData::ConditionData(const RE::TESConditionItem* a_condition) :
		conditionItem(*a_condition->data.object),
		functionID(*a_condition->data.functionData.function),
		param1(a_condition->data.functionData.params[0]),
		param2(a_condition->data.functionData.params[1]),
		opCode(a_condition->data.flags.opCode),
		value(a_condition->data.comparisonValue.f),
		andOr(a_condition->data.flags.isOR)
	{}

    bool ConditionData::operator==(RE::TESConditionItem* a_item) const
    {
		return *this == ConditionData(a_item);
    }

    PARAMS GetFuncType(FUNC_ID a_funcID)
	{
		PARAMS paramPair;

		switch (a_funcID) {
		case FUNC_ID::kGetWantBlocking:
		case FUNC_ID::kGetLocked:
		case FUNC_ID::kGetSecondsPassed:
		case FUNC_ID::kGetCurrentTime:
		case FUNC_ID::kGetScale:
		case FUNC_ID::kIsMoving:
		case FUNC_ID::kIsTurning:
		case FUNC_ID::kGetDisabled:
		case FUNC_ID::kGetDisease:
		case FUNC_ID::kGetClothingValue:
		case FUNC_ID::kGetDead:
		case FUNC_ID::kGetGold:
		case FUNC_ID::kGetSleeping:
		case FUNC_ID::kGetTalkedToPC:
		case FUNC_ID::kGetAlarmed:
		case FUNC_ID::kIsRaining:
		case FUNC_ID::kGetAttacked:
		case FUNC_ID::kGetIsCreature:
		case FUNC_ID::kGetLockLevel:
		case FUNC_ID::kIsSnowing:
		case FUNC_ID::kGetRandomPercent:
		case FUNC_ID::kGetLevel:
		case FUNC_ID::kIsRotating:
		case FUNC_ID::kGetIsAlerted:
		case FUNC_ID::kIsWeaponMagicOut:
		case FUNC_ID::kIsTorchOut:
		case FUNC_ID::kIsShieldOut:
		case FUNC_ID::kIsFacingUp:
		case FUNC_ID::kGetKnockedState:
		case FUNC_ID::kGetWeaponAnimType:
		case FUNC_ID::kGetCurrentAIPackage:
		case FUNC_ID::kIsWaiting:
		case FUNC_ID::kIsIdlePlaying:
		case FUNC_ID::kIsIntimidatedByPlayer:
		case FUNC_ID::kGetActorAggroRadiusViolated:
		case FUNC_ID::kIsGreetingPlayer:
		case FUNC_ID::kIsGuard:
		case FUNC_ID::kHasBeenEaten:
		case FUNC_ID::kGetStaminaPercentage:
		case FUNC_ID::kSameFactionAsPC:
		case FUNC_ID::kSameRaceAsPC:
		case FUNC_ID::kSameSexAsPC:
		case FUNC_ID::kIsTalking:
		case FUNC_ID::kGetWalkSpeed:
		case FUNC_ID::kGetCurrentAIProcedure:
		case FUNC_ID::kGetTrespassWarningLevel:
		case FUNC_ID::kIsTrespassing:
		case FUNC_ID::kIsInMyOwnedCell:
		case FUNC_ID::kGetWindSpeed:
		case FUNC_ID::kGetCurrentWeatherPercent:
		case FUNC_ID::kIsContinuingPackagePCNear:
		case FUNC_ID::kCanHaveFlames:
		case FUNC_ID::kHasFlames:
		case FUNC_ID::kGetOpenState:
		case FUNC_ID::kGetSitting:
		case FUNC_ID::kGetDayOfWeek:
		case FUNC_ID::kIsPCSleeping:
		case FUNC_ID::kIsPCAMurderer:
		case FUNC_ID::kIsSwimming:
		case FUNC_ID::kGetAmountSoldStolen:
		case FUNC_ID::kGetIgnoreCrime:
		case FUNC_ID::kGetDestroyed:
		case FUNC_ID::kGetDefaultOpen:
		case FUNC_ID::kGetAnimAction:
		case FUNC_ID::kGetVATSMode:
		case FUNC_ID::kGetPersuasionNumber:
		case FUNC_ID::kGetVampireFeed:
		case FUNC_ID::kGetCannibal:
		case FUNC_ID::kGetClassDefaultMatch:
		case FUNC_ID::kGetVatsTargetHeight:
		case FUNC_ID::kGetIsGhost:
		case FUNC_ID::kGetUnconscious:
		case FUNC_ID::kGetRestrained:
		case FUNC_ID::kIsInDialogueWithPlayer:
		case FUNC_ID::kGetIsPlayableRace:
		case FUNC_ID::kGetOffersServicesNow:
		case FUNC_ID::kIsWeaponOut:
		case FUNC_ID::kIsTimePassing:
		case FUNC_ID::kIsPleasant:
		case FUNC_ID::kIsCloudy:
		case FUNC_ID::kIsSmallBump:
		case FUNC_ID::kIsHorseStolen:
		case FUNC_ID::kIsLeftUp:
		case FUNC_ID::kIsSneaking:
		case FUNC_ID::kIsRunning:
		case FUNC_ID::kGetFriendHit:
		case FUNC_ID::kIsInInterior:
		case FUNC_ID::kIsWaterObject:
		case FUNC_ID::kGetPlayerAction:
		case FUNC_ID::kIsActorUsingATorch:
		case FUNC_ID::kIsXBox:
		case FUNC_ID::kGetPairedAnimation:
		case FUNC_ID::kIsActorAVictim:
		case FUNC_ID::kGetTotalPersuasionNumber:
		case FUNC_ID::kGetIdleDoneOnce:
		case FUNC_ID::kGetNoRumors:
		case FUNC_ID::kGetCombatState:
		case FUNC_ID::kIsRidingMount:
		case FUNC_ID::kIsFleeing:
		case FUNC_ID::kIsInDangerousWater:
		case FUNC_ID::kGetIgnoreFriendlyHits:
		case FUNC_ID::kIsPlayersLastRiddenMount:
		case FUNC_ID::kIsActor:
		case FUNC_ID::kIsEssential:
		case FUNC_ID::kIsPlayerMovingIntoNewSpace:
		case FUNC_ID::kGetTimeDead:
		case FUNC_ID::kIsChild:
		case FUNC_ID::kGetLastPlayerAction:
		case FUNC_ID::kGetCrimeGoldViolent:
		case FUNC_ID::kGetCrimeGoldNonviolent:
		case FUNC_ID::kGetHitLocation:
		case FUNC_ID::kIsPC1stPerson:
		case FUNC_ID::kGetCauseofDeath:
		case FUNC_ID::kIsBribedbyPlayer:
		case FUNC_ID::kGetGroupMemberCount:
		case FUNC_ID::kGetGroupTargetCount:
		case FUNC_ID::kGetPlantedExplosive:
		case FUNC_ID::kIsScenePackageRunning:
		case FUNC_ID::kGetHealthPercentage:
		case FUNC_ID::kGetDialogueEmotion:
		case FUNC_ID::kGetDialogueEmotionValue:
		case FUNC_ID::kGetPlayerTeammate:
		case FUNC_ID::kGetPlayerTeammateCount:
		case FUNC_ID::kGetActorCrimePlayerEnemy:
		case FUNC_ID::kGetCrimeGold:
		case FUNC_ID::kGetDestructionStage:
		case FUNC_ID::kIsProtected:
		case FUNC_ID::kIsCarryable:
		case FUNC_ID::kGetConcussed:
		case FUNC_ID::kGetMapMarkerVisible:
		case FUNC_ID::kGetKillingBlowLimb:
		case FUNC_ID::kCanPayCrimeGold:
		case FUNC_ID::kGetDaysInJail:
		case FUNC_ID::kEPAlchemyGetMakingPoison:
		case FUNC_ID::kGetAllowWorldInteractions:
		case FUNC_ID::kGetLastHitCritical:
		case FUNC_ID::kGetIsLockBroken:
		case FUNC_ID::kIsPS3:
		case FUNC_ID::kIsWin32:
		case FUNC_ID::kGetXPForNextLevel:
		case FUNC_ID::kGetInfamy:
		case FUNC_ID::kGetInfamyViolent:
		case FUNC_ID::kGetInfamyNonViolent:
		case FUNC_ID::kIsGoreDisabled:
		case FUNC_ID::kGetActorsInHigh:
		case FUNC_ID::kHasLoaded3D:
		case FUNC_ID::kIsSprinting:
		case FUNC_ID::kIsBlocking:
		case FUNC_ID::kGetAttackState:
		case FUNC_ID::kIsBleedingOut:
		case FUNC_ID::kGetMovementDirection:
		case FUNC_ID::kIsInScene:
		case FUNC_ID::kGetIsFlying:
		case FUNC_ID::kGetHighestRelationshipRank:
		case FUNC_ID::kGetLowestRelationshipRank:
		case FUNC_ID::kHasFamilyRelationshipAny:
		case FUNC_ID::kGetPathingTargetSpeed:
		case FUNC_ID::kGetMovementSpeed:
		case FUNC_ID::kIsDualCasting:
		case FUNC_ID::kIsEnteringInteractionQuick:
		case FUNC_ID::kIsCasting:
		case FUNC_ID::kGetFlyingState:
		case FUNC_ID::kIsInFavorState:
		case FUNC_ID::kHasTwoHandedWeaponEquipped:
		case FUNC_ID::kIsExitingInstant:
		case FUNC_ID::kIsInFriendStateWithPlayer:
		case FUNC_ID::kIsUnique:
		case FUNC_ID::kGetLastBumpDirection:
		case FUNC_ID::kGetIsInjured:
		case FUNC_ID::kGetIsCrashLandRequest:
		case FUNC_ID::kGetIsHastyLandRequest:
		case FUNC_ID::kGetBribeSuccess:
		case FUNC_ID::kGetIntimidateSuccess:
		case FUNC_ID::kGetArrestedState:
		case FUNC_ID::kGetArrestingActor:
		case FUNC_ID::kEPTemperingItemIsEnchanted:
		case FUNC_ID::kIsAttacking:
		case FUNC_ID::kIsPowerAttacking:
		case FUNC_ID::kIsLastHostileActor:
		case FUNC_ID::kGetCurrentShoutVariation:
		case FUNC_ID::kGetActivationHeight:
		case FUNC_ID::kGetPathingCurrentSpeed:
		case FUNC_ID::kGetNoBleedoutRecovery:
		case FUNC_ID::kIsAllowedToFly:
		case FUNC_ID::kIsCommandedActor:
		case FUNC_ID::kIsStaggered:
		case FUNC_ID::kIsRecoiling:
		case FUNC_ID::kIsExitingInteractionQuick:
		case FUNC_ID::kIsPathing:
		case FUNC_ID::kGetCombatGroupMemberCount:
		case FUNC_ID::kIsIgnoringCombat:
		case FUNC_ID::kGetLightLevel:
		case FUNC_ID::kIsBeingRidden:
		case FUNC_ID::kIsUndead:
		case FUNC_ID::kGetRealHoursPassed:
		case FUNC_ID::kIsUnlockedDoor:
		case FUNC_ID::kIsPoison:
		case FUNC_ID::kGetItemHealthPercent:
		case FUNC_ID::kEffectWasDualCast:
		case FUNC_ID::kGetKnockStateEnum:
		case FUNC_ID::kDoesNotExist:
		case FUNC_ID::kIsOnFlyingMount:
		case FUNC_ID::kCanFlyHere:
		case FUNC_ID::kIsFlyingMountPatrolQueued:
		case FUNC_ID::kIsFlyingMountFastTravelling:
		case FUNC_ID::kIsOverEncumbered:
		case FUNC_ID::kGetActorWarmth:
			paramPair = { std::nullopt, std::nullopt };
			break;
		case FUNC_ID::kGetDistance:
		case FUNC_ID::kGetLineOfSight:
		case FUNC_ID::kGetInSameCell:
		case FUNC_ID::kGetHeadingAngle:
		case FUNC_ID::kGetIsReference:
		case FUNC_ID::kIsCurrentFurnitureRef:
		case FUNC_ID::kGetRelationshipRank:
		case FUNC_ID::kExists:
		case FUNC_ID::kIsPlayerGrabbedRef:
		case FUNC_ID::kGetVATSRightAreaFree:
		case FUNC_ID::kGetVATSLeftAreaFree:
		case FUNC_ID::kGetVATSBackAreaFree:
		case FUNC_ID::kGetVATSFrontAreaFree:
		case FUNC_ID::kGetVATSRightTargetVisible:
		case FUNC_ID::kGetVATSLeftTargetVisible:
		case FUNC_ID::kGetVATSBackTargetVisible:
		case FUNC_ID::kGetVATSFrontTargetVisible:
		case FUNC_ID::kGetInContainer:
		case FUNC_ID::kGetInSharedCrimeFaction:
		case FUNC_ID::kGetTargetHeight:
			paramPair = { PARAM_TYPE::kObjectRef, std::nullopt };
			break;
		case FUNC_ID::kGetPos:
		case FUNC_ID::kGetAngle:
		case FUNC_ID::kGetStartingPos:
		case FUNC_ID::kGetStartingAngle:
		case FUNC_ID::kGetVelocity:
		case FUNC_ID::kGetPathingTargetOffset:
		case FUNC_ID::kGetPathingTargetAngleOffset:
		case FUNC_ID::kGetPathingTargetSpeedAngle:
		case FUNC_ID::kGetPathingCurrentSpeedAngle:
			paramPair = { PARAM_TYPE::kAxis, std::nullopt };
			break;
		case FUNC_ID::kGetActorValue:
		case FUNC_ID::kIsWeaponSkillType:
		case FUNC_ID::kGetBaseActorValue:
		case FUNC_ID::kGetPermanentActorValue:
		case FUNC_ID::kGetActorValuePercent:
		case FUNC_ID::kEPModSkillUsage_IsAdvanceSkill:
		case FUNC_ID::kEPMagic_SpellHasSkill:
			paramPair = { PARAM_TYPE::kActorValue, std::nullopt };
			break;
		case FUNC_ID::kMenuMode:
		case FUNC_ID::kIsInCombat:
		case FUNC_ID::kIsPlayerActionActive:
		case FUNC_ID::kGetHasNote:
		case FUNC_ID::kIsLimbGone:
		case FUNC_ID::kGetIsCreatureType:
		case FUNC_ID::kGetNumericPackageData:
			paramPair = { PARAM_TYPE::kInt, std::nullopt };
			break;
		case FUNC_ID::kSameFaction:
		case FUNC_ID::kSameRace:
		case FUNC_ID::kSameSex:
		case FUNC_ID::kGetDetected:
		case FUNC_ID::kGetShouldAttack:
		case FUNC_ID::kGetTalkedToPCParam:
		case FUNC_ID::kHasFamilyRelationship:
		case FUNC_ID::kHasParentRelationship:
		case FUNC_ID::kIsTalkingActivatorActor:
		case FUNC_ID::kIsKiller:
		case FUNC_ID::kGetFactionRelation:
		case FUNC_ID::kGetThreatRatio:
		case FUNC_ID::kIsCombatTarget:
		case FUNC_ID::kShouldAttackKill:
		case FUNC_ID::kGetShouldHelp:
		case FUNC_ID::kIsHostileToActor:
			paramPair = { PARAM_TYPE::kActor, std::nullopt };
			break;
		case FUNC_ID::kGetItemCount:
		case FUNC_ID::kGetEquipped:
			paramPair = { PARAM_TYPE::kInvObjectOrFormList, std::nullopt };
			break;
		case FUNC_ID::kGetScriptVariable:
		case FUNC_ID::kGetVMScriptVariable:
			paramPair = { PARAM_TYPE::kObjectRef, PARAM_TYPE::kChar };
			break;
		case FUNC_ID::kGetQuestRunning:
		case FUNC_ID::kGetStage:
		case FUNC_ID::kGetQuestCompleted:
			paramPair = { PARAM_TYPE::kQuest, std::nullopt };
			break;
		case FUNC_ID::kGetStageDone:
			paramPair = { PARAM_TYPE::kQuest, PARAM_TYPE::kInt };
			break;
		case FUNC_ID::kGetFactionRankDifference:
			paramPair = { PARAM_TYPE::kFaction, PARAM_TYPE::kActor };
			break;
		case FUNC_ID::kGetInCell:
			paramPair = { PARAM_TYPE::kCell, std::nullopt };
			break;
		case FUNC_ID::kGetIsClass:
		case FUNC_ID::kGetPCIsClass:
		case FUNC_ID::kGetIsClassDefault:
			paramPair = { PARAM_TYPE::kClass, std::nullopt };
			break;
		case FUNC_ID::kGetIsRace:
		case FUNC_ID::kGetPCIsRace:
			paramPair = { PARAM_TYPE::kRace, std::nullopt };
			break;
		case FUNC_ID::kGetIsSex:
		case FUNC_ID::kGetPCIsSex:
			paramPair = { PARAM_TYPE::kSex, std::nullopt };
			break;
		case FUNC_ID::kGetInFaction:
		case FUNC_ID::kGetFactionRank:
		case FUNC_ID::kGetPCInFaction:
		case FUNC_ID::kGetIsCrimeFaction:
		case FUNC_ID::kGetPCExpelled:
		case FUNC_ID::kGetPCFactionMurder:
		case FUNC_ID::kGetPCEnemyofFaction:
		case FUNC_ID::kGetPCFactionAttack:
		case FUNC_ID::kGetStolenItemValueNoCrime:
		case FUNC_ID::kGetStolenItemValue:
			paramPair = { PARAM_TYPE::kFaction, std::nullopt };
			break;
		case FUNC_ID::kGetIsID:
		case FUNC_ID::kGetIsUsedItem:
			paramPair = { PARAM_TYPE::kObjectOrFormList, std::nullopt };
			break;
		case FUNC_ID::kGetGlobalValue:
			paramPair = { PARAM_TYPE::kGlobal, std::nullopt };
			break;
		case FUNC_ID::kGetQuestVariable:
		case FUNC_ID::kGetVMQuestVariable:
			paramPair = { PARAM_TYPE::kQuest, PARAM_TYPE::kChar };
			break;
		case FUNC_ID::kGetDeadCount:
			paramPair = { PARAM_TYPE::kActorBase, std::nullopt };
			break;
		case FUNC_ID::kGetPlayerControlsDisabled:
			paramPair = { PARAM_TYPE::kInt, PARAM_TYPE::kInt };
			break;
		case FUNC_ID::kIsPlayerInRegion:
			paramPair = { PARAM_TYPE::kRegion, std::nullopt };
			break;
		case FUNC_ID::kGetCrime:
			paramPair = { PARAM_TYPE::kActor, PARAM_TYPE::kCrimeType };
			break;
		case FUNC_ID::kGetIsCurrentWeather:
			paramPair = { PARAM_TYPE::kWeather, std::nullopt };
			break;
		case FUNC_ID::kGetIsCurrentPackage:
			paramPair = { PARAM_TYPE::kPackage, std::nullopt };
			break;
		case FUNC_ID::kIsCurrentFurnitureObj:
			paramPair = { PARAM_TYPE::kFurnitureOrFormList, std::nullopt };
			break;
		case FUNC_ID::kHasSameEditorLocAsRef:
		case FUNC_ID::kIsInSameCurrentLocAsRef:
		case FUNC_ID::kIsLinkedTo:
			paramPair = { PARAM_TYPE::kObjectRef, PARAM_TYPE::kKeyword };
			break;
		case FUNC_ID::kHasSameEditorLocAsRefAlias:
		case FUNC_ID::kIsInSameCurrentLocAsRefAlias:
		case FUNC_ID::kGetKeywordDataForAlias:
		case FUNC_ID::kLocAliasHasKeyword:
			paramPair = { PARAM_TYPE::kAlias, PARAM_TYPE::kKeyword };
			break;
		case FUNC_ID::kHasMagicEffect:
			paramPair = { PARAM_TYPE::kMagicEffect, std::nullopt };
			break;
		case FUNC_ID::kIsSpellTarget:
		case FUNC_ID::kHasSpell:
		case FUNC_ID::kGetSpellUsageNum:
			paramPair = { PARAM_TYPE::kMagicItem, std::nullopt };
			break;
		case FUNC_ID::kGetInCellParam:
			paramPair = { PARAM_TYPE::kCell, PARAM_TYPE::kObjectRef };
			break;
		case FUNC_ID::kGetIsUsedItemType:
		case FUNC_ID::kGetIsObjectType:
			paramPair = { PARAM_TYPE::kFormType, std::nullopt };
			break;
		case FUNC_ID::kIsScenePlaying:
			paramPair = { PARAM_TYPE::kBGSScene, std::nullopt };
			break;
		case FUNC_ID::kGetLocationCleared:
		case FUNC_ID::kGetInCurrentLoc:
		case FUNC_ID::kGetIsEditorLocation:
		case FUNC_ID::kIsLocationLoaded:
			paramPair = { PARAM_TYPE::kLocation, std::nullopt };
			break;
		case FUNC_ID::kHasAssociationType:
			paramPair = { PARAM_TYPE::kActor, PARAM_TYPE::kAssociationType };
			break;
		case FUNC_ID::kIsWarningAbout:
		case FUNC_ID::kIsInList:
		case FUNC_ID::kIsWeaponInList:
		case FUNC_ID::kIsKillerObject:
		case FUNC_ID::kGetInCurrentLocFormList:
			paramPair = { PARAM_TYPE::kFormList, std::nullopt };
			break;
		case FUNC_ID::kIsOwner:
			paramPair = { PARAM_TYPE::kOwner, std::nullopt };
			break;
		case FUNC_ID::kIsCellOwner:
			paramPair = { PARAM_TYPE::kCell, PARAM_TYPE::kOwner };
			break;
		case FUNC_ID::kGetInWorldspace:
			paramPair = { PARAM_TYPE::kWorldOrList, std::nullopt };
			break;
		case FUNC_ID::kGetPCMiscStat:
			paramPair = { PARAM_TYPE::kMiscStat, std::nullopt };
			break;
		case FUNC_ID::kGetWithinPackageLocation:
		case FUNC_ID::kIsNullPackageData:
			paramPair = { PARAM_TYPE::kPackageDataCanBeNull, std::nullopt };
			break;
		case FUNC_ID::kGetInCurrentLocAlias:
		case FUNC_ID::kGetIsAliasRef:
		case FUNC_ID::kGetIsEditorLocAlias:
		case FUNC_ID::kGetLocationAliasCleared:
		case FUNC_ID::kIsLocAliasLoaded:
			paramPair = { PARAM_TYPE::kAlias, std::nullopt };
			break;
		case FUNC_ID::kHasLinkedRef:
		case FUNC_ID::kGetKeywordItemCount:
		case FUNC_ID::kEPAlchemyEffectHasKeyword:
		case FUNC_ID::kHasKeyword:
		case FUNC_ID::kLocationHasKeyword:
		case FUNC_ID::kGetKeywordDataForCurrentLocation:
		case FUNC_ID::kEPTemperingItemHasKeyword:
		case FUNC_ID::kWornHasKeyword:
		case FUNC_ID::kEPModSkillUsage_AdvanceObjectHasKeyword:
		case FUNC_ID::kEPMagic_SpellHasKeyword:
		case FUNC_ID::kIsAttackType:
		case FUNC_ID::kHasMagicEffectKeyword:
		case FUNC_ID::kGetCombatTargetHasKeyword:
		case FUNC_ID::kWornApparelHasKeywordCount:
			paramPair = { PARAM_TYPE::kKeyword, std::nullopt };
			break;
		case FUNC_ID::kHasShout:
		case FUNC_ID::kGetEquippedShout:
			paramPair = { PARAM_TYPE::kShout, std::nullopt };
			break;
		case FUNC_ID::kGetVATSValue:
			paramPair = { PARAM_TYPE::kInt, PARAM_TYPE::kInt };
			break;
		case FUNC_ID::kGetFactionCombatReaction:
			paramPair = { PARAM_TYPE::kFaction, PARAM_TYPE::kFaction };
			break;
		case FUNC_ID::kGetIsVoiceType:
			paramPair = { PARAM_TYPE::kVoiceType, std::nullopt };
			break;
		case FUNC_ID::kGetInZone:
			paramPair = { PARAM_TYPE::kEncounterZone, std::nullopt };
			break;
		case FUNC_ID::kGetGraphVariableFloat:
		case FUNC_ID::kGetGraphVariableInt:
			paramPair = { PARAM_TYPE::kChar, std::nullopt };
			break;
		case FUNC_ID::kHasPerk:
			paramPair = { PARAM_TYPE::kPerk, PARAM_TYPE::kInt };
			break;
		case FUNC_ID::kIsLastIdlePlayed:
			paramPair = { PARAM_TYPE::kIdleForm, std::nullopt };
			break;
		case FUNC_ID::kGetIsAlignment:
			paramPair = { PARAM_TYPE::kAlignment, std::nullopt };
			break;
		case FUNC_ID::kGetIsUsedItemEquipType:
			paramPair = { PARAM_TYPE::kEquipType, std::nullopt };
			break;
		case FUNC_ID::kPlayerKnows:
			paramPair = { PARAM_TYPE::kKnowableForm, std::nullopt };
			break;
		case FUNC_ID::kIsInCriticalStage:
			paramPair = { PARAM_TYPE::kCritStage, std::nullopt };
			break;
		case FUNC_ID::kIsSceneActionComplete:
			paramPair = { PARAM_TYPE::kBGSScene, PARAM_TYPE::kInt };
			break;
		case FUNC_ID::kHasRefType:
		case FUNC_ID::kLocationHasRefType:
			paramPair = { PARAM_TYPE::kRefType, std::nullopt };
			break;
		case FUNC_ID::kHasEquippedSpell:
		case FUNC_ID::kGetCurrentCastingType:
		case FUNC_ID::kGetCurrentDeliveryType:
		case FUNC_ID::kGetEquippedItemType:
		case FUNC_ID::kGetReplacedItemType:
		case FUNC_ID::kHasBoundWeaponEquipped:
			paramPair = { PARAM_TYPE::kCastingSource, std::nullopt };
			break;
		case FUNC_ID::kGetEventData:
			paramPair = { PARAM_TYPE::kEventFunction, PARAM_TYPE::kEventFunctionData };  // third parameter in xEdit but who cares, we're skipping this
			break;
		case FUNC_ID::kIsCloserToAThanB:
			paramPair = { PARAM_TYPE::kObjectRef, PARAM_TYPE::kObjectRef };
			break;
		case FUNC_ID::kGetRelativeAngle:
			paramPair = { PARAM_TYPE::kObjectRef, PARAM_TYPE::kAxis };
			break;
		case FUNC_ID::kGetRefTypeDeadCount:
		case FUNC_ID::kGetRefTypeAliveCount:
			paramPair = { PARAM_TYPE::kLocation, PARAM_TYPE::kRefType };
			break;
		case FUNC_ID::kIsCurrentSpell:
			paramPair = { PARAM_TYPE::kMagicItem, PARAM_TYPE::kCastingSource };
			break;
		case FUNC_ID::kSpellHasKeyword:
			paramPair = { PARAM_TYPE::kCastingSource, PARAM_TYPE::kKeyword };
			break;
		case FUNC_ID::kGetLocAliasRefTypeDeadCount:
		case FUNC_ID::kGetLocAliasRefTypeAliveCount:
			paramPair = { PARAM_TYPE::kAlias, PARAM_TYPE::kRefType };
			break;
		case FUNC_ID::kIsWardState:
			paramPair = { PARAM_TYPE::kWardState, std::nullopt };
			break;
		case FUNC_ID::kLocAliasIsLocation:
			paramPair = { PARAM_TYPE::kAlias, PARAM_TYPE::kLocation };
			break;
		case FUNC_ID::kGetKeywordDataForLocation:
			paramPair = { PARAM_TYPE::kLocation, PARAM_TYPE::kKeyword };
			break;
		case FUNC_ID::kIsFurnitureAnimType:
		case FUNC_ID::kIsInFurnitureState:
			paramPair = { PARAM_TYPE::kFurnitureAnimType, std::nullopt };
			break;
		case FUNC_ID::kIsFurnitureEntryType:
			paramPair = { PARAM_TYPE::kFurnitureEntryType, std::nullopt };
			break;
		case FUNC_ID::kHasAssociationTypeAny:
			paramPair = { PARAM_TYPE::kAssociationType, std::nullopt };
			break;
		case FUNC_ID::kGetWithinDistance:
			paramPair = { PARAM_TYPE::kObjectRef, PARAM_TYPE::kFloat };
			break;
		case FUNC_ID::kEPModSkillUsage_IsAdvanceAction:
			paramPair = { PARAM_TYPE::kSkillAction, std::nullopt };
			break;
		case FUNC_ID::kSpellHasCastingPerk:
			paramPair = { PARAM_TYPE::kPerk, std::nullopt };
			break;
		default:
			paramPair = { std::nullopt, std::nullopt };
			break;
		}

		return paramPair;
	}

	bool ParseVoidParams(const std::string& a_str, void*& a_param, std::optional<PARAM_TYPE> a_type)
	{
		if (!a_type) {
			return true;
		}

		bool result = false;

		switch (*a_type) {
		case PARAM_TYPE::kObjectRef:
		case PARAM_TYPE::kActor:
			{
				if (string::icontains(a_str, "player")) {
					a_param = RE::PlayerCharacter::GetSingleton();
					result = true;
				} else {
					if (a_str.contains("~"sv)) {
						auto splitID = string::split(a_str, " ~ ");
						auto [formID, modName] = std::make_pair(string::to_num<RE::FormID>(splitID[0], true), splitID[1]);
						if (const auto form = RE::TESDataHandler::GetSingleton()->LookupForm(formID, modName)) {
							a_param = form;
							result = true;
						}
					} else if (string::is_only_hex(a_str)) {
						const auto formID = string::to_num<RE::FormID>(a_str, true);
						if (const auto form = RE::TESForm::LookupByID(formID)) {
							a_param = form;
							result = true;
						}
					} else {
						if (const auto form = RE::TESForm::LookupByEditorID(a_str)) {
							a_param = form;
							result = true;
						}
					}
				}
			}
			break;
		case PARAM_TYPE::kEquipType:
		case PARAM_TYPE::kInventoryObject:
		case PARAM_TYPE::kInvObjectOrFormList:
		case PARAM_TYPE::kVoiceType:
		case PARAM_TYPE::kIdleForm:
		case PARAM_TYPE::kFormList:
		case PARAM_TYPE::kQuest:
		case PARAM_TYPE::kFaction:
		case PARAM_TYPE::kCell:
		case PARAM_TYPE::kClass:
		case PARAM_TYPE::kRace:
		case PARAM_TYPE::kActorBase:
		case PARAM_TYPE::kGlobal:
		case PARAM_TYPE::kWeather:
		case PARAM_TYPE::kPackage:
		case PARAM_TYPE::kEncounterZone:
		case PARAM_TYPE::kPerk:
		case PARAM_TYPE::kOwner:
		case PARAM_TYPE::kFurnitureOrFormList:
		case PARAM_TYPE::kMagicItem:
		case PARAM_TYPE::kMagicEffect:
		case PARAM_TYPE::kWorldOrList:
		case PARAM_TYPE::kObject:
		case PARAM_TYPE::kRegion:
		case PARAM_TYPE::kKeyword:
		case PARAM_TYPE::kShout:
		case PARAM_TYPE::kLocation:
		case PARAM_TYPE::kRefType:
		case PARAM_TYPE::kAssociationType:
		case PARAM_TYPE::kBGSScene:
		case PARAM_TYPE::kKnowableForm:
			{
				if (a_str.contains("~"sv)) {
					auto splitID = string::split(a_str, " ~ ");
					auto [formID, modName] = std::make_pair(string::to_num<RE::FormID>(splitID[0], true), splitID[1]);
					if (const auto form = RE::TESDataHandler::GetSingleton()->LookupForm(formID, modName)) {
						a_param = form;
						result = true;
					}
				} else if (string::is_only_hex(a_str)) {
					const auto formID = string::to_num<RE::FormID>(a_str, true);
					if (const auto form = RE::TESForm::LookupByID(formID)) {
						a_param = form;
						result = true;
					}
				} else {
					if (const auto form = RE::TESForm::LookupByEditorID(a_str)) {
						a_param = form;
						result = true;
					}
				}
			}
			break;
		default:
			break;
		}

		return result;
	}

	std::vector<ConditionData> ParseConditionList(const std::vector<std::string>& a_conditionList)
	{
		using TYPE = ConditionData::TYPE;

		std::vector<ConditionData> dataVec{};

		for (auto& condition : a_conditionList) {
			ConditionData data{};
			PARAMS paramPair = { std::nullopt, std::nullopt };

			auto split_condition = string::split(condition, " | ");
			for (auto& conditionData : split_condition) {
				string::trim(conditionData);
			}

			try {
				//conditionItemObject
				if (auto condItem = util::get_value<OBJECT>(split_condition[TYPE::kConditionItemObject])) {
					data.conditionItem = *condItem;
				} else {
					continue;
				}

				switch (data.conditionItem) {
				case OBJECT::kRef:
				case OBJECT::kLinkedRef:
				case OBJECT::kQuestAlias:
				case OBJECT::kPackData:
				case OBJECT::kEventData:
					continue;
				default:
					break;
				}

				//functionID
				if (auto funcID = util::get_value<FUNC_ID>(split_condition[TYPE::kFunctionID])) {
					data.functionID = *funcID;
				} else {
					continue;
				}
				paramPair = GetFuncType(data.functionID);

				//param1
				auto& param1Str = split_condition[TYPE::kParam1];
				if (dist::is_valid_entry(param1Str)) {
					const auto result = ParseVoidParams(param1Str, data.param1, paramPair.first);
					if (!result) {
						continue;
					}
				}

				//param2
				auto& param2Str = split_condition[TYPE::kParam2];
				if (dist::is_valid_entry(param2Str)) {
					const auto result = ParseVoidParams(param2Str, data.param2, paramPair.second);
					if (!result) {
						continue;
					}
				}

				//OPCode
				if (auto opCode = util::get_value<OP_CODE>(split_condition[TYPE::kOPCode])) {
					data.opCode = *opCode;
				} else {
					continue;
				}

				//float
				data.value = string::to_num<float>(split_condition[TYPE::kFloat]);

				//operator
				data.andOr = string::iequals(split_condition[TYPE::kANDOR], "OR"sv);

			} catch (...) {
				continue;
			}

			dataVec.push_back(data);
		}

		return dataVec;
	}

	bool BuildVoidParams(std::string& a_str, void* a_param, std::optional<PARAM_TYPE> a_type)
	{
		if (!a_type || !a_param) {
			a_str += "NONE"sv;
			return true;
		}

		bool result = false;

		switch (*a_type) {
		case PARAM_TYPE::kInt:
			{
				const auto integer = static_cast<std::int32_t>(reinterpret_cast<intptr_t>(a_param));
				a_str += std::to_string(integer);

				result = true;
			}
			break;
		case PARAM_TYPE::kFloat:
			{
				const auto num = *reinterpret_cast<float*>(&a_param);
				a_str += std::to_string(num);

				result = true;
			}
			break;
		case PARAM_TYPE::kChar:
			{
				a_str += *static_cast<const char*>(a_param);
				result = true;
			}
			break;
		case PARAM_TYPE::kSex:
			{
				const auto sex = static_cast<std::uint32_t>(reinterpret_cast<uintptr_t>(a_param));
				a_str += sex == 0 ? "Male" : "Female";

				result = true;
			}
			break;
		case PARAM_TYPE::kActorValue:
			{
				auto av = static_cast<std::uint32_t>(reinterpret_cast<uintptr_t>(a_param));

				const auto avList = RE::ActorValueList::GetSingleton();
				if (const auto info = avList ? avList->GetActorValue(static_cast<RE::ActorValue>(av)) : nullptr) {
					a_str += info->enumName;
				}

				result = true;
			}
			break;
		case PARAM_TYPE::kObjectRef:
		case PARAM_TYPE::kActor:
			{
				if (const auto player = static_cast<RE::PlayerCharacter*>(a_param); player) {
					a_str += "PlayerRef"sv;
				} else if (const auto form = static_cast<RE::TESForm*>(a_param); form) {
					a_str += fmt::format("0x{:X}", form->GetLocalFormID());
					a_str += " ~ "sv;
					a_str += form->GetFile(0)->fileName;

					result = true;
				} else {
					a_str += "NONE"sv;
				}
			}
			break;
		case PARAM_TYPE::kEquipType:
		case PARAM_TYPE::kInventoryObject:
		case PARAM_TYPE::kInvObjectOrFormList:
		case PARAM_TYPE::kVoiceType:
		case PARAM_TYPE::kIdleForm:
		case PARAM_TYPE::kFormList:
		case PARAM_TYPE::kQuest:
		case PARAM_TYPE::kFaction:
		case PARAM_TYPE::kCell:
		case PARAM_TYPE::kClass:
		case PARAM_TYPE::kRace:
		case PARAM_TYPE::kActorBase:
		case PARAM_TYPE::kGlobal:
		case PARAM_TYPE::kWeather:
		case PARAM_TYPE::kPackage:
		case PARAM_TYPE::kEncounterZone:
		case PARAM_TYPE::kPerk:
		case PARAM_TYPE::kOwner:
		case PARAM_TYPE::kFurnitureOrFormList:
		case PARAM_TYPE::kMagicItem:
		case PARAM_TYPE::kMagicEffect:
		case PARAM_TYPE::kWorldOrList:
		case PARAM_TYPE::kObject:
		case PARAM_TYPE::kRegion:
		case PARAM_TYPE::kKeyword:
		case PARAM_TYPE::kShout:
		case PARAM_TYPE::kLocation:
		case PARAM_TYPE::kRefType:
		case PARAM_TYPE::kAssociationType:
		case PARAM_TYPE::kBGSScene:
		case PARAM_TYPE::kKnowableForm:
			{
				if (const auto form = static_cast<RE::TESForm*>(a_param)) {
					a_str += fmt::format("0x{:X}", form->GetLocalFormID());
					a_str += " ~ "sv;
					a_str += form->GetFile(0)->fileName;

					result = true;
				} else {
					a_str += "NONE"sv;
				}
			}
			break;
		default:
			break;
		}

		return result;
	}

	std::vector<std::string> BuildConditionList(const RE::TESCondition* a_conditions)
	{
		using OPCODE = RE::CONDITION_ITEM_DATA::OpCode;

		std::vector<ConditionData> conditionVec{};

		if (a_conditions) {
			auto tmp = a_conditions->head;
			while (tmp != nullptr) {
				conditionVec.emplace_back(tmp);
				tmp = tmp->next;
			}
		}

		if (conditionVec.empty()) {
			return {};
		}

		std::vector<std::string> vec;
		vec.reserve(conditionVec.size());

		for (auto& [conditionItem, functionID, param1, param2, operationCode, floatVal, operatorVal] : conditionVec) {
			std::string condition;
			//condition
			auto condItem = stl::to_underlying(conditionItem);
			if (auto condItemStr = util::get_value<std::string>(conditionObjs, condItem)) {
				condition += *condItemStr;
			} else {
				condition += std::to_string(condItem);
			}
			condition.append(" | "sv);
			//functionID
			auto funcID = stl::to_underlying(functionID);
			if (auto funcIDStr = util::get_value<std::string>(funcIDs, funcID)) {
				condition += *funcIDStr;
			} else {
				condition += std::to_string(funcID);
			}
			const auto paramPair = GetFuncType(functionID);
			//param1
			condition.append(" | "sv);
			try {
				if (!BuildVoidParams(condition, param1, paramPair.first)) {
					continue;
				}
			} catch (...) {
				condition.append("NONE"sv);
			}
			condition.append(" | "sv);
			//param2
			try {
				if (!BuildVoidParams(condition, param2, paramPair.second)) {
					continue;
				}
			} catch (...) {
				condition.append("NONE"sv);
			}
			condition.append(" | "sv);
			//opCode
			auto opCode = stl::to_underlying(operationCode);
			if (auto opCodeStr = util::get_value<std::string>(opCodes, opCode)) {
				condition += *opCodeStr;
			} else {
				condition += std::to_string(opCode);
			}
			condition.append(" | "sv);
			//floatVal
			condition += std::to_string(std::roundf(floatVal));
			condition.append(" | "sv);
			//ANDOR
			condition.append(operatorVal ? "OR"sv : "AND"sv);

			vec.emplace_back(condition.c_str());
		}

		return vec;
	}

	RE::TESCondition* GetConditions(RE::TESForm& a_form, std::uint32_t a_index)
	{
		RE::TESCondition* condition = nullptr;

		switch (a_form.GetFormType()) {
		case RE::FormType::MagicEffect:
			{
				const auto effect = a_form.As<RE::EffectSetting>();
				condition = &effect->conditions;
			}
			break;
		case RE::FormType::Spell:
		case RE::FormType::Enchantment:
		case RE::FormType::Ingredient:
		case RE::FormType::AlchemyItem:
		case RE::FormType::Scroll:
			{
				const auto magicItem = a_form.As<RE::MagicItem>();
				condition = a_index < magicItem->effects.size() ? &magicItem->effects[a_index]->conditions : nullptr;
			}
			break;
		case RE::FormType::Info:
			{
				const auto topic = a_form.As<RE::TESTopicInfo>();
				condition = &topic->objConditions;
			}
			break;
		case RE::FormType::Package:
			{
				const auto package = a_form.As<RE::TESPackage>();
				condition = &package->packConditions;
			}
			break;
		case RE::FormType::Perk:
			{
				const auto perk = a_form.As<RE::BGSPerk>();
				condition = &perk->perkConditions;
			}
			break;
		default:
			break;
		}

		return condition;
	}
}
