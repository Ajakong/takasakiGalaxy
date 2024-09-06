#pragma once
#include "../MyLib/Physics/Collidable.h"
class WarpGate : public MyEngine::Collidable
{
public:
	WarpGate(Vec3 pos,int handle);
	~WarpGate();

	void SetWarpPos(Vec3 warpPos) { m_warpPos=warpPos; }

	void Init();
	void Update();
	void Draw();
	
	virtual void OnCollideEnter(std::shared_ptr<Collidable> colider);
private:
	Vec3 m_warpPos;
	int m_emitterHandle;
};

