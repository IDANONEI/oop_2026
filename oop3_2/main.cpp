#include "Calculator.h"
#include "ControllerCalculator.h"
#include <iostream>
int main()
{
	Calculator calculator;
	ControllerCalculator controller(calculator, std::cin, std::cout);
	controller.Run();
	return 0;
}