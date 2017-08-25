#pragma once

struct Area;
struct Unit;

struct Object
{
	bool	enabled;
	int		objectType;
	Vec2	pos;
	Area*	joinedArea;

	Object() : enabled(false) {}
	void	reset();
};

extern Array<Object>	objects;

void	initObjects();
void	updateObjects();
void	drawObjects();