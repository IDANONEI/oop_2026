#pragma once
#include <map>
#include <string>
#include <vector>
#include <utility>

#include "Variable.h"
#include "Function.h"

class Calculator
{
public:
	using Identifier = std::string;
	using Value = double;

	using VariableMap = std::map<Identifier, Variable>;
	using FunctionMap = std::map<Identifier, Function>;

	using NameValuePair = std::pair<Identifier, Value>;
	using NameValueList = std::vector<NameValuePair>;

	bool DeclareVariable(const Identifier& name);

	bool Assign(const Identifier& name, Value value);
	bool Assign(const Identifier& name, const Identifier& sourceName);

	bool DeclareFunction(const Identifier& name, const Identifier& operandName);
	bool DeclareFunction(const Identifier& name, const Identifier& leftName, Operation operation, const Identifier& rightName);

	Value GetIdentifierValue(const Identifier& name) const;

	NameValueList GetSortedVariables() const;
	NameValueList GetSortedFunctions() const;

	bool Exists(const Identifier& name) const;
	bool IsVariable(const Identifier& name) const;
	bool IsFunction(const Identifier& name) const;

private:
	Value ResolveIdentifierValue(const Identifier& name) const;
	Value EvaluateFunction(const Identifier& functionName) const;

	void InvalidateDependents(const Identifier& name);

	VariableMap m_variables;
	FunctionMap m_functions;
};