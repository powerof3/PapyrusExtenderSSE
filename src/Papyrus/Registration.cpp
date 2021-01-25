#include "Papyrus/Registration.h"

#include "Papyrus/ActiveMagicEffect.h"
#include "Papyrus/Actor.h"
#include "Papyrus/ActorBase.h"
#include "Papyrus/Alias.h"
#include "Papyrus/ArmorAddon.h"
#include "Papyrus/Array.h"
#include "Papyrus/Cell.h"
#include "Papyrus/Debug.h"
#include "Papyrus/EffectShader.h"
#include "Papyrus/Events.h"
#include "Papyrus/ExtendedObjectTypes.h"
#include "Papyrus/Form.h"
#include "Papyrus/Furniture.h"
#include "Papyrus/Game.h"
#include "Papyrus/Hazard.h"
#include "Papyrus/Light.h"
#include "Papyrus/Location.h"
#include "Papyrus/MagicEffect.h"
#include "Papyrus/ObjectReference.h"
#include "Papyrus/Package.h"
#include "Papyrus/Projectile.h"
#include "Papyrus/Sound.h"
#include "Papyrus/Spell.h"
#include "Papyrus/Strings.h"
#include "Papyrus/Utility.h"
#include "Papyrus/VisualEffect.h"
#include "Papyrus/Weather.h"


namespace Papyrus
{
	void Functions::Register()
	{
		auto papyrus = SKSE::GetPapyrusInterface();

		logger::info("{:*^30}", "FUNCTIONS"sv);
		
		papyrus->Register(extendedObjectTypes::RegisterTypes);

		papyrus->Register(papyrusActiveMagicEffect::RegisterFuncs);
		logger::info("Registered activemagiceffect functions"sv);

		papyrus->Register(papyrusActor::RegisterFuncs);
		logger::info("Registered actor functions"sv);

		papyrus->Register(papyrusActorBase::RegisterFuncs);
		logger::info("Registered actorbase functions"sv);

		papyrus->Register(papyrusArmorAddon::RegisterFuncs);
		logger::info("Registered armor addon functions"sv);

		papyrus->Register(papyrusAlias::RegisterFuncs);
		logger::info("Registered alias functions"sv);

		papyrus->Register(papyrusArray::RegisterFuncs);
		logger::info("Registered array functions"sv);

		papyrus->Register(papyrusCell::RegisterFuncs);
		logger::info("Registered cell functions"sv);

		papyrus->Register(papyrusDebug::RegisterFuncs);
		logger::info("Registered debug functions"sv);

		papyrus->Register(papyrusEffectShader::RegisterFuncs);
		logger::info("Registered effect shader functions"sv);

		papyrus->Register(papyrusForm::RegisterFuncs);
		logger::info("Registered form functions"sv);

		papyrus->Register(papyrusFurniture::RegisterFuncs);
		logger::info("Registered furniture functions"sv);

		papyrus->Register(papyrusGame::RegisterFuncs);
		logger::info("Registered game functions"sv);

		papyrus->Register(papyrusHazard::RegisterFuncs);
		logger::info("Registered hazard functions"sv);

		papyrus->Register(papyrusLight::RegisterFuncs);
		logger::info("Registered light functions"sv);

		papyrus->Register(papyrusLocation::RegisterFuncs);
		logger::info("Registered location functions"sv);

		papyrus->Register(papyrusMagicEffect::RegisterFuncs);
		logger::info("Registered magiceffect functions"sv);

		papyrus->Register(papyrusObjectReference::RegisterFuncs);
		logger::info("Registered objectreference functions"sv);

		papyrus->Register(papyrusPackage::RegisterFuncs);
		logger::info("Registered package functions"sv);

		papyrus->Register(papyrusProjectile::RegisterFuncs);
		logger::info("Registered projectile functions"sv);

		papyrus->Register(papyrusSound::RegisterFuncs);
		logger::info("Registered sound functions"sv);

		papyrus->Register(papyrusSpell::RegisterFuncs);
		logger::info("Registered spell functions"sv);

		papyrus->Register(papyrusStrings::RegisterFuncs);
		logger::info("Registered string functions"sv);

		papyrus->Register(papyrusUtility::RegisterFuncs);
		logger::info("Registered utility functions"sv);

		papyrus->Register(papyrusVisualEffect::RegisterFuncs);
		logger::info("Registered visual effect functions"sv);

		papyrus->Register(papyrusWeather::RegisterFuncs);
		logger::info("Registered weather functions"sv);
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
