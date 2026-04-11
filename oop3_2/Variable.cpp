#include "Variable.h"
#include <limits>
#include <cmath>

Variable::Variable(const std::string& name)
	: m_name(name)
	, m_value(std::numeric_limits<double>::quiet_NaN())
{
}

Variable::Variable(const std::string& name, double value)
	: m_name(name)
	, m_value(value)
{
}

std::string Variable::GetName() const
{
	return m_name;
}

double Variable::GetValue() const
{
	return m_value;
}

void Variable::SetValue(double value)
{
	m_value = value;
}

bool Variable::IsDefined() const
{
	return !std::isnan(m_value);
}