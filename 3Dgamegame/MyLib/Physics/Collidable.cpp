#include "Collidable.h"
#include "ColliderSphere.h"

using namespace MyEngine;

Collidable::Collidable(Priority priority, ObjectTag tag) :
	m_priority(priority),
	m_tag(tag)
{
}

Collidable::~Collidable()
{
}

std::shared_ptr<ColliderBase> MyEngine::Collidable::AddCollider(const ColliderBase::Kind& kind)
{
	std::shared_ptr<ColliderBase> collider;

	if (kind == ColliderBase::Kind::Sphere)
	{
		collider = std::make_shared<ColliderSphere>();
	}
	else if (kind == ColliderBase::Kind::Capsule)
	{

	}

	if (collider)
	{
		m_colliders.emplace_back(collider);
	}

	return collider;
}
