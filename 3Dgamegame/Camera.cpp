#include "Camera.h"
#include"Pad.h"
#include <math.h>
#include"MyLib/Vec3.h"

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
	SetCameraNearFar(0.1f, 1000.0f);

	// FOV(éãñÏäp)Ç60ìxÇ…
	SetupCamera_Perspective(60.0f * (static_cast<float>(DX_PI_F) / 180.0f));

	pos = VGet(0, 300, -300.0f);
}

Camera::~Camera()
{
	// èàóùÇ»Çµ.
}

void Camera::Update()
{
	//if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_LEFT))
	if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_5))
	{
		cameraAngle += kcameraRotateSpeed;
	}
	if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_6))
	{
		cameraAngle -= kcameraRotateSpeed;
	}

	SetCameraNearFar(kCameraNear, kCameraFar);
	VECTOR cameraPos;

	float Length = sqrt(pos.x * pos.x + pos.z * pos.z);


	cameraPos.x = Length * static_cast<float>(cos(-fmodf(cameraAngle, 360)));
	cameraPos.z = Length * static_cast<float>(sin(-fmodf(cameraAngle, 360)));
	cameraPos.y = pos.y;
	pos = cameraPos;

	
	SetCameraNearFar(1.0f, 500.0f);
	SetupCamera_Perspective(DX_PI_F / 3.0f);
	SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0, 20, 0));

	Pad::Update();
}

Vec3 Camera::cameraToPlayer(const Vec3& targetPos)
{

	Vec3 cameraToPlayer = VGet(sqrt((targetPos.x - pos.x) * (targetPos.x - pos.x)), 0.0f, sqrt((targetPos.z - pos.z) * (targetPos.z - pos.z)));
	return cameraToPlayer;
}
