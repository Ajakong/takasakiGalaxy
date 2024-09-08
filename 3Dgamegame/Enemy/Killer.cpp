#include "Killer.h"
#include"Enemy.h"
namespace
{
	/// <summary>
	/// ‹…‚Ì“–‚½‚è”»’è”¼Œa
	/// </summary>
	constexpr float kSphereRadius = 10.0f;
	/// <summary>
	/// ‹…‚Ì¶¬ŠÔŠu
	/// </summary>
	constexpr int kSphereCreateFrame = 50;
	const char* name = "Killer";
}
Killer::Killer(MyEngine::Collidable::Priority priority, ObjectTag tag, std::shared_ptr<MyEngine::Collidable>enemy, std::shared_ptr<MyEngine::Collidable>target, Vec3 pos, Vec3 velocity, int moveNum, int color):EnemySphere(priority,tag,enemy,pos,velocity,moveNum,color)
{
	m_counterFlag = false;
	m_target = target;
	m_moveUpdate = &Killer::SearchUpDate;
}

Killer::~Killer()
{
}

void Killer::Init()
{
}

void Killer::Update()
{
	if (m_counterFlag)
	{
		m_moveUpdate = &Killer::CounterUpdate;
	}
	(this->*m_moveUpdate)();
}

void Killer::Draw()
{
	DrawSphere3D(m_rigid->GetPos().VGet(), kSphereRadius, 10, 0x440044, m_color, true);
}

void Killer::Hit()
{
}

void Killer::OnCollideEnter(std::shared_ptr<Collidable> colider)
{

}

void Killer::SearchUpDate()
{
	Vec3 toVec = m_target->GetRigidbody()->GetPos() - m_rigid->GetPos();
	m_velocity = toVec.GetNormalized();

	m_rigid->SetVelocity(m_velocity * 5);
	if (toVec.Length() <= 150)
	{
		m_moveUpdate = &Killer::StraightUpdate;
	}
}

void Killer::StraightUpdate()
{
	m_rigid->SetVelocity(m_velocity * 10);
}

void Killer::CounterUpdate()
{
	Vec3 toVec = m_enemy->GetRigidbody()->GetPos() - m_rigid->GetPos();
	m_velocity = toVec.GetNormalized();

	m_rigid->SetVelocity(m_velocity * 20);
}
