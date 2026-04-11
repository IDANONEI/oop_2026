#pragma once
#include <string>

class Variable
{
public:
	explicit Variable(const std::string& name);
	Variable(const std::string& name, double value);

	std::string GetName() const;
	double GetValue() const;
	void SetValue(double value);
	bool IsDefined() const;

private:
	std::string m_name;
	double m_value;
};
