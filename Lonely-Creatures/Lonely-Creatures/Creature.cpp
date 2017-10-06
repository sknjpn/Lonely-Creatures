#include"Object.h"
#include"Assets.h"
#include"Field.h"

double	Creature::size() const {
	switch (type)
	{
	case Type::Clematis:return 16.0;
	case Type::Slug:	return 24.0;
	case Type::Criket:	return 32.0;
	default: return 16.0;
	}
}
int		Creature::maxHealth() const {
	switch (type)
	{
	case Type::Clematis:return 6;
	case Type::Slug:	return 8;
	case Type::Criket:	return 12;
	default: return 8;
	}
}
void	Creature::update() {

	switch (type) {
	case Type::Clematis:
		if (RandomBool(0.001)) state = Creature::State::Adult;
		break;
	case Type::Slug:
	{
		//Šl•¨
		{
			auto func = [](Vec2 pos, Creature* ct) {
				if (ct->type != Creature::Type::Clematis || ct->state != Creature::State::Adult) return 0.0;
				return 32.0 - (ct->pos - pos).length();
			};

			auto* ct = field->table.searchCreature(pos, 128.0, func);
			if (ct != nullptr)
			{
				angle = (ct->pos - pos).normalized();
				if ((ct->pos - pos).length() < (size() + ct->size()) / 2.0)
				{
					for (int i = 0; i < Random(1, 3); i++)
					{
						field->materials.emplace_back(ct->pos, Material::Type::Leaf);
						field->materials.back().vy = 1.0;
						field->materials.back().v = RandomVec2(1.0);
					}
					ct->state = Creature::State::Child;
					if (RandomBool(0.25)) state = Creature::State::Adult;
				}
			}
			else
			{
				if (RandomBool(0.01)) angle = RandomVec2();
			}
			v += angle*0.02;
		}
	}

	break;
	case Type::Criket:
	{
		//Šl•¨
		{
			auto func = [](Vec2 pos, Creature* ct) {
				if (ct->type != Creature::Type::Slug || ct->state != Creature::State::Adult) return 0.0;
				return 128.0 - (ct->pos - pos).length();
			};

			auto* ct = field->table.searchCreature(pos, 128.0, func);
			if (ct != nullptr)
			{
				angle = (ct->pos - pos).normalized();
				if ((ct->pos - pos).length() < (size() + ct->size()) / 2.0 + 4.0 && y == 0)
				{
					vy = 3;
					v += angle*0.8;
				}
				if ((ct->pos - pos).length() < (size() + ct->size()) / 2.0 && y > 0)
				{
					field->materials.emplace_back(ct->pos, Material::Type::Meat);
					field->materials.back().vy = 2.0;
					ct->v += angle*2.0;
					ct->vy += 2.0;
					ct->health -= Random(4, 6);
					if (ct->health < 0)
					{
						ct->health = ct->maxHealth();
						ct->state = Creature::State::Child;
					}
				}
				v += angle*0.05;
			}
			else
			{
				if (RandomBool(0.01) && y == 0) vy = 3;
				v += angle*0.02;
				if (RandomBool(0.01)) angle = RandomVec2();
			}
		}
	}
	break;
	}

	//ƒ‰ƒCƒt‰ñ•œ
	if (RandomBool(0.01) && health < maxHealth()) health += 1;

	vy -= 0.2;
	y += vy;
	if (y <= 0)
	{
		y = 0;
		vy = 0;
	}

	v /= 1.05;

	if ((field->region.br() - pos).x < v.x) v.x = (field->region.br() - pos).x;
	if ((field->region.br() - pos).y < v.y) v.y = (field->region.br() - pos).y;
	if ((field->region.pos - pos).x > v.x)  v.x = (field->region.pos - pos).x;
	if ((field->region.pos - pos).y > v.y)  v.y = (field->region.pos - pos).y;

	if (!isSigned || field->table.chip(pos) != field->table.chip(pos += v)) {
		field->table.chip(pos)->remove(this);
		field->table.chip(pos + v)->set(this);
	}
	pos += v;

	++age;
}
void	Creature::draw() const {

}