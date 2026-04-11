#include "Function.h"

Function::Function(const std::string& name, const std::string& operand)
	: m_name(name)
	, m_leftOperandName(operand)
	, m_rightOperandName("")
	, m_operation(Operation::None)
	, m_cachedValue(std::numeric_limits<Value>::quiet_NaN())  //optional
	, m_isCacheValid(false)
{
}

Function::Function(const std::string& name, const std::string& left, Operation operation, const std::string& right)
	: m_name(name)
	, m_leftOperandName(left)
	, m_rightOperandName(right)
	, m_operation(operation)
	, m_cachedValue(std::numeric_limits<Value>::quiet_NaN())
	, m_isCacheValid(false)
{
}
bool Function::DependsOn(const Identifier& name) const
{
	if (name == m_leftOperandName)
	{
		return true;
	}

	if (IsBinary() && name == m_rightOperandName)
	{
		return true;
	}

	return false;
}

bool Function::IsCacheValid() const
{
	return m_isCacheValid;
}

Function::Value Function::GetCachedValue() const
{
	return m_cachedValue;
}

void Function::SetCachedValue(Value value) const
{
	m_cachedValue = value;
	m_isCacheValid = true;
}

void Function::InvalidateCache() const
{
	m_isCacheValid = false;
}

std::string Function::GetName() const
{
	return m_name;
}

std::string Function::GetLeftOperandName() const
{
	return m_leftOperandName;
}

std::string Function::GetRightOperandName() const
{
	return m_rightOperandName;
}

Operation Function::GetOperation() const
{
	return m_operation;
}

bool Function::IsBinary() const
{
	return m_operation != Operation::None;
}