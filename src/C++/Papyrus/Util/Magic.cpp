#include "Magic.h"
#include "ConditionParser.h"

namespace MAGIC
{
	std::string_view get_archetype_as_string(Archetype a_archetype)
	{
		return archetypeMap.at(a_archetype);
	}

	namespace detail
	{
		bool effect_is_match(const RE::Effect* a_effect, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost)
		{
			return a_effect->baseEffect == a_mgef && numeric::essentially_equal(a_effect->effectItem.magnitude, a_mag) && a_effect->effectItem.area == a_area && a_effect->effectItem.duration == a_dur && numeric::essentially_equal(a_effect->cost, a_cost);
		}

        RE::Effect* get_match(RE::MagicItem* a_item, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost)
		{
            const auto it = std::ranges::find_if(a_item->effects,
				[&](const auto& effect) {
					return effect && effect_is_match(effect, a_mgef, a_mag, a_area, a_dur, a_cost);
				});
			return it != a_item->effects.end() ? *it : nullptr;
		}
    }

	void add_effect_item(RE::MagicItem* a_item, RE::MagicItem* a_copyItem, std::uint32_t a_index, float a_cost)
	{
		const auto copyEffect = a_copyItem->effects[a_index];
		auto effect = copyEffect ? new RE::Effect() : nullptr;

		if (effect) {
			effect->effectItem.magnitude = copyEffect->effectItem.magnitude;
			effect->effectItem.area = copyEffect->effectItem.area;
			effect->effectItem.duration = copyEffect->effectItem.duration;
			effect->baseEffect = copyEffect->baseEffect;
			effect->cost = a_cost == -1.0f ? copyEffect->cost : a_cost;
			effect->conditions = copyEffect->conditions;

			auto head = copyEffect->conditions.head;
			while (head) {
				auto newNode = new RE::TESConditionItem;
				if (newNode) {
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
		}
	}

	RESULT add_magic_effect(RE::MagicItem* a_item, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost, const std::vector<RE::BSFixedString>& a_conditionList)
	{
		auto effect = !detail::get_match(a_item, a_mgef, a_mag, a_area, a_dur, a_cost) ? new RE::Effect() : nullptr;
		if (effect) {
			effect->effectItem.magnitude = a_mag;
			effect->effectItem.area = a_area;
			effect->effectItem.duration = a_dur;
			effect->baseEffect = a_mgef;
			effect->cost = a_cost;

			if (!a_conditionList.empty() && !a_conditionList.front().empty()) {
				auto conditions = CONDITION::ParseConditions(a_conditionList);
				if (!conditions.empty()) {
					for (auto& [object, functionID, param1, param2, opCode, value, ANDOR] : conditions) {
						auto newNode = new RE::TESConditionItem;
						if (newNode) {
							newNode->next = nullptr;
							newNode->data.object = object;
							newNode->data.functionData.function = functionID;
							newNode->data.functionData.params[0] = param1;
							newNode->data.functionData.params[1] = param2;
							newNode->data.flags.opCode = opCode;
							newNode->data.comparisonValue.f = value;
							newNode->data.flags.isOR = ANDOR;

							if (effect->conditions.head == nullptr) {
								effect->conditions.head = newNode;
							} else {
								newNode->next = effect->conditions.head;
								effect->conditions.head = newNode;
							}
						}
					}
				} else {
					return RESULT::kFailParse;
				}
			}
			a_item->effects.push_back(effect);
			return RESULT::kSuccess;
		}
		return RESULT::kFailExists;
	}

	void remove_effect_item(RE::MagicItem* a_item, RE::MagicItem* a_copyItem, std::uint32_t a_index)
	{
        const auto copyEffect = a_copyItem->effects[a_index];
		if (copyEffect && copyEffect->baseEffect) {
            const auto it = std::ranges::find_if(a_item->effects,
				[&](const auto& effect) { return effect && detail::effect_is_match(effect,
															   copyEffect->baseEffect,
															   copyEffect->effectItem.magnitude,
															   copyEffect->effectItem.area,
															   copyEffect->effectItem.duration,
															   copyEffect->cost); });

			if (it != a_item->effects.end()) {
				a_item->effects.erase(it);
			}
		}
	}

	void remove_magic_effect(RE::MagicItem* a_item, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost)
	{
        const auto it = std::ranges::find_if(a_item->effects,
			[&](const auto& effect) { return effect && detail::effect_is_match(effect, a_mgef, a_mag, a_area, a_dur, a_cost); });
		if (it != a_item->effects.end()) {
			a_item->effects.erase(it);
		}
	}
}
