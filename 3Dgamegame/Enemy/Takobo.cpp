#include "Takobo.h"
#include"../MyLib/Physics/ColliderSphere.h"

namespace
{
	constexpr float kCollisionRadius = 50.f;
}

Takobo::Takobo():Enemy(MV1LoadModel("../Model/Enemy/bodyeater.mv1"),Priority::Low,ObjectTag::Character),
m_position(0,0,0)
{
	m_rigid.SetPos(m_position);
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = kCollisionRadius;
}

Takobo::~Takobo()
{
}

void Takobo::Init()
{
}

void Takobo::Update()
{
	m_rigid.SetVelocity(VGet(1,0,0));

	
}

void Takobo::SetMatrix()
{
	MATRIX moving = MGetTranslate(m_rigid.GetPos().VGet());

	MV1SetMatrix(m_handle, moving);
}

void Takobo::Draw()
{

	DrawSphere3D(m_rigid.GetPos().VGet(), kCollisionRadius, 10, 0xff0000, 0xff0000, false);
	MV1DrawModel(m_handle);
}
