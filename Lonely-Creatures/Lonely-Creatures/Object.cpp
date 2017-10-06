#include"Object.h"
#include"Field.h"
#include"Assets.h"

Field*	Object::field;
Assets*	Object::assets;

Vec2	Object::drawPos() const { return pos.movedBy(0, -y); }
double	Object::angleAsRadian() const { return atan2(angle.y, angle.x); }