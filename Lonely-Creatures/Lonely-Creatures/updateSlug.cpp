#include"Field.h"

void	Field::updateSlug(Creature* _c) {

	//—t‚Á‚Ï‚Ì’Tõ
	auto func1 = [](Vec2 pos, Material* mt) {
		if (mt->type != MType::Leaf) return 0.0;
		return 32.0 - (mt->pos - pos).length();
	};

	switch (_c->state)
	{
	case CState::Egg:
		if (_c->age > 300) _c->state = CState::Adult;
		break;
	case CState::Child:
		if (_c->age > 420) _c->state = CState::Adult;
		break;
	case CState::Adult:
	{
		auto* mt = table.searchMaterial(_c->pos, 32.0, func1);
		if (mt != nullptr) {
			_c->angle = (mt->pos - _c->pos).normalized();
			if ((mt->pos - _c->pos).length() < _c->size() / 2.0 + 4.0) {
				mt->type = MType::Fertilizer;
			}
		}
		else if (RandomBool(0.01)) _c->angle = RandomVec2();
		_c->v += _c->angle*0.02;
		break;
	}
	}
}