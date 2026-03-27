#include <catch2/catch_test_macros.hpp>
#include <map>
#include <string>

#include "../processor.h"

TEST_CASE("Базовая подстановка параметров")
{
	SECTION("Один параметр")
	{
		std::string tpl = "Hello, %USER_NAME%!";
		std::map<std::string, std::string> params =
		{
			{"%USER_NAME%", "Ivan"}
		};

		auto result = ExpandTemplate(tpl, params);

		CHECK(result == "Hello, Ivan!");
	}

	SECTION("Несколько параметров")
	{
		std::string tpl = "Hello, %USER_NAME%. Today is {DAY}.";
		std::map<std::string, std::string> params =
		{
			{"%USER_NAME%", "Ivan"},
			{"{DAY}", "Friday"}
		};

		auto result = ExpandTemplate(tpl, params);

		CHECK(result == "Hello, Ivan. Today is Friday.");
	}

	SECTION("Несколько вхождений одного параметра")
	{
		std::string tpl = "%A% and %A% again %A%";
		std::map<std::string, std::string> params =
		{
			{"%A%", "X"}
		};

		auto result = ExpandTemplate(tpl, params);

		CHECK(result == "X and X again X");
	}
}

TEST_CASE("Пустые и граничные случаи")
{
	SECTION("Пустой шаблон")
	{
		std::string tpl;
		std::map<std::string, std::string> params =
		{
			{"A", "B"}
		};

		auto result = ExpandTemplate(tpl, params);

		CHECK(result.empty());
	}

	SECTION("Пустой набор параметров")
	{
		std::string tpl = "Nothing should change";
		std::map<std::string, std::string> params;

		auto result = ExpandTemplate(tpl, params);

		CHECK(result == "Nothing should change");
	}

	SECTION("Шаблон без совпадений")
	{
		std::string tpl = "Hello world";
		std::map<std::string, std::string> params =
		{
			{"ABC", "X"},
			{"DEF", "Y"}
		};

		auto result = ExpandTemplate(tpl, params);

		CHECK(result == "Hello world");
	}

	SECTION("Пустое значение параметра")
	{
		std::string tpl = "Hello, %USER_NAME%!";
		std::map<std::string, std::string> params =
		{
			{"%USER_NAME%", ""}
		};

		auto result = ExpandTemplate(tpl, params);

		CHECK(result == "Hello, !");
	}

	SECTION("Пустой ключ игнорируется")
	{
		std::string tpl = "ABC";
		std::map<std::string, std::string> params =
		{
			{"", "XXX"},
			{"A", "1"}
		};

		auto result = ExpandTemplate(tpl, params);

		CHECK(result == "1BC");
	}
}

TEST_CASE("Выбор наиболее длинного совпадения")
{
	SECTION("Простой случай")
	{
		std::string tpl = "AA";
		std::map<std::string, std::string> params =
		{
			{"A", "[a]"},
			{"AA", "[aa]"}
		};

		auto result = ExpandTemplate(tpl, params);

		CHECK(result == "[aa]");
	}

	SECTION("ТЗ")
	{
		std::string tpl = "-AABBCCCCCABC+";
		std::map<std::string, std::string> params =
		{
			{"A", "[a]"},
			{"AA", "[aa]"},
			{"B", "[b]"},
			{"BB", "[bb]"},
			{"C", "[c]"},
			{"CC", "[cc]"}
		};

		auto result = ExpandTemplate(tpl, params);

		CHECK(result == "-[aa][bb][cc][cc][c][a][b][c]+");
	}

	SECTION("Пересекающиеся шаблоны")
	{
		std::string tpl = "ABCD";
		std::map<std::string, std::string> params =
		{
			{"AB", "X"},
			{"ABC", "Y"},
			{"BC", "Z"}
		};

		auto result = ExpandTemplate(tpl, params);

		CHECK(result == "YD");
	}
}

TEST_CASE("Подстановка не выполняется повторно")
{
	SECTION("Значение само содержит ключ")
	{
		std::string tpl = "%USER_NAME%";
		std::map<std::string, std::string> params =
		{
			{"%USER_NAME%", "Super %USER_NAME%"}
		};

		auto result = ExpandTemplate(tpl, params);

		CHECK(result == "Super %USER_NAME%");
	}

	SECTION("Пример с вложенными параметрами")
	{
		std::string tpl = "Hello, %USER_NAME%. Today is {WEEK_DAY}.";
		std::map<std::string, std::string> params =
		{
			{"%USER_NAME%", "Super %USER_NAME% {WEEK_DAY}"},
			{"{WEEK_DAY}", "Friday. {WEEK_DAY}"}
		};

		auto result = ExpandTemplate(tpl, params);

		CHECK(result == "Hello, Super %USER_NAME% {WEEK_DAY}. Today is Friday. {WEEK_DAY}.");
	}
}

TEST_CASE("Спецсимволы и произвольные ключи")
{
	SECTION("Ключи с символами процента и фигурными скобками")
	{
		std::string tpl = "%USER_NAME% lives in {CITY}.";
		std::map<std::string, std::string> params =
		{
			{"%USER_NAME%", "Ivan"},
			{"{CITY}", "Moscow"}
		};

		auto result = ExpandTemplate(tpl, params);

		CHECK(result == "Ivan lives in Moscow.");
	}

	SECTION("Ключи с пробелами")
	{
		std::string tpl = "I like very cold tea.";
		std::map<std::string, std::string> params =
		{
			{"very cold", "hot"}
		};

		auto result = ExpandTemplate(tpl, params);

		CHECK(result == "I like hot tea.");
	}

	SECTION("Ключи со знаками пунктуации")
	{
		std::string tpl = "a+b=c";
		std::map<std::string, std::string> params =
		{
			{"a+b", "x"}
		};

		auto result = ExpandTemplate(tpl, params);

		CHECK(result == "x=c");
	}
}

TEST_CASE("Многострочный шаблон")
{
	SECTION("Замена в нескольких строках")
	{
		std::string tpl = "Hello, %USER_NAME%.\nToday is {DAY}.";
		std::map<std::string, std::string> params =
		{
			{"%USER_NAME%", "Ivan"},
			{"{DAY}", "Friday"}
		};

		auto result = ExpandTemplate(tpl, params);

		CHECK(result == "Hello, Ivan.\nToday is Friday.");
	}

	SECTION("Несколько одинаковых параметров в разных строках")
	{
		std::string tpl = "%X%\n%X%\n%X%";
		std::map<std::string, std::string> params =
		{
			{"%X%", "42"}
		};

		auto result = ExpandTemplate(tpl, params);

		CHECK(result == "42\n42\n42");
	}
}