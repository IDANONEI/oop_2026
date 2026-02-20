

#include <iostream>
#include <string>

int CharToDigit(char letter)
{
    if (letter >= '0' && letter <= '9') return letter - '0';
    if (letter >= 'A' && letter <= 'Z') return (10 + (letter - 'A'));
    if (letter >= 'a' && letter <= 'z') return (10 + (letter - '0'));
    return -1;
}

char DigitToChar(int digit)
{
    if (digit >= 0 && digit <= 9) return char('0' + digit);
    return char('A' + (digit - 10));
}

int StringToInt(const std::string& str, int radix, bool& wasError)
{
    wasError = false;
    if (radix < 2 || radix > 36)
    {
        wasError = true;
        return 0;
    }

    if (radix < 2 || radix > 36)
    {
        wasError = true;
        return 0;
    }


}

std::string IntToString(int n, int radix, bool& wasError)
{

}

void PrintHelp(std::ostream& out)
{
    out << "Usage:\n"
        << "  <source notation> <destination notation> <value>\n";
}


int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        PrintHelp(std::cout);
    }

    int sourceNotation = std::stoi(argv[1]);
    int destinationNotation = std::stoi(argv[2]);
    bool wasError = false;
    std::string value = argv[3];

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

