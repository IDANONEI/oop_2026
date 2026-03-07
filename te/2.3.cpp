#include "VectorUtils.h"

int main()
{
	try
	{
		auto numbers = ReadNumbers(std::cin);
		MultiplyElementsByMin(numbers);
		PrintSortedVector(numbers, std::cout);
	}
	catch (const std::exception&)
	{
		std::cout << "ERROR";
		return 0;
	}


	return EXIT_SUCCESS;
}