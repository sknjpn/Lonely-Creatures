#pragma once

class Assets;
struct Creature;

class Display
{
public:
	Assets* assets;
	Creature*	selectedCreature;

	Display(Assets* _assets);
};