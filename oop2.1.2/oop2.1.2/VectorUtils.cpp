#include "VectorUtils.h"

#include <algorithm>
#include <iomanip>
#include <sstream>
namespace {
    bool ParseToken(const std::string& token, double& value)
    {
        std::istringstream iss(token);
        iss >> value;
        return iss && iss.eof(); 
    }
}

std::vector<double> ReadNumbers(std::istream& input)
{
    std::vector<double> numbers;
    std::string token;
    double value = 0;

    while (input >> token)
    {
        
        if (!ParseToken(token, value))
        {
            throw std::runtime_error("Invalid input");
        }
        numbers.push_back(value);
    }

    return numbers;
}


void MultiplyElementsByMin(std::vector<double>& numbers)
{
    if (numbers.empty())
    {
        return;
    }
    const double minElement = *std::min_element(numbers.begin(), numbers.end());
    std::transform(numbers.begin(), numbers.end(), numbers.begin(), [minElement](double num) { return num * minElement; });
}

void SortedVector(std::vector<double>& numbers)
{
    std::sort(numbers.begin(), numbers.end());
}

void PrintSortedVector(std::vector<double> numbers, std::ostream& output)
{
    if (numbers.empty())
    {
        return;
    }

    SortedVector(numbers);
    output << std::fixed << std::setprecision(3);
    for (size_t i = 0; i < numbers.size(); ++i)
    {
        if (i) output << ' ';
        output << numbers[i];
    }
}