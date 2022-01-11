#include "Manager.h"

#include "EventHandler.h"
#include "Cache.h"

namespace Game
{
	void Register()
	{
		Cache::Register();
		Event::Register();
		
		DETECTION::Register();
	}
}
