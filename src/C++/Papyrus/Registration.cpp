#include "Registration.h"

#include "ActiveMagicEffect.h"
#include "Actor.h"
#include "ActorBase.h"
#include "Alias.h"
#include "ArmorAddon.h"
#include "Array.h"
#include "Book.h"
#include "Cell.h"
#include "Debug.h"
#include "EffectShader.h"
#include "Enchantment.h"
#include "Events.h"
#include "ExtendedObjectTypes.h"
#include "Form.h"
#include "FEC.h"
#include "Furniture.h"
#include "Game.h"
#include "Hazard.h"
#include "Light.h"
#include "Location.h"
#include "MagicEffect.h"
#include "ObjectReference.h"
#include "Package.h"
#include "Potion.h"
#include "Projectile.h"
#include "Scroll.h"
#include "Sound.h"
#include "Spell.h"
#include "Strings.h"
#include "Utility.h"
#include "VisualEffect.h"
#include "Weather.h"

namespace Papyrus
{
	bool Script::Register(RE::BSScript::IVirtualMachine* a_vm)
	{
		logger::info("{:*^30}", "FUNCTIONS"sv);

		if (!a_vm) {
			logger::critical("couldn't get VMState"sv);
			return false;
		}

		Papyrus::ExtendedObjectTypes::Register(*a_vm);

		Papyrus::ActiveMagicEffect::Register(*a_vm);

		Papyrus::Actor::Register(*a_vm);

		Papyrus::ActorBase::Register(*a_vm);

		Papyrus::ArmorAddon::Register(*a_vm);

		Papyrus::Alias::Register(*a_vm);

		Papyrus::Array::Register(*a_vm);

		Papyrus::Book::Register(*a_vm);

		Papyrus::Cell::Register(*a_vm);

		Papyrus::Debug::Register(*a_vm);

		Papyrus::EffectShader::Register(*a_vm);

		Papyrus::Enchantment::Register(*a_vm);

		Papyrus::Form::Register(*a_vm);

		Papyrus::FEC::Register(*a_vm);

		Papyrus::Furniture::Register(*a_vm);

		Papyrus::Game::Register(*a_vm);

		Papyrus::Hazard::Register(*a_vm);

		Papyrus::Light::Register(*a_vm);

		Papyrus::Location::Register(*a_vm);

		Papyrus::MagicEffect::Register(*a_vm);

		Papyrus::ObjectReference::Register(*a_vm);

		Papyrus::Package::Register(*a_vm);

		Papyrus::Potion::Register(*a_vm);
		
		Papyrus::Projectile::Register(*a_vm);

		Papyrus::Scroll::Register(*a_vm);
		
		Papyrus::Sound::Register(*a_vm);

		Papyrus::Spell::Register(*a_vm);

		Papyrus::Strings::Register(*a_vm);

		Papyrus::Utility::Register(*a_vm);

		Papyrus::VisualEffect::Register(*a_vm);

		Papyrus::Weather::Register(*a_vm);

		return true;
	}

	void Events::RegisterScriptEvents()
	{
		using namespace ScriptEvents;

		logger::info("{:*^30}", "SCRIPT EVENTS"sv);

		auto events = RE::ScriptEventSourceHolder::GetSingleton();
		if (events) {
			events->AddEventSink(CellFullyLoadedEventHandler::GetSingleton());
			logger::info("Registered cell loaded event handler"sv);

			events->AddEventSink(QuestStartStopEventHandler::GetSingleton());
			logger::info("Registered quest start stop event handler"sv);

			events->AddEventSink(QuestStageEventHandler::GetSingleton());
			logger::info("Registered quest stage event handler"sv);

			events->AddEventSink(ObjectLoadedEventHandler::GetSingleton());
			logger::info("Registered object loaded event handler"sv);

			events->AddEventSink(GrabReleaseEventHandler::GetSingleton());
			logger::info("Registered grab release event handler"sv);
		} else {
			logger::critical("Failed to sink the script event handler"sv);
		}
	}

	void Events::RegisterStoryEvents()
	{
		using namespace StoryEvents;

		logger::info("{:*^30}", "STORY EVENTS"sv);

		auto actorKill = RE::ActorKill::GetEventSource();
		if (actorKill) {
			actorKill->AddEventSink(ActorKillEventHandler::GetSingleton());
			logger::info("Registered actor kill event handler"sv);
		}

		auto books = RE::BooksRead::GetEventSource();
		if (books) {
			books->AddEventSink(BooksReadEventHandler::GetSingleton());
			logger::info("Registered book read event handler"sv);
		}

		auto critHit = RE::CriticalHit::GetEventSource();
		if (critHit) {
			critHit->AddEventSink(CriticalHitEventHandler::GetSingleton());
			logger::info("Registered critical hit event handler"sv);
		}

		auto disarmed = RE::DisarmedEvent::GetEventSource();
		if (disarmed) {
			disarmed->AddEventSink(DisarmedEventHandler::GetSingleton());
			logger::info("Registered disarmed event handler"sv);
		}

		auto dragonSoulGained = RE::DragonSoulsGained::GetEventSource();
		if (dragonSoulGained) {
			dragonSoulGained->AddEventSink(DragonSoulsGainedEventHandler::GetSingleton());
			logger::info("Registered dragon souls gained event handler"sv);
		}

		auto itemHarvest = RE::TESHarvestedEvent::GetEventSource();
		if (itemHarvest) {
			itemHarvest->AddEventSink(ItemHarvestedEventHandler::GetSingleton());
			logger::info("Registered item harvested gained event handler"sv);
		}

		auto levelIncrease = RE::LevelIncrease::GetEventSource();
		if (levelIncrease) {
			levelIncrease->AddEventSink(LevelIncreaseEventHandler::GetSingleton());
			logger::info("Registered level increase event handler"sv);
		}

		auto locDiscovery = RE::LocationDiscovery::GetEventSource();
		if (locDiscovery) {
			locDiscovery->AddEventSink(LocationDiscoveryEventHandler::GetSingleton());
			logger::info("Registered location discovery event handler"sv);
		}

		auto shoutAttack = RE::ShoutAttack::GetEventSource();
		if (shoutAttack) {
			shoutAttack->AddEventSink(ShoutAttackEventHandler::GetSingleton());
			logger::info("Registered shout attack event handler"sv);
		}

		auto skillIncrease = RE::SkillIncrease::GetEventSource();
		if (skillIncrease) {
			skillIncrease->AddEventSink(SkillIncreaseEventHandler::GetSingleton());
			logger::info("Registered skill increase event handler"sv);
		}

		auto soulsTrapped = RE::SoulsTrapped::GetEventSource();
		if (soulsTrapped) {
			soulsTrapped->AddEventSink(SoulsTrappedEventHandler::GetSingleton());
			logger::info("Registered soul trap event handler"sv);
		}

		auto spellsLearned = RE::SpellsLearned::GetEventSource();
		if (spellsLearned) {
			spellsLearned->AddEventSink(SpellsLearnedEventHandler::GetSingleton());
			logger::info("Registered spells learned event handler"sv);
		}
	}
}
