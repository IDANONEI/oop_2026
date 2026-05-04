#pragma once

#include "CPoint.h"
#include "CSolidShape.h"

class CRectangle : public CSolidShape
{
public:
	CRectangle(
		CPoint leftTop,
		double width,
		double height,
		uint32_t outlineColor = DEFAULT_COLOR_OUTLINE,
		uint32_t fillColor = DEFAULT_COLOR_FILL
	);

	double GetArea() const override;
	double GetPerimeter() const override;
	std::string ToString() const override;


	double GetWidth() const;
	double GetHeight() const;
	CPoint GetLeftTop() const;
	void Draw(ICanvas& canvas) const override;

private:
	CPoint m_leftTop;
	double m_width;
	double m_height;
};