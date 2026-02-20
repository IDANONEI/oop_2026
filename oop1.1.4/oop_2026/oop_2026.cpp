#include <iostream>
#include <string>
#include <fstream>

constexpr const char* HELP_FLAG = "-h";

std::string ReplaceString(const std::string& line, const std::string& search, const std::string& repl)
{
    if (search.empty()) return line;

    std::string out;
    size_t pos = 0;
    while (true) 
    {
        size_t found = line.find(search, pos);
        if (found == std::string::npos) 
        {
            out.append(line, pos, std::string::npos);
            break;
        }
        out.append(line, pos, found - pos);
        out += repl;
        pos = found + search.size();
    }
    return out;
}

void ReplaceFile(const std::string& search, const std::string& repl,
    std::istream& inFile, std::ostream& outFile)
{
    std::string line;
    while (std::getline(inFile, line))
    {
        outFile << ReplaceString(line, search, repl);
        outFile << "\n";

    }
}

void PrintHelp(std::ostream& outFile)
{
    outFile << "Usage:\n";
    outFile << "  replace.exe <input file> <output file> <search string> <replace string>\n";
    outFile << "  replace.exe            (stdin mode)\n";
    outFile << "  replace.exe -h         (help)\n";
}

bool IsHelpFlag(const std::string& flag, std::ostream& out)
{
    if (flag == HELP_FLAG)
    {
        PrintHelp(out);
        return true;
    }
    return false;
}


bool CanUseStdin(std::string& search, std::string& repl)
{
    std::string param;

    if (!std::getline(std::cin, param))
    {
        std::cout << "ERROR";
        return false;
    }

    if (IsHelpFlag(param, std::cout))
    {
        return false;
    }
    search = param;

    if (!std::getline(std::cin, repl))
    {
        std::cout << "ERROR";
        return false;
    }

    return true;
}

int RunStdinMode()
{
    std::string search, repl;
    if (CanUseStdin(search, repl))
    {
        ReplaceFile(search, repl, std::cin, std::cout);
    }
    return EXIT_SUCCESS;
}

bool CanUseFiles(const std::string& inputFile, const std::string& outputFile, std::ifstream& inFile, std::ofstream& outFile)
{
    if (inputFile == outputFile)
    {
        std::cout << "ERROR";
        return false;
    }

    inFile.open(inputFile);
    if (!inFile.is_open())
    {
        std::cout << "ERROR";
        return false;
    }

    outFile.open(outputFile);
    if (!outFile.is_open())
    {
        std::cout << "ERROR";
        inFile.close();
        return false;
    }

    return true;
}

int RunFilesMode(char* argv[])
{
    std::string inputNameFile = argv[1];
    std::string outputNameFile = argv[2];
    std::string search = argv[3];
    std::string repl = argv[4];

    std::ifstream inFile;
    std::ofstream outFile;

    if (CanUseFiles(inputNameFile, outputNameFile, inFile, outFile))
    {
        ReplaceFile(search, repl, inFile, outFile);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}


int main(int argc, char* argv[])
{
    if (argc == 2 && IsHelpFlag(std::string(argv[1]), std::cout))
    {
        return EXIT_SUCCESS;
    }
    if (argc == 1) return RunStdinMode();
    if (argc == 5) return RunFilesMode(argv);

    std::cout << "ERROR";
    return EXIT_FAILURE;
}
