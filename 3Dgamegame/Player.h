#pragma once
#include "MyLib/Physics/Collidable.h"
#include"MyLib/Vec3.h"

class Camera;

class Player : public MyEngine::Collidable
{
public:
	Player(int modelhandle);
	~Player();

	void Init();

	void Update();
	void SetMatrix();
	void Draw();


	Vec3 GetPos() const { return  m_rigid.GetPos(); }
	int GetHp() { return m_Hp; }
	void SetCameraToPlayer(Vec3 cameraToPlayer);
	Vec3 GetMoveDir() const{ return m_moveDir; }
	float GetRegenerationRange() { return m_regeneRange; }
	virtual void OnCollideEnter(const Collidable& colider);
	int WatchHp()const { return m_Hp; }

	void SetCameraAngle(float cameraAngle);
	void SetSideVec(Vec3 right) { m_sideVec = right; }
	void SetFrontVec(Vec3 front) { m_frontVec = front; }
	int GetPlayerModelhandle() { return m_modelHandle; }

	int& SetReverse() { return m_reverseFlag; }


	virtual void OnCollideEnter(std::shared_ptr<Collidable> colider);
	//�����o�֐��|�C���^
	using playerState_t = void(Player::*)();
	playerState_t m_playerUpdate;

	using cameraState_t = void(Player::*)();
	cameraState_t m_cameraUpdate;

private:
	//�A�j���[�V�����̐i�s
	//���[�v�������ǂ�����Ԃ�
	bool UpdateAnim(int attachNo);
	//�A�j���[�V�����̕ύX
	void ChangeAnim(int animIndex);

	//��ԕʊ֐�(�|�C���^�ŌĂяo��)
	/*m_playerUpdate�Ŏg��*/
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
	void SpiningUpdate();
	/// <summary>
	/// �Փˊ֐�
	/// </summary>
	void HitUpdate();
	/// <summary>
	/// ���
	/// </summary>
	void AvoidUpdate();
	/*m_cameraUpdate�Ŏg��*/
	void Planet1Update();

	
	Vec3 GetCameraToPlayer()const;

private:
	struct UserData
	{
		float dissolveY;	// �f�B�]��������������
		float minY;
		float maxY;
		float dummy;
		float clickedU;
		float clickedV;
		float dummy2[2];
	};

	int m_Hp;
	int m_modelHandle = 0;

	int rotRad = 0;

	/// <summary>
	/// �s���̃t���[�����Ǘ�����
	/// </summary>
	int actionFrame = 0;

	float m_regeneRange;
	float m_angle;
	float m_spinAngle;
	float m_radius = 0;

	Vec3 m_cameraToPlayer;
	Vec3 m_cameraPos;
	//std::shared_ptr<Camera> m_camera;
	Vec3 m_moveDir;

	Vec3 m_frontVec;
	Vec3 m_sideVec;
	
	
	int m_currentAnimNo;//���݂̃A�j���[�V����
	int m_prevAnimNo;//�ύX�O�̃A�j���[�V����
	float m_animBlendRate;//�A�j���[�V�����̍�������
	//0.0f:prev���Đ�
	//1.0:current���Đ�
	int m_reverseFlag=0;

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

