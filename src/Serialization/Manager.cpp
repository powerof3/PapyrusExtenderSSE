#include "Serialization/Manager.h"

#include "Papyrus/Util/ConditionParser.h"
#include "Serialization/EventHolder.h"

namespace Serialization
{
	bool KeywordHolder::Process(RE::TESForm* a_form, RE::BGSKeyword* a_data, std::uint32_t a_index)
	{
		if (const auto keywordForm = a_form->As<RE::BGSKeywordForm>(); keywordForm) {
			return a_index == FORM::kAdd ?
			           keywordForm->AddKeyword(a_data) :
			           keywordForm->RemoveKeyword(a_data);
		}
		return false;
	}

	bool PerkHolder::Process(RE::Actor* a_form, RE::BGSPerk* a_data, std::uint32_t a_index)
	{
		bool success = false;

		if (const auto actorbase = a_form->GetActorBase(); actorbase) {
			success = (a_index == FORM::kAdd) ?
			              actorbase->AddPerk(a_data, 1) :
			              actorbase->RemovePerk(a_data);
			if (success) {
				for (const auto& perkEntry : a_data->perkEntries) {
					if (perkEntry) {
						a_index == FORM::kAdd ?
							perkEntry->ApplyPerkEntry(a_form) :
							perkEntry->RemovePerkEntry(a_form);
					}
				}
				a_form->OnArmorActorValueChanged();
				if (const auto invChanges = a_form->GetInventoryChanges()) {
					invChanges->armorWeight = invChanges->totalWeight;
					invChanges->totalWeight = -1.0f;
					a_form->equippedWeight = -1.0f;
				}
			}
		}

		return success;
	}

	bool MGEFData::equals(RE::Effect* a_effect) const
	{
		return mgef == a_effect->baseEffect &&
		       numeric::essentially_equal(mag, a_effect->effectItem.magnitude) &&
		       area == a_effect->effectItem.area &&
		       dur == a_effect->effectItem.duration &&
		       numeric::essentially_equal(cost, a_effect->cost);
	}

	RE::Effect* MGEFData::get_matching_effect(RE::MagicItem* a_item) const
	{
		const auto it = std::ranges::find_if(a_item->effects,
			[this](const auto& a_effect) {
				return a_effect && this->equals(a_effect);
			});
		return it != a_item->effects.cend() ? *it : nullptr;
	}

	bool MGEFData::add_magic_effect(RE::MagicItem* a_item) const
	{
		if (get_matching_effect(a_item)) {
			return false;
		}

		if (auto effect = new RE::Effect()) {
			effect->effectItem.magnitude = mag;
			effect->effectItem.area = area;
			effect->effectItem.duration = dur;
			effect->baseEffect = mgef;
			effect->cost = cost;

			if (!conditionList.empty() && !conditionList.front().empty()) {
				if (const auto conditions = CONDITION::ParseConditionList(conditionList); !conditions.empty()) {
					for (auto& conditionData : conditions) {
						if (const auto newNode = new RE::TESConditionItem) {
							newNode->next = nullptr;
							newNode->data = conditionData;

							if (effect->conditions.head == nullptr) {
								effect->conditions.head = newNode;
							} else {
								newNode->next = effect->conditions.head;
								effect->conditions.head = newNode;
							}
						}
					}
				}
			}
			a_item->effects.push_back(effect);
			return true;
		}
		return false;
	}

	bool MGEFData::remove_magic_effect(RE::MagicItem* a_item) const
	{
		const auto it = std::ranges::find_if(a_item->effects,
			[this](const auto& effect) { return effect && this->equals(effect); });
		if (it != a_item->effects.end()) {
			a_item->effects.erase(it);
			return true;
		}
		return false;
	}

	bool MGEFData::save(SKSE::SerializationInterface* a_intfc) const
	{
		if (!a_intfc->WriteRecordData(mgefFormID)) {
			logger::error("Failed to save effect formID ({:X})", mgefFormID);
			return false;
		}
		if (!a_intfc->WriteRecordData(mag)) {
			logger::error("Failed to save effect magnitude ({})", mag);
			return false;
		}
		if (!a_intfc->WriteRecordData(area)) {
			logger::error("Failed to save effect area ({})", area);
			return false;
		}
		if (!a_intfc->WriteRecordData(dur)) {
			logger::error("Failed to save effect duration ({})", dur);
			return false;
		}
		if (!a_intfc->WriteRecordData(cost)) {
			logger::error("Failed to save effect cost ({})", cost);
			return false;
		}
		const std::size_t numConditions = conditionList.size();
		if (!a_intfc->WriteRecordData(numConditions)) {
			logger::error("Failed to save number of conditions ({})", numConditions);
			return false;
		}
		for (auto& condition : conditionList) {
			stl::write_string(a_intfc, condition);
		}
		return true;
	}

	bool MGEFData::load(SKSE::SerializationInterface* a_intfc, std::size_t index)
	{
		bool success = true;
		if (!stl::read_formID(a_intfc, mgefFormID)) {
			logger::warn("{} : Failed to resolve effect formID {:X}"sv, index, mgefFormID);
			success = false;
		}
		if (!a_intfc->ReadRecordData(mag)) {
			logger::warn("Failed to resolve magnitude ({})", mag);
			success = false;
		}
		if (!a_intfc->ReadRecordData(area)) {
			logger::warn("Failed to read area ({})", area);
			success = false;
		}
		if (!a_intfc->ReadRecordData(dur)) {
			logger::error("Failed to read duration ({})", dur);
			success = false;
		}
		if (!a_intfc->ReadRecordData(cost)) {
			logger::warn("Failed to resolve cost ({})", cost);
			success = false;
		}
		std::size_t numConditions;
		a_intfc->ReadRecordData(numConditions);
		for (std::size_t k = 0; k < numConditions; k++) {
			std::string str;
			if (stl::read_string(a_intfc, str)) {
				conditionList.emplace_back(str);
			} else {
				success = false;
			}
		}
		return success;
	}

	bool EffectData::equals(const RE::Effect* a_effect, const RE::Effect* a_copyEffect) const
	{
		return a_effect->baseEffect == a_copyEffect->baseEffect &&
		       numeric::essentially_equal(a_effect->effectItem.magnitude, a_copyEffect->effectItem.magnitude) &&
		       a_effect->effectItem.area == a_copyEffect->effectItem.area &&
		       a_effect->effectItem.duration == a_copyEffect->effectItem.duration &&
		       numeric::essentially_equal(a_effect->cost, a_copyEffect->cost);
	}

	bool EffectData::add_effect_item(RE::MagicItem* a_item) const
	{
		const auto copyEffect = magicItem->effects[index];
		if (!copyEffect) {
			return false;
		}

		auto effect = new RE::Effect();

		if (effect) {
			effect->effectItem.magnitude = copyEffect->effectItem.magnitude;
			effect->effectItem.area = copyEffect->effectItem.area;
			effect->effectItem.duration = copyEffect->effectItem.duration;
			effect->baseEffect = copyEffect->baseEffect;
			effect->cost = cost == -1.0f ? copyEffect->cost : cost;
			effect->conditions = copyEffect->conditions;

			auto head = copyEffect->conditions.head;
			while (head) {
				if (const auto newNode = new RE::TESConditionItem) {
					newNode->next = nullptr;
					newNode->data.comparisonValue = head->data.comparisonValue;
					newNode->data.runOnRef = head->data.runOnRef;
					newNode->data.object = head->data.object;
					newNode->data.dataID = head->data.dataID;
					newNode->data.functionData.function = head->data.functionData.function;
					newNode->data.functionData.params[0] = head->data.functionData.params[0];
					newNode->data.functionData.params[1] = head->data.functionData.params[1];
					newNode->data.flags = head->data.flags;
					newNode->data.object = head->data.object;

					if (effect->conditions.head == nullptr) {
						effect->conditions.head = newNode;
					} else {
						newNode->next = effect->conditions.head;
						effect->conditions.head = newNode;
					}
				}
				head = head->next;
			}
			a_item->effects.push_back(effect);

			return true;
		}

		return false;
	}

	bool EffectData::remove_effect_item(RE::MagicItem* a_item) const
	{
		if (const auto copyEffect = magicItem->effects[index]; copyEffect) {
			const auto it = std::ranges::find_if(a_item->effects,
				[&](const auto& effect) { return effect && this->equals(effect, copyEffect); });
			if (it != a_item->effects.end()) {
				a_item->effects.erase(it);

				return true;
			}
		}

		return false;
	}

	bool EffectData::save(SKSE::SerializationInterface* a_intfc) const
	{
		if (!a_intfc->WriteRecordData(magicItemFormID)) {
			logger::error("Failed to save effect formID ({:X})", magicItemFormID);
			return false;
		}
		if (!a_intfc->WriteRecordData(index)) {
			logger::error("Failed to save index ({})", index);
			return false;
		}
		if (!a_intfc->WriteRecordData(cost)) {
			logger::error("Failed to save cost ({})", cost);
			return false;
		}
		return true;
	}

	bool EffectData::load(SKSE::SerializationInterface* a_intfc, std::size_t a_index)
	{
		bool success = true;
		if (!stl::read_formID(a_intfc, magicItemFormID)) {
			logger::warn("{} : Failed to resolve effect formID {:X}"sv, a_index, magicItemFormID);
			success = false;
		}
		if (!a_intfc->ReadRecordData(index)) {
			logger::warn("Failed to resolve index ({})", index);
			success = false;
		}
		if (!a_intfc->ReadRecordData(cost)) {
			logger::warn("Failed to read cost ({})", cost);
			success = false;
		}
		return success;
	}

	void MGEFHolder::ProcessEntry(const RE::FormID a_key, std::vector<MGEFData>& a_data, std::uint32_t a_index)
	{
		const auto form = RE::TESForm::LookupByID(a_key);
		if (const auto magicItem = form ? form->As<RE::MagicItem>() : nullptr; magicItem) {
			for (auto& effectData : a_data) {
				effectData.mgef = RE::TESForm::LookupByID<RE::EffectSetting>(effectData.mgefFormID);
				if (effectData.mgef) {
					Process(magicItem, effectData, a_index);
				}
			}
		}
	}

	bool MGEFHolder::SaveImpl(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index) const
	{
		assert(a_intfc);
		Locker locker(_lock);

		auto& formMap = GetData(a_index);

		const std::size_t numRegs = formMap.size();
		if (!a_intfc->WriteRecordData(numRegs)) {
			logger::error("Failed to save reg count ({})", numRegs);
			return false;
		}

		for (const auto& [formID, dataSet] : formMap) {
			if (!a_intfc->WriteRecordData(formID)) {
				logger::error("Failed to save formID ({:X})", formID);
				return false;
			}
			const std::size_t numData = dataSet.size();
			if (!a_intfc->WriteRecordData(numData)) {
				logger::error("Failed to save data set count ({})", numData);
				return false;
			}
			for (auto& data : dataSet) {
				if (!data.save(a_intfc)) {
					return false;
				}
			}
		}

		return true;
	}

	bool MGEFHolder::Process(RE::MagicItem* a_form, const MGEFData& a_data, std::uint32_t a_index)
	{
		return a_index == FORM::kAdd ?
		           a_data.add_magic_effect(a_form) :
		           a_data.remove_magic_effect(a_form);
	}

	void EffectHolder::ProcessEntry(const RE::FormID a_key, std::vector<EffectData>& a_data, std::uint32_t a_index)
	{
		const auto form = RE::TESForm::LookupByID(a_key);
		if (const auto magicItem = form ? form->As<RE::MagicItem>() : nullptr) {
			for (auto& effectData : a_data) {
				effectData.magicItem = RE::TESForm::LookupByID<RE::MagicItem>(effectData.magicItemFormID);
				if (effectData.magicItem && effectData.index < magicItem->effects.size()) {
					Process(magicItem, effectData, a_index);
				}
			}
		}
	}

	bool EffectHolder::SaveImpl(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index) const
	{
		assert(a_intfc);
		Locker locker(_lock);

		auto&             formMap = GetData(a_index);
		const std::size_t numRegs = formMap.size();
		if (!a_intfc->WriteRecordData(numRegs)) {
			logger::error("Failed to save reg count ({})", numRegs);
			return false;
		}

		for (const auto& [formID, dataSet] : formMap) {
			if (!a_intfc->WriteRecordData(formID)) {
				logger::error("Failed to save formID ({:X})", formID);
				return false;
			}
			const std::size_t numData = dataSet.size();
			if (!a_intfc->WriteRecordData(numData)) {
				logger::error("Failed to save data reg count ({})", numData);
				return false;
			}
			for (auto& data : dataSet) {
				if (!data.save(a_intfc)) {
					return false;
				}
			}
		}

		return true;
	}

	bool EffectHolder::Process(RE::MagicItem* a_form, const EffectData& a_data, std::uint32_t a_index)
	{
		return a_index == FORM::kAdd ?
		           a_data.add_effect_item(a_form) :
		           a_data.remove_effect_item(a_form);
	}

	std::string Manager::DecodeTypeCode(std::uint32_t a_typeCode)
	{
		constexpr std::size_t SIZE = sizeof(std::uint32_t);

		std::string sig;
		sig.resize(SIZE);
		const char* iter = reinterpret_cast<char*>(&a_typeCode);
		for (std::size_t i = 0, j = SIZE - 2; i < SIZE - 1; ++i, --j) {
			sig[j] = iter[i];
		}

		return sig;
	}

	void Manager::Register()
	{
		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(GetSingleton());
		logger::info("Registered form deletion event handler"sv);
	}

	void Manager::Save(SKSE::SerializationInterface* a_intfc)
	{
		Event::ScriptEventHolder::GetSingleton()->Save(a_intfc, kSerializationVersion);
		Event::StoryEventHolder::GetSingleton()->Save(a_intfc, kSerializationVersion);
		Event::GameEventHolder::GetSingleton()->Save(a_intfc, kSerializationVersion);

		save(perks, a_intfc);
		save(keywords, a_intfc);
		save(detectionTargets, a_intfc);
		save(detectionSources, a_intfc);
		save(mgefs, a_intfc);
		save(effectItems, a_intfc);

		logger::info("Finished saving data"sv);
	}

	void Manager::Load(SKSE::SerializationInterface* a_intfc)
	{
		std::uint32_t type;
		std::uint32_t version;
		std::uint32_t length;
		while (a_intfc->GetNextRecordInfo(type, version, length)) {
			if (version != kSerializationVersion) {
				logger::critical("Loaded data is out of date! Read ({}), expected ({}) for type code ({})", version, std::to_underlying(kSerializationVersion), DecodeTypeCode(type));
				continue;
			}
			switch (type) {
			case kAddPerks:
				load(perks, a_intfc, FORM::kAdd);
				break;
			case kRemovePerks:
				load(perks, a_intfc, FORM::kRemove);
				break;
			case kAddKeywords:
				load(keywords, a_intfc, FORM::kAdd);
				break;
			case kRemoveKeywords:
				load(keywords, a_intfc, FORM::kRemove);
				break;
			case kTargetHide:
				load(detectionTargets, a_intfc, DETECTION::kHide);
				break;
			case kTargetAlert:
				load(detectionTargets, a_intfc, DETECTION::kAlert);
				break;
			case kSourceHide:
				load(detectionSources, a_intfc, DETECTION::kHide);
				break;
			case kSourceAlert:
				load(detectionSources, a_intfc, DETECTION::kAlert);
				break;
			case kAddMGEF:
				load(mgefs, a_intfc, FORM::kAdd);
				break;
			case kRemoveMGEF:
				load(mgefs, a_intfc, FORM::kRemove);
				break;
			case kAddEffect:
				load(effectItems, a_intfc, FORM::kAdd);
				break;
			case kRemoveEffect:
				load(effectItems, a_intfc, FORM::kRemove);
				break;
			default:
				{
					Event::ScriptEventHolder::GetSingleton()->Load(a_intfc, type);
					Event::StoryEventHolder::GetSingleton()->Load(a_intfc, type);
					Event::GameEventHolder::GetSingleton()->Load(a_intfc, type);
				}
				break;
			}
		}
		logger::info("Finished loading data"sv);
	}

	void Manager::Revert(SKSE::SerializationInterface* a_intfc)
	{
		Event::ScriptEventHolder::GetSingleton()->Revert(a_intfc);
		Event::StoryEventHolder::GetSingleton()->Revert(a_intfc);
		Event::GameEventHolder::GetSingleton()->Revert(a_intfc);

		keywords.Revert(a_intfc);
		perks.Revert(a_intfc);
		detectionTargets.Revert(a_intfc);
		detectionSources.Revert(a_intfc);
		mgefs.Revert(a_intfc);
		effectItems.Revert(a_intfc);

		logger::info("Finished reverting data"sv);
	}

	void Manager::FormDelete(RE::VMHandle a_handle)
	{
		Event::ScriptEventHolder::GetSingleton()->FormDelete(a_handle);
		Event::StoryEventHolder::GetSingleton()->FormDelete(a_handle);
		Event::GameEventHolder::GetSingleton()->FormDelete(a_handle);
	}

	RE::BSEventNotifyControl Manager::ProcessEvent(const RE::TESFormDeleteEvent* a_event, RE::BSTEventSource<RE::TESFormDeleteEvent>*)
	{
		if (a_event && a_event->formID != 0) {
			const auto formID = a_event->formID;

			keywords.Remove(formID);
			perks.Remove(formID);
			detectionTargets.Remove(formID);
			detectionSources.Remove(formID);

			Event::ScriptEventHolder::GetSingleton()->FormDelete(formID);
			Event::GameEventHolder::GetSingleton()->FormDelete(formID);
		}

		return RE::BSEventNotifyControl::kContinue;
	}

	void SaveCallback(SKSE::SerializationInterface* a_intfc)
	{
		Manager::GetSingleton()->Save(a_intfc);
	}

	void LoadCallback(SKSE::SerializationInterface* a_intfc)
	{
		Manager::GetSingleton()->Load(a_intfc);
	}

	void RevertCallback(SKSE::SerializationInterface* a_intfc)
	{
		Manager::GetSingleton()->Revert(a_intfc);
	}

	void FormDeleteCallback(RE::VMHandle a_handle)
	{
		Manager::GetSingleton()->FormDelete(a_handle);
	}
}
