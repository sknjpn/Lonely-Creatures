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


Field::Field(Assets* _assets) {

	region = RectF(0, 0, 1024, 1024);
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
			c.pos += c.angle;
			if (RandomBool(0.01)) c.angle = RandomVec2();
			break;
		case CType::Criket:
			if(RandomBool(0.01) && c.y == 0) c.vy = 3;
			c.pos += c.angle;
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

		c.pos += c.v;
		if (region.br().x < c.pos.x) c.pos.x = region.br().x;
		if (region.br().y < c.pos.y) c.pos.y = region.br().y;
		if (region.x > c.pos.x) c.pos.x = region.x;
		if (region.y > c.pos.y) c.pos.y = region.y;

	}

	for (auto& m : materials) {

	}

}

void	Field::draw() const {

	region.draw(Palette::Darkkhaki);
	region.drawFrame(16, Palette::Red);

	//shadow
	for (auto& c : creatures) {
		Circle(c.pos, c.size() / 6.0).draw(Color(00, 192));
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