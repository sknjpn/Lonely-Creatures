#include"Field.h"
#include"Assets.h"

typedef Creature::Type CType;
typedef Material::Type MType;
typedef Creature::State CState;

Field*	Object::field;
Assets*	Object::assets;
int		Creature::numEnabled = 0;
int		Material::numEnabled = 0;

void	Material::erase() {
	numEnabled--;
	enabled = false;
	if (registeredChip != nullptr) registeredChip->remove(this);
}
void	Creature::erase() {
	numEnabled--;
	enabled = false;
	if (registeredChip != nullptr) registeredChip->remove(this);
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
		switch (state)
		{
		case CState::Seed:	return 4.0;
		case CState::Child:	return 8.0;
		case CState::Adult:	return 16.0;
		}
	case Type::Slug:
		switch (state)
		{
		case CState::Egg:	return 6.0;
		case CState::Child:	return 8.0;
		case CState::Adult:	return 12.0;
		}
	case Type::Cricket:
		switch (state)
		{
		case CState::Egg:	return 6.0;
		case CState::Child:	return 32.0;
		case CState::Adult:	return 48.0;
		}
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

	for (int i = 0; i < 1024; i++) {
		auto* c = newCreature();
		c->pos = RandomVec2(region);
		c->type = CType::Clematis;
		c->state = CState::Seed;
	}
	for (int i = 0; i < 128; i++) {
		auto* c = newCreature();
		c->pos = RandomVec2(region);
		c->type = CType::Slug;
		c->state = CState::Egg;
	}
	for (int i = 0; i < 16; i++) {
		auto* c = newCreature();
		c->pos = RandomVec2(region);
		c->type = CType::Cricket;
		c->state = CState::Egg;
	}
}
void	Field::update() {

	for (auto& c : creatures) {
		if (!c.enabled) continue;

		switch (c.type) {
		case CType::Clematis:
			switch (c.state)
			{
			case CState::Seed:
				if (c.age > 300) c.state = CState::Child;
				break;
			case CState::Child:
				if (c.age > 600) c.state = CState::Adult;
				break;
			case CState::Adult:
				if (c.age > 1200)
				{
					int n = Random(2, 3);
					for (int i = 0; i < n; i++)
					{
						auto* cc = newCreature();
						cc->pos = c.pos;
						cc->type = CType::Clematis;
						cc->state = CState::Seed;
						cc->v = RandomVec2(2.0);
						cc->vy = 2.0;
					}
					c.enabled = false;
				}
				break;
			}
			break;
		case CType::Slug:
		{
			switch (c.state)
			{
			case CState::Egg:
				if (c.age > 300) c.state = CState::Child;
				break;
			case CState::Child:
				if (c.age > 420) c.state = CState::Adult;
				break;
			case CState::Adult:
			{
				if (c.age > 1200) {
					int n = Random(2, 3);
					for (int i = 0; i < n; i++)
					{
						auto* cc = newCreature();
						cc->pos = c.pos;
						cc->type = CType::Slug;
						cc->state = CState::Egg;
					}
					c.enabled = false;
				}
				//行動
				auto func1 = [](Vec2 pos, Material* ct) {
					if (ct->type != MType::Leaf) return 0.0;
					return 32.0 - (ct->pos - pos).length();
				};
				auto func2 = [](Vec2 pos, Creature* ct) {
					if (ct->type != CType::Clematis || ct->state != CState::Adult) return 0.0;
					return 32.0 - (ct->pos - pos).length();
				};
				auto* mt = table.searchMaterial(c.pos, 32.0, func1);
				if (mt != nullptr) {
					c.angle = (mt->pos - c.pos).normalized();
					if ((mt->pos - c.pos).length() < c.size() / 2.0 + 4.0) {
						mt->erase();
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
								m->type = MType::Leaf;
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
				break;
			}
			}
		}
		break;
		case CType::Cricket:
		{
			switch (c.state)
			{
			case CState::Egg:
				c.state = CState::Child;
				break;
			case CState::Child:
				break;
			case CState::Adult:
				break;

			default:
				break;
			}
			auto func1 = [](Vec2 pos, Material* ct) {
				if (ct->type != MType::Meat) return 0.0;
				return 128.0 - (ct->pos - pos).length();
			};
			auto func2 = [](Vec2 pos, Creature* ct) {
				if (ct->type != CType::Slug || ct->state != CState::Adult) return 0.0;
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
								m->type = MType::Meat;
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

		//ヘルス回復
		if (RandomBool(0.01) && c.health < c.maxHealth()) c.health += 1;

		//カウンタ
		++c.age;

		//運動処理
		c.vy -= 0.2;
		c.y += c.vy;
		if (c.y <= 0) { c.y = 0; c.vy = 0; }
		c.v /= 1.05;
		if ((region.br() - c.pos).x < c.v.x) c.v.x = (region.br() - c.pos).x;
		if ((region.br() - c.pos).y < c.v.y) c.v.y = (region.br() - c.pos).y;
		if ((region.pos - c.pos).x > c.v.x)  c.v.x = (region.pos - c.pos).x;
		if ((region.pos - c.pos).y > c.v.y)  c.v.y = (region.pos - c.pos).y;
		if (c.registeredChip == nullptr || c.registeredChip != table.chip(c.pos += c.v)) {
			if (c.registeredChip != nullptr) c.registeredChip->remove(&c);
			table.chip(c.pos + c.v)->set(&c);
		}
		c.pos += c.v;

		if (c.age > 2400) c.erase();
	}

	for (auto& m : materials) {
		if (!m.enabled) continue;


		if (m.age > 600) {
			if (m.type == MType::Fertilizer) m.erase();
			else if (RandomBool(0.5)) m.erase();
			else {
				m.age = 0;
				m.type = MType::Fertilizer;
			}
		}

		//カウンタ
		++m.age;

		//運動処理
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
		if (m.registeredChip == nullptr || m.registeredChip != table.chip(m.pos += m.v)) {
			if (m.registeredChip != nullptr) m.registeredChip->remove(&m);
			table.chip(m.pos + m.v)->set(&m);
		}
		m.pos += m.v;

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
		if (c.type == CType::Cricket) {
			auto func = [](Vec2 pos, Creature* ct) {
				if (ct->type != CType::Slug || ct->state != CState::Adult) return 0.0;
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
			switch (c.state)
			{
			case CState::Seed:
				assets->texture(L"clematisSeed.png").resize(c.size(), c.size()).drawAt(p);
				break;
			case CState::Child:
				assets->texture(L"clematisLeaf.png").resize(c.size(), c.size()).drawAt(p);
				break;
			case CState::Adult:
				assets->texture(L"clematisLeaf.png").resize(c.size(), c.size()).drawAt(p);
				assets->texture(L"clematisFlower.png").resize(c.size(), c.size()).drawAt(p);
				break;
			}
			break;
		case CType::Slug:
			switch (c.state)
			{
			case CState::Egg:
				assets->texture(L"slugEgg.png").resize(c.size(), c.size()).drawAt(p);
				break;
			case CState::Child:
				assets->texture(L"slugChild.png").resize(c.size(), c.size()).rotate(angleAsRadian).drawAt(p);
				break;
			case CState::Adult:
				assets->texture(L"slugAdult.png").resize(c.size(), c.size()).rotate(angleAsRadian).drawAt(p);
				break;
			}
			break;
		case CType::Cricket:
			switch (c.state)
			{
			case CState::Egg:
				assets->texture(L"cricketEgg.png").resize(c.size(), c.size()).drawAt(p);
				break;
			case CState::Child:
				assets->texture(L"cricketChild.png").resize(c.size(), c.size()).rotate(angleAsRadian).drawAt(p);
				break;
			case CState::Adult:
			{
				auto t = assets->texture(L"cricketAdult.png")(48 * (c.age % 4), 0, 48, 48);
				t.resize(c.size(), c.size()).rotate(angleAsRadian).drawAt(p);
			}
			break;
			}
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
		case MType::Meat:
			assets->texture(L"meat.png").resize(8.0, 8.0).drawAt(p);
			break;
		case MType::Leaf:
			assets->texture(L"leaf.png").resize(8.0, 8.0).drawAt(p);
			break;
		case MType::Fertilizer:
			assets->texture(L"fertilizer.png").resize(8.0, 8.0).drawAt(p);
			break;
		default:
			break;
		}
	}

}