#pragma once
#include "Enemy.h"
class Takobo : public Enemy
{
public:
	Takobo();
	virtual ~Takobo();

	void Init();
	void Update();
	void SetMatrix();
	void Draw();

private:
	Vec3 m_position;

};

