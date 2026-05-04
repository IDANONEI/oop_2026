//
// Created by  Daniil on 11.04.2026.
//

#include "CCircle.h"
#include <sstream>


CCircle::CCircle(
	CPoint center,
	double radius,
	uint32_t outlineColor,
	uint32_t fillColor
)
	: CSolidShape(outlineColor, fillColor)
	, m_center(std::move(center))
	, m_radius(radius)
{
	if (radius <= 0)
	{
		throw std::invalid_argument("Circle radius must be positive");
	}
}
double CCircle::GetArea() const
{
	return M_PI * m_radius * m_radius;
}

double CCircle::GetPerimeter() const
{
	return 2.0 * M_PI * m_radius;
}

double CCircle::GetRadius() const
{
	return m_radius;
}

CPoint CCircle::GetCenter() const
{
	return m_center;
}

std::string CCircle::ToString() const
{
	std::ostringstream strm;
	strm << "Circle: center=" << m_center.ToString()
		 << ", radius=" << m_radius;
	return strm.str();
}


void CCircle::Draw(ICanvas& canvas) const
{
	canvas.FillCircle(m_center, m_radius, GetFillColor());
	canvas.DrawCircle(m_center, m_radius, GetOutlineColor());
}
