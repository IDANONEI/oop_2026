#include <catch2/catch_test_macros.hpp>
#include <cmath>

#include "Calculator.h"
#include "Function.h"
#include "Operation.h"

TEST_CASE("Кэш функции")
{
	SECTION("После сохранения значения кэш становится валидным")
	{
		Function function("f", "x");

		function.SetCachedValue(42.5);

		REQUIRE(function.IsCacheValid());
		REQUIRE(function.GetCachedValue() == 42.5);
	}

	SECTION("После инвалидирования кэш становится невалидным")
	{
		Function function("f", "x");

		function.SetCachedValue(10.0);
		REQUIRE(function.IsCacheValid());
		function.InvalidateCache();
		REQUIRE_FALSE(function.IsCacheValid());
	}
}

TEST_CASE("Калькулятор использует кэш и сбрасывает его при изменении зависимостей")
{
	SECTION("После первого вычисления значение функции остается тем же при повторном запросе")
	{
		Calculator calculator;
		calculator.Assign("x", 2.0);
		calculator.Assign("y", 3.0);
		calculator.DeclareFunction("sum", "x", Operation::Add, "y");

		double firstValue = calculator.GetIdentifierValue("sum");
		double secondValue = calculator.GetIdentifierValue("sum");
		double thirdValue = calculator.GetIdentifierValue("sum");

		REQUIRE(firstValue == 5.0);
		REQUIRE(secondValue == 5.0);
		REQUIRE(thirdValue == 5.0);
	}

	SECTION("После изменения переменной зависимая функция пересчитывается")
	{
		Calculator calculator;
		calculator.Assign("x", 2.0);
		calculator.Assign("y", 3.0);
		calculator.DeclareFunction("sum", "x", Operation::Add, "y");

		REQUIRE(calculator.GetIdentifierValue("sum") == 5.0);

		calculator.Assign("x", 10.0);

		REQUIRE(calculator.GetIdentifierValue("sum") == 13.0);
	}

	SECTION("После изменения переменной пересчитываются все зависимые функции по цепочке")
	{
		Calculator calculator;
		calculator.Assign("x", 2.0);
		calculator.Assign("y", 3.0);

		calculator.DeclareFunction("sum", "x", Operation::Add, "y");
		calculator.DeclareFunction("mul", "sum", Operation::Multiply, "x");
		calculator.DeclareFunction("result", "mul", Operation::Add, "y");

		REQUIRE(calculator.GetIdentifierValue("sum") == 5.0);
		REQUIRE(calculator.GetIdentifierValue("mul") == 10.0);
		REQUIRE(calculator.GetIdentifierValue("result") == 13.0);

		calculator.Assign("x", 4.0);

		REQUIRE(calculator.GetIdentifierValue("sum") == 7.0);
		REQUIRE(calculator.GetIdentifierValue("mul") == 28.0);
		REQUIRE(calculator.GetIdentifierValue("result") == 31.0);
	}
}

TEST_CASE("Оптимизация вычислений на цепочках функций")
{
	SECTION("Цепочка функций вычисляется корректно после повторных обращений")
	{
		Calculator calculator;
		calculator.Assign("v0", 0.0);
		calculator.Assign("v1", 1.0);

		calculator.DeclareFunction("fib0", "v0");
		calculator.DeclareFunction("fib1", "v1");
		calculator.DeclareFunction("fib2", "fib1", Operation::Add, "fib0");
		calculator.DeclareFunction("fib3", "fib2", Operation::Add, "fib1");
		calculator.DeclareFunction("fib4", "fib3", Operation::Add, "fib2");
		calculator.DeclareFunction("fib5", "fib4", Operation::Add, "fib3");
		calculator.DeclareFunction("fib6", "fib5", Operation::Add, "fib4");

		REQUIRE(calculator.GetIdentifierValue("fib6") == 8.0);
		REQUIRE(calculator.GetIdentifierValue("fib6") == 8.0);
		REQUIRE(calculator.GetIdentifierValue("fib6") == 8.0);
	}

	SECTION("После изменения исходной переменной цепочка функций пересчитывается корректно")
	{
		Calculator calculator;
		calculator.Assign("v0", 0.0);
		calculator.Assign("v1", 1.0);

		calculator.DeclareFunction("fib0", "v0");
		calculator.DeclareFunction("fib1", "v1");
		calculator.DeclareFunction("fib2", "fib1", Operation::Add, "fib0");
		calculator.DeclareFunction("fib3", "fib2", Operation::Add, "fib1");
		calculator.DeclareFunction("fib4", "fib3", Operation::Add, "fib2");
		calculator.DeclareFunction("fib5", "fib4", Operation::Add, "fib3");
		calculator.DeclareFunction("fib6", "fib5", Operation::Add, "fib4");

		REQUIRE(calculator.GetIdentifierValue("fib6") == 8.0);

		calculator.Assign("v0", 1.0);

		REQUIRE(calculator.GetIdentifierValue("fib6") == 13.0);
	}
}


TEST_CASE("fib50")
{
	Calculator calculator;

	calculator.Assign("v0", 0.0);
	calculator.Assign("v1", 1.0);

	calculator.DeclareFunction("fib0", "v0");
	calculator.DeclareFunction("fib1", "v1");

	const int n = 50;

	for (int i = 2; i <= n; ++i)
	{
		calculator.DeclareFunction(
			"fib" + std::to_string(i),
			"fib" + std::to_string(i - 1),
			Operation::Add,
			"fib" + std::to_string(i - 2)
		);
	}
	REQUIRE(calculator.GetIdentifierValue("fib50") == 12586269025.0);
}