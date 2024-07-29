#pragma once
#include "../MyLib/Physics/Collidable.h"

class Enemy;

class EnemySphere : public MyEngine::Collidable
{
public:
	EnemySphere(Priority priority, ObjectTag tag);
	virtual ~EnemySphere();

	void Init();
	void Update();
	void Draw();

private:
	int m_color;
	bool m_deleteFlag;

	float m_radius = 0;

	Vec3 m_velocity;
	std::shared_ptr<Enemy>m_enemy;
};

