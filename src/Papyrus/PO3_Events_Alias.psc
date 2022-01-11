Scriptname PO3_Events_Alias  Hidden 

;EVENTS SHOULD BE CALLED ON AN ALIAS - script that is attached to a reference alias/location alias.

;ACTOR FALL LONG DISTANCE
;fires when the actor falls enough distance to take fall damage
	
	Function RegisterForActorFallLongDistance(ReferenceAlias akRefAlias) global native	
	Function UnregisterForActorFallLongDistance(ReferenceAlias akRefAlias) global native
	
	Event OnActorFallLongDistance(Actor akTarget, float afFallDistance, float afFallDamage)
	EndEvent

;ACTOR KILL
	
	Function RegisterForActorKilled(Alias akAlias) global native	
	Function UnregisterForActorKilled(Alias akAlias) global native
	
	Event OnActorKilled(Actor akVictim, Actor akKiller)
	EndEvent
	
;ACTOR REANIMATE
;start fires when actor is reanimated and stop when the reanimate effect is dispelled
	
	Function RegisterForActorReanimateStart(Alias akAlias) global native	
	Function UnregisterForActorReanimateStart(Alias akAlias) global native
	
	Function RegisterForActorReanimateStop(Alias akAlias) global native	
	Function UnregisterForActorReanimateStop(Alias akAlias) global native
	
	Event OnActorReanimateStart(Actor akTarget, Actor akCaster)
	EndEvent
	
	Event OnActorReanimateStop(Actor akTarget, Actor akCaster)
	EndEvent		
	
;ACTOR RESURRECT
;fires when the target has been resurrected via script or console command
	
	Function RegisterForActorResurrected(Alias akAlias) global native	
	Function UnregisterForActorResurrected(Alias akAlias) global native
	
	Event OnActorResurrected(Actor akTarget, bool abResetInventory)
	EndEvent
	
;BOOKS READ
	
	Function RegisterForBookRead(Alias akAlias) global native	
	Function UnregisterForBookRead(Alias akAlias) global native
	
	Event OnBookRead(Book akBook)
	EndEvent

;CELL FULLY LOADED
;Can fire multiple times in exteriors, for each cell that is fully loaded.
	
	Function RegisterForCellFullyLoaded(Alias akAlias) global native	
	Function UnregisterForCellFullyLoaded(Alias akAlias) global native
	
	Event OnCellFullyLoaded(Cell akCell)
	EndEvent
	
;CRITICAL HIT
;Player only event?
	
	Function RegisterForCriticalHit(Alias akAlias) global native	
	Function UnregisterForCriticalHit(Alias akAlias) global native
	
	Event OnCriticalHit(Actor akAggressor, Weapon akWeapon, bool abSneakHit)
	EndEvent
	
;DISARMED
	
	Function RegisterForDisarmed(Alias akAlias) global native	
	Function UnregisterForDisarmed(Alias akAlias) global native
	
	Event OnDisarmed(Actor akSource, Weapon akTarget)
	EndEvent
	
;DRAGON SOUL ABSORBED
	
	Function RegisterForDragonSoulGained(Alias akAlias) global native	
	Function UnregisterForDragonSoulGained(Alias akAlias) global native
	
	Event OnDragonSoulGained(float afSouls)
	EndEvent
	
;ITEM HARVESTED
;Player only event
	
	Function RegisterForItemHarvested(Alias akAlias) global native	
	Function UnregisterForItemHarvested(Alias akAlias) global native
	
	Event OnItemHarvested(Form akProduce)
	EndEvent
	
;LEVEL INCREASE
	
	Function RegisterForLevelIncrease(Alias akAlias) global native	
	Function UnregisterForLevelIncrease(Alias akAlias) global native
	
	Event OnLevelIncrease(int aiLevel)
	EndEvent
	
;LOCATION DISCOVERY
	
	Function RegisterForLocationDiscovery(Alias akAlias) global native	
	Function UnregisterForLocationDiscovery(Alias akAlias) global native
	
	Event OnLocationDiscovery(String asRegionName, String asWorldspaceName)
	EndEvent
		
;OBJECT GRAB/RELEASE
;Doesn't work with telekinesis and when the player grabs the same object in a row

	Function RegisterForObjectGrab(Alias akAlias) global native	
	Function UnregisterForObjectGrab(Alias akAlias) global native
	
	Event OnObjectGrab(ObjectReference akObjectRef)
	EndEvent
	
	Event OnObjectRelease(ObjectReference akObjectRef)
	EndEvent	
		
;OBJECT LOADED/UNLOADED
;Not all objects fire this event. It is somewhat inconsistent.

	Function RegisterForObjectLoaded(Alias akAlias, int formType) global native	
	Function UnregisterForObjectLoaded(Alias akAlias, int formType) global native
	Function UnregisterForAllObjectsLoaded(Alias akAlias) global native
		
	Event OnObjectLoaded(ObjectReference akRef, int aiFormType)
	EndEvent
	
	Event OnObjectUnloaded(ObjectReference akRef, int aiFormType)
	EndEvent	
	
;QUEST START/STOP

	Function RegisterForQuest(Alias akAlias, Quest akQuest) global native	
	Function UnregisterForQuest(Alias akAlias, Quest akQuest) global native
	Function UnregisterForAllQuests(Alias akAlias) global native
	
	Event OnQuestStart(Quest akQuest)
	EndEvent
	
	Event OnQuestStop(Quest akQuest)
	EndEvent
	
;QUEST STAGE CHANGE

	Function RegisterForQuestStage(Alias akAlias, Quest akQuest) global native	
	Function UnregisterForQuestStage(Alias akAlias, Quest akQuest) global native
	Function UnregisterForAllQuestStages(Alias akAlias) global native
	
	Event OnQuestStageChange(Quest akQuest, Int aiNewStage)
	EndEvent
	
;SHOUT ATTACK
;Player only event

	Function RegisterForShoutAttack(Alias akAlias) global native	
	Function UnregisterForShoutAttack(Alias akAlias) global native
	
	Event OnShoutAttack(Shout akShout)
	EndEvent
	
;SKILL INCREASE
;4.5.6 - Event had its params changed from String to Int as a workaround for only the first registered event recieving any events
;See https://github.com/Ryan-rsm-McKenzie/CommonLibSSE/blob/master/include/RE/A/ActorValues.h


	Function RegisterForSkillIncrease(Alias akAlias) global native	
	Function UnregisterForSkillIncrease(Alias akAlias) global native
	
	Event OnSkillIncrease(Int aiSkill)
	EndEvent
	
;SOUL TRAP
;Event will fire after OnDying/OnDeath

	Function RegisterForSoulTrapped(Alias akAlias) global native	
	Function UnregisterForSoulTrapped(Alias akAlias) global native
		
	Event OnSoulTrapped(Actor akVictim, Actor akKiller)
	EndEvent
	
;SPELL LEARNED

	Function RegisterForSpellLearned(Alias akAlias) global native	
	Function UnregisterForSpellLearned(Alias akAlias) global native
		
	Event OnSpellLearned(Spell akSpell)
	EndEvent
	
;WEATHER CHANGE

	Function RegisterForWeatherChange(Alias akAlias) global native	
	Function UnregisterForWeatherChange(Alias akAlias) global native
		
	Event OnWeatherChange(Weather akOldWeather, Weather akNewWeather)
	EndEvent
	
;MAGIC EFFECT APPLY
;Filter takes in a matching magic effect, a keyword, or a formlist containing keywords.
;bApplied will return if the magic effect was applied or not, unlike vanilla event which fires for everything.

	Function RegisterForMagicEffectApplyEx(ReferenceAlias akRefAlias, Form akEffectFilter, bool abMatch) global native	
	Function UnregisterForMagicEffectApplyEx(ReferenceAlias akRefAlias, Form akEffectFilter, bool abMatch) global native
	Function UnregisterForAllMagicEffectApplyEx(ReferenceAlias akRefAlias) global native
		
	Event OnMagicEffectApplyEx(ObjectReference akCaster, MagicEffect akEffect, Form akSource, bool abApplied)
	EndEvent
	
;ON WEAPON HIT
;Event OnHit except weapons only AND the aggressor recieves this event for each target hit by it
;Statics have no hit flags - 0
	
	;/ FLAGS - use SKSE's LogicalAnd to check if flag is set
			
		kBlocked = 1,
		kBlockWithWeapon = 2,
		kBlockCandidate = 4,
		kCritical = 8,
		kCriticalOnDeath = 16,
		kFatal = 32,
		kDismemberLimb = 64,
		kExplodeLimb = 128,
		kCrippleLimb = 256,
		kDisarm = 512,
		kDisableWeapon = 1024,
		kSneakAttack = 2048,
		kIgnoreCritical = 4096,
		kPredictDamage = 8192,
		kPredictBaseDamage = 16384,
		kBash = 32768,
		kTimedBash = 65536,
		kPowerAttack = 131072,
		kMeleeAttack = 262144,
		kRicochet = 524288,
		kExplosion = 1048576
	/;

	Function RegisterForWeaponHit(ReferenceAlias akRefAlias) global native	
	Function UnregisterForWeaponHit(ReferenceAlias akRefAlias) global native
		
	Event OnWeaponHit(ObjectReference akTarget, Form akSource, Projectile akProjectile, Int aiHitFlagMask)
	EndEvent
	
;ON MAGIC HIT
;Event OnHit except for magic AND the aggressor recieves this event for each target hit by it

	Function RegisterForMagicHit(ReferenceAlias akRefAlias) global native	
	Function UnregisterForMagicHit(ReferenceAlias akRefAlias) global native
		
	Event OnMagicHit(ObjectReference akTarget, Form akSource, Projectile akProjectile)
	EndEvent

;ON PROJECTILE HIT
;Event OnHit except for projectiles AND the aggressor recieves this event for each target hit by it

	Function RegisterForProjectileHit(ReferenceAlias akRefAlias) global native	
	Function UnregisterForProjectileHit(ReferenceAlias akRefAlias) global native
		
	Event OnProjectileHit(ObjectReference akTarget, Form akSource, Projectile akProjectile)
	EndEvent	