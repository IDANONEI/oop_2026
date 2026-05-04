//
// Created by  Daniil on 11.04.2026.
//

#pragma once
#include "CShape.h"
#include "ISolidShape.h"
constexpr uint32_t DEFAULT_COLOR_FILL = 0x000000;

class CSolidShape : public CShape, public ISolidShape
{
public:
	CSolidShape(
		uint32_t outlineColor,
		uint32_t fillColor = DEFAULT_COLOR_FILL
	)
		: CShape(outlineColor)
		, m_fillColor(fillColor)
	{
	}

	uint32_t GetFillColor() const override
	{
		return m_fillColor;
	}

private:
	uint32_t m_fillColor;
};