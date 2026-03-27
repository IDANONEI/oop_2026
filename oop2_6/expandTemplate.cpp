#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <map>
#include <vector>

#include "processor.h"

constexpr const char* HELP_FLAG = "-h";
constexpr const int START_INDEX = 3;

void PrintHelp(std::ostream& out)
{
    out << "Usage:\n";
    out << "  expand_template.exe\n";
    out << "  expand_template.exe -h\n";
    out << "  expand_template.exe <input-file> <output-file> [<param> <value> [<param> <value> ...]]\n";
}

bool IsHelpFlag(const std::string& flag)
{
    return flag == HELP_FLAG;
}

std::string TrimLeft(const std::string& s)
{
	std::size_t i = 0;
	while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i])))
	{
		++i;
	}
	return s.substr(i);
}

std::string TrimRight(const std::string& s)
{
	if (s.empty())
	{
		return s;
	}
	std::size_t i = s.size();
	while (i > 0 && std::isspace(static_cast<unsigned char>(s[i - 1])))
	{
		--i;
	}
	return s.substr(0, i);
}

bool ParseStdinParamsAndTemplate(std::istream& in, std::map<std::string, std::string>& params, std::string& tpl, std::string& errorMessage)
{
    params.clear();
    tpl.clear();
    errorMessage.clear();

    std::string line;
    bool separatorFound = false;
    std::vector<std::string> templateLines;

    while (std::getline(in, line))
    {
        if (!separatorFound)
        {
            if (line.empty())
            {
                separatorFound = true;
                continue;
            }
            std::size_t pos = line.find(":=");
            if (pos == std::string::npos)
            {
                errorMessage = "Invalid input";
                return false;
            }
        	std::string key = TrimRight(line.substr(0, pos));
        	std::string value = TrimLeft(line.substr(pos + 2));
        	if (key.empty())
        	{
        		continue;
        	}
            if (key.empty())
            {
                continue;
            }
            if (params.find(key) != params.end())
            {
                errorMessage = "Non-unique key";
                return false;
            }
            params[key] = value;
        }
        else
        {
            templateLines.push_back(line);
        }
    }

    if (!separatorFound || templateLines.empty())
    {
        errorMessage = "No template specified";
        return false;
    }

    for (std::size_t i = 0; i < templateLines.size(); ++i)
    {
        if (i > 0)
        {
            tpl += '\n';
        }
        tpl += templateLines[i];
    }

    return true;
}

bool ParseCommandLineParams(int argc, char* argv[], std::map<std::string, std::string>& params, std::string& errorMessage)
{
    params.clear();
    const int argsCount = argc - START_INDEX;
    if (argsCount % 2 != 0)
    {
        errorMessage = "Invalid input";
        return false;
    }
    for (int i = START_INDEX; i + 1 < argc; i += 2)
    {
        std::string key = argv[i];
        std::string value = argv[i + 1];
        if (key.empty())
        {
            continue;
        }
        if (params.find(key) != params.end())
        {
            errorMessage = "Non-unique key";
            return false;
        }
        params[key] = value;
    }
    return true;
}

bool OpenFiles(const std::string& inputFile, const std::string& outputFile, std::ifstream& inFile, std::ofstream& outFile)
{
    if (inputFile == outputFile)
    {
        return false;
    }

    inFile.open(inputFile);
    if (!inFile.is_open())
    {
        return false;
    }

    outFile.open(outputFile);
    if (!outFile.is_open())
    {
        inFile.close();
        return false;
    }

    return true;
}

int RunStdinMode()
{
    std::map<std::string, std::string> params;
    std::string tpl;
    std::string errorMessage;

    if (!ParseStdinParamsAndTemplate(std::cin, params, tpl, errorMessage))
    {
        std::cout << errorMessage;
        return EXIT_FAILURE;
    }

    std::cout << ExpandTemplate(tpl, params);
    return EXIT_SUCCESS;
}

int RunFilesMode(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "Invalid input";
        return EXIT_FAILURE;
    }

    std::string inputFileName = argv[1];
    std::string outputFileName = argv[2];

    std::ifstream inFile;
    std::ofstream outFile;

    if (!OpenFiles(inputFileName, outputFileName, inFile, outFile))
    {
        std::cout << "Invalid input";
        return EXIT_FAILURE;
    }

    std::map<std::string, std::string> params;
    std::string errorMessage;

    if (!ParseCommandLineParams(argc, argv, params, errorMessage))
    {
        std::cout << errorMessage;
        inFile.close();
        outFile.close();
        return EXIT_FAILURE;
    }

    const std::string tpl = ReadAll(inFile);
    const std::string result = ExpandTemplate(tpl, params);

    outFile << result;

    inFile.close();
    outFile.close();

    return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
    if (argc == 2 && IsHelpFlag(argv[1]))
    {
        PrintHelp(std::cout);
        return EXIT_SUCCESS;
    }

    if (argc == 1)
    {
        return RunStdinMode();
    }

    if (argc >= 3)
    {
        return RunFilesMode(argc, argv);
    }

    std::cout << "Invalid input";
    return EXIT_FAILURE;
}