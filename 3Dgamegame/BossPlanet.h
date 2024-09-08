#pragma once
#include "SpherePlanet.h"
class BossPlanet : public SpherePlanet
{
public:
	BossPlanet(Vec3 pos, int color);
	virtual ~BossPlanet();

	virtual void Init();
	virtual void Update();
	virtual void Draw();

private:
	int m_color;
};

