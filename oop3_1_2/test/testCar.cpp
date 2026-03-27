#include <catch2/catch_test_macros.hpp>
#include <stdexcept>

#include "Car.h"

TEST_CASE("Движение вперед")
{
	SECTION("1 передача")
	{
		Car car;
		car.TurnOnEngine();
		car.SetGear(1);

		REQUIRE_NOTHROW(car.SetSpeed(10));
		REQUIRE(car.GetSpeed() == 10);
		REQUIRE(car.GetGear() == Car::Gear::First);
		REQUIRE(car.GetDirection() == Car::Direction::Forward);
	}

	SECTION("Заднюю передача при движении вперед")
	{
		Car car;
		car.TurnOnEngine();
		car.SetGear(1);
		car.SetSpeed(10);

		REQUIRE_THROWS_AS(car.SetGear(-1), std::logic_error);
		REQUIRE(car.GetGear() == Car::Gear::First);
		REQUIRE(car.GetDirection() == Car::Direction::Forward);
	}

	SECTION("Остановка - standing still")
	{
		Car car;
		car.TurnOnEngine();
		car.SetGear(1);
		car.SetSpeed(10);
		car.SetSpeed(0);

		REQUIRE(car.GetSpeed() == 0);
		REQUIRE(car.GetDirection() == Car::Direction::StandingStill);
	}

	SECTION("На нейтрали после движения вперед можно замедляться")
	{
		Car car;
		car.TurnOnEngine();
		car.SetGear(1);
		car.SetSpeed(20);
		car.SetGear(0);

		REQUIRE_NOTHROW(car.SetSpeed(10));
		REQUIRE(car.GetSpeed() == 10);
		REQUIRE(car.GetDirection() == Car::Direction::Forward);
	}

	SECTION("Остановка -> 1 передачу")
	{
		Car car;
		car.TurnOnEngine();
		car.SetGear(-1);
		car.SetSpeed(10);
		car.SetGear(0);
		car.SetSpeed(0);

		REQUIRE_NOTHROW(car.SetGear(1));
		REQUIRE(car.GetGear() == Car::Gear::First);
	}
}

TEST_CASE("Движение назад")
{
	SECTION("Назад на задней передаче")
	{
		Car car;
		car.TurnOnEngine();
		car.SetGear(-1);

		REQUIRE_NOTHROW(car.SetSpeed(10));
		REQUIRE(car.GetSpeed() == 10);
		REQUIRE(car.GetGear() == Car::Gear::Reverse);
		REQUIRE(car.GetDirection() == Car::Direction::Backward);
	}

	SECTION("Передняя передачу при движении назад")
	{
		Car car;
		car.TurnOnEngine();
		car.SetGear(-1);
		car.SetSpeed(10);
		car.SetGear(0);

		REQUIRE_THROWS_AS(car.SetGear(1), std::logic_error);
		REQUIRE(car.GetGear() == Car::Gear::Neutral);
		REQUIRE(car.GetDirection() == Car::Direction::Backward);
	}


	SECTION("На нейтрали можно замедляться")
	{
		Car car;
		car.TurnOnEngine();
		car.SetGear(-1);
		car.SetSpeed(15);
		car.SetGear(0);

		REQUIRE_NOTHROW(car.SetSpeed(5));
		REQUIRE(car.GetSpeed() == 5);
		REQUIRE(car.GetDirection() == Car::Direction::Backward);
	}

	SECTION("standing still")
	{
		Car car;
		car.TurnOnEngine();
		car.SetGear(-1);
		car.SetSpeed(15);
		car.SetGear(0);
		car.SetSpeed(0);

		REQUIRE(car.GetSpeed() == 0);
		REQUIRE(car.GetDirection() == Car::Direction::StandingStill);
	}
}

TEST_CASE("Корректные состояния")
{
	SECTION("Начальное состояние")
	{
		Car car;

		REQUIRE_FALSE(car.IsTurnedOn());
		REQUIRE(car.GetGear() == Car::Gear::Neutral);
		REQUIRE(car.GetSpeed() == 0);
		REQUIRE(car.GetDirection() == Car::Direction::StandingStill);
	}

	SECTION("включить двигатель")
	{
		Car car;

		REQUIRE_NOTHROW(car.TurnOnEngine());
		REQUIRE(car.IsTurnedOn());
	}

	SECTION("Нельзя выключить двигатель на не нейтральной передаче")
	{
		Car car;
		car.TurnOnEngine();
		car.SetGear(1);

		REQUIRE_THROWS_AS(car.TurnOffEngine(), std::logic_error);
		REQUIRE(car.IsTurnedOn());
	}

	SECTION("Нельзя выключить двигатель при не нулевой скорости")
	{
		Car car;
		car.TurnOnEngine();
		car.SetGear(1);
		car.SetSpeed(10);

		REQUIRE_THROWS_AS(car.TurnOffEngine(), std::logic_error);
		REQUIRE(car.IsTurnedOn());
	}

	SECTION("нетралка")
	{
		Car car;

		REQUIRE_NOTHROW(car.SetGear(0));
		REQUIRE(car.GetGear() == Car::Gear::Neutral);
	}

	SECTION("нетралка при выключенном")
	{
		Car car;

		REQUIRE_THROWS_AS(car.SetGear(1), std::logic_error);
		REQUIRE_THROWS_AS(car.SetGear(-1), std::logic_error);
		REQUIRE(car.GetGear() == Car::Gear::Neutral);
	}

	SECTION("отрицательная скорость")
	{
		Car car;
		car.TurnOnEngine();
		car.SetGear(1);

		REQUIRE_THROWS_AS(car.SetSpeed(-1), std::logic_error);
		REQUIRE(car.GetSpeed() == 0);
	}

	SECTION("установить скорость при выключенном двигателе")
	{
		Car car;

		REQUIRE_THROWS_AS(car.SetSpeed(10), std::logic_error);
		REQUIRE(car.GetSpeed() == 0);
	}
}

TEST_CASE("Разгон")
{
	SECTION("диапазон 1")
	{
		Car car;
		car.TurnOnEngine();
		car.SetGear(1);

		REQUIRE_NOTHROW(car.SetSpeed(30));
		REQUIRE(car.GetSpeed() == 30);
	}

	SECTION("вышке диапазона 1")
	{
		Car car;
		car.TurnOnEngine();
		car.SetGear(1);

		REQUIRE_THROWS_AS(car.SetSpeed(31), std::logic_error);
		REQUIRE(car.GetSpeed() == 0);
	}

	SECTION("диапазон 2")
	{
		Car car;
		car.TurnOnEngine();
		car.SetGear(1);
		car.SetSpeed(20);

		REQUIRE_NOTHROW(car.SetGear(2));
		REQUIRE(car.GetGear() == Car::Gear::Second);
	}

	SECTION("недостаточно диапазон 2")
	{
		Car car;
		car.TurnOnEngine();
		car.SetGear(1);
		car.SetSpeed(19);

		REQUIRE_THROWS_AS(car.SetGear(2), std::logic_error);
		REQUIRE(car.GetGear() == Car::Gear::First);
	}

	SECTION("нельзя снизить скорость ниже диапазона второй передачи")
	{
		Car car;
		car.TurnOnEngine();
		car.SetGear(1);
		car.SetSpeed(20);
		car.SetGear(2);

		REQUIRE_THROWS_AS(car.SetSpeed(10), std::logic_error);
		REQUIRE(car.GetSpeed() == 20);
	}

	SECTION("до пятой передачи")
	{
		Car car;
		car.TurnOnEngine();

		car.SetGear(1);
		car.SetSpeed(30);

		REQUIRE_NOTHROW(car.SetGear(2));
		car.SetSpeed(50);

		REQUIRE_NOTHROW(car.SetGear(3));
		car.SetSpeed(60);

		REQUIRE_NOTHROW(car.SetGear(4));
		car.SetSpeed(90);

		REQUIRE_NOTHROW(car.SetGear(5));
		REQUIRE(car.GetGear() == Car::Gear::Fifth);
		REQUIRE(car.GetSpeed() == 90);
		REQUIRE(car.GetDirection() == Car::Direction::Forward);
	}

}