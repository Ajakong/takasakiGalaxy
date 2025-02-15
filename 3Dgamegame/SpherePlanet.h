#pragma once
#include "Planet.h"
class SpherePlanet : public Planet
{
public:
	SpherePlanet(Vec3 pos,int color,float m_gravityPower, int modelHandle,int radius = 500);
	virtual ~SpherePlanet();

	virtual void Init();
	virtual void Update();
	virtual void Draw();

	virtual Vec3 GravityEffect(std::shared_ptr<Collidable> obj);
	virtual Vec3 GetNormVec(Vec3 pos);
	virtual void OnTriggerEnter(std::shared_ptr<Collidable> colider);
	virtual void OnTriggerExit(std::shared_ptr<Collidable> colider);
	
protected:
	int m_enemyCount;
	int m_color;
	int m_modelHandle;
	int m_radius;
};

