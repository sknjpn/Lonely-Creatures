#pragma once

class Assets;
struct Field;
struct Chip;
struct Material;

enum struct MType {
	Leaf,
	Meat,
	Iron,
	Fertilizer,
};
enum struct CType {
	Clematis,
	Slug,
	Cricket,
};
enum struct CState {
	Egg,
	Seed,
	Child,
	Adult,
};

//基礎オブジェクト
struct Object {


	int		age;
	Chip*	registeredChip;
	bool	enabled;
	double	y;
	double	vy;
	Vec2	v;
	Vec2	pos;
	Vec2	angle;

	static	Field*	field;
	static	Assets*	assets;

	Object()
		: age(0)
		, registeredChip(nullptr)
		, enabled(true)
		, y(0.0)
		, vy(0.0)
		, v(Vec2::Zero())
		, pos(Vec2::Zero())
		, angle(RandomVec2())
	{}
	Vec2	drawPos() const {
		return pos.movedBy(0, -y);
	}
};

//生物オブジェクト
struct Creature : Object {

	int		timer;
	int		health;
	CType	type;
	CState	state;

	static int	numEnabled;

	Creature()
		: timer(0)
	{
		Creature::numEnabled++; 
	}

	int		maxHealth() const;
	double	size() const;
	void	erase();
	void	addMaterial(MType _type, double _force = 0.0, int _num = 1);
};

//マテリアルオブジェクト
struct Material : Object {

	MType	type;

	static int	numEnabled;

	Material() { Material::numEnabled++; }
	double	size() const { return 8.0; }
	void	erase();
};

struct Chip {

	Array<Creature*> creatures;
	Array<Material*> materials;

	void	remove(Creature* _creature);
	void	remove(Material* _material);
	void	set(Creature* _creature);
	void	set(Material* _material);
};

struct Table {

	Grid<Chip> chips;
	double	width;
	Size	size;

	Table(double _width, const Size& _size);

	//最も評価値の高いものを返す、0.0以下のものしかない場合、nullptrを返す
	Creature*	searchCreature(Vec2 _pos, double _range, double(*func)(Vec2, Creature*)) const;
	Material*	searchMaterial(Vec2 _pos, double _range, double(*func)(Vec2, Material*)) const;
	Chip*	chip(const Vec2& _pos);
};

//フィールド
struct Field {

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
	Creature*	newCreature(CType _type, CState _state, const Vec2& _pos);
	Creature*	newCreature(CType _type, CState _state);
	Material*	newMaterial(MType _type, const Vec2& _pos);
	Material*	newMaterial(MType _type);
};