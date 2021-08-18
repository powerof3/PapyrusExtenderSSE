#include "Papyrus.h"

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
#include "Functions/Utility.h"
#include "Functions/VisualEffect.h"
#include "Functions/Weather.h"
#include "ObjectTypes.h"

namespace Papyrus
{
	bool Bind(VM* a_vm)
	{
		if (!a_vm) {
			logger::critical("couldn't get VMState"sv);
			return false;
		}

		logger::info("{:*^30}", "FUNCTIONS"sv);

		Papyrus::ObjectTypes::Bind(*a_vm);

		Papyrus::ActiveMagicEffect::Bind(*a_vm);

		Papyrus::Actor::Bind(*a_vm);

		Papyrus::ActorBase::Bind(*a_vm);

		Papyrus::ArmorAddon::Bind(*a_vm);

		Papyrus::Alias::Bind(*a_vm);

		Papyrus::Array::Bind(*a_vm);

		Papyrus::Book::Bind(*a_vm);

		Papyrus::Cell::Bind(*a_vm);

		Papyrus::Debug::Bind(*a_vm);

		Papyrus::EffectShader::Bind(*a_vm);

		Papyrus::Enchantment::Bind(*a_vm);

		Papyrus::Form::Bind(*a_vm);

		Papyrus::FEC::Bind(*a_vm);

		Papyrus::Furniture::Bind(*a_vm);

		Papyrus::Game::Bind(*a_vm);

		Papyrus::Hazard::Bind(*a_vm);

		Papyrus::Light::Bind(*a_vm);

		Papyrus::Location::Bind(*a_vm);

		Papyrus::MagicEffect::Bind(*a_vm);

		Papyrus::ObjectReference::Bind(*a_vm);

		Papyrus::Package::Bind(*a_vm);

		Papyrus::Potion::Bind(*a_vm);

		Papyrus::Projectile::Bind(*a_vm);

		Papyrus::Scroll::Bind(*a_vm);

		Papyrus::Sound::Bind(*a_vm);

		Papyrus::Spell::Bind(*a_vm);

		Papyrus::Strings::Bind(*a_vm);

		Papyrus::Utility::Bind(*a_vm);

		Papyrus::VisualEffect::Bind(*a_vm);

		Papyrus::Weather::Bind(*a_vm);

		return true;
	}
}
