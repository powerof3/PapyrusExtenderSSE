Scriptname PO3_SKSEFunctions Hidden 

;----------------------------------------------------------------------------------------------------------	
;ACTORS 
;----------------------------------------------------------------------------------------------------------

	;-------
	;GETTERS
	;-------
	 
	;Gets all magiceffects currently on the actor. Filters out inactive and hideinui spells.
	MagicEffect[] Function GetActiveEffects(Actor akActor, bool abShowInactive = false) global native
	
	float Function GetActorAlpha(Actor akActor) global native
	
	float Function GetActorRefraction(Actor akActor) global native
	
	;/	ACTOR STATE
		Alive = 0
		Dying = 1
		Dead = 2
		Unconscious = 3
		Reanimate = 4
		Recycle" = 5
		Restrained = 6
		EssentialDown = 7
		Bleedout = 8
	/;
	
	;Gets actor state 
	int Function GetActorState(Actor thisActor) global native
	
	;Gets actor soul size
	int Function GetActorSoulSize(Actor thisActor) global native
	
	;Gets actor critical stage
	int Function GetCriticalStage(Actor thisActor) global native
		
	;Gets all allies of the actor, if in combat
	Actor[] Function GetCombatAllies(Actor akActor) global native
	
	;Gets all targets of the actor, if in combat
	Actor[] Function GetCombatTargets(Actor akActor) global native
	
	;Gets current hair color on actor. Fails if hair headpart doesn't exist
	ColorForm Function GetHairColor(Actor akActor) global native
	
	;Gets textureset belonging to headpart, if any. 
	TextureSet Function GetHeadPartTextureSet(Actor akActor, int aiType) global native
	
	;Gets the actor's local gravity.
	Float Function GetLocalGravityActor(Actor akActor) global native
	
	;Gets object under actor's feet (eg. table). Does not work if the player is standing on the ground.
	ObjectReference Function GetObjectUnderFeet(Actor akActor) global native
	
	;Gets actual current package on actor, including internal packages used by the game (see GetPackageType below)
	Package Function GetRunningPackage(Actor akActor) global native
	
	;Gets current skin color on actor. 
	ColorForm Function GetSkinColor(Actor akActor) global native
		
	;Similar to GetTimeDead console command. Returns 0.0 if actor is alive
	float Function GetTimeDead(Actor akActor) global native
	
	;Returns time of death in game days passed
	float Function GetTimeOfDeath(Actor akActor) global native
	
	;HasSpell but checks if the spell is present on the actor (i.e active and not dispelled)
	bool Function HasActiveSpell(Actor akActor, Spell akSpell) global native
		
	;Returns whether the actor is in deferred kill mode
	bool Function HasDeferredKill(Actor akActor) global native
	
	;Checks if activemagiceffect with given archetype is present on actor. Archetype MUST be typed as given below.
	bool Function HasMagicEffectWithArchetype(Actor akActor, String asArchetype) global native
	
	;Returns whether the actor is in cell water or lava
	bool Function IsActorInWater(Actor akActor) global native
	
	;Returns whether the actor is underwater
	bool Function IsActorUnderwater(Actor akActor) global native
	
	;/	LIMB
		None = -1
		Torso = 0
		Head = 1
		...
	/;
	
	;Returns whether limb is gone (i.e, the head, but adding the whole enum in case someone expands the dismemberment system in the future)
	bool Function IsLimbGone(Actor akActor, int aiLimb) global native
	
	;Returns whether the actor is a quadruped
	bool Function IsQuadruped(Actor akActor) global native
	
	;Returns whether target is soul trapped / capable of being soul trapped successfully (if using mods that bypass vanilla soul trap system).
	bool Function IsSoulTrapped(Actor akActor) global native
		
	;-------
	;SETTERS
	;-------
	
	;Adds all equipped items to array
	Form[] Function AddAllEquippedItemsToArray(Actor akActor) global native
	
	;Adds perks to the actorbase, works on leveled actors/unique NPCs. Function serializes data to skse cosave, so perks are applied correctly on loading/reloading saves.
	bool Function AddBasePerk(Actor akActor, Perk akPerk) global native
	
	;Adds spells to actorbase, works on player/leveled actors/unique NPCs. Function serializes data to skse cosave, so spells are applied correctly on loading/reloading saves.
	bool Function AddBaseSpell(Actor akActor, Spell akSpell) global native
	
	;/
		BLEND MODES
		Darken = 0
		Multiply = 1
		ColorBurn = 2
		LinearBurn = 3
		DarkerColor = 4
		Lighten = 5
		Screen = 6
		ColorDodge = 7
		LinearDodge = 8
		LighterColor = 9	
		Overlay = 10
		SoftLight = 11
		HardLight = 12
		VividLight = 13
		LinearLight = 14
		PinLight = 15
		HardMix = 16	
		Difference = 17
		Exclusion = 18
		Subtract = 19
		Divide = 20
	/;
	
	;Blends existing skin color with specified color, using photoshop blend formulas, with alpha (opacity).
	;If true, autoLuminance calculates skin tone relative luminance. The opacity value is then used as a multiplier on top of that, final value is clamped to 0-1
	;If false, only opacity will be used. Recommend to use autoluminance because colors will not blend well for all skin tones using flat values. 
	Function BlendColorWithSkinTone(Actor akActor, ColorForm akColor, int aiBlendMode, bool abAutoLuminance, float afOpacity) global native
		
	;Decapitates living and dead actors. Living actors will not die when this is called!
	Function DecapitateActor(Actor akActor) global native

	;0 - EnableAI + toggling record hits flags so they don't go flying 300 feet when unfrozen.
	;1 - Paralyzes actor, even when dead.
	Function FreezeActor(Actor akActor, int type, bool abFreeze) global native
		
	;Quick and dirty hack to instantly kill the actor and set as dead.
	Function KillNoWait(Actor akActor) global native
	
	;DEPRECIATED
	;Blends existing skin color with specified color. 
	;True - intensity is manually calculated using percentage 0-1.0, False - automatically calculated using skin tone luminance 
	Function MixColorWithSkinTone(Actor akActor, ColorForm akColor, bool abManualMode, float afPercentage) global native
		
	;Removes perks from the actorbase
	;Perk effects may not be removed from unique actors, more testing required.
	;Function serializes data to skse cosave, so perks are applied correctly on loading/reloading saves.
	bool Function RemoveBasePerk(Actor akActor, Perk akPerk) global native
	
	;Removes spells from the actorbase, works on player/leveled actors/unique NPCs. Function serializes data to skse cosave, so spells are applied correctly on loading/reloading saves.
	bool Function RemoveBaseSpell(Actor akActor, Spell akSpell) global native
	
	;/FEC only Function 
	0 - charred/skeleton
	1 - drained
	2 - poisoned/frightened
	3-  aged
	4 - charred creature
	5 - frozen/;	
	Function RemoveEffectsNotOfType(Actor akActor, int aiEffectType) global native
	
	;Replaces specified source textureset on worn armor with target textureset. Lasts for one single gaming session.
	;If texture type is -1, the entire textureset is replaced, otherwise the texture map specified at [textureType] index is replaced (diffuse is 0, normal is 1...)
	Function ReplaceArmorTextureSet(Actor akActor, Armor akArmor, TextureSet akSourceTXST, TextureSet akTargetTXST, int aiTextureType = -1) global native
	
	;Replaces face textureset. Lasts one gaming session. Can be applied to non-unique actors.
	;If texture type is -1, the entire textureset is replaced, otherwise the texture map specified at [textureType] index is replaced. Replacing the entire textureset may cause a visible neckseam.
	Function ReplaceFaceTextureSet(Actor akActor, TextureSet akMaleTXST, TextureSet akFemaleTXST, int aiTextureType = -1) global native
	
	;Replaces skin textureset for given slotmask (ie. body/hand). Lasts one gaming session. Has to be reapplied when re-equipping armor.
	;If texture type is -1, the entire textureset is replaced, otherwise the texture map specified at [textureType] index is replaced.
	Function ReplaceSkinTextureSet(Actor akActor, TextureSet akMaleTXST, TextureSet akFemaleTXST, int aiSlotMask, int aiTextureType = -1) global native
	
	;Checks for NiExtraData nodes on actor - PO3_TINT/PO3_ALPHA/PO3_TXST/PO3_TOGGLE/PO3_SHADER
	;Stops all effect shaders and
	;PO3_TINT - resets tint, rebuilds facegen if actor is player
	;PO3_ALPHA - resets skin alpha
	;PO3_TXST - resets texturesets with texturepaths containing folderName
	;PO3_TOGGLE - unhides all children of nodes that were written to the extraData
	;PO3_SHADER - recreates the original shader type (as close as possible, projectedUV params are not restored)
	bool Function ResetActor3D(Actor akActor, String asFolderName) global native
	
	; 0 - permanent
	; 1 - temporary
	; 2 - frozenActor
	; 3 - frozenCol
	Function SendFECResetEvent(Actor akActor, int aiType, bool abReset3D) global native
	
	;0.0 disables refraction, 1.0 is max refraction
	Function SetActorRefraction(Actor thisActor, float afRefraction) global native
	
	;Sets hair color on actor. Changes may persist throughout gaming session, even when reloading previous saves.
	Function SetHairColor(Actor akActor, ColorForm akColor) global native
	
	;Sets headpart's mesh alpha. Doesn't work for some hair types and heterochromic eyes
	Function SetHeadPartAlpha(Actor akActor, int aiPartType, float afAlpha) global native
	
	;Sets textureset belonging to headpart, if any. 
	Function SetHeadPartTextureSet(Actor akActor, TextureSet headpartTXST, int aiType) global native	
	
	;Sets velocity of the actor. May not take place immediately.
	Function SetLinearVelocity(Actor akActor, float afX, float afY, float afZ) global native
	
	;Sets local gravity of the actor. Negative values will cause them to fly. May not take place immediately.
	Function SetLocalGravityActor(Actor akActor, float afValue, bool abDisableGravityOnGround) global native
	
	;Sets alpha on face, base skin form and armor meshes with visible skin. Has to be re-applied when armor is un/re-equipped.
	Function SetSkinAlpha(Actor akActor, float afAlpha) global native
	
	;Sets skin color (face and body). Has to be re-applied when armor is un/re-equipped.
	Function SetSkinColor(Actor akActor, ColorForm akColor) global native
	
	;Sets the flag used by the game to determine soul trapped NPCs
	Function SetSoulTrapped(Actor akActor, bool abTrapped) global native
	
	;Toggles any hair wigs (geometry with hair shader) found on slots Hair/LongHair
	Function ToggleHairWigs(Actor akActor, bool abDisable) global native
	
	;/	ARMOR TYPE
		Light = 0
		Heavy = 1
		Clothing = 2
	/;
	
	;Unequips all armor of type, optionally skipping biped slots.
	Function UnequipAllOfType(Actor akActor, int afArmorType, int[] aiSlotsToSkip) global native
			
;----------------------------------------------------------------------------------------------------------
;ACTORBASE
;----------------------------------------------------------------------------------------------------------

	;Get actorbase perk at nth index
	Perk Function GetNthPerk(Actorbase akBase, int aiIndex) global native
	
	;Get total actorbase perk count
	int Function GetPerkCount(Actorbase akBase) global native
	
;----------------------------------------------------------------------------------------------------------
;ARMOR/ADDONS
;----------------------------------------------------------------------------------------------------------
	
	;Equips armor if actor has skin/armor with skin present
	Function EquipArmorIfSkinVisible(Actor akActor, Armor akArmorToCheck, Armor akArmorToEquip) global native
	
	;Gets armor addon's footstep set 
	FootstepSet Function GetFootstepSet(ArmorAddon akArma) global native
	
	;Sets armor addon's footstep set 
	Function SetFootstepSet(ArmorAddon akArma, FootstepSet akFootstepSet) global native
	
;----------------------------------------------------------------------------------------------------------	
;ARRAYS
;----------------------------------------------------------------------------------------------------------
	
	;Adds actor to array. Modifies array directly, it must be initialized!
	bool Function AddActorToArray(Actor akActor, Actor[] actorArray) global native

	;Adds string to array. Modifies array directly, it must be initialized!
	bool Function AddStringToArray(String asString, String[] asStrings) global native

	;Counts how many instances of a string are in an array.
	int Function ArrayStringCount(String asString, String[] asStrings) global native

	;Alphabetically sorts and returns truncated sring array.
	String[] Function SortArrayString(String[] asStrings) global native	
	
	;Gets sorted string array of all the actors in the area, sorted alphabetically. Generic actors are merged (ie. 3 Whiterun Guard(s)). Filter keyword optional
	String[] Function GetSortedActorNameArray(Keyword akKeyword, String asPlural = "(s)", bool abInvertKeyword) global native	
	
;----------------------------------------------------------------------------------------------------------	
;CELL
;----------------------------------------------------------------------------------------------------------
	
	;Gets cell north rotation/worldspace north rotation for exterior cells. Rotation is in degrees.
	float Function GetCellNorthRotation(Cell akCell) global native
	
	;Gets cell lighting template
	LightingTemplate Function GetLightingTemplate(Cell akCell) global native
	
	;Sets cell lighting template
	Function SetLightingTemplate(Cell akCell, LightingTemplate akLightingTemplate) global native
	
	;Sets cell north rotation.
	Function SetCellNorthRotation(Cell akCell, float afAngle) global native
	
;----------------------------------------------------------------------------------------------------------	
;DEBUG
;----------------------------------------------------------------------------------------------------------
	
	;Adds all functional spells (ie. spells that can be learned from spell books, and not all 2000+ spells like psb)
	Function GivePlayerSpellBook() global native
	
;----------------------------------------------------------------------------------------------------------	
;EFFECTSHADER
;----------------------------------------------------------------------------------------------------------

	;-------
	;GETTERS
	;-------
	
	int property kEffectShader_NoMembraneShader = 0x00000001 AutoReadOnly ; 0
	int property kEffectShader_MembraneGreyscaleColor = 0x00000002 AutoReadOnly ; 1
	int property kEffectShader_MembraneGreyscaleAlpha = 0x00000004 AutoReadOnly ; 2
	int property kEffectShader_NoParticleShader = 0x00000008 AutoReadOnly ; 3
	int property kEffectShader_EdgeEffectInverse = 0x00000010 AutoReadOnly ; 4
	int property kEffectShader_AffectSkinOnly = 0x00000020 AutoReadOnly ; 5
	int property kEffectShader_IgnoreAlpha = 0x00000040 AutoReadOnly ; 6
	int property kEffectShader_ProjectUV = 0x00000080 AutoReadOnly ; 7
	int property kEffectShader_IgnoreBaseGeometryAlpha = 0x00000100 AutoReadOnly ; 8
	int property kEffectShader_Lighting = 0x00000200 AutoReadOnly ; 9
	int property kEffectShader_NoWeapons = 0x00000400 AutoReadOnly ; 10
	int property kEffectShader_ParticleAnimated = 0x00008000 AutoReadOnly ; 15
	int property kEffectShader_ParticleGreyscaleColor = 0x00010000 AutoReadOnly ; 16
	int property kEffectShader_ParticleGreyscaleAlpha = 0x00020000 AutoReadOnly ; 17
	int property kEffectShader_UseBloodGeometry = 0x01000000 AutoReadOnly ; 24
	
	;Gets addon models
	Debris Function GetAddonModels(EffectShader akEffectShader) global native
	
	;Returns the total number of effect shaders present/present and active (on objects) within the loaded area. 
	int Function GetEffectShaderTotalCount(EffectShader akEffectShader, bool abActive) global native
	
	;Is effect shader flag set?
	bool Function IsEffectShaderFlagSet(EffectShader akEffectShader, int aiFlag) global native
	
	;Get fill texture
	String Function GetMembraneFillTexture(EffectShader akEffectShader) global native
	
	;Get holes texture
	String Function GetMembraneHolesTexture(EffectShader akEffectShader) global native
	
	;Get membrane palette texture
	String Function GetMembranePaletteTexture(EffectShader akEffectShader) global native
	
	;Gets full particle count.
	float Function GetParticleFullCount(EffectShader akEffectShader) global native
	
	;Get particle palette texture
	String Function GetParticlePaletteTexture(EffectShader akEffectShader) global native
	
	;Get particle shader texture
	String Function GetParticleShaderTexture(EffectShader akEffectShader) global native
	
	;Gets persistent count.
	float Function GetParticlePersistentCount(EffectShader akEffectShader) global native
	
	;-------
	;SETTERS
	;-------
	
	;Clears effect shader flag.
	Function ClearEffectShaderFlag(EffectShader akEffectShader, int aiFlag) global native
	
	;Gets addon models
	Function SetAddonModels(EffectShader akEffectShader, Debris akDebris) global native
	
	;Set effect shader flag.
	Function SetEffectShaderFlag(EffectShader akEffectShader, int aiFlag) global native
	
	;Set membrane color key
	Function SetMembraneColorKeyData(EffectShader akEffectShader, int aiColorKey, int[] aiRGB, float afAlpha, float afTime) global native
	
	;Set membrane fill texture
	Function SetMembraneFillTexture(EffectShader akEffectShader, String asTextureName) global native
	
	;Set membrane holes texture
	Function SetMembraneHolesTexture(EffectShader akEffectShader, String asTextureName) global native
	
	;Set membrane palette texture
	Function SetMembranePaletteTexture(EffectShader akEffectShader, String asTextureName) global native
	
	;Set particle color key
	Function SetParticleColorKeyData(EffectShader akEffectShader, int aiColorKey, int[] aiRGB, float afAlpha, float afTime) global native
	
	;Sets full particle count.
	Function SetParticleFullCount(EffectShader akEffectShader, float afParticleCount) global native
	
	;Set particle shader texture
	Function SetParticlePaletteTexture(EffectShader akEffectShader, String asTextureName) global native
	
	;Sets persistent particle count.
	Function SetParticlePersistentCount(EffectShader akEffectShader, float afParticleCount) global native
	
	;Set particle shader texture
	Function SetParticleShaderTexture(EffectShader akEffectShader, String asTextureName) global native
	
;----------------------------------------------------------------------------------------------------------	
;FEC
;----------------------------------------------------------------------------------------------------------
	
	;FEC function
	;returns effect type, effect skill level, and projectile type, of the highest magnitude effect present on the actor
	;permanent - SUN, ACID, FIRE, FROST, SHOCK, DRAIN
	;temporary - POISON, FEAR
	int[] Function GetDeathEffectType(Actor akActor, int type) global native
		
;----------------------------------------------------------------------------------------------------------	
;FORM
;----------------------------------------------------------------------------------------------------------

	;-------
	;GETTERS
	;-------
	
	;evakluates condition lists for spells/potions/enchantments/mgefs and returns if they can be fullfilled
	bool Function EvaluateConditionList(Form akForm, ObjectReference akActionRef, ObjectReference akTargetRef) global native
	
	;Builds a list of conditions present on the form. Index is for spells/other forms that have lists with conditions
	;Some conditions may be skipped (conditions that require non player references, overly complex conditions involving packages/aliases)
	String[] Function GetConditionList(Form akForm, int aiIndex = 0) global native
	
	;Returns whether the form is temporary (ie. has a formID beginning with FF).
	bool Function IsGeneratedForm(Form akForm) global native	
	
	;-------
	;SETTERS
	;-------
	
	;Adds keyword to form. Fails if the form doesn't accept keywords.
	Function AddKeywordToForm(Form akForm, Keyword akKeyword) global native
	
	;Replaces given keyword with new one on form. Only lasts for a single gaming session. [ported from DienesTools].
	Function ReplaceKeywordOnForm(Form akForm, Keyword akKeywordAdd, Keyword akKeywordRemove) global native
	
	;Removes keyword, if present, from form.
	bool Function RemoveKeywordOnForm(Form akForm, Keyword akKeyword) global native

;----------------------------------------------------------------------------------------------------------	
;FURNITURE
;----------------------------------------------------------------------------------------------------------
		
	;/	FURNITURE TYPES	
		Perch = 0
		Lean = 1
		Sit = 2
		Sleep = 3
	/;
	
	;Gets furniture type
	int Function GetFurnitureType(Furniture akFurniture) global native
	
;----------------------------------------------------------------------------------------------------------	
;GAME
;----------------------------------------------------------------------------------------------------------
		
	;Gets all enchantments from base game + mods, filtered using optional keyword array
	Enchantment[] Function GetAllEnchantments(Keyword[] akKeywords = None) global native
	
	;Gets all races from base game + mods, filtered using optional keyword array
	Race[] Function GetAllRaces(Keyword[] akKeywords = None) global native
	
	;Gets all spells from base game + mods, filtered using optional keyword array. IsPlayable filters out spells that are not found in spellbooks. 
	Spell[] Function GetAllSpells(Keyword[] akKeywords = None, bool abIsPlayable = false) global native
				
	
	;/	AI PROCESS LEVEL
		HighProcess = 0
		MiddleHighProcess = 1
		MiddleLowProcess = 2
		LowProcess = 3
	/;
		
	;Gets all actors by AI processing type. https://geck.bethsoft.com/index.php?title=GetActorsByProcessingLevel for more info	
	Actor[] Function GetActorsByProcessingLevel(int aiLevel) global native
	
	;Gets all enchantments added by a specified mod/game esm, filtered using optional keyword array. 
	Enchantment[] Function GetAllEnchantmentsInMod(String asModName, Keyword[] akKeywords = None) global native
	
	;Gets all races added by a specified mod/game esm, filtered using optional keyword array. 
	Race[] Function GetAllRacesInMod(String asModName, Keyword[] akKeywords = None) global native
	
	;Gets all spells added by a specified mod/game esm, filtered using optional keyword array. 
	Spell[] Function GetAllSpellsInMod(String asModName, Keyword[] akKeywords = None, bool abIsPlayable = false) global native
	
	;Gets current cell if in interior/attached cells in exterior/sky cells if in worldspace with no attached cells??
	Cell[] Function GetAttachedCells() global native
	
	;Gets the value of the boolean gamesetting. Returns -1 if gmst is None or not a bool.
	Int Function GetGameSettingBool(String asGameSetting) global native
	
	;Gets local gravity of the exterior worldspace/interior cell. Default gravity is [0.0, 0.0, -9.81]
	Float[] Function GetLocalGravity() global native
	
	;Gets how many actors are in high process
	int Function GetNumActorsInHigh() global native
	
	;Returns whether plugin exists
	bool Function IsPluginFound(String akName) global native
	
	;Returns whether CC Survival Mode is enabled
	bool Function IsSurvivalModeActive() global native
	
	;Sets local gravity (ms-2) of the exterior worldspace/interior cell. 
	Function SetLocalGravity(float afXAxis, float afYAxis, float afZAxis) global native
	
;----------------------------------------------------------------------------------------------------------	
;HAZARD
;----------------------------------------------------------------------------------------------------------

	;-------
	;GETTERS
	;-------
	
	int property kHazard_None = 0 AutoReadOnly ; 0
	int property kHazard_PCOnly = 0x00000001 AutoReadOnly ; 1
	int property kHazard_InheritDuration = 0x00000002 AutoReadOnly ; 2
	int property kHazard_AlignToNormal = 0x00000004 AutoReadOnly ; 3
	int property kHazard_InheritRadius = 0x00000008 AutoReadOnly ; 4
	int property kHazard_DropToGround = 0x00000010 AutoReadOnly ; 5
	
	;Gets hazard art path, eg. "Effects/MyHazardArt.nif"
	String Function GetHazardArt(Hazard akHazard) global native
	
	;Gets associated IMOD
	ImageSpaceModifier Function GetHazardIMOD(Hazard akHazard) global native

	;Gets IMOD radius
	float Function GetHazardIMODRadius(Hazard akHazard) global native
	
	;Gets impact data set 
	ImpactDataSet Function GetHazardIPDS(Hazard akHazard) global native
	
	;Gets hazard lifetime
	float Function GetHazardLifetime(Hazard akHazard) global native
	
	;Gets hazard light
	Light Function GetHazardLight(Hazard akHazard) global native

	;Gets hazard limit
	int Function GetHazardLimit(Hazard akHazard) global native
	
	;Gets hazard radius
	float Function GetHazardRadius(Hazard akHazard) global native

	;Gets hazard sound
	SoundDescriptor Function GetHazardSound(Hazard akHazard) global native
	
	;Gets hazard spell
	Spell Function GetHazardSpell(Hazard akHazard) global native

	;Gets target interval (duration between casts)
	float Function GetHazardTargetInterval(Hazard akHazard) global native

	;Is hazard flag set?
	bool Function IsHazardFlagSet(Hazard akHazard, int aiFlag) global native

	;-------
	;SETTERS
	;-------
	
	;Clears hazard flag
	Function ClearHazardFlag(Hazard akHazard, int aiFlag) global native
	
	;Sets hazard art path. Does not work on active hazards
	Function SetHazardArt(Hazard akHazard, String asPath) global native

	;Set flag
	Function SetHazardFlag(Hazard akHazard, int aiFlag) global native

	;Sets IMOD
	Function SetHazardIMOD(Hazard akHazard, ImageSpaceModifier akIMOD) global native

	;Sets IMOD radius
	Function SetHazardIMODRadius(Hazard akHazard, float afRadius) global native
	
	;Sets impact data set
	Function SetHazardIPDS(Hazard akHazard, ImpactDataSet akIPDS) global native
	
	;Sets hazard lifetime
	Function SetHazardLifetime(Hazard akHazard, float afLifetime) global native
	
	;Sets hazard light
	Function SetHazardLight(Hazard akHazard, Light akLight) global native

	;Sets hazard limit
	Function SetHazardLimit(Hazard akHazard, int aiLimit) global native
	
	;Sets hazard radius
	Function SetHazardRadius(Hazard akHazard, float afRadius) global native

	;Sets hazard sound
	Function SetHazardSound(Hazard akHazard, SoundDescriptor akSound) global native

	;Sets hazard spell
	Function SetHazardSpell(Hazard akHazard, Spell akspell) global native

	;Sets hazard interval
	Function SetHazardTargetInterval(Hazard akHazard, float afInterval) global native
		
;----------------------------------------------------------------------------------------------------------
;LIGHT
;----------------------------------------------------------------------------------------------------------

	;-------
	;GETTERS
	;-------

	;Gets light color.
	ColorForm Function GetLightColor(Light akLight) global native
		
	;Gets light fade range.
	float Function GetLightFade(Light akLight) global native
	
	;Gets base light FOV.
	float Function GetLightFOV(Light akLight) global native
	
	;Gets light radius (radius is actually int but changing that would break mods so fixed that in source).
	float Function GetLightRadius(Light akLight) global native
	
	;Gets light color as RGB array [0-255].
	int[] Function GetLightRGB(Light akLight) global native
	
	;Gets depth bias, returns 1 if not set.
	float Function GetLightShadowDepthBias(ObjectReference akLightObject) global native
	
	;/	LIGHT TYPES
		HemiShadow = 1,
		Omni = 2;
		OmniShadow = 3,
		Spot = 4,
		SpotShadow = 5,
	/;
	
	;Get light type
	int Function GetLightType(Light akLight) global native
	
	;-------
	;SETTERS
	;-------
	
	;Sets light color.
	Function SetLightColor(Light akLight, ColorForm akColorform) global native
		
	;Sets light fade range.
	Function SetLightFade(Light akLight, float afRange) global native
		
	;sets base light FOV.
	Function SetLightFOV(Light akLight, float afFOV) global native
	
	;Sets light radius (minimum light radius is 16) .
	Function SetLightRadius(Light akLight, float afRadius) global native
	
	;Sets light color using RGB array [0-255]. Array must contain 3 elements (r,g,b).
	Function SetLightRGB(Light akLight, int[] aiRGB) global native
	
	;sets depth bias on light reference. Creates new depth bias extradata on lights that don't have it set.
	Function SetLightShadowDepthBias(ObjectReference akLightObject, float afDepthBias) global native
	
	;Sets light type. Does not persist between sessions.
	Function SetLightType(Light akLight, int aiLightType) global native
	
;----------------------------------------------------------------------------------------------------------
;LOCATION
;----------------------------------------------------------------------------------------------------------

	;-------
	;GETTERS
	;-------

	;Get parent location.
	Location Function GetParentLocation(Location akLoc) global native
	
	;-------
	;SETTERS
	;-------
	
	;Set parent location.
	Function SetParentLocation(Location akLoc, Location akNewLoc) global native
	
;----------------------------------------------------------------------------------------------------------		
;MAGIC EFFECTS
;----------------------------------------------------------------------------------------------------------		

	;---------
	;GETTERS
	;---------
	
	;Gets associated form (Light for Light spells, Actor for Summon Creature...), if any
	Form Function GetAssociatedForm(MagicEffect akMagicEffect) global native
	
	;/	EFFECT ARCHETYPES
		ValueMod = 0
		Script = 1
		Dispel = 2
		CureDisease = 3
		Absorb = 4
		DualValueMod = 5
		Calm = 6
		Demoralize = 7
		Frenzy = 8
		Disarm = 9
		CommandSummoned = 10
		Invisibility = 11
		Light = 12
		Darkness = 13
		NightEye = 14
		Lock = 15
		Open = 16
		BoundWeapon = 17
		SummonCreature = 18
		DetectLife = 19
		Telekinesis = 20
		Paralysis = 21
		Reanimate = 22
		SoulTrap = 23
		TurnUndead = 24
		Guide = 25
		WerewolfFeed = 26
		CureParalysis = 27
		CureAddiction = 28
		CurePoison = 29
		Concussion = 30
		ValueAndParts = 31
		AccumulateMagnitude = 32
		Stagger = 33
		PeakValueMod = 34
		Cloak = 35
		Werewolf = 36
		SlowTime = 37
		Rally = 38
		EnhanceWeapon = 39
		SpawnHazard = 40
		Etherealize = 41
		Banish = 42
		SpawnScriptedRef = 43
		Disguise = 44
		GrabActor = 45
		VampireLord = 46
	/;
	
	;Gets effect archetype of magiceffect and returns as int (0-46).
	int Function GetEffectArchetypeAsInt(MagicEffect akMagicEffect) global native
	
	;Gets effect archetype of magiceffect and returns as String.
	String Function GetEffectArchetypeAsString(MagicEffect akMagicEffect) global native
	
	;Gets primary actor value as string, if any (FrostResist, SpeedMult).
	String Function GetPrimaryActorValue(MagicEffect akMagicEffect) global native
	
	;Gets secondary actor value as string, if any.
	String Function GetSecondaryActorValue(MagicEffect akMagicEffect) global native
		
	;/	MGEF SOUND TYPES
		Draw/Sheathe = 0
		Charge = 1
		Ready = 2
		Release = 3
		Concentration Cast Loop = 4
		On Hit = 5
	/;
	
	;Gets Sound attached to index of Sound type specified in magic effect.
	SoundDescriptor Function GetMagicEffectSound(MagicEffect akMagicEffect, int aiType) global native
	
	;-------
	;SETTERS
	;-------
	
	;Sets sound descriptor attached to index of Sound type specified in magic effect.
	Function SetMagicEffectSound(MagicEffect akMagicEffect, SoundDescriptor akSoundDescriptor, int aiType) global native
	
;----------------------------------------------------------------------------------------------------------
;OBJECTREFERENCES
;----------------------------------------------------------------------------------------------------------

	;--------
	;GETTERS
	;--------
		
	;Finds all references of form type in loaded cells, from ref.
	ObjectReference[] Function FindAllReferencesOfFormType(ObjectReference akRef, int formType, float afRadius) global native
	
	;Find all references with keyword in loaded cells, from ref.
	ObjectReference[] Function FindAllReferencesWithKeyword(ObjectReference akRef, Form keywordOrList, float afRadius, bool abMatchAll) global native
	
	;Find all references matching base form/in formlist, from ref.
	ObjectReference[] Function FindAllReferencesOfType(ObjectReference akRef, Form akFormOrList, float afRadius) global native
	
	;Gets the first item in inventory that exists in formlist.
	Form Function FindFirstItemInList(ObjectReference akRef, FormList akList) global native
	
	;Gets activate children - see IsActivateChild
	ObjectReference[] Function GetActivateChildren(ObjectReference akRef) global native
	
	;Gets actor responsible for object.
	Actor Function GetActorCause(ObjectReference akRef) global native
	
	;Get all art objects attached to this object.
	Art[] Function GetAllArtObjects(ObjectReference akRef) global native
	
	;Get all effect shaders attached to this object.
	EffectShader[] Function GetAllEffectShaders(ObjectReference akRef) global native
		
	;Gets closest actor to ref (without returning the reference itself).
	Actor Function GetClosestActorFromRef(ObjectReference akRef, bool abIgnorePlayer) global native
	
	;Gets duration of the effectshader on the ref.
	float Function GetEffectShaderDuration(ObjectReference akRef, EffectShader akShader) global native
	
	;Gets the door which is linked to this load door.
	ObjectReference Function GetDoorDestination(ObjectReference akRef) global native
	
	;Gets all refs linked to akRef. Keyword optional.
	ObjectReference[] Function GetLinkedChildren(ObjectReference akRef, Keyword akKeyword) global native
	
	;Gets the source of the magic effect (spell/enchantment/scroll etc) and the caster. Magic effect must be present on the reference.
	Form[] Function GetMagicEffectSource(ObjectReference akRef, MagicEffect akEffect) global native
		
	;/	MATERIAL TYPES - String
		StoneBroken
		BlockBlade1Hand
		Meat
		CarriageWheel
		MetalLight
		WoodLight
		Snow
		Gravel
		ChainMetal
		Bottle
		Wood
		Ash
		Skin
		BlockBlunt
		DLC1DeerSkin
		Insect
		Barrel
		CeramicMedium
		Basket
		Ice
		GlassStairs
		StoneStairs
		Water
		DraugrSkeleton
		Blade1Hand
		Book
		Carpet
		MetalSolid
		Axe1Hand
		BlockBlade2Hand
		OrganicLarge
		Amulet
		WoodStairs
		Mud
		BoulderSmall
		SnowStairs
		StoneHeavy
		DragonSkeleton
		Trap
		BowsStaves
		Alduin
		BlockBowsStaves
		WoodAsStairs
		SteelGreatSword
		Grass
		BoulderLarge
		StoneAsStairs
		Blade2Hand
		BottleSmall
		BoneActor
		Sand
		MetalHeavy
		DLC1SabreCatPelt
		IceForm
		Dragon
		Blade1HandSmall
		SkinSmall
		PotsPans
		SkinSkeleton
		Blunt1Hand
		StoneStairsBroken
		SkinLarge
		Organic
		Bone
		WoodHeavy
		Chain
		Dirt
		Ghost
		SkinMetalLarge
		BlockAxe
		ArmorLight
		ShieldLight
		Coin
		BlockBlunt2Hand
		ShieldHeavy
		ArmorHeavy
		Arrow
		Glass
		Stone
		WaterPuddle
		Cloth
		SkinMetalSmall
		Ward
		Web
		TrailerSteelSword
		Blunt2Hand
		DLC1SwingingBridge
		BoulderMedium
	/;
	
	;Gets the specified collision shape's havok material types as string array. Returns the first material type if nodeName is empty
	String[] Function GetMaterialType(ObjectReference akRef, String asNodeName = "") global native
	
	;Gets the motion type of the object (see vanilla SetMotionType for types). Returns -1 if 3d is not loaded
	int Function GetMotionType(ObjectReference akRef) global native
	
	;Gets random actor near ref (without returning the reference itself).
	Actor Function GetRandomActorFromRef(ObjectReference akRef, float afRadius, bool abIgnorePlayer) global native
		
	;Returns the size of the stored soul in a soulgem objectreference
	int Function GetStoredSoulSize(ObjectReference akRef) global native
	
	;Returns the number of instances of the specified art object (attached using visual effects) on the reference.
	int Function HasArtObject(ObjectReference akRef, Art akArtObject, bool abActive = false) global native
	
	;Returns the number of instances of the specified effect shader on the reference.
	int Function HasEffectShader(ObjectReference akRef, EffectShader akShader, bool abActive = false) global native
	
	;Returns whether the reference has niextradata (attached to root 3D node). Partial matches accepted.
	bool Function HasNiExtraData(ObjectReference akRef, String asName) global native
	
	;Is door a load door?
	bool Function IsLoadDoor(ObjectReference akRef) global native
	
	;Is a quest object?
	bool Function IsQuestItem(ObjectReference akRef) global native
	
	;Is a VIP (object that is needed by quest)?
	bool Function IsVIP(ObjectReference akRef) global native
	
	;-------
	;SETTERS
	;-------
	
	;Adds all inventory items to array, filtering out equipped, favourited and quest items. 
	Form[] Function AddAllItemsToArray(ObjectReference akRef, bool abNoEquipped = true, bool abNoFavorited = false, bool abNoQuestItem = false) global native
	
	;Adds all inventory items to formlist, filtering out equipped, favourited and quest items. 
	Function AddAllItemsToList(ObjectReference akRef, Formlist akList, bool abNoEquipped = true, bool abNoFavorited = false, bool abNoQuestItem = false) global native
	
	;Wrapper function for AddKeywordToForm.
	Function AddKeywordToRef(ObjectReference akRef, Keyword akKeyword) global native	
	
	;Snaps the object to the nearest navmesh point closest to its current position in the cell.
	Function MoveToNearestNavmeshLocation(ObjectReference akRef) global native
				
	;Wrapper function for RemoveKeywordFromForm.
	bool Function RemoveKeywordFromRef(ObjectReference akRef, Keyword akKeyword) global native
	
	;Wrapper function for ReplaceKeywordOnForm.
	Function ReplaceKeywordOnRef(ObjectReference akRef, Keyword akKeywordAdd, Keyword akKeywordRemove) global native
	
	;Scales node & collision (bhkBoxShape, bhkSphereShape). Entire nif will be scaled if string is empty. Collision has to be directly attached to named nodes.
	;Adds "PO3_SCALE" niextradata to root node.
	Function ScaleObject3D(ObjectReference akRef, String asNodeName, float afScale) global native
	
	;Sets the door as the new linked door
	bool Function SetDoorDestination(ObjectReference akRef, ObjectReference akDoor) global native
	
	;Sets effectshader duration. Internal duration is set when the effectshader begins and does not change with time.
	Function SetEffectShaderDuration(ObjectReference akRef, EffectShader akShader, float afTime, bool abAbsolute) global native
		
	;Sets linked ref. Pass None into akTargetRef to unset the linked ref.
	Function SetLinkedRef(ObjectReference akRef, ObjectReference akTargetRef, Keyword akKeyword = None) global native
	
	;Sets havok material type. Use oldMaterial string to select what material you want to change from to (eg. from stone to wood), and nodeName to apply it to the specific node. 
	;If both are empty, every collision material will be set.
	Function SetMaterialType(ObjectReference akRef, String asNewMaterial, String asOldMaterial = "", String asNodeName = "") global native
	
	;Copies skin tint color from actorbase to bodyparts nif
	Function SetupBodyPartGeometry(ObjectReference akRef, actor akActor) global native
	
	;SHADER TYPES
	int property kDefault = 0 AutoReadOnly ;
	int property kEnvironmentMap = 1 AutoReadOnly ;
	int property kGlowMap = 2 AutoReadOnly ;
	int property kParallax = 3 AutoReadOnly ;
	int property kFaceGen = 4 AutoReadOnly ;
	int property kFaceGenRGBTint = 5 AutoReadOnly ;
	int property kHairTint = 6 AutoReadOnly ;
	int property kParallaxOcc = 7 AutoReadOnly ;
	int property kMultiTexLand = 8 AutoReadOnly ;
	int property kLODLand = 9 AutoReadOnly ;
	int property kMultilayerParallax = 11 AutoReadOnly ;
	int property kTreeAnim = 12 AutoReadOnly ;
	int property kMultiIndexTriShapeSnow = 14 AutoReadOnly ;
	int property kLODObjectsHD = 15 AutoReadOnly ;
	int property kEye = 16 AutoReadOnly ;
	int property kCloud = 17 AutoReadOnly ;
	int property kLODLandNoise = 18 AutoReadOnly ;
	int property kMultiTexLandLODBlend = 19 AutoReadOnly ;
	
	;sets the ref's shader material type ie. default to cubemap
	;template needs to be loaded
	;if texture type is -1, the reference's entire textureset is replaced using the template's textureset;
	;if texture type is 0-9 the template's textureset is still applied but reference's texture at that index will take priority.
	;optional diffuse path can be used to filter shapes to apply the shader to, partial matches are accepted like "Draugr.dds"
	;limitations - cannot be used on geometry with no normals (ie. body skin meshes)	
	Function SetShaderType(ObjectReference akRef, ObjectReference akTemplate, String asDiffusePath, int aiShaderType, int aiTextureType, bool abNoWeapons, bool abNoAlphaProperty) global native
								
	;Stops ALL effect shaders and art objects (visual effects) currently on this actor
	Function StopAllShaders(ObjectReference akRef) global native
	
	;Removes all instances of the art object (hit magic effect/visual effect) attached to the reference.
	Function StopArtObject(ObjectReference akRef, Art akArt) global native
	
	;Toggles node visibility.
	Function ToggleChildNode(ObjectReference akRef, String asNodeName, bool abDisable) global native
	
	;Moves hit effect art to new node (ie. from "MagicEffectsNode" to "NPC Head [Head]"), optionally updating translate, rotate, and scale values.
	;Translate and Rotate arrays must have three values in order to work. Rotate uses euler angles in degrees (XYZ). Scale is relative, and is multiplied by existing scale.
	;If the hit effect art is removed and reattached, it will revert back to the values in the nif.
	Function UpdateHitEffectArtNode(ObjectReference akRef, Art akArt, String asNewNode, float[] afTranslate, float[] afRotate, float afRelativeScale = 1.0) global native
							
;----------------------------------------------------------------------------------------------------------
;PACKAGES
;----------------------------------------------------------------------------------------------------------

	;GETTERS
	
	;/	PACKAGE TYPES
		Find = 0
		Follow = 1
		Escort = 2
		Eat = 3
		Sleep = 4
		Wander = 5
		Travel = 6
		Accompany = 7
		UseItemAt = 8
		Ambush = 9
		FleeNotCombat = 10
		CastMagic = 11
		Sandbox = 12
		Patrol = 13
		Guard = 14
		Dialogue = 15
		UseWeapon = 16
		Find2 = 17
		Package = 18
		PackageTemplate = 19
		Activate = 20
		Alarm = 21
		Flee = 22
		Trespass = 23
		Spectator = 24
		ReactToDead = 25
		GetUpFromChair = 26
		DoNothing = 27
		InGameDialogue = 28
		Surface =  29
		SearchForAttacker = 30
		AvoidPlayer = 31
		ReactToDestroyedObject = 32
		ReactToGrenadeOrMine = 33
		StealWarning = 34
		PickPocketWarning = 35
		MovementBlocked = 36
		VampireFeed = 37
		CannibalFeed = 38
		Unknown39 = 39
		Unknown40 = 40
		Unknown41 = 41
	/;
	
	;Gets package type. Returns -1 if package is none
	int Function GetPackageType(Package akPackage) global native
		
;----------------------------------------------------------------------------------------------------------
;PROJECTILES
;----------------------------------------------------------------------------------------------------------

	;--------
	;GETTERS
	;--------
	
	;Gets projectile gravity (usually 0.0 for non arrow projectiles).
	float Function GetProjectileGravity(Projectile akProjectile) global native
	
	;Gets projectile impact force. 
	float Function GetProjectileImpactForce(Projectile akProjectile) global native
	
	;Gets projectile range.
	float Function GetProjectileRange(Projectile akProjectile) global native
	
	;Gets projectile speed.
	float Function GetProjectileSpeed(Projectile akProjectile) global native
	
	
	;/	PROJECTILE TYPES
		Missile = 1
		Lobber = 2
		Beam = 3
		Flame = 4
		Cone = 5
		Barrier = 6
		Arrow = 7
	/;
	
	;Get projectile type. 0 if projectile is None.
	int Function GetProjectileType(Projectile akProjectile) global native
	
	;-------
	;SETTERS
	;-------
	
	;Sets projectile gravity. 
	Function SetProjectileGravity(Projectile akProjectile, float afGravity) global native
	
	;Sets projectile impact force. 
	Function SetProjectileImpactForce(Projectile akProjectile, float afImpactForce) global native
	
	;Sets projectile range.
	Function SetProjectileRange(Projectile akProjectile, float afRange) global native
	
	;Sets projectile speed. 
	Function SetProjectileSpeed(Projectile akProjectile, float afSpeed) global native

	
;-----------------------------------------------------------------------------------------------------------	
;SOUND
;-----------------------------------------------------------------------------------------------------------	

	;Sets sound descriptor attached to the sound.	
	Function SetSoundDescriptor(Sound akSound, SoundDescriptor akSoundDescriptor) global native
	
;-----------------------------------------------------------------------------------------------------------	
;SPELL
;-----------------------------------------------------------------------------------------------------------	
	;--------
	;GETTERS
	;--------
	
	;/	SPELL TYPES
		Spell = 0
		Disease = 1
		Power = 2
		LesserPower = 3
		Ability = 4
		Poison = 5
		Addition = 6
		Voice = 7
	/;
	
	;Returns spell type. -1 if spell is None
	int Function GetSpellType(Spell akSpell) global native
		
	;--------
	;SETTERS
	;--------
	
	;ConditionItemObject | Function ID | parameter 1 | parameter 2 | OPCode | float | ANDOR

	;conditions which have no parameters (eg. IsSneaking) / take in forms (GetIsRace) work
	;conditions which accept int/float/strings are skipped
	
	;Subject	| HasMagicEffectKeyword	| MagicInvisibility		| NONE | == | 0.0 | AND - in game
	;Subject 	| HasMagicEffectKeyword	| 0001EA6F ~ Skyrim.esm | NONE | == | 0.0 | AND	- in papyrus	
	
	Function AddMagicEffectToSpell(Spell akSpell, MagicEffect akMagicEffect, float afMagnitude, int aiArea, int aiDuration, float afCost = 0.0, String[] asConditionList) global native	
	
	;Adds effectitem from spell to target spell, at given index. Same as above function, but less verbose, and preserves all conditions.
	Function AddEffectItemToSpell(Spell akSpell, Spell akSpellToCopyFrom, int aiIndex) global native	
	
	;Removes magic effect from spell that matches magnitude/area/duration/cost.
	Function RemoveMagicEffectFromSpell(Spell akSpell, MagicEffect akMagicEffect, float afMagnitude, int aiArea, int aiDuration, float afCost = 0.0) global native
	
	;Removes effectitem from spell that matches spell at index.
	Function RemoveEffectItemFromSpell(Spell akSpell, Spell akSpellToMatchFrom, int aiIndex) global native
			
;----------------------------------------------------------------------------------------------------------	
;STRINGS
;----------------------------------------------------------------------------------------------------------

	;Converts string to hex value if valid
	String Function IntToString(int aiValue, bool abHex) global native
	
	;Converts strng to int. Returns -1 for out of bound values.
	int Function StringToInt(String asString) global native
	
;----------------------------------------------------------------------------------------------------------
;UTILITY
;----------------------------------------------------------------------------------------------------------
	
	;Calculates a random float between afMin and afMax, based on Mersenne Twister
	float Function GenerateRandomFloat(float afMin, float afMax) global native
	
	;Calculates a random integer between afMin and afMax, based on Mersenne Twister
	int Function GenerateRandomInt(int afMin, int afMax) global native
	
;-----------------------------------------------------------------------------------------------------------
;VISUALEFFECTS
;----------------------------------------------------------------------------------------------------------		

	;--------
	;GETTERS
	;--------
	
	;Gets the art object associated with the visual effect.
	Art Function GetArtObject(VisualEffect akEffect) global native
	
	;Returns the total number of art objects present/active (on objects) within the loaded area.
	int Function GetArtObjectTotalCount(VisualEffect akEffect, bool abActive) global native
	
	;--------
	;SETTERS
	;--------
	
	;Sets the art object associated with the visual effect.
	Function SetArtObject(VisualEffect akEffect, Art akArt) global native
			
;-----------------------------------------------------------------------------------------------------------
;WEATHER
;----------------------------------------------------------------------------------------------------------		

	;Gets wind speed as shown as in CK conditions (0.0-1.0).
	float Function GetWindSpeedAsFloat(Weather akWeather) global native
	
	;Gets wind speed as shown in the weather form (0-255).
	int Function GetWindSpeedAsInt(Weather akWeather) global native
	
	;/	WEATHER TYPES
		Pleasant = 0
		Cloudy = 1
		Rainy = 2
		Snow = 3
	/;
	
	;Gets weather/current weather type if akWeather is None
	int Function GetWeatherType(Weather akWeather = None) global native 