#include"Field.h"

void	Field::updateSlug(Creature* _c) {

	//—t‚Á‚Ï‚Ì’Tõ
	auto func1 = [](Vec2 pos, Material* mt) {
		if (mt->type != MType::Leaf) return 0.0;
		return 64.0 - (mt->pos - pos).length();
	};

	switch (_c->state)
	{
	case CState::Egg:
		if (_c->age > 300 && RandomBool(0.001)) {
			_c->state = CState::Adult;
			_c->timer = 0;
		}
		break;
	case CState::Child:
		if (_c->age > 420) _c->state = CState::Adult;
		break;
	case CState::Adult:
	{
		auto* mt = table.searchMaterial(_c->pos, 64.0, func1);
		if (mt != nullptr) {
			_c->angle = (mt->pos - _c->pos).normalized();

			if ((mt->pos - _c->pos).length() < _c->size() / 2.0 + 4.0) {
				if (RandomBool(0.75) || int(_c->timer / 450.0) < _c->mCount) mt->type = MType::Fertilizer;
				else {
					mt->erase();
					_c->mCount++;
				}
			}
		}
		else if (RandomBool(0.01)) _c->angle = RandomVec2();
		if (int(_c->timer / 450.0) > _c->mCount) {
			_c->addMaterial(MType::Meat, 0.25, _c->mCount);
			_c->erase();
			return;
		}
		else if (_c->timer >= 1800) {
			_c->addMaterial(MType::Meat, 0.25, _c->mCount);
			int n = Random(1, 2);
			for (int i = 0; i < n; i++) {
				auto* cc = newCreature(CType::Slug, CState::Egg, _c->pos);
				cc->v = RandomVec2(0.5);
			}
			_c->erase();
			return;
		}
		_c->v += _c->angle*0.02;
		break;
	}
	}
}