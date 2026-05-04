#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include <string>

#include "CCanvas.h"
#include "IShape.h"
#include "CShapeStorage.h"

namespace
{
void PrintShapeInfo(const std::string& title, const std::shared_ptr<IShape>& shape)
{
		if (!shape)
		{
			std::cout << title << ": not found\n";
			return;
		}

		std::cout << title << ":\n";
		std::cout << "  Area: " << shape->GetArea() << "\n";
		std::cout << "  Perimeter: " << shape->GetPerimeter() << "\n";
		std::cout << "  Outline color: 0x" << std::hex << shape->GetOutlineColor() << std::dec << "\n";
		std::cout << "  " << shape->ToString() << "\n";
		std::cout << "\n";
	}
}

int main()
{
	CShapeStorage storage;
	std::string line;
	while (std::getline(std::cin, line))
	{
		if (line.empty())
		{
			continue;
		}
		try
		{
			storage.AddShapeFromString(line);
		}
		catch (const std::exception& e)
		{
			std::cerr << "Parsing error: " << e.what() << "\n";
		}
	}

	if (storage.GetShapes().empty())
	{
		std::cout << "No shapes were loaded\n";
		return 0;
	}

	PrintShapeInfo("Shape with maximum area", storage.GetShapeWithMaxArea());
	PrintShapeInfo("Shape with minimum perimeter", storage.GetShapeWithMinPerimeter());

	sf::RenderWindow window(sf::VideoMode({ 1000, 800 }), "Geometric Shapes");
	CCanvas canvas(window);

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		window.clear(sf::Color::White);

		for (const auto& shape : storage.GetShapes())
		{
			shape->Draw(canvas);
		}

		window.display();
	}

	return 0;
}