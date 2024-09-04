#pragma once
#include "../MyLib/Physics/Collidable.h"
class WarpGate : public MyEngine::Collidable
{
public:
	WarpGate();
	~WarpGate();

	void Init();
	void Update();
	void Draw();
private:

};

