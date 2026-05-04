#include "CShapeStorage.h"
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include "CLineSegment.h"
#include "CCircle.h"
#include "CRectangle.h"
#include "CTriangle.h"

void CShapeStorage::AddShape(const std::shared_ptr<IShape>& shape)
{
	if (!shape)
	{
		throw std::invalid_argument("Shape is null");
	}
	m_shapes.push_back(shape);
}

void CShapeStorage::AddShapeFromString(const std::string& line)
{
	AddShape(ParseShape(line));
}

std::shared_ptr<IShape> CShapeStorage::GetShapeWithMaxArea() const
{
	if (m_shapes.empty())
	{
		return nullptr;
	}

	return *std::max_element(
		m_shapes.begin(),
		m_shapes.end(),
		[](const auto& lsh, const auto& rsh)
		{
			return lsh->GetArea() < rsh->GetArea();
		}
	);
}

std::shared_ptr<IShape> CShapeStorage::GetShapeWithMinPerimeter() const
{
	if (m_shapes.empty())
	{
		return nullptr;
	}

	return *std::min_element(
		m_shapes.begin(),
		m_shapes.end(),
		[](const auto& lsh, const auto& rsh)
		{
			return lsh-> GetPerimeter() < rsh-> GetPerimeter();
		}
	);
}

const std::vector<std::shared_ptr<IShape>>& CShapeStorage::GetShapes() const
{
	return m_shapes;
}

std::shared_ptr<IShape> CShapeStorage::ParseShape(const std::string& line) const
{
	std::istringstream input(line);
	std::string type;
	input >> type;

	if (type == "line")
	{
		double x1, y1, x2, y2;
		uint32_t outlineColor;

		if (input >> x1 >> y1 >> x2 >> y2 >> std::hex >> outlineColor)
		{
			return std::shared_ptr<IShape>(
				new CLineSegment(
					CPoint{ x1, y1 },
					CPoint{ x2, y2 },
					outlineColor
				)
			);
		}
	}
	else if (type == "circle")
	{
		double cx, cy, radius;
		uint32_t outlineColor, fillColor;

		if (input >> cx >> cy >> radius >> std::hex >> outlineColor >> fillColor)
		{
			return std::shared_ptr<IShape>(
				new CCircle(
					CPoint{ cx, cy },
					radius,
					outlineColor,
					fillColor
				)
			);
		}
	}
	else if (type == "rectangle")
	{
		double x, y, width, height;
		uint32_t outlineColor, fillColor;

		if (input >> x >> y >> width >> height >> std::hex >> outlineColor >> fillColor)
		{
			return std::shared_ptr<IShape>(
				new CRectangle(
					CPoint{ x, y },
					width,
					height,
					outlineColor,
					fillColor
				)
			);
		}
	}
	else if (type == "triangle")
	{
		double x1, y1, x2, y2, x3, y3;
		uint32_t outlineColor, fillColor;

		if (input >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> std::hex >> outlineColor >> fillColor)
		{
			return std::shared_ptr<IShape>(
				new CTriangle(
					CPoint{ x1, y1 },
					CPoint{ x2, y2 },
					CPoint{ x3, y3 },
					outlineColor,
					fillColor
				)
			);
		}
	}

	throw std::runtime_error("Unknown or invalid shape description: " + line);
}