Scriptname PO3_Events_Form  Hidden 

;EVENTS SHOULD BE CALLED ON A FORM

;ACTOR KILL
	
	Function RegisterForActorKilled(Form akForm) global native	
	Function UnregisterForActorKilled(Form akForm) global native
	
	Event OnActorKilled(Actor akVictim, Actor akKiller)
	EndEvent
	
;ACTOR REANIMATE
;start fires when actor is reanimated and stop when the reanimate effect is dispelled
	
	Function RegisterForActorReanimateStart(Form akForm) global native	
	Function UnregisterForActorReanimateStart(Form akForm) global native
	
	Function RegisterForActorReanimateStop(Form akForm) global native	
	Function UnregisterForActorReanimateStop(Form akForm) global native
	
	Event OnActorReanimateStart(Actor akTarget, Actor akCaster)
	EndEvent
	
	Event OnActorReanimateStop(Actor akTarget, Actor akCaster)
	EndEvent		
	
;ACTOR RESURRECT
;fires when the target has been resurrected via script or console command
	
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
;Can fire multiple times in exteriors, for each cell that is fully loaded.
	
	Function RegisterForCellFullyLoaded(Form akForm) global native	
	Function UnregisterForCellFullyLoaded(Form akForm) global native
	
	Event OnCellFullyLoaded(Cell akCell)
	EndEvent
	
;CRITICAL HIT
;Player only event?
	
	Function RegisterForCriticalHit(Form akForm) global native	
	Function UnregisterForCriticalHit(Form akForm) global native
	
	Event OnCriticalHit(Actor akAggressor, Weapon akWeapon, book abSneakHit)
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
	
;ITEM HARVESTED
;Player only event
	
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
;Doesn't work with telekinesis and when the player grabs the same object in a row

	Function RegisterForObjectGrab(Form akForm) global native	
	Function UnregisterForObjectGrab(Form akForm) global native
	
	Event OnObjectGrab(ObjectReference akObjectRef)
	EndEvent
	
	Event OnObjectRelease(ObjectReference akObjectRef)
	EndEvent	
		
;OBJECT LOADED/UNLOADED
;Not all objects fire this event. It is somewhat inconsistent.

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
;Player only event

	Function RegisterForShoutAttack(Form akForm) global native	
	Function UnregisterForShoutAttack(Form akForm) global native
	
	Event OnShoutAttack(Shout akShout)
	EndEvent
	
;SKILL INCREASE

	Function RegisterForSkillIncrease(Form akForm) global native	
	Function UnregisterForSkillIncrease(Form akForm) global native
	
	Event OnSkillIncrease(String asSkill)
	EndEvent
	
;SOUL TRAP
;Event will fire after OnDying/OnDeath

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
	
;FEC - RESET ACTOR EFFECTS

	Function RegisterForFECReset(Form akForm, int aiType) global native	
	Function UnregisterForFECReset(Form akForm, int aiType) global native
	Function UnregisterForAllFECResets(Form akForm) global native
		
	Event OnFECReset(Actor akActor, int aiType, bool abReset3D)
	EndEvent
	