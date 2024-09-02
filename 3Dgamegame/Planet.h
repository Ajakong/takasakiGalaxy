#pragma once
#include"MyLib/Physics/Collidable.h"

class Planet : public MyEngine::Collidable
{
public:
	Planet();
	Planet(int modelHandle);
	virtual ~Planet();

	virtual void Init();
	virtual void Update();
	virtual void Draw();

	virtual Vec3 GravityEffect(std::shared_ptr<Collidable> obj)=0;//Physics�ɌĂяo�����,�Ԃ�l�ŕ␳��̃x�N�g����Ԃ�
	virtual Vec3 GetNormVec(Vec3 pos) = 0;
protected:
	int modelH;
	float gravityPower;
	bool clearFlag=false;
};

