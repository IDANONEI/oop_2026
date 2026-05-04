#pragma once
#include "IShape.h"

class ISolidShape : public virtual IShape
{
public:
	virtual ~ISolidShape() = default;

	virtual uint32_t GetFillColor() const = 0;
};



