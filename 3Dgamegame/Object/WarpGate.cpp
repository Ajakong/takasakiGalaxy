#include "WarpGate.h"
#include"../MyLib/Physics/ColliderSphere.h"

WarpGate::WarpGate(Vec3 pos):Collidable(Priority::Static, ObjectTag::WarpGate)
{
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);//‚±‚±‚Å“ü‚ê‚½‚Ì‚Íd—Í‚Ì‰e‹¿”ÍˆÍ
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = 60;
	m_rigid.SetPos(pos);
}

WarpGate::~WarpGate()
{
}

void WarpGate::Init()
{
}

void WarpGate::Update()
{
}

void WarpGate::Draw()
{
	DrawSphere3D(m_rigid.GetPos().VGet(), 60, 7, 0xff00ff, 0xff00ff, false);
	DrawCube3D(Vec3(m_rigid.GetPos() + Vec3(60, 60, 60)).VGet(), Vec3(m_rigid.GetPos() + Vec3(-60, -60, -60)).VGet(), 0xffff00, 0xffff00, false);
}

void WarpGate::OnCollideEnter(std::shared_ptr<Collidable> colider)
{
	if (colider->GetTag() == ObjectTag::Player)
	{
		colider->GetRigidbody().SetPos(m_warpPos);
	}
}
