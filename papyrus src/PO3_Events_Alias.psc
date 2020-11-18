Scriptname PO3_Events_Alias  Hidden 

;EVENTS SHOULD BE CALLED ON AN ALIAS - script that is attached to a reference alias/location alias.

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
	
	Event OnCriticalHit(Actor akAggressor, Weapon akWeapon, book abSneakHit)
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

	Function RegisterForSkillIncrease(Alias akAlias) global native	
	Function UnregisterForSkillIncrease(Alias akAlias) global native
	
	Event OnSkillIncrease(String asSkill)
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
	