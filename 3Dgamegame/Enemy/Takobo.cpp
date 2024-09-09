#include "Takobo.h"
#include"../MyLib/Physics/ColliderSphere.h"
#include"../MyLib/Physics/Physics.h"
#include"../SoundManager.h"

namespace
{
	constexpr float kCollisionRadius = 50.f;

	/// <summary>
		/// �ő�HP
		/// </summary>
	constexpr int kHp = 50;

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
	constexpr int kAttackCoolDownTime = 300;

	/// <summary>
	/// �X�e�[�W���f���̏c���T�C�Y/2
	/// </summary>
	constexpr int kStageSizeHalf = 200;

	const char* kShotSEhandlePath = "Shot.mp3";

	const char* name = "takobo";
}


Takobo::Takobo(Vec3 pos) :Enemy(-1, Priority::Low, ObjectTag::Takobo),
m_Hp(kHp),
m_attackCoolDownCount(0),
m_centerToEnemyAngle(0)
{
	SetCreate3DSoundFlag(true);
	m_shotSEHandle = SoundManager::GetInstance().GetSoundData(kShotSEhandlePath);
	SetCreate3DSoundFlag(false);
	Set3DRadiusSoundMem(1000, m_shotSEHandle);
	m_enemyUpdate = &Takobo::IdleUpdate;
	m_rigid->SetPos(pos);
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = kCollisionRadius;
	m_moveShaftPos = m_rigid->GetPos();
	AddThroughTag(ObjectTag::Takobo);
	AddThroughTag(ObjectTag::Gorori);
	AddThroughTag(ObjectTag::WarpGate);
}

Takobo::~Takobo()
{

}

void Takobo::Init()
{
}

void Takobo::Update()
{
	(this->*m_enemyUpdate)();

	for (auto& sphere : m_sphere)
	{
		if (m_sphere.size() == 0)return;
		sphere->Update();
	}


}

void Takobo::SetMatrix()
{
	MATRIX moving = MGetTranslate(m_rigid->GetPos().VGet());

	MV1SetMatrix(m_handle, moving);
}

void Takobo::DeleteManage()
{
	auto result = remove_if(m_sphere.begin(), m_sphere.end(), [this](const auto& sphere)
		{
			bool isOut = sphere->IsDelete() == true;
	if (isOut == true)
	{
		m_sphereNum--;
		MyEngine::Physics::GetInstance().Exit(sphere);
	}
	return isOut;
		});
	m_sphere.erase(result, m_sphere.end());
}

void Takobo::Draw()
{

	DrawSphere3D(m_rigid->GetPos().VGet(), kCollisionRadius, 10, 0xff0000, 0xff0000, false);
	MV1DrawModel(m_handle);

	for (auto& sphere : m_sphere)
	{
		if (m_sphere.size() == 0)return;
		sphere->Draw();
	}
}

void Takobo::OnCollideEnter(std::shared_ptr<Collidable> colider)
{
	if (colider->GetTag() == ObjectTag::Stage)
	{
		m_nowPlanetPos = colider->GetRigidbody()->GetPos();
	}
	if (colider->GetTag() == ObjectTag::Player)
	{
		m_Hp -= 20;
	}
	if (colider->GetTag() == ObjectTag::EnemyAttack)
	{
		auto attack= dynamic_pointer_cast<EnemySphere>(colider);
		attack->DeleteFlag();
		if (attack->GetCounterFlag())
		{
			m_Hp -= 60;
		}
	}
}

Vec3 Takobo::GetMyPos()
{
	return  VGet(m_rigid->GetPos().x, m_rigid->GetPos().y + kFootToCenter, m_rigid->GetPos().z);
}

void Takobo::SetTarget(std::shared_ptr<Collidable> target)
{
	m_target = target;
}

void Takobo::IdleUpdate()
{
	m_vec.x = 1;
	if (abs(m_rigid->GetPos().x - m_moveShaftPos.x) > 5)
	{
		m_vec.x *= -1;
	}

	m_rigid->SetVelocity(VGet(m_vec.x, 0, 0));

	m_attackCoolDownCount++;

	if (m_attackCoolDownCount > kAttackCoolDownTime)
	{
		int attackState = GetRand(1);
		switch (attackState)
		{
		case 0:
		{
			Vec3 norm = (m_rigid->GetPos() - m_nowPlanetPos).GetNormalized();
			Vec3 toTarget = ToVec(norm, m_target->GetRigidbody()->GetPos());
			if (toTarget.Length() > 500)break;
			float a = acos(Dot(norm, toTarget.GetNormalized())) * 180 / DX_PI_F;

			if (a < 120)
			{
				m_attackCoolDownCount = 0;
				m_attackDir = GetAttackDir();//�I�u�W�F�N�g�Ɍ������x�N�g���𐳋K����������
				m_enemyUpdate = &Takobo::AttackSphereUpdate;
			}
			break;
		}
		default:
			m_attackCoolDownCount = 250;
			break;
		}
	}
}

void Takobo::AttackSphereUpdate()
{
	m_rigid->SetVelocity(VGet(0, 0, 0));

	m_sphereNum++;

	m_createFrameCount = 0;
	Set3DPositionSoundMem(m_rigid->GetPos().VGet(), m_shotSEHandle);
	PlaySoundMem(m_shotSEHandle,DX_PLAYTYPE_BACK);
	m_sphere.push_back(std::make_shared<EnemySphere>(Priority::Low, ObjectTag::EnemyAttack, shared_from_this(), GetMyPos(), m_attackDir, 1, 0xff0000));
	MyEngine::Physics::GetInstance().Entry(m_sphere.back());

	m_enemyUpdate = &Takobo::IdleUpdate;
}

Vec3 Takobo::GetAttackDir() const
{
	Vec3 toVec = ToVec(m_rigid->GetPos(), m_target->GetRigidbody()->GetPos());
	Vec3 vec = norm(ToVec(m_rigid->GetPos(), m_target->GetRigidbody()->GetPos()));
	vec = VGet(vec.x * abs(toVec.x), vec.y * abs(toVec.y), vec.z * abs(toVec.z));
	return vec;
}

//void Takobo::SetAttackDir(VECTOR targetPos)
//{
//	m_attackDir = norm(ToVec(GetMyPos(), targetPos));
//}
//
//VECTOR Takobo::GetMyPos()
//{
//	return VGet(m_pos.x, m_pos.y + kFootToCenter, m_pos.z);//���f���̒��S�̍��W
//}
//
//void Takobo::StartUpdate()
//{
//	m_velocity.y = -1;
//
//	//���f���̃T�C�Y����S
//	MATRIX scaleMtx = MGetScale(VGet(0.5f, 0.5f, 0.5f));//XYZ���ꂼ��1/2�X�P�[��
//	m_attackDir = norm(ToVec(m_pos, m_obj->GetPos()));//�I�u�W�F�N�g�Ɍ������x�N�g���𐳋K����������
//
//	float Angle = -DX_PI_F / 2 - atan2(m_attackDir.z, m_attackDir.x);
//
//	m_pos.y += m_velocity.y;
//
//	MATRIX transMtx = MGetTranslate(m_pos);
//	MATRIX rotateMtx = MGetRotY(Angle);
//	MATRIX Mtx = MMult(scaleMtx, rotateMtx);
//	Mtx = MMult(Mtx, transMtx);
//
//	MV1SetMatrix(m_modelHandle, Mtx);
//
//	if (m_pos.y <= 0)
//	{
//		m_pos.y = 0;
//		m_TakoboUpdate = &Takobo::IdleUpdate;
//	}
//}
//
//void Takobo::IdleUpdate()
//{
//	//���f���̃T�C�Y����S
//	MATRIX scaleMtx = MGetScale(VGet(0.5f, 0.5f, 0.5f));//XYZ���ꂼ��1/2�X�P�[��
//
//	m_centerToTakoboAngle += m_idleSpeed;
//	float Angle = -DX_PI_F / 2 - atan2(GetAttackDir().z, GetAttackDir().x);
//	float Length = sqrt(m_pos.x * m_pos.x + m_pos.z * m_pos.z);
//
//	m_pos.x = Length * static_cast<float>(cos(m_centerToTakoboAngle * DX_PI_F / 180.0));
//	m_pos.z = Length * static_cast<float>(sin(m_centerToTakoboAngle * DX_PI_F / 180.0));
//
//	MATRIX transMtx = MGetTranslate(m_pos);
//	MATRIX rotateMtx = MGetRotY(Angle);
//	MATRIX Mtx = MMult(scaleMtx, rotateMtx);
//	Mtx = MMult(Mtx, transMtx);
//
//	MV1SetMatrix(m_modelHandle, Mtx);
//
//	m_attackCoolDownCount++;
//
//	if (m_attackCoolDownCount > kAttackCoolDownTime)
//	{
//		int attackState = GetRand(1);
//		switch (attackState)
//		{
//		case 0:
//			m_attackCoolDownCount = 0;
//			m_TakoboUpdate = &Takobo::AttackSphereUpdate;
//
//			/*m_attackCoolDownCount = 0;
//			m_TakoboUpdate = &Takobo::AttackBombUpdate;*/
//		default:
//			break;
//		}
//	}
//}
//
//void Takobo::OnDamageUpdate()
//{
//
//}
//
//void Takobo::AttackSphereUpdate()
//{
//	m_createFrameCount++;
//
//
//	if (m_createFrameCount > kSphereCreateFrame)
//	{
//		m_sphereNum++;
//		if (m_sphereNum <= 5)
//		{
//			m_attackDir = GetAttackDir();//�I�u�W�F�N�g�Ɍ������x�N�g���𐳋K����������
//
//			m_createFrameCount = 0;
//			m_sphere.push_back(std::make_shared<TakoboAttackSphere>(shared_from_this(), GetMyPos(), m_attackDir, 1));
//		}
//		else
//		{
//			m_sphereNum = 0;
//			m_sphere.push_back(std::make_shared<FightBackObj>(shared_from_this(), GetMyPos(), m_attackDir, 1, 0x00ff00));
//
//			m_idleSpeed = static_cast<float>(GetRand(kIdleSpeed) + 1);//�U����̈ړ����x
//			m_TakoboUpdate = &Takobo::IdleUpdate;
//		}
//	}
//}
//
//void Takobo::AttackBombUpdate()
//{
//	m_createFrameCount++;
//	if (m_createFrameCount > kBombCreateFrame)
//	{
//		m_bombNum++;
//
//		m_sphere.push_back(std::make_shared<TakoboAttackBomb>(shared_from_this(),
//			VGet(static_cast<float>(GetRand(kStageSizeHalf) - kStageSizeHalf), kBombCreatePosY, static_cast<float>(GetRand(kStageSizeHalf) - kStageSizeHalf)),
//			VGet(0, -1, 0), 1));
//	}
//	if (m_bombNum == 1)
//	{
//		m_bombNum = 0;
//		m_idleSpeed = static_cast<float>(GetRand(kIdleSpeed) + 1);
//		m_TakoboUpdate = &Takobo::IdleUpdate;
//	}
//}

