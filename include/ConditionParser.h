#pragma once


namespace Condition
{
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


	enum class PARAMETER : std::uint32_t
	{
		kFormID = 0,
		kESP,
	};


	using ConditionData = std::tuple<RE::CONDITIONITEMOBJECT, RE::FUNCTION_DATA::FunctionID, void*, void*, RE::CONDITION_ITEM_DATA::OpCode, float, bool>;
	using ConditionDataVec = std::vector<ConditionData>;

	ConditionDataVec ParseConditions(const std::vector<RE::BSFixedString>& a_conditionList);
}
