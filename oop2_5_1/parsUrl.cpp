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
	UrlComponents components;

	if (!ParseURL(url, components))
	{
		std::cout << "ERROR" << std::endl;
		return EXIT_FAILURE;
	}

	 PrintResult(url, components);

	return EXIT_SUCCESS;
}