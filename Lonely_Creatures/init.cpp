#include"Unit.h"
#include"Area.h"
#include"Object.h"
#include"TinyCamera.h"
#include"Material.h"


void	initAreas()
{
	areas.resize(int(areaSize.x / areaWidth + 1), int(areaSize.y / areaWidth + 1));
}
void	initObjects()
{
	objects.resize(4096);
}
void	initUnits()
{
	units.resize(4096);

	for (int i = 0; i < 1024; i++)
	{
		auto& u = units[i];
		u.enabled = true;
		u.angle = RandomVec2();
		u.age = i % 60;
		u.pos = RandomVec2(RectF(areaSize));
		u.unitType = 0;
	}
	for (int i = 0; i < 128; i++)
	{
		auto& u = units[i];
		u.enabled = true;
		u.angle = RandomVec2();
		u.age = i % 60;
		u.pos = RandomVec2(RectF(areaSize));
		u.unitType = 1;
	}
}
void	initMaterials()
{
	materials.resize(4096);

	for (int i = 0; i < 512; i++)
	{
		auto& m = materials[i];
		m.materialType = 0;
		m.enabled = true;
		m.joinedArea = nullptr;
		m.n = 8.0;
		m.pos = RandomVec2(RectF(areaSize));
		m.joinArea();
	}
}
void	initTinyCamera()
{
	tinyCamera.drawingRegion = Window::ClientRect();
	tinyCamera.smoothDrawingRegion = tinyCamera.drawingRegion;
}