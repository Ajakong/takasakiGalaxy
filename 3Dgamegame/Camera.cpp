#include "Camera.h"
#include"Pad.h"
#include <math.h>


namespace
{
	constexpr float kCameraDist = 96;
	constexpr float kCameraHeight = 32;

	constexpr float kCameraNear = 1.0f;
	constexpr float kCameraFar = 1000.0f;

	//constexpr VECTOR kFirstPos=VGet(300, 300, -200);

	constexpr float kCameraFOV = 60.0f;

	constexpr float kcameraRotateSpeed = 0.05f;
}

Camera::Camera()
{
	//âúçs0.1Å`1000Ç‹Ç≈ÇÉJÉÅÉâÇÃï`âÊîÕàÕÇ∆Ç∑ÇÈ
	SetCameraNearFar(1.f,500.f);

	// FOV(éãñÏäp)Ç60ìxÇ…
	SetupCamera_Perspective(60.0f * (static_cast<float>(DX_PI_F) / 180.0f));

	m_pos = { 0,0, -300.0f };
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
	/*Vec3 toVec = m_pos - LookPoint;
	m_cameraAngle = atan2f(toVec.z, toVec.x);*/

	SetCameraNearFar(kCameraNear, kCameraFar);
	
	float angle = fmodf(m_cameraAngle, 360.f);
	m_myQ.SetMove(angle, m_upVec);
	Vec3 zero(0, 0, 0);
	m_pos =m_myQ.Move(m_pos,zero);
	
	SetCameraNearFar(1.f, 500.f);
	SetupCamera_Perspective(DX_PI_F / 3.0f);
	Vec3 vecY = m_upVec * 300;
	SetCameraPositionAndTargetAndUpVec(VAdd(VAdd(m_pos.VGet(),LookPoint.VGet()) , vecY.VGet()),LookPoint.VGet(), m_upVec.VGet());

	Pad::Update();
}

Vec3 Camera::cameraToPlayer(const Vec3& targetPos)
{

	Vec3 cameraToPlayer (sqrt((targetPos.x - m_pos.x) * (targetPos.x - m_pos.x)), 0.0f, sqrt((targetPos.z - m_pos.z) * (targetPos.z - m_pos.z)) );
	return cameraToPlayer;
}
