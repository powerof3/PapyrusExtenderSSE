#pragma once

#include "Registration/DataMap.h"
#include "Registration/DataSet.h"

namespace FORM
{
	enum : std::uint32_t
	{
		kRemove = 0,
		kAdd = 1
	};

	class KeywordManager final :
		public ISingleton<KeywordManager>,
		public FormMapPair<RE::TESForm, RE::BGSKeyword>
	{
		bool Process(RE::TESForm* a_form, RE::BGSKeyword* a_data, std::uint32_t a_index) override
		{
			if (const auto keywordForm = a_form->As<RE::BGSKeywordForm>(); keywordForm) {
				return a_index == FORM::kAdd ?
                           keywordForm->AddKeyword(a_data) :
                           keywordForm->RemoveKeyword(a_data);
			}
			return false;
		}
	};

	class PerkManager final :
		public ISingleton<PerkManager>,
		public FormMapPair<RE::Actor, RE::BGSPerk>
	{
		bool Process(RE::Actor* a_form, RE::BGSPerk* a_data, std::uint32_t a_index) override
		{
			bool success = false;

			if (const auto actorbase = a_form->GetActorBase(); actorbase) {
				success = a_index == FORM::kAdd ?
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
	};
}

namespace DETECTION
{
	enum : std::uint32_t
	{
		kAlert = 0,
		kHide = 1,
	};

	//target
	class TargetManager final :
		public ISingleton<TargetManager>,
		public FormSetPair<RE::Actor>
	{};

	//searcher
	class SourceManager final :
		public ISingleton<SourceManager>,
		public FormSetPair<RE::Actor>
	{};
}

namespace MAGIC
{
	struct MGEFData
	{
		bool operator<(const MGEFData& a_rhs) const { return mgef.second < a_rhs.mgef.second; }

		// members
		std::pair<RE::EffectSetting*, RE::FormID> mgef;
		float                                     mag;
		std::uint32_t                             area;
		std::uint32_t                             dur;
		float                                     cost;
		std::vector<std::string>                  conditionList;
	};

	class MGEFManager final :
		public ISingleton<MGEFManager>,
		public FormDataMapPair<RE::MagicItem, MGEFData>
	{
	public:
		bool Save_Impl(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index) override
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
					logger::error("Failed to save data set count ({})", numData);
					return false;
				}
				for (auto& data : dataSet) {
					if (!a_intfc->WriteRecordData(data.mgef.second)) {
						logger::error("Failed to save effect formID ({:X})", data.mgef.second);
						return false;
					}
					if (!a_intfc->WriteRecordData(data.mag)) {
						logger::error("Failed to save effect magnitude ({})", data.mag);
						return false;
					}
					if (!a_intfc->WriteRecordData(data.area)) {
						logger::error("Failed to save effect area ({})", data.area);
						return false;
					}
					if (!a_intfc->WriteRecordData(data.dur)) {
						logger::error("Failed to save effect duration ({})", data.dur);
						return false;
					}
					if (!a_intfc->WriteRecordData(data.cost)) {
						logger::error("Failed to save effect cost ({})", data.cost);
						return false;
					}
					const std::size_t numConditions = data.conditionList.size();
					if (!a_intfc->WriteRecordData(numConditions)) {
						logger::error("Failed to save number of conditions ({})", numConditions);
						return false;
					}
					for (auto& condition : data.conditionList) {
						stl::write_string(a_intfc, condition);
					}
				}
			}

			return true;
		}
		bool Load(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index) override
		{
			assert(a_intfc);
			std::size_t numRegs;
			a_intfc->ReadRecordData(numRegs);

			Locker locker(_lock);

			auto& formMap = GetData(a_index);
			formMap.clear();

			for (std::size_t i = 0; i < numRegs; i++) {
				RE::FormID formID;
				if (!stl::read_formID(a_intfc, formID)) {
					logger::warn("{} : Failed to resolve formID {:X}"sv, i, formID);
					continue;
				}
				std::size_t numData;
				a_intfc->ReadRecordData(numData);
				for (std::size_t j = 0; j < numData; j++) {
					MGEFData data;
					if (!stl::read_formID(a_intfc, data.mgef.second)) {
						logger::warn("{} : Failed to resolve effect formID {:X}"sv, i, data.mgef.second);
						continue;
					}
					if (!a_intfc->ReadRecordData(data.mag)) {
						logger::warn("Failed to resolve magnitude ({})", data.mag);
						continue;
					}
					if (!a_intfc->ReadRecordData(data.area)) {
						logger::warn("Failed to read area ({})", data.area);
						continue;
					}
					if (!a_intfc->ReadRecordData(data.dur)) {
						logger::error("Failed to read duration ({})", data.dur);
						continue;
					}
					if (!a_intfc->ReadRecordData(data.cost)) {
						logger::warn("Failed to resolve cost ({})", data.cost);
						continue;
					}
					std::size_t numConditions;
					a_intfc->ReadRecordData(numConditions);
					for (std::size_t k = 0; k < numConditions; k++) {
						std::string str;
						if (!stl::read_string(a_intfc, str)) {
							break;
						}
						data.conditionList.emplace_back(str);
					}
					formMap[formID].insert(std::move(data));
				}
			}

			for (auto& [dataID, dataSet] : formMap) {
				const auto form = RE::TESForm::LookupByID(dataID);
				if (const auto magicItem = form ? form->As<RE::MagicItem>() : nullptr; magicItem) {
					for (auto effectData : dataSet) {
						auto& [mgef, mgefID] = effectData.mgef;
						mgef = RE::TESForm::LookupByID<RE::EffectSetting>(mgefID);
						if (mgef) {
							Process(magicItem, effectData, a_index);
						}
					}
				}
			}

			return true;
		}

	private:
		struct detail
		{
			static bool effect_is_match(const RE::Effect* a_effect, const MGEFData& a_data)
			{
				return a_effect->baseEffect == a_data.mgef.first &&
				       numeric::essentially_equal(a_effect->effectItem.magnitude, a_data.mag) &&
				       a_effect->effectItem.area == a_data.area &&
				       a_effect->effectItem.duration == a_data.dur &&
				       numeric::essentially_equal(a_effect->cost, a_data.cost);
			}

			static RE::Effect* get_match(RE::MagicItem* a_item, const MGEFData& a_data)
			{
				const auto it = std::ranges::find_if(a_item->effects,
					[&](const auto& effect) {
						return effect && effect_is_match(effect, a_data);
					});
				return it != a_item->effects.end() ? *it : nullptr;
			}

			static bool add_magic_effect(RE::MagicItem* a_item, const MGEFData& a_data)
			{
				if (const auto effect = !detail::get_match(a_item, a_data) ? new RE::Effect() : nullptr) {
					effect->effectItem.magnitude = a_data.mag;
					effect->effectItem.area = a_data.area;
					effect->effectItem.duration = a_data.dur;
					effect->baseEffect = a_data.mgef.first;
					effect->cost = a_data.cost;

					if (!a_data.conditionList.empty() && !a_data.conditionList.front().empty()) {
						if (const auto conditions = CONDITION::ParseConditionList(a_data.conditionList); !conditions.empty()) {
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

			static bool remove_magic_effect(RE::MagicItem* a_item, const MGEFData& a_data)
			{
				const auto it = std::ranges::find_if(a_item->effects,
					[&](const auto& effect) { return effect && detail::effect_is_match(effect, a_data); });
				if (it != a_item->effects.end()) {
					a_item->effects.erase(it);
					return true;
				}
				return false;
			}
		};

		bool Process(RE::MagicItem* a_form, const MGEFData& a_data, std::uint32_t a_index) override
		{
			return a_index == FORM::kAdd ?
                       detail::add_magic_effect(a_form, a_data) :
                       detail::remove_magic_effect(a_form, a_data);
		}
	};

	struct EffectData
	{
		bool operator<(const EffectData& a_rhs) const { return magicItem.second < a_rhs.magicItem.second; }

		// members
		std::pair<RE::MagicItem*, RE::FormID> magicItem;
		std::uint32_t                         index;
		float                                 cost;
	};

	class EffectManager final :
		public ISingleton<EffectManager>,
		public FormDataMapPair<RE::MagicItem, EffectData>
	{
	public:
		bool Save_Impl(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index) override
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
					if (!a_intfc->WriteRecordData(data.magicItem.second)) {
						logger::error("Failed to save effect formID ({:X})", data.magicItem.second);
						return false;
					}
					if (!a_intfc->WriteRecordData(data.index)) {
						logger::error("Failed to save index ({})", data.index);
						return false;
					}
					if (!a_intfc->WriteRecordData(data.cost)) {
						logger::error("Failed to save cost ({})", data.cost);
						return false;
					}
				}
			}

			return true;
		}
		bool Load(SKSE::SerializationInterface* a_intfc, std::uint32_t a_index) override
		{
			assert(a_intfc);
			std::size_t numRegs;
			a_intfc->ReadRecordData(numRegs);

			Locker locker(_lock);

			auto& formMap = GetData(a_index);
			formMap.clear();

			RE::FormID  formID;
			std::size_t numData;
			EffectData  data;

			for (std::size_t i = 0; i < numRegs; i++) {
				if (!stl::read_formID(a_intfc, formID)) {
					logger::warn("{} : Failed to resolve formID {:X}"sv, i, formID);
					continue;
				}
				a_intfc->ReadRecordData(numData);
				for (std::size_t j = 0; j < numData; j++) {
					if (!stl::read_formID(a_intfc, data.magicItem.second)) {
						logger::warn("{} : Failed to resolve effect formID {:X}"sv, i, data.magicItem.second);
						continue;
					}
					if (!a_intfc->ReadRecordData(data.index)) {
						logger::warn("Failed to resolve index ({})", data.index);
						continue;
					}
					if (!a_intfc->ReadRecordData(data.cost)) {
						logger::warn("Failed to read cost ({})", data.cost);
						continue;
					}
					formMap[formID].insert(data);
				}
			}

			for (auto& [dataID, dataSet] : formMap) {
				const auto form = RE::TESForm::LookupByID(dataID);
				if (const auto magicItem = form ? form->As<RE::MagicItem>() : nullptr) {
					for (auto effectData : dataSet) {
						auto& [copyItem, copyItemID] = effectData.magicItem;
						copyItem = RE::TESForm::LookupByID<RE::MagicItem>(copyItemID);
						if (copyItem && effectData.index < magicItem->effects.size()) {
							Process(magicItem, effectData, a_index);
						}
					}
				}
			}

			return true;
		}

	private:
		struct detail
		{
			static bool effect_is_match(const RE::Effect* a_effect, const RE::Effect* a_copyEffect)
			{
				return a_effect->baseEffect == a_copyEffect->baseEffect &&
				       numeric::essentially_equal(a_effect->effectItem.magnitude, a_copyEffect->effectItem.magnitude) &&
				       a_effect->effectItem.area == a_copyEffect->effectItem.area &&
				       a_effect->effectItem.duration == a_copyEffect->effectItem.duration &&
				       numeric::essentially_equal(a_effect->cost, a_copyEffect->cost);
			}

			static bool add_effect_item(RE::MagicItem* a_item, const EffectData& a_data)
			{
				const auto copyEffect = a_data.magicItem.first->effects[a_data.index];
				auto       effect = copyEffect ? new RE::Effect() : nullptr;

				if (effect) {
					effect->effectItem.magnitude = copyEffect->effectItem.magnitude;
					effect->effectItem.area = copyEffect->effectItem.area;
					effect->effectItem.duration = copyEffect->effectItem.duration;
					effect->baseEffect = copyEffect->baseEffect;
					effect->cost = a_data.cost == -1.0f ? copyEffect->cost : a_data.cost;
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

			static bool remove_effect_item(RE::MagicItem* a_item, const EffectData& a_data)
			{
				if (const auto copyEffect = a_data.magicItem.first->effects[a_data.index]; copyEffect) {
					const auto it = std::ranges::find_if(a_item->effects,
						[&](const auto& effect) { return effect && detail::effect_is_match(effect, copyEffect); });

					if (it != a_item->effects.end()) {
						a_item->effects.erase(it);

						return true;
					}
				}

				return false;
			}
		};

		bool Process(RE::MagicItem* a_form, const EffectData& a_data, std::uint32_t a_index) override
		{
			return a_index == FORM::kAdd ?
                       detail::add_effect_item(a_form, a_data) :
                       detail::remove_effect_item(a_form, a_data);
		}
	};
}
