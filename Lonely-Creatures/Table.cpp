#include"Field.h"

Table::Table(double _width, const Size& _size)
	: width(_width)
	, size(_size)
{
	chips.resize(_size);
}
Chip*	Table::chip(const Vec2& _pos) {
	Point p(int(_pos.x / width), int(_pos.y / width));
	if (p.x < 0 || p.y < 0 || p.x >= size.x || p.y >= size.y) return nullptr;
	return &chips.at(p);
}
Creature*	Table::searchCreature(Vec2 _pos, double _range, double(*func)(Vec2, Creature*)) const {

	Point	minP(int((_pos.x - _range) / width), int((_pos.y - _range) / width));
	Point	maxP(int((_pos.x + _range) / width), int((_pos.y + _range) / width));
	double	max = 0.0;
	double	rangeSq = _range*_range;
	Creature*	target = nullptr;

	for (int x = minP.x - 1; x <= maxP.x + 1; x++)
	{
		if (x < 0 || x >= size.x) continue;
		for (int y = minP.y - 1; y <= maxP.y + 1; y++)
		{
			if (y < 0 || y >= size.y) continue;
			const auto& c = chips.at(Point(x, y));
			for (auto* cr : c.creatures)
			{
				if ((cr->pos - _pos).lengthSq() <= rangeSq)
				{
					double value = func(_pos, cr);
					if (value > 0 && (value > max || target == nullptr))
					{
						target = cr;
						max = value;
					}
				}
			}
		}
	}

	return target;
}
Material*	Table::searchMaterial(Vec2 _pos, double _range, double(*func)(Vec2, Material*)) const {

	Point	minP(int((_pos.x - _range) / width), int((_pos.y - _range) / width));
	Point	maxP(int((_pos.x + _range) / width), int((_pos.y + _range) / width));
	double	max = 0.0;
	double	rangeSq = _range*_range;
	Material*	target = nullptr;

	for (int x = minP.x - 1; x <= maxP.x + 1; x++)
	{
		if (x < 0 || x >= size.x) continue;
		for (int y = minP.y - 1; y <= maxP.y + 1; y++)
		{
			if (y < 0 || y >= size.y) continue;
			const auto& c = chips.at(Point(x, y));
			for (auto* cr : c.materials)
			{
				if ((cr->pos - _pos).lengthSq() <= rangeSq)
				{
					double value = func(_pos, cr);
					if (value > 0 && (value > max || target == nullptr))
					{
						target = cr;
						max = value;
					}
				}
			}
		}
	}

	return target;
}
void	Chip::remove(Creature* _creature) {
	creatures.remove(_creature);
	_creature->registeredChip = nullptr;
}
void	Chip::remove(Material* _material) {
	materials.remove(_material);
	_material->registeredChip = nullptr;
}
void	Chip::set(Creature* _creature) {
	creatures.emplace_back(_creature);
	_creature->registeredChip = this;
}
void	Chip::set(Material* _material) {
	materials.emplace_back(_material);
	_material->registeredChip = this;
}