#include"Field.h"
#include"Assets.h"

void	Creature::addMaterial(MType _type, double _force, int _num) {
	for (int i = 0; i < _num; i++) {
		auto* m = field->newMaterial();
		m->pos = pos;
		m->type = _type;
		m->vy = _force;
		m->v = RandomVec2(_force);
	}
}