#pragma once

class Assets;
struct Field;

/*
Objectクラスは物体運動を表現するためのクラス

*/

//基礎オブジェクト
struct Object {

	bool	isSigned;	//Tableに登録されたか
	bool	eraseFlag;	//次のフレームで消去する
	double	y;		//高度
	double	vy;		//上下方向の速度
	Vec2	v;		//平面方向の速度
	Vec2	pos;	//座標
	Vec2	angle;	//向いている方向 len = 1.0

	static	Field*	field;
	static	Assets*	assets;

	Object()
		: isSigned(false)
		, eraseFlag(false)
		, y(0.0)
		, vy(0.0)
		, v(0.0, 0.0)
		, pos(0.0, 0.0)
		, angle(RandomVec2())
	{}

	Vec2	drawPos() const;
	double	angleAsRadian() const;	//直径
};

//生物オブジェクト
struct Creature : Object {

	enum struct State {
		Child,
		Adult,
	};

	enum struct Type {
		Clematis,
		Slug,
		Criket,
	};

	int		age;
	int		health;
	Type	type;
	State	state;

	Creature()
		: age(0)
		, health(0)
		, type(Type::Clematis)
	{}

	double	size() const;
	int		maxHealth() const;
	void	update();
	void	draw() const;
};

//マテリアルオブジェクト
struct Material : Object {

	enum struct Type {
		Leaf,
		Meat,
		Fertilizer,
	};

	int		age;
	Type	type;

	Material()
		: age(0)
		, type(Type::Leaf)
	{}
};
