#pragma once
#include "MyLib/Physics/Collidable.h"
#include"MyLib/Vec3.h"
class Player : public MyEngine::Collidable
{
public:
	Player(int modelhandle);
	~Player();

	void Init();

	void Update();
	void Draw();


	void SetCameraToPlayer(Vec3 cameraToPlayer);

	void Hit();
	int WatchHp()const { return m_Hp; }

	void SetCameraAngle(float cameraAngle);

	int GetPlayerModelhandle() { return m_modelHandle; }

	//�����o�֐��|�C���^
	using playerState_t = void(Player::*)();
	playerState_t m_playerUpdate;

private:
	//�A�j���[�V�����̐i�s
	//���[�v�������ǂ�����Ԃ�
	bool UpdateAnim(int attachNo);
	//�A�j���[�V�����̕ύX
	void ChangeAnim(int animIndex);

	//��ԕʊ֐�(�|�C���^�ŌĂяo��)

	/// <summary>
	/// �J�n����ɌĂ΂��
	/// </summary>
	void StartUpdate();
	/// <summary>
	/// �ʏ펞
	/// </summary>
	void NeutralUpdate();
	void WalkingUpdate();
	void JumpingUpdate();
	/// <summary>
	/// �Փˊ֐�
	/// </summary>
	void HitUpdate();
	/// <summary>
	/// ���
	/// </summary>
	void AvoidUpdate();

	Vec3 GetCameraToPlayer()const;

private:
	int m_Hp;
	int m_modelHandle = 0;

	int rotRad = 0;

	/// <summary>
	/// �s���̃t���[�����Ǘ�����
	/// </summary>
	int actionFrame = 0;

	float m_radius = 0;

	Vec3 m_velocity;
	Vec3 m_cameraToPlayer;

	int m_currentAnimNo;//���݂̃A�j���[�V����
	int m_prevAnimNo;//�ύX�O�̃A�j���[�V����
	float m_animBlendRate;//�A�j���[�V�����̍�������
	//0.0f:prev���Đ�
	//1.0:current���Đ�

	//�A�j���[�V�����ϐ�
	int m_anim_nutral = 0;
	int m_anim_move = 0;
	int m_anim_hit = 0;
	int m_anim_jump = 0;
	int m_attach_move = 0;
	int m_attach_hit = 0;
	int m_attach_jump = 0;
	int m_attach_nutral = 0;

	float m_playAnimTime = 0.0f;
	float m_animTime_move = 0.0f;

	float m_playerRotateY = 0;
	float m_cameraAngle = 0;

	bool m_isVisibleFlag = false;
	int m_visibleCount = 0;

	int HitCount = 0;
};

