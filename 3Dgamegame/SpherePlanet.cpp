#include"MyLib/Physics/ColliderSphere.h"
#include "SpherePlanet.h"

namespace
{
	constexpr float kGroundRadius = 500;
	constexpr float  kGravityRange = 1500;
	constexpr float  kGravityPower = 3;


}

SpherePlanet::SpherePlanet(Vec3 pos):Planet(),
m_enemyCount(0)
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
	m_rigid.SetPos(pos);
}

SpherePlanet::~SpherePlanet()
{
}

void SpherePlanet::Init()
{
}

void SpherePlanet::Update()
{
}

void SpherePlanet::Draw()
{
	DrawSphere3D(m_rigid.GetPos().VGet(), kGravityRange, 10, 0xddddff, 0x0000ff, false);
	DrawSphere3D(m_rigid.GetPos().VGet(), kGroundRadius, 50, 0xaadd33, 0xff0000, true);
	//printfDX("m_enemyCount:%d", m_enemyCount);
}

Vec3 SpherePlanet::GravityEffect(std::shared_ptr<Collidable> obj)//�������ƂɌv�Z���A�␳��̃x�N�g����Ԃ�
{
	////�f���̒��S����y�����ɐL�΂����������ɂ��A�I�u�W�F�N�g�̈ʒu�����Ď��Ƙf���̒��S����I�u�W�F�N�g�Ɍ������x�N�g���̊p�x�������I�u�W�F�N�g�̃x���V�e�B��y�����ɉe��������Ƃ����l�����AX�ɐi�݂����ꍇ����X�����,Z�ɐi�݂����ꍇ����Z���
	////Y�͖@���̊p�x�ɉ�]������
	Vec3 ansVelocity;
	Vec3 objPos=obj->PlanetOnlyGetRigid().GetPos();
	Vec3 objVelocity = obj->PlanetOnlyGetRigid().GetVelocity();
	Vec3 toObj= m_rigid.GetPos()-objPos;
	toObj=toObj.GetNormalized();

	//float angleX = DX_PI_F / 2 + atan2(toObj.y, toObj.x);
	//float angleZ= DX_PI_F / 2 + atan2(toObj.y, toObj.z);
	//ansVelocity = { objVelocity.x * cos(angleX), objVelocity.x * sin(angleX) + objVelocity.z * sin(angleZ), objVelocity.z * cos(angleZ) };
	//ansVelocity += toObj*objVelocity.y;//�v���C���[�̃W�����v���̃x�N�g���̉��Z

	//ansVelocity += toObj * kGravityPower;
	//obj->SetReverceGravityVec(toObj.GetNormalized());

	///*VECTOR ANSVECTOR = VGet(objVelocity.x * cos(angleX), objVelocity.x * sin(angleX) + objVelocity.z * sin(angleZ), objVelocity.z * cos(angleZ));
	//ANSVECTOR = VAdd(ANSVECTOR, objVelocity.y * toObj);
	//ansVelocity = ANSVECTOR;*/
	////ansVelocity -= toObj;
	//return ansVelocity;
	
	//�d�͂̂�
	toObj = toObj * gravityPower+objVelocity;
	return toObj;
}

Vec3 SpherePlanet::GetNormVec(Vec3 pos)
{
	Vec3 norm = pos - m_rigid.GetPos();
	norm.Normalize();
	return norm;
}

void SpherePlanet::OnTriggerEnter(std::shared_ptr<Collidable> colider)
{
	if (colider->GetTag() == ObjectTag::Takobo)
	{
		m_enemyCount++;
	}
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
