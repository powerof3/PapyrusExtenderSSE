Scriptname PO3_Events_AME  Hidden 

;EVENTS SHOULD BE CALLED ON AN ACTIVEFFECT - script that is attached to a magic effect.
;DOCUMENTATION IS AVAILABLE AT https://github.com/powerof3/PapyrusExtenderSSE/wiki

;ACTOR FALL LONG DISTANCE
	
	Function RegisterForActorFallLongDistance(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForActorFallLongDistance(ActiveMagicEffect akActiveEffect) global native
	
	Event OnActorFallLongDistance(Actor akTarget, float afFallDistance, float afFallDamage)
	EndEvent

;ACTOR KILL
	
	Function RegisterForActorKilled(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForActorKilled(ActiveMagicEffect akActiveEffect) global native
	
	Event OnActorKilled(Actor akVictim, Actor akKiller)
	EndEvent
	
;ACTOR REANIMATE
	
	Function RegisterForActorReanimateStart(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForActorReanimateStart(ActiveMagicEffect akActiveEffect) global native
	
	Function RegisterForActorReanimateStop(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForActorReanimateStop(ActiveMagicEffect akActiveEffect) global native
	
	Event OnActorReanimateStart(Actor akTarget, Actor akCaster)
	EndEvent
	
	Event OnActorReanimateStop(Actor akTarget, Actor akCaster)
	EndEvent		
	
;ACTOR RESURRECT
	
	Function RegisterForActorResurrected(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForActorResurrected(ActiveMagicEffect akActiveEffect) global native
	
	Event OnActorResurrected(Actor akTarget, bool abResetInventory)
	EndEvent
	
;BOOKS READ
	
	Function RegisterForBookRead(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForBookRead(ActiveMagicEffect akActiveEffect) global native
	
	Event OnBookRead(Book akBook)
	EndEvent

;CELL FULLY LOADED
	
	Function RegisterForCellFullyLoaded(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForCellFullyLoaded(ActiveMagicEffect akActiveEffect) global native
	
	Event OnCellFullyLoaded(Cell akCell)
	EndEvent
	
;CRITICAL HIT

	Function RegisterForCriticalHit(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForCriticalHit(ActiveMagicEffect akActiveEffect) global native
	
	Event OnCriticalHit(Actor akAggressor, Weapon akWeapon, bool abSneakHit)
	EndEvent
	
;DISARMED
	
	Function RegisterForDisarmed(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForDisarmed(ActiveMagicEffect akActiveEffect) global native
	
	Event OnDisarmed(Actor akSource, Weapon akTarget)
	EndEvent
	
;DRAGON SOUL ABSORBED
	
	Function RegisterForDragonSoulGained(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForDragonSoulGained(ActiveMagicEffect akActiveEffect) global native
	
	Event OnDragonSoulGained(float afSouls)
	EndEvent
	
;ON HIT EX

	Function RegisterForHitEventEx(ActiveMagicEffect akActiveEffect, Form akAggressorFilter = None, Form akSourceFilter = None, Form akProjectileFilter = None, \
	int aiPowerFilter = -1, int aiSneakFilter = -1, int aiBashFilter = -1, int aiBlockFilter = -1, bool abMatch = true) global native	
	
	Function UnregisterForHitEventEx(ActiveMagicEffect akActiveEffect, Form akAggressorFilter = None, Form akSourceFilter = None, Form akProjectileFilter = None, \
	int aiPowerFilter = -1, int aiSneakFilter = -1, int aiBashFilter = -1, int aiBlockFilter = -1, bool abMatch = true) global native
	
	Function UnregisterForAllHitEventsEx(ActiveMagicEffect akActiveEffect) global native
		
	Event OnHitEx(ObjectReference akAggressor, Form akSource, Projectile akProjectile, bool abPowerAttack, bool abSneakAttack, bool abBashAttack, bool abHitBlocked)
	EndEvent
	
;ITEM CRAFTED

	Function RegisterForItemCrafted(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForItemCrafted(ActiveMagicEffect akActiveEffect) global native
	
	Event OnItemCrafted(ObjectReference akBench, Location akLocation, Form akCreatedItem)
	EndEvent
	
;ITEM HARVESTED
	
	Function RegisterForItemHarvested(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForItemHarvested(ActiveMagicEffect akActiveEffect) global native
	
	Event OnItemHarvested(Form akProduce)
	EndEvent
	
;LEVEL INCREASE
	
	Function RegisterForLevelIncrease(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForLevelIncrease(ActiveMagicEffect akActiveEffect) global native
	
	Event OnLevelIncrease(int aiLevel)
	EndEvent
	
;LOCATION DISCOVERY
	
	Function RegisterForLocationDiscovery(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForLocationDiscovery(ActiveMagicEffect akActiveEffect) global native
	
	Event OnLocationDiscovery(String asRegionName, String asWorldspaceName)
	EndEvent
		
;OBJECT GRAB/RELEASE

	Function RegisterForObjectGrab(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForObjectGrab(ActiveMagicEffect akActiveEffect) global native
	
	Event OnObjectGrab(ObjectReference akObjectRef)
	EndEvent
	
	Event OnObjectRelease(ObjectReference akObjectRef)
	EndEvent	
		
;OBJECT LOADED/UNLOADED

	Function RegisterForObjectLoaded(ActiveMagicEffect akActiveEffect, int formType) global native	
	Function UnregisterForObjectLoaded(ActiveMagicEffect akActiveEffect, int formType) global native
	Function UnregisterForAllObjectsLoaded(ActiveMagicEffect akActiveEffect) global native
		
	Event OnObjectLoaded(ObjectReference akRef, int aiFormType)
	EndEvent
	
	Event OnObjectUnloaded(ObjectReference akRef, int aiFormType)
	EndEvent	
	
;QUEST START/STOP

	Function RegisterForQuest(ActiveMagicEffect akActiveEffect, Quest akQuest) global native	
	Function UnregisterForQuest(ActiveMagicEffect akActiveEffect, Quest akQuest) global native
	Function UnregisterForAllQuests(ActiveMagicEffect akActiveEffect) global native
	
	Event OnQuestStart(Quest akQuest)
	EndEvent
	
	Event OnQuestStop(Quest akQuest)
	EndEvent
	
;QUEST STAGE CHANGE

	Function RegisterForQuestStage(ActiveMagicEffect akActiveEffect, Quest akQuest) global native	
	Function UnregisterForQuestStage(ActiveMagicEffect akActiveEffect, Quest akQuest) global native
	Function UnregisterForAllQuestStages(ActiveMagicEffect akActiveEffect) global native
	
	Event OnQuestStageChange(Quest akQuest, Int aiNewStage)
	EndEvent
	
;SHOUT ATTACK

	Function RegisterForShoutAttack(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForShoutAttack(ActiveMagicEffect akActiveEffect) global native
	
	Event OnPlayerShoutAttack(Shout akShout)
	EndEvent
	
;SKILL INCREASE

	Function RegisterForSkillIncrease(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForSkillIncrease(ActiveMagicEffect akActiveEffect) global native
	
	Event OnSkillIncrease(Int aiSkill)
	EndEvent
	
;SOUL TRAP

	Function RegisterForSoulTrapped(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForSoulTrapped(ActiveMagicEffect akActiveEffect) global native
		
	Event OnSoulTrapped(Actor akVictim, Actor akKiller)
	EndEvent
	
;SPELL LEARNED

	Function RegisterForSpellLearned(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForSpellLearned(ActiveMagicEffect akActiveEffect) global native
		
	Event OnSpellLearned(Spell akSpell)
	EndEvent
	
;WEATHER CHANGE

	Function RegisterForWeatherChange(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForWeatherChange(ActiveMagicEffect akActiveEffect) global native
		
	Event OnWeatherChange(Weather akOldWeather, Weather akNewWeather)
	EndEvent
	
;MAGIC EFFECT APPLY

	Function RegisterForMagicEffectApplyEx(ActiveMagicEffect akActiveEffect, Form akEffectFilter, bool abMatch) global native	
	Function UnregisterForMagicEffectApplyEx(ActiveMagicEffect akActiveEffect, Form akEffectFilter, bool abMatch) global native
	Function UnregisterForAllMagicEffectApplyEx(ActiveMagicEffect akActiveEffect) global native
		
	Event OnMagicEffectApplyEx(ObjectReference akCaster, MagicEffect akEffect, Form akSource, bool abApplied)
	EndEvent
	
;ON WEAPON HIT
	
	Function RegisterForWeaponHit(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForWeaponHit(ActiveMagicEffect akActiveEffect) global native
		
	Event OnWeaponHit(ObjectReference akTarget, Form akSource, Projectile akProjectile, Int aiHitFlagMask)
	EndEvent
	
;ON MAGIC HIT

	Function RegisterForMagicHit(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForMagicHit(ActiveMagicEffect akActiveEffect) global native
		
	Event OnMagicHit(ObjectReference akTarget, Form akSource, Projectile akProjectile)
	EndEvent
	
;ON PROJECTILE HIT

	Function RegisterForProjectileHit(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForProjectileHit(ActiveMagicEffect akActiveEffect) global native
		
	Event OnProjectileHit(ObjectReference akTarget, Form akSource, Projectile akProjectile)
	EndEvent
	