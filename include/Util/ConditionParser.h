#pragma once


namespace Condition
{
	namespace STRING = SKSE::UTIL::STRING;

	using FUNC_ID = RE::FUNCTION_DATA::FunctionID;
	using OBJECT = RE::CONDITIONITEMOBJECT;
	using OP_CODE = RE::CONDITION_ITEM_DATA::OpCode;

	using PARAM_TYPE = RE::SCRIPT_PARAM_TYPE;
	using PARAMS = std::pair<std::optional<PARAM_TYPE>, std::optional<PARAM_TYPE>>;

	using AV = RE::ActorValue;


	enum class TYPE : std::uint32_t
	{
		kConditionItemObject = 0,
		kFunctionID,
		kParam1,
		kParam2,
		kOPCode,
		kFloat,
		kANDOR
	};


	enum FORMID : std::uint32_t
	{
		kFormID = 0,
		kESP,
	};


	inline bool isStringValid(const std::string& a_str)
	{
		return !a_str.empty() && a_str.find("NONE"sv) == std::string::npos && !STRING::onlySpace(a_str);
	}

	using ConditionData = std::tuple<OBJECT, FUNC_ID, void*, void*, OP_CODE, float, bool>;
	using ConditionDataVec = std::vector<ConditionData>;

	ConditionDataVec ParseConditions(const std::vector<RE::BSFixedString>& a_conditionList);
	std::vector<RE::BSFixedString> BuildConditions(const ConditionDataVec& a_conditions);
}
