#include"Area.h"
#include"Unit.h"
#include"Material.h"
#include"Object.h"
#include"TinyCamera.h"

TinyCamera tinyCamera;
Array<Unit>		units;
Array<Object>	objects;
Array<Material>	materials;
Grid<Area>		areas;
double	areaWidth = 64.0;
Vec2	areaSize = { 1024, 1024 };
int	numMaterialTypes = 2;