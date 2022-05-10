Scriptname PO3_SKSEFunctions Hidden 

;DOCUMENTATION IS AVAILABLE AT https://github.com/powerof3/PapyrusExtenderSSE/wiki

;----------------------------------------------------------------------------------------------------------
;ACTIVE EFFECT
;----------------------------------------------------------------------------------------------------------
	
	bool Function IsScriptAttachedToActiveEffect(ActiveMagicEffect akActiveEffect, String asScriptName) global native

;----------------------------------------------------------------------------------------------------------	
;ACTORS 
;----------------------------------------------------------------------------------------------------------

	;-------
	;GETTERS
	;-------
	 
	MagicEffect[] Function GetActiveEffects(Actor akActor, bool abShowInactive = false) global native
	
	float Function GetActorAlpha(Actor akActor) global native
	
	float Function GetActorRefraction(Actor akActor) global native
	
	int Function GetActorState(Actor akActor) global native
	
	int Function GetActorSoulSize(Actor akActor) global native
	
	float Function GetActorValueModifier(Actor akActor, int aiModifier, String asActorValue) global native
	
	int Function GetCriticalStage(Actor akActor) global native
		
	Actor[] Function GetCombatAllies(Actor akActor) global native
	
	Actor[] Function GetCombatTargets(Actor akActor) global native
	
	Actor[] Function GetCommandedActors(Actor akActor) global native
	
	Actor Function GetCommandingActor(Actor akActor) global native
	
	Ammo Function GetEquippedAmmo(Actor akActor) global native

	bool Function GetEquippedWeaponIsPoisoned(Actor akActor, bool abLeftHand) global native	
	
	Potion Function GetEquippedWeaponPoison(Actor akActor, bool abLeftHand) global native	
	
	int Function GetEquippedWeaponPoisonCount(Actor akActor, bool abLeftHand) global native	
	
	ColorForm Function GetHairColor(Actor akActor) global native
	
	TextureSet Function GetHeadPartTextureSet(Actor akActor, int aiType) global native
	
	Float Function GetLocalGravityActor(Actor akActor) global native
	
	Actor Function GetMount(Actor akActor) global native
	
	ObjectReference Function GetObjectUnderFeet(Actor akActor) global native
	
	bool Function GetOffersServices(Actor akActor) global native
	
	Actor Function GetRider(Actor akActor) global native
	
	Package Function GetRunningPackage(Actor akActor) global native
	
	ColorForm Function GetSkinColor(Actor akActor) global native
		
	float Function GetTimeDead(Actor akActor) global native
	
	float Function GetTimeOfDeath(Actor akActor) global native
	
	Faction Function GetVendorFaction(Actor akActor) global native
	
	bool Function HasActiveSpell(Actor akActor, Spell akSpell) global native
		
	bool Function HasDeferredKill(Actor akActor) global native
	
	bool Function HasMagicEffectWithArchetype(Actor akActor, String asArchetype) global native
	
	bool Function HasSkin(Actor akActor, Armor akArmorToCheck) global native
	
	bool Function IsActorInWater(Actor akActor) global native
	
	bool Function IsActorUnderwater(Actor akActor) global native
	
	bool Function IsLimbGone(Actor akActor, int aiLimb) global native
	
	bool Function IsQuadruped(Actor akActor) global native

	bool Function IsSoulTrapped(Actor akActor) global native
		
	;-------
	;SETTERS
	;-------
	
	Form[] Function AddAllEquippedItemsToArray(Actor akActor) global native
	
	bool Function AddBasePerk(Actor akActor, Perk akPerk) global native
	
	bool Function AddBaseSpell(Actor akActor, Spell akSpell) global native
	
	Function BlendColorWithSkinTone(Actor akActor, ColorForm akColor, int aiBlendMode, bool abAutoLuminance, float afOpacity) global native
		
	Function DecapitateActor(Actor akActor) global native

	Function FreezeActor(Actor akActor, int type, bool abFreeze) global native
		
	Function KillNoWait(Actor akActor) global native
	
	;DEPECRIATED
	Function MixColorWithSkinTone(Actor akActor, ColorForm akColor, bool abManualMode, float afPercentage) global native
		
	Function RemoveAddedSpells(Actor akActor, String modName, Keyword[] keywords, bool abMatchAll) global native
	
	bool Function RemoveBasePerk(Actor akActor, Perk akPerk) global native
	
	bool Function RemoveBaseSpell(Actor akActor, Spell akSpell) global native
		
	Function ReplaceArmorTextureSet(Actor akActor, Armor akArmor, TextureSet akSourceTXST, TextureSet akTargetTXST, int aiTextureType = -1) global native
	
	Function ReplaceFaceTextureSet(Actor akActor, TextureSet akMaleTXST, TextureSet akFemaleTXST, int aiTextureType = -1) global native
	
	Function ReplaceSkinTextureSet(Actor akActor, TextureSet akMaleTXST, TextureSet akFemaleTXST, int aiSlotMask, int aiTextureType = -1) global native
	
	bool Function ResetActor3D(Actor akActor, String asFolderName) global native
	
	Function SetActorRefraction(Actor akActor, float afRefraction) global native
	
	Function SetHairColor(Actor akActor, ColorForm akColor) global native
	
	Function SetHeadPartAlpha(Actor akActor, int aiPartType, float afAlpha) global native
	 
	Function SetHeadPartTextureSet(Actor akActor, TextureSet headpartTXST, int aiType) global native	
	
	Function SetLinearVelocity(Actor akActor, float afX, float afY, float afZ) global native
	
	Function SetLocalGravityActor(Actor akActor, float afValue, bool abDisableGravityOnGround) global native
	
	Function SetSkinAlpha(Actor akActor, float afAlpha) global native
	
	Function SetSkinColor(Actor akActor, ColorForm akColor) global native
	
	Function SetSoulTrapped(Actor akActor, bool abTrapped) global native
	
	Function ToggleHairWigs(Actor akActor, bool abDisable) global native
	
	Function UnequipAllOfType(Actor akActor, int afArmorType, int[] aiSlotsToSkip) global native
			
;----------------------------------------------------------------------------------------------------------
;ACTORBASE
;----------------------------------------------------------------------------------------------------------

	;-------
	;GETTERS
	;-------
	
	AssociationType Function GetAssociationType(Actorbase akBase1, Actorbase akBase2) global native
	
	LeveledItem Function GetDeathItem(Actorbase akBase) global native
	
	Perk Function GetNthPerk(Actorbase akBase, int aiIndex) global native
	
	int Function GetPerkCount(Actorbase akBase) global native
	
	Actorbase[] Function GetRelationships(Actorbase akBase, AssociationType akAssocType) global native
	
	;-------
	;SETTERS
	;-------
	
	Function SetDeathItem(Actorbase akBase, LeveledItem akLeveledItem) global native
	
;----------------------------------------------------------------------------------------------------------
;ALIAS
;----------------------------------------------------------------------------------------------------------
	
	bool Function IsScriptAttachedToAlias(Alias akAlias, String asScriptName) global native
	
;----------------------------------------------------------------------------------------------------------
;ARMOR/ADDONS
;----------------------------------------------------------------------------------------------------------
	
	FootstepSet Function GetFootstepSet(ArmorAddon akArma) global native
	
	Function SetFootstepSet(ArmorAddon akArma, FootstepSet akFootstepSet) global native
	
;----------------------------------------------------------------------------------------------------------	
;ARRAYS
;----------------------------------------------------------------------------------------------------------
	
	bool Function AddActorToArray(Actor akActor, Actor[] actorArray) global native

	bool Function AddStringToArray(String asString, String[] asStrings) global native

	int Function ArrayStringCount(String asString, String[] asStrings) global native

	String[] Function SortArrayString(String[] asStrings) global native	
	
	String[] Function GetSortedActorNames(Keyword akKeyword, String asPlural = "(s)", bool abInvertKeyword) global native	
	
	String[] Function GetSortedNPCNames(ActorBase[] aiActorBases, String asPlural = "(s)") global native	
	
;----------------------------------------------------------------------------------------------------------	
;BOOK
;----------------------------------------------------------------------------------------------------------
	
	Function ClearReadFlag(Book akBook) global native
	
	Function SetReadFlag(Book akBook) global native
	
;----------------------------------------------------------------------------------------------------------	
;CELL
;----------------------------------------------------------------------------------------------------------
	
	float Function GetCellNorthRotation(Cell akCell) global native
	
	LightingTemplate Function GetLightingTemplate(Cell akCell) global native
	
	Function SetLightingTemplate(Cell akCell, LightingTemplate akLightingTemplate) global native
	
	Function SetCellNorthRotation(Cell akCell, float afAngle) global native
	
;----------------------------------------------------------------------------------------------------------	
;DEBUG
;----------------------------------------------------------------------------------------------------------
	
	Function GivePlayerSpellBook() global native
	
	Function DumpAnimationVariables(Actor akActor, String asAnimationVarPrefix) global native

;----------------------------------------------------------------------------------------------------------	
;DETECTION
;----------------------------------------------------------------------------------------------------------
	
	int Function CanActorBeDetected(Actor akActor) global native

	int Function CanActorDetect(Actor akActor) global native

	Function ForceActorDetection(Actor akActor) global native

	Function ForceActorDetecting(Actor akActor) global native
	
	Bool Function IsDetectedByAnyone(Actor akActor) global native
	
	Function PreventActorDetection(Actor akActor) global native

	Function PreventActorDetecting(Actor akActor) global native

	Function ResetActorDetection(Actor akActor) global native

	Function ResetActorDetecting(Actor akActor) global native
		
;----------------------------------------------------------------------------------------------------------	
;EFFECTSHADER
;----------------------------------------------------------------------------------------------------------

	;-------
	;GETTERS
	;-------
	
	Debris Function GetAddonModels(EffectShader akEffectShader) global native
	
	int Function GetEffectShaderTotalCount(EffectShader akEffectShader, bool abActive) global native
	
	bool Function IsEffectShaderFlagSet(EffectShader akEffectShader, int aiFlag) global native
	
	String Function GetMembraneFillTexture(EffectShader akEffectShader) global native
	
	String Function GetMembraneHolesTexture(EffectShader akEffectShader) global native
	
	String Function GetMembranePaletteTexture(EffectShader akEffectShader) global native
	
	float Function GetParticleFullCount(EffectShader akEffectShader) global native
	
	String Function GetParticlePaletteTexture(EffectShader akEffectShader) global native
	
	String Function GetParticleShaderTexture(EffectShader akEffectShader) global native
	
	float Function GetParticlePersistentCount(EffectShader akEffectShader) global native
	
	;-------
	;SETTERS
	;-------
	
	Function ClearEffectShaderFlag(EffectShader akEffectShader, int aiFlag) global native
	
	Function SetAddonModels(EffectShader akEffectShader, Debris akDebris) global native
	
	Function SetEffectShaderFlag(EffectShader akEffectShader, int aiFlag) global native
	
	Function SetMembraneColorKeyData(EffectShader akEffectShader, int aiColorKey, int[] aiRGB, float afAlpha, float afTime) global native
	
	Function SetMembraneFillTexture(EffectShader akEffectShader, String asTextureName) global native
	
	Function SetMembraneHolesTexture(EffectShader akEffectShader, String asTextureName) global native
	
	Function SetMembranePaletteTexture(EffectShader akEffectShader, String asTextureName) global native
	
	Function SetParticleColorKeyData(EffectShader akEffectShader, int aiColorKey, int[] aiRGB, float afAlpha, float afTime) global native
	
	Function SetParticleFullCount(EffectShader akEffectShader, float afParticleCount) global native
	
	Function SetParticlePaletteTexture(EffectShader akEffectShader, String asTextureName) global native
	
	Function SetParticlePersistentCount(EffectShader akEffectShader, float afParticleCount) global native
	
	Function SetParticleShaderTexture(EffectShader akEffectShader, String asTextureName) global native
	
;-----------------------------------------------------------------------------------------------------------	
;ENCHANTMENT
;-----------------------------------------------------------------------------------------------------------	
	;--------
	;GETTERS
	;--------

	int Function GetEnchantmentType(Enchantment akEnchantment) global native
		
	;--------
	;SETTERS
	;--------
	
	Function AddMagicEffectToEnchantment(Enchantment akEnchantment, MagicEffect akMagicEffect, float afMagnitude, int aiArea, int aiDuration, float afCost = 0.0, String[] asConditionList) global native	
	
	Function AddEffectItemToEnchantment(Enchantment akEnchantment, Enchantment akEnchantmentToCopyFrom, int aiIndex, float afCost = -1.0) global native	
	
	Function RemoveMagicEffectFromEnchantment(Enchantment akEnchantment, MagicEffect akMagicEffect, float afMagnitude, int aiArea, int aiDuration, float afCost = 0.0) global native
	
	Function RemoveEffectItemFromEnchantment(Enchantment akEnchantment, Enchantment akEnchantmentToMatchFrom, int aiIndex) global native
	
;----------------------------------------------------------------------------------------------------------	
;FACTION
;----------------------------------------------------------------------------------------------------------
		
	ObjectReference Function GetVendorFactionContainer(Faction akVendorFaction)  global native	
	
;----------------------------------------------------------------------------------------------------------	
;FORM
;----------------------------------------------------------------------------------------------------------

	;-------
	;GETTERS
	;-------

	bool Function EvaluateConditionList(Form akForm, ObjectReference akActionRef, ObjectReference akTargetRef) global native
	
	Function ClearRecordFlag(Form akForm, int aiFlag) global native
	
	String[] Function GetConditionList(Form akForm, int aiIndex = 0) global native
	
	String Function GetDescription(Form akForm) global native
	
	String Function GetFormEditorID(Form akForm) global native
	
	bool Function IsFormInMod(Form akForm, String asModName) global native
	
	bool Function IsGeneratedForm(Form akForm) global native	
	
	bool Function IsRecordFlagSet(Form akForm, int aiFlag) global native
	
	bool Function IsScriptAttachedToForm(Form akForm, String asScriptName) global native
	
	Function SetRecordFlag(Form akForm, int aiFlag) global native
	
	;-------
	;SETTERS
	;-------
	
	Function AddKeywordToForm(Form akForm, Keyword akKeyword) global native
	
	Function MarkItemAsFavorite(Form akForm) global native
	
	Function ReplaceKeywordOnForm(Form akForm, Keyword akKeywordAdd, Keyword akKeywordRemove) global native
	
	bool Function RemoveKeywordOnForm(Form akForm, Keyword akKeyword) global native
	
	Function UnmarkItemAsFavorite(Form akForm) global native

;----------------------------------------------------------------------------------------------------------	
;FURNITURE
;----------------------------------------------------------------------------------------------------------
		
	int Function GetFurnitureType(Furniture akFurniture) global native
	
;----------------------------------------------------------------------------------------------------------	
;GAME
;----------------------------------------------------------------------------------------------------------
		
	;-------
	;GETTERS
	;-------
	
	Enchantment[] Function GetAllEnchantments(Keyword[] akKeywords = None) global native
	
	Form[] Function GetAllForms(int aiFormType, Keyword[] akKeywords = None) global native
	
	Race[] Function GetAllRaces(Keyword[] akKeywords = None) global native
	
	Spell[] Function GetAllSpells(Keyword[] akKeywords = None, bool abIsPlayable = false) global native
				
	Actor[] Function GetActorsByProcessingLevel(int aiLevel) global native
	
	Form[] Function GetAllFormsInMod(String asModName, int aiFormType, Keyword[] akKeywords = None) global native
	
	Enchantment[] Function GetAllEnchantmentsInMod(String asModName, Keyword[] akKeywords = None) global native
	
	Race[] Function GetAllRacesInMod(String asModName, Keyword[] akKeywords = None) global native
	
	Spell[] Function GetAllSpellsInMod(String asModName, Keyword[] akKeywords = None, bool abIsPlayable = false) global native
	
	Cell[] Function GetAttachedCells() global native
	
	Form Function GetFormFromEditorID(String asEditorID) global native
	
	Int Function GetGameSettingBool(String asGameSetting) global native
	
	Bool Function GetGodMode() global native
	
	Float[] Function GetLocalGravity() global native
	
	int Function GetNumActorsInHigh() global native
	
	Actor[] Function GetPlayerFollowers() global native
	
	String Function GetSurfaceMaterialType(float afX, float afY, float afZ) global native
	
	bool Function IsPluginFound(String akName) global native
	
	bool Function IsSurvivalModeActive() global native
	
	;-------
	;SETTERS
	;-------
	
	Function ClearCachedFactionFightReactions() global native
	
	Function SetLocalGravity(float afXAxis, float afYAxis, float afZAxis) global native
	
;----------------------------------------------------------------------------------------------------------	
;HAZARD
;----------------------------------------------------------------------------------------------------------

	String Function GetHazardArt(Hazard akHazard) global native
	
	ImageSpaceModifier Function GetHazardIMOD(Hazard akHazard) global native

	float Function GetHazardIMODRadius(Hazard akHazard) global native
	
	ImpactDataSet Function GetHazardIPDS(Hazard akHazard) global native
	
	float Function GetHazardLifetime(Hazard akHazard) global native
	
	Light Function GetHazardLight(Hazard akHazard) global native

	int Function GetHazardLimit(Hazard akHazard) global native
	
	float Function GetHazardRadius(Hazard akHazard) global native

	SoundDescriptor Function GetHazardSound(Hazard akHazard) global native
	
	Spell Function GetHazardSpell(Hazard akHazard) global native

	float Function GetHazardTargetInterval(Hazard akHazard) global native

	bool Function IsHazardFlagSet(Hazard akHazard, int aiFlag) global native

	;-------
	;SETTERS
	;-------
	
	Function ClearHazardFlag(Hazard akHazard, int aiFlag) global native
	
	Function SetHazardArt(Hazard akHazard, String asPath) global native

	Function SetHazardFlag(Hazard akHazard, int aiFlag) global native

	Function SetHazardIMOD(Hazard akHazard, ImageSpaceModifier akIMOD) global native

	Function SetHazardIMODRadius(Hazard akHazard, float afRadius) global native
	
	Function SetHazardIPDS(Hazard akHazard, ImpactDataSet akIPDS) global native
	
	Function SetHazardLifetime(Hazard akHazard, float afLifetime) global native
	
	Function SetHazardLight(Hazard akHazard, Light akLight) global native

	Function SetHazardLimit(Hazard akHazard, int aiLimit) global native
	
	Function SetHazardRadius(Hazard akHazard, float afRadius) global native

	Function SetHazardSound(Hazard akHazard, SoundDescriptor akSound) global native

	Function SetHazardSpell(Hazard akHazard, Spell akspell) global native

	Function SetHazardTargetInterval(Hazard akHazard, float afInterval) global native
		
;----------------------------------------------------------------------------------------------------------
;LIGHT
;----------------------------------------------------------------------------------------------------------

	;-------
	;GETTERS
	;-------

	ColorForm Function GetLightColor(Light akLight) global native
		
	float Function GetLightFade(Light akLight) global native
	
	float Function GetLightFOV(Light akLight) global native
	
	float Function GetLightRadius(Light akLight) global native
	
	int[] Function GetLightRGB(Light akLight) global native
	
	float Function GetLightShadowDepthBias(ObjectReference akLightObject) global native
	
	int Function GetLightType(Light akLight) global native
	
	;-------
	;SETTERS
	;-------
	
	Function SetLightColor(Light akLight, ColorForm akColorform) global native
		
	Function SetLightFade(Light akLight, float afRange) global native
		
	Function SetLightFOV(Light akLight, float afFOV) global native
	
	Function SetLightRadius(Light akLight, float afRadius) global native
	
	Function SetLightRGB(Light akLight, int[] aiRGB) global native
	
	Function SetLightShadowDepthBias(ObjectReference akLightObject, float afDepthBias) global native
	
	Function SetLightType(Light akLight, int aiLightType) global native
	
;----------------------------------------------------------------------------------------------------------
;LOCATION
;----------------------------------------------------------------------------------------------------------

	;-------
	;GETTERS
	;-------

	Location Function GetParentLocation(Location akLoc) global native
	
	;-------
	;SETTERS
	;-------
	
	Function SetParentLocation(Location akLoc, Location akNewLoc) global native
	
;----------------------------------------------------------------------------------------------------------		
;MAGIC EFFECTS
;----------------------------------------------------------------------------------------------------------		

	;---------
	;GETTERS
	;---------
	
	Form Function GetAssociatedForm(MagicEffect akMagicEffect) global native
	
	int Function GetEffectArchetypeAsInt(MagicEffect akMagicEffect) global native
	
	String Function GetEffectArchetypeAsString(MagicEffect akMagicEffect) global native
	
	String Function GetPrimaryActorValue(MagicEffect akMagicEffect) global native
	
	String Function GetSecondaryActorValue(MagicEffect akMagicEffect) global native
		
	SoundDescriptor Function GetMagicEffectSound(MagicEffect akMagicEffect, int aiType) global native
	
	;-------
	;SETTERS
	;-------
	
	Function SetAssociatedForm(MagicEffect akMagicEffect, Form akForm) global native
	
	Function SetMagicEffectSound(MagicEffect akMagicEffect, SoundDescriptor akSoundDescriptor, int aiType) global native
	
;----------------------------------------------------------------------------------------------------------
;OBJECTREFERENCES
;----------------------------------------------------------------------------------------------------------

	;--------
	;GETTERS
	;--------
		
	Form[] Function AddAllItemsToArray(ObjectReference akRef, bool abNoEquipped = true, bool abNoFavorited = false, bool abNoQuestItem = false) global native
	
	Function AddAllItemsToList(ObjectReference akRef, Formlist akList, bool abNoEquipped = true, bool abNoFavorited = false, bool abNoQuestItem = false) global native
	
	Form[] Function AddItemsOfTypeToArray(ObjectReference akRef, int aiFormType, bool abNoEquipped = true, bool abNoFavorited = false, bool abNoQuestItem = false) global native
	
	Function AddItemsOfTypeToList(ObjectReference akRef, Formlist akList, int aiFormType, bool abNoEquipped = true, bool abNoFavorited = false, bool abNoQuestItem = false) global native
	
	ObjectReference[] Function FindAllReferencesOfFormType(ObjectReference akRef, int formType, float afRadius) global native
	
	ObjectReference[] Function FindAllReferencesWithKeyword(ObjectReference akRef, Form keywordOrList, float afRadius, bool abMatchAll) global native
	
	ObjectReference[] Function FindAllReferencesOfType(ObjectReference akRef, Form akFormOrList, float afRadius) global native
	
	Form Function FindFirstItemInList(ObjectReference akRef, FormList akList) global native
	
	ObjectReference[] Function GetActivateChildren(ObjectReference akRef) global native
	
	String Function GetActiveGamebryoAnimation(ObjectReference akRef) global native
	
	Actor Function GetActorCause(ObjectReference akRef) global native
	
	Art[] Function GetAllArtObjects(ObjectReference akRef) global native
	
	EffectShader[] Function GetAllEffectShaders(ObjectReference akRef) global native
		
	Actor Function GetClosestActorFromRef(ObjectReference akRef, bool abIgnorePlayer) global native
	
	float Function GetEffectShaderDuration(ObjectReference akRef, EffectShader akShader) global native
	
	ObjectReference Function GetDoorDestination(ObjectReference akRef) global native
	
	ObjectReference[] Function GetLinkedChildren(ObjectReference akRef, Keyword akKeyword) global native
	
	Form[] Function GetMagicEffectSource(ObjectReference akRef, MagicEffect akEffect) global native
		
	String[] Function GetMaterialType(ObjectReference akRef, String asNodeName = "") global native
	
	int Function GetMotionType(ObjectReference akRef) global native
	
	Actor Function GetRandomActorFromRef(ObjectReference akRef, float afRadius, bool abIgnorePlayer) global native
	
	Form[] Function GetQuestItems(ObjectReference akRef, bool abNoEquipped = false, bool abNoFavorited = false) global native
	
	Alias[] Function GetRefAliases(ObjectReference akRef) global native
		
	int Function GetStoredSoulSize(ObjectReference akRef) global native
	
	int Function HasArtObject(ObjectReference akRef, Art akArtObject, bool abActive = false) global native
	
	int Function HasEffectShader(ObjectReference akRef, EffectShader akShader, bool abActive = false) global native
	
	bool Function HasNiExtraData(ObjectReference akRef, String asName) global native
	
	bool Function IsCasting(ObjectReference akRef, Form akMagicItem) global native
	
	bool Function IsLoadDoor(ObjectReference akRef) global native
	
	bool Function IsQuestItem(ObjectReference akRef) global native
	
	bool Function IsVIP(ObjectReference akRef) global native
	
	;-------
	;SETTERS
	;-------
		
	Function ApplyMaterialShader(ObjectReference akRef, MaterialObject akMatObject, float directionalThresholdAngle) global native
	
	Function AddKeywordToRef(ObjectReference akRef, Keyword akKeyword) global native	
	
	Function MoveToNearestNavmeshLocation(ObjectReference akRef) global native
				
	Function RemoveAllModItems(ObjectReference akRef, String asModName, bool abOnlyUnequip = false) global native
	
	bool Function RemoveKeywordFromRef(ObjectReference akRef, Keyword akKeyword) global native
	
	Function ReplaceKeywordOnRef(ObjectReference akRef, Keyword akKeywordAdd, Keyword akKeywordRemove) global native
	
	Function PlayDebugShader(ObjectReference akRef, float[] afRGBA) global native
	
	Function ScaleObject3D(ObjectReference akRef, String asNodeName, float afScale) global native
	
	Function SetBaseObject(ObjectReference akRef, Form akBaseObject) global native
	
	Function SetCollisionLayer(ObjectReference akRef, String asNodeName, int aiCollisionLayer) global native
	
	bool Function SetDoorDestination(ObjectReference akRef, ObjectReference akDoor) global native
	
	Function SetEffectShaderDuration(ObjectReference akRef, EffectShader akShader, float afTime, bool abAbsolute) global native
		
	Function SetLinkedRef(ObjectReference akRef, ObjectReference akTargetRef, Keyword akKeyword = None) global native
	
	Function SetMaterialType(ObjectReference akRef, String asNewMaterial, String asOldMaterial = "", String asNodeName = "") global native
	
	Function SetupBodyPartGeometry(ObjectReference akRef, actor akActor) global native
	
	Function SetShaderType(ObjectReference akRef, ObjectReference akTemplate, String asDiffusePath, int aiShaderType, int aiTextureType, bool abNoWeapons, bool abNoAlphaProperty) global native
								
	Function StopAllShaders(ObjectReference akRef) global native
	
	Function StopArtObject(ObjectReference akRef, Art akArt) global native
	
	Function ToggleChildNode(ObjectReference akRef, String asNodeName, bool abDisable) global native
	
	Function UpdateHitEffectArtNode(ObjectReference akRef, Art akArt, String asNewNode, float[] afTranslate, float[] afRotate, float afRelativeScale = 1.0) global native
							
;----------------------------------------------------------------------------------------------------------
;PACKAGES
;----------------------------------------------------------------------------------------------------------

	;-------
	;GETTERS
	;-------
	
	int Function GetPackageType(Package akPackage) global native
	
	Idle[] Function GetPackageIdles(Package akPackage) global native
	
	;-------
	;SETTERS
	;-------
	
	Function AddPackageIdle(Package akPackage, Idle akIdle) global native
	
	Function RemovePackageIdle(Package akPackage, Idle akIdle) global native
	
;----------------------------------------------------------------------------------------------------------
;PAPYRUS EXTENDER
;----------------------------------------------------------------------------------------------------------

	;(major,minor,patch / 5,1,0)
	int[] Function GetPapyrusExtenderVersion() global native
	
;-----------------------------------------------------------------------------------------------------------	
;POTION
;-----------------------------------------------------------------------------------------------------------	
	
	Function AddMagicEffectToPotion(Potion akPotion, MagicEffect akMagicEffect, float afMagnitude, int aiArea, int aiDuration, float afCost = 0.0, String[] asConditionList) global native	
	
	Function AddEffectItemToPotion(Potion akPotion, Potion akPotionToCopyFrom, int aiIndex, float afCost = -1.0) global native	
	
	Function RemoveMagicEffectFromPotion(Potion akPotion, MagicEffect akMagicEffect, float afMagnitude, int aiArea, int aiDuration, float afCost = 0.0) global native
	
	Function RemoveEffectItemFromPotion(Potion akPotion, Potion akPotionToMatchFrom, int aiIndex) global native
		
;----------------------------------------------------------------------------------------------------------
;PROJECTILES
;----------------------------------------------------------------------------------------------------------

	;--------
	;GETTERS
	;--------
	
	float Function GetProjectileGravity(Projectile akProjectile) global native
	
	float Function GetProjectileImpactForce(Projectile akProjectile) global native
	
	float Function GetProjectileRange(Projectile akProjectile) global native
	
	float Function GetProjectileSpeed(Projectile akProjectile) global native
	
	int Function GetProjectileType(Projectile akProjectile) global native
	
	;-------
	;SETTERS
	;-------
	
	Function SetProjectileGravity(Projectile akProjectile, float afGravity) global native
	
	Function SetProjectileImpactForce(Projectile akProjectile, float afImpactForce) global native
	
	Function SetProjectileRange(Projectile akProjectile, float afRange) global native
	
	Function SetProjectileSpeed(Projectile akProjectile, float afSpeed) global native
	
;-----------------------------------------------------------------------------------------------------------	
;QUEST
;-----------------------------------------------------------------------------------------------------------	

	int[] Function GetAllQuestObjectives(Quest akQuest) global native

;-----------------------------------------------------------------------------------------------------------	
;SCROLL
;-----------------------------------------------------------------------------------------------------------	
	
	Function AddMagicEffectToScroll(Scroll akScroll, MagicEffect akMagicEffect, float afMagnitude, int aiArea, int aiDuration, float afCost = 0.0, String[] asConditionList) global native	
	
	Function AddEffectItemToScroll(Scroll akScroll, Scroll akScrollToCopyFrom, int aiIndex, float afCost = -1.0) global native	
	
	Function RemoveMagicEffectFromScroll(Scroll akScroll, MagicEffect akMagicEffect, float afMagnitude, int aiArea, int aiDuration, float afCost = 0.0) global native
	
	Function RemoveEffectItemFromScroll(Scroll akScroll, Scroll akScrollToMatchFrom, int aiIndex) global native
	
;-----------------------------------------------------------------------------------------------------------	
;SOUND
;-----------------------------------------------------------------------------------------------------------	

	Function SetSoundDescriptor(Sound akSound, SoundDescriptor akSoundDescriptor) global native
	
;-----------------------------------------------------------------------------------------------------------	
;SPELL
;-----------------------------------------------------------------------------------------------------------	
	;--------
	;GETTERS
	;--------
	
	int Function GetSpellType(Spell akSpell) global native
		
	;--------
	;SETTERS
	;--------
	
	Function AddMagicEffectToSpell(Spell akSpell, MagicEffect akMagicEffect, float afMagnitude, int aiArea, int aiDuration, float afCost = 0.0, String[] asConditionList) global native	
	
	Function AddEffectItemToSpell(Spell akSpell, Spell akSpellToCopyFrom, int aiIndex, float afCost = -1.0) global native	
	
	Function RemoveMagicEffectFromSpell(Spell akSpell, MagicEffect akMagicEffect, float afMagnitude, int aiArea, int aiDuration, float afCost = 0.0) global native
	
	Function RemoveEffectItemFromSpell(Spell akSpell, Spell akSpellToMatchFrom, int aiIndex) global native
	
	Function SetSpellCastingType(Spell akSpell, int aiType) global native
	
	Function SetSpellDeliveryType(Spell akSpell, int aiType) global native
			
;----------------------------------------------------------------------------------------------------------	
;STRINGS
;----------------------------------------------------------------------------------------------------------

	String Function IntToString(int aiValue, bool abHex) global native
	
	int Function StringToInt(String asString) global native
	
;----------------------------------------------------------------------------------------------------------
;UI
;----------------------------------------------------------------------------------------------------------
	
	ObjectReference Function GetMenuContainer() global native
	
;----------------------------------------------------------------------------------------------------------
;UTILITY
;----------------------------------------------------------------------------------------------------------
	
	float Function GenerateRandomFloat(float afMin, float afMax) global native
	
	int Function GenerateRandomInt(int afMin, int afMax) global native
	
	int[] Function GetSystemTime() global native
	
;-----------------------------------------------------------------------------------------------------------
;VISUALEFFECTS
;----------------------------------------------------------------------------------------------------------		

	;--------
	;GETTERS
	;--------
	
	Art Function GetArtObject(VisualEffect akEffect) global native
	
	int Function GetArtObjectTotalCount(VisualEffect akEffect, bool abActive) global native
	
	;--------
	;SETTERS
	;--------
	
	Function SetArtObject(VisualEffect akEffect, Art akArt) global native
			
;-----------------------------------------------------------------------------------------------------------
;WEATHER
;----------------------------------------------------------------------------------------------------------		

	float Function GetWindSpeedAsFloat(Weather akWeather) global native
	
	int Function GetWindSpeedAsInt(Weather akWeather) global native
	
	int Function GetWeatherType(Weather akWeather = None) global native 