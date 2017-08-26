#include"Area.h"

Point	getAreaPos(const Vec2& _pos)
{
	return { int(_pos.x / areaWidth),int(_pos.y / areaWidth) };
}
Area*	getArea(const Vec2& _pos)
{
	return &areas.at(getAreaPos(_pos));
}