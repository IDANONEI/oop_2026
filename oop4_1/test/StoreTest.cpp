#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "CShapeStorage.h"

TEST_CASE("CShapeStorage")
{
	SECTION("Добавление фигур из строки")
	{
		CShapeStorage storage;

		storage.AddShapeFromString("circle 0 0 5 ff0000 00ff00");
		storage.AddShapeFromString("rectangle 0 0 4 5 ff0000 00ff00");

		REQUIRE(storage.GetShapes().size() == 2);
	}

	SECTION("Максимальная площадь")
	{
		CShapeStorage storage;

		// circle: площадь ~78.5
		storage.AddShapeFromString("circle 0 0 5 ff0000 00ff00");

		// rectangle: площадь 20
		storage.AddShapeFromString("rectangle 0 0 4 5 ff0000 00ff00");

		auto shape = storage.GetShapeWithMaxArea();

		REQUIRE(shape != nullptr);
		REQUIRE(shape->GetArea() == Catch::Approx(M_PI * 25));
	}

	SECTION("Минимальный периметр")
	{
		CShapeStorage storage;

		// circle: периметр ~31.4
		storage.AddShapeFromString("circle 0 0 5 ff0000 00ff00");

		// rectangle: периметр 18
		storage.AddShapeFromString("rectangle 0 0 4 5 ff0000 00ff00");

		auto shape = storage.GetShapeWithMinPerimeter();

		REQUIRE(shape != nullptr);
		REQUIRE(shape->GetPerimeter() == Catch::Approx(18));
	}

	SECTION("Пустое хранилище")
	{
		CShapeStorage storage;

		REQUIRE(storage.GetShapeWithMaxArea() == nullptr);
		REQUIRE(storage.GetShapeWithMinPerimeter() == nullptr);
	}

	SECTION("Некорректный ввод")
	{
		CShapeStorage storage;

		REQUIRE_THROWS_AS(
			storage.AddShapeFromString("unknown 1 2 3"),
			std::runtime_error
		);
	}

	SECTION("Несколько фигур — корректный выбор")
	{
		CShapeStorage storage;

		storage.AddShapeFromString("circle 0 0 2 ff0000 00ff00");
		storage.AddShapeFromString("rectangle 0 0 3 3 ff0000 00ff00");
		storage.AddShapeFromString("circle 0 0 4 ff0000 00ff00");

		auto maxArea = storage.GetShapeWithMaxArea();

		REQUIRE(maxArea != nullptr);
		REQUIRE(maxArea->GetArea() == Catch::Approx(M_PI * 16));
	}
}