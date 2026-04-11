#include "ControllerCalculator.h"

#include <cmath>
#include <iomanip>
#include <sstream>

ControllerCalculator::ControllerCalculator(Calculator& calculator, std::istream& input, std::ostream& output)
    : m_calculator(calculator)
    , m_input(input)
    , m_output(output)
{
}

void ControllerCalculator::Run()
{
    std::string line;
    while (std::getline(m_input, line))
    {
        ProcessLine(line);
    }
}

void ControllerCalculator::ProcessLine(const std::string& line)
{
    std::string trimmed = Trim(line);
    if (trimmed.empty())
    {
        return;
    }

    if (trimmed == "printvars")
    {
        HandlePrintVars();
        return;
    }

    if (trimmed == "printfns")
    {
        HandlePrintFns();
        return;
    }

    auto firstSpace = trimmed.find(' ');
    std::string command = (firstSpace == std::string::npos) ? trimmed : trimmed.substr(0, firstSpace);
    std::string args = (firstSpace == std::string::npos) ? "" : Trim(trimmed.substr(firstSpace + 1));

    if (command == "var")
    {
        HandleVar(args);
    }
    else if (command == "let")
    {
        HandleLet(args);
    }
    else if (command == "fn")
    {
        HandleFn(args);
    }
    else if (command == "print")
    {
        HandlePrint(args);
    }
    else
    {
        PrintError("Unknown command");
    }
}

void ControllerCalculator::HandleVar(const std::string& args)
{
    std::string name = Trim(args);

    if (name.empty() || !IsValidIdentifier(name))
    {
        PrintError("Invalid usage");
        return;
    }

    if (m_calculator.Exists(name))
    {
        PrintError("Name already exists");
        return;
    }

    if (!m_calculator.DeclareVariable(name))
    {
        PrintError("Name already exists");
    }
}

void ControllerCalculator::HandleLet(const std::string& args)
{
    size_t eqPos = args.find('=');
    if (eqPos == std::string::npos || args.find('=', eqPos + 1) != std::string::npos)
    {
        PrintError("Invalid usage");
        return;
    }

    std::string left = Trim(args.substr(0, eqPos));
    std::string right = Trim(args.substr(eqPos + 1));

    if (!IsValidIdentifier(left) || right.empty())
    {
        PrintError("Invalid usage");
        return;
    }

    if (m_calculator.IsFunction(left))
    {
        PrintError("Invalid usage");
        return;
    }

    double number = 0.0;
    if (TryParseDouble(right, number))
    {
        m_calculator.Assign(left, number);
        return;
    }

    if (!IsValidIdentifier(right))
    {
        PrintError("Invalid usage");
        return;
    }

    if (!m_calculator.Exists(right))
    {
        PrintError("Name does not exist");
        return;
    }

    m_calculator.Assign(left, right);
}

void ControllerCalculator::HandleFn(const std::string& args)
{
    size_t eqPos = args.find('=');
    if (eqPos == std::string::npos || args.find('=', eqPos + 1) != std::string::npos)
    {
        PrintError("Invalid usage");
        return;
    }

    std::string name = Trim(args.substr(0, eqPos));
    std::string expression = Trim(args.substr(eqPos + 1));

    if (!IsValidIdentifier(name) || expression.empty())
    {
        PrintError("Invalid usage");
        return;
    }

    if (m_calculator.Exists(name))
    {
        PrintError("Name already exists");
        return;
    }

    std::string left;
    std::string right;
    Operation operation = Operation::None;

    if (ParseBinaryExpression(expression, left, operation, right))
    {
        if (!IsValidIdentifier(left) || !IsValidIdentifier(right))
        {
            PrintError("Invalid usage");
            return;
        }

        if (!m_calculator.Exists(left) || !m_calculator.Exists(right))
        {
            PrintError("Name does not exist");
            return;
        }

        if (!m_calculator.DeclareFunction(name, left, operation, right))
        {
            PrintError("Invalid usage");
        }
        return;
    }

    std::string operand = Trim(expression);
    if (!IsValidIdentifier(operand))
    {
        PrintError("Invalid usage");
        return;
    }

    if (!m_calculator.Exists(operand))
    {
        PrintError("Name does not exist");
        return;
    }

    if (!m_calculator.DeclareFunction(name, operand))
    {
        PrintError("Invalid usage");
    }
}

void ControllerCalculator::HandlePrint(const std::string& args)
{
    std::string name = Trim(args);

    if (!IsValidIdentifier(name))
    {
        PrintError("Invalid usage");
        return;
    }

    if (!m_calculator.Exists(name))
    {
        PrintError("Name does not exist");
        return;
    }

    PrintValue(m_calculator.GetIdentifierValue(name));
}

void ControllerCalculator::HandlePrintVars()
{
    for (const auto& [name, value] : m_calculator.GetSortedVariables())
    {
        m_output << name << ":";
        PrintValue(value);
    }
}

void ControllerCalculator::HandlePrintFns()
{
    for (const auto& [name, value] : m_calculator.GetSortedFunctions())
    {
        m_output << name << ":";
        PrintValue(value);
    }
}

void ControllerCalculator::PrintValue(double value)
{
    if (std::isnan(value))
    {
        m_output << "nan" << '\n';
        return;
    }

    m_output << std::fixed << std::setprecision(2) << value << '\n';
}

void ControllerCalculator::PrintError(const std::string& message)
{
    m_output << message << '\n';
}

bool ControllerCalculator::IsValidIdentifier(const std::string& name) const
{
    if (name.empty())
    {
        return false;
    }

    unsigned char first = static_cast<unsigned char>(name[0]);
    if (!(std::isalpha(first) || name[0] == '_'))
    {
        return false;
    }

    for (char ch : name)
    {
        unsigned char uch = static_cast<unsigned char>(ch);
        if (!(std::isalnum(uch) || ch == '_'))
        {
            return false;
        }
    }

    return true;
}

std::string ControllerCalculator::Trim(const std::string& text) const
{
    size_t start = 0;
    while (start < text.size() && std::isspace(static_cast<unsigned char>(text[start])))
    {
        ++start;
    }

    size_t end = text.size();
    while (end > start && std::isspace(static_cast<unsigned char>(text[end - 1])))
    {
        --end;
    }

    return text.substr(start, end - start);
}

bool ControllerCalculator::TryParseDouble(const std::string& text, double& value) const
{
    std::istringstream input(text);
    input >> value;

    if (!input)
    {
        return false;
    }

    char extra = '\0';
    if (input >> extra)
    {
        return false;
    }

    return true;
}

bool ControllerCalculator::ParseBinaryExpression(const std::string& text, std::string& left, Operation& operation, std::string& right) const
{
    std::string expr;
    expr.reserve(text.size());

    for (char ch : text)
    {
        if (!std::isspace(static_cast<unsigned char>(ch)))
        {
            expr.push_back(ch);
        }
    }

    size_t opPos = std::string::npos;
    char opChar = '\0';

    for (size_t i = 0; i < expr.size(); ++i)
    {
        char ch = expr[i];
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
        {
            if (opPos != std::string::npos)
            {
                return false;
            }
            opPos = i;
            opChar = ch;
        }
    }

    if (opPos == std::string::npos)
    {
        return false;
    }

    if (opPos == 0 || opPos + 1 >= expr.size())
    {
        return false;
    }

    left = expr.substr(0, opPos);
    right = expr.substr(opPos + 1);

    switch (opChar)
    {
    case '+':
        operation = Operation::Add;
        break;
    case '-':
        operation = Operation::Subtract;
        break;
    case '*':
        operation = Operation::Multiply;
        break;
    case '/':
        operation = Operation::Divide;
        break;
    default:
        return false;
    }

    return true;
}