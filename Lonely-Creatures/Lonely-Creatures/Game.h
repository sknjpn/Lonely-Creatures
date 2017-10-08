#pragma once

#include"Assets.h"
#include"Display.h"
#include"Field.h"
#include"Camera.h"

class Game
{
public:
	Assets	assets;
	Field	field;
	Camera	camera;
	Display	display;
	int		playSpeed;
	bool	drawObjectName;

	Game();

	void	update();
};