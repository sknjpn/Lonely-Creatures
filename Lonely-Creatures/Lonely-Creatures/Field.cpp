#include"Field.h"
#include"Assets.h"

typedef Creature::Type CType;
typedef Material::Type MType;

Material::Material(const Vec2& _pos, const Type& _type) {

	pos = _pos;
	type = _type;
	y = 0;
	age = 0;
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


Field::Field(Assets* _assets)
	: table(64.0, Size(64, 64))
{

	region = RectF(0, 0, 2048, 2048);
	assets = _assets;
	maxNumCreatures = 1024;
	maxNumMaterials = 1024;

	creatures.reserve(maxNumCreatures);
	materials.reserve(maxNumMaterials);

	for (int i = 0; i < 1024; i++)  creatures.emplace_back(RandomVec2(region), CType::Clematis);
	for (int i = 0; i < 128; i++)  creatures.emplace_back(RandomVec2(region), CType::Slug);
	for (int i = 0; i < 16; i++)  creatures.emplace_back(RandomVec2(region), CType::Cricket);
}

void	Field::update() {

	for (auto& c : creatures) {

		switch (c.type) {
		case CType::Clematis:
			if (RandomBool(0.001)) c.state = Creature::State::Adult;
			break;
		case CType::Slug:
		{
			//獲物
			{
				auto func = [](Vec2 pos, Creature* ct) {
					if (ct->type != Creature::Type::Clematis || ct->state != Creature::State::Adult) return 0.0;
					return 32.0 - (ct->pos - pos).length();
				};

				auto* ct = table.searchCreature(c.pos, 128.0, func);
				if (ct != nullptr)
				{
					c.angle = (ct->pos - c.pos).normalized();
					if ((ct->pos - c.pos).length() < (c.size() + ct->size()) / 2.0)
					{
						//葉っぱのドロップ
						int n = Random(1, 3);
						for (int i = 0; i < n; i++)
						{
							auto& m = materials.emplace_back(ct->pos, Material::Type::Leaf);
							m.vy = 2.0;
							m.v = RandomVec2(1.0);
						}
						ct->state = Creature::State::Child;
						if (RandomBool(0.25)) c.state = Creature::State::Adult;
					}
				}
				else
				{
					if (RandomBool(0.01)) c.angle = RandomVec2();
				}
				c.v += c.angle*0.02;
			}
		}

		break;
		case CType::Cricket:
		{
			//獲物
			{
				auto func = [](Vec2 pos, Creature* ct) {
					if (ct->type != Creature::Type::Slug || ct->state != Creature::State::Adult) return 0.0;
					return 128.0 - (ct->pos - pos).length();
				};

				auto* ct = table.searchCreature(c.pos, 128.0, func);
				if (ct != nullptr)
				{
					c.angle = (ct->pos - c.pos).normalized();
					if ((ct->pos - c.pos).length() < (c.size() + ct->size()) / 2.0 + 4.0 && c.y == 0)
					{
						c.vy = 3;
						c.v += c.angle*0.8;
					}
					if ((ct->pos - c.pos).length() < (c.size() + ct->size()) / 2.0 && c.y > 0)
					{
						ct->v += c.angle*2.0;
						ct->vy += 2.0;
						ct->health -= Random(4, 6);
						if (ct->health < 0)
						{
							//肉のドロップ
							int n = Random(3, 4);
							for (int i = 0; i < n; i++)
							{
								auto& m = materials.emplace_back(ct->pos, Material::Type::Meat);
								m.vy = 1.0;
								m.v = RandomVec2(0.5);
							}
							ct->health = ct->maxHealth();
							ct->state = Creature::State::Child;
						}
					}
					c.v += c.angle*0.05;
				}
				else
				{
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

		if (!c.isSigned || table.chip(c.pos) != table.chip(c.pos += c.v)) {
			table.chip(c.pos)->remove(&c);
			table.chip(c.pos + c.v)->set(&c);
		}
		c.pos += c.v;
	}

	for (auto& m : materials) {

		++m.age;

		m.vy -= 0.2;
		m.y += m.vy;
		if (m.y <= 0) {
			m.y = 0;
			m.vy = 0;
		}
		m.v /= 1.05;
		m.pos += m.v;

		if (m.age > 600) {
			if (m.type == Material::Type::Fertilizer) m.eraseFlag = true;
			else if (RandomBool(0.5)) m.eraseFlag = true;
			else {
				m.age = 0;
				m.type = Material::Type::Fertilizer;
			}
		}
	}

	materials.remove_if([](Material& m) { return m.eraseFlag; });

}

void	Field::draw() const {

	region.draw(Palette::Darkkhaki);
	region.drawFrame(8, Palette::Red);

	//shadow
	for (auto& c : creatures) Circle(c.pos, c.size() / 6.0).draw(Color(0, 128));
	for (auto& m : materials) Circle(m.pos, 1.5).draw(Color(0, 128));


	for (auto& c : creatures) {
		auto func = [](Vec2 pos, Creature* ct) {
			if (pos == ct->pos) return 0.0;
			return 32.0 - (ct->pos - pos).length();
		};
		Circle(c.pos, 32.0).draw(Color(128, 64));

		auto* ct = table.searchCreature(c.pos, 32.0, func);
		if (ct != nullptr)
		{
			Line(c.pos, ct->pos).drawArrow();
		}
	}

	for (auto& c : creatures) {
		if (c.type == Creature::Type::Cricket)
		{
			auto func = [](Vec2 pos, Creature* ct) {
				if (ct->type != Creature::Type::Slug || ct->state != Creature::State::Adult) return 0.0;
				return 128.0 - (ct->pos - pos).length();
			};
			Circle(c.pos, 128.0).draw(Color(Palette::Red, 64));

			auto* ct = table.searchCreature(c.pos, 128.0, func);
			if (ct != nullptr)
			{
				Line(c.pos, ct->pos).drawArrow(1.0, Vec2(5.0, 5.0), Palette::Red);
			}
		}
	}


	for (auto& c : creatures) {

		double angleAsRadian = atan2(c.angle.y, c.angle.x);
		Vec2 p = c.pos.movedBy(0, -c.y);

		switch (c.type)
		{
		case CType::Clematis:
			assets->texture(L"clematisLeaf.png").resize(c.size(), c.size()).drawAt(p);
			if (c.state == Creature::State::Adult) assets->texture(L"clematisFlower.png").resize(c.size(), c.size()).drawAt(p);
			break;
		case CType::Slug:
			if (c.state == Creature::State::Adult)
			{
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
		double rate = double(c.health) / double(c.maxHealth());
		auto p = c.pos.movedBy(-c.size() / 2.0, -c.size() / 2.0);
		Line(p, p.movedBy(c.size(), 0.0)).draw(2, Palette::Red);
		Line(p, p.movedBy(c.size()*rate, 0.0)).draw(2, Palette::Green);
	}

	for (auto& m : materials) {

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