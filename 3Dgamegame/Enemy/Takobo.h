#pragma once
#include "Enemy.h"
/// <summary>
/// ���E�Ɉړ����A���Ԋu�Ŏˌ����Ă���G
/// ���݂�����ƂԂ��
/// </summary>
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
	Vec3 m_moveShaftPos;

	Vec3 m_vec;
};

