#include <catch2/catch_test_macros.hpp>

#include "CDate.h"

TEST_CASE("Конструктор")
{
	SECTION("По умолчанию")
	{
		CDate date;
		REQUIRE(date.GetDay() == 1);
		REQUIRE(date.GetMonth() == Month::JANUARY);
		REQUIRE(date.GetYear() == 1970);
		REQUIRE(date.GetWeekDay() == WeekDay::THURSDAY);
	}

	SECTION("день-месяц-год")
	{
		CDate date(25, Month::APRIL, 2026);

		REQUIRE(date.GetDay() == 25);
		REQUIRE(date.GetMonth() == Month::APRIL);
		REQUIRE(date.GetYear() == 2026);
	}

	SECTION("таймстамп")
	{
		CDate date(0);

		REQUIRE(date.GetDay() == 1);
		REQUIRE(date.GetMonth() == Month::JANUARY);
		REQUIRE(date.GetYear() == 1970);

		CDate date2(32);
		REQUIRE(date2.GetDay() == 2);
		REQUIRE(date2.GetMonth() == Month::FEBRUARY);
		REQUIRE(date2.GetYear() == 1970);
	}

	SECTION("Високосный год")
	{
		CDate date(29, Month::FEBRUARY, 1972);

		REQUIRE(date.GetDay() == 29);
	}

}

TEST_CASE("jтрицательные тесты")
{
	SECTION("день")
	{
		REQUIRE_THROWS(CDate(0, Month::JANUARY, 1970));
		REQUIRE_THROWS(CDate(32, Month::JANUARY, 1970));
	}

	SECTION("месяц")
	{
		REQUIRE_THROWS(CDate(1, static_cast<Month>(0), 1970));
		REQUIRE_THROWS(CDate(1, static_cast<Month>(13), 1970));
	}

	SECTION("год")
	{
		REQUIRE_THROWS(CDate(1, Month::JANUARY, 1969));
		REQUIRE_THROWS(CDate(1, Month::JANUARY, 10000));
	}

	SECTION("даты в месяцах")
	{
		REQUIRE_THROWS(CDate(31, Month::APRIL, 2020));
		REQUIRE_THROWS(CDate(31, Month::JUNE, 2020));
		REQUIRE_THROWS(CDate(31, Month::SEPTEMBER, 2020));
		REQUIRE_THROWS(CDate(31, Month::NOVEMBER, 2020));
	}

	SECTION("не високосный")
	{
		REQUIRE_THROWS(CDate(29, Month::FEBRUARY, 2021));
	}
}

TEST_CASE("Операторы")
{
    SECTION("Префиксный инкремент")
    {
        CDate date(31, Month::JANUARY, 2020);

        ++date;

        REQUIRE(date.GetDay() == 1);
        REQUIRE(date.GetMonth() == Month::FEBRUARY);
    }

    SECTION("Постфиксный инкремент")
    {
        CDate date(31, Month::JANUARY, 2020);

        CDate old = date++;

        REQUIRE(old.GetDay() == 31);
        REQUIRE(date.GetDay() == 1);
    }

    SECTION("Префиксный декремент")
    {
        CDate date(1, Month::MARCH, 2020);

        --date;

        REQUIRE(date.GetDay() == 29);
        REQUIRE(date.GetMonth() == Month::FEBRUARY);
    }

    SECTION("Постфиксный декремент")
    {
        CDate date(1, Month::MARCH, 2020);

        CDate old = date--;

        REQUIRE(old.GetDay() == 1);
        REQUIRE(date.GetDay() == 29);
    }

    SECTION("Сложение дат")
    {
        CDate date(28, Month::FEBRUARY, 2010);

        CDate result = date + 3;

        REQUIRE(result.GetDay() == 3);
        REQUIRE(result.GetMonth() == Month::MARCH);
    }

    SECTION("Сложение лней и дваты")
    {
        CDate date(28, Month::FEBRUARY, 2010);

        CDate result = 3 + date;

        REQUIRE(result.GetDay() == 3);
    }

    SECTION("Вычитание дней")
    {
        CDate date(1, Month::JANUARY, 2010);

        CDate result = date - 2;

        REQUIRE(result.GetDay() == 30);
        REQUIRE(result.GetMonth() == Month::DECEMBER);
    }

    SECTION("Разность дат")
    {
        CDate d1(1, Month::JANUARY, 2010);
        CDate d2(30, Month::DECEMBER, 2009);
        CDate d3(3, Month::JANUARY, 2010);

        REQUIRE(d1 - d2 == 2);
        REQUIRE(d1 - d3 == -2);
    }

    SECTION("Операторы += и -=")
    {
        CDate date(28, Month::FEBRUARY, 2010);

        date += 3;
        REQUIRE(date.GetDay() == 3);

        date -= 3;
        REQUIRE(date.GetDay() == 28);
    }

    SECTION("Сравнение дат")
    {
        CDate d1(1, Month::JANUARY, 2020);
        CDate d2(1, Month::JANUARY, 2020);
        CDate d3(2, Month::JANUARY, 2020);

        REQUIRE(d1 == d2);
        REQUIRE(d1 != d3);
        REQUIRE(d1 < d3);
        REQUIRE(d3 > d1);
        REQUIRE(d1 <= d2);
        REQUIRE(d3 >= d1);
    }

    SECTION("День недели")
    {
        CDate d1(1, Month::JANUARY, 1970);
        REQUIRE(d1.GetWeekDay() == WeekDay::THURSDAY);

        CDate d2(2, Month::JANUARY, 1970);
        REQUIRE(d2.GetWeekDay() == WeekDay::FRIDAY);
    }
}

TEST_CASE("jтрицательные тесты в операторах")
{

	SECTION("Выход за нижнюю границу через --")
	{
		CDate date(1, Month::JANUARY, 1970);

		REQUIRE_THROWS(--date);
	}

	SECTION("Выход за нижнюю границу через -")
	{
		CDate date(1, Month::JANUARY, 1970);

		REQUIRE_THROWS(date - 1);
	}
}