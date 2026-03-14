#include "Dictionary.h"
#include "ConsoleUI.h"

#include <iostream>
#include <string>

void HandleUnknownWord(DictionaryStorage& dictionary, const std::string& word, std::istream& in, bool& dictionaryChanged)
{
	PrintInfoForUser(MessageType::UnknownWord, word, "");
	std::string translation = ReadUserInput(in);

	if (translation.empty())
	{
		PrintInfoForUser(MessageType::WordIgnored, word, "");
		return;
	}

	AddTranslation(dictionary, word, translation);
	dictionaryChanged = true;

	PrintInfoForUser(MessageType::WordSaved, word, translation);
}

void ProcessUserQueries(DictionaryStorage& dictionary, std::istream& in, bool& dictionaryChanged)
{
	while (true)
	{
		std::string word = ReadUserInput(in);

		if (word == "...")
		{
			break;
		}

		TranslationResult translations = FindTranslation(dictionary.direct, word);

		if (!translations)
		{
			translations = FindTranslation(dictionary.reverse, word);
		}

		if (translations)
		{
			PrintInfoForUser(MessageType::Translation, "", JoinTranslations(translations->get()));
		}
		else
		{
			HandleUnknownWord(dictionary, word, in, dictionaryChanged);
		}
	}
}

void RunApp(const std::string& fileDictName, std::istream& in)
{
	DictionaryStorage dictionary;
	bool dictionaryChanged = false;

	LoadDictionary(fileDictName, dictionary);
	ProcessUserQueries(dictionary, in, dictionaryChanged);

	if (dictionaryChanged)
	{
		if (AskSaveChanges(in))
		{
			ExportDictionary(fileDictName, dictionary);
			PrintInfoForUser(MessageType::SaveSuccess, "", "");
		}
	}

	PrintInfoForUser(MessageType::Goodbye, "", "");
}

int main(int argc, char* argv[])
{
	std::setlocale(LC_ALL, "");

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