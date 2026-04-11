#pragma once
#include <istream>
#include <ostream>
#include <string>

#include "Calculator.h"
#include "Operation.h"

class ControllerCalculator
{
public:
	ControllerCalculator(Calculator& calculator, std::istream& input, std::ostream& output);

	void Run();

private:
	void ProcessLine(const std::string& line);

	void HandleVar(const std::string& args);
	void HandleLet(const std::string& args);
	void HandleFn(const std::string& args);
	void HandlePrint(const std::string& args);
	void HandlePrintVars();
	void HandlePrintFns();

	void PrintValue(double value);
	void PrintError(const std::string& message);

	bool IsValidIdentifier(const std::string& name) const;
	std::string Trim(const std::string& text) const;
	bool TryParseDouble(const std::string& text, double& value) const;
	bool ParseBinaryExpression(const std::string& text, std::string& left, Operation& operation, std::string& right) const;

private:
	Calculator& m_calculator;
	std::istream& m_input;
	std::ostream& m_output;
};