#pragma once
#include "Enemy.h"
#include"EnemySphere.h"



/// <summary>
/// ���E�Ɉړ����A���Ԋu�Ŏˌ����Ă���Ge
/// ���݂�����ƂԂ��
/// </summary>
class Takobo : public Enemy
{
public:

	/// <param name="m_modelhandle">�G�l�~�[�̃��f��</param>
	/// <param name="obj">�e����^����I�u�W�F</param>
	Takobo(Vec3 pos);
	~Takobo();

	void Init();
	void Update();
	void SetMatrix();
	void Draw();

	void WantToPlayer(Vec3 toPlayer);

	virtual void OnCollideEnter(std::shared_ptr<Collidable> colider);


	void SetCameraAngle(float cameraAngle);

	void SetAttackDir(Vec3 targetPos);
	Vec3 GetMyPos();
	int WatchHp() const { return m_Hp; }
	float GetColRadius() { return m_radius; }

	float GetIdleSpeed() { return m_idleSpeed; }


	std::list<std::shared_ptr<EnemySphere>> GetAttackObj() { return m_sphere; }

	//�����o�֐��|�C���^
	using enemyState_t = void(Takobo::*)();
	enemyState_t m_enemyUpdate;
private:
	/// <summary>
	/// �o�ꎞ
	/// </summary>
	void StartUpdate();
	/// <summary>
	/// �ʏ�(�N�[���_�E��)���
	/// </summary>
	void IdleUpdate();
	/// <summary>
	/// �_���[�W���󂯂���
	/// </summary>
	void OnDamageUpdate();
	/// <summary>
	/// ���̂𐶐����čU��
	/// </summary>
	void AttackSphereUpdate();
	/// <summary>
	/// ���e���ォ�瓊�����čU��
	/// </summary>
	void AttackBombUpdate();

	Vec3 GetAttackDir() const;
private:
	int m_Hp;

	int m_attackCoolDownCount;

	float m_radius = 0;

	int m_modelHandle = 0;

	int m_createFrameCount = 0;

	float m_centerToEnemyAngle;

	float m_idleSpeed = 0;

	int m_sphereNum = 0;
	int m_bombNum = 0;

	Vec3 m_vec;
	Vec3 m_attackDir;
	Vec3 m_moveShaftPos;
	//�A�j���[�V�����ϐ�
	int m_anim_nutral = 0;
	int m_anim_move = 0;
	int m_anim_jump = 0;
	int m_anim_attack = 0;

	std::list<std::shared_ptr<EnemySphere>> m_sphere;
};

