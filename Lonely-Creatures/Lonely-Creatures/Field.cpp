#include"Field.h"
#include"Assets.h"

typedef Creature::Type CType;
typedef Material::Type MType;

Field*	Object::field;
Assets*	Object::assets;

void	Material::erase() {
	enabled = false;
	registered = false;
	auto* c = field->table.chip(pos);
	if (c != nullptr) c->remove(this);
}
void	Creature::erase() {
	enabled = false;
	registered = false;
	auto* c = field->table.chip(pos);
	if (c != nullptr) c->remove(this);
}
Material::Material() {
	pos = RandomVec2(field->region);;
	type = Type::Leaf;
	y = 0;
	age = 0;
}
Material::Material(const Vec2& _pos, const Type& _type) {
	pos = _pos;
	type = _type;
	y = 0;
	age = 0;
}
Creature::Creature() {
	angle = RandomVec2();
	pos = RandomVec2(field->region);
	type = Type::Clematis;
	health = maxHealth();
	y = 0;
	vy = 0;
	v = Vec2::Zero();
}
Creature::Creature(const Vec2& _pos, const Type& _type) {
	angle = RandomVec2();
	pos = _pos;
	type = _type;
	health = maxHealth();
	y = 0;
	vy = 0;
	v = Vec2::Zero();
}
int		Creature::maxHealth() const {
	switch (type)
	{
	case Type::Clematis:return 5;
	case Type::Slug:	return 12;
	case Type::Cricket:	return 20;
	default: return 100;
	}
}
double	Creature::size() const {
	switch (type)
	{
	case Type::Clematis:
		if (state == State::Adult) return 16.0;
		else return 8.0;
	case Type::Slug:
		if (state == State::Adult) return 18.0;
		else return 8.0;
	case Type::Cricket:	return 32.0;
	default: return 16.0;
	}
}
Field::Field(Assets* _assets)
	: table(64.0, Size(64, 64))
{

	region = RectF(0, 0, 2048, 2048);
	assets = _assets;
	maxNumCreatures = 65536;
	maxNumMaterials = 65536;

	creatures.reserve(maxNumCreatures);
	materials.reserve(maxNumMaterials);

	Object::field = this;
	Object::assets = _assets;

	for (int i = 0; i < 1024; i++)  creatures.emplace_back(RandomVec2(region), CType::Clematis);
	for (int i = 0; i < 128; i++)  creatures.emplace_back(RandomVec2(region), CType::Slug);
	for (int i = 0; i < 16; i++)  creatures.emplace_back(RandomVec2(region), CType::Cricket);
}
void	Field::update() {

	for (auto& c : creatures) {
		if (!c.enabled) continue;

		switch (c.type) {
		case CType::Clematis:
			switch (c.state)
			{
			case Creature::State::Child:
				if (RandomBool(0.001)) c.state = Creature::State::Adult;
				break;
			case Creature::State::Adult:
				if (RandomBool(0.001))
				{
					auto* cc = newCreature();
					cc->pos = c.pos;
					cc->type = CType::Clematis;
					cc->v = RandomVec2(2.0);
					cc->vy = 2.0;
				}
				break;
			}
			break;
		case CType::Slug:
		{
			auto func1 = [](Vec2 pos, Material* ct) {
				if (ct->type != Material::Type::Leaf) return 0.0;
				return 32.0 - (ct->pos - pos).length();
			};
			auto func2 = [](Vec2 pos, Creature* ct) {
				if (ct->type != Creature::Type::Clematis || ct->state != Creature::State::Adult) return 0.0;
				return 32.0 - (ct->pos - pos).length();
			};
			auto* mt = table.searchMaterial(c.pos, 32.0, func1);
			if (mt != nullptr) {
				c.angle = (mt->pos - c.pos).normalized();
				if ((mt->pos - c.pos).length() < c.size() / 2.0 + 4.0) {

					mt->erase();

					if (RandomBool(0.25))
					{
						if (c.state != Creature::State::Adult) c.state = Creature::State::Adult;
						else {
							auto* cc = newCreature();
							cc->pos = c.pos;
							cc->type = CType::Slug;
						}
					}
				}
			}
			else
			{
				auto* ct = table.searchCreature(c.pos, 32.0, func2);
				if (ct != nullptr) {
					c.angle = (ct->pos - c.pos).normalized();
					if ((ct->pos - c.pos).length() < (c.size() + ct->size()) / 2.0) {
						//葉っぱのドロップ
						int n = Random(1, 3);
						for (int i = 0; i < n; i++) {
							auto* m = newMaterial();
							m->pos = ct->pos;
							m->type = Material::Type::Leaf;
							m->vy = 2.0;
							m->v = RandomVec2(1.0);
						}
						ct->erase();
					}
				}
				else {
					if (RandomBool(0.01)) c.angle = RandomVec2();
				}
			}
			c.v += c.angle*0.02;
		}

		break;
		case CType::Cricket:
		{
			auto func1 = [](Vec2 pos, Material* ct) {
				if (ct->type != Material::Type::Meat) return 0.0;
				return 128.0 - (ct->pos - pos).length();
			};
			auto func2 = [](Vec2 pos, Creature* ct) {
				if (ct->type != Creature::Type::Slug || ct->state != Creature::State::Adult) return 0.0;
				return 128.0 - (ct->pos - pos).length();
			};

			auto* mt = table.searchMaterial(c.pos, 128.0, func1);
			if (mt != nullptr) {
				c.angle = (mt->pos - c.pos).normalized();
				if ((mt->pos - c.pos).length() < c.size() / 2.0) mt->erase();
				c.v += c.angle*0.05;
			}
			else {

				auto* ct = table.searchCreature(c.pos, 128.0, func2);
				if (ct != nullptr) {
					c.angle = (ct->pos - c.pos).normalized();
					if ((ct->pos - c.pos).length() < (c.size() + ct->size()) / 2.0 + 4.0 && c.y == 0) {
						c.vy = 3;
						c.v += c.angle*0.8;
					}
					if ((ct->pos - c.pos).length() < (c.size() + ct->size()) / 2.0 && c.y > 0) {
						ct->v += c.angle*2.0;
						ct->vy += 2.0;
						ct->health -= Random(4, 6);
						if (ct->health < 0)
						{
							//肉のドロップ
							int n = Random(3, 4);
							for (int i = 0; i < n; i++)
							{
								auto* m = newMaterial();
								m->pos = ct->pos;
								m->type = Material::Type::Meat;
								m->vy = 1.0;
								m->v = RandomVec2(0.5);
							}
							ct->erase();
						}
					}
					c.v += c.angle*0.05;
				}
				else {
					if (RandomBool(0.01) && c.y == 0) c.vy = 3;
					c.v += c.angle*0.02;
					if (RandomBool(0.01)) c.angle = RandomVec2();
				}
			}
		}
		break;
		}

		++c.age;

		if (RandomBool(0.01) && c.health < c.maxHealth()) c.health += 1;

		c.vy -= 0.2;
		c.y += c.vy;
		if (c.y <= 0) {
			c.y = 0;
			c.vy = 0;
		}

		c.v /= 1.05;

		if ((region.br() - c.pos).x < c.v.x) c.v.x = (region.br() - c.pos).x;
		if ((region.br() - c.pos).y < c.v.y) c.v.y = (region.br() - c.pos).y;
		if ((region.pos - c.pos).x > c.v.x)  c.v.x = (region.pos - c.pos).x;
		if ((region.pos - c.pos).y > c.v.y)  c.v.y = (region.pos - c.pos).y;

		if (!c.registered || table.chip(c.pos) != table.chip(c.pos += c.v)) {
			table.chip(c.pos)->remove(&c);
			table.chip(c.pos + c.v)->set(&c);
		}
		c.pos += c.v;

		if (c.age > 2400) c.erase();
	}

	for (auto& m : materials) {
		if (!m.enabled) continue;

		++m.age;

		m.vy -= 0.2;
		m.y += m.vy;
		if (m.y <= 0) {
			m.y = 0;
			m.vy = 0;
		}
		m.v /= 1.05;

		if ((region.br() - m.pos).x < m.v.x) m.v.x = (region.br() - m.pos).x;
		if ((region.br() - m.pos).y < m.v.y) m.v.y = (region.br() - m.pos).y;
		if ((region.pos - m.pos).x > m.v.x)  m.v.x = (region.pos - m.pos).x;
		if ((region.pos - m.pos).y > m.v.y)  m.v.y = (region.pos - m.pos).y;

		if (!m.registered || table.chip(m.pos) != table.chip(m.pos += m.v)) {
			table.chip(m.pos)->remove(&m);
			table.chip(m.pos + m.v)->set(&m);
		}

		m.pos += m.v;

		if (m.age > 600) {
			if (m.type == Material::Type::Fertilizer) m.erase();
			else if (RandomBool(0.5)) m.erase();
			else {
				m.age = 0;
				m.type = Material::Type::Fertilizer;
			}
		}
	}

	
	while (!materials.isEmpty() && !materials.back().enabled) materials.pop_back();
	while (!creatures.isEmpty() && !creatures.back().enabled) creatures.pop_back();
}

void	Field::draw() const {

	region.draw(Palette::Darkkhaki);
	region.drawFrame(8, Palette::Red);

	//shadow
	for (auto& c : creatures) {
		if (!c.enabled) continue;
		Circle(c.pos, c.size() / 6.0).draw(Color(0, 128));
	}
	for (auto& m : materials) {
		if (!m.enabled) continue;
		Circle(m.pos, 1.5).draw(Color(0, 128));
	}
	/*
	for (auto& c : creatures) {
		auto func = [](Vec2 pos, Creature* ct) {
			if (pos == ct->pos) return 0.0;
			return 32.0 - (ct->pos - pos).length();
		};
		Circle(c.pos, 32.0).draw(Color(128, 64));

		auto* ct = table.searchCreature(c.pos, 32.0, func);
		if (ct != nullptr) {
			Line(c.pos, ct->pos).drawArrow();
		}
	}
	*/

	for (auto& c : creatures) {
		if (!c.enabled) continue;
		if (c.type == Creature::Type::Cricket) {
			auto func = [](Vec2 pos, Creature* ct) {
				if (ct->type != Creature::Type::Slug || ct->state != Creature::State::Adult) return 0.0;
				return 128.0 - (ct->pos - pos).length();
			};
			Circle(c.pos, 128.0).draw(Color(Palette::Red, 64));

			auto* ct = table.searchCreature(c.pos, 128.0, func);
			if (ct != nullptr) {
				Line(c.pos, ct->pos).drawArrow(1.0, Vec2(5.0, 5.0), Palette::Red);
			}
		}
	}


	for (auto& c : creatures) {
		if (!c.enabled) continue;
		double angleAsRadian = atan2(c.angle.y, c.angle.x);
		Vec2 p = c.pos.movedBy(0, -c.y);

		switch (c.type)
		{
		case CType::Clematis:
			assets->texture(L"clematisLeaf.png").resize(c.size(), c.size()).drawAt(p);
			if (c.state == Creature::State::Adult) assets->texture(L"clematisFlower.png").resize(c.size(), c.size()).drawAt(p);
			break;
		case CType::Slug:
			if (c.state == Creature::State::Adult) {
				if (c.vy > 0.0) assets->texture(L"slugAdultDamaged.png").resize(c.size(), c.size()).rotate(angleAsRadian).drawAt(p);
				else assets->texture(L"slugAdult.png").resize(c.size(), c.size()).rotate(angleAsRadian).drawAt(p);
			}
			else assets->texture(L"slugChild.png").resize(c.size(), c.size()).rotate(angleAsRadian).drawAt(p);
			break;
		case CType::Cricket:
			assets->texture(L"cricket.png").resize(c.size(), c.size()).rotate(angleAsRadian).drawAt(p);
			break;
		default:
			break;
		}
	}

	//healthゲージ
	for (auto& c : creatures) {
		if (!c.enabled) continue;
		double rate = double(c.health) / double(c.maxHealth());
		auto p = c.pos.movedBy(-c.size() / 2.0, -c.size() / 2.0);
		Line(p, p.movedBy(c.size(), 0.0)).draw(2, Palette::Red);
		Line(p, p.movedBy(c.size()*rate, 0.0)).draw(2, Palette::Green);
	}

	for (auto& m : materials) {
		if (!m.enabled) continue;
		Vec2 p = m.pos.movedBy(0, -m.y - 3.0 + 0.8*sin(m.age / 20.0));

		switch (m.type)
		{
		case Material::Type::Meat:
			assets->texture(L"meat.png").resize(8.0, 8.0).drawAt(p);
			break;
		case Material::Type::Leaf:
			assets->texture(L"leaf.png").resize(8.0, 8.0).drawAt(p);
			break;
		case Material::Type::Fertilizer:
			assets->texture(L"fertilizer.png").resize(8.0, 8.0).drawAt(p);
			break;
		default:
			break;
		}
	}

}