#include "Physics.h"
#include <cassert>
#include "Collidable.h"
#include "ColliderSphere.h"
#include "../DebugDraw.h"
#include"../../Planet.h"

using namespace MyEngine;

namespace
{
	// ”»’èÅ‘å‰ñ”
	constexpr int CHECK_COUNT_MAX = 1000;
}

Physics::Physics()
{
}

Physics& Physics::GetInstance()
{
	static Physics instance;
	return instance;
}

void Physics::Entry(const std::shared_ptr<Collidable>& collidable)
{
	bool isFound = std::find(m_collidables.begin(), m_collidables.end(), collidable) != m_collidables.end();
	//// –¢“o˜^‚È‚ç’Ç‰Á
	//if (!isFound)
	//{
	//    m_collidables.emplace_back(collidable);
	//}
	//// “o˜^Ï‚İ‚È‚ç–³‹
	//else
	//{
	//    assert(false);
	//}

	// –¢“o˜^‚È‚ç’Ç‰Á
	if (!isFound && collidable->GetTag() != ObjectTag::Stage)
	{
		m_collidables.emplace_back(collidable);
	}
	else if (!isFound)
	{
		m_stageCollidables.emplace_back(collidable);
		m_collidables.emplace_back(collidable);
	}
	// “o˜^Ï‚İ‚È‚ç–³‹
	else
	{
		assert(false);
	}
}

void Physics::Exit(const std::shared_ptr<Collidable>& collidable)
{
	bool isFound = std::find(m_collidables.begin(), m_collidables.end(), collidable) != m_collidables.end();
	// “o˜^Ï‚İ‚È‚çíœ
	if (isFound)
	{
		m_collidables.remove(collidable);
	}
	// –¢“o˜^‚È‚ç–³‹
	else
	{
		assert(false);
	}
}

void Physics::Update()
{
	m_preCollideInfo = m_newCollideInfo;
	m_newCollideInfo.clear();
	m_preTirrigerInfo = m_newTirrigerInfo;
	m_newTirrigerInfo.clear();
	m_onCollideInfo.clear();

	MoveNextPos();

	CheckCollide();

	CheckSendOnCollideInfo(m_preCollideInfo, m_newCollideInfo, false);
	CheckSendOnCollideInfo(m_preTirrigerInfo, m_newTirrigerInfo, true);

	FixPos();

	for (const auto& info : m_onCollideInfo)
	{
		OnCollideInfo(info.own, info.send, info.kind);
	}
}

/// <summary>
/// •¨—‚©‚ç‚ÌˆÚ“®‚ğ–¢—ˆ‚ÌÀ•W‚É“K—p
/// </summary>
void MyEngine::Physics::MoveNextPos() const
{
	for (const auto& item : m_stageCollidables)
	{
		for (auto& obj : m_collidables)
		{
			if (obj->GetTag() != ObjectTag::Stage)
			{
				auto planet = dynamic_cast<Planet*>(item.get());
				obj->m_rigid.SetVelocity(planet->GravityEffect(obj));
			}

		}

	}

	for (const auto& item : m_collidables)
	{
		auto& rigid = item->m_rigid;

		auto pos = rigid.GetPos();
		auto nextPos = pos + rigid.GetVelocity();

		rigid.SetNextPos(nextPos);

#ifdef _DEBUG
		auto& debug = DebugDraw::GetInstance();

		for (const auto& collider : item->m_colliders)
		{
			auto kind = collider->GetKind();
			if (kind == ColliderBase::Kind::Sphere)
			{
				auto sphereData = dynamic_cast<ColliderSphere*>(collider.get());
				DebugDraw::SphereInfo preInfo;
				preInfo.center = pos;
				preInfo.radius = sphereData->radius;
				preInfo.color = DebugDraw::COL_BEFOR;
				debug.DrawSphere(preInfo);
				DebugDraw::SphereInfo newInfo;
				newInfo.center = nextPos;
				newInfo.radius = sphereData->radius;
				newInfo.color = DebugDraw::COL_NEXT;
				debug.DrawSphere(newInfo);
			}
		}
#endif
	}
}

void MyEngine::Physics::CheckCollide()
{
	bool isCheck = true;
	int checkCount = 0;
	std::unordered_map<Collidable*, std::list<Collidable*>> newCollideInfo;
	while (isCheck)
	{
		isCheck = false;
		++checkCount;

		for (const auto& objA : m_collidables)
		{
			for (const auto& objB : m_collidables)
			{
				if (objA == objB) continue;
				// ‚Ç‚¿‚ç‚©‚ÌƒXƒ‹[‘ÎÛ‚Æ‚µ‚Äƒ^ƒO‚ª“ü‚Á‚Ä‚¢‚½‚ç–³‹
				if (objA->IsThroughTarget(objB) || objB->IsThroughTarget(objA))continue;
				for (const auto& colA : objA->m_colliders)
				{
					for (const auto& colB : objB->m_colliders)
					{


						if (!IsCollide(objA->m_rigid, objB->m_rigid, colA, colB)) continue;

						bool isTrigger = colA->isTrigger || colB->isTrigger;

						if (isTrigger)
						{
							AddNewCollideInfo(objA.get(), objB.get(), m_newTirrigerInfo);
						}
						else
						{
							AddNewCollideInfo(objA.get(), objB.get(), m_newCollideInfo);
						}


						// Trigger‚Ìê‡‚ÍˆÊ’u•â³‚Í‚µ‚È‚¢
						if (isTrigger)
						{
							isCheck = false;
							continue;
						}
						auto primary = objA;
						auto secondary = objB;
						auto primaryCollider = colA;
						auto secondaryCollider = colB;
						if (objA->m_priority < objB->m_priority)
						{
							primary = objB;
							secondary = objA;
							primaryCollider = colB;
							secondaryCollider = colA;
						}

						FixNextPos(primary->m_rigid, secondary->m_rigid, primaryCollider, secondaryCollider);
						// ˆÊ’u•â³‚ğ‚µ‚½‚ç‚à‚¤ˆê“x‰‚ß‚©‚çs‚¤
						isCheck = true;
						break;
					}

					if (isCheck) break;
				}

				if (isCheck) break;
			}

			if (isCheck) break;
		}

		if (isCheck && checkCount > CHECK_COUNT_MAX)
		{
			printfDx(L"‹K’è”(%d)‚ğ’´‚¦‚Ü‚µ‚½", CHECK_COUNT_MAX);
			break;
		}
	}
}

bool Physics::IsCollide(const Rigidbody& rigidA, const Rigidbody& rigidB, const std::shared_ptr<ColliderBase>& colliderA, const std::shared_ptr<ColliderBase>& colliderB)
{

	bool isCollide = false;

	auto kindA = colliderA->GetKind();
	auto kindB = colliderB->GetKind();

	if (kindA == ColliderBase::Kind::Sphere && kindB == ColliderBase::Kind::Sphere)
	{
		auto sphereA = dynamic_cast<ColliderSphere*>(colliderA.get());
		auto sphereB = dynamic_cast<ColliderSphere*>(colliderB.get());

		auto aToB = rigidB.GetNextPos() - rigidA.GetNextPos();
		float sumRadius = sphereA->radius + sphereB->radius;
		isCollide = (aToB.SqLength() < sumRadius * sumRadius);
	}

	return isCollide;
}

void MyEngine::Physics::FixNextPos(const Rigidbody& primaryRigid, Rigidbody& secondaryRigid, const std::shared_ptr<ColliderBase>& primaryCollider, const std::shared_ptr<ColliderBase>& secondaryCollider)
{
	Vec3 fixedPos = secondaryRigid.GetNextPos();

	auto primaryKind = primaryCollider->GetKind();
	auto secondaryKind = secondaryCollider->GetKind();

	if (primaryKind == ColliderBase::Kind::Sphere)
	{
		if (secondaryKind == ColliderBase::Kind::Sphere)
		{
			auto primarySphere = dynamic_cast<ColliderSphere*>(primaryCollider.get());
			auto secondarySphere = dynamic_cast<ColliderSphere*>(secondaryCollider.get());

			// primary‚©‚çsecondary‚Ö‚ÌƒxƒNƒgƒ‹‚ğì¬
			auto primaryToSecondary = secondaryRigid.GetNextPos() - primaryRigid.GetNextPos();
			// ‚»‚Ì‚Ü‚Ü‚¾‚Æ‚¿‚å‚¤‚Ç“–‚½‚éˆÊ’u‚É‚È‚é‚Ì‚Å­‚µ—]•ª‚É—£‚·
			float  awayDist = primarySphere->radius + secondarySphere->radius + 0.0001f;
			// ’·‚³‚ğ’²®
			primaryToSecondary = primaryToSecondary.GetNormalized() * awayDist;
			// primary‚©‚çƒxƒNƒgƒ‹•ûŒü‚É‰Ÿ‚·
			fixedPos = primaryRigid.GetNextPos() + primaryToSecondary;
		}
	}

	secondaryRigid.SetNextPos(fixedPos);
}

void MyEngine::Physics::AddNewCollideInfo(Collidable* objA, Collidable* objB, SendCollideInfo& info)
{
	// A‚ªe‚Æ‚µ‚Äæ“¾‚µ‚Ä‚¢‚é‚©
	bool isParentA = info.find(objA) != info.end();
	// B‚ªe‚Æ‚µ‚Äæ“¾‚µ‚Ä‚¢‚é‚©
	bool isParentB = info.find(objB) != info.end();
	// A‚ªB‚Ç‚¿‚ç‚©‚ªæ“¾‚µ‚Ä‚¢‚éê‡
	if (isParentA || isParentB)
	{
		Collidable* parent = objA;
		Collidable* child = objB;
		if (isParentB)
		{
			parent = objB;
			child = objA;
		}
		// e‚ªq‚ğ‚Á‚Ä‚¢‚é‚©
		bool isChild = std::find(info[parent].begin(), info[parent].end(), child) != info[parent].end();
		// q‚Æ‚µ‚Ä‚Á‚Ä‚¢‚È‚¯‚ê‚Î’Ç‰Á
		if (!isChild)
		{
			info[parent].emplace_back(child);
		}
	}
	// ‚Ç‚¿‚ç‚àæ“¾‚µ‚Ä‚¢‚È‚¢ê‡
	else
	{
		// •’Ê‚É’Ç‰Á
		info[objA].emplace_back(objB);
	}
}

void MyEngine::Physics::CheckSendOnCollideInfo(SendCollideInfo& preSendInfo, SendCollideInfo& newSendInfo, bool isTrigger)
{
	for (auto& parent : newSendInfo)
	{
		// ˆÈ‘O‚Ìî•ñ‚Ée‚Æ‚µ‚Ä“o˜^‚³‚ê‚Ä‚¢‚é‚©
		bool isPreParent = preSendInfo.find(parent.first) != preSendInfo.end();
		bool isAllEnter = true;

		for (auto& child : parent.second)
		{
			bool isPreChild = false;
			if (isPreParent)
			{
				// ˆÈ‘O‚Ìî•ñ‚Éq‚Æ‚µ‚Ä“o˜^‚³‚ê‚Ä‚¢‚é‚©
				auto& preParent = preSendInfo[parent.first];
				isPreChild = std::find(preParent.begin(), preParent.end(), child) != preParent.end();
			}

			// ¡‰ñ“ü‚Á‚Ä‚«‚½ê‡‚ÍEnter‚ğŒÄ‚Ô(q‚Æ‚µ‚Ä“o˜^‚³‚ê‚Ä‚¢‚È‚¢)
			if (!isPreChild)
			{
				if (isTrigger)
				{
					AddOnCollideInfo(parent.first, child, OnCollideInfoKind::TriggerEnter);
					AddOnCollideInfo(child, parent.first, OnCollideInfoKind::TriggerEnter);
				}
				else
				{
					AddOnCollideInfo(parent.first, child, OnCollideInfoKind::CollideEnter);
					AddOnCollideInfo(child, parent.first, OnCollideInfoKind::CollideEnter);
				}
			}

			// Stary‚Í–ˆ“xŒÄ‚Ô
			if (isTrigger)
			{
				AddOnCollideInfo(parent.first, child, OnCollideInfoKind::TriggerStay);
				AddOnCollideInfo(child, parent.first, OnCollideInfoKind::TriggerStay);
			}
			else
			{
				AddOnCollideInfo(parent.first, child, OnCollideInfoKind::CollideStay);
				AddOnCollideInfo(child, parent.first, OnCollideInfoKind::CollideStay);
			}

			// “o˜^‚³‚ê‚Ä‚¢‚½î•ñ‚ğíœ
			if (isPreChild)
			{
				preSendInfo[parent.first].remove(child);
			}
			// ‘S‚Ä“o˜^‚³‚ê‚Ä‚¢‚È‚©‚Á‚½‚Æ‚·‚é
			else
			{
				isAllEnter = false;
			}
		}

		// ‘S‚Ä“o˜^‚³‚ê‚Ä‚¢‚½‚çe‚Ìî•ñ‚àÁ‚·
		if (isAllEnter)
		{
			preSendInfo.erase(parent.first);
		}
	}

	// c‚Á‚Ä‚¢‚é‘Oî•ñ‚©‚çExist‚ğŒÄ‚Ô(“o˜^‚ªc‚Á‚Ä‚¢‚é¡‰ñ”²‚¯‚½)
	for (auto& parent : preSendInfo)
	{
		for (auto& child : parent.second)
		{
			if (isTrigger)
			{
				AddOnCollideInfo(parent.first, child, OnCollideInfoKind::TriggerExit);
				AddOnCollideInfo(child, parent.first, OnCollideInfoKind::TriggerExit);
			}
			else
			{
				AddOnCollideInfo(parent.first, child, OnCollideInfoKind::CollideExit);
				AddOnCollideInfo(child, parent.first, OnCollideInfoKind::CollideExit);
			}
		}
	}
}

void MyEngine::Physics::AddOnCollideInfo(Collidable* own, Collidable* send, OnCollideInfoKind kind)
{
	OnCollideInfoData info;
	info.own = own;
	info.send = send;
	info.kind = kind;
	m_onCollideInfo.emplace_back(info);
}

void MyEngine::Physics::OnCollideInfo(Collidable* own, Collidable* send, OnCollideInfoKind kind)
{
	if (kind == OnCollideInfoKind::CollideEnter)
	{
		own->OnCollideEnter(*send);
	}
	else if (kind == OnCollideInfoKind::CollideStay)
	{
		own->OnCollideStay(*send);
	}
	else if (kind == OnCollideInfoKind::CollideExit)
	{
		own->OnCollideExit(*send);
	}
	else if (kind == OnCollideInfoKind::TriggerEnter)
	{
		own->OnTriggerEnter(*send);
	}
	else if (kind == OnCollideInfoKind::TriggerStay)
	{
		own->OnTriggerStay(*send);
	}
	else if (kind == OnCollideInfoKind::TriggerExit)
	{
		own->OnTriggerExit(*send);
	}
}

/// <summary>
/// ÅI“I‚È–¢—ˆ‚ÌÀ•W‚©‚çŒ»İ‚ÌÀ•W‚É“K—p
/// </summary>
void Physics::FixPos() const
{
	for (const auto& item : m_collidables)
	{
		auto& rigid = item->m_rigid;

		rigid.SetPos(rigid.GetNextPos());

#ifdef _DEBUG
		auto& debug = DebugDraw::GetInstance();
		for (const auto& collider : item->m_colliders)
		{
			auto kind = collider->GetKind();
			if (kind == ColliderBase::Kind::Sphere)
			{
				auto sphereData = dynamic_cast<ColliderSphere*>(collider.get());
				DebugDraw::SphereInfo info;
				info.center = rigid.GetPos();
				info.radius = sphereData->radius;
				info.color = DebugDraw::COL_AFFTER;
				debug.DrawSphere(info);
			}
		}
#endif
	}

}
