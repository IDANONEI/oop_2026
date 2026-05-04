//
// Created by  Daniil on 11.04.2026.
//

#include "CRectangle.h"
#include <vector>
#include <sstream>
#include <stdexcept>

CRectangle::CRectangle(
	CPoint leftTop,
	double width,
	double height,
	uint32_t outlineColor,
	uint32_t fillColor
)
	: CSolidShape(outlineColor, fillColor)
	, m_leftTop(std::move(leftTop))
	, m_width(width)
	, m_height(height)
{
	if (width <= 0)
	{
		throw std::invalid_argument("Rectangle width must be positive");
	}
	if (height <= 0)
	{
		throw std::invalid_argument("Rectangle height must be positive");
	}
}

double CRectangle::GetArea() const
{
	return m_width * m_height;
}

double CRectangle::GetPerimeter() const
{
	return 2.0 * (m_width + m_height);
}

std::string CRectangle::ToString() const
{
	std::ostringstream strm;
	strm << "Rectangle: leftTop=" << m_leftTop.ToString()
		 << ", width=" << m_width
		 << ", height=" << m_height;
	return strm.str();
}

double CRectangle::GetWidth() const
{
	return m_width;
}

double CRectangle::GetHeight() const
{
	return m_height;
}

CPoint CRectangle::GetLeftTop() const
{
	return m_leftTop;
}

void CRectangle::Draw(ICanvas& canvas) const
{
	CPoint lt = m_leftTop;
	CPoint rt{ lt.x + m_width, lt.y };
	CPoint rb{ lt.x + m_width, lt.y + m_height };
	CPoint lb{ lt.x, lt.y + m_height };

	std::vector<CPoint> points = { lt, rt, rb, lb };

	canvas.FillPolygon(points, GetFillColor());

	canvas.DrawLine(lt, rt, GetOutlineColor());
	canvas.DrawLine(rt, rb, GetOutlineColor());
	canvas.DrawLine(rb, lb, GetOutlineColor());
	canvas.DrawLine(lb, lt, GetOutlineColor());
}
