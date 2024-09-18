#include "BossPlanet.h"
#include"MyLib/Physics/ColliderSphere.h"

namespace
{
	constexpr float kGroundRadius = 750;
	constexpr float  kGravityRange = 2000;
	constexpr float  kGravityPower = 30;

	const char* name = "bossplanet";
	const char* atom = "atomosphere";

}

BossPlanet::BossPlanet(Vec3 pos, int color):SpherePlanet(pos,color,3,-1),
m_color(color)
{
	gravityPower = 3;
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);//�����œ��ꂽ�̂͏d�͂̉e���͈�
	m_colliders.back()->isTrigger = true;
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = kGravityRange;
	AddThroughTag(ObjectTag::Stage);
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);//�}�b�v�̓����蔻��
	auto item2 = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item2->radius = kGroundRadius;
}

BossPlanet::~BossPlanet()
{
}

void BossPlanet::Init()
{
}

void BossPlanet::Update()
{
}

void BossPlanet::Draw()
{
	DrawSphere3D(m_rigid->GetPos().VGet(), kGroundRadius, 50, m_color, 0xffffff, !m_isSearch);
}