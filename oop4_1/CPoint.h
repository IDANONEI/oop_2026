#pragma once
#include <sstream>
#include <string>
#include <cmath>

struct CPoint
{
	double x = 0.0;
	double y = 0.0;

	double DistanceTo(const CPoint& point2) const
	{
		return std::sqrt(
			(x - point2.x) * (x - point2.x) +
			(y - point2.y) * (y - point2.y));
	}

	std::string ToString() const
	{
		std::ostringstream strm;
		strm << "(" << x << ", " << y << ")";
		return strm.str();
	}
};