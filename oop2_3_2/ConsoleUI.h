#pragma once
#include <iostream>
#include <string>
#include <unordered_set>

bool CheckDictionaryFileArgument(int argc);

enum class MessageType
{
	Translation,
	UnknownWord,
	WordSaved,
	WordIgnored,
	SaveChanges,
	SaveSuccess,
	Goodbye
};

void PrintInfoForUser(MessageType conditional, const std::string& key = "", const std::string& translation = "");
std::string ReadUserInput(std::istream& in);
std::string JoinTranslations(const std::unordered_set<std::string>& translations);
bool AskSaveChanges(std::istream& in);
