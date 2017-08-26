#include"Unit.h"
#include"Material.h"
#include"Area.h"

int		Unit::id() const
{
	return int(this - &units.front());
}
void	Unit::reset()
{
	state = State::Egg;
	enabled = false;
	unitType = -1;
	pos = Vec2::Zero();
	angle = Vec2::Right();
	joinedArea = nullptr;
	age = 0;
	v = Vec2::Zero();
	ownMaterials.resize(numMaterialTypes);
	ownMaterials.fill(0);
}
void	Unit::joinArea()
{
	getArea(pos)->units.emplace_back(this);
}
void	Unit::erase()
{
	if (joinedArea != nullptr) joinedArea->units.remove(this);
	reset();
}
void	Unit::die()
{

	const double s = 4.0;	//分割数
	const double f = 1.0;	//射出スピード

	for (int i = 0; i < int(ownMaterials.size()); i++)
	{
		auto& m = ownMaterials[i];
		if (m > 0)
		{
			for (;;)
			{
				if (m > s)
				{
					setMaterial(i, s, pos, f);
					m -= s;
				}
				else
				{
					setMaterial(i, m, pos, f);
					break;
				}
			}
		}
	}
	erase();
}
Unit*	Unit::makeChild()
{
	auto* u = newUnit();
	u->enabled = true;
	u->angle = RandomVec2();
	u->pos = pos;
	u->unitType = unitType;
	return u;
}
Unit*	Unit::newUnit()
{
	for (auto& u : units)
	{
		if (!u.enabled) return &u;
	}
	return nullptr;
}