#include"Field.h"
#include"Assets.h"

Field*	Object::field;
Assets*	Object::assets;
int		Creature::numEnabled = 0;
int		Material::numEnabled = 0;

void	Creature::addMaterial(MType _type, double _force, int _num) {
	for (int i = 0; i < _num; i++) {
		auto* m = field->newMaterial(_type, pos);
		m->vy = _force;
		m->v = RandomVec2(_force);
	}
}
void	Material::erase() {
	numEnabled--;
	enabled = false;
	if (registeredChip != nullptr) registeredChip->remove(this);
}
void	Creature::erase() {
	numEnabled--;
	enabled = false;
	if (registeredChip != nullptr) registeredChip->remove(this);
}