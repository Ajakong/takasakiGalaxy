#pragma once
#include"MyLib/Physics/Collidable.h"

class Planet : public MyEngine::Collidable
{
public:
	Planet();
	virtual ~Planet();

	virtual void Init();
	virtual void Update();
	virtual void Draw();
protected:
	int modelH;
};

