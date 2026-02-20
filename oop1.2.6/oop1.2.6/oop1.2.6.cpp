#include <iostream>
#include <string>
#include <algorithm>
#include <climits>
#include <optional>

enum class ErrorType
{
	None,
	InvalidRadix,
	EmptyString,
	InvalidDigit,
	Overflow
};

void PrintError(ErrorType error)
{
	switch (error)
	{
	case ErrorType::InvalidRadix:
		std::cerr << "Error: invalid radix (must be 2..36)" << std::endl;
		break;
	case ErrorType::EmptyString:
		std::cerr << "Error: empty string" << std::endl;
		break;
	case ErrorType::InvalidDigit:
		std::cerr << "Error: invalid digit for given radix" << std::endl;
		break;
	case ErrorType::Overflow:
		std::cerr << "Error: overflow" << std::endl;
		break;
	default:
		break;
	}
}

std::optional<int> CharToDigit(char ch)
{
	if (ch >= '0' && ch <= '9') return ch - '0';
	if (ch >= 'A' && ch <= 'Z') return 10 + (ch - 'A');
	if (ch >= 'a' && ch <= 'z') return 10 + (ch - 'a');
	return std::nullopt;
}

std::optional<char> DigitToChar(int digit)
{
	if (digit >= 0 && digit <= 9) return '0' + digit;
	if (digit >= 10 && digit <= 35) return 'A' + (digit - 10);
	return std::nullopt;
}

bool ValidateRadix(int radix, ErrorType& error)
{
	error = ErrorType::None;
	if (radix < 2 || radix > 36)
	{
		error = ErrorType::InvalidRadix;
		return false;
	}
	return true;
}

bool CheckRadix(int radix, const std::string& str, ErrorType& error)
{
	error = ErrorType::None;

	if (!ValidateRadix(radix, error))
		return false;

	if (str.empty())
	{
		error = ErrorType::EmptyString;
		return false;
	}

	return true;
}

bool CheckRadix(int radix, ErrorType& error)
{
	error = ErrorType::None;
	return ValidateRadix(radix, error);
}

int StringToInt(const std::string& str, int radix, ErrorType& error)
{
    if (!CheckRadix(radix, str, error))
        return 0;

    size_t i = 0;
    bool negative = false;

    if (str[i] == '+' || str[i] == '-')
    {
        negative = (str[i] == '-');
        ++i;
        if (i == str.size())
        {
            error = ErrorType::InvalidDigit;
            return 0;
        }
    }

    int result = 0;
    const int LIMIT = negative ? INT_MIN : INT_MAX;
	int signedDigit = 0;
	int digit = 0;

    for (; i < str.size(); ++i)
    {
        auto digitOpt = CharToDigit(str[i]);
        if (!digitOpt)
        {
            error = ErrorType::InvalidDigit;
            return 0;
        }

        digit = digitOpt.value();

        if (digit >= radix)
        {
            error = ErrorType::InvalidDigit;
            return 0;
        }

        signedDigit = negative ? -digit : digit;

        if (!negative)
        {
            if (result > (LIMIT - digit) / radix)
            {
                error = ErrorType::Overflow;
                return 0;
            }
        }
        else
        {
            if (result < (LIMIT + digit) / radix)
            {
                error = ErrorType::Overflow;
                return 0;
            }
        }

        result = result * radix + signedDigit;
    }

    error = ErrorType::None;
    return result;
}

std::string IntToString(int number, int radix, ErrorType& error)
{
    if (!CheckRadix(radix, error))
        return "";

    if (number == 0)
    {
        error = ErrorType::None;
        return "0";
    }

    bool negative = (number < 0);
    std::string result;

	int rem = 0;
    while (number != 0)
    {
		rem = number % radix;  
        if (rem < 0) rem = -rem;    
        auto chOpt = DigitToChar(rem);
        if (!chOpt)
        {
            error = ErrorType::InvalidDigit;
            return "";
        }
        result.push_back(*chOpt);

        number /= radix;          
    }

    if (negative)
        result.push_back('-');

    std::reverse(result.begin(), result.end());

    error = ErrorType::None;
    return result;
}

void PrintHelp(std::ostream& out)
{
	out << "Usage:\n"
		<< "  <source notation> <destination notation> <value>\n";
}

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		PrintHelp(std::cout);
		return EXIT_FAILURE;
	}

	ErrorType error = ErrorType::None;

	int sourceNotation = StringToInt(argv[1], 10, error);
	if (error != ErrorType::None)
	{
		PrintError(error);
		return EXIT_FAILURE;
	}

	int destinationNotation = StringToInt(argv[2], 10, error);
	if (error != ErrorType::None)
	{
		PrintError(error);
		return EXIT_FAILURE;
	}

	int number = StringToInt(argv[3], sourceNotation, error);
	if (error != ErrorType::None)
	{
		PrintError(error);
		return EXIT_FAILURE;
	}

	std::string result = IntToString(number, destinationNotation, error);
	if (error != ErrorType::None)
	{
		PrintError(error);
		return EXIT_FAILURE;
	}

	std::cout << result << std::endl;
	return EXIT_SUCCESS;
}