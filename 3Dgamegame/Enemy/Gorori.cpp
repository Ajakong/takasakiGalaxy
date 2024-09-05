#include "Gorori.h"
#include"../MyLib/Physics/ColliderSphere.h"
#include"../MyLib/Physics/Physics.h"

namespace
{
	constexpr float kCollisionRadius = 50.f;

	/// <summary>
		/// �ő�HP
		/// </summary>
	constexpr int kHp = 20;

	constexpr int kStartPosX = 200;
	constexpr int kStartPosY = 50;
	constexpr int kStartPosZ = 0;

	/// <summary>
	/// �������烂�f���̒��S�܂ł̋���
	/// </summary>
	constexpr int kFootToCenter = 30;

	/// <summary>
	/// �U���N�[���^�C�����̍Œ�ړ����x
	/// </summary>
	constexpr int kIdleSpeed = 20;
	/// <summary>
/// ���̐����Ԋu
/// </summary>
	constexpr int kSphereCreateFrame = 50;
	/// <summary>
	/// �čU���܂ł̃N�[���^�C��
	/// </summary>
	constexpr int kAttackCoolDownTime = 100;

	/// <summary>
	/// �X�e�[�W���f���̏c���T�C�Y/2
	/// </summary>
	constexpr int kStageSizeHalf = 200;



}

/*�v���g�^�C�v�錾*/
Vec3 ToVec(Vec3 a, Vec3 b);
Vec3 norm(Vec3 a);
float lerp(float start, float end, float t);

Gorori::Gorori(Vec3 pos) :Enemy(-1, Priority::Static, ObjectTag::Gorori),
m_Hp(kHp),
m_attackCoolDownCount(0),
m_centerToEnemyAngle(0)
{
	m_enemyUpdate = &Gorori::IdleUpdate;
	m_rigid.SetPos(pos);
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = kCollisionRadius;
	m_moveShaftPos = m_rigid.GetPos();
}

Gorori::~Gorori()
{

}

void Gorori::Init()
{
}

void Gorori::Update()
{
	(this->*m_enemyUpdate)();
}

void Gorori::SetMatrix()
{
	MATRIX moving = MGetTranslate(m_rigid.GetPos().VGet());

	MV1SetMatrix(m_handle, moving);
}

void Gorori::Draw()
{
	DrawSphere3D(m_rigid.GetPos().VGet(), kCollisionRadius, 10, 0xaaaa11, 0xff0000, false);
	MV1DrawModel(m_handle);
}

void Gorori::OnCollideEnter(std::shared_ptr<Collidable> colider)
{
	if (colider->GetTag() == ObjectTag::Player)
	{
		m_Hp -= 20;
	}
}

Vec3 Gorori::GetMyPos()
{
	return  VGet(m_rigid.GetPos().x, m_rigid.GetPos().y + kFootToCenter, m_rigid.GetPos().z);;
}

void Gorori::SetTarget(std::shared_ptr<Collidable> target)
{
	m_target = target;
}

void Gorori::IdleUpdate()
{
	m_vec.x = 1;
	if (abs(m_rigid.GetPos().x - m_moveShaftPos.x) > 5)
	{
		m_vec.x *= -1;
	}

	m_rigid.SetVelocity(VGet(m_vec.x, 0, 0));

	m_attackCoolDownCount++;

	if ((m_rigid.GetPos() - m_target->GetRigidbody().GetPos()).Length() > 100)
	{
		if (m_attackCoolDownCount > kAttackCoolDownTime)
		{
			int attackState = GetRand(1);
			switch (attackState)
			{
			case 0:
				m_attackCoolDownCount = 0;
				m_attackDir = GetAttackDir();//�I�u�W�F�N�g�Ɍ������x�N�g���𐳋K����������
				m_enemyUpdate = &Gorori::AttackUpdate;

				break;
			default:
				m_attackCoolDownCount = 250;
				break;
			}
		}
	}
}

void Gorori::AttackUpdate()
{

	m_rigid.SetVelocity(m_attackDir * 5);
	m_attackCount++;
	if (m_attackCount > 1300)
	{
		m_enemyUpdate = &Gorori::IdleUpdate;
	}
}

Vec3 Gorori::GetAttackDir() const
{
	Vec3 toVec = ToVec(m_rigid.GetPos(), m_target->GetRigidbody().GetPos());
	Vec3 vec = norm(ToVec(m_rigid.GetPos(), m_target->GetRigidbody().GetPos()));
	vec = VGet(vec.x * abs(toVec.x), vec.y * abs(toVec.y), vec.z * abs(toVec.z));
	return vec;
}
