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
			{
				double size = pow(u.m1, 0.5);
				switch (u.state)
				{
				case State::Egg:
					Circle(u.pos, 1).draw(Palette::Black);

					break;
				case State::Larva:
				{
					auto* m = nearestMaterial(u.pos, 0);
					if (m != nullptr)
					{
						Line(m->pos, u.pos).draw(1, Color(Palette::Red, 64));
					}
					Circle(u.pos.movedBy(size, size), size * 1.5).draw(Palette::Green);
					Circle(u.pos.movedBy(-size, size), size * 1.5).draw(Palette::Green);
					Circle(u.pos.movedBy(size, -size), size * 1.5).draw(Palette::Green);
					Circle(u.pos.movedBy(-size, -size), size * 1.5).draw(Palette::Green);
				}
					break;
				default:
					break;
				}			}
			break;
			case 1:
				Circle(u.pos, 3).draw(Palette::Brown);
				break;
			default:
				break;
			}
		}
	}
}
void	drawObjects()
{

}