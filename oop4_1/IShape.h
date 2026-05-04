#pragma once
#include <string>
#include <cstdint>
#include "ICanvasDrawable.h"

class IShape : public virtual ICanvasDrawable
{
public:
	virtual ~IShape() = default;

	virtual double GetArea() const = 0;
	virtual double GetPerimeter() const = 0;
	virtual std::string ToString() const = 0;
	virtual uint32_t GetOutlineColor() const = 0;
};


//обьснить более понятно про виртуальноле наследование