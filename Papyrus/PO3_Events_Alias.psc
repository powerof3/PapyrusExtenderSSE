Scriptname PO3_Events_Alias  Hidden 

;EVENTS SHOULD BE CALLED ON AN ALIAS - script that is attached to a reference alias/location alias.
;DOCUMENTATION IS AVAILABLE AT https://github.com/powerof3/PapyrusExtenderSSE/wiki


;ACTOR FALL LONG DISTANCE
	
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
	
	Function RegisterForActorReanimateStart(Alias akAlias) global native	
	Function UnregisterForActorReanimateStart(Alias akAlias) global native
	
	Function RegisterForActorReanimateStop(Alias akAlias) global native	
	Function UnregisterForActorReanimateStop(Alias akAlias) global native
	
	Event OnActorReanimateStart(Actor akTarget, Actor akCaster)
	EndEvent
	
	Event OnActorReanimateStop(Actor akTarget, Actor akCaster)
	EndEvent		
	
;ACTOR RESURRECT
	
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
	
	Function RegisterForCellFullyLoaded(Alias akAlias) global native	
	Function UnregisterForCellFullyLoaded(Alias akAlias) global native
	
	Event OnCellFullyLoaded(Cell akCell)
	EndEvent
	
;CRITICAL HIT
	
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
	
;ON HIT EX

	Function RegisterForHitEventEx(ReferenceAlias akRefAlias, Form akAggressorFilter = None, Form akSourceFilter = None, Form akProjectileFilter = None, \
	int aiPowerFilter = -1, int aiSneakFilter = -1, int aiBashFilter = -1, int aiBlockFilter = -1, bool abMatch = true) global native	
	
	Function UnregisterForHitEventEx(ReferenceAlias akRefAlias, Form akAggressorFilter = None, Form akSourceFilter = None, Form akProjectileFilter = None, \
	int aiPowerFilter = -1, int aiSneakFilter = -1, int aiBashFilter = -1, int aiBlockFilter = -1, bool abMatch = true) global native
	
	Function UnregisterForAllHitEventsEx(ReferenceAlias akRefAlias) global native
		
	Event OnHitEx(ObjectReference akAggressor, Form akSource, Projectile akProjectile, bool abPowerAttack, bool abSneakAttack, bool abBashAttack, bool abHitBlocked)
	EndEvent
	
;ITEM CRAFTED

	Function RegisterForItemCrafted(Alias akAlias) global native	
	Function UnregisterForItemCrafted(Alias akAlias) global native
	
	Event OnItemCrafted(ObjectReference akBench, Location akLocation, Form akCreatedItem)
	EndEvent
	
;ITEM HARVESTED
	
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

	Function RegisterForObjectGrab(Alias akAlias) global native	
	Function UnregisterForObjectGrab(Alias akAlias) global native
	
	Event OnObjectGrab(ObjectReference akObjectRef)
	EndEvent
	
	Event OnObjectRelease(ObjectReference akObjectRef)
	EndEvent	
		
;OBJECT LOADED/UNLOADED

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

	Function RegisterForShoutAttack(Alias akAlias) global native	
	Function UnregisterForShoutAttack(Alias akAlias) global native
	
	Event OnShoutAttack(Shout akShout)
	EndEvent
	
;SKILL INCREASE

	Function RegisterForSkillIncrease(Alias akAlias) global native	
	Function UnregisterForSkillIncrease(Alias akAlias) global native
	
	Event OnSkillIncrease(Int aiSkill)
	EndEvent
	
;SOUL TRAP

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

	Function RegisterForMagicEffectApplyEx(ReferenceAlias akRefAlias, Form akEffectFilter, bool abMatch) global native	
	Function UnregisterForMagicEffectApplyEx(ReferenceAlias akRefAlias, Form akEffectFilter, bool abMatch) global native
	Function UnregisterForAllMagicEffectApplyEx(ReferenceAlias akRefAlias) global native
		
	Event OnMagicEffectApplyEx(ObjectReference akCaster, MagicEffect akEffect, Form akSource, bool abApplied)
	EndEvent
	
;ON WEAPON HIT

	Function RegisterForWeaponHit(ReferenceAlias akRefAlias) global native	
	Function UnregisterForWeaponHit(ReferenceAlias akRefAlias) global native
		
	Event OnWeaponHit(ObjectReference akTarget, Form akSource, Projectile akProjectile, Int aiHitFlagMask)
	EndEvent
	
;ON MAGIC HIT

	Function RegisterForMagicHit(ReferenceAlias akRefAlias) global native	
	Function UnregisterForMagicHit(ReferenceAlias akRefAlias) global native
		
	Event OnMagicHit(ObjectReference akTarget, Form akSource, Projectile akProjectile)
	EndEvent

;ON PROJECTILE HIT

	Function RegisterForProjectileHit(ReferenceAlias akRefAlias) global native	
	Function UnregisterForProjectileHit(ReferenceAlias akRefAlias) global native
		
	Event OnProjectileHit(ObjectReference akTarget, Form akSource, Projectile akProjectile)
	EndEvent	