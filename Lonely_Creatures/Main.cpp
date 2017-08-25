#include"Unit.h"
#include"Object.h"
#include"Material.h"
#include"TinyCamera.h"
#include"Area.h"

void Main()
{
	Graphics::SetBackground(Palette::Skyblue);

	Window::Resize(1280, 720);

	initTinyCamera();
	initUnits();
	initAreas();
	initMaterials();

	while (System::Update())
	{
		tinyCamera.update();
		{
			auto t = tinyCamera.createTransformer();
			updateMaterials();
			updateObjects();
			updateUnits();

			drawAreas();
			drawMaterials();
			drawObjects();
			drawUnits();
		}
	}
}
