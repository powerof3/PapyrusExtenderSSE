#include "Papyrus/Enchantment.h"

#include "Util/ConditionParser.h"


void papyrusEnchantment::AddMagicEffectToEnchantment(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EnchantmentItem* a_enchantment, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost, std::vector<RE::BSFixedString> a_conditionList)
{
	if (!a_enchantment) {
		a_vm->TraceStack("Enchantment is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_mgef) {
		a_vm->TraceStack("MagicEffect is None", a_stackID, Severity::kWarning);
		return;
	}
	if (a_mgef->data.castingType != a_enchantment->data.castingType) {
		a_vm->TraceStack("Casting types don't match", a_stackID, Severity::kWarning);
		return;
	}
	if (a_mgef->data.delivery != a_enchantment->data.delivery) {
		a_vm->TraceStack("Delivery types don't match", a_stackID, Severity::kWarning);
		return;
	}

	auto effect = a_enchantment->GetEffectIsMatch(a_mgef, a_mag, a_area, a_dur, a_cost);
	if (!effect) {
		effect = new RE::Effect();
		if (effect) {
			effect->effectItem.magnitude = a_mag;
			effect->effectItem.area = a_area;
			effect->effectItem.duration = a_dur;
			effect->baseEffect = a_mgef;
			effect->cost = a_cost;

			if (!a_conditionList.empty() && !a_conditionList.front().empty()) {
				auto conditions = Condition::ParseConditions(a_conditionList);
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
					a_vm->TraceStack("Failed to parse condition list", a_stackID, Severity::kInfo);
				}
			}
			a_enchantment->effects.push_back(effect);
		}
	}
}


auto papyrusEnchantment::GetEnchantmentType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EnchantmentItem* a_enchantment) -> std::int32_t
{
	if (!a_enchantment) {
		a_vm->TraceStack("Enchantment is None", a_stackID, Severity::kWarning);
		return -1;
	}

	return to_underlying(a_enchantment->data.spellType);
}



void papyrusEnchantment::AddEffectItemToEnchantment(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EnchantmentItem* a_enchantment, RE::EnchantmentItem* a_copyEnchantment, std::uint32_t a_index)
{
	if (!a_enchantment) {
		a_vm->TraceStack("Enchantment is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_copyEnchantment) {
		a_vm->TraceStack("Copy Enchantment is None", a_stackID, Severity::kWarning);
		return;
	}
	if (a_index > a_copyEnchantment->effects.size()) {
		a_vm->TraceStack("Copy Enchantment index exceeds effect list size", a_stackID, Severity::kWarning);
		return;
	}
	if (a_enchantment->data.castingType != a_copyEnchantment->data.castingType) {
		a_vm->TraceStack("Casting types don't match", a_stackID, Severity::kWarning);
		return;
	}
	if (a_enchantment->data.delivery != a_copyEnchantment->data.delivery) {
		a_vm->TraceStack("Delivery types don't match", a_stackID, Severity::kWarning);
		return;
	}

	auto copyEffect = a_copyEnchantment->effects[a_index];
	if (copyEffect) {
		auto effect = new RE::Effect();
		if (effect) {
			effect->effectItem.magnitude = copyEffect->effectItem.magnitude;
			effect->effectItem.area = copyEffect->effectItem.area;
			effect->effectItem.duration = copyEffect->effectItem.duration;
			effect->baseEffect = copyEffect->baseEffect;
			effect->cost = copyEffect->cost;
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
			a_enchantment->effects.push_back(effect);
		}
	}
}


void papyrusEnchantment::RemoveMagicEffectFromEnchantment(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EnchantmentItem* a_enchantment, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost)
{
	if (!a_enchantment) {
		a_vm->TraceStack("Enchantment is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_mgef) {
		a_vm->TraceStack("MagicEffect is None", a_stackID, Severity::kWarning);
		return;
	}

	auto it = std::find_if(a_enchantment->effects.begin(), a_enchantment->effects.end(),
		[&](const auto& effect) { return effect->IsMatch(a_mgef, a_mag, a_area, a_dur, a_cost); });
	if (it != a_enchantment->effects.end()) {
		a_enchantment->effects.erase(it);
	}
}


void papyrusEnchantment::RemoveEffectItemFromEnchantment(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::EnchantmentItem* a_enchantment, RE::EnchantmentItem* a_copyEnchantment, std::uint32_t a_index)
{
	if (!a_enchantment) {
		a_vm->TraceStack("Enchantment is None", a_stackID, Severity::kWarning);
		return;
	}
	if (!a_copyEnchantment) {
		a_vm->TraceStack("Copy Enchantment is None", a_stackID, Severity::kWarning);
		return;
	}
	if (a_index > a_copyEnchantment->effects.size()) {
		a_vm->TraceStack("Copy Enchantment index exceeds effect list size", a_stackID, Severity::kWarning);
		return;
	}

	auto copyEffect = a_copyEnchantment->effects[a_index];
	if (copyEffect && copyEffect->baseEffect) {
		auto it = std::find_if(a_enchantment->effects.begin(), a_enchantment->effects.end(),
			[&](const auto& effect) { return effect->IsMatch(copyEffect->baseEffect, copyEffect->effectItem.magnitude, copyEffect->effectItem.area, copyEffect->effectItem.duration, copyEffect->cost); });
		if (it != a_enchantment->effects.end()) {
			a_enchantment->effects.erase(it);
		}
	}
}


bool papyrusEnchantment::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("papyrusEnchantment - couldn't get VMState"sv);
		return false;
	}

	auto constexpr Functions = "PO3_SKSEFunctions"sv;

	a_vm->RegisterFunction("AddMagicEffectToEnchantment"sv, Functions, AddMagicEffectToEnchantment);

	a_vm->RegisterFunction("AddEffectItemToEnchantment"sv, Functions, AddEffectItemToEnchantment);

	a_vm->RegisterFunction("GetEnchantmentType"sv, Functions, GetEnchantmentType);

	a_vm->RegisterFunction("RemoveMagicEffectFromEnchantment"sv, Functions, RemoveMagicEffectFromEnchantment);

	a_vm->RegisterFunction("RemoveEffectItemFromEnchantment"sv, Functions, RemoveEffectItemFromEnchantment);

	return true;
}
