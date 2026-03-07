#include "Dictionary.h"
#include "ConsoleUI.h"

#include <iostream>
#include <string>

void HandleUnknownWord(DictionaryMap& dict, DictionaryMap& reverseDict, const std::string& word, std::istream& in, bool& dictionaryChanged)
{
	PrintInfoForUser(MessageType::UnknownWord, word, "");
	std::string translation = ReadUserInput(in);

	if (translation.empty())
	{
		PrintInfoForUser(MessageType::WordIgnored, word, "");
		return;
	}

	AddTranslation(dict, reverseDict, word, translation);
	dictionaryChanged = true;

	PrintInfoForUser(MessageType::WordSaved, word, translation);
}

void ProcessUserQueries(DictionaryMap& dict, DictionaryMap& reverseDict, std::istream& in, bool& dictionaryChanged)
{
	while (true)
	{
		std::string word = ReadUserInput(in);

		if (word == "...")
		{
			break;
		}

		const auto* translations = FindTranslation(dict, word);

		if (translations == nullptr)
		{
			translations = FindTranslation(reverseDict, word);
		}

		if (translations != nullptr)
		{
			PrintInfoForUser(MessageType::Translation,"",JoinTranslations(*translations));
		}
		else
		{
			HandleUnknownWord(dict, reverseDict, word, in, dictionaryChanged);
		}
	}
}

void RunApp(const std::string& fileDictName, std::istream& in)
{
	DictionaryMap dict;
	DictionaryMap reverseDict;
	bool dictionaryChanged = false;

	LoadDictionary(fileDictName, dict, reverseDict);
	ProcessUserQueries(dict, reverseDict, in, dictionaryChanged);

	if (dictionaryChanged)
	{
		if (AskSaveChanges(in))
		{
			ExportDictionary(fileDictName, dict);
			PrintInfoForUser(MessageType::SaveSuccess, "", "");
		}
	}

	PrintInfoForUser(MessageType::Goodbye, "", "");
}


int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");

	if (!CheckDictionaryFileArgument(argc))
	{
		return 1;
	}
	try
	{
		std::string fileDictName = argv[1];
		RunApp(fileDictName, std::cin);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}