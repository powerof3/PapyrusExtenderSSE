#include "Papyrus/Spell.h"

#include "Util/ConditionParser.h"


void papyrusSpell::AddMagicEffectToSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::SpellItem* a_spell, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost, std::vector<RE::BSFixedString> a_conditionList)
{
	if (!a_spell) {
		a_vm->TraceStack("Spell is None", a_stackID, Severity::kWarning);
		return;
	}
    if (!a_mgef) {
        a_vm->TraceStack("MagicEffect is None", a_stackID, Severity::kWarning);
        return;
    }
    if (a_mgef->data.castingType != a_spell->data.castingType) {
        a_vm->TraceStack("Casting types don't match", a_stackID, Severity::kWarning);
        return;
    }
    if (a_mgef->data.delivery != a_spell->data.delivery) {
        a_vm->TraceStack("Delivery types don't match", a_stackID, Severity::kWarning);
        return;
    }

    auto effect = a_spell->GetEffectIsMatch(a_mgef, a_mag, a_area, a_dur, a_cost);
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
			a_spell->effects.push_back(effect);
		}
	}
}


auto papyrusSpell::GetSpellType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::SpellItem* a_spell) -> std::int32_t
{
	if (!a_spell) {
		a_vm->TraceStack("Spell is None", a_stackID, Severity::kWarning);
		return -1;
	}

	return to_underlying(a_spell->data.spellType);
}


void papyrusSpell::RemoveMagicEffectFromSpell(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::SpellItem* a_spell, RE::EffectSetting* a_mgef, float a_mag, std::uint32_t a_area, std::uint32_t a_dur, float a_cost)
{
	if (!a_spell) {
		a_vm->TraceStack("Spell is None", a_stackID, Severity::kWarning);
		return;
	}
    if (!a_mgef) {
        a_vm->TraceStack("MagicEffect is None", a_stackID, Severity::kWarning);
        return;
    }

    auto it = std::find_if(a_spell->effects.begin(), a_spell->effects.end(),
		[&](const auto& effect) { return effect->IsMatch(a_mgef, a_mag, a_area, a_dur, a_cost); });
	if (it != a_spell->effects.end()) {
		a_spell->effects.erase(it);
	}
}


auto papyrusSpell::RegisterFuncs(VM* a_vm) -> bool
{
	if (!a_vm) {
		logger::critical("papyrusSpell - couldn't get VMState"sv);
		return false;
	}

	auto constexpr Functions = "PO3_SKSEFunctions"sv;

    a_vm->RegisterFunction("AddMagicEffectToSpell"sv, Functions, AddMagicEffectToSpell);

	a_vm->RegisterFunction("GetSpellType"sv, Functions, GetSpellType);

	a_vm->RegisterFunction("RemoveMagicEffectFromSpell"sv, Functions, RemoveMagicEffectFromSpell);

	return true;
}
