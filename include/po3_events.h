#pragma once

//----------------------------------------------------------------------------------------------

/*template <typename D>
class EventRegistration
{
public:

	UInt64	handle;
	D		params;

	bool operator<(const EventRegistration & rhs) const { return handle < rhs.handle; }
};

class NullParameters
{
public:
};

template <typename K, typename D = NullParameters>
class RegistrationMapHolder : public SafeDataHolder<std::map<K, std::set<EventRegistration<D>>>>
{
	typedef std::set<EventRegistration<D>>	RegSet;
	typedef std::map<K, RegSet>				RegMap;

public:

	template <typename T>
	void Register(K & key, UInt32 type, T * classType, D * params = NULL)
	{
		VMClassRegistry		* registry = (*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy = registry->GetHandlePolicy();

		EventRegistration<D> reg;
		reg.handle = policy->Create(type, (void *)classType);
		if (params)
			reg.params = *params;

		Lock();

		if (m_data[key].insert(reg).second)
			policy->AddRef(reg.handle);

		Release();
	}

	template <typename T>
	void Unregister(K & key, UInt32 type, T * classType)
	{
		VMClassRegistry		* registry = (*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy = registry->GetHandlePolicy();

		EventRegistration<D> reg;
		reg.handle = policy->Create(type, (void *)classType);

		Lock();

		if (m_data[key].erase(reg))
			policy->Release(reg.handle);

		Release();
	}

	template <typename F>
	void ForEach(K & key, F & functor)
	{
		Lock();

		RegMap::iterator handles = m_data.find(key);

		if (handles != m_data.end())
			for (RegSet::iterator iter = handles->second.begin(); iter != handles->second.end(); ++iter)
				functor(*iter);

		Release();
	}

};

template <typename D = NullParameters>
class RegistrationSetHolder : public SafeDataHolder<std::set<EventRegistration<D>>>
{
	typedef std::set<EventRegistration<D>>	RegSet;

public:

	template <typename T>
	void Register(UInt32 type, T * classType, D * params = NULL)
	{
		VMClassRegistry		* registry = (*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy = registry->GetHandlePolicy();

		EventRegistration<D> reg;
		reg.handle = policy->Create(type, (void *)classType);
		if (params)
			reg.params = *params;

		Lock();

		if (m_data.insert(reg).second)
			policy->AddRef(reg.handle);

		Release();
	}

	template <typename T>
	void Unregister(UInt32 type, T * classType)
	{
		VMClassRegistry		* registry = (*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy = registry->GetHandlePolicy();

		EventRegistration<D> reg;
		reg.handle = policy->Create(type, (void *)classType);

		Lock();

		if (m_data.erase(reg))
			policy->Release(reg.handle);

		Release();
	}

	template <typename F>
	void ForEach(F & functor)
	{
		Lock();

		for (RegSet::iterator iter = m_data.begin(); iter != m_data.end(); ++iter)
			functor(*iter);

		Release();
	}
};

//-----------------------------------------------------------------------------------------------------------------------------

extern RegistrationSetHolder<NullParameters>						po3_cellFullyLoadedEventRegs;
extern RegistrationSetHolder<NullParameters>						po3_grabReleaseEventRegs;
extern RegistrationSetHolder<NullParameters>						po3_questStartStopEventRegs;
extern RegistrationMapHolder<TESObjectREFR*>						po3_activeEffectRemoveEventRegs;
extern RegistrationSetHolder<NullParameters>						po3_questStartStopEventRegs;
extern RegistrationMapHolder<UInt32>								po3_objectLoadedEventRegs;

//-------------------------------------------------------------------------------------------------------------------------------

class cellFullyLoadedEventHandler : public BSTEventSink <TESCellFullyLoadedEvent>
{
public:
	virtual	EventResult		ReceiveEvent(TESCellFullyLoadedEvent * evn, EventDispatcher<TESCellFullyLoadedEvent> * dispatcher);
};

class activeEffectRemoveEventHandler : public BSTEventSink <TESActiveEffectApplyRemoveEvent>
{
public:
	virtual	EventResult		ReceiveEvent(TESActiveEffectApplyRemoveEvent * evn, EventDispatcher<TESActiveEffectApplyRemoveEvent> * dispatcher);
};

class grabReleaseEventHandler : public BSTEventSink <TESGrabReleaseEvent>
{
public:
	virtual	EventResult		ReceiveEvent(TESGrabReleaseEvent * evn, EventDispatcher <TESGrabReleaseEvent> * dispatcher);
};

class objectLoadedEventHandler : public BSTEventSink <TESObjectLoadedEvent>
{
public:
	virtual	EventResult		ReceiveEvent(TESObjectLoadedEvent * evn, EventDispatcher<TESObjectLoadedEvent> * dispatcher);
};

class questStartStopEventHandler : public BSTEventSink <TESQuestStartStopEvent>
{
public:
	virtual	EventResult		ReceiveEvent(TESQuestStartStopEvent * evn, EventDispatcher<TESQuestStartStopEvent> * dispatcher);
};

extern cellFullyLoadedEventHandler		po3_cellFullyLoadedEventHandler;
extern activeEffectRemoveEventHandler 	po3_activeEffectRemoveEventHandler;
extern grabReleaseEventHandler 			po3_grabReleaseEventHandler;
extern objectLoadedEventHandler 		po3_objectLoadedEventHandler;
extern questStartStopEventHandler 		po3_questStartStopEventHandler;

//----------------------------------------------------------------------------------------------------------------------

namespace APIEvents
{
	bool RegisterEvents(VMClassRegistry * registry);
};*/
