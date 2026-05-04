#include "CLineSegment.h"
#include <sstream>

CLineSegment::CLineSegment(
	CPoint startPoint,
	CPoint endPoint,
	uint32_t outlineColor
)
	: CShape(outlineColor)
	, m_startPoint(std::move(startPoint))
	, m_endPoint(std::move(endPoint))
{
	if (m_startPoint.DistanceTo(m_endPoint) == 0.0)
	{
		throw std::invalid_argument("Line segment endpoints must not coincide");
	}
}

double CLineSegment::GetArea() const
{
	return 0.0;
}

double CLineSegment::GetPerimeter() const
{
	return m_startPoint.DistanceTo(m_endPoint);
}


std::string CLineSegment::ToString() const
{
	std::ostringstream strm;
	strm << "LineSegment: "
		 << "start=" << m_startPoint.ToString()
		 << ", end=" << m_endPoint.ToString();
	return strm.str();
}


void CLineSegment::Draw(ICanvas& canvas) const
{
	canvas.DrawLine(m_startPoint, m_endPoint, GetOutlineColor());
}
