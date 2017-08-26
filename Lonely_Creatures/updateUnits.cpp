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

		//マテリアル回収
		for (;;)
		{
			const double r = 0.1;
			if (ownMaterials[0] <= r)
			{
				auto* m = nearestMaterial(pos, 0);
				if (m != nullptr && (m->pos - pos).length() < 32.0)
				{
					if (m->n > r)
					{
						ownMaterials[0] += r;
						m->n -= r;
					}
					else
					{
						ownMaterials[0] += m->n;
						m->n = 0;
						m->erase();
					}
				}
				else break;
			}
			else break;
		}

		//マテリアル変換
		if (ownMaterials[0] >= 0.01)
		{
			ownMaterials[1] += 0.01;
			ownMaterials[0] -= 0.01;
		}
		else	//マテリアル不足の場合、死亡
		{
			die();
			return;
		}

		if (age > 300)
		{
			age = 0;
			state = State::Imago;
		}
		break;
	case  State::Imago:

		//マテリアル回収
		for (;;)
		{
			const double r = 0.1;
			if (ownMaterials[0] <= r)
			{
				auto* m = nearestMaterial(pos, 0);
				if (m != nullptr && (m->pos - pos).length() < 32.0)
				{
					if (m->n > r)
					{
						ownMaterials[0] += r;
						m->n -= r;
					}
					else
					{
						ownMaterials[0] += m->n;
						m->n = 0;
						m->erase();
					}
				}
				else break;
			}
			else break;
		}

		//マテリアル変換
		if (ownMaterials[0] >= 0.01)
		{
			ownMaterials[1] += 0.01;
			ownMaterials[0] -= 0.01;
		}
		else	//マテリアル不足の場合、死亡
		{
			die();
			return;
		}

		if (age > 150)
		{
			//子孫を残す
			for (int i = 0; i < 2; i++)
			{
				auto* nu = makeChild();
				if (nu != nullptr) nu->v = RandomVec2(3);
			}

			die();
			return;
		}
		break;
	default:
		break;
	}
	if (!v.isZero())
	{
		pos += v;
		v *= 0.90;
		if (pos.x < 0) pos.x = 0;
		if (pos.y < 0) pos.y = 0;
		if (pos.x > 1024) pos.x = 1024;
		if (pos.y > 1024) pos.y = 1024;
	}
}
void	Unit::updateSlug()
{
	switch (state)
	{
	case State::Egg:
		if (age > 600)
		{
			age = 0;
			state = State::Larva;
			health = 1.0;
		}
		break;
	case State::Larva:

		angle.rotate(sin(age / 5.0)*0.01);
		{
			auto* m = nearestMaterial(pos, 1);
			if (m == nullptr || (m->pos - pos).length() >= 64.0 || ownMaterials[1] > 6.0)
			{
				v += angle*0.02;
				angle.rotate(sin(age / 60.0)*0.05);
			}
			else if ((m->pos - pos).length() < 4.0)
			{
				const double r = 1.0;

				if (m->n > r)
				{
					ownMaterials[1] += r;
					m->n -= r;
				}
				else
				{
					ownMaterials[1] += m->n;
					m->n = 0;
					m->erase();
				}
			}
			else
			{
				v += angle*0.02;
				if ((m->pos - pos).cross(angle) > 0) angle.rotate(-0.05);
				else angle.rotate(0.05);
			}
		}
		//マテリアル変換
		if (ownMaterials[1] >= 0.1)
		{
			ownMaterials[0] += 0.1;
			ownMaterials[1] -= 0.1;

			if (ownMaterials[0] > 1.0)
			{
				setMaterial(0, 1.0, pos, 0.0);
				ownMaterials[0] -= 1.0;
			}
			health += 0.1;
			if (health > 1.0) health = 1.0;
		}
		else	//マテリアル不足の場合、死亡
		{
			health -= 0.002;
			if (health <= 0.0)
			{
				die();
				return;
			}
		}

		if (age > 1000)
		{
			//子孫を残す
			for (int i = 0; i < 3; i++)
			{
				auto* nu = makeChild();
				if (nu != nullptr)
				{
					nu->v = RandomVec2(3);
					nu->age = Random(600);
				}
			}

			die();
			return;
		}
		break;
	}
	if (!v.isZero())
	{
		pos += v;
		v *= 0.90;
		if (pos.x < 0) pos.x = 0;
		if (pos.y < 0) pos.y = 0;
		if (pos.x > 1024) pos.x = 1024;
		if (pos.y > 1024) pos.y = 1024;
	}
}