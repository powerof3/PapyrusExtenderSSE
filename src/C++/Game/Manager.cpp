#include "Manager.h"

#include "Cache.h"
#include "EventHandler.h"

namespace Game
{
	void Register()
	{
		Cache::Register();
		Event::Register();

		DETECTION::Register();
	}
}
