#include "SpherePlanet.h"
#include"MyLib/Physics/ColliderSphere.h"

namespace
{
	constexpr float kGroundRadius = 100;
	constexpr float  kGravityRange = 150;

}

SpherePlanet::SpherePlanet():Planet()
{
	m_rigid.SetGravity(30);
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);//ここで入れたのは重力の影響範囲
	m_colliders.back()->isTrigger = true;
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = kGravityRange;
	AddThroughTag(ObjectTag::Stage);
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);//マップの当たり判定
	auto item2 = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item2->radius = kGroundRadius;
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
}

Vec3 SpherePlanet::GravityEffect(std::shared_ptr<Collidable> obj)//成分ごとに計算し、補正後のベクトルを返す
{
	//惑星の中心からy方向に伸ばした線を軸にし、オブジェクトの位置を見て軸と惑星の中心からオブジェクトに向かうベクトルの角度分だけオブジェクトのベロシティのy方向に影響させるという考え方、Xに進みたい場合軸のXを基準に,Zに進みたい場合軸のZを基準
	//Yは法線の角度に回転させる
	Vec3 ansVelocity;
	Vec3 objPos=obj->PlanetOnlyGetRigid().GetPos();
	Vec3 objVelocity = obj->PlanetOnlyGetRigid().GetVelocity();
	Vec3 toObj= m_rigid.GetPos()-objPos;
	toObj=toObj.GetNormalized();
	float angleX = DX_PI_F / 2 + atan2(toObj.y, toObj.x);
	float angleZ= DX_PI_F / 2 + atan2(toObj.y, toObj.z);
	ansVelocity = { objVelocity.x * cos(angleX), objVelocity.x * sin(angleX) + objVelocity.z * sin(angleZ), objVelocity.z * cos(angleZ) };
	ansVelocity += toObj*objVelocity.y;//プレイヤーのジャンプ分のベクトルの加算

	/*VECTOR ANSVECTOR = VGet(objVelocity.x * cos(angleX), objVelocity.x * sin(angleX) + objVelocity.z * sin(angleZ), objVelocity.z * cos(angleZ));
	ANSVECTOR = VAdd(ANSVECTOR, objVelocity.y * toObj);
	ansVelocity = ANSVECTOR;*/
	//ansVelocity -= toObj;
	return ansVelocity;
}
