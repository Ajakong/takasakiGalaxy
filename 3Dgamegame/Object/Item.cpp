#include "Item.h"
#include"../MyLib/Physics/ColliderSphere.h"
#include"../MyLib/Physics/Physics.h"

Item::Item(Vec3 pos):Collidable(Priority::Low,ObjectTag::Item)
{
	m_rigid.SetPos(pos);
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = 60;
}

Item::~Item()
{
}
