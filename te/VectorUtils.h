
#pragma once

#include <iostream>
#include <vector>

std::vector<double> ReadNumbers(std::istream& input);
void MultiplyElementsByMin(std::vector<double>& numbers);
void SortedVector(std::vector<double>& numbers);
void PrintSortedVector(std::vector<double> numbers, std::ostream& output);