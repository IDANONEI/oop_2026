#include "ConsoleUI.h"
#include <iostream>
#include <string>
#include <unordered_set>

bool CheckDictionaryFileArgument(int argc)
{
	if (argc < 2)
	{
		std::cout << "Не указано имя файла словаря." << std::endl;
		return false;
	}
	return true;
}

void PrintInfoForUser(MessageType conditional, const std::string& key, const std::string& translation)
{
	switch (conditional)
	{
	case MessageType::Translation:
		std::cout << translation << std::endl;
		break;

	case MessageType::UnknownWord:
		std::cout << "Неизвестное слово \"" << key
				  << "\". Введите перевод или пустую строку для отказа."
				  << std::endl;
		break;

	case MessageType::WordSaved:
		std::cout << "Слово \"" << key
				  << "\" сохранено в словаре как \"" << translation << "\"."
				  << std::endl;
		break;

	case MessageType::WordIgnored:
		std::cout << "Слово \"" << key << "\" проигнорировано."
				  << std::endl;
		break;

	case MessageType::SaveChanges:
		std::cout << "В словарь были внесены изменения. "
					 "Введите Y или y для сохранения перед выходом."
				  << std::endl;
		break;

	case MessageType::SaveSuccess:
		std::cout << "Изменения сохранены." << std::endl;
		break;

	case MessageType::Goodbye:
		std::cout << "До свидания." << std::endl;
		break;
	}
}

std::string ReadUserInput(std::istream& in)
{
	std::string userInput;
	std::getline(in, userInput);
	return userInput;
}

std::string JoinTranslations(const std::unordered_set<std::string>& translations)
{
	std::string result;
	bool isFirst = true;

	for (const auto& translation : translations)
	{
		if (!isFirst)
		{
			result += ", ";
		}
		result += translation;
		isFirst = false;
	}

	return result;
}

bool AskSaveChanges(std::istream& in)
{
	PrintInfoForUser(MessageType::SaveChanges, "", "");
	std::string answer = ReadUserInput(in);
	return answer == "Y" || answer == "y";
}