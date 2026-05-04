#include "CCanvas.h"
#include <array>
#include <iostream>

const float BAZE_THICKNESS = 1.f;

namespace
{
	sf::Color ConvertColor(uint32_t color)
	{
		return sf::Color(
			static_cast<std::uint8_t>(color >> 16),
			static_cast<std::uint8_t>(color >> 8),
			static_cast<std::uint8_t>(color)
		);
	}

	sf::CircleShape CreateCircleShape(const CPoint& center, double radius)
		{
			sf::CircleShape circle(static_cast<float>(radius));

			circle.setPosition(sf::Vector2f(
				static_cast<float>(center.x - radius),
				static_cast<float>(center.y - radius)
			));

			return circle;
		}
}

CCanvas::CCanvas(sf::RenderTarget& target)
	: m_target(target)
{
}

void CCanvas::DrawLine(const CPoint& from, const CPoint& to, uint32_t lineColor)
{
	const sf::Color color = ConvertColor(lineColor);

	std::array<sf::Vertex, 2> line =
	{
		sf::Vertex{ sf::Vector2f(static_cast<float>(from.x), static_cast<float>(from.y)), color },
		sf::Vertex{ sf::Vector2f(static_cast<float>(to.x), static_cast<float>(to.y)), color }
	};

	m_target.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
}

void CCanvas::FillPolygon(const std::vector<CPoint>& points, uint32_t fillColor)
{
	if (points.empty())
	{
		return;
	}

	sf::ConvexShape polygon;
	polygon.setPointCount(points.size());

	for (std::size_t i = 0; i < points.size(); ++i)
	{
		polygon.setPoint(
			i,
			sf::Vector2f(
				static_cast<float>(points[i].x),
				static_cast<float>(points[i].y)
			)
		);
	}

	polygon.setFillColor(ConvertColor(fillColor));
	m_target.draw(polygon);
}

void CCanvas::DrawCircle(const CPoint& center, double radius, uint32_t lineColor)
{
	sf::CircleShape circle = CreateCircleShape(center, radius);
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineColor(ConvertColor(lineColor));
	circle.setOutlineThickness(BAZE_THICKNESS);
	m_target.draw(circle);
}

void CCanvas::FillCircle(const CPoint& center, double radius, uint32_t fillColor)
{
	sf::CircleShape circle = CreateCircleShape(center, radius);
	circle.setFillColor(ConvertColor(fillColor));
	m_target.draw(circle);
}