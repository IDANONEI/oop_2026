#include <catch2/catch_test_macros.hpp>
#include "../Dictionary.h"

TEST_CASE("Поиск перевода слова")
{
	DictionaryStorage dictionary;

	AddTranslation(dictionary, "board", "доска");
	AddTranslation(dictionary, "board", "плата");
	AddTranslation(dictionary, "board", "щит");

	SECTION("Слово есть в словаре")
	{
		auto result = FindTranslation(dictionary.direct, "board");

		REQUIRE(result);
		CHECK(result->get().size() == 3);
		CHECK(result->get().contains("доска"));
		CHECK(result->get().contains("плата"));
		CHECK(result->get().contains("щит"));
	}

	SECTION("Слово вводится в другом регистре")
	{
		#include <clocale>
		std::setlocale(LC_ALL, "");

		auto result = FindTranslation(dictionary.direct, "bOaRd");

		REQUIRE(result);
		CHECK(result->get().contains("доска"));
	}

	SECTION("Русское в другом регистре")
	{
		auto result = FindTranslation(dictionary.reverse, "ДоСкА");

		REQUIRE(result);
		CHECK(result->get().contains("board"));
	}

	SECTION("Слова нет в словаре")
	{
		CHECK_FALSE(FindTranslation(dictionary.direct, "fish"));
	}

	SECTION("Пустой словарь")
	{
		DictionaryMap emptyDict;
		CHECK_FALSE(FindTranslation(emptyDict, "board"));
	}
}

TEST_CASE("Добавление перевода")
{
	std::string board = "board";

	SECTION("Добавление нового слова")
	{
		DictionaryStorage dictionary;

		AddTranslation(dictionary, board, "доска");

		REQUIRE(dictionary.direct.count("board") == 1);
		CHECK(dictionary.direct["board"].size() == 1);
		CHECK(dictionary.direct["board"].contains("доска"));

		REQUIRE(dictionary.reverse.count("доска") == 1);
		CHECK(dictionary.reverse["доска"].contains("board"));
	}

	SECTION("Одинаковый перевод не добавляется дважды")
	{
		DictionaryStorage dictionary;

		AddTranslation(dictionary, board, "доска");
		AddTranslation(dictionary, board, "доска");

		REQUIRE(dictionary.direct.count("board") == 1);
		CHECK(dictionary.direct["board"].size() == 1);
	}

	SECTION("Несколько переводов одного слова")
	{
		DictionaryStorage dictionary;

		AddTranslation(dictionary, board, "доска");
		AddTranslation(dictionary, board, "плата");

		REQUIRE(dictionary.direct.count("board") == 1);
		CHECK(dictionary.direct["board"].size() == 2);
		CHECK(dictionary.direct["board"].contains("доска"));
		CHECK(dictionary.direct["board"].contains("плата"));
	}

	SECTION("Слово в разном регистре считается одним и тем же")
	{
		DictionaryStorage dictionary;

		AddTranslation(dictionary, "board", "доска");
		AddTranslation(dictionary, "BoArD", "щит");

		REQUIRE(dictionary.direct.size() == 1);
		CHECK(dictionary.direct["board"].size() == 2);
	}
}

TEST_CASE("Словарь с несколькими словами")
{
	DictionaryStorage dictionary;

	AddTranslation(dictionary, "board", "доска");
	AddTranslation(dictionary, "cat", "кот");
	AddTranslation(dictionary, "cat", "кошка");

	SECTION("Переводы первого слова")
	{
		auto result = FindTranslation(dictionary.direct, "board");

		REQUIRE(result);
		CHECK(result->get().size() == 1);
		CHECK(result->get().contains("доска"));
	}

	SECTION("Переводы второго слова")
	{
		auto result = FindTranslation(dictionary.direct, "cat");

		REQUIRE(result);
		CHECK(result->get().size() == 2);
		CHECK(result->get().contains("кот"));
		CHECK(result->get().contains("кошка"));
	}

	SECTION("Неизвестное слово")
	{
		CHECK_FALSE(FindTranslation(dictionary.direct, "dog"));
	}
}

TEST_CASE("Обратный перевод")
{
	DictionaryStorage dictionary;

	AddTranslation(dictionary, "cat", "кот");
	AddTranslation(dictionary, "cat", "кошка");

	SECTION("Русское слово переводится обратно")
	{
		auto result = FindTranslation(dictionary.reverse, "кот");

		REQUIRE(result);
		CHECK(result->get().contains("cat"));
	}

	SECTION("Разные слова на одно английское")
	{
		auto result1 = FindTranslation(dictionary.reverse, "кот");
		auto result2 = FindTranslation(dictionary.reverse, "кошка");

		REQUIRE(result1);
		REQUIRE(result2);

		CHECK(result1->get().contains("cat"));
		CHECK(result2->get().contains("cat"));
	}
}

TEST_CASE("Отрицательные сценарии")
{
	DictionaryStorage dictionary;

	SECTION("Поиск пустой строки")
	{
		CHECK_FALSE(FindTranslation(dictionary.direct, ""));
	}

	SECTION("Добавление пустого перевода")
	{
		AddTranslation(dictionary, "board", "");
		REQUIRE(dictionary.direct.count("board") == 1);
		CHECK(dictionary.direct["board"].contains(""));
	}

	SECTION("Поиск в пустом reverse словаре")
	{
		CHECK_FALSE(FindTranslation(dictionary.reverse, "кот"));
	}

	SECTION("Поиск неизвестного слова после добавления других")
	{
		AddTranslation(dictionary, "board", "доска");
		AddTranslation(dictionary, "cat", "кот");

		CHECK_FALSE(FindTranslation(dictionary.direct, "dog"));
	}


}

TEST_CASE("Загрузка несуществующего файла")
{
	DictionaryStorage dictionary;

	REQUIRE_NOTHROW(LoadDictionary("not_exist.txt", dictionary));
	CHECK(dictionary.direct.empty());
	CHECK(dictionary.reverse.empty());
}