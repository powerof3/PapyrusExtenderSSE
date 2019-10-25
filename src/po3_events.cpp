#include "po3_events.h"
#include "po3_functions.h"

//-----------------------------------Global instances-----------------------------------------

/*RegistrationSetHolder<NullParameters>						po3_cellFullyLoadedEventRegs;
RegistrationSetHolder<NullParameters>						po3_grabReleaseEventRegs;
RegistrationSetHolder<NullParameters>						po3_questStartStopEventRegs;
RegistrationMapHolder<TESObjectREFR*>						po3_activeEffectRemoveEventRegs;
RegistrationMapHolder<UInt32>								po3_objectLoadedEventRegs;

cellFullyLoadedEventHandler									po3_cellFullyLoadedEventHandler;
activeEffectRemoveEventHandler								po3_activeEffectRemoveEventHandler;
grabReleaseEventHandler										po3_grabReleaseEventHandler;
questStartStopEventHandler									po3_questStartStopEventHandler;
objectLoadedEventHandler									po3_objectLoadedEventHandler;

//-----------------------------------Generic functors-----------------------------------------

template <typename T> void SetVMValue(VMValue * val, T arg)
{
	VMClassRegistry * registry = (*g_skyrimVM)->GetClassRegistry();
	PackValue(val, &arg, registry);
}

template <> void SetVMValue<bool>(VMValue * val, bool arg) { val->SetBool(arg); }
template <> void SetVMValue<SInt32>(VMValue * val, SInt32 arg) { val->SetInt(arg); }
template <> void SetVMValue<float>(VMValue * val, float arg) { val->SetFloat(arg); }
template <> void SetVMValue<BSFixedString>(VMValue * val, BSFixedString arg) { val->SetString(arg.data); }

template <typename T1>
class EventQueueFunctor1 : public IFunctionArguments
{
public:
	EventQueueFunctor1(BSFixedString & a_eventName, T1 a_arg1)
		: eventName(a_eventName.data), arg1(a_arg1) {}

	virtual bool	Copy(Output * dst)
	{
		dst->Resize(1);
		SetVMValue(dst->Get(0), arg1);

		return true;
	}

	void			operator() (const EventRegistration<NullParameters> & reg)
	{
		VMClassRegistry * registry = (*g_skyrimVM)->GetClassRegistry();
		registry->QueueEvent(reg.handle, &eventName, this);
	}

private:
	BSFixedString	eventName;
	T1				arg1;
};

template <typename T1, typename T2>
class EventQueueFunctor2 : public IFunctionArguments
{
public:
	EventQueueFunctor2(BSFixedString & a_eventName, T1 a_arg1, T2 a_arg2)
		: eventName(a_eventName.data), arg1(a_arg1), arg2(a_arg2) {}

	virtual bool	Copy(Output * dst)
	{
		dst->Resize(2);
		SetVMValue(dst->Get(0), arg1);
		SetVMValue(dst->Get(1), arg2);

		return true;
	}

	void			operator() (const EventRegistration<NullParameters> & reg)
	{
		VMClassRegistry * registry = (*g_skyrimVM)->GetClassRegistry();
		registry->QueueEvent(reg.handle, &eventName, this);
	}

private:
	BSFixedString	eventName;
	T1				arg1;
	T2				arg2;
};

template <typename T1, typename T2, typename T3>
class EventQueueFunctor3 : public IFunctionArguments
{
public:
	EventQueueFunctor3(BSFixedString & a_eventName, T1 a_arg1, T2 a_arg2, T3 a_arg3)
		: eventName(a_eventName.data), arg1(a_arg1), arg2(a_arg2), arg3(a_arg3){}

	virtual bool	Copy(Output * dst)
	{
		dst->Resize(3);
		SetVMValue(dst->Get(0), arg1);
		SetVMValue(dst->Get(1), arg2);
		SetVMValue(dst->Get(2), arg3);
		return true;
	}

	void			operator() (const EventRegistration<NullParameters> & reg)
	{
		VMClassRegistry * registry = (*g_skyrimVM)->GetClassRegistry();
		registry->QueueEvent(reg.handle, &eventName, this);
	}

private:
	BSFixedString	eventName;
	T1				arg1;
	T2				arg2;
	T3				arg3;
};

template <typename T1, typename T2, typename T3, typename T4>
class EventQueueFunctor4 : public IFunctionArguments
{
public:
	EventQueueFunctor4(BSFixedString & a_eventName, T1 a_arg1, T2 a_arg2, T3 a_arg3, T4 a_arg4)
		: eventName(a_eventName.data), arg1(a_arg1), arg2(a_arg2), arg3(a_arg3), arg4(a_arg4) {}

	virtual bool	Copy(Output * dst)
	{
		dst->Resize(4);
		SetVMValue(dst->Get(0), arg1);
		SetVMValue(dst->Get(1), arg2);
		SetVMValue(dst->Get(2), arg3);
		SetVMValue(dst->Get(3), arg4);
		return true;
	}

	void			operator() (const EventRegistration<NullParameters> & reg)
	{
		VMClassRegistry * registry = (*g_skyrimVM)->GetClassRegistry();
		registry->QueueEvent(reg.handle, &eventName, this);
	}

private:
	BSFixedString	eventName;
	T1				arg1;
	T2				arg2;
	T3				arg3;
	T4				arg4;
};

//--------------------------------------Event handlers---------------------------------------------------

EventResult cellFullyLoadedEventHandler::ReceiveEvent(TESCellFullyLoadedEvent * evn, EventDispatcher<TESCellFullyLoadedEvent> * dispatcher)
{
	if (evn->cell == nullptr)
		return kEvent_Continue;
	
	po3_cellFullyLoadedEventRegs.ForEach(
		EventQueueFunctor1<TESObjectCELL*>(BSFixedString("OnCellFullyLoaded"), evn->cell)
	);

	return kEvent_Continue;
}

EventResult activeEffectRemoveEventHandler::ReceiveEvent(TESActiveEffectApplyRemoveEvent * evn, EventDispatcher<TESActiveEffectApplyRemoveEvent> * dispatcher)
{
	if (evn->caster == nullptr || evn->target == nullptr)
		return kEvent_Continue;

	EffectSetting* mgef = nullptr;
	
	mgef = (evn->unk1C == 1) ? evn->effect2->effect->mgef : evn->effect1->effect->mgef;

	if (mgef == nullptr)
		return kEvent_Continue;
	
	po3_activeEffectRemoveEventRegs.ForEach(evn->target,
			EventQueueFunctor3<TESObjectREFR*, TESObjectREFR*, EffectSetting*>(BSFixedString("OnMagicEffectRemove"), evn->caster, evn->target, mgef)
		);

	return kEvent_Continue;
}

EventResult grabReleaseEventHandler::ReceiveEvent(TESGrabReleaseEvent * evn, EventDispatcher<TESGrabReleaseEvent> * dispatcher)
{
	if (evn->ref == nullptr)
		return kEvent_Continue;

	BSFixedString eventName = (evn->isGrabbing) ? BSFixedString("OnObjectGrab") : BSFixedString("OnObjectRelease");

	po3_grabReleaseEventRegs.ForEach(
		EventQueueFunctor1<TESObjectREFR*>(eventName, evn->ref)
	);

	return kEvent_Continue;
}

EventResult objectLoadedEventHandler::ReceiveEvent(TESObjectLoadedEvent * evn, EventDispatcher<TESObjectLoadedEvent> * dispatcher)
{
	TESForm * thisForm = LookupFormByID(evn->formID);
	
	if (thisForm == nullptr)
		return kEvent_Continue;

	BSFixedString eventName = (evn->loaded == 1) ? BSFixedString("OnObjectLoaded") : BSFixedString("OnObjectUnloaded");
	UInt32 formType = thisForm->GetFormType();

	po3_objectLoadedEventRegs.ForEach(formType,
		EventQueueFunctor2<TESForm*, UInt32>(eventName, thisForm, formType)
	);

	return kEvent_Continue;
}

EventResult questStartStopEventHandler::ReceiveEvent(TESQuestStartStopEvent * evn, EventDispatcher<TESQuestStartStopEvent> * dispatcher)
{
	TESQuest * quest = DYNAMIC_CAST(LookupFormByID(evn->questFormID), TESForm, TESQuest);

	if (quest == nullptr)
		return kEvent_Continue;

	BSFixedString eventName = (evn->unk04) ? BSFixedString("OnQuestStart") : BSFixedString("OnQuestStop");

	po3_questStartStopEventRegs.ForEach(
		EventQueueFunctor1<TESQuest*>(eventName, quest)
	);

	return kEvent_Continue;
}

//----------------------------------EVENTS------------------------------------------------------

namespace po3EventRegistrations
{
	// CELL LOADED FULLY
	void RegisterForCellFullyLoaded_Alias(StaticFunctionTag * base, BGSRefAlias * thisAlias)
	{
		if (!thisAlias)
			return;

		po3_cellFullyLoadedEventRegs.Register<BGSRefAlias>(BGSRefAlias::kTypeID, thisAlias);
	}

	void UnregisterForCellFullyLoaded_Alias(StaticFunctionTag * base, BGSRefAlias * thisAlias)
	{
		if (!thisAlias)
			return;

		po3_cellFullyLoadedEventRegs.Unregister<BGSRefAlias>(BGSRefAlias::kTypeID, thisAlias);
	}

	void RegisterForCellFullyLoaded_ActiveEffect(StaticFunctionTag * base, ActiveEffect * thisEffect)
	{
		if (!thisEffect)
			return;

		po3_cellFullyLoadedEventRegs.Register<ActiveEffect>(ActiveEffect::kTypeID, thisEffect);
	}

	void UnregisterForCellFullyLoaded_ActiveEffect(StaticFunctionTag * base, ActiveEffect * thisEffect)
	{
		if (!thisEffect)
			return;

		po3_cellFullyLoadedEventRegs.Unregister<ActiveEffect>(ActiveEffect::kTypeID, thisEffect);
	}

	// ACTIVE EFFECT REMOVE
	void RegisterForMagicEffectRemove_Form(StaticFunctionTag * base, TESForm * thisForm, TESObjectREFR * ref)
	{
		if (!thisForm || !ref)
			return;

		po3_activeEffectRemoveEventRegs.Register<TESForm>(ref, thisForm->GetFormType(), thisForm);
	}

	void UnregisterForMagicEffectRemove_Form(StaticFunctionTag * base, TESForm * thisForm, TESObjectREFR * ref)
	{
		if (!thisForm || !ref)
			return;

		po3_activeEffectRemoveEventRegs.Unregister<TESForm>(ref, thisForm->GetFormType(), thisForm);
	}

	void RegisterForMagicEffectRemove_Alias(StaticFunctionTag * base, BGSRefAlias * thisAlias, TESObjectREFR * ref)
	{
		if (!thisAlias || !ref)
			return;

		po3_activeEffectRemoveEventRegs.Register<BGSRefAlias>(ref, BGSRefAlias::kTypeID, thisAlias);
	}

	void UnregisterForMagicEffectRemove_Alias(StaticFunctionTag * base, BGSRefAlias * thisAlias, TESObjectREFR * ref)
	{
		if (!thisAlias || !ref)
			return;

		po3_activeEffectRemoveEventRegs.Unregister<BGSRefAlias>(ref, BGSRefAlias::kTypeID, thisAlias);
	}

	void RegisterForMagicEffectRemove_ActiveEffect(StaticFunctionTag * base, ActiveEffect * thisEffect, TESObjectREFR * ref)
	{
		if (!thisEffect || !ref)
			return;

		po3_activeEffectRemoveEventRegs.Register<ActiveEffect>(ref, ActiveEffect::kTypeID, thisEffect);
	}

	void UnregisterForMagicEffectRemove_ActiveEffect(StaticFunctionTag * base, ActiveEffect * thisEffect, TESObjectREFR * ref)
	{
		if (!thisEffect || !ref)
			return;

		po3_activeEffectRemoveEventRegs.Unregister<ActiveEffect>(ref, ActiveEffect::kTypeID, thisEffect);
	}

	// ON GRAB/RELEASE
	void RegisterForObjectGrabRelease_Alias(StaticFunctionTag * base, BGSRefAlias * thisAlias)
	{
		if (!thisAlias)
			return;

		po3_grabReleaseEventRegs.Register<BGSRefAlias>(BGSRefAlias::kTypeID, thisAlias);
	}

	void UnregisterForObjectGrabRelease_Alias(StaticFunctionTag * base, BGSRefAlias * thisAlias)
	{
		if (!thisAlias)
			return;

		po3_grabReleaseEventRegs.Unregister<BGSRefAlias>(BGSRefAlias::kTypeID, thisAlias);
	}

	void RegisterForObjectGrabRelease_ActiveEffect(StaticFunctionTag * base, ActiveEffect * thisEffect)
	{
		if (!thisEffect)
			return;

		po3_grabReleaseEventRegs.Register<ActiveEffect>(ActiveEffect::kTypeID, thisEffect);
	}

	void UnregisterForObjectGrabRelease_ActiveEffect(StaticFunctionTag * base, ActiveEffect * thisEffect)
	{
		if (!thisEffect)
			return;

		po3_grabReleaseEventRegs.Unregister<ActiveEffect>(ActiveEffect::kTypeID, thisEffect);
	}

	// OBJECT LOADED
	void RegisterForObjectLoaded_Form(StaticFunctionTag * base, TESForm * thisForm, UInt32 formType)
	{
		if (!thisForm)
			return;

		po3_objectLoadedEventRegs.Register<TESForm>(formType, thisForm->GetFormType(), thisForm);
	}

	void UnregisterForObjectLoaded_Form(StaticFunctionTag * base, TESForm * thisForm, UInt32 formType)
	{
		if (!thisForm)
			return;

		po3_objectLoadedEventRegs.Unregister<TESForm>(formType, thisForm->GetFormType(), thisForm);
	}

	void RegisterForObjectLoaded_Alias(StaticFunctionTag * base, BGSRefAlias * thisAlias, UInt32 formType)
	{
		if (!thisAlias)
			return;

		po3_objectLoadedEventRegs.Register<BGSRefAlias>(formType, BGSRefAlias::kTypeID, thisAlias);
	}

	void UnregisterForObjectLoaded_Alias(StaticFunctionTag * base, BGSRefAlias * thisAlias, UInt32 formType)
	{
		if (!thisAlias)
			return;

		po3_objectLoadedEventRegs.Unregister<BGSRefAlias>(formType, BGSRefAlias::kTypeID, thisAlias);
	}

	void RegisterForObjectLoaded_ActiveEffect(StaticFunctionTag * base, ActiveEffect * thisEffect, UInt32 formType)
	{
		if (!thisEffect)
			return;

		po3_objectLoadedEventRegs.Register<ActiveEffect>(formType, ActiveEffect::kTypeID, thisEffect);
	}

	void UnregisterForObjectLoaded_ActiveEffect(StaticFunctionTag * base, ActiveEffect * thisEffect, UInt32 formType)
	{
		if (!thisEffect)
			return;

		po3_objectLoadedEventRegs.Unregister<ActiveEffect>(formType, ActiveEffect::kTypeID, thisEffect);
	}

	// ON QUEST START/STOP
	void RegisterForQuestStartStop_Form(StaticFunctionTag * base, TESForm * thisForm)
	{
		if (!thisForm)
			return;

		po3_questStartStopEventRegs.Register<TESForm>(thisForm->GetFormType(), thisForm);
	}

	void UnregisterForQuestStartStop_Form(StaticFunctionTag * base, TESForm * thisForm)
	{
		if (!thisForm)
			return;

		po3_questStartStopEventRegs.Unregister<TESForm>(thisForm->GetFormType(), thisForm);
	}

	void RegisterForQuestStartStop_Alias(StaticFunctionTag * base, BGSRefAlias * thisAlias)
	{
		if (!thisAlias)
			return;

		po3_questStartStopEventRegs.Register<BGSRefAlias>(BGSRefAlias::kTypeID, thisAlias);
	}

	void UnregisterForQuestStartStop_Alias(StaticFunctionTag * base, BGSRefAlias * thisAlias)
	{
		if (!thisAlias)
			return;

		po3_questStartStopEventRegs.Unregister<BGSRefAlias>(BGSRefAlias::kTypeID, thisAlias);
	}

	void RegisterForQuestStartStop_ActiveEffect(StaticFunctionTag * base, ActiveEffect * thisEffect)
	{
		if (!thisEffect)
			return;

		po3_questStartStopEventRegs.Register<ActiveEffect>(ActiveEffect::kTypeID, thisEffect);
	}

	void UnregisterForQuestStartStop_ActiveEffect(StaticFunctionTag * base, ActiveEffect * thisEffect)
	{
		if (!thisEffect)
			return;

		po3_questStartStopEventRegs.Unregister<ActiveEffect>(ActiveEffect::kTypeID, thisEffect);
	}
};

//------------------------------------------------------------------------------------------------------------------------------

namespace APIEvents
{
	// Tell the game about the new functions.
	bool RegisterEvents(VMClassRegistry * registry)
	{
		// The name of the script these functions belong to.
		const char* eventClass = "po3_SKSEEvents";

		if (!registry)
		{
			_MESSAGE("ERROR: papyrus function registry NULL");
			return false;
		}

		//po3EventFunctions

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, void, BGSRefAlias*>(
				"a_RegisterForCellFullyLoaded", eventClass, po3EventRegistrations::RegisterForCellFullyLoaded_Alias, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, void, BGSRefAlias*>(
				"a_UnregisterForCellFullyLoaded", eventClass, po3EventRegistrations::UnregisterForCellFullyLoaded_Alias, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, void, ActiveEffect*>(
				"m_RegisterForCellFullyLoaded", eventClass, po3EventRegistrations::RegisterForCellFullyLoaded_ActiveEffect, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, void, ActiveEffect*>(
				"m_UnregisterForCellFullyLoaded", eventClass, po3EventRegistrations::UnregisterForCellFullyLoaded_ActiveEffect, registry));
		//
		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, void, TESForm*, TESObjectREFR*>(
				"f_RegisterForMagicEffectRemove", eventClass, po3EventRegistrations::RegisterForMagicEffectRemove_Form, registry));

		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, void, TESForm*, TESObjectREFR*>(
				"f_UnregisterForMagicEffectRemove", eventClass, po3EventRegistrations::UnregisterForMagicEffectRemove_Form, registry));

		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, void, BGSRefAlias*, TESObjectREFR*>(
				"a_RegisterForMagicEffectRemove", eventClass, po3EventRegistrations::RegisterForMagicEffectRemove_Alias, registry));

		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, void, BGSRefAlias*, TESObjectREFR*>(
				"a_UnregisterForMagicEffectRemove", eventClass, po3EventRegistrations::UnregisterForMagicEffectRemove_Alias, registry));

		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, void, ActiveEffect*, TESObjectREFR*>(
				"m_RegisterForMagicEffectRemove", eventClass, po3EventRegistrations::RegisterForMagicEffectRemove_ActiveEffect, registry));

		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, void, ActiveEffect*, TESObjectREFR*>(
				"m_UnregisterForMagicEffectRemove", eventClass, po3EventRegistrations::UnregisterForMagicEffectRemove_ActiveEffect, registry));

		//
		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, void, BGSRefAlias*>(
				"a_RegisterForObjectGrabRelease", eventClass, po3EventRegistrations::RegisterForObjectGrabRelease_Alias, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, void, BGSRefAlias*>(
				"a_UnregisterForObjectGrabRelease", eventClass, po3EventRegistrations::UnregisterForObjectGrabRelease_Alias, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, void, ActiveEffect*>(
				"m_RegisterForObjectGrabRelease", eventClass, po3EventRegistrations::RegisterForObjectGrabRelease_ActiveEffect, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, void, ActiveEffect*>(
				"m_UnregisterForObjectGrabRelease", eventClass, po3EventRegistrations::UnregisterForObjectGrabRelease_ActiveEffect, registry));
		//
		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, void, TESForm*, UInt32>(
				"f_RegisterForObjectLoaded", eventClass, po3EventRegistrations::RegisterForObjectLoaded_Form, registry));

		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, void, TESForm*, UInt32>(
				"f_UnregisterForObjectLoaded", eventClass, po3EventRegistrations::UnregisterForObjectLoaded_Form, registry));

		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, void, BGSRefAlias*, UInt32>(
				"a_RegisterForObjectLoaded", eventClass, po3EventRegistrations::RegisterForObjectLoaded_Alias, registry));

		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, void, BGSRefAlias*, UInt32>(
				"a_UnregisterForObjectLoaded", eventClass, po3EventRegistrations::UnregisterForObjectLoaded_Alias, registry));

		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, void, ActiveEffect*, UInt32>(
				"m_RegisterForObjectLoaded", eventClass, po3EventRegistrations::RegisterForObjectLoaded_ActiveEffect, registry));

		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, void, ActiveEffect*, UInt32>(
				"m_UnregisterForObjectLoaded", eventClass, po3EventRegistrations::UnregisterForObjectLoaded_ActiveEffect, registry));

		//
		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, void, TESForm*>(
				"f_RegisterForQuestStartStop", eventClass, po3EventRegistrations::RegisterForQuestStartStop_Form, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, void, TESForm*>(
				"f_UnregisterForQuestStartStop", eventClass, po3EventRegistrations::UnregisterForQuestStartStop_Form, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, void, BGSRefAlias*>(
				"a_RegisterForQuestStartStop", eventClass, po3EventRegistrations::RegisterForQuestStartStop_Alias, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, void, BGSRefAlias*>(
				"a_UnregisterForQuestStartStop", eventClass, po3EventRegistrations::UnregisterForQuestStartStop_Alias, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, void, ActiveEffect*>(
				"m_RegisterForQuestStartStop", eventClass, po3EventRegistrations::RegisterForQuestStartStop_ActiveEffect, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, void, ActiveEffect*>(
				"m_UnregisterForQuestStartStop", eventClass, po3EventRegistrations::UnregisterForQuestStartStop_ActiveEffect, registry));

		// Mark the functions as thread-save so the VM doesn't need to wait while running one.
		registry->SetFunctionFlags(eventClass, "a_RegisterForCellFullyLoaded", VMClassRegistry::kFunctionFlag_NoWait);
		registry->SetFunctionFlags(eventClass, "a_UnregisterForCellFullyLoaded", VMClassRegistry::kFunctionFlag_NoWait);
		registry->SetFunctionFlags(eventClass, "m_RegisterForCellFullyLoaded", VMClassRegistry::kFunctionFlag_NoWait);
		registry->SetFunctionFlags(eventClass, "m_UnregisterForCellFullyLoaded", VMClassRegistry::kFunctionFlag_NoWait);
		//
		registry->SetFunctionFlags(eventClass, "f_RegisterForMagicEffectRemove", VMClassRegistry::kFunctionFlag_NoWait);
		registry->SetFunctionFlags(eventClass, "f_UnregisterForMagicEffectRemove", VMClassRegistry::kFunctionFlag_NoWait);
		registry->SetFunctionFlags(eventClass, "a_RegisterForMagicEffectRemove", VMClassRegistry::kFunctionFlag_NoWait);
		registry->SetFunctionFlags(eventClass, "a_UnregisterForMagicEffectRemove", VMClassRegistry::kFunctionFlag_NoWait);
		registry->SetFunctionFlags(eventClass, "m_RegisterForMagicEffectRemove", VMClassRegistry::kFunctionFlag_NoWait);
		registry->SetFunctionFlags(eventClass, "m_UnregisterForMagicEffectRemove", VMClassRegistry::kFunctionFlag_NoWait);
		//
		registry->SetFunctionFlags(eventClass, "a_RegisterForObjectGrabRelease", VMClassRegistry::kFunctionFlag_NoWait);
		registry->SetFunctionFlags(eventClass, "a_UnregisterForObjectGrabRelease", VMClassRegistry::kFunctionFlag_NoWait);
		registry->SetFunctionFlags(eventClass, "m_RegisterForObjectGrabRelease", VMClassRegistry::kFunctionFlag_NoWait);
		registry->SetFunctionFlags(eventClass, "m_UnregisterForObjectGrabRelease", VMClassRegistry::kFunctionFlag_NoWait);
		//
		registry->SetFunctionFlags(eventClass, "f_RegisterForObjectLoaded", VMClassRegistry::kFunctionFlag_NoWait);
		registry->SetFunctionFlags(eventClass, "f_UnregisterForObjectLoaded", VMClassRegistry::kFunctionFlag_NoWait);
		registry->SetFunctionFlags(eventClass, "a_RegisterForObjectLoaded", VMClassRegistry::kFunctionFlag_NoWait);
		registry->SetFunctionFlags(eventClass, "a_UnregisterForObjectLoaded", VMClassRegistry::kFunctionFlag_NoWait);
		registry->SetFunctionFlags(eventClass, "m_RegisterForObjectLoaded", VMClassRegistry::kFunctionFlag_NoWait);
		registry->SetFunctionFlags(eventClass, "m_UnregisterForObjectLoaded", VMClassRegistry::kFunctionFlag_NoWait);
		//
		registry->SetFunctionFlags(eventClass, "f_RegisterForQuestStartStop", VMClassRegistry::kFunctionFlag_NoWait);
		registry->SetFunctionFlags(eventClass, "f_UnregisterForQuestStartStop", VMClassRegistry::kFunctionFlag_NoWait);
		registry->SetFunctionFlags(eventClass, "a_RegisterForQuestStartStop", VMClassRegistry::kFunctionFlag_NoWait);
		registry->SetFunctionFlags(eventClass, "a_UnregisterForQuestStartStop", VMClassRegistry::kFunctionFlag_NoWait);
		registry->SetFunctionFlags(eventClass, "m_RegisterForQuestStartStop", VMClassRegistry::kFunctionFlag_NoWait);
		registry->SetFunctionFlags(eventClass, "m_UnregisterForQuestStartStop", VMClassRegistry::kFunctionFlag_NoWait);

		return true;
	}
};*/