#include"Unit.h"
#include"Object.h"
#include"Material.h"
#include"TinyCamera.h"
#include"Area.h"

Array<Texture>	textures;

void Main()
{
	Graphics::SetBackground(Palette::Chocolate);

	Window::Resize(1280, 720);
	Graphics::SetFullScreen(true, Graphics::EnumOutputs().front().displayModes.back().size);

	initTinyCamera();
	initUnits();
	initAreas();
	initMaterials();

	textures.emplace_back(L"Image/ClematisFlower.png");
	textures.emplace_back(L"Image/ClematisLerf.png");
	textures.emplace_back(L"Image/Slug.png");

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
