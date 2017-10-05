#include"Field.h"
#include"Assets.h"

typedef Field::Creature::Type CType;
typedef Field::Material::Type MType;

Field::Material::Material(const Vec2& _pos, const Type& _type) {

	pos = _pos;
	type = _type;
	y = 0;
}

Field::Creature::Creature(const Vec2& _pos, const Type& _type) {

	angle = RandomVec2();
	pos = _pos;
	type = _type;
	health = maxHealth() / 4;
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

	for (int i = 0; i < 1024; i++)  creatures.emplace_back(RandomVec2(region), CType::Crematis);
	for (int i = 0; i < 128; i++)  creatures.emplace_back(RandomVec2(region), CType::Slug);
	for (int i = 0; i < 16; i++)  creatures.emplace_back(RandomVec2(region), CType::Criket);
}

void	Field::update() {

	for (auto& c : creatures) {

		switch (c.type) {
		case CType::Crematis:
			break;
		case CType::Slug:
			c.v += c.angle*0.02;
			if (RandomBool(0.01)) c.angle = RandomVec2();
			break;
		case CType::Criket:
			if (RandomBool(0.01) && c.y == 0) c.vy = 3;
			c.v += c.angle*0.02;
			if (RandomBool(0.01)) c.angle = RandomVec2();
			break;
		default:
			break;
		}

		++c.age;

		if (RandomBool(0.1) && c.health < c.maxHealth()) c.health += 1;

		c.vy -= 0.2;
		c.y += c.vy;
		if (c.y <= 0)
		{
			c.y = 0;
			c.vy = 0;
		}

		c.v /= 1.05;

		if (region.br().x < (c.pos + c.v).x) c.v.x = (region.br() - c.pos).x;
		if (region.br().y < (c.pos + c.v).y)  c.v.y = (region.br() - c.pos).y;
		if (region.x > (c.pos + c.v).x)  c.v.x = (region.pos - c.pos).x;
		if (region.y > (c.pos + c.v).y)  c.v.y = (region.pos - c.pos).y;

		if (!c.isSigned || table.chip(c.pos) != table.chip(c.pos += c.v)) {
			table.chip(c.pos)->remove(&c);
			table.chip(c.pos + c.v)->set(&c);
		}
		c.pos += c.v;
	}

	for (auto& m : materials) {

	}

}

void	Field::draw() const {

	region.draw(Palette::Darkkhaki);
	region.drawFrame(8, Palette::Red);

	//shadow
	for (auto& c : creatures) {
		Circle(c.pos, c.size() / 6.0).draw(Color(00, 192));
	}

	for (auto& c : creatures) {
		auto func = [](Vec2 pos, Creature* ct) {
			if (pos == ct->pos) return 0.0;
			return 32.0 - (ct->pos - pos).length();
		};
		Circle(c.pos, 32.0).draw(Color(128, 128));

		auto* ct = table.searchCreature(c.pos, 32.0, func);
		if (ct != nullptr)
		{
			Line(c.pos, ct->pos).drawArrow();
		}
	}

	for (auto& c : creatures) {
		if (c.type == Creature::Type::Criket)
		{
			auto func = [](Vec2 pos, Creature* ct) {
				if (ct->type != Creature::Type::Slug) return 0.0;
				return 128.0 - (ct->pos - pos).length();
			};
			Circle(c.pos, 128.0).draw(Color(Palette::Red, 128));

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
		case CType::Crematis:
			assets->texture(L"crematis.png")(RectF(32.0, 32.0)).resize(c.size(), c.size()).drawAt(p);
			assets->texture(L"crematis.png")(RectF(32.0, 0.0, 32.0, 32.0)).resize(c.size(), c.size()).drawAt(p);
			break;
		case CType::Slug:
			assets->texture(L"slug.png").resize(c.size(), c.size()).rotate(angleAsRadian).drawAt(p);
			break;
		case CType::Criket:
			assets->texture(L"criket.png").resize(c.size(), c.size()).rotate(angleAsRadian).drawAt(p);
			break;
		default:
			break;
		}
	}

	//healthÉQÅ[ÉW
	for (auto& c : creatures) {
		double rate = double(c.health) / double(c.maxHealth());
		auto p = c.pos.movedBy(-c.size() / 2.0, -c.size() / 2.0);
		Line(p, p.movedBy(c.size(), 0.0)).draw(2, Palette::Red);
		Line(p, p.movedBy(c.size()*rate, 0.0)).draw(2, Palette::Green);
	}

	for (auto& m : materials) {

	}

}