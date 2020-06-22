#pragma once

#include "main.h"


namespace Condition
{
	enum class TYPE : UInt8
	{
		kConditionItemObject = 0,
		kFunctionID,
		kParam1,
		kParam2,
		kOPCode,
		kFloat,
		kANDOR
	};


	enum class PARAMETER : UInt8
	{
		kFormID = 0,
		kESP,
	};


	using ConditionData = std::tuple<RE::CONDITIONITEMOBJECT, RE::FUNCTION_DATA::FunctionID, void*, void*, RE::CONDITION_ITEM_DATA::OpCode, float, bool>;
	using ConditionDataVec = std::vector<ConditionData>;

	ConditionDataVec ParseConditions(const std::vector<RE::BSFixedString>& a_conditionList);
}
