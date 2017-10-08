#include"Field.h"

void	Field::updateClematis(Creature* _c) {

	//‰h—{‚Ì‹zŽû
	auto func = [](Vec2 pos, Material* mt) {
		if (mt->type != MType::Fertilizer) return 0.0;
		return 64.0 - (mt->pos - pos).length();
	};

	switch (_c->state)
	{
	case CState::Seed:
		if (_c->timer > 360) {
			_c->addMaterial(MType::Fertilizer, 0.5, _c->mCount);
			_c->erase();
			return;
		}
		//‰h—{‚ð‹z‚Á‚ÄŽŸ‚ÌƒXƒe[ƒW‚Ö
		if (_c->timer > 120 && _c->timer % 5 == 0)
		{
			auto* mt = table.searchMaterial(_c->pos, 64.0, func);
			if (mt != nullptr) {
				mt->v += (_c->pos - mt->pos).normalized()*0.1;
				if ((_c->pos - mt->pos).length() < (_c->size() + mt->size()) / 2.0)
				{
					mt->erase();
					_c->mCount++;
					if (_c->mCount >= 1) {
						_c->timer = 0;
						_c->state = CState::Child;
					}
				}
			}
		}
		break;
	case CState::Child:
		if (_c->timer > 360) {
			_c->addMaterial(MType::Leaf, 0.5, _c->mCount);
			_c->erase();
			return;
		}
		//‰h—{‚ð‹z‚Á‚ÄŽŸ‚ÌƒXƒe[ƒW‚Ö
		if (_c->timer > 120 && _c->timer % 5 == 0)
		{
			auto* mt = table.searchMaterial(_c->pos, 64.0, func);
			if (mt != nullptr) {
				mt->v += (_c->pos - mt->pos).normalized()*0.1;
				if ((_c->pos - mt->pos).length() < (_c->size() + mt->size()) / 2.0)
				{
					mt->erase();
					_c->mCount++;
					if (_c->mCount >= 3) {
						_c->timer = 0;
						_c->state = CState::Adult;
					}
				}
			}
		}
		break;
	case CState::Adult:
		if (_c->timer > 240) {
			int n = Random(2, 4);
			for (int i = 0; i < n; i++) {
				auto* cc = newCreature(CType::Clematis, CState::Seed, _c->pos);
				cc->v = RandomVec2(1.0);
				cc->vy = 2.0;
			}
			_c->addMaterial(MType::Leaf, 0.5, _c->mCount);
			_c->erase();
			return;
		}
		break;
	}
}