#include <catch2/catch_test_macros.hpp>
#include "../ParseUrlModules.h"

TEST_CASE("Парсинг корректных URL")
{
	SECTION("С портом и файлом")
	{
		UrlComponents components;
		bool result = ParseURL("http://example.com:8080/index.html", components);

		CHECK(result == true);
		CHECK(components.protocol == Protocol::HTTP);
		CHECK(components.host == "example.com");
		CHECK(components.port == 8080);
		CHECK(components.document == "index.html");
	}

	SECTION("Без порта")
	{
		UrlComponents components;
		bool result = ParseURL("https://example.com/page.html", components);

		CHECK(result == true);
		CHECK(components.protocol == Protocol::HTTPS);
		CHECK(components.host == "example.com");
		CHECK(components.port == 443);
		CHECK(components.document == "page.html");
	}

	SECTION("Без файла")
	{
		UrlComponents components;
		bool result = ParseURL("ftp://example.com/path/to/page", components);

		CHECK(result == true);
		CHECK(components.protocol == Protocol::FTP);
		CHECK(components.host == "example.com");
		CHECK(components.port == 21);
		CHECK(components.document == "");
	}
}

TEST_CASE("Парсинг query и fragment")
{
	SECTION("Query без файла")
	{
		UrlComponents components;
		bool result = ParseURL("http://example.com/test?u=124.trt", components);

		CHECK(result == true);
		CHECK(components.document == "");
	}

	SECTION("Query с файлом")
	{
		UrlComponents components;
		bool result = ParseURL("http://example.com/docs/page.html?x=1", components);

		CHECK(result == true);
		CHECK(components.document == "page.html");
	}

	SECTION("Fragment с файлом")
	{
		UrlComponents components;
		bool result = ParseURL("http://example.com/docs/page.html#top", components);

		CHECK(result == true);
		CHECK(components.document == "page.html");
	}
}

TEST_CASE("Парсинг некорректных URL")
{
	SECTION("Неизвестный протокол")
	{
		UrlComponents components;
		CHECK(ParseURL("smtp://example.com/file.txt", components) == false);
	}

	SECTION("Порт меньше 1")
	{
		UrlComponents components;
		CHECK(ParseURL("http://example.com:0/file.txt", components) == false);
	}

	SECTION("Порт больше 65535")
	{
		UrlComponents components;
		CHECK(ParseURL("http://example.com:70000/file.txt", components) == false);
	}

	SECTION("Пустая строка")
	{
		UrlComponents components;
		CHECK(ParseURL("", components) == false);
	}
}