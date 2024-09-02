#pragma once
#include <memory>
#include <vector>
#include "../ObjectTag.h"
#include "Rigidbody.h"
#include "ColliderBase.h"

namespace MyEngine
{
	class Physics;
//	class ColliderBase;

	/// <summary>
	/// �Փ˂ł������
	/// </summary>
	class Collidable : public std::enable_shared_from_this<Collidable>
	{
		friend Physics;
	public:
		// �D��x����
		enum class Priority : int
		{
			Low,		// ��
			Middle,		// ��
			High,		// ��
			Static,		// �����Ȃ��i�ō��j
		};
	public:
		Collidable(Priority priority, ObjectTag tag);
		Collidable(Collidable* col);
		virtual ~Collidable();

		// �Փ˂����Ƃ�
		virtual void OnCollideEnter(std::shared_ptr<Collidable> colider) {}
		virtual void OnCollideStay(std::shared_ptr<Collidable> colider) {}
		virtual void OnCollideExit(std::shared_ptr<Collidable> colider) {}
		virtual void OnTriggerEnter(std::shared_ptr<Collidable> colider) {}
		virtual void OnTriggerStay(std::shared_ptr<Collidable> colider) {}
		virtual void OnTriggerExit(std::shared_ptr<Collidable> colider) {}

		/* Getter */
		ObjectTag GetTag() const { return m_tag; }
		Priority GetPriority() const { return m_priority; }

		// �����蔻��𖳎��i�X���[�j����^�O�̒ǉ�/�폜
		void AddThroughTag(ObjectTag tag);
		void RemoveThroughTag(ObjectTag tag);
		Rigidbody PlanetOnlyGetRigid() { return m_rigid; };
		void SetReverceGravityVec(Vec3 gravityReverce) { m_upVec = gravityReverce; }

		// �����蔻��𖳎��i�X���[�j����Ώۂ��ǂ���
		bool IsThroughTarget(std::shared_ptr<Collidable>) const;

	protected:
		std::shared_ptr<ColliderBase> AddCollider(const ColliderBase::Kind& kind);

	protected:
		// �����f�[�^
		Rigidbody m_rigid;
		// �����蔻��f�[�^
		std::list<std::shared_ptr<ColliderBase>> m_colliders;
		Vec3 m_upVec;
	
	private:
		std::list<ObjectTag>	throughTags;

		ObjectTag m_tag;
		Priority m_priority;
	};
}

