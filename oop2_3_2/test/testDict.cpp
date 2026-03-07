#include <catch2/catch_test_macros.hpp>
#include "../Dictionary.h"

TEST_CASE("Поиск перевода слова")
{
	DictionaryMap dict;
	DictionaryMap reverseDict;

	AddTranslation(dict, reverseDict, "board", "доска");
	AddTranslation(dict, reverseDict, "board", "плата");
	AddTranslation(dict, reverseDict, "board", "щит");

	SECTION("Слово есть в словаре")
	{
		const auto* result = FindTranslation(dict, "board");

		REQUIRE(result != nullptr);
		CHECK(result->size() == 3);
		CHECK(result->contains("доска"));
		CHECK(result->contains("плата"));
		CHECK(result->contains("щит"));
	}

	SECTION("Слово вводится в другом регистре")
	{
		const auto* result = FindTranslation(dict, "bOaRd");

		REQUIRE(result != nullptr);
		CHECK(result->contains("доска"));
	}

	SECTION("Слова нет в словаре")
	{
		CHECK(FindTranslation(dict, "fish") == nullptr);
	}

	SECTION("Пустой словарь")
	{
		DictionaryMap emptyDict;
		CHECK(FindTranslation(emptyDict, "board") == nullptr);
	}
}

TEST_CASE("Добавление перевода")
{
	std::string board = "board";

	SECTION("Добавление нового слова")
	{
		DictionaryMap dict;
		DictionaryMap reverseDict;

		AddTranslation(dict, reverseDict, board, "доска");

		REQUIRE(dict.count("board") == 1);
		CHECK(dict["board"].size() == 1);
		CHECK(dict["board"].contains("доска"));

		REQUIRE(reverseDict.count("доска") == 1);
		CHECK(reverseDict["доска"].contains("board"));
	}

	SECTION("Одинаковый перевод не добавляется дважды")
	{
		DictionaryMap dict;
		DictionaryMap reverseDict;

		AddTranslation(dict, reverseDict, board, "доска");
		AddTranslation(dict, reverseDict, board, "доска");

		REQUIRE(dict.count("board") == 1);
		CHECK(dict["board"].size() == 1);
	}

	SECTION("Несколько переводов одного слова")
	{
		DictionaryMap dict;
		DictionaryMap reverseDict;

		AddTranslation(dict, reverseDict, board, "доска");
		AddTranslation(dict, reverseDict, board, "плата");

		REQUIRE(dict.count("board") == 1);
		CHECK(dict["board"].size() == 2);
		CHECK(dict["board"].contains("доска"));
		CHECK(dict["board"].contains("плата"));
	}

	SECTION("Слово в разном регистре считается одним и тем же")
	{
		DictionaryMap dict;
		DictionaryMap reverseDict;

		AddTranslation(dict, reverseDict, "board", "доска");
		AddTranslation(dict, reverseDict, "BoArD", "щит");

		REQUIRE(dict.size() == 1);
		CHECK(dict["board"].size() == 2);
	}
}

TEST_CASE("Словарь с несколькими словами")
{
	DictionaryMap dict;
	DictionaryMap reverseDict;

	AddTranslation(dict, reverseDict, "board", "доска");
	AddTranslation(dict, reverseDict, "cat", "кот");
	AddTranslation(dict, reverseDict, "cat", "кошка");

	SECTION("Переводы первого слова")
	{
		const auto* result = FindTranslation(dict, "board");

		REQUIRE(result != nullptr);
		CHECK(result->size() == 1);
		CHECK(result->contains("доска"));
	}

	SECTION("Переводы второго слова")
	{
		const auto* result = FindTranslation(dict, "cat");

		REQUIRE(result != nullptr);
		CHECK(result->size() == 2);
		CHECK(result->contains("кот"));
		CHECK(result->contains("кошка"));
	}

	SECTION("Неизвестное слово")
	{
		CHECK(FindTranslation(dict, "dog") == nullptr);
	}
}

TEST_CASE("Обратный перевод")
{
	DictionaryMap dict;
	DictionaryMap reverseDict;

	AddTranslation(dict, reverseDict, "cat", "кот");
	AddTranslation(dict, reverseDict, "cat", "кошка");

	SECTION("Русское слово переводится обратно")
	{
		const auto* result = FindTranslation(reverseDict, "кот");

		REQUIRE(result != nullptr);
		CHECK(result->contains("cat"));
	}

	SECTION("Разные слова на одно английское")
	{
		const auto* result1 = FindTranslation(reverseDict, "кот");
		const auto* result2 = FindTranslation(reverseDict, "кошка");

		REQUIRE(result1 != nullptr);
		REQUIRE(result2 != nullptr);

		CHECK(result1->contains("cat"));
		CHECK(result2->contains("cat"));
	}
}


TEST_CASE("Отрицательные сценарии")
{
	DictionaryMap dict;
	DictionaryMap reverseDict;
	SECTION("Поиск пустой строки")
	{
		CHECK(FindTranslation(dict, "") == nullptr);
	}
	SECTION("Добавление пустого перевода")
	{
		AddTranslation(dict, reverseDict, "board", "");
		REQUIRE(dict.count("board") == 1);
		CHECK(dict["board"].contains(""));
	}
	SECTION("Поиск в пустом reverse словаре")
	{
		CHECK(FindTranslation(reverseDict, "кот") == nullptr);
	}
	SECTION("Поиск неизвестного слова после добавления других")
	{
		AddTranslation(dict, reverseDict, "board", "доска");
		AddTranslation(dict, reverseDict, "cat", "кот");

		CHECK(FindTranslation(dict, "dog") == nullptr);
	}
}