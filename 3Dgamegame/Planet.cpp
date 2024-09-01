#include "Planet.h"
#include"MyLib/Physics/ColliderBase.h"


Planet::Planet():Collidable(Priority::Static, ObjectTag::Stage)
, modelH(-1)
{
	
}

Planet::Planet(int modelHandle):Collidable(Priority::Static,ObjectTag::Stage)
	,modelH(modelHandle)
{
	
}

Planet::~Planet()
{
}

void Planet::Init()
{
}

void Planet::Update()
{
}

void Planet::Draw()
{
	
}