#pragma once
#include"MyLib/Vec3.h"
#include"Quaternion.h"
using namespace std;

class Player;


class Camera
{
public:
	Camera();							// �R���X�g���N�^.
	~Camera();							// �f�X�g���N�^.

	void Update(Vec3 LookPoint);	// �X�V.
	void DebagDraw();
	Vec3 cameraToPlayer(const Vec3& targetPos);
	
	const Vec3& GetPos() const { return m_pos; }

	void SetCameraPos(Vec3 LookPoint);
	void SetCameraFirstPersonPos(Vec3 LookPoint);
	void SetCameraThirdPersonPos(Vec3 LookPoint);
	float GetCameraAngle() const { return m_cameraAngle; }
	void SetUpVec(Vec3 upDir) { if (m_watchCount == 0) m_upVec = upDir; }
	Vec3 GetUpVec() const { return m_upVec; }
	void SetBoost(bool boost) { m_isBoost = boost; }
	void SetCameraPoint(Vec3 pos) { if (m_watchCount == 0)m_cameraPoint = pos; }
	void WatchThis(Vec3 lookpoint,Vec3 cameraPos,Vec3 upVec);
	void NeutralUpdate(Vec3 LookPoint);
	//�����o�֐��|�C���^
	using cameraState_t = void(Camera::*)(Vec3 lookpoint);
	cameraState_t m_cameraUpdate;
private:
	
	void WatchThisUpdate(Vec3 LookPoint);

private:
	int m_lightHandle = -1;
	int m_watchCount;
	bool m_isFirstPerson;
	bool m_isBoost;

	Vec3 m_frontVec;
	Quaternion m_myQ;
	Vec3 m_pos;			// �|�W�V����.
	Vec3 m_cameraPoint;//�J�������ړ����Ăق����ʒu
	Vec3 m_lookPoint;
	Vec3 m_postLookPointPos;
	float m_cameraAngle = -DX_PI_F / 2;
	float m_pitchAngle;
	Vec3 m_velocity;
	Vec3 m_playerNormVec;
	Vec3 m_upVec;
	/// <summary>
	/// �v���C���[���猩���J�����̈ʒu�x�N�g��
	/// </summary>
	Vec3 m_playerToCameraVec;
	Vec3 m_fowardVec;
};