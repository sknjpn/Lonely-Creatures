#include"Unit.h"
#include"Material.h"
#include"Area.h"
#include"Object.h"

void	updateUnits()
{
	for (auto& u : units)
	{
		if (u.enabled)
		{
			u.age++;

			switch (u.unitType)
			{
			case 0:
				u.updateCreamatis();
				break;

			case 1:
				u.updateSlug();
				break;

			default:
				break;
			}
		}
	}
}
void	updateObjects()
{
	for (auto& o : objects)
	{
		if (o.enabled)
		{

		}
	}
}
void	Unit::updateCreamatis()
{
	switch (state)
	{
	case State::Egg:
		if (age > 60)
		{
			age = 0;
			state = State::Larva;
		}
		break;
	case State::Larva:
		if (m0 <= 1.0)
		{
			auto* m = nearestMaterial(pos, 0);
			if (m != nullptr && (m->pos - pos).length() < 32.0)
			{
				if (m->n > 1.0)
				{
					m0 += 1.0;
					m->n -= 1.0;
				}
				else
				{
					m0 += m->n;
					m->n = 0;
					m->erase();
				}
			}
		}

		if (m0 >= 0.1)
		{
			m1 += 0.1;
			m0 -= 0.1;
		}
		else
		{
			m1 += m0;
			m0 = 0;
			if (m1 > 0) setMaterial(0, m1, pos, 2);
			erase();
			return;
		}

		if (age > 60)
		{
			for (int i = 0; i < 2; i++)
			{
				auto* nu = newUnit();
				if (nu != nullptr)
				{
					nu->enabled = true;
					nu->angle = RandomVec2();
					nu->pos = pos + RandomVec2(32);
					nu->unitType = unitType;
					nu->age = Random(-120, 0);
					if (nu->pos.x < 0) nu->pos.x = 0;
					if (nu->pos.y < 0) nu->pos.y = 0;
					if (nu->pos.x > 1024) nu->pos.x = 1024;
					if (nu->pos.y > 1024) nu->pos.y = 1024;
				}
			}
			if (m0 > 0) setMaterial(0, m0, pos, 2);
			if (m1 > 0)
			{
				double s = 2.0;
				for (;;)
				{
					if (m1 > s)
					{
						setMaterial(1, s, pos, 2);
						m1 -= s;
					}
					else
					{
						setMaterial(1, m1, pos, 2);
						break;
					}
				}

			}
			erase();
			return;
		}
		break;
	default:
		break;
	}
}
void	Unit::updateSlug()
{

}