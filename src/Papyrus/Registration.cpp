#include "Papyrus/Registration.h"

#include "Papyrus/PapyrusActor.h"
#include "Papyrus/PapyrusActorBase.h"
#include "Papyrus/PapyrusArray.h"
#include "Papyrus/PapyrusDebug.h"
#include "Papyrus/PapyrusEffectShader.h"
#include "Papyrus/PapyrusForm.h"
#include "Papyrus/PapyrusGame.h"
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

	papyrus->Register(papyrusActor::RegisterFuncs);
	_MESSAGE("Registered actor functions");

	papyrus->Register(papyrusActorBase::RegisterFuncs);
	_MESSAGE("Registered actorbase functions");

	papyrus->Register(papyrusArray::RegisterFuncs);
	_MESSAGE("Registered array functions");

	papyrus->Register(papyrusDebug::RegisterFuncs);
	_MESSAGE("Registered debug functions");

	papyrus->Register(papyrusEffectShader::RegisterFuncs);
	_MESSAGE("Registered effect shader functions");

	papyrus->Register(papyrusForm::RegisterFuncs);
	_MESSAGE("Registered form functions");

	papyrus->Register(papyrusGame::RegisterFuncs);
	_MESSAGE("Registered game functions");

	papyrus->Register(papyrusLight::RegisterFuncs);
	_MESSAGE("Registered light functions");

	papyrus->Register(papyrusLocation::RegisterFuncs);
	_MESSAGE("Registered location functions");

	papyrus->Register(papyrusMagicEffect::RegisterFuncs);
	_MESSAGE("Registered magiceffect functions");

	papyrus->Register(papyrusObjectReference::RegisterFuncs);
	_MESSAGE("Registered objectreference functions");

	papyrus->Register(papyrusPackage::RegisterFuncs);
	_MESSAGE("Registered package functions");

	papyrus->Register(papyrusProjectile::RegisterFuncs);
	_MESSAGE("Registered projectile functions");

	papyrus->Register(papyrusSound::RegisterFuncs);
	_MESSAGE("Registered sound functions");

	papyrus->Register(papyrusSpell::RegisterFuncs);
	_MESSAGE("Registered spell functions");

	papyrus->Register(papyrusStrings::RegisterFuncs);
	_MESSAGE("Registered string functions");

	papyrus->Register(papyrusUtility::RegisterFuncs);
	_MESSAGE("Registered utility functions");

	papyrus->Register(papyrusVisualEffect::RegisterFuncs);
	_MESSAGE("Registered visual effect functions");

	papyrus->Register(papyrusWeather::RegisterFuncs);
	_MESSAGE("Registered weather functions");
}
