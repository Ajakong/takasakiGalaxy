#pragma once
#include "../MyLib/Physics/Collidable.h"
class Item : public MyEngine::Collidable
{
public:
	Item(Vec3 pos, bool antiGravity=false);
	~Item();

	void Init();
	void Update();
	void Draw();

	bool GetDeleteFlag() { return m_deleteFlag; }
	Vec3 GetUpVec() { return m_upVec; }
	virtual void OnCollideEnter(std::shared_ptr<Collidable> colider);
	virtual void OnTriggerEnter(std::shared_ptr<Collidable> colider);
private:
	float angle=0;
	bool m_deleteFlag = false;
	Vec3 m_nowPlanetPos;
	Vec3 m_upVec;
};

