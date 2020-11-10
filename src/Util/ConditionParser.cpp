#include "Util/ConditionParser.h"


Condition::ConditionDataVec Condition::ParseConditions(const std::vector<RE::BSFixedString>& a_conditionList)
{
	namespace STRING = SKSE::UTIL::STRING;
	
	ConditionDataVec dataVec;

	for (auto& condition : a_conditionList) {
		ConditionData data;
		auto& [conditionItem, functionID, param1, param2, opCode, floatVal, operatorVal] = data;

		auto split_condition = STRING::split(condition.c_str(), "| ");
		//conditionItemObject
		try {
			auto str = STRING::trim(split_condition.at(TYPE::kConditionItemObject));
			conditionItem = static_cast<RE::CONDITIONITEMOBJECT>(std::stoul(str));
		} catch (...) {
			continue;
		}
		//functionID
		try {
			auto str = STRING::trim(split_condition.at(TYPE::kFunctionID));
			functionID = static_cast<RE::FUNCTION_DATA::FunctionID>(std::stoul(str));
		} catch (...) {
			continue;
		}
		//param1
		try {
			auto str = STRING::trim(split_condition.at(TYPE::kParam1));
			if (!str.empty() && str.find("NONE") == std::string::npos && !STRING::onlySpace(str)) {
				auto split_param = STRING::split(str.c_str(), " ~ ");
				const auto formID = std::stoul(split_param.at(PARAMETER::kFormID), nullptr, 16);
				auto esp = split_param.at(PARAMETER::kESP);

				auto dataHandler = RE::TESDataHandler::GetSingleton();
				if (dataHandler) {
					auto form = dataHandler->LookupForm(formID, esp);
					if (form) {
						param1 = form;
					}
				}
			}
		} catch (...) {
			continue;
		}
		//param2
		try {
			auto str = STRING::trim(split_condition.at(to_underlying(TYPE::kParam2)));
			if (!str.empty() && str.find("NONE") == std::string::npos && !STRING::onlySpace(str)) {
				auto split_param = STRING::split(str.c_str(), " ~ ");
				const auto formID = std::stoul(split_param.at(PARAMETER::kFormID), nullptr, 16);
				auto esp = split_param.at(PARAMETER::kESP);

				auto dataHandler = RE::TESDataHandler::GetSingleton();
				if (dataHandler) {
					auto form = dataHandler->LookupForm(formID, esp);
					if (form) {
						param2 = form;
					}
				}
			}
		} catch (...) {
			continue;
		}
		//OPCode
		try {
			auto str = STRING::trim(split_condition.at(TYPE::kOPCode));
			opCode = static_cast<RE::CONDITION_ITEM_DATA::OpCode>(std::stoul(str));
		} catch (...) {
			continue;
		}
		//float
		try {
			auto str = STRING::trim(split_condition.at(TYPE::kFloat));
			floatVal = std::stof(str);
		} catch (...) {
			continue;
		}
		//operator
		try {
			auto str = STRING::trim(split_condition.at(TYPE::kANDOR));
			operatorVal = str.find("OR") != std::string::npos ? true : false;
		} catch (...) {
			continue;
		}

		dataVec.push_back(data);
	}

	return dataVec;
}
