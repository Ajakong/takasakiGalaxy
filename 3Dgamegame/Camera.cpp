#include "Camera.h"
#include"Pad.h"
#include <math.h>


namespace
{
	constexpr float kCameraDist = 96;
	constexpr float kCameraHeight = 32;

	constexpr float kCameraNear = 1.0f;
	constexpr float kCameraFar = 5000.0f;

	//constexpr VECTOR kFirstPos=VGet(300, 300, -200);

	constexpr float kCameraFOV = 60.0f;

	constexpr float kcameraRotateSpeed = 0.05f;

	constexpr float kOneRapAngle = 360.f;
}

Camera::Camera()
{
	//���s0.1�`1000�܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(kCameraNear, kCameraFar);

	// FOV(����p)��60�x��
	SetupCamera_Perspective(kCameraFOV * (static_cast<float>(DX_PI_F) / 180.0f));

	m_pos = { 0.0f,300.0f, -100.0f };
	m_playerToCameraVec = { 0.f,300.f,-100.f };
	m_postLookPointPos = { 0,0,0 };
	m_fowardVec = { 0.f,0.f,0.1f };
}

Camera::~Camera()
{
	// �����Ȃ�.
}

void Camera::Update(Vec3 LookPoint)
{
	m_setCameraPos = &Camera::SetCameraThirdPersonPos;

	//m_cameraAngle = 0;
	////if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_LEFT))
	//if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_5))
	//{
	//	m_cameraAngle = kcameraRotateSpeed;
	//}
	//if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_6))
	//{
	//	m_cameraAngle = -kcameraRotateSpeed;
	//}
	//Vec3 toVec = m_pos;
	//Vec3 right = Vec3::Right();
	//
	//Vec3 zero(0, 0, 0);
	//
	///*float angleZ = DX_PI_F / 2 + atan2(m_upVec.y, m_upVec.z);
	//ansVelocity = { objVelocity.x * cos(angleX), objVelocity.x * sin(angleX) + objVelocity.z * sin(angleZ), objVelocity.z * cos(angleZ) };*/
	//
	//m_myQ.SetQuaternion(m_pos);
	////m_myQ.SetMove(m_pitchAngle, right);
	////
	////m_pos = m_myQ.Move(m_pos, zero);
	////
	////m_pos = m_myQ.Move(toVec, zero);

	//float yawAngle = fmodf(m_cameraAngle, 360.f);
	//m_myQ.SetMove(yawAngle, m_upVec);
	//

	//Vec3 item = LookPoint;
	//m_pos = m_myQ.Move(m_pos,zero );
	//
	//Vec3 vecY = m_upVec * 300;
	//
	//Vec3 pos = LookPoint-m_fowardVec*100;
	//Vec3 toFowardVec = (LookPoint - m_postLookPointPos).GetNormalized();
	//if ((LookPoint - m_postLookPointPos).GetNormalized().Length() > 0.5f)
	//{
	//	m_fowardVec = Vec3(toFowardVec.x,-toFowardVec.y,toFowardVec.z);
	//}
	//
	//DrawSphere3D(pos.VGet(), 50, 8, 0xffffff, 0xffffff, true);
	//SetCameraPositionAndTargetAndUpVec(pos.VGet(), LookPoint.VGet(), m_upVec.VGet());
	////SetCameraPositionAndTarget_UpVecY(VGet(0, 0.0f, -1000.0f), pos.VGet());

	////�v���C���[�̑O�x�N�g���Ɩ@���x�N�g���̊O�ςŎ���������
	//
	////Vec3 cross = Cross(fowardVec, m_upVec);
	////Quaternion q;
	////q.SetQuaternion(pos);
	////if (m_upVec.y > 0)
	////	m_pitchAngle = (fmodf(atan2f(m_upVec.y, m_upVec.z) - m_pitchAngle - DX_PI_F / 2, DX_PI_F * 2)) * DX_PI_F / 360;
	////else
	////	m_pitchAngle = (fmodf(atan2f(m_upVec.y, m_upVec.z) - m_pitchAngle + DX_PI_F / 2, DX_PI_F * 2)) * DX_PI_F / 360;
	////q.SetMove(acosf(Dot(fowardVec, m_upVec) / 1), cross);
	////pos = q.Move(pos,item);
	//
	//
	//Pad::Update();
	//m_postLookPointPos = LookPoint;

	m_cameraAngle = 0;
	
	if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_5))
	{
		m_cameraAngle = kcameraRotateSpeed;
		m_setCameraPos = &Camera::SetCameraFirstPersonPos;
	}
	if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_6))
	{
		m_cameraAngle = -kcameraRotateSpeed;
		m_setCameraPos = &Camera::SetCameraFirstPersonPos;
	}
	Vec3 toVec = m_pos;
	Vec3 right = Vec3::Right();

	Vec3 zero(0, 0, 0);


	m_myQ.SetQuaternion(m_pos);
	
	float yawAngle = fmodf(m_cameraAngle, 360.f);
	m_myQ.SetMove(yawAngle, m_upVec);
	


	Vec3 item = LookPoint;
	m_pos = m_myQ.Move(m_pos, zero);

	Vec3 vecY = m_upVec * 300;
	if ((LookPoint - m_postLookPointPos).Length() != 0)
	{
		m_fowardVec = LookPoint - m_postLookPointPos;
	}
	m_pos = LookPoint;

	m_pos += m_upVec * 300 + LookPoint - m_fowardVec * 100;
	

	/*DrawSphere3D(m_pos.VGet(), 50, 8, 0xffffff, 0xffffff, true);
	SetCameraPositionAndTargetAndUpVec(m_pos.VGet(), LookPoint.VGet(), m_upVec.VGet());*/

	Pad::Update();
	m_postLookPointPos = LookPoint;
}

void Camera::DebagDraw()
{
	
}

Vec3 Camera::cameraToPlayer(const Vec3& targetPos)
{

	Vec3 cameraToPlayer (sqrt((targetPos.x - m_pos.x) * (targetPos.x - m_pos.x)), 0.0f, sqrt((targetPos.z - m_pos.z) * (targetPos.z - m_pos.z)) );
	return cameraToPlayer;
}

void Camera::SetCameraPos(Vec3 LookPoint)
{
	(this->*m_setCameraPos)(LookPoint);
}

void Camera::SetCameraFirstPersonPos(Vec3 LookPoint)
{
	Vec3 target = LookPoint + m_fowardVec.GetNormalized();
	DrawSphere3D(LookPoint.VGet(), 50, 8, 0xffffff, 0xffffff, true);
	SetCameraPositionAndTargetAndUpVec(LookPoint.VGet(),target.VGet(), m_upVec.VGet());
}

void Camera::SetCameraThirdPersonPos(Vec3 LookPoint)
{

	DrawSphere3D(m_pos.VGet(), 50, 8, 0xffffff, 0xffffff, true);
	SetCameraPositionAndTargetAndUpVec(m_pos.VGet(), LookPoint.VGet(), m_upVec.VGet());
}
