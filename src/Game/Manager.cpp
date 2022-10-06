#include "Game/Manager.h"
#include "Game/EventHandler.h"

namespace Game
{
	void Register()
	{
		Event::Register();

		DETECTION::Register();
	}
}
