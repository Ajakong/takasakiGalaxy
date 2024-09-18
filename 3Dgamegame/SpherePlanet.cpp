#include"MyLib/Physics/ColliderSphere.h"
#include "SpherePlanet.h"
#include"ModelManager.h"

namespace
{
	constexpr float kGroundRadius = 500;
	constexpr float  kGravityRange = 1500;
	constexpr float  kGravityPower = 30;

	const char* name = "planet";
	const char* atom = "atomosphere";

}

SpherePlanet::SpherePlanet(Vec3 pos,int color,float gravity,int modelHandle) :Planet(),
m_enemyCount(3),
m_modelHandle(modelHandle)
{
	m_color = color;
	m_rigid->SetPos(pos);
	gravityPower = gravity;
	m_pointLightHandle = CreatePointLightHandle(m_rigid->GetPos().VGet(), 5000.0f, 0.0f, 0.002f, 0.0f);
	MV1SetScale(m_modelHandle, VGet(5, 5, 5));
	MV1SetPosition(m_modelHandle,m_rigid->GetPos().VGet());

}

SpherePlanet::~SpherePlanet()
{
}

void SpherePlanet::Init()
{
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);//�����œ��ꂽ�̂͏d�͂̉e���͈�
	m_colliders.back()->isTrigger = true;
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = kGravityRange;
	AddThroughTag(ObjectTag::Stage);
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);//�}�b�v�̓����蔻��
	auto item2 = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item2->radius = kGroundRadius;
}

void SpherePlanet::Update()
{
}

void SpherePlanet::Draw()
{

	//DrawSphere3D(m_rigid->GetPos().VGet(), kGravityRange, 10, 0xddddff, 0x0000ff, false);
	if (m_isSearch)
	{
		DrawSphere3D(m_rigid->GetPos().VGet(), kGroundRadius, 50, m_color, 0x0000ff, false);

	}
	else
	{
		MV1DrawModel(m_modelHandle);
	}
}

Vec3 SpherePlanet::GravityEffect(std::shared_ptr<Collidable> obj)//�������ƂɌv�Z���A�␳��̃x�N�g����Ԃ�
{
	Vec3 objVelocity = obj->PlanetOnlyGetRigid()->GetVelocity();
	if (obj->IsAntiGravity())
	{
		return objVelocity;
	}

	if (obj->GetTag() == ObjectTag::EnemyAttack)
	{
		return objVelocity;
	}

	////�f���̒��S����y�����ɐL�΂����������ɂ��A�I�u�W�F�N�g�̈ʒu�����Ď��Ƙf���̒��S����I�u�W�F�N�g�Ɍ������x�N�g���̊p�x�������I�u�W�F�N�g�̃x���V�e�B��y�����ɉe��������Ƃ����l�����AX�ɐi�݂����ꍇ����X�����,Z�ɐi�݂����ꍇ����Z���
	////Y�͖@���̊p�x�ɉ�]������
	Vec3 ansVelocity;
	Vec3 objPos = obj->PlanetOnlyGetRigid()->GetPos();
	Vec3 toObj = m_rigid->GetPos() - objPos;
	toObj = toObj.GetNormalized();
	if (obj->GetTag() == ObjectTag::Gorori||obj->GetTag()==ObjectTag::KillerTheSeeker)
	{
		float angleX = DX_PI_F / 2 + atan2(toObj.y, toObj.x);
		float angleZ = DX_PI_F / 2 + atan2(toObj.y, toObj.z);
		ansVelocity = { objVelocity.x * cos(angleX), objVelocity.x * sin(angleX) + objVelocity.z * sin(angleZ), objVelocity.z * cos(angleZ) };
		ansVelocity += toObj * objVelocity.y;//�v���C���[�̃W�����v���̃x�N�g���̉��Z

		ansVelocity += toObj * kGravityPower;
		obj->SetReverceGravityVec(toObj.GetNormalized());

		/*VECTOR ANSVECTOR = VGet(objVelocity.x * cos(angleX), objVelocity.x * sin(angleX) + objVelocity.z * sin(angleZ), objVelocity.z * cos(angleZ));
		ANSVECTOR = VAdd(ANSVECTOR, objVelocity.y * toObj);
		ansVelocity = ANSVECTOR;*/
		//ansVelocity -= toObj;
		return ansVelocity+ toObj * gravityPower*40*((kGravityRange+(obj->GetRigidbody()->GetPos() - m_rigid->GetPos()).Length() -(obj->GetRigidbody()->GetPos()-m_rigid->GetPos()).Length())/ kGravityRange);
	}

	if (obj->GetTag() == ObjectTag::Player)
	{
		//�d�͂̂�
		toObj = toObj * gravityPower*0.05f* ((kGravityRange+ (obj->GetRigidbody()->GetPos() - m_rigid->GetPos()).Length() - (obj->GetRigidbody()->GetPos() - m_rigid->GetPos()).Length()) / kGravityRange) + objVelocity;
		return toObj;
	}

	//�d�͂̂�
	toObj = toObj * gravityPower * ((kGravityRange +(obj->GetRigidbody()->GetPos()-m_rigid->GetPos()).Length()- (obj->GetRigidbody()->GetPos() - m_rigid->GetPos()).Length()) / kGravityRange) + objVelocity;
	return toObj;
}

Vec3 SpherePlanet::GetNormVec(Vec3 pos)
{
	Vec3 norm = pos - m_rigid->GetPos();
	norm.Normalize();
	return norm;
}

void SpherePlanet::OnTriggerEnter(std::shared_ptr<Collidable> colider)
{
	/*if (colider->GetTag() == ObjectTag::Takobo)
	{
		m_enemyCount++;
	}*/
}

void SpherePlanet::OnTriggerExit(std::shared_ptr<Collidable> colider)
{
	if (colider->GetTag() == ObjectTag::Takobo)
	{
		m_enemyCount--;
	}
	if (m_enemyCount <= 0)
	{
		clearFlag = true;
	}
}
