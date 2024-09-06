#include "WarpGate.h"
#include"../MyLib/Physics/ColliderSphere.h"
#include"../Player.h"
#include<EffekseerForDXLib.h>

WarpGate::WarpGate(Vec3 pos,int handle):Collidable(Priority::Static, ObjectTag::WarpGate),
m_emitterHandle(-1)
{
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);//‚±‚±‚Å“ü‚ê‚½‚Ì‚Íd—Í‚Ì‰e‹¿”ÍˆÍ
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = 60;
	m_rigid->SetPos(pos);

	m_emitterHandle =handle;
	PlayEffekseer3DEffect(m_emitterHandle);
}

WarpGate::~WarpGate()
{
	StopEffekseer3DEffect(m_emitterHandle);
	DeleteEffekseerEffect(m_emitterHandle);
}

void WarpGate::Init()
{
}

void WarpGate::Update()
{
	SetPosPlayingEffekseer3DEffect(m_emitterHandle, m_rigid->GetPos().x, m_rigid->GetPos().y, m_rigid->GetPos().z);
}

void WarpGate::Draw()
{
	DrawLine3D(m_rigid->GetPos().VGet(), ((m_warpPos-m_rigid->GetPos()).GetNormalized()*300+m_rigid->GetPos()).VGet(), 0xaaaaff);
	DrawSphere3D(m_rigid->GetPos().VGet(), 60, 7, 0xff00ff, 0xff00ff, false);
	DrawCube3D(Vec3(m_rigid->GetPos() + Vec3(60, 60, 60)).VGet(), Vec3(m_rigid->GetPos() + Vec3(-60, -60, -60)).VGet(), 0xffff00, 0xffff00, false);
}

void WarpGate::OnCollideEnter(std::shared_ptr<Collidable> colider)
{
	if (colider->GetTag() == ObjectTag::Player)
	{
		colider->GetRigidbody()->SetVelocity(Vec3(m_warpPos - colider->GetRigidbody()->GetPos()).GetNormalized() * 100);
		auto player = std::dynamic_pointer_cast<Player>(colider);
		player->m_playerUpdate = &Player::JumpingUpdate;
		player->IsWarp();
	}
}
