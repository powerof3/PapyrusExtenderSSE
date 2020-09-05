#include "Papyrus/Registration.h"

#include "Papyrus/ExtendedObjectTypes.h"
#include "Papyrus/PapyrusActor.h"
#include "Papyrus/PapyrusActorBase.h"
#include "Papyrus/PapyrusArmorAddon.h"
#include "Papyrus/PapyrusArray.h"
#include "Papyrus/PapyrusCell.h"
#include "Papyrus/PapyrusDebug.h"
#include "Papyrus/PapyrusEffectShader.h"
#include "Papyrus/PapyrusForm.h"
#include "Papyrus/PapyrusFurniture.h"
#include "Papyrus/PapyrusGame.h"
#include "Papyrus/PapyrusHazard.h"
#include "Papyrus/PapyrusLight.h"
#include "Papyrus/PapyrusLocation.h"
#include "Papyrus/PapyrusMagicEffect.h"
#include "Papyrus/PapyrusObjectReference.h"
#include "Papyrus/PapyrusPackage.h"
#include "Papyrus/PapyrusProjectile.h"
#include "Papyrus/PapyrusSound.h"
#include "Papyrus/PapyrusSpell.h"
#include "Papyrus/PapyrusStrings.h"
#include "Papyrus/PapyrusUtility.h"
#include "Papyrus/PapyrusVisualEffect.h"
#include "Papyrus/PapyrusWeather.h"


void Papyrus::Register()
{
	auto papyrus = SKSE::GetPapyrusInterface();

	papyrus->Register(extendedObjectTypes::RegisterTypes);
	logger::info("Registered actor functions");

	papyrus->Register(papyrusActor::RegisterFuncs);
	logger::info("Registered actor functions");

	papyrus->Register(papyrusActorBase::RegisterFuncs);
	logger::info("Registered actorbase functions");

	papyrus->Register(papyrusArmorAddon::RegisterFuncs);
	logger::info("Registered armor addon functions");

	papyrus->Register(papyrusArray::RegisterFuncs);
	logger::info("Registered array functions");

	papyrus->Register(papyrusCell::RegisterFuncs);
	logger::info("Registered cell functions");

	papyrus->Register(papyrusDebug::RegisterFuncs);
	logger::info("Registered debug functions");

	papyrus->Register(papyrusEffectShader::RegisterFuncs);
	logger::info("Registered effect shader functions");

	papyrus->Register(papyrusForm::RegisterFuncs);
	logger::info("Registered form functions");

	papyrus->Register(papyrusFurniture::RegisterFuncs);
	logger::info("Registered furniture functions");

	papyrus->Register(papyrusGame::RegisterFuncs);
	logger::info("Registered game functions");

	papyrus->Register(papyrusHazard::RegisterFuncs);
	logger::info("Registered hazard functions");

	papyrus->Register(papyrusLight::RegisterFuncs);
	logger::info("Registered light functions");

	papyrus->Register(papyrusLocation::RegisterFuncs);
	logger::info("Registered location functions");

	papyrus->Register(papyrusMagicEffect::RegisterFuncs);
	logger::info("Registered magiceffect functions");

	papyrus->Register(papyrusObjectReference::RegisterFuncs);
	logger::info("Registered objectreference functions");

	papyrus->Register(papyrusPackage::RegisterFuncs);
	logger::info("Registered package functions");

	papyrus->Register(papyrusProjectile::RegisterFuncs);
	logger::info("Registered projectile functions");

	papyrus->Register(papyrusSound::RegisterFuncs);
	logger::info("Registered sound functions");

	papyrus->Register(papyrusSpell::RegisterFuncs);
	logger::info("Registered spell functions");

	papyrus->Register(papyrusStrings::RegisterFuncs);
	logger::info("Registered string functions");

	papyrus->Register(papyrusUtility::RegisterFuncs);
	logger::info("Registered utility functions");

	papyrus->Register(papyrusVisualEffect::RegisterFuncs);
	logger::info("Registered visual effect functions");

	papyrus->Register(papyrusWeather::RegisterFuncs);
	logger::info("Registered weather functions");
}
