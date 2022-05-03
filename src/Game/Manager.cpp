#include "Game/Manager.h"

#include "Game/Cache.h"
#include "Game/EventHandler.h"

namespace Game
{
	void Register()
	{
		Cache::Register();
		Event::Register();

		DETECTION::Register();
	}
}
