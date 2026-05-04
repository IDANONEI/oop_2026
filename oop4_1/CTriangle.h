#pragma once

#include "CPoint.h"
#include "CSolidShape.h"

class CTriangle : public CSolidShape
{
public:
	CTriangle(
	    CPoint pointA,
		CPoint pointB,
		CPoint pointC,
		uint32_t outlineColor = DEFAULT_COLOR_OUTLINE,
		uint32_t fillColor = DEFAULT_COLOR_FILL
	);

	double GetArea() const override;
	double GetPerimeter() const override;
	std::string ToString() const override;


	const CPoint& GetPointA() const;
	const CPoint& GetPointB() const;
	const CPoint& GetPointC() const;
	void Draw(ICanvas& canvas) const override;

private:
	CPoint m_pointA;
	CPoint m_pointB;
	CPoint m_pointC;
};