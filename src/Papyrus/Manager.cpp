#include "Papyrus/Manager.h"

#include "Papyrus/Functions/ActiveMagicEffect/Events.h"
#include "Papyrus/Functions/ActiveMagicEffect/Functions.h"
#include "Papyrus/Functions/Actor.h"
#include "Papyrus/Functions/ActorBase.h"
#include "Papyrus/Functions/Alias/Events.h"
#include "Papyrus/Functions/Alias/Functions.h"
#include "Papyrus/Functions/ArmorAddon.h"
#include "Papyrus/Functions/Array.h"
#include "Papyrus/Functions/Book.h"
#include "Papyrus/Functions/Cell.h"
#include "Papyrus/Functions/Debug.h"
#include "Papyrus/Functions/EffectShader.h"
#include "Papyrus/Functions/Enchantment.h"
#include "Papyrus/Functions/Faction.h"
#include "Papyrus/Functions/Form/Events.h"
#include "Papyrus/Functions/Form/Functions.h"
#include "Papyrus/Functions/Furniture.h"
#include "Papyrus/Functions/Game.h"
#include "Papyrus/Functions/Hazard.h"
#include "Papyrus/Functions/Light.h"
#include "Papyrus/Functions/Location.h"
#include "Papyrus/Functions/MagicEffect.h"
#include "Papyrus/Functions/ObjectReference.h"
#include "Papyrus/Functions/Package.h"
#include "Papyrus/Functions/Potion.h"
#include "Papyrus/Functions/Projectile.h"
#include "Papyrus/Functions/Quest.h"
#include "Papyrus/Functions/Scroll.h"
#include "Papyrus/Functions/Sound.h"
#include "Papyrus/Functions/Spell.h"
#include "Papyrus/Functions/Strings.h"
#include "Papyrus/Functions/UI.h"
#include "Papyrus/Functions/Utility.h"
#include "Papyrus/Functions/VisualEffect.h"
#include "Papyrus/Functions/Weather.h"
#include "Papyrus/ObjectTypes.h"

namespace Papyrus
{
	bool Bind(VM* a_vm)
	{
		if (!a_vm) {
			logger::critical("couldn't get VM State"sv);
			return false;
		}

		logger::info("{:*^30}", "FUNCTIONS"sv);

		ObjectTypes::Bind(*a_vm);

		ActiveMagicEffect::Events::Bind(*a_vm);
		ActiveMagicEffect::Functions::Bind(*a_vm);

		Actor::Bind(*a_vm);
		ActorBase::Bind(*a_vm);
		ArmorAddon::Bind(*a_vm);

		Alias::Events::Bind(*a_vm);
		Alias::Functions::Bind(*a_vm);

		Array::Bind(*a_vm);
		Book::Bind(*a_vm);
		Cell::Bind(*a_vm);
		Debug::Bind(*a_vm);
		EffectShader::Bind(*a_vm);
		Enchantment::Bind(*a_vm);

		Form::Events::Bind(*a_vm);
		Form::Functions::Bind(*a_vm);

		Faction::Bind(*a_vm);
		Furniture::Bind(*a_vm);
		Game::Bind(*a_vm);
		Hazard::Bind(*a_vm);
		Light::Bind(*a_vm);
		Location::Bind(*a_vm);
		MagicEffect::Bind(*a_vm);
		ObjectReference::Bind(*a_vm);
		Package::Bind(*a_vm);
		Potion::Bind(*a_vm);
		Projectile::Bind(*a_vm);
		Quest::Bind(*a_vm);
		Scroll::Bind(*a_vm);
		Sound::Bind(*a_vm);
		Spell::Bind(*a_vm);
		Strings::Bind(*a_vm);
		UI::Bind(*a_vm);
		Utility::Bind(*a_vm);
		VisualEffect::Bind(*a_vm);
		Weather::Bind(*a_vm);

		return true;
	}
}
