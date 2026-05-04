#pragma once


#include "CPoint.h"
#include "CSolidShape.h"

class CCircle : public CSolidShape
{
public:
	CCircle(
		CPoint center,
		double radius,
		uint32_t outlineColor = DEFAULT_COLOR_OUTLINE,
		uint32_t fillColor = DEFAULT_COLOR_FILL
	);

	double GetArea() const override;
	double GetPerimeter() const override;
	std::string ToString() const override;

	double GetRadius() const;
	CPoint GetCenter() const;
	void Draw(ICanvas& canvas) const override;

private:
	CPoint m_center;
	double m_radius;
};