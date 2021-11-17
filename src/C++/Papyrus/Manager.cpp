#include "Manager.h"

#include "Functions/ActiveMagicEffect.h"
#include "Functions/Actor.h"
#include "Functions/ActorBase.h"
#include "Functions/Alias.h"
#include "Functions/ArmorAddon.h"
#include "Functions/Array.h"
#include "Functions/Book.h"
#include "Functions/Cell.h"
#include "Functions/Debug.h"
#include "Functions/EffectShader.h"
#include "Functions/Enchantment.h"
#include "Functions/FEC.h"
#include "Functions/Form.h"
#include "Functions/Furniture.h"
#include "Functions/Game.h"
#include "Functions/Hazard.h"
#include "Functions/Light.h"
#include "Functions/Location.h"
#include "Functions/MagicEffect.h"
#include "Functions/ObjectReference.h"
#include "Functions/Package.h"
#include "Functions/Potion.h"
#include "Functions/Projectile.h"
#include "Functions/Scroll.h"
#include "Functions/Sound.h"
#include "Functions/Spell.h"
#include "Functions/Strings.h"
#include "Functions/UI.h"
#include "Functions/Utility.h"
#include "Functions/VisualEffect.h"
#include "Functions/Weather.h"
#include "ObjectTypes.h"

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

        ActiveMagicEffect::Bind(*a_vm);

        Actor::Bind(*a_vm);

        ActorBase::Bind(*a_vm);

        ArmorAddon::Bind(*a_vm);

        Alias::Bind(*a_vm);

        Array::Bind(*a_vm);

        Book::Bind(*a_vm);

        Cell::Bind(*a_vm);

        Debug::Bind(*a_vm);

        EffectShader::Bind(*a_vm);

        Enchantment::Bind(*a_vm);

        Form::Bind(*a_vm);

        FEC::Bind(*a_vm);

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
