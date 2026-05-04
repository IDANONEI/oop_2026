#include "CTriangle.h"
#include <vector>
#include <cmath>
#include <sstream>
#include <stdexcept>

namespace
{
	double GetDoubleTriangleArea(const CPoint& a, const CPoint& b, const CPoint& c)
	{
		return std::abs(
			(b.x - a.x) * (c.y - a.y) -
			(b.y - a.y) * (c.x - a.x)
		);
	}
}

CTriangle::CTriangle(
	CPoint pointA,
	CPoint pointB,
	CPoint pointC,
	uint32_t outlineColor,
	uint32_t fillColor
)
	: CSolidShape(outlineColor, fillColor)
	, m_pointA(std::move(pointA))
	, m_pointB(std::move(pointB))
	, m_pointC(std::move(pointC))
{
	if (GetDoubleTriangleArea(m_pointA, m_pointB, m_pointC) <= 0.0)
	{
		throw std::invalid_argument("Triangle vertices must form a non-degenerate triangle");
	}
}

double CTriangle::GetPerimeter() const
{
	const double sideAB = m_pointA.DistanceTo(m_pointB);
	const double sideBC = m_pointB.DistanceTo(m_pointC);
	const double sideCA = m_pointC.DistanceTo(m_pointA);

	return sideAB + sideBC + sideCA;
}

double CTriangle::GetArea() const
{
	const double sideAB = m_pointA.DistanceTo(m_pointB);
	const double sideBC = m_pointB.DistanceTo(m_pointC);
	const double sideCA = m_pointC.DistanceTo(m_pointA);
	const double semiPerimeter = GetPerimeter() / 2.0;

	return std::sqrt(
		semiPerimeter *
		(semiPerimeter - sideAB) *
		(semiPerimeter - sideBC) *
		(semiPerimeter - sideCA)
	);
}

std::string CTriangle::ToString() const
{
	std::ostringstream strm;
	strm << "Triangle: A=" << m_pointA.ToString()
		 << ", B=" << m_pointB.ToString()
		 << ", C=" << m_pointC.ToString();
	return strm.str();
}

const CPoint& CTriangle::GetPointA() const
{
	return m_pointA;
}

const CPoint& CTriangle::GetPointB() const
{
	return m_pointB;
}

const CPoint& CTriangle::GetPointC() const
{
	return m_pointC;
}

void CTriangle::Draw(ICanvas& canvas) const
{
	canvas.FillPolygon(
		{m_pointA,m_pointB,m_pointC},
		GetFillColor()
	);

	canvas.DrawLine(m_pointA, m_pointB, GetOutlineColor());
	canvas.DrawLine(m_pointB, m_pointC, GetOutlineColor());
	canvas.DrawLine(m_pointC, m_pointA, GetOutlineColor());
}

