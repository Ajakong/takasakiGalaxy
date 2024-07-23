#include "Enemy.h"

Enemy::Enemy(int handle, Priority priority, ObjectTag tag):Collidable(priority,tag),
	m_handle(handle)
{
}

Enemy::~Enemy()
{
}
