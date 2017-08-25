#pragma once

struct Area;

struct Material
{
	bool	enabled;
	int		materialType;
	double	n;
	Vec2	pos;
	Vec2	v;
	Area*	joinedArea;

	Material() {reset(); }
	void	joinArea();
	Point	getAreaPos() const;
	void	erase();
	void	reset();
};

extern Array<Material>	materials;

void	initMaterials();
void	drawMaterials();
void	updateMaterials();
Material*	setMaterial(int _type, double _n, const Vec2& _pos, const double _f);
Material*	newMaterial();
Material*	nearestMaterial(const Vec2& _pos, int _materialType);