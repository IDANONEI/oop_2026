#include <catch2/catch_test_macros.hpp>
#include <cmath>

#include "Calculator.h"

TEST_CASE("Объявление переменных")
{
	SECTION("Можно объявить новую переменную")
	{
		Calculator calculator;

		REQUIRE(calculator.DeclareVariable("x"));
		REQUIRE(calculator.Exists("x"));
		REQUIRE(calculator.IsVariable("x"));
		REQUIRE_FALSE(calculator.IsFunction("x"));
		REQUIRE(std::isnan(calculator.GetIdentifierValue("x")));
	}

	SECTION("Нельзя объявить переменную с уже существующим именем переменной")
	{
		Calculator calculator;
		calculator.DeclareVariable("x");

		REQUIRE_FALSE(calculator.DeclareVariable("x"));
	}

	SECTION("Нельзя объявить переменную с именем уже существующей функции")
	{
		Calculator calculator;
		calculator.DeclareVariable("x");
		calculator.DeclareFunction("f", "x");

		REQUIRE_FALSE(calculator.DeclareVariable("f"));
	}
}

TEST_CASE("Присваивание числового значения переменной")
{
	SECTION("Можно присвоить значение уже объявленной переменной")
	{
		Calculator calculator;
		calculator.DeclareVariable("x");

		REQUIRE(calculator.Assign("x", 42.5));
		REQUIRE(calculator.GetIdentifierValue("x") == 42.5);
	}

	SECTION("let автоматически создает новую переменную")
	{
		Calculator calculator;

		REQUIRE(calculator.Assign("x", 10.0));
		REQUIRE(calculator.Exists("x"));
		REQUIRE(calculator.IsVariable("x"));
		REQUIRE(calculator.GetIdentifierValue("x") == 10.0);
	}

	SECTION("Нельзя присвоить значение функции")
	{
		Calculator calculator;
		calculator.Assign("x", 5.0);
		calculator.DeclareFunction("f", "x");

		REQUIRE_FALSE(calculator.Assign("f", 100.0));
		REQUIRE(calculator.GetIdentifierValue("f") == 5.0);
	}
}

TEST_CASE("Присваивание значения одного идентификатора другому")
{
	SECTION("Можно присвоить переменной значение другой переменной")
	{
		Calculator calculator;
		calculator.Assign("x", 12.34);

		REQUIRE(calculator.Assign("y", "x"));
		REQUIRE(calculator.IsVariable("y"));
		REQUIRE(calculator.GetIdentifierValue("y") == 12.34);
	}

	SECTION("Можно присвоить переменной значение функции")
	{
		Calculator calculator;
		calculator.Assign("x", 10.0);
		calculator.Assign("y", 20.0);
		calculator.DeclareFunction("sum", "x", Operation::Add, "y");

		REQUIRE(calculator.Assign("z", "sum"));
		REQUIRE(calculator.GetIdentifierValue("z") == 30.0);
	}

	SECTION("Если источник не определен, присваивается nan")
	{
		Calculator calculator;
		calculator.DeclareVariable("x");

		REQUIRE(calculator.Assign("y", "x"));
		REQUIRE(std::isnan(calculator.GetIdentifierValue("y")));
	}

	SECTION("Нельзя присвоить значение от несуществующего идентификатора")
	{
		Calculator calculator;

		REQUIRE_FALSE(calculator.Assign("x", "unknown"));
		REQUIRE_FALSE(calculator.Exists("x"));
	}

	SECTION("Нельзя присвоить значение функции")
	{
		Calculator calculator;
		calculator.Assign("x", 7.0);
		calculator.DeclareFunction("f", "x");

		REQUIRE_FALSE(calculator.Assign("f", "x"));
		REQUIRE(calculator.GetIdentifierValue("f") == 7.0);
	}
}

TEST_CASE("Объявление простых функций")
{
	SECTION("Можно объявить функцию-алиас от переменной")
	{
		Calculator calculator;
		calculator.Assign("x", 15.0);

		REQUIRE(calculator.DeclareFunction("fx", "x"));
		REQUIRE(calculator.Exists("fx"));
		REQUIRE(calculator.IsFunction("fx"));
		REQUIRE_FALSE(calculator.IsVariable("fx"));
		REQUIRE(calculator.GetIdentifierValue("fx") == 15.0);
	}

	SECTION("Функция-алиас возвращает nan, если операнд не определен")
	{
		Calculator calculator;
		calculator.DeclareVariable("x");
		calculator.DeclareFunction("fx", "x");

		REQUIRE(std::isnan(calculator.GetIdentifierValue("fx")));
	}

	SECTION("Функция вычисляется по текущему значению идентификатора")
	{
		Calculator calculator;
		calculator.Assign("x", 10.0);
		calculator.DeclareFunction("fx", "x");

		REQUIRE(calculator.GetIdentifierValue("fx") == 10.0);

		REQUIRE(calculator.Assign("x", 25.0));
		REQUIRE(calculator.GetIdentifierValue("fx") == 25.0);
	}

	SECTION("Нельзя объявить функцию с уже существующим именем переменной")
	{
		Calculator calculator;
		calculator.Assign("x", 1.0);

		REQUIRE_FALSE(calculator.DeclareFunction("x", "x"));
	}

	SECTION("Нельзя объявить функцию с уже существующим именем функции")
	{
		Calculator calculator;
		calculator.Assign("x", 1.0);
		calculator.DeclareFunction("f", "x");

		REQUIRE_FALSE(calculator.DeclareFunction("f", "x"));
	}

	SECTION("Нельзя объявить функцию от несуществующего идентификатора")
	{
		Calculator calculator;

		REQUIRE_FALSE(calculator.DeclareFunction("f", "unknown"));
		REQUIRE_FALSE(calculator.Exists("f"));
	}
}

TEST_CASE("Объявление бинарных функций")
{
	SECTION("Сложение")
	{
		Calculator calculator;
		calculator.Assign("x", 2.0);
		calculator.Assign("y", 3.0);

		REQUIRE(calculator.DeclareFunction("sum", "x", Operation::Add, "y"));
		REQUIRE(calculator.GetIdentifierValue("sum") == 5.0);
	}

	SECTION("Вычитание")
	{
		Calculator calculator;
		calculator.Assign("x", 7.0);
		calculator.Assign("y", 2.0);

		REQUIRE(calculator.DeclareFunction("sub", "x", Operation::Subtract, "y"));
		REQUIRE(calculator.GetIdentifierValue("sub") == 5.0);
	}

	SECTION("Умножение")
	{
		Calculator calculator;
		calculator.Assign("x", 4.0);
		calculator.Assign("y", 2.5);

		REQUIRE(calculator.DeclareFunction("mul", "x", Operation::Multiply, "y"));
		REQUIRE(calculator.GetIdentifierValue("mul") == 10.0);
	}

	SECTION("Деление")
	{
		Calculator calculator;
		calculator.Assign("x", 10.0);
		calculator.Assign("y", 2.0);

		REQUIRE(calculator.DeclareFunction("div", "x", Operation::Divide, "y"));
		REQUIRE(calculator.GetIdentifierValue("div") == 5.0);
	}

	SECTION("Деление на ноль дает nan")
	{
		Calculator calculator;
		calculator.Assign("x", 10.0);
		calculator.Assign("y", 0.0);

		REQUIRE(calculator.DeclareFunction("div", "x", Operation::Divide, "y"));
		REQUIRE(std::isnan(calculator.GetIdentifierValue("div")));
	}

	SECTION("Если левый операнд не определен, результат nan")
	{
		Calculator calculator;
		calculator.DeclareVariable("x");
		calculator.Assign("y", 5.0);

		REQUIRE(calculator.DeclareFunction("sum", "x", Operation::Add, "y"));
		REQUIRE(std::isnan(calculator.GetIdentifierValue("sum")));
	}

	SECTION("Если правый операнд не определен, результат nan")
	{
		Calculator calculator;
		calculator.Assign("x", 5.0);
		calculator.DeclareVariable("y");

		REQUIRE(calculator.DeclareFunction("sum", "x", Operation::Add, "y"));
		REQUIRE(std::isnan(calculator.GetIdentifierValue("sum")));
	}

	SECTION("Нельзя объявить бинарную функцию, если левый операнд не существует")
	{
		Calculator calculator;
		calculator.Assign("y", 5.0);

		REQUIRE_FALSE(calculator.DeclareFunction("sum", "x", Operation::Add, "y"));
		REQUIRE_FALSE(calculator.Exists("sum"));
	}

	SECTION("Нельзя объявить бинарную функцию, если правый операнд не существует")
	{
		Calculator calculator;
		calculator.Assign("x", 5.0);

		REQUIRE_FALSE(calculator.DeclareFunction("sum", "x", Operation::Add, "y"));
		REQUIRE_FALSE(calculator.Exists("sum"));
	}
}

TEST_CASE("Функции могут зависеть от других функций")
{
	SECTION("Функция от функции")
	{
		Calculator calculator;
		calculator.Assign("x", 3.0);
		calculator.Assign("y", 4.0);

		calculator.DeclareFunction("sum", "x", Operation::Add, "y");
		calculator.Assign("z", 7.0);

		REQUIRE(calculator.DeclareFunction("result", "sum", Operation::Multiply, "z"));
		REQUIRE(calculator.GetIdentifierValue("result") == 49.0);
	}

	SECTION("Изменение переменной влияет на все зависящие функции")
	{
		Calculator calculator;
		calculator.Assign("x", 2.0);
		calculator.Assign("y", 3.0);

		calculator.DeclareFunction("sum", "x", Operation::Add, "y");
		calculator.DeclareFunction("doubleSum", "sum", Operation::Multiply, "x");

		REQUIRE(calculator.GetIdentifierValue("sum") == 5.0);
		REQUIRE(calculator.GetIdentifierValue("doubleSum") == 10.0);

		calculator.Assign("x", 10.0);

		REQUIRE(calculator.GetIdentifierValue("sum") == 13.0);
		REQUIRE(calculator.GetIdentifierValue("doubleSum") == 130.0);
	}
}

TEST_CASE("Получение списков переменных и функций")
{
	SECTION("Список переменных возвращается в алфавитном порядке")
	{
		Calculator calculator;
		calculator.Assign("b", 2.0);
		calculator.Assign("a", 1.0);
		calculator.DeclareVariable("c");

		const auto variables = calculator.GetSortedVariables();

		REQUIRE(variables.size() == 3);
		REQUIRE(variables[0].first == "a");
		REQUIRE(variables[0].second == 1.0);
		REQUIRE(variables[1].first == "b");
		REQUIRE(variables[1].second == 2.0);
		REQUIRE(variables[2].first == "c");
		REQUIRE(std::isnan(variables[2].second));
	}

	SECTION("Список функций возвращается в алфавитном порядке")
	{
		Calculator calculator;
		calculator.Assign("x", 2.0);
		calculator.Assign("y", 3.0);

		calculator.DeclareFunction("zFunc", "x");
		calculator.DeclareFunction("aFunc", "x", Operation::Add, "y");

		const auto functions = calculator.GetSortedFunctions();

		REQUIRE(functions.size() == 2);
		REQUIRE(functions[0].first == "aFunc");
		REQUIRE(functions[0].second == 5.0);
		REQUIRE(functions[1].first == "zFunc");
		REQUIRE(functions[1].second == 2.0);
	}

	SECTION("Если функция не определена, в списке будет nan")
	{
		Calculator calculator;
		calculator.DeclareVariable("x");
		calculator.DeclareFunction("f", "x");

		const auto functions = calculator.GetSortedFunctions();

		REQUIRE(functions.size() == 1);
		REQUIRE(functions[0].first == "f");
		REQUIRE(std::isnan(functions[0].second));
	}
}

TEST_CASE("Проверка существования идентификаторов")
{
	SECTION("Существующая переменная")
	{
		Calculator calculator;
		calculator.Assign("x", 1.0);

		REQUIRE(calculator.Exists("x"));
		REQUIRE(calculator.IsVariable("x"));
		REQUIRE_FALSE(calculator.IsFunction("x"));
	}

	SECTION("Существующая функция")
	{
		Calculator calculator;
		calculator.Assign("x", 1.0);
		calculator.DeclareFunction("f", "x");

		REQUIRE(calculator.Exists("f"));
		REQUIRE_FALSE(calculator.IsVariable("f"));
		REQUIRE(calculator.IsFunction("f"));
	}

	SECTION("Несуществующий идентификатор")
	{
		Calculator calculator;

		REQUIRE_FALSE(calculator.Exists("unknown"));
		REQUIRE_FALSE(calculator.IsVariable("unknown"));
		REQUIRE_FALSE(calculator.IsFunction("unknown"));
	}
}