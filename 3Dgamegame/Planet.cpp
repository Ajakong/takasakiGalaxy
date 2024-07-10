#include "Planet.h"
#include"MyLib/Physics/ColliderBase.h"
#include"MyLib/Physics/ColliderSphere.h"

namespace
{
	constexpr float kGroundRadius = 100;
	constexpr float  kGravityRange= 150;

}

Planet::Planet():Collidable(Priority::Static, ObjectTag::Stage)
, modelH(-1)
{
	m_rigid.SetGravity(30);
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);//ここで入れたのは重力の影響範囲
	m_colliders.back()->isTrigger = true;
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = kGravityRange;
	AddThroughTag(ObjectTag::Stage);
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);//マップの当たり判定
	auto item2 = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item2->radius = kGroundRadius;
}

Planet::Planet(int modelHandle):Collidable(Priority::Static,ObjectTag::Stage)
	,modelH(modelHandle)
{
	m_rigid.SetGravity(30);
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);//ここで入れたのは重力の影響範囲
	m_colliders.back()->isTrigger = true;
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = kGravityRange;

	AddCollider(MyEngine::ColliderBase::Kind::Sphere);//マップの当たり判定
	auto item2 = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item2->radius = kGroundRadius;
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
	DrawSphere3D(m_rigid.GetPos().VGet(), kGravityRange, 10, 0x0000ff, 0x0000ff, false);
	DrawSphere3D(m_rigid.GetPos().VGet(), kGroundRadius, 50, 0xff00ff, 0xff0000, true);
}
