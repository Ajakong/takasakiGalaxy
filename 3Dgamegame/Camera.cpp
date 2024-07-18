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
}

Camera::Camera()
{
	//âúçs0.1Å`1000Ç‹Ç≈ÇÉJÉÅÉâÇÃï`âÊîÕàÕÇ∆Ç∑ÇÈ
	SetCameraNearFar(kCameraNear, kCameraFar);

	// FOV(éãñÏäp)Ç60ìxÇ…
	SetupCamera_Perspective(kCameraFOV * (static_cast<float>(DX_PI_F) / 180.0f));

	m_pos = { 0.0f,300.0f, -100.0f };
	m_playerToCameraVec = { 0.f,300.f,-100.f };
	SetCameraPositionAndTarget_UpVecY(VGet(0,500.0f,-500.0f), VGet(0, 0, 0));
}

Camera::~Camera()
{
	// èàóùÇ»Çµ.
}

void Camera::Update(Vec3 LookPoint)
{
	m_cameraAngle = 0;
	//if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_LEFT))
	if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_5))
	{
		m_cameraAngle = kcameraRotateSpeed;
	}
	if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_6))
	{
		m_cameraAngle = -kcameraRotateSpeed;
	}
	Vec3 toVec = m_pos - LookPoint;
	
	
	Vec3 zero(0, 0, 0);
	m_pos = LookPoint;
	/*float angleZ = DX_PI_F / 2 + atan2(m_upVec.y, m_upVec.z);
	ansVelocity = { objVelocity.x * cos(angleX), objVelocity.x * sin(angleX) + objVelocity.z * sin(angleZ), objVelocity.z * cos(angleZ) };*/
	float pitchAngle = fmodf(atan2f(m_upVec.y, m_upVec.z), 360.f);
	m_myQ.SetQuaternion(m_pos);
	m_myQ.SetMove(pitchAngle, Vec3::Right());
	
	float yawAngle = fmodf(m_cameraAngle, 360.f);
	m_myQ.SetMove(yawAngle, m_upVec);
	
	m_pos = m_myQ.Move(m_pos, zero);

	
	Vec3 vecY = m_upVec * 300;
	Vec3 cameraPos = VAdd(VAdd(m_pos.VGet(), LookPoint.VGet()), vecY.VGet());
	//SetCameraPositionAndTargetAndUpVec(VAdd(m_pos.VGet(), LookPoint.VGet()),LookPoint.VGet(), m_upVec.VGet());

	Pad::Update();
}

void Camera::DebagDraw()
{
	DrawSphere3D(m_pos.VGet(), 50, 8, 0xffffff, 0xffffff,true);
}

Vec3 Camera::cameraToPlayer(const Vec3& targetPos)
{

	Vec3 cameraToPlayer (sqrt((targetPos.x - m_pos.x) * (targetPos.x - m_pos.x)), 0.0f, sqrt((targetPos.z - m_pos.z) * (targetPos.z - m_pos.z)) );
	return cameraToPlayer;
}
