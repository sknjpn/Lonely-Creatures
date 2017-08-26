#include"Area.h"
#include"Material.h"
#include"Unit.h"

void	drawAreas()
{
	for (double x = 0; x < areaSize.x; x += areaWidth)
	{
		Line(x, 0, x, areaSize.y).draw(2, Palette::Gray);
	}
	for (double y = 0; y < areaSize.y; y += areaWidth)
	{
		Line(0, y, areaSize.x, y).draw(2, Palette::Gray);
	}

	RectF(areaSize).drawFrame(2, Palette::Red);
}
void	drawMaterials()
{
	for (auto& m : materials)
	{
		if (m.enabled)
		{
			switch (m.materialType)
			{
			case 0:
				Circle(m.pos, pow(m.n, 0.5) * 2).draw(ColorF(Palette::Rosybrown, 0.5));
				break;
			case 1:
				Circle(m.pos, pow(m.n, 0.5) * 2).draw(ColorF(Palette::Darkgreen, 0.5));
				break;
			default:
				break;
			}
		}
	}
}
void	drawUnits()
{
	for (auto& u : units)
	{
		if (u.enabled)
		{
			switch (u.unitType)
			{
			case 0:
				switch (u.state)
				{
				case State::Egg:
					Circle(u.pos, 1).draw(Palette::Black);
					break;

				case State::Larva:
				{
					const double sizeLerf = EaseOut(Easing::Back, 0.0, 10.0, u.age / 300.0);
					textures.at(0).resize(sizeLerf).drawAt(u.pos);
				}
				break;

				case State::Imago:
				{
					const double sizeFlower = EaseOut(Easing::Back, 0.0, 4.0, u.age / 150.0);

					textures.at(0).resize(10.0).drawAt(u.pos);
					textures.at(1).resize(sizeFlower).drawAt(u.pos.movedBy(0, -1));
				}
				break;

				}
				break;

			case 1:
				switch (u.state)
				{
				case State::Egg:
					Circle(u.pos, 1).draw(Palette::Black);
					break;

				case State::Larva:
					textures.at(2).resize(4.0).rotate(atan2(u.angle.y, u.angle.x)).drawAt(u.pos);
					break;
				}
				break;

			}
		}
	}
}
void	drawObjects()
{

}