Scriptname PO3_Events_Form  Hidden 

;EVENTS SHOULD BE CALLED ON A FORM - script that is attached to form/reference.
;DOCUMENTATION IS AVAILABLE AT https://github.com/powerof3/PapyrusExtenderSSE/wiki

;ACTOR FALL LONG DISTANCE
;calling script must extend ObjectReference
	
	Function RegisterForActorFallLongDistance(Form akForm) global native	
	Function UnregisterForActorFallLongDistance(Form akForm) global native
	
	Event OnActorFallLongDistance(Actor akTarget, float afFallDistance, float afFallDamage)
	EndEvent

;ACTOR KILL
	
	Function RegisterForActorKilled(Form akForm) global native	
	Function UnregisterForActorKilled(Form akForm) global native
	
	Event OnActorKilled(Actor akVictim, Actor akKiller)
	EndEvent
	
;ACTOR REANIMATE
;calling script must extend ObjectReference
	
	Function RegisterForActorReanimateStart(Form akForm) global native	
	Function UnregisterForActorReanimateStart(Form akForm) global native
	
	Function RegisterForActorReanimateStop(Form akForm) global native	
	Function UnregisterForActorReanimateStop(Form akForm) global native
	
	Event OnActorReanimateStart(Actor akTarget, Actor akCaster)
	EndEvent
	
	Event OnActorReanimateStop(Actor akTarget, Actor akCaster)
	EndEvent		
	
;ACTOR RESURRECT
;calling script must extend ObjectReference
	
	Function RegisterForActorResurrected(Form akForm) global native	
	Function UnregisterForActorResurrected(Form akForm) global native
	
	Event OnActorResurrected(Actor akTarget, bool abResetInventory)
	EndEvent
	
;BOOKS READ
	
	Function RegisterForBookRead(Form akForm) global native	
	Function UnregisterForBookRead(Form akForm) global native
	
	Event OnBookRead(Book akBook)
	EndEvent

;CELL FULLY LOADED
	
	Function RegisterForCellFullyLoaded(Form akForm) global native	
	Function UnregisterForCellFullyLoaded(Form akForm) global native
	
	Event OnCellFullyLoaded(Cell akCell)
	EndEvent
	
;CRITICAL HIT
	
	Function RegisterForCriticalHit(Form akForm) global native	
	Function UnregisterForCriticalHit(Form akForm) global native
	
	Event OnCriticalHit(Actor akAggressor, Weapon akWeapon, bool abSneakHit)
	EndEvent
	
;DISARMED
	
	Function RegisterForDisarmed(Form akForm) global native	
	Function UnregisterForDisarmed(Form akForm) global native
	
	Event OnDisarmed(Actor akSource, Weapon akTarget)
	EndEvent
	
;DRAGON SOUL ABSORBED
	
	Function RegisterForDragonSoulGained(Form akForm) global native	
	Function UnregisterForDragonSoulGained(Form akForm) global native
	
	Event OnDragonSoulGained(float afSouls)
	EndEvent
	
;ON HIT EX
;calling script must extend ObjectReference

	Function RegisterForHitEventEx(Form akForm, Form akAggressorFilter = None, Form akSourceFilter = None, Form akProjectileFilter = None, \
	int aiPowerFilter = -1, int aiSneakFilter = -1, int aiBashFilter = -1, int aiBlockFilter = -1, bool abMatch = true) global native	
	
	Function UnregisterForHitEventEx(Form akForm, Form akAggressorFilter = None, Form akSourceFilter = None, Form akProjectileFilter = None, \
	int aiPowerFilter = -1, int aiSneakFilter = -1, int aiBashFilter = -1, int aiBlockFilter = -1, bool abMatch = true) global native
	
	Function UnregisterForAllHitEventsEx(Form akForm) global native
		
	Event OnHitEx(ObjectReference akAggressor, Form akSource, Projectile akProjectile, bool abPowerAttack, bool abSneakAttack, bool abBashAttack, bool abHitBlocked)
	EndEvent
	
;ITEM CRAFTED
	
	Function RegisterForItemCrafted(Form akForm) global native	
	Function UnregisterForItemCrafted(Form akForm) global native
	
	Event OnItemCrafted(ObjectReference akBench, Location akLocation, Form akCreatedItem)
	EndEvent
	
;ITEM HARVESTED
	
	Function RegisterForItemHarvested(Form akForm) global native	
	Function UnregisterForItemHarvested(Form akForm) global native
	
	Event OnItemHarvested(Form akProduce)
	EndEvent
	
;LEVEL INCREASE
	
	Function RegisterForLevelIncrease(Form akForm) global native	
	Function UnregisterForLevelIncrease(Form akForm) global native
	
	Event OnLevelIncrease(int aiLevel)
	EndEvent
	
;LOCATION DISCOVERY
	
	Function RegisterForLocationDiscovery(Form akForm) global native	
	Function UnregisterForLocationDiscovery(Form akForm) global native
	
	Event OnLocationDiscovery(String asRegionName, String asWorldspaceName)
	EndEvent
		
;OBJECT GRAB/RELEASE

	Function RegisterForObjectGrab(Form akForm) global native	
	Function UnregisterForObjectGrab(Form akForm) global native
	
	Event OnObjectGrab(ObjectReference akObjectRef)
	EndEvent
	
	Event OnObjectRelease(ObjectReference akObjectRef)
	EndEvent	
		
;OBJECT LOADED/UNLOADED

	Function RegisterForObjectLoaded(Form akForm, int formType) global native	
	Function UnregisterForObjectLoaded(Form akForm, int formType) global native
	Function UnregisterForAllObjectsLoaded(Form akForm) global native
		
	Event OnObjectLoaded(ObjectReference akRef, int aiFormType)
	EndEvent
	
	Event OnObjectUnloaded(ObjectReference akRef, int aiFormType)
	EndEvent	
	
;QUEST START/STOP

	Function RegisterForQuest(Form akForm, Quest akQuest) global native	
	Function UnregisterForQuest(Form akForm, Quest akQuest) global native
	Function UnregisterForAllQuests(Form akForm) global native
	
	Event OnQuestStart(Quest akQuest)
	EndEvent
	
	Event OnQuestStop(Quest akQuest)
	EndEvent
	
;QUEST STAGE CHANGE

	Function RegisterForQuestStage(Form akForm, Quest akQuest) global native	
	Function UnregisterForQuestStage(Form akForm, Quest akQuest) global native
	Function UnregisterForAllQuestStages(Form akForm) global native
	
	Event OnQuestStageChange(Quest akQuest, Int aiNewStage)
	EndEvent
	
;SHOUT ATTACK

	Function RegisterForShoutAttack(Form akForm) global native	
	Function UnregisterForShoutAttack(Form akForm) global native
	
	Event OnPlayerShoutAttack(Shout akShout)
	EndEvent
	
;SKILL INCREASE

	Function RegisterForSkillIncrease(Form akForm) global native	
	Function UnregisterForSkillIncrease(Form akForm) global native
	
	Event OnSkillIncrease(Int aiSkill)
	EndEvent
	
;SOUL TRAP

	Function RegisterForSoulTrapped(Form akForm) global native	
	Function UnregisterForSoulTrapped(Form akForm) global native
		
	Event OnSoulTrapped(Actor akVictim, Actor akKiller)
	EndEvent
	
;SPELL LEARNED

	Function RegisterForSpellLearned(Form akForm) global native	
	Function UnregisterForSpellLearned(Form akForm) global native
		
	Event OnSpellLearned(Spell akSpell)
	EndEvent
	
;WEATHER CHANGE

	Function RegisterForWeatherChange(Form akForm) global native	
	Function UnregisterForWeatherChange(Form akForm) global native
		
	Event OnWeatherChange(Weather akOldWeather, Weather akNewWeather)
	EndEvent
	
;MAGIC EFFECT APPLY
;calling script must extend ObjectReference

	Function RegisterForMagicEffectApplyEx(Form akForm, Form akEffectFilter, bool abMatch) global native	
	Function UnregisterForMagicEffectApplyEx(Form akForm, Form akEffectFilter, bool abMatch) global native
	Function UnregisterForAllMagicEffectApplyEx(Form akForm) global native
		
	Event OnMagicEffectApplyEx(ObjectReference akCaster, MagicEffect akEffect, Form akSource, bool abApplied)
	EndEvent
	
;ON WEAPON HIT
;calling script must extend ObjectReference
	
	Function RegisterForWeaponHit(Form akForm) global native	
	Function UnregisterForWeaponHit(Form akForm) global native
		
	Event OnWeaponHit(ObjectReference akTarget, Form akSource, Projectile akProjectile, Int aiHitFlagMask)
	EndEvent
	
;ON MAGIC HIT
;calling script must extend ObjectReference

	Function RegisterForMagicHit(Form akForm) global native	
	Function UnregisterForMagicHit(Form akForm) global native
		
	Event OnMagicHit(ObjectReference akTarget, Form akSource, Projectile akProjectile)
	EndEvent
	
;ON PROJECTILE HIT
;calling script must extend ObjectReference

	Function RegisterForProjectileHit(Form akForm) global native	
	Function UnregisterForProjectileHit(Form akForm) global native
		
	Event OnProjectileHit(ObjectReference akTarget, Form akSource, Projectile akProjectile)
	EndEvent
	