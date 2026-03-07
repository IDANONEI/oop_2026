#include <catch2/catch_test_macros.hpp>
#include "../ConsoleUI.h"
#include <sstream>

TEST_CASE("Проверка аргументов командной строки")
{
	SECTION("Аргумент передан")
	{
		CHECK(CheckDictionaryFileArgument(2) == true);
	}
	SECTION("Аргумент не передан")
	{
		CHECK(CheckDictionaryFileArgument(1) == false);
	}
}

TEST_CASE("Чтение пользовательского ввода")
{
	SECTION("Считывается строка из потока")
	{
		std::stringstream input("hello\n");
		std::string result = ReadUserInput(input);
		CHECK(result == "hello");
	}
	SECTION("Пустая строка")
	{
		std::stringstream input("\n");
		std::string result = ReadUserInput(input);
		CHECK(result == "");
	}
}

TEST_CASE("Объединение переводов")
{
	SECTION("Несколько переводов")
	{
		std::unordered_set<std::string> translations = { "кот", "кошка" };
		std::string result = JoinTranslations(translations);

		CHECK((result == "кот, кошка" ||  result == "кошка, кот"));
	}
	SECTION("Один перевод")
	{
		std::unordered_set<std::string> translations = { "кот" };
		std::string result = JoinTranslations(translations);
		CHECK(result == "кот");
	}
}

TEST_CASE("Запрос сохранения словаря")
{
	SECTION("y")
	{
		std::stringstream input("y\n");
		CHECK(AskSaveChanges(input) == true);
	}
	SECTION("Y")
	{
		std::stringstream input("Y\n");
		CHECK(AskSaveChanges(input) == true);
	}

	SECTION("что-то другое")
	{
		std::stringstream input("n\n");
		CHECK(AskSaveChanges(input) == false);
	}
}