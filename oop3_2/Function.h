#pragma once
#include <string>
#include <vector>
#include "Operation.h"

class Function
{
public:
	using Identifier = std::string;
	using Dependencies = std::vector<Identifier>;
	using Value = double;


	Function(const std::string& name, const std::string& operand);
	Function(const std::string& name, const std::string& left, Operation operation, const std::string& right);

	std::string GetName() const;
	std::string GetLeftOperandName() const;
	std::string GetRightOperandName() const;
	Operation GetOperation() const;
	bool IsBinary() const;

	bool DependsOn(const Identifier& name) const;
	bool IsCacheValid() const;
	Value GetCachedValue() const;
	void SetCachedValue(Value value) const;
	void InvalidateCache() const;

private:
	std::string m_name;
	std::string m_leftOperandName;
	std::string m_rightOperandName;
	Operation m_operation;
	mutable Value m_cachedValue;
	mutable bool m_isCacheValid;
};