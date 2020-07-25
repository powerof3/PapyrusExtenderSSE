#include "ConditionParser.h"


namespace UTIL
{
	std::vector<std::string> split(const char* phrase, const std::string& delimiter)
	{
		std::vector<std::string> list;
		std::string s = phrase;
		size_t pos = 0;
		std::string token;
		while ((pos = s.find(delimiter)) != std::string::npos) {
			token = s.substr(0, pos);
			list.push_back(token);
			s.erase(0, pos + delimiter.length());
		}
		list.push_back(s);
		return list;
	}


	std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
	{
		str.erase(0, str.find_first_not_of(chars));
		return str;
	}


	std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
	{
		str.erase(str.find_last_not_of(chars) + 1);
		return str;
	}


	std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
	{
		return ltrim(rtrim(str, chars), chars);
	}
}


Condition::ConditionDataVec Condition::ParseConditions(const std::vector<RE::BSFixedString>& a_conditionList)
{
	ConditionDataVec dataVec;

	for (auto& condition : a_conditionList) {
		ConditionData data;

		auto split_condition = UTIL::split(condition.c_str(), "| ");
		//conditionItemObject
		try {
			auto str = UTIL::trim(split_condition.at(to_underlying(TYPE::kConditionItemObject)));
			std::get<to_underlying(TYPE::kConditionItemObject)>(data) = static_cast<RE::CONDITIONITEMOBJECT>(std::stoul(str));
		} catch (...) {
			continue;
		}
		//functionID
		try {
			auto str = UTIL::trim(split_condition.at(to_underlying(TYPE::kFunctionID)));
			std::get<to_underlying(TYPE::kFunctionID)>(data) = static_cast<RE::FUNCTION_DATA::FunctionID>(std::stoul(str));
		} catch (...) {
			continue;
		}
		//param1
		try {
			auto param1 = UTIL::trim(split_condition.at(to_underlying(TYPE::kParam1)));
			if (param1.find("NONE") == std::string::npos) {
				auto split_param = UTIL::split(param1.c_str(), " ~ ");
				const auto formID = std::stoul(split_param.at(to_underlying(PARAMETER::kFormID)), nullptr, 16);
				auto esp = split_param.at(to_underlying(PARAMETER::kESP));

				auto dataHandler = RE::TESDataHandler::GetSingleton();
				if (dataHandler) {
					auto form = dataHandler->LookupForm(formID, esp);
					if (form) {
						std::get<to_underlying(TYPE::kParam1)>(data) = form;
					}
				}
			}
		} catch (...) {
			continue;
		}
		//param2
		try {
			auto param2 = UTIL::trim(split_condition.at(to_underlying(TYPE::kParam2)));
			if (param2.find("NONE") == std::string::npos) {
				auto split_param = UTIL::split(param2.c_str(), " ~ ");
				const auto formID = std::stoul(split_param.at(to_underlying(PARAMETER::kFormID)), nullptr, 16);
				auto esp = split_param.at(to_underlying(PARAMETER::kESP));

				auto dataHandler = RE::TESDataHandler::GetSingleton();
				if (dataHandler) {
					auto form = dataHandler->LookupForm(formID, esp);
					if (form) {
						std::get<to_underlying(TYPE::kParam2)>(data) = form;
					}
				}
			}
		} catch (...) {
			continue;
		}
		//OPCode
		try {
			auto str = UTIL::trim(split_condition.at(to_underlying(TYPE::kOPCode)));
			std::get<to_underlying(TYPE::kOPCode)>(data) = static_cast<RE::CONDITION_ITEM_DATA::OpCode>(std::stoul(str));
		} catch (...) {
			continue;
		}
		//float
		try {
			auto str = UTIL::trim(split_condition.at(to_underlying(TYPE::kFloat)));
			std::get<to_underlying(TYPE::kFloat)>(data) = std::stof(str);
		} catch (...) {
			continue;
		}
		//operator
		try {
			auto str = UTIL::trim(split_condition.at(to_underlying(TYPE::kANDOR)));
			std::get<to_underlying(TYPE::kANDOR)>(data) = str.find("OR") != std::string::npos ? true : false;
		} catch (...) {
			continue;
		}

		dataVec.push_back(data);
	}

	return dataVec;
}
