#pragma once

#include "CPoint.h"
#include "CShape.h"

class CLineSegment : public CShape
{
public:
	CLineSegment(
		CPoint startPoint,
		CPoint endPoint,
		uint32_t outlineColor = DEFAULT_COLOR_OUTLINE
	);

	double GetArea() const override;
	double GetPerimeter() const override;
	std::string ToString() const override;


	const CPoint& GetStartPoint() const;
	const CPoint& GetEndPoint() const;

	void Draw(ICanvas& canvas) const override;

private:
	CPoint m_startPoint;
	CPoint m_endPoint;
};