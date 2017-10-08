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
	virtual Vec2 drawPos() const {
		return pos.movedBy(0, -y);
	}
	virtual	String name() const {
		return L"Object";
	}
};

//生物オブジェクト
struct Creature : Object {

	int		mCount;	//マテリアル系
	int		timer;
	int		health;
	CType	type;
	CState	state;

	static int	numEnabled;

	Creature()
		: timer(0)
		, mCount(0)
	{
		Creature::numEnabled++; 
	}

	int		maxHealth() const;
	double	size() const;
	void	erase();
	void	addMaterial(MType _type, double _force = 0.0, int _num = 1);
	String	name() const {
		switch (type)
		{
		case CType::Clematis:	return L"Clematis";
		case CType::Slug:		return L"Slug";
		case CType::Cricket:	return L"Cricket";
		default:	return L"Hoge";
		}
	}
};

//マテリアルオブジェクト
struct Material : Object {

	MType	type;

	static int	numEnabled;

	Material() { Material::numEnabled++; }
	double	size() const { return 4.0; }
	void	erase();
	String	name() const {
		switch (type)
		{
		case MType::Fertilizer:	return L"Fertilizer";
		case MType::Iron:		return L"Iron";
		case MType::Leaf:		return L"Leaf";
		case MType::Meat:		return L"Meat";
		default:	return L"Hoge";
		}
	}
	Vec2 drawPos() const {
		return pos.movedBy(0, -y - 0.8 + 0.4*sin(age / 20.0));
	}
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

	bool	drawHealthBar;
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

	void	updateClematis(Creature* _c);
	void	updateSlug(Creature* _c);
};