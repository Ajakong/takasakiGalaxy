#pragma once
#include "../MyLib/Physics/Collidable.h"
class Item : public MyEngine::Collidable
{
public:
	Item(Vec3 pos);
	~Item();

	void Init();
	void Update();
	void Draw();

	bool GetDeleteFlag() { return m_deleteFlag; }

	virtual void OnCollideEnter(std::shared_ptr<Collidable> colider);

private:
	float angle=0;
	bool m_deleteFlag = false;
	int m_emitterHandle;
};

