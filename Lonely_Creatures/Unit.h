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
	Area*	joinedArea;
	int		age;
	double	m0;
	double	m1;
	State	state;


	Unit() { reset(); }
	void	updateCreamatis();
	void	updateSlug();
	void	reset();
	int		id() const;
	void	joinArea();
	Point	getAreaPos() const;
	void	erase();
};

extern Array<Unit> units;

void	updateUnits();
void	drawUnits();
void	initUnits();
Unit*	newUnit();