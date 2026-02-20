#include <iostream>
#include <string>
#include <algorithm>
#include <climits>


int CharToDigit(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'Z') return 10 + (c - 'A');
    //if (c >= 'a' && c <= 'z') return 10 + (c - 'a');
    return -1;
}

char DigitToChar(int digit)
{
    if (digit >= 0 && digit <= 9)
        return '0' + digit;

    if (digit >= 10 && digit <= 35)
        return 'A' + (digit - 10);

    return '?';
}

bool ValidateRadix(int radix, bool& wasError)
{
    wasError = false;
    if (radix < 2 || radix > 36)
    {
        wasError = true;
        return false;
    }
    return true;
}

bool CheckRadix(int radix, const std::string& str, bool& wasError)
{
    wasError = false;
    if (!ValidateRadix(radix, wasError)) 
        return false;

    if (str.empty())
    {
        wasError = true;
        return false;
    }

    return true;
}

bool CheckRadix(int radix, int n, bool& wasError)
{
    wasError = false;
    return ValidateRadix(radix, wasError);
}

int StringToInt(const std::string& str, int radix, bool& wasError)
{
    if (!CheckRadix(radix, str, wasError))
        return 0;

    size_t i = 0;
    bool negative = false;

    if (str[i] == '+' || str[i] == '-')
    {
        negative = (str[i] == '-');
        ++i;
        if (i == str.size())
        {
            wasError = true;
            return 0;
        }
    }

    long long result = 0;
    long long limit = negative ? -(long long)INT_MIN : (long long)INT_MAX;

    for (; i < str.size(); ++i)
    {
        int digit = CharToDigit(str[i]);
        if (digit < 0 || digit >= radix)
        {
            wasError = true;
            return 0;
        }

        if (result > (limit - digit) / radix)
        {
            wasError = true;
            return 0;
        }

        result = result * radix + digit;
    }

    result = negative ? -result : result;
    return (int)result;
}

std::string IntToString(int n, int radix, bool& wasError)
{
    if (!CheckRadix(radix, n, wasError))
        return "";

    if (n == 0)
        return "0";

    bool negative = (n < 0);
    unsigned int value;

    if (negative)
        value = static_cast<unsigned int>(-(n + 1)) + 1;
    else
        value = static_cast<unsigned int>(n);

    std::string result;
    while (value > 0)
    {
        int remainder = value % radix;
        result.push_back(DigitToChar(remainder));
        value /= radix;
    }

    if (negative)
        result.push_back('-');

    std::reverse(result.begin(), result.end());
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

    int sourceNotation = std::stoi(argv[1]);
    int destinationNotation = std::stoi(argv[2]);
    std::string value = argv[3];
    bool wasError = false;

    int number = StringToInt(value, sourceNotation, wasError);

    if (wasError)
    {
        std::cerr << "Error transformation string to int";
        return EXIT_FAILURE;
    }

    std::string result = IntToString(number, destinationNotation, wasError);

    if (wasError)
    {
        std::cerr << "Error transformation int to string";
        return EXIT_FAILURE;
    }

    std::cout << result << std::endl;
    return EXIT_SUCCESS;
}

