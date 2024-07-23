#pragma once
#include "Planet.h"
class SpherePlanet : public Planet
{
public:
	SpherePlanet();
	virtual ~SpherePlanet();

	virtual void Init();
	virtual void Update();
	virtual void Draw();

	virtual Vec3 GravityEffect(std::shared_ptr<Collidable> obj);
	virtual Vec3 GetNormVec(Vec3 pos);
};

