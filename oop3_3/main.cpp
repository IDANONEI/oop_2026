#include "Simulation.h"

#include <iostream>
#include <string>
#include <stdexcept>

int main(int argc, char* argv[])
{
	try
	{
		size_t iterations = 0;

		if (argc > 2)
		{
			std::cerr << "Usage: economy [NUM_ITERATIONS]\n";
			return 1;
		}

		if (argc == 2)
		{
			const std::string arg = argv[1];
			size_t pos = 0;
			const unsigned long long value = std::stoull(arg, &pos);

			if (pos != arg.size())
			{
				std::cerr << "Invalid number of iterations\n";
				return 1;
			}

			iterations = static_cast<size_t>(value);
		}
		else
		{
			if (!(std::cin >> iterations))
			{
				std::cerr << "Failed to read number of iterations from stdin\n";
				return 1;
			}
		}

		Simulation simulation;
		simulation.Run(iterations);
		return 0;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << '\n';
		return 1;
	}
}