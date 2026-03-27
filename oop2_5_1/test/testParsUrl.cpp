#include <catch2/catch_test_macros.hpp>
#include "../ParseURLModules.h"

TEST_CASE("Парсинг корректных URL")
{
	SECTION("С портом и файлом")
	{
		auto result = GetParseURL("http://example.com:8080/index.html");

		REQUIRE(result.has_value());
		CHECK(result->protocol == Protocol::HTTP);
		CHECK(result->host == "example.com");
		CHECK(result->port == 8080);
		CHECK(result->document == "index.html");
	}

	SECTION("Без порта")
	{
		auto result = GetParseURL("https://example.com/page.html");

		REQUIRE(result.has_value());
		CHECK(result->protocol == Protocol::HTTPS);
		CHECK(result->host == "example.com");
		CHECK(result->port == 443);
		CHECK(result->document == "page.html");
	}

	SECTION("Без файла")
	{
		auto result = GetParseURL("ftp://example.com/path/to/page");

		REQUIRE(result.has_value());
		CHECK(result->protocol == Protocol::FTP);
		CHECK(result->host == "example.com");
		CHECK(result->port == 21);
		CHECK(result->document == "");
	}
}

TEST_CASE("query")
{
	SECTION("Query без файла")
	{
		auto result = GetParseURL("http://example.com/test?u=124.trt");

		REQUIRE(result.has_value());
		CHECK(result->document == "");
	}

	SECTION("Query с файлом")
	{
		auto result = GetParseURL("http://example.com/docs/page.html?x=1");

		REQUIRE(result.has_value());
		CHECK(result->document == "page.html");
	}

	SECTION("Fragment с файлом")
	{
		auto result = GetParseURL("http://example.com/docs/page.html#top");

		REQUIRE(result.has_value());
		CHECK(result->document == "page.html");
	}
}

TEST_CASE("Парсинг некорректных URL")
{
	SECTION("Неизвестный протокол")
	{
		auto result = GetParseURL("smtp://example.com/file.txt");
		CHECK_FALSE(result.has_value());
	}

	SECTION("Порт меньше 1")
	{
		auto result = GetParseURL("http://example.com:0/file.txt");
		CHECK_FALSE(result.has_value());
	}

	SECTION("Порт больше 65535")
	{
		auto result = GetParseURL("http://example.com:65536/file.txt");
		CHECK_FALSE(result.has_value());
	}

	SECTION("Порт содержит буквы")
	{
		auto result = GetParseURL("http://example.com:80a/file.txt");
		CHECK_FALSE(result.has_value());
	}

	SECTION("Пустая строка")
	{
		auto result = GetParseURL("");
		CHECK_FALSE(result.has_value());
	}
}

TEST_CASE("Парсинг URL с граничными портами")
{
	SECTION("Минимальный допустимый порт")
	{
		auto result = GetParseURL("http://example.com:1/file.txt");
		REQUIRE(result.has_value());
		CHECK(result->port == 1);
	}

	SECTION("Максимальный допустимый порт")
	{
		auto result = GetParseURL("http://example.com:65535/file.txt");
		REQUIRE(result.has_value());
		CHECK(result->port == 65535);
	}
}