//
// Created by  Daniil on 11.04.2026.
//
#pragma once
#include "IShape.h"

constexpr uint32_t DEFAULT_COLOR_OUTLINE = 0xffffff;

class CShape : public virtual IShape
{
public:
	explicit CShape(uint32_t outlineColor = DEFAULT_COLOR_OUTLINE)
		: m_outlineColor(outlineColor)
	{
	}

	uint32_t GetOutlineColor() const override
	{
		return m_outlineColor;
	}

private:
	uint32_t m_outlineColor;
};