#include "Papyrus/Util/ConditionParser.h"

namespace RE
{
	bool operator==(const CONDITION_ITEM_DATA& a_this, const CONDITION_ITEM_DATA& a_data)
	{
		return a_this.functionData.function == a_data.functionData.function &&
		       a_this.object == a_data.object &&
		       a_this.functionData.params[0] == a_data.functionData.params[0] &&
		       a_this.functionData.params[1] == a_data.functionData.params[1] &&
		       a_this.flags.opCode == a_data.flags.opCode &&
		       a_this.comparisonValue.f == a_data.comparisonValue.f &&
		       a_this.flags.isOR == a_data.flags.isOR;
	}
}

namespace CONDITION
{
	PARAM_TYPES GetFuncType(FUNC_ID a_funcID)
	{
		PARAM_TYPES paramTypes;

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
			paramTypes = { std::nullopt, std::nullopt };
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
			paramTypes = { PARAM_TYPE::kObjectRef, std::nullopt };
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
			paramTypes = { PARAM_TYPE::kAxis, std::nullopt };
			break;
		case FUNC_ID::kGetActorValue:
		case FUNC_ID::kIsWeaponSkillType:
		case FUNC_ID::kGetBaseActorValue:
		case FUNC_ID::kGetPermanentActorValue:
		case FUNC_ID::kGetActorValuePercent:
		case FUNC_ID::kEPModSkillUsage_IsAdvanceSkill:
		case FUNC_ID::kEPMagic_SpellHasSkill:
			paramTypes = { PARAM_TYPE::kActorValue, std::nullopt };
			break;
		case FUNC_ID::kMenuMode:
		case FUNC_ID::kIsInCombat:
		case FUNC_ID::kIsPlayerActionActive:
		case FUNC_ID::kGetHasNote:
		case FUNC_ID::kIsLimbGone:
		case FUNC_ID::kGetIsCreatureType:
		case FUNC_ID::kGetNumericPackageData:
			paramTypes = { PARAM_TYPE::kInt, std::nullopt };
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
			paramTypes = { PARAM_TYPE::kActor, std::nullopt };
			break;
		case FUNC_ID::kGetItemCount:
		case FUNC_ID::kGetEquipped:
			paramTypes = { PARAM_TYPE::kInvObjectOrFormList, std::nullopt };
			break;
		case FUNC_ID::kGetScriptVariable:
		case FUNC_ID::kGetVMScriptVariable:
			paramTypes = { PARAM_TYPE::kObjectRef, PARAM_TYPE::kChar };
			break;
		case FUNC_ID::kGetQuestRunning:
		case FUNC_ID::kGetStage:
		case FUNC_ID::kGetQuestCompleted:
			paramTypes = { PARAM_TYPE::kQuest, std::nullopt };
			break;
		case FUNC_ID::kGetStageDone:
			paramTypes = { PARAM_TYPE::kQuest, PARAM_TYPE::kInt };
			break;
		case FUNC_ID::kGetFactionRankDifference:
			paramTypes = { PARAM_TYPE::kFaction, PARAM_TYPE::kActor };
			break;
		case FUNC_ID::kGetInCell:
			paramTypes = { PARAM_TYPE::kCell, std::nullopt };
			break;
		case FUNC_ID::kGetIsClass:
		case FUNC_ID::kGetPCIsClass:
		case FUNC_ID::kGetIsClassDefault:
			paramTypes = { PARAM_TYPE::kClass, std::nullopt };
			break;
		case FUNC_ID::kGetIsRace:
		case FUNC_ID::kGetPCIsRace:
			paramTypes = { PARAM_TYPE::kRace, std::nullopt };
			break;
		case FUNC_ID::kGetIsSex:
		case FUNC_ID::kGetPCIsSex:
			paramTypes = { PARAM_TYPE::kSex, std::nullopt };
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
			paramTypes = { PARAM_TYPE::kFaction, std::nullopt };
			break;
		case FUNC_ID::kGetIsID:
		case FUNC_ID::kGetIsUsedItem:
			paramTypes = { PARAM_TYPE::kObjectOrFormList, std::nullopt };
			break;
		case FUNC_ID::kGetGlobalValue:
			paramTypes = { PARAM_TYPE::kGlobal, std::nullopt };
			break;
		case FUNC_ID::kGetQuestVariable:
		case FUNC_ID::kGetVMQuestVariable:
			paramTypes = { PARAM_TYPE::kQuest, PARAM_TYPE::kChar };
			break;
		case FUNC_ID::kGetDeadCount:
			paramTypes = { PARAM_TYPE::kActorBase, std::nullopt };
			break;
		case FUNC_ID::kGetPlayerControlsDisabled:
			paramTypes = { PARAM_TYPE::kInt, PARAM_TYPE::kInt };
			break;
		case FUNC_ID::kIsPlayerInRegion:
			paramTypes = { PARAM_TYPE::kRegion, std::nullopt };
			break;
		case FUNC_ID::kGetCrime:
			paramTypes = { PARAM_TYPE::kActor, PARAM_TYPE::kCrimeType };
			break;
		case FUNC_ID::kGetIsCurrentWeather:
			paramTypes = { PARAM_TYPE::kWeather, std::nullopt };
			break;
		case FUNC_ID::kGetIsCurrentPackage:
			paramTypes = { PARAM_TYPE::kPackage, std::nullopt };
			break;
		case FUNC_ID::kIsCurrentFurnitureObj:
			paramTypes = { PARAM_TYPE::kFurnitureOrFormList, std::nullopt };
			break;
		case FUNC_ID::kHasSameEditorLocAsRef:
		case FUNC_ID::kIsInSameCurrentLocAsRef:
		case FUNC_ID::kIsLinkedTo:
			paramTypes = { PARAM_TYPE::kObjectRef, PARAM_TYPE::kKeyword };
			break;
		case FUNC_ID::kHasSameEditorLocAsRefAlias:
		case FUNC_ID::kIsInSameCurrentLocAsRefAlias:
		case FUNC_ID::kGetKeywordDataForAlias:
		case FUNC_ID::kLocAliasHasKeyword:
			paramTypes = { PARAM_TYPE::kAlias, PARAM_TYPE::kKeyword };
			break;
		case FUNC_ID::kHasMagicEffect:
			paramTypes = { PARAM_TYPE::kMagicEffect, std::nullopt };
			break;
		case FUNC_ID::kIsSpellTarget:
		case FUNC_ID::kHasSpell:
		case FUNC_ID::kGetSpellUsageNum:
			paramTypes = { PARAM_TYPE::kMagicItem, std::nullopt };
			break;
		case FUNC_ID::kGetInCellParam:
			paramTypes = { PARAM_TYPE::kCell, PARAM_TYPE::kObjectRef };
			break;
		case FUNC_ID::kGetIsUsedItemType:
		case FUNC_ID::kGetIsObjectType:
			paramTypes = { PARAM_TYPE::kFormType, std::nullopt };
			break;
		case FUNC_ID::kIsScenePlaying:
			paramTypes = { PARAM_TYPE::kBGSScene, std::nullopt };
			break;
		case FUNC_ID::kGetLocationCleared:
		case FUNC_ID::kGetInCurrentLoc:
		case FUNC_ID::kGetIsEditorLocation:
		case FUNC_ID::kIsLocationLoaded:
			paramTypes = { PARAM_TYPE::kLocation, std::nullopt };
			break;
		case FUNC_ID::kHasAssociationType:
			paramTypes = { PARAM_TYPE::kActor, PARAM_TYPE::kAssociationType };
			break;
		case FUNC_ID::kIsWarningAbout:
		case FUNC_ID::kIsInList:
		case FUNC_ID::kIsWeaponInList:
		case FUNC_ID::kIsKillerObject:
		case FUNC_ID::kGetInCurrentLocFormList:
			paramTypes = { PARAM_TYPE::kFormList, std::nullopt };
			break;
		case FUNC_ID::kIsOwner:
			paramTypes = { PARAM_TYPE::kOwner, std::nullopt };
			break;
		case FUNC_ID::kIsCellOwner:
			paramTypes = { PARAM_TYPE::kCell, PARAM_TYPE::kOwner };
			break;
		case FUNC_ID::kGetInWorldspace:
			paramTypes = { PARAM_TYPE::kWorldOrList, std::nullopt };
			break;
		case FUNC_ID::kGetPCMiscStat:
			paramTypes = { PARAM_TYPE::kMiscStat, std::nullopt };
			break;
		case FUNC_ID::kGetWithinPackageLocation:
		case FUNC_ID::kIsNullPackageData:
			paramTypes = { PARAM_TYPE::kPackageDataCanBeNull, std::nullopt };
			break;
		case FUNC_ID::kGetInCurrentLocAlias:
		case FUNC_ID::kGetIsAliasRef:
		case FUNC_ID::kGetIsEditorLocAlias:
		case FUNC_ID::kGetLocationAliasCleared:
		case FUNC_ID::kIsLocAliasLoaded:
			paramTypes = { PARAM_TYPE::kAlias, std::nullopt };
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
			paramTypes = { PARAM_TYPE::kKeyword, std::nullopt };
			break;
		case FUNC_ID::kHasShout:
		case FUNC_ID::kGetEquippedShout:
			paramTypes = { PARAM_TYPE::kShout, std::nullopt };
			break;
		case FUNC_ID::kGetVATSValue:
			paramTypes = { PARAM_TYPE::kInt, PARAM_TYPE::kInt };
			break;
		case FUNC_ID::kGetFactionCombatReaction:
			paramTypes = { PARAM_TYPE::kFaction, PARAM_TYPE::kFaction };
			break;
		case FUNC_ID::kGetIsVoiceType:
			paramTypes = { PARAM_TYPE::kVoiceType, std::nullopt };
			break;
		case FUNC_ID::kGetInZone:
			paramTypes = { PARAM_TYPE::kEncounterZone, std::nullopt };
			break;
		case FUNC_ID::kGetGraphVariableFloat:
		case FUNC_ID::kGetGraphVariableInt:
			paramTypes = { PARAM_TYPE::kChar, std::nullopt };
			break;
		case FUNC_ID::kHasPerk:
			paramTypes = { PARAM_TYPE::kPerk, PARAM_TYPE::kInt };
			break;
		case FUNC_ID::kIsLastIdlePlayed:
			paramTypes = { PARAM_TYPE::kIdleForm, std::nullopt };
			break;
		case FUNC_ID::kGetIsAlignment:
			paramTypes = { PARAM_TYPE::kAlignment, std::nullopt };
			break;
		case FUNC_ID::kGetIsUsedItemEquipType:
			paramTypes = { PARAM_TYPE::kEquipType, std::nullopt };
			break;
		case FUNC_ID::kPlayerKnows:
			paramTypes = { PARAM_TYPE::kKnowableForm, std::nullopt };
			break;
		case FUNC_ID::kIsInCriticalStage:
			paramTypes = { PARAM_TYPE::kCritStage, std::nullopt };
			break;
		case FUNC_ID::kIsSceneActionComplete:
			paramTypes = { PARAM_TYPE::kBGSScene, PARAM_TYPE::kInt };
			break;
		case FUNC_ID::kHasRefType:
		case FUNC_ID::kLocationHasRefType:
			paramTypes = { PARAM_TYPE::kRefType, std::nullopt };
			break;
		case FUNC_ID::kHasEquippedSpell:
		case FUNC_ID::kGetCurrentCastingType:
		case FUNC_ID::kGetCurrentDeliveryType:
		case FUNC_ID::kGetEquippedItemType:
		case FUNC_ID::kGetReplacedItemType:
		case FUNC_ID::kHasBoundWeaponEquipped:
			paramTypes = { PARAM_TYPE::kCastingSource, std::nullopt };
			break;
		case FUNC_ID::kGetEventData:
			paramTypes = { PARAM_TYPE::kEventFunction, PARAM_TYPE::kEventFunctionData };  // third parameter in xEdit but who cares, we're skipping this
			break;
		case FUNC_ID::kIsCloserToAThanB:
			paramTypes = { PARAM_TYPE::kObjectRef, PARAM_TYPE::kObjectRef };
			break;
		case FUNC_ID::kGetRelativeAngle:
			paramTypes = { PARAM_TYPE::kObjectRef, PARAM_TYPE::kAxis };
			break;
		case FUNC_ID::kGetRefTypeDeadCount:
		case FUNC_ID::kGetRefTypeAliveCount:
			paramTypes = { PARAM_TYPE::kLocation, PARAM_TYPE::kRefType };
			break;
		case FUNC_ID::kIsCurrentSpell:
			paramTypes = { PARAM_TYPE::kMagicItem, PARAM_TYPE::kCastingSource };
			break;
		case FUNC_ID::kSpellHasKeyword:
			paramTypes = { PARAM_TYPE::kCastingSource, PARAM_TYPE::kKeyword };
			break;
		case FUNC_ID::kGetLocAliasRefTypeDeadCount:
		case FUNC_ID::kGetLocAliasRefTypeAliveCount:
			paramTypes = { PARAM_TYPE::kAlias, PARAM_TYPE::kRefType };
			break;
		case FUNC_ID::kIsWardState:
			paramTypes = { PARAM_TYPE::kWardState, std::nullopt };
			break;
		case FUNC_ID::kLocAliasIsLocation:
			paramTypes = { PARAM_TYPE::kAlias, PARAM_TYPE::kLocation };
			break;
		case FUNC_ID::kGetKeywordDataForLocation:
			paramTypes = { PARAM_TYPE::kLocation, PARAM_TYPE::kKeyword };
			break;
		case FUNC_ID::kIsFurnitureAnimType:
		case FUNC_ID::kIsInFurnitureState:
			paramTypes = { PARAM_TYPE::kFurnitureAnimType, std::nullopt };
			break;
		case FUNC_ID::kIsFurnitureEntryType:
			paramTypes = { PARAM_TYPE::kFurnitureEntryType, std::nullopt };
			break;
		case FUNC_ID::kHasAssociationTypeAny:
			paramTypes = { PARAM_TYPE::kAssociationType, std::nullopt };
			break;
		case FUNC_ID::kGetWithinDistance:
			paramTypes = { PARAM_TYPE::kObjectRef, PARAM_TYPE::kFloat };
			break;
		case FUNC_ID::kEPModSkillUsage_IsAdvanceAction:
			paramTypes = { PARAM_TYPE::kSkillAction, std::nullopt };
			break;
		case FUNC_ID::kSpellHasCastingPerk:
			paramTypes = { PARAM_TYPE::kPerk, std::nullopt };
			break;
		default:
			paramTypes = { std::nullopt, std::nullopt };
			break;
		}

		return paramTypes;
	}

	PARAM::STATE PARAM::GetParamState(std::optional<PARAM_TYPE> a_paramType)
	{
		if (!a_paramType) {
			return STATE::kNull;
		}

		switch (*a_paramType) {
		case PARAM_TYPE::kInt:
		case PARAM_TYPE::kStage:
		case PARAM_TYPE::kRelationshipRank:
		case PARAM_TYPE::kActorValue:
		case PARAM_TYPE::kSex:
		// case PARAM_TYPE::kFormType:  // needs work (RE::OBJECT_TYPE ?)
		case PARAM_TYPE::kCastingSource:
		case PARAM_TYPE::kWardState:
		case PARAM_TYPE::kCritStage:
		case PARAM_TYPE::kCrimeType:
		// case PARAM_TYPE::kMiscStat:  // too many stats ahhh
		case PARAM_TYPE::kSkillAction:
		// case PARAM_TYPE::kAnimGroup:	// too many anims
		case PARAM_TYPE::kAxis:
		case PARAM_TYPE::kAlignment:
		case PARAM_TYPE::kFurnitureAnimType:
		case PARAM_TYPE::kFurnitureEntryType:

		case PARAM_TYPE::kFloat:

		case PARAM_TYPE::kObjectRef:
		case PARAM_TYPE::kActor:
		case PARAM_TYPE::kMapMarker:
		case PARAM_TYPE::kContainerRef:
		case PARAM_TYPE::kEquipType:
		case PARAM_TYPE::kInventoryObject:
		case PARAM_TYPE::kSpellItem:
		case PARAM_TYPE::kMessage:
		case PARAM_TYPE::kInvObjectOrFormList:
		case PARAM_TYPE::kObjectOrFormList:
		case PARAM_TYPE::kMusic:
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
		case PARAM_TYPE::kShout:
		case PARAM_TYPE::kLocation:
		case PARAM_TYPE::kRefType:
		case PARAM_TYPE::kAssociationType:
		case PARAM_TYPE::kBGSScene:
		case PARAM_TYPE::kForm:
		case PARAM_TYPE::kKnowableForm:
		case PARAM_TYPE::kWordOfPower:
		case PARAM_TYPE::kTopic:
		case PARAM_TYPE::kNPC:
		case PARAM_TYPE::kSound:
		case PARAM_TYPE::kCombatStyle:
		case PARAM_TYPE::kShaderEffect:
		case PARAM_TYPE::kReferenceEffect:
		case PARAM_TYPE::kMenuIcon:
		case PARAM_TYPE::kNote:
		case PARAM_TYPE::kImagespaceMod:
		case PARAM_TYPE::kImagespace:
		case PARAM_TYPE::kKeyword:
			return STATE::kValid;
		default:
			return STATE::kInvalid;
		}
	}

	PARAM::PARAM(std::optional<PARAM_TYPE> a_paramType, void* a_param) :
		type(a_paramType),
		state(GetParamState(a_paramType)),
		data(nullptr)
	{
		if (!type || state != STATE::kValid || !a_param) {
			return;
		}

		switch (*type) {
		case PARAM_TYPE::kInt:
		case PARAM_TYPE::kStage:
		case PARAM_TYPE::kRelationshipRank:
		case PARAM_TYPE::kActorValue:
		case PARAM_TYPE::kSex:
		case PARAM_TYPE::kFormType:  // needs work (RE::OBJECT_TYPE ?)
		case PARAM_TYPE::kCastingSource:
		case PARAM_TYPE::kWardState:
		case PARAM_TYPE::kCritStage:
		case PARAM_TYPE::kCrimeType:
		case PARAM_TYPE::kMiscStat:  // too much
		case PARAM_TYPE::kSkillAction:
		case PARAM_TYPE::kAxis:
		case PARAM_TYPE::kAlignment:
		case PARAM_TYPE::kFurnitureAnimType:
		case PARAM_TYPE::kFurnitureEntryType:
			data.i = static_cast<std::int32_t>(reinterpret_cast<intptr_t>(a_param));
			break;
		case PARAM_TYPE::kFloat:
			data.f = *reinterpret_cast<float*>(&a_param);
			break;
		case PARAM_TYPE::kObjectRef:
		case PARAM_TYPE::kActor:
		case PARAM_TYPE::kMapMarker:
		case PARAM_TYPE::kContainerRef:
		case PARAM_TYPE::kEquipType:
		case PARAM_TYPE::kInventoryObject:
		case PARAM_TYPE::kSpellItem:
		case PARAM_TYPE::kMessage:
		case PARAM_TYPE::kInvObjectOrFormList:
		case PARAM_TYPE::kObjectOrFormList:
		case PARAM_TYPE::kMusic:
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
		case PARAM_TYPE::kShout:
		case PARAM_TYPE::kLocation:
		case PARAM_TYPE::kRefType:
		case PARAM_TYPE::kAssociationType:
		case PARAM_TYPE::kBGSScene:
		case PARAM_TYPE::kForm:
		case PARAM_TYPE::kKnowableForm:
		case PARAM_TYPE::kWordOfPower:
		case PARAM_TYPE::kTopic:
		case PARAM_TYPE::kNPC:
		case PARAM_TYPE::kSound:
		case PARAM_TYPE::kCombatStyle:
		case PARAM_TYPE::kShaderEffect:
		case PARAM_TYPE::kReferenceEffect:
		case PARAM_TYPE::kMenuIcon:
		case PARAM_TYPE::kNote:
		case PARAM_TYPE::kImagespaceMod:
		case PARAM_TYPE::kImagespace:
		case PARAM_TYPE::kKeyword:
			data.ptr = static_cast<RE::TESForm*>(a_param);
			break;
		default:
			break;
		}
	}

	PARAM::PARAM(std::optional<PARAM_TYPE> a_type, const std::string& a_param) :
		type(a_type),
		state(GetParamState(a_type)),
		data(nullptr)
	{
		const auto lookup_form = [&](const std::string& b_str) {
			if (b_str.contains("~"sv)) {
				auto splitID = string::split(b_str, " ~ ");
				auto [formID, modName] = std::make_pair(string::to_num<RE::FormID>(splitID[0], true), splitID[1]);
				return RE::TESDataHandler::GetSingleton()->LookupForm(formID, modName);
			} else if (string::is_only_hex(b_str)) {
				const auto formID = string::to_num<RE::FormID>(b_str, true);
				return RE::TESForm::LookupByID(formID);
			} else {
				return RE::TESForm::LookupByEditorID(b_str);
			}
		};

		if (!type || state != STATE::kValid) {
			return;
		}

		if (!a_param.empty() || string::icontains(a_param, "NONE"sv) && !stl::is_in(*type, PARAM_TYPE::kWardState, PARAM_TYPE::kCritStage)) {
			type = std::nullopt;
			state = STATE::kNull;
			return;
		}

		switch (*type) {
		case PARAM_TYPE::kInt:
		case PARAM_TYPE::kStage:
		case PARAM_TYPE::kRelationshipRank:
			data.i = string::to_num<std::int32_t>(a_param);
			break;
		case PARAM_TYPE::kFloat:
			data.f = string::to_num<float>(a_param);
			break;
		case PARAM_TYPE::kActorValue:
			data.i = static_cast<std::int32_t>(RE::ActorValueList::GetSingleton()->LookupActorValueByName(a_param));
			break;
		case RE::SCRIPT_PARAM_TYPE::kAxis:
			{
				switch (string::const_hash(a_param)) {
				case "X"_h:
					data.i = 88;
					break;
				case "Y"_h:
					data.i = 89;
					break;
				case "Z"_h:
					data.i = 90;
					break;
				default:
					data.i = -1;
					break;
				}
			}
			break;
		case RE::SCRIPT_PARAM_TYPE::kSex:
			{
				switch (string::const_hash(a_param)) {
				case "Male"_h:
					data.i = RE::SEX::kMale;
					break;
				case "Female"_h:
					data.i = RE::SEX::kFemale;
					break;
				default:
					data.i = -1;
					break;
				}
			}
			break;
		case PARAM_TYPE::kFormType:
			data.i = static_cast<std::int32_t>(RE::StringToFormType(a_param));
			break;
		case PARAM_TYPE::kCastingSource:
			{
				switch (string::const_hash(a_param)) {
				case "Instant"_h:
					data.i = static_cast<std::int32_t>(RE::MagicSystem::CastingSource::kInstant);
					break;
				case "Left"_h:
					data.i = static_cast<std::int32_t>(RE::MagicSystem::CastingSource::kLeftHand);
					break;
				case "Right"_h:
					data.i = static_cast<std::int32_t>(RE::MagicSystem::CastingSource::kRightHand);
					break;
				case "Voice"_h:
					data.i = static_cast<std::int32_t>(RE::MagicSystem::CastingSource::kOther);
					break;
				default:
					data.i = -1;
					break;
				}
			}
			break;
		case PARAM_TYPE::kWardState:
			{
				switch (string::const_hash(a_param)) {
				case "Absorb"_h:
					data.i = static_cast<std::int32_t>(RE::MagicSystem::WardState::kAbsorb);
					break;
				case "Break"_h:
					data.i = static_cast<std::int32_t>(RE::MagicSystem::WardState::kBreak);
					break;
				case "NONE"_h:
					data.i = static_cast<std::int32_t>(RE::MagicSystem::WardState::kNone);
					break;
				default:
					data.i = -1;
					break;
				}
			}
			break;
		case PARAM_TYPE::kCritStage:
			{
				switch (string::const_hash(a_param)) {
				case "NONE"_h:
					data.i = static_cast<std::int32_t>(RE::ACTOR_CRITICAL_STAGE::kNone);
					break;
				case "GooStart"_h:
					data.i = static_cast<std::int32_t>(RE::ACTOR_CRITICAL_STAGE::kGooStart);
					break;
				case "GooEnd"_h:
					data.i = static_cast<std::int32_t>(RE::ACTOR_CRITICAL_STAGE::kGooEnd);
					break;
				case "DisintegrateStart"_h:
					data.i = static_cast<std::int32_t>(RE::ACTOR_CRITICAL_STAGE::kDisintegrateStart);
					break;
				case "DisintegrateEnd"_h:
					data.i = static_cast<std::int32_t>(RE::ACTOR_CRITICAL_STAGE::kDisintegrateEnd);
					break;
				default:
					data.i = -1;
					break;
				}
			}
			break;
		case PARAM_TYPE::kCrimeType:
			{
				switch (string::const_hash(a_param)) {
				case "Steal"_h:
					data.i = static_cast<std::int32_t>(RE::PackageNS::CRIME_TYPE::kSteal);
					break;
				case "Pickpocket"_h:
					data.i = static_cast<std::int32_t>(RE::PackageNS::CRIME_TYPE::kPickpocket);
					break;
				case "Trespass"_h:
					data.i = static_cast<std::int32_t>(RE::PackageNS::CRIME_TYPE::kTrespass);
					break;
				case "Attack"_h:
					data.i = static_cast<std::int32_t>(RE::PackageNS::CRIME_TYPE::kAttack);
					break;
				case "Murder"_h:
					data.i = static_cast<std::int32_t>(RE::PackageNS::CRIME_TYPE::kMurder);
					break;
				case "Escape"_h:
					data.i = static_cast<std::int32_t>(RE::PackageNS::CRIME_TYPE::kEscape);
					break;
				case "Werewolf Transformation"_h:
					data.i = static_cast<std::int32_t>(RE::PackageNS::CRIME_TYPE::kUnused);
					break;
				default:
					data.i = -1;
					break;
				}
			}
			break;
		case PARAM_TYPE::kSkillAction:
			{
				switch (string::const_hash(a_param)) {
				case "Normal Usage"_h:
					data.i = 0;
					break;
				case "Power Attack"_h:
					data.i = 1;
					break;
				case "Bash"_h:
					data.i = 2;
					break;
				case "Lockpick Success"_h:
					data.i = 3;
					break;
				case "Lockpick Broken"_h:
					data.i = 4;
					break;
				default:
					data.i = -1;
					break;
				}
			}
			break;
		case PARAM_TYPE::kAlignment:
			{
				switch (string::const_hash(a_param)) {
				case "Good"_h:
					data.i = 0;
					break;
				case "Neutral"_h:
					data.i = 1;
					break;
				case "Evil"_h:
					data.i = 2;
					break;
				case "VeryGood"_h:
					data.i = 3;
					break;
				case "VeryEvil"_h:
					data.i = 4;
					break;
				default:
					data.i = -1;
					break;
				}
			}
			break;
		case PARAM_TYPE::kFurnitureAnimType:
			{
				switch (string::const_hash(a_param)) {
				case "Sit"_h:
					data.i = 1;
					break;
				case "Lay"_h:
					data.i = 2;
					break;
				case "Lean"_h:
					data.i = 4;
					break;
				default:
					data.i = -1;
					break;
				}
			}
			break;
		case PARAM_TYPE::kFurnitureEntryType:
			{
				switch (string::const_hash(a_param)) {
				case "Front"_h:
					data.i = 1 << 16;
					break;
				case "Behind"_h:
					data.i = 1 << 17;
					break;
				case "Left"_h:
					data.i = 1 << 19;
					break;
				case "Right"_h:
					data.i = 1 << 18;
					break;
				case "Up"_h:
					data.i = 1 << 20;
					break;
				default:
					data.i = -1;
					break;
				}
			}
			break;
		case PARAM_TYPE::kObjectRef:
		case PARAM_TYPE::kActor:
		case PARAM_TYPE::kContainerRef:
			{
				if (string::icontains(a_param, "Player")) {
					data.ptr = RE::PlayerCharacter::GetSingleton();
				} else {
					data.ptr = lookup_form(a_param);
				}
			}
			break;
		case PARAM_TYPE::kMapMarker:
		case PARAM_TYPE::kEquipType:
		case PARAM_TYPE::kInventoryObject:
		case PARAM_TYPE::kSpellItem:
		case PARAM_TYPE::kMessage:
		case PARAM_TYPE::kInvObjectOrFormList:
		case PARAM_TYPE::kObjectOrFormList:
		case PARAM_TYPE::kMusic:
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
		case PARAM_TYPE::kShout:
		case PARAM_TYPE::kLocation:
		case PARAM_TYPE::kRefType:
		case PARAM_TYPE::kAssociationType:
		case PARAM_TYPE::kBGSScene:
		case PARAM_TYPE::kForm:
		case PARAM_TYPE::kKnowableForm:
		case PARAM_TYPE::kWordOfPower:
		case PARAM_TYPE::kTopic:
		case PARAM_TYPE::kNPC:
		case PARAM_TYPE::kSound:
		case PARAM_TYPE::kCombatStyle:
		case PARAM_TYPE::kShaderEffect:
		case PARAM_TYPE::kReferenceEffect:
		case PARAM_TYPE::kMenuIcon:
		case PARAM_TYPE::kNote:
		case PARAM_TYPE::kImagespaceMod:
		case PARAM_TYPE::kImagespace:
			data.ptr = lookup_form(a_param);
			break;
		case PARAM_TYPE::kKeyword:
			{
				switch (dist::get_record_type(a_param)) {
				case dist::kEditorID:
					{
						const auto& keywordArray = RE::TESDataHandler::GetSingleton()->GetFormArray<RE::BGSKeyword>();
						const auto  it = std::ranges::find_if(keywordArray, [&](const auto& keyword) { return keyword->formEditorID == a_param.c_str(); });
						if (it != keywordArray.end()) {
							data.ptr = *it;
						}
					}
					break;
				default:
					data.ptr = lookup_form(a_param);
					break;
				}
			}
			break;
		default:
			break;
		}
	}

	std::string PARAM::ToString()
	{
		const auto lookup_form_id = [&]() {
			if (data.ptr) {
				if (auto edid = editorID::get_editorID(data.ptr); !edid.empty()) {
					return edid;
				} else if (data.ptr->GetFile(0)) {
					return fmt::format("0x{:X}", data.ptr->GetLocalFormID()).append(" ~ ").append(data.ptr->GetFile(0)->fileName);
				} else {  // bad
					return fmt::format("0x{:X}", data.ptr->GetFormID());
				}
			}
			return "NONE"s;
		};

		if (state != STATE::kValid) {
			return "NONE"s;
		}

		switch (*type) {
		case PARAM_TYPE::kInt:
		case PARAM_TYPE::kStage:
		case PARAM_TYPE::kRelationshipRank:
			return std::to_string(data.i);
		case PARAM_TYPE::kFloat:
			return std::to_string(data.f);
		case PARAM_TYPE::kActorValue:
			return RE::ActorValueList::GetSingleton()->GetActorValue(static_cast<RE::ActorValue>(data.i))->enumName;
		case PARAM_TYPE::kAxis:
			{
				switch (data.i) {
				case 88:
					return "X"s;
				case 89:
					return "Y"s;
				case 90:
					return "Z"s;
				default:
					return "NONE"s;
				}
			}
		case PARAM_TYPE::kSex:
			{
				switch (data.i) {
				case RE::SEX::kMale:
					return "Male"s;
				case RE::SEX::kFemale:
					return "Female"s;
				default:
					return "NONE"s;
				}
			}
		case PARAM_TYPE::kFormType:
			return std::string(RE::FormTypeToString(static_cast<RE::FormType>(data.i)));
		case PARAM_TYPE::kCastingSource:
			{
				switch (data.i) {
				case static_cast<std::int32_t>(RE::MagicSystem::CastingSource::kInstant):
					return "Instant"s;
				case static_cast<std::int32_t>(RE::MagicSystem::CastingSource::kLeftHand):
					return "Left"s;
				case static_cast<std::int32_t>(RE::MagicSystem::CastingSource::kRightHand):
					return "Right"s;
				case static_cast<std::int32_t>(RE::MagicSystem::CastingSource::kOther):
					return "Voice"s;
				default:
					return "NONE"s;
				}
			}
		case PARAM_TYPE::kWardState:
			{
				switch (data.i) {
				case static_cast<std::int32_t>(RE::MagicSystem::WardState::kAbsorb):
					return "Absorb"s;
				case static_cast<std::int32_t>(RE::MagicSystem::WardState::kBreak):
					return "Break"s;
				default:
					return "NONE"s;
				}
			}
		case PARAM_TYPE::kCritStage:
			{
				switch (data.i) {
				case static_cast<std::int32_t>(RE::ACTOR_CRITICAL_STAGE::kGooStart):
					return "GooStart"s;
				case static_cast<std::int32_t>(RE::ACTOR_CRITICAL_STAGE::kGooEnd):
					return "GooEnd"s;
				case static_cast<std::int32_t>(RE::ACTOR_CRITICAL_STAGE::kDisintegrateStart):
					return "DisintegrateStart"s;
				case static_cast<std::int32_t>(RE::ACTOR_CRITICAL_STAGE::kDisintegrateEnd):
					return "DisintegrateEnd"s;
				default:
					return "NONE"s;
				}
			}
		case PARAM_TYPE::kCrimeType:
			{
				switch (data.i) {
				case static_cast<std::int32_t>(RE::PackageNS::CRIME_TYPE::kSteal):
					return "Steal"s;
				case static_cast<std::int32_t>(RE::PackageNS::CRIME_TYPE::kPickpocket):
					return "Pickpocket"s;
				case static_cast<std::int32_t>(RE::PackageNS::CRIME_TYPE::kTrespass):
					return "Trespass"s;
				case static_cast<std::int32_t>(RE::PackageNS::CRIME_TYPE::kAttack):
					return "Attack"s;
				case static_cast<std::int32_t>(RE::PackageNS::CRIME_TYPE::kMurder):
					return "Murder"s;
				case static_cast<std::int32_t>(RE::PackageNS::CRIME_TYPE::kEscape):
					return "Escape"s;
				case static_cast<std::int32_t>(RE::PackageNS::CRIME_TYPE::kUnused):
					return "Werewolf Transformation"s;
				default:
					return "ANY";
				}
			}
		case PARAM_TYPE::kSkillAction:
			{
				switch (data.i) {
				case 0:
					return "Normal Usage"s;
				case 1:
					return "Power Attack"s;
				case 2:
					return "Bash"s;
				case 3:
					return "Lockpick Success"s;
				case 4:
					return "Lockpick Broken"s;
				default:
					return "NONE"s;
				}
			}
		case PARAM_TYPE::kAlignment:
			{
				switch (data.i) {
				case 0:
					return "Good"s;
				case 1:
					return "Neutral"s;
				case 2:
					return "Evil"s;
				case 3:
					return "VeryGood"s;
				case 4:
					return "VeryEvil"s;
				default:
					return "NONE"s;
				}
			}
		case PARAM_TYPE::kFurnitureAnimType:
			{
				switch (data.i) {
				case 1:
					return "Sit"s;
				case 2:
					return "Lay"s;
				case 4:
					return "Lean"s;
				default:
					return "NONE"s;
				}
			}
		case PARAM_TYPE::kFurnitureEntryType:
			{
				switch (data.i) {
				case 1 << 16:
					return "Front"s;
				case 1 << 17:
					return "Behind"s;
				case 1 << 19:
					return "Left"s;
				case 1 << 18:
					return "Right"s;
				case 1 << 20:
					return "Up"s;
				default:
					return "NONE"s;
				}
			}
		case PARAM_TYPE::kObjectRef:
		case PARAM_TYPE::kActor:
		case PARAM_TYPE::kContainerRef:
			{
				if (data.ptr == RE::PlayerCharacter::GetSingleton()) {
					return "PlayerRef"s;
				} else {
					return lookup_form_id();
				}
			}
		case PARAM_TYPE::kMapMarker:
		case PARAM_TYPE::kEquipType:
		case PARAM_TYPE::kInventoryObject:
		case PARAM_TYPE::kSpellItem:
		case PARAM_TYPE::kMessage:
		case PARAM_TYPE::kInvObjectOrFormList:
		case PARAM_TYPE::kObjectOrFormList:
		case PARAM_TYPE::kMusic:
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
		case PARAM_TYPE::kShout:
		case PARAM_TYPE::kLocation:
		case PARAM_TYPE::kRefType:
		case PARAM_TYPE::kAssociationType:
		case PARAM_TYPE::kBGSScene:
		case PARAM_TYPE::kForm:
		case PARAM_TYPE::kKnowableForm:
		case PARAM_TYPE::kWordOfPower:
		case PARAM_TYPE::kTopic:
		case PARAM_TYPE::kNPC:
		case PARAM_TYPE::kSound:
		case PARAM_TYPE::kCombatStyle:
		case PARAM_TYPE::kShaderEffect:
		case PARAM_TYPE::kReferenceEffect:
		case PARAM_TYPE::kMenuIcon:
		case PARAM_TYPE::kNote:
		case PARAM_TYPE::kImagespaceMod:
		case PARAM_TYPE::kImagespace:
		case PARAM_TYPE::kKeyword:
			return lookup_form_id();
		default:
			return "NONE"s;
		}
	}

	bool PARAM::IsValid() const
	{
		return state != STATE::kInvalid;
	}

	std::vector<RE::CONDITION_ITEM_DATA> ParseConditionList(const std::vector<std::string>& a_conditionList)
	{
		enum TYPE : std::uint32_t
		{
			kConditionItemObject = 0,
			kFunctionID,
			kParam1,
			kParam2,
			kOPCode,
			kFloat,
			kANDOR,

			kTotal = 7
		};

		std::vector<RE::CONDITION_ITEM_DATA> dataVec{};

		for (auto& condition : a_conditionList) {
			if (condition.empty()) {
				continue;
			}

			auto split_condition = string::split(condition, " | ");
			if (split_condition.size() != kTotal) {
				continue;
			}

			for (auto& conditionData : split_condition) {
				string::trim(conditionData);
			}

			RE::CONDITION_ITEM_DATA condData;

			//conditionItemObject
			if (auto condItem = map::get_value<COND_OBJECT>(split_condition[kConditionItemObject])) {
				condData.object = *condItem;
			} else {
				continue;
			}

			switch (*condData.object) {
			case COND_OBJECT::kRef:
			case COND_OBJECT::kLinkedRef:
			case COND_OBJECT::kQuestAlias:
			case COND_OBJECT::kPackData:
			case COND_OBJECT::kEventData:
				continue;
			default:
				break;
			}

			//functionID
			auto funcID = map::get_value<FUNC_ID>(split_condition[kFunctionID]);
			if (funcID) {
				condData.functionData.function = *funcID;
			} else {
				continue;
			}

			//params
			const auto [param1Type, param2Type] = GetFuncType(*funcID);

			PARAM param1(param1Type, split_condition[kParam1]);
			if (param1.IsValid()) {
				condData.functionData.params[0] = std::bit_cast<void*>(param1.data);
			} else {
				continue;
			}

			PARAM param2(param2Type, split_condition[kParam2]);
			if (param2.IsValid()) {
				condData.functionData.params[1] = std::bit_cast<void*>(param2.data);
			} else {
				continue;
			}

			//OPCode
			if (auto opCode = map::get_value<OP_CODE>(split_condition[kOPCode])) {
				condData.flags.opCode = *opCode;
			} else {
				continue;
			}

			//float
			condData.comparisonValue.f = string::to_num<float>(split_condition[kFloat]);

			//operator
			condData.flags.isOR = string::iequals(split_condition[kANDOR], "OR"sv);

			dataVec.emplace_back(condData);
		}

		return dataVec;
	}

	std::vector<std::string> BuildConditionList(const RE::TESCondition* a_conditions)
	{
		std::vector<RE::CONDITION_ITEM_DATA> condDataVec{};

		if (a_conditions) {
			auto tmp = a_conditions->head;
			while (tmp != nullptr) {
				condDataVec.emplace_back(tmp->data);
				tmp = tmp->next;
			}
		}

		if (condDataVec.empty()) {
			return {};
		}

		std::vector<std::string> vec;
		vec.reserve(condDataVec.size());

		for (auto& condData : condDataVec) {
			std::string condition;

			//condition
			auto condObject = stl::to_underlying(*condData.object);
			if (auto condItemStr = map::get_value<std::string>(map::conditionObjs, condObject)) {
				condition += *condItemStr;
			} else {
				condition += std::to_string(condObject);
			}
			condition.append(" | "sv);

			//functionID
			auto funcID = stl::to_underlying(*condData.functionData.function);
			if (auto funcIDStr = map::get_value<std::string>(map::funcIDs, funcID)) {
				condition += *funcIDStr;
			} else {
				condition += std::to_string(funcID);
			}
			condition.append(" | "sv);

			//params
			const auto [param1Type, param2Type] = GetFuncType(*condData.functionData.function);

			PARAM param1(param1Type, condData.functionData.params[0]);
			if (param1.IsValid()) {
				condition.append(param1.ToString());
				condition.append(" | "sv);
			} else {
				continue;
			}

			PARAM param2(param2Type, condData.functionData.params[1]);
			if (param2.IsValid()) {
				condition.append(param2.ToString());
				condition.append(" | "sv);
			} else {
				continue;
			}

			//opCode
			auto opCode = stl::to_underlying(condData.flags.opCode);
			if (auto opCodeStr = map::get_value<std::string>(map::opCodes, opCode)) {
				condition += *opCodeStr;
			} else {
				condition += std::to_string(opCode);
			}
			condition.append(" | "sv);

			//floatVal
			condition += std::to_string(std::roundf(condData.comparisonValue.f));
			condition.append(" | "sv);

			//ANDOR
			condition.append(condData.flags.isOR ? "OR"sv : "AND"sv);

			vec.emplace_back(condition.c_str());
		}

		return vec;
	}

	RE::TESCondition* GetConditions(RE::TESForm& a_form, std::uint32_t a_index)
	{
		RE::TESCondition* condition = nullptr;

		switch (a_form.GetFormType()) {
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
		case RE::FormType::MagicEffect:
			condition = &a_form.As<RE::EffectSetting>()->conditions;
			break;
		case RE::FormType::Info:
			condition = &a_form.As<RE::TESTopicInfo>()->objConditions;
			break;
		case RE::FormType::Package:
			condition = &a_form.As<RE::TESPackage>()->packConditions;
			break;
		case RE::FormType::Perk:
			condition = &a_form.As<RE::BGSPerk>()->perkConditions;
			break;
		case RE::FormType::CameraPath:
			condition = &a_form.As<RE::BGSCameraPath>()->conditions;
			break;
		case RE::FormType::ConstructibleObject:
			condition = &a_form.As<RE::BGSConstructibleObject>()->conditions;
			break;
		case RE::FormType::Faction:
			condition = a_form.As<RE::TESFaction>()->vendorData.vendorConditions;
			break;
		case RE::FormType::Idle:
			condition = &a_form.As<RE::TESIdleForm>()->conditions;
			break;
		case RE::FormType::LoadScreen:
			condition = &a_form.As<RE::TESLoadScreen>()->conditions;
			break;
		default:
			break;
		}

		return condition;
	}
}
