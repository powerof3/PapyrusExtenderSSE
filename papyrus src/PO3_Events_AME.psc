Scriptname PO3_Events_AME  Hidden 

;EVENTS SHOULD BE CALLED ON AN ACTIVEFFECT - script that is attached to a magic effect.

;ACTOR KILL
	
	Function RegisterForActorKilled(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForActorKilled(ActiveMagicEffect akActiveEffect) global native
	
	Event OnActorKilled(Actor akVictim, Actor akKiller)
	EndEvent
	
;ACTOR REANIMATE
;start fires when actor is reanimated and stop when the reanimate effect is dispelled
	
	Function RegisterForActorReanimateStart(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForActorReanimateStart(ActiveMagicEffect akActiveEffect) global native
	
	Function RegisterForActorReanimateStop(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForActorReanimateStop(ActiveMagicEffect akActiveEffect) global native
	
	Event OnActorReanimateStart(Actor akTarget, Actor akCaster)
	EndEvent
	
	Event OnActorReanimateStop(Actor akTarget, Actor akCaster)
	EndEvent		
	
;ACTOR RESURRECT
;fires when the target has been resurrected via script or console command
	
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
;Can fire multiple times in exteriors, for each cell that is fully loaded.
	
	Function RegisterForCellFullyLoaded(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForCellFullyLoaded(ActiveMagicEffect akActiveEffect) global native
	
	Event OnCellFullyLoaded(Cell akCell)
	EndEvent
	
;CRITICAL HIT
;Player only event?
	
	Function RegisterForCriticalHit(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForCriticalHit(ActiveMagicEffect akActiveEffect) global native
	
	Event OnCriticalHit(Actor akAggressor, Weapon akWeapon, book abSneakHit)
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
	
;ITEM HARVESTED
;Player only event
	
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
;Doesn't work with telekinesis and when the player grabs the same object in a row

	Function RegisterForObjectGrab(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForObjectGrab(ActiveMagicEffect akActiveEffect) global native
	
	Event OnObjectGrab(ObjectReference akObjectRef)
	EndEvent
	
	Event OnObjectRelease(ObjectReference akObjectRef)
	EndEvent	
		
;OBJECT LOADED/UNLOADED
;Not all objects fire this event. It is somewhat inconsistent.

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
;Player only event

	Function RegisterForShoutAttack(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForShoutAttack(ActiveMagicEffect akActiveEffect) global native
	
	Event OnPlayerShoutAttack(Shout akShout)
	EndEvent
	
;SKILL INCREASE

	Function RegisterForSkillIncrease(ActiveMagicEffect akActiveEffect) global native	
	Function UnregisterForSkillIncrease(ActiveMagicEffect akActiveEffect) global native
	
	Event OnSkillIncrease(String asSkill)
	EndEvent
	
;SOUL TRAP
;Event will fire after OnDying/OnDeath

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
	
;FEC - RESET ACTOR EFFECTS

	Function RegisterForFECReset(ActiveMagicEffect akActiveEffect, int aiType) global native	
	Function UnregisterForFECReset(ActiveMagicEffect akActiveEffect, int aiType) global native
	Function UnregisterForAllFECResets(ActiveMagicEffect akActiveEffect) global native
		
	Event OnFECReset(Actor akActor, int aiType, bool abReset3D)
	EndEvent
	