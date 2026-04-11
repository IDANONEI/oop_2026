#include "Calculator.h"
#include <limits>
#include <cmath>

namespace
{
    Calculator::Value MakeNaN()
    {
        return std::numeric_limits<Calculator::Value>::quiet_NaN();
    }
}

bool Calculator::DeclareVariable(const Identifier& name)
{
    if (Exists(name))
    {
        return false;
    }

    m_variables.emplace(name, Variable(name));
    return true;
}

bool Calculator::Assign(const Identifier& name, Value value)
{
    if (IsFunction(name))
    {
        return false;
    }

    auto it = m_variables.find(name);
    if (it == m_variables.end())
    {
        m_variables.emplace(name, Variable(name, value));
    }
    else
    {
        it->second.SetValue(value);
    }

	InvalidateDependents(name);

    return true;
}

bool Calculator::Assign(const Identifier& name, const Identifier& sourceName)
{
    if (!Exists(sourceName))
    {
        return false;
    }

    if (IsFunction(name))
    {
        return false;
    }

    Value value = GetIdentifierValue(sourceName);
    return Assign(name, value);
}

bool Calculator::DeclareFunction(const Identifier& name, const Identifier& operandName)
{
    if (Exists(name))
    {
        return false;
    }

    if (!Exists(operandName))
    {
        return false;
    }

    m_functions.emplace(name, Function(name, operandName));
    return true;
}

bool Calculator::DeclareFunction(const Identifier& name, const Identifier& leftName, Operation operation, const Identifier& rightName)
{
    if (Exists(name))
    {
        return false;
    }

    if (!Exists(leftName) || !Exists(rightName))
    {
        return false;
    }

    m_functions.emplace(name, Function(name, leftName, operation, rightName));
    return true;
}


Calculator::Value Calculator::GetIdentifierValue(const Identifier& name) const
{
    return ResolveIdentifierValue(name);
}

Calculator::NameValueList Calculator::GetSortedVariables() const
{
    NameValueList result;
    result.reserve(m_variables.size());

    for (const auto& [name, variable] : m_variables)
    {
        result.emplace_back(name, variable.GetValue());
    }

    return result;
}

Calculator::NameValueList Calculator::GetSortedFunctions() const
{
    NameValueList result;
    result.reserve(m_functions.size());

    for (const auto& [name, function] : m_functions)
    {
        (void)function;
        result.emplace_back(name, EvaluateFunction(name));
    }

    return result;
}

bool Calculator::Exists(const Identifier& name) const
{
    return IsVariable(name) || IsFunction(name);
}

bool Calculator::IsVariable(const Identifier& name) const
{
    return m_variables.find(name) != m_variables.end();
}

bool Calculator::IsFunction(const Identifier& name) const
{
    return m_functions.find(name) != m_functions.end();
}


Calculator::Value Calculator::ResolveIdentifierValue(const Identifier& name) const
{
    auto varIt = m_variables.find(name);
    if (varIt != m_variables.end())
    {
        return varIt->second.GetValue();
    }

    auto fnIt = m_functions.find(name);
    if (fnIt != m_functions.end())
    {
        return EvaluateFunction(name);
    }

    return MakeNaN();
}



Calculator::Value Calculator::EvaluateFunction(const Identifier& functionName) const
{
	auto fnIt = m_functions.find(functionName);
	if (fnIt == m_functions.end())
	{
		return MakeNaN();
	}

	const Function& function = fnIt->second;


	if (function.IsCacheValid())
	{
		return function.GetCachedValue();
	}

	Value result = MakeNaN();

	Value leftValue = ResolveIdentifierValue(function.GetLeftOperandName());
	if (std::isnan(leftValue))
	{
		function.SetCachedValue(MakeNaN());
		return function.GetCachedValue();
	}

	if (!function.IsBinary())
	{
		function.SetCachedValue(leftValue);
		return function.GetCachedValue();
	}

	Value rightValue = ResolveIdentifierValue(function.GetRightOperandName());
	if (std::isnan(rightValue))
	{
		function.SetCachedValue(MakeNaN());
		return function.GetCachedValue();
	}

	switch (function.GetOperation())
	{
	case Operation::Add:
		result = leftValue + rightValue;
		break;

	case Operation::Subtract:
		result = leftValue - rightValue;
		break;

	case Operation::Multiply:
		result = leftValue * rightValue;
		break;

	case Operation::Divide:
		if (rightValue == 0.0)
		{
			result = MakeNaN();
		}
		else
		{
			result = leftValue / rightValue;
		}
		break;

	case Operation::None:
	default:
		result = leftValue;
		break;
	}

	function.SetCachedValue(result);
	return function.GetCachedValue();
}



void Calculator::InvalidateDependents(const Identifier& name)
{
	std::vector<Identifier> dependentFunctions;

	for (auto& [functionName, function] : m_functions)
	{
		if (function.DependsOn(name))
		{
			function.InvalidateCache();
			dependentFunctions.push_back(functionName);
		}
	}
	for (const auto& dependentFunctionName : dependentFunctions)
	{
		InvalidateDependents(dependentFunctionName);
	}
}