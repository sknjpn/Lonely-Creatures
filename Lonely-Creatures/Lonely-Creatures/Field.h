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

		Type	type;
		double	y;
		double	vy;
		Vec2	v;
		Vec2	pos;
		Vec2	angle;
		double	mass;

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
			case Type::Crematis:return 250;
			case Type::Slug:	return 500;
			case Type::Criket:	return 2000;
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

	RectF	region;
	Assets*	assets;
	size_t	maxNumCreatures;
	size_t	maxNumMaterials;
	Array<Creature>	creatures;
	Array<Material>	materials;

	Field(Assets* _assets);

	void	update();
	void	draw() const;
};