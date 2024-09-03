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
	void SetUpVec(Vec3 upDir) { m_upVec = upDir; }
	Vec3 GetUpVec() const { return m_upVec; }
	void SetCameraPoint(Vec3 pos) { m_cameraPoint = pos; }

	//�����o�֐��|�C���^
	using cameraState_t = void(Camera::*)(Vec3 LookPoint);
	cameraState_t m_setCameraPos;
private:
	Quaternion m_myQ;
	Vec3	m_pos;			// �|�W�V����.
	Vec3 m_cameraPoint;//�J�������ړ����Ăق����ʒu
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