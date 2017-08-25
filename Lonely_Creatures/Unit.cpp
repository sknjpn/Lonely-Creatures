#include"Unit.h"
#include"Material.h"
#include"Area.h"

int		Unit::id() const
{
	return int(this - &units.front());
}
Point	Unit::getAreaPos() const
{
	return { int(pos.x / areaWidth),int(pos.y / areaWidth) };
}
void	Unit::reset()
{
	state = State::Egg;
	m0 = 0;
	m1 = 0;
	enabled = false;
	unitType = -1;
	pos = Vec2::Zero();
	angle = Vec2::Right();
	joinedArea = nullptr;
	age = 0;
}
void	Unit::joinArea()
{
	areas.at(getAreaPos()).units.emplace_back(this);
}
void	Unit::erase()
{
	if (joinedArea != nullptr) joinedArea->units.remove(this);
	reset();
}
Unit*	newUnit()
{
	for (auto& u : units)
	{
		if (!u.enabled) return &u;
	}
	return nullptr;
}