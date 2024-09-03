#pragma once
#include "../MyLib/Physics/Collidable.h"
class Item : public MyEngine::Collidable
{
public:
	Item(Vec3 pos);
	~Item();
};

