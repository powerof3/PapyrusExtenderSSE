Scriptname PO3_SKSEFunctions Hidden 

;----------------------------------------------------------------------------------------------------------	
;ACTORS 
;----------------------------------------------------------------------------------------------------------

	;-------
	;GETTERS
	;-------
 
	float Function GetActorAlpha(Actor akActor) global native
	
	float Function GetActorRefraction(Actor akActor) global native
	
	;Gets actor state 
	; 0 - "Alive"
	; 1 - "Dying"
	; 2 - "Dead"
	; 3 - "Unconscious"
	; 4 - "Reanimate"
	; 5 - "Recycle"
	; 6 - "Restrained"
	; 7 - "Essential Down"
	; 8 - "Bleedout"
	int Function GetActorState(Actor thisActor) global native;
	
	;Gets all magiceffects currently on the actor. Filters out inactive and hideinui spells.
	MagicEffect[] Function GetAllActiveEffectsOnActor(Actor akActor, bool showInactive = false) global native
	
	;Gets current hair color on actor. 
	;Fails if hair doesn't exist
	ColorForm Function GetHairColor(Actor akActor) global native
	
	;Gets textureset belonging to headpart, if any. 
	TextureSet Function GetHeadPartTextureSet(Actor akActor, int type) global native
	
	;Gets actual current package on actor, including internal packages used by the game atm (see GetPackageType below)
	Package Function GetRunningPackage(Actor akActor) global native
	
	;Gets current skin color on actor. 
	ColorForm Function GetSkinColor(Actor akActor) global native
		
	;Similar to GetTimeDead console command. Returns 0.0 if actor is alive
	float Function GetTimeDead(Actor akActor) global native
	
	;Returns time of death in game days passed
	float Function GetTimeOfDeath(Actor akActor) global native
	
	;returns if spell is present and not inactive/dispelled
	bool Function HasActiveSpell(Actor akActor, Spell akSpell) global native
	
	;Checks if activemagiceffect with given archetype is present on actor. Archetype MUST be typed as given below.
	bool Function HasMagicEffectWithArchetype(Actor akActor, String akArchetype) global native
	
	;Returns whether the actor is in cell water or lava
	bool Function IsActorInWater(Actor akActor) global native
	
	;Returns whether target is capable of being soul trapped successfully. 
	;Has to be called before the target is actually soul trapped
	bool Function IsActorSoulTrapped(Actor akActor) global native
	
	;Returns whether the actor is underwater
	bool Function IsActorUnderwater(Actor akActor) global native
	
	;-------
	;SETTERS
	;-------
	
	;Adds all equipped items to array
	Form[] Function AddAllEquippedItemsToArray(Actor akActor) global native
	
	;Adds perks to the actorbase, works on leveled actors/unique NPCs
	;vanilla AddPerk is called if used on the player.
	;Function serializes data to skse cosave, so perks are applied correctly on loading/reloading saves.
	bool Function AddBasePerk(Actor akActor, Perk akPerk) global native
	
	;Adds spells to actorbase, works on player/leveled actors/unique NPCs
	;Function serializes data to skse cosave, so spells are applied correctly on loading/reloading saves.
	bool Function AddBaseSpell(Actor akActor, Spell akSpell) global native
	
	;BLEND MODES
	;Darken = 0
	;Multiply = 1
	;ColorBurn = 2
	;LinearBurn = 3
	;DarkerColor = 4
	;Lighten = 5
	;Screen = 6
	;ColorDodge = 7
	;LinearDodge = 8
	;LighterColor = 9	
	;Overlay = 10
	;SoftLight = 11
	;HardLight = 12
	;VividLight = 13
	;LinearLight = 14
	;PinLight = 15
	;HardMix = 16;	
	;Difference =  17;
	;Exclusion = 18
	;Subtract = 19
	;Divide = 20
	
	;Blends existing skin color with specified color, using photoshop blend formulas, with alpha (opacity).
	;if true, autoLuminance calculates skin tone relative luminance. The opacity value is then used as a multiplier on top of that, final value is clamped to 0-1
	;if false, only opacity will be used. Recommend to use autoluminance because colors will not blend well for all skin tones using flat values. 
	Function BlendColorWithSkinTone(Actor akActor, ColorForm color, int blendMode, bool autoLuminance, float opacity) global native
	
	;Decapitates living and dead actors. Living actors will not die when this is called!
	;This is what is called internally during beheading scenes
	Function DecapitateActor(Actor akActor) global native	
		
	;returns whether the actor was instantly killed and set dead
	;doesn't work on creatures - AI gets fucked up and they don't die.
	bool Function InstantKill(Actor akActor) global native
	
	;DEPRECIATED
	;Blends existing skin color with specified color. 
	;True - intensity is manually calculated using percentage 0-1.0, False - automatically calculated using skin tone luminance 
	Function MixColorWithSkinTone(Actor akActor, ColorForm color, bool manualMode, float percentage) global native
	
	;Removes perks from the actorbase
	;perk effects may not be removed from unique actors, more testing required.
	;vanilla RemovePerk is called if used on the player.
	;Function serializes data to skse cosave, so perks are applied correctly on loading/reloading saves.
	bool Function RemoveBasePerk(Actor akActor, Perk akPerk) global native
	
	;Removes spells from the actorbase, works on player/leveled actors/unique NPCs
	;Function serializes data to skse cosave, so spells are applied correctly on loading/reloading saves.
	bool Function RemoveBaseSpell(Actor akActor, Spell akSpell) global native
	
	;FEC only Function 
	;0 - charred/skeleton
	;1 - drained
	;2 - poisoned/frightened
	;3-  aged
	;4 - charred creature
	;5 - frozen
	Function RemoveEffectsNotOfType(Actor akActor, int effectType) global native
	
	;Replaces specified source textureset on worn armor with target textureset. Lasts for one single gaming session.
	;If texture type is -1, the entire textureset is replaced, otherwise the texture map specified at [textureType] index is replaced.
	;i.e diffuse is 0, normal is 1
	Function ReplaceArmorTextureSet(Actor akActor, Armor akArmor, TextureSet sourceTXST, TextureSet targetTXST, int textureType = -1) global native
	
	;Replaces face textureset. Lasts one gaming session. Can be applied to non-unique actors.
	;If texture type is -1, the entire textureset is replaced, otherwise the texture map specified at [textureType] index is replaced. Replacing the entire textureset may cause a visible neckseam.
	;i.e diffuse is 0, normal is 1.
	Function ReplaceFaceTextureSet(Actor akActor, TextureSet maleTXST, TextureSet femaleTXST, int textureType = -1) global native
	
	;Replaces skin textureset for given slotmask (ie. body/hand). Lasts one gaming session. Has to be reapplied when re-equipping armor.
	;If texture type is -1, the entire textureset is replaced, otherwise the texture map specified at [textureType] index is replaced.
	;i.e diffuse is 0, normal is 1.
	Function ReplaceSkinTextureSet(Actor akActor, TextureSet maleTXST, TextureSet femaleTXST, int slotMask, int textureType = -1) global native
	
	;Checks for NiExtraData nodes on actor - PO3_TINT/PO3_ALPHA/PO3_TXST/PO3_TOGGLE/PO3_SHADER
	;Stops all effect shaders
	;PO3_TINT - resets tint, rebuilds facegen if actor is player
	;PO3_ALPHA - resets inventory, resets skin alpha
	;PO3_TXST - resets texturesets with texturepaths containing folderName
	;PO3_TOGGLE - unhides all children of nodes that were written to the extraData
	;PO3_SHADER - recreates the original shader type (as close as possible, projectedUV params are not restored)
	bool Function ResetActor3D(Actor akActor, String folderName) global native
	
	;0.0 disables refraction, 1.0 is max refraction
	Function SetActorRefraction(Actor thisActor, float refraction) global native;
	
	;Sets hair color on actor. Changes may persist throughout gaming session, even when reloading previous saves.
	Function SetHairColor(Actor akActor, ColorForm color) global native
	
	;Sets headpart's mesh alpha.
	;Zero alpha fails for some hair types and heterochromic eyes
	;Works for mouths (which is the only headpart that you cannot replace or set using Replace/SetHeadPart())
	Function SetHeadPartAlpha(Actor akActor, int partType, float alpha) global native
	
	;sets textureset belonging to headpart, if any. 
	Function SetHeadPartTextureSet(Actor akActor, TextureSet headpartTXST, int type) global native	
	
	Function SetLinearVelocity(Actor akActor, float x, float y, float z) global native
	
	Function SetLocalGravity(Actor akActor, float value, bool disableGravityOnGround) global native
	
	;sets alpha on face, base skin form and armor meshes with visible skin - resets after reloading. Has to be re-applied when armor is un/re-equipped.
	Function SetSkinAlpha(Actor akActor, float alpha) global native
	
	;Sets skin color (face and body). Changes may persist throughout gaming session, even when reloading previous saves.
	Function SetSkinColor(Actor akActor, ColorForm color) global native
	
	;Toggles node visibility
	Function ToggleChildNode(Actor akActor, String nodeName, bool disable) global native
		
;----------------------------------------------------------------------------------------------------------
;ACTORBASE
;----------------------------------------------------------------------------------------------------------

	Perk Function GetNthPerk(Actorbase akActorbase, int index) global native
	
	int Function GetPerkCount(Actorbase akActorbase) global native
	
;----------------------------------------------------------------------------------------------------------
;ARMOR/ADDONS
;----------------------------------------------------------------------------------------------------------

	;Checks whether an armor has geometry with skin shaders, and then equips another set of armor
	Function EquipArmorIfSkinVisible(Actor akActor, Armor akArmorToCheck, Armor akArmorToEquip) global native
	
;----------------------------------------------------------------------------------------------------------	
;ARRAYS
;----------------------------------------------------------------------------------------------------------

	;Adds actor to array. Modifies array directly, it must be initialized!
	bool Function AddActorToArray(Actor akActor, Actor[] actorArray) global native

	;Adds string to array. Modifies array directly, it must be initialized!
	bool Function AddStringToArray(String akString, String[] stringArray) global native

	;Counts how many instances of a string are in an array.
	int Function ArrayStringCount(String akString, String[] stringArray) global native

	;Alphabetically sorts and returns truncated sring array.
	String[] Function SortArrayString(String[] stringArray) global native	
	
	;gets sorted string array of all the actors in the area, sorted alphabetically. Generic actors are merged (ie. 3 Whiterun Guards)
	;filter keyword optional
	String[] Function GetSortedActorNameArray(Keyword akKeyword, bool invert) global native	
	
;----------------------------------------------------------------------------------------------------------	
;CELL
;----------------------------------------------------------------------------------------------------------
	
;??
	
;----------------------------------------------------------------------------------------------------------	
;DEBUG
;----------------------------------------------------------------------------------------------------------
	
	;Adds all functional spells (ie. spells that can be learned from spell books, and not all 2000+ spells like psb)
	Function GivePlayerSpellBook() global native
	
;----------------------------------------------------------------------------------------------------------	
;EFFECTSHADER
;----------------------------------------------------------------------------------------------------------

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

	;-------
	;GETTERS
	;-------

	;Gets full particle count.
	float Function GetEffectShaderFullParticleCount(EffectShader akEffectShader) global native
	
	;Gets persistent count.
	float Function GetEffectShaderPersistentParticleCount(EffectShader akEffectShader) global native
		
	;Returns the total number of effect shaders present/present and active (on objects) within the loaded area. 
	int Function GetEffectShaderTotalCount(EffectShader akEffectShader, bool active) global native
	
	;Is effect shader flag set?
	bool Function IsEffectShaderFlagSet(EffectShader akEffectShader, int flag) global native
	
	;-------
	;SETTERS
	;-------
	
	;Clears effect shader flag.
	Function ClearEffectShaderFlag(EffectShader akEffectShader, int flag) global native
	
	;Set effect shader flag.
	Function SetEffectShaderFlag(EffectShader akEffectShader, int flag) global native
	
	;Sets full particle count.
	Function SetEffectShaderFullParticleCount(EffectShader akEffectShader, float particleCount) global native
	
	;Sets persistent particle count.
	Function SetEffectShaderPersistentParticleCount(EffectShader akEffectShader, float particleCount) global native
		
;----------------------------------------------------------------------------------------------------------	
;FORM
;----------------------------------------------------------------------------------------------------------

	Function AddKeywordToForm(Form akForm, Keyword KYWDtoAdd) global native
	
	;Replaces given keyword with new one on form. Only lasts for a single gaming session. [ported from DienesTools]
	Function ReplaceKeywordOnForm(Form akForm, Keyword KYWDtoAdd, Keyword KYWDtoRemove) global native
	
	bool Function RemoveKeywordOnForm(Form akForm, Keyword KYWDtoRemove) global native
	
;----------------------------------------------------------------------------------------------------------	
;GAME
;----------------------------------------------------------------------------------------------------------
	
	Race[] Function GetAllRaces(Keyword[] keywords = None) global native
	
	;IsPlayable filters out spells that are not found in spellbooks. 
	Spell[] Function GetAllSpells(Keyword[] keywords = None, bool IsPlayable = false) global native
				
	;highProcess = 0,
	;middleHighProcess = 1;
	;middleLowProcess = 2,
	;lowProcess = 3,
	;see https://geck.bethsoft.com/index.php?title=GetActorsByProcessingLevel for more info	
	;middle-low process is empty/unused?
	;more efficient than GetNumRefs
	Actor[] Function GetActorsByProcessingLevel(int Level) global native
	
	;Checks to see if any of the keywords in the array are found, && returns all races added by a specified mod/game esm. 
	Race[] Function GetAllRacesInMod(String modName, Keyword[] keywords = None) global native
	
	;Checks to see if any of the keywords in the array are found, && returns all spells added by a specified mod/game esm. IsPlayable filters out spells that are not found in spellbooks. 
	Spell[] Function GetAllSpellsInMod(String modName, Keyword[] keywords = None, bool IsPlayable = false) global native
	
	;returns current cell if in interior/attached cells in exterior/sky cells if in worldspace with no attached cells??
	Cell[] Function GetAttachedCells() global native
	
	;Gets how many actors are in high process
	int Function GetNumActorsInHigh() global native
	
	bool Function IsPluginFound(String akName) global native
		
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
	
	;Gets base light FOV
	float Function GetLightFOV(Light akLight) global native
	
	;Gets light radius.
	;actually int but changing that would break mods so fixed that in source
	float Function GetLightRadius(Light akLight) global native
	
	;Gets depth bias, returns 1 if not set
	float Function GetLightShadowDepthBias(ObjectReference akLightObject) global native
	
	;Get light type (1-5); returns 0 if no lights exist.
	;kFlag_TypeHemiShadow = 1,
	;kFlag_TypeOmni = 2;
	;kFlag_TypeOmniShadow = 3,
	;kFlag_TypeSpot = 4,
	;kFlag_TypeSpotShadow = 5,
	int Function GetLightType(Light akLight) global native
	
	;-------
	;SETTERS
	;-------
	
	;Sets light color.
	Function SetLightColor(Light akLight, ColorForm akColorform) global native
		
	;Sets light fade range.
	Function SetLightFade(Light akLight, float range) global native
		
	;sets base light FOV
	Function SetLightFOV(Light akLight, float FOV) global native
	
	;Sets light radius (minimum light radius is 16) 
	Function SetLightRadius(Light akLight, float radius) global native
	
	;sets depth bias on light reference. Creates new depth bias extradata on lights that don't have it set.
	Function SetLightShadowDepthBias(ObjectReference akLightObject, float depthBias) global native
	
	;Sets light type. Does not persist between sessions.
	Function SetLightType(Light akLight, int lightType) global native
	
;----------------------------------------------------------------------------------------------------------
;LOCATION
;----------------------------------------------------------------------------------------------------------

	;-------
	;GETTERS
	;-------

	;Get parent location
	Location Function GetParentLocation(Location akLocation) global native
	
	;-------
	;SETTERS
	;-------
	
	;Set parent location
	Function SetParentLocation(Location akLocation, Location newLocation) global native
	
;----------------------------------------------------------------------------------------------------------		
;MAGIC EFFECTS
;----------------------------------------------------------------------------------------------------------		

	;---------
	;GETTERS
	;---------
	
	;Gets associated form (Light for Light spells, Actor for Summon Creature...), if any
	Form Function GetAssociatedForm(MagicEffect mgef) global native;

	;Gets effect archetype of magiceffect and returns as int (0-46).
	int Function GetEffectArchetypeAsInt(MagicEffect mgef) global native;
	
	;Gets effect archetype of magiceffect and returns as String
	;ValueMod
	;Script
	;Dispel
	;CureDisease
	;Absorb
	;DualValueMod
	;Calm
	;Demoralize
	;Frenzy
	;Disarm
	;CommandSummoned
	;Invisibility
	;Light;
	;Lock
	;Open
	;BoundWeapon
	;SummonCreature
	;DetectLife
	;Telekinesis
	;Paralysis
	;Reanimate
	;SoulTrap
	;TurnUndead
	;Guide
	;WerewolfFeed
	;CureParalysis
	;CureAddiction
	;CurePoison
	;Concussion
	;ValueAndParts
	;AccumulateMagnitude
	;Stagger
	;PeakValueMod
	;Cloak
	;Werewolf
	;SlowTime
	;Rally
	;EnhanceWeapon
	;SpawnHazard
	;Etherealize
	;Banish
	;Disguise
	;GrabActor
	;VampireLord
	String Function GetEffectArchetypeAsString(MagicEffect mgef) global native
	
	;Gets primary actor value as string, if any (FrostResist, SpeedMult)
	String Function GetPrimaryActorValue(MagicEffect mgef) global native
	
	;Gets secondary actor value as string, if any
	String Function GetSecondaryActorValue(MagicEffect mgef) global native
	
	;Gets Sound attached to index of Sound type specified in magic effect.
	;0 = Draw/Sheathe
	;1 = Charge
	;2 = Ready
	;3 = Release
	;4 = Concentration Cast Loop
	;5 = On Hit
	SoundDescriptor Function GetMagicEffectSound(MagicEffect mgef, int type) global native
	
	;-------
	;SETTERS
	;-------
	
	;Sets Sound attached to index of Sound type specified in magic effect
	Function SetMagicEffectSound(MagicEffect mgef, SoundDescriptor mgefSound, int type) global native
	
;----------------------------------------------------------------------------------------------------------
;OBJECTREFERENCES
;----------------------------------------------------------------------------------------------------------

	;--------
	;GETTERS
	;--------
		
	ObjectReference[] Function FindAllReferencesOfFormType(ObjectReference akRef, Int formType, float radius) global native
	
	ObjectReference[] Function FindAllReferencesWithKeyword(ObjectReference akRef, Form keywordOrList, float radius, bool matchAll) global native
	
	ObjectReference[] Function FindAllReferencesOfType(ObjectReference akRef, Form formOrList, float radius) global native
	
	;Gets actor responsible for object
	Actor Function GetActorCause(ObjectReference akRef) global native
	
	;Get all art objects attached to this object.
	Art[] Function GetAllArtObjects(ObjectReference akTarget) global native
	
	;Get all effect shaders attached to this object.
	EffectShader[] Function GetAllEffectShaders(ObjectReference akTarget) global native
		
	;Gets closest actor to ref, without returning that ref which is what the vanilla Function does
	Actor Function GetClosestActorFromRef(ObjectReference akRef, bool ignorePlayer) global native
	
	float Function GetEffectShaderDuration(ObjectReference akRef, EffectShader akShader) global native
	
	;Gets random actor near ref, without returning that ref which is what the vanilla Function does
	Actor Function GetRandomActorFromRef(ObjectReference akRef, float radius, bool ignorePlayer) global native
	
	;Checks if the specified art object is attached to reference, and if so returns the number of instances of that art. Otherwise returns zero. 
	int Function HasArtObject(ObjectReference akTarget, Art akArtObject, bool active = false) global native
	
	;Checks if the specified effect shader is playing on the reference, and if so returns the number of instances of that shader playing on it. Otherwise returns zero. 
	int Function HasEffectShader(ObjectReference akTarget, EffectShader akShader, bool active = false) global native
	
	;accepts partial matches
	bool Function HasNiExtraData(ObjectReference akRef, String name) global native
	
	;Is door a load door?
	bool Function IsLoadDoor(ObjectReference akDoor) global native
	
	;-------
	;SETTERS
	;-------
	
	;Function  is untested in cases where the inventory count exceeds normal papyrus array limit of 128.
	Form[] Function AddAllInventoryItemsToArray(ObjectReference akRef, bool noEquipped = true, bool noFavourited = false, bool noQuestItem = false) global native
	
	;Adds content of inventory to formlist, filtering out equipped, favourited and quest items. 
	Function AddAllInventoryItemsToList(ObjectReference akRef, Formlist akList, bool noEquipped = true, bool noFavourited = false, bool noQuestItem = false) global native
	
	;keywords are now serialized to skse cosave so they will be applied correctly on loading/reloading saves. 	
	Function AddKeywordToRef(ObjectReference akTarget, Keyword KYWDtoAdd) global native	
	
	;Snaps the object to the nearest navmesh point closest to its current position in the cell
	Function MoveToNearestNavmeshLocation(ObjectReference akTarget) global native
				
	bool Function RemoveKeywordFromRef(ObjectReference akTarget, Keyword KYWDtoRemove) global native
	
	;Replaces given keyword with new one on form. Only lasts for a single gaming session. [ported from DienesTools]
	Function ReplaceKeywordOnRef(ObjectReference akRef, Keyword KYWDtoAdd, Keyword KYWDtoRemove) global native
	
	;sets EffectShader duration. Internal duration is set when the effectshader begins and does not change with time.
	Function SetEffectShaderDuration(ObjectReference akRef, EffectShader akShader, float time, bool absolute) global native
	
	;copies skin tint color from actorbase to bodyparts nif
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
	Function SetShaderType(ObjectReference akRef, ObjectReference akTemplate, String diffusePath, int shaderType, int textureType, bool noWeapons, bool noAlphaProperty) global native
								
	;Stops ALL effect shaders and art objects (VFX) currently on this actor
	Function StopAllShaders(ObjectReference akTarget) global native
	
	;Checks if the specified art object (hit magic effect/not just visual effect) is attached to reference, and if so removes it.
	Function StopArtObject(ObjectReference akTarget, Art akArtObject) global native
							
;----------------------------------------------------------------------------------------------------------
;PACKAGES
;----------------------------------------------------------------------------------------------------------

	;GETTERS
	
	;Gets package type. Returns -1 if package is none
	;kPackageType_Find = 0
	;kPackageType_Follow = 1
	;kPackageType_Escort = 2
	;kPackageType_Eat = 3
	;kPackageType_Sleep = 4
	;kPackageType_Wander = 5
	;kPackageType_Travel = 6
	;kPackageType_Accompany = 7
	;kPackageType_UseItemAt = 8
	;kPackageType_Ambush = 9
	;kPackageType_FleeNotCombat = 10
	;kPackageType_CastMagic = 11
	;kPackageType_Sandbox = 12
	;kPackageType_Patrol = 13
	;kPackageType_Guard = 14
	;kPackageType_Dialogue = 15
	;kPackageType_UseWeapon = 16
	;kPackageType_Find2 = 17
	;kPackageType_Package = 18
	;kPackageType_PackageTemplate = 19
	;kPackageType_Activate = 20
	;kPackageType_Alarm = 21
	;kPackageType_Flee = 22
	;kPackageType_Trespass = 23
	;kPackageType_Spectator = 24
	;kPackageType_ReactToDead = 25
	;kPackageType_GetUpFromChair = 26
	;kPackageType_DoNothing = 27
	;kPackageType_InGameDialogue = 28
	;kPackageType_Surface =  29
	;kPackageType_SearchForAttacker = 30
	;kPackageType_AvoidPlayer = 31
	;kPackageType_ReactToDestroyedObject = 32
	;kPackageType_ReactToGrenadeOrMine = 33
	;kPackageType_StealWarning = 34
	;kPackageType_PickPocketWarning = 35
	;kPackageType_MovementBlocked = 36
	;kPackageType_VampireFeed = 37
	;kPackageType_CannibalFeed = 38
	;kPackageType_Unknown39 = 39
	;kPackageType_Unknown40 = 40
	;kPackageType_Unknown41 = 41
	int Function GetPackageType(Package akPackage) global native
		
;----------------------------------------------------------------------------------------------------------
;PROJECTILES
;----------------------------------------------------------------------------------------------------------

	;--------
	;GETTERS
	;--------
	
	;Gets projectile gravity (usually 0.0 for non arrow projectiles).
	float Function GetProjectileGravity(Projectile akProjectile) global native
	
	;Gets projectile impact force 
	float Function GetProjectileImpactForce(Projectile akProjectile) global native
	
	;Gets projectile range.
	float Function GetProjectileRange(Projectile akProjectile) global native
	
	;Gets projectile speed.
	float Function GetProjectileSpeed(Projectile akProjectile) global native
	
	;Get projectile type (1-7); returns 0 if no projectiles exist.
	;kMissile = 1
	;kLobber = 2
	;kBeam = 3
	;kFlame = 4
	;kCone = 5
	;kBarrier = 6
	;kArrow = 7
	int Function GetProjectileType(Projectile akProjectile) global native
	
	;-------
	;SETTERS
	;-------
	
	;Sets projectile gravity. 
	Function SetProjectileGravity(Projectile akProjectile, float gravity) global native
	
	;Sets projectile impact force. 
	Function SetProjectileImpactForce(Projectile akProjectile, float impactForce) global native
	
	;Sets projectile range.
	Function SetProjectileRange(Projectile akProjectile, float range) global native
	
	;Sets projectile speed. 
	Function SetProjectileSpeed(Projectile akProjectile, float speed) global native

	
;-----------------------------------------------------------------------------------------------------------	
;SOUND
;-----------------------------------------------------------------------------------------------------------	

	;Sets Sound descriptor.	
	Function SetSoundDescriptor(Sound akSound, SoundDescriptor akSoundDescriptor) global native
	
;-----------------------------------------------------------------------------------------------------------	
;SPELL
;-----------------------------------------------------------------------------------------------------------	
	;to get ConditionItemObject / function IDs / OPCodes
	;https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/FormComponents/Components/TESCondition.h
	
	;ConditionItemObject | Function ID | parameter 1 | parameter 2 | OPCode | float | ANDOR
	;param1/param2 didn't work for two conditions (GetGlobal and HasMagicEffectKeyword) - needs more testing
	;conditions which have no parameters (eg. IsSneaking) work
	
	;Subject	| HasMagicEffectKeyword	| MagicInvisibility		| NONE | == | 0.0 | AND
	;0 			| 699					| 0001EA6F ~ Skyrim.esm | NONE | 0  | 0.0 | AND		
	
	Function AddMagicEffectToSpell(Spell akSpell, MagicEffect mgef, float magnitude, int area, int duration, float cost = 0.0, String[] conditionList = None) global native
	
	;returns which type of spell it is (0-7) / returns -1 if spell is None
	;kTypeSpell = 0
	;kTypeDisease = 1
	;kTypePower = 2
	;kTypeLesserPower = 3
	;kTypeAbility = 4
	;kTypePoison = 5
	;kTypeAddition = 6
	;kTypeVoice = 7
	int Function GetSpellType(Spell akSpell) global native
	
	;effect that matches magnitude/area/dur will be deleted. Not persistent.
	Function RemoveMagicEffectFromSpell(Spell akSpell, MagicEffect mgef, float magnitude, int area, int duration, float cost = 0.0) global native
			
;----------------------------------------------------------------------------------------------------------	
;STRINGS
;----------------------------------------------------------------------------------------------------------

	;converts string to hex value if valid
	String Function IntToString(int value, bool hex) global native
	
	;returns -1 for out of bound values
	int Function StringToInt(String akString) global native
	
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
	
	Art Function GetArtObject(VisualEffect effect) global native
	
	;Returns the total number of art objects present/present and active (on objects) within the loaded area.
	int Function GetArtObjectTotalCount(VisualEffect effect, bool active) global native
	
	;--------
	;SETTERS
	;--------
	
	Function SetArtObject(VisualEffect effect, Art artObject) global native
			
;-----------------------------------------------------------------------------------------------------------
;WEATHER
;----------------------------------------------------------------------------------------------------------		

	;Gets wind speed as shown as in CK conditions (0.0-1.0).
	float Function GetWindSpeedAsFloat(Weather akWeather) global native
	
	;Gets wind speed as shown in the weather form (0-255).
	int Function GetWindSpeedAsInt(Weather akWeather) global native
	
	;0 - Pleasant
	;1 - Cloudy
	;2 - Rainy
	;3 - Snow
	;Gets weather type or current weather type if akWeather is passed as none
	int Function GetWeatherType(Weather akWeather = none) global native 