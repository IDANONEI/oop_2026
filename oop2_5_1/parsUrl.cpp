#include <iostream>
#include <string>
#include "ParseUrlModules.h"

void PrintResult(const std::string& url, const UrlComponents& components)
{
	std::cout << "Parsing URL : " << url << std::endl;
	PrintComponents(components);
}

int main()
{
	std::string url;
	std::getline(std::cin, url);
	auto result = GetParseURL(url);
	if (result)
	{
		PrintResult(url, *result);
	}
	else
	{
		std::cout << "Invalid URL" << std::endl;
	}

	return EXIT_SUCCESS;
}