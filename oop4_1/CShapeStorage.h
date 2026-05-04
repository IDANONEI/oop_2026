#pragma once

#include <memory>
#include <string>
#include <vector>

#include "IShape.h"

class CShapeStorage
{
public:
	void AddShape(const std::shared_ptr<IShape>& shape);
	void AddShapeFromString(const std::string& line);

	std::shared_ptr<IShape> GetShapeWithMaxArea() const;
	std::shared_ptr<IShape> GetShapeWithMinPerimeter() const;

	const std::vector<std::shared_ptr<IShape>>& GetShapes() const;

private:
	std::shared_ptr<IShape> ParseShape(const std::string& line) const;
	std::vector<std::shared_ptr<IShape>> m_shapes;
};