#include <iostream>
#include "Car.h"
#include "CarController.h"

int main()
{
	Car car;
	CarController controller(car, std::cin, std::cout);

	while (controller.HandleCommand())
	{
	}

	return 0;
}