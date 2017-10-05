#pragma once

class Assets;

//フィールド
class Field {
public:


	//基礎オブジェクト
	struct Object {

		enum struct Type {
			Creature,
			Material,
		};

		bool	isSigned;
		Type	type;
		double	y;
		double	vy;
		Vec2	v;
		Vec2	pos;
		Vec2	angle;
		double	mass;

		Object() : isSigned(false) {}
	};

	//生物オブジェクト
	struct Creature : Object {

		enum struct Type {
			Crematis,
			Slug,
			Criket,
		};

		Type	type;
		int		age;
		int		health;

		Creature(const Vec2& _pos, const Type& _type);

		int	maxHealth() const {
			switch (type)
			{
			case Type::Crematis:return 10;
			case Type::Slug:	return 20;
			case Type::Criket:	return 50;
			default: return 100;
			}
		}
		double	size() const {
			switch (type)
			{
			case Type::Crematis:return 16.0;
			case Type::Slug:	return 18.0;
			case Type::Criket:	return 32.0;
			default: return 16.0;
			}
		}
	};

	//マテリアルオブジェクト
	struct Material : Object {

		enum struct Type {
			Iron,
			Copper,
			Platinum,
		};

		Type	type;

		Material(const Vec2& _pos, const Type& _type);
	};

	struct Table {

		struct Chip {

			Array<Creature*> creatures;
			Array<Material*> materials;

			void	remove(Creature* _creature) {
				creatures.remove(_creature);
			}
			void	remove(Material* _material) {
				materials.remove(_material);
			}
			void	set(Creature* _creature) {
				creatures.emplace_back(_creature);
			}
			void	set(Material* _material) {
				materials.emplace_back(_material);
			}
		};

		Grid<Chip> chips;
		double	width;
		Size	size;

		Table(double _width, const Size& _size) : width(_width), size(_size) {
			chips.resize(_size);
		}

		Chip*	chip(const Vec2& _pos) {
			Point p(int(_pos.x / width), int(_pos.y / width));
			if (p.x < 0 || p.y < 0 || p.x >= size.x || p.y >= size.y) return nullptr;
			return &chips.at(p);
		}
		Chip*	chip(const Point& _p) {
			if (_p.x < 0 || _p.y < 0 || _p.x >= size.x || _p.y >= size.y) return nullptr;
			return &chips.at(_p);
		}

		//最も評価値の高いものを返す、0.0以下のものしかない場合、nullptrを返す
		Creature*	searchCreature(Vec2 _pos, double _range, double(*func)(Vec2, Creature*)) const {

			Point	minP(int((_pos.x - _range) / width), int((_pos.y - _range) / width));
			Point	maxP(int((_pos.x + _range) / width), int((_pos.y + _range) / width));
			double	max = 0.0;
			Creature*	target = nullptr;

			for (int x = minP.x - 1; x <= maxP.x + 1; x++)
			{
				if (x < 0 || x >= size.x) continue;
				for (int y = minP.y - 1; y <= maxP.y + 1; y++)
				{
					if (y < 0 || y >= size.y) continue;
					const auto& c = chips.at(Point(x, y));
					for (auto* cr : c.creatures)
					{
						double value = func(_pos, cr);
						if (value > 0 && (value > max || target == nullptr))
						{
							target = cr;
							max = value;
						}
					}
				}
			}

			return target;
		}

	};

	RectF	region;
	Table	table;
	Assets*	assets;
	size_t	maxNumCreatures;
	size_t	maxNumMaterials;
	Array<Creature>	creatures;
	Array<Material>	materials;


	Field(Assets* _assets);

	void	update();
	void	draw() const;
};