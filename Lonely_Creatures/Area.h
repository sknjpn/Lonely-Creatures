#pragma once

struct Unit;
struct Object;
struct Material;

struct Area
{
	Array<Unit*>		units;
	Array<Object*>		objects;
	Array<Material*>	materials;
	Array<Area*>		nearAreas;

	Point	pos() const;
};

extern Grid<Area>	areas;
extern double	areaWidth;
extern Vec2		areaSize;

void	drawAreas();
void	initAreas();