#pragma once

struct Area;
struct Object;
struct Material;

enum struct State
{
	Egg,
	Larva,
	Imago,
};

struct Unit
{
	bool	enabled;
	int		unitType;
	Vec2	pos;
	Vec2	angle;
	Vec2	v;
	Area*	joinedArea;
	int		age;
	Array<double>	ownMaterials;
	State	state;
	double	health;


	Unit() { reset(); }
	void	updateCreamatis();
	void	updateSlug();
	void	reset();
	int		id() const;
	void	joinArea();
	void	erase();
	void	die();
	Unit*	makeChild();
	Unit*	newUnit();
};

extern Array<Unit> units;
extern Array<Texture>	textures;


void	updateUnits();
void	drawUnits();
void	initUnits();