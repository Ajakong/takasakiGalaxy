#include "EnemySphere.h"
#include "Enemy.h"
#include "../MyLib/Physics/ColliderSphere.h"
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
	const char* name = "Sphere";
}
EnemySphere::EnemySphere(MyEngine::Collidable::Priority priority, ObjectTag tag, std::shared_ptr<MyEngine::Collidable>enemy, Vec3 pos, Vec3 velocity, int moveNum, int color) : Collidable(priority,tag),
m_enemy(std::dynamic_pointer_cast<Enemy>(enemy)),
m_radius(kSphereRadius),
m_velocity(velocity)
{
	m_rigid->SetPos(pos);
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = m_radius;
	m_color = color;
	//moveNum‚É‚æ‚Á‚Ä‹““®‚ª•Ï‚í‚é‚Ì‚©‚à‚µ‚ê‚È‚¢(À‘•‚·‚é‚©‚í‚©‚ç‚ñ)
	//if (moveNum == 0)
	{
		m_moveUpdate = &EnemySphere::StraightUpdate;
	}
}

EnemySphere::~EnemySphere()
{
}

void EnemySphere::Init()
{
}

void EnemySphere::Update()
{
	(this->*m_moveUpdate)();
	DeleteJudge();
}

void EnemySphere::Draw()
{
	DrawSphere3D(m_rigid->GetPos().VGet(), kSphereRadius, 10, 0xffff00, m_color, false);
}

void EnemySphere::Hit()
{
	//m_isDeleteFlag = true;

}

void EnemySphere::StraightUpdate()
{
	m_rigid->SetVelocity(VGet(m_velocity.x * 20, m_velocity.y * 20, m_velocity.z * 20));
}

void EnemySphere::DeleteJudge()
{
}