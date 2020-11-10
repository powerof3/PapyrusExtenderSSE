#pragma once


namespace Serialization
{
	enum : std::uint32_t
	{
		kSerializationVersion = 2,

		kPapyrusExtender = 'P3PE',

		kAddPerks = 'APTN',
		kRemovePerks = 'RPFN',
		kAddKeywords = 'AKTF',
		kRemoveKeywords = 'RKOF',

		kOnCellFullyLoaded = 'CELL',
		kQuestStart = 'QSTR',
		kQuestStop = 'QSTP',
		kQuestStage = 'QSTG',
		kObjectLoaded = 'LOAD',
		kObjectUnloaded = 'UNLD',
		kGrab = 'GRAB',
		kRelease = 'RELS',

		kActorKill = 'KILL',
		kBookRead = 'BOOK',
		kCritHit = 'CRIT',
		kDisarm = 'DSRM',
		kDragonSoul = 'DRGN',
		kHarvest = 'HVST',
		kLevelIncrease = 'LEVL',
		kLocDiscovery = 'DISC',
		kSkillIncrease = 'SKIL',
		kSoulTrap = 'SOUL',
		kShoutAttack = 'SHOU',
		kSpellLearned = 'SPEL',

		kActorResurrect = 'RSUR',
		kActorReanimateStart = 'RSTA',
		kActorReanimateStop = 'REND',
		kWeatherChange = 'WEAT'
	};
	
	std::string DecodeTypeCode(std::uint32_t a_typeCode);
	
	void SaveCallback(SKSE::SerializationInterface* a_intfc);
	void LoadCallback(SKSE::SerializationInterface* a_intfc);
}