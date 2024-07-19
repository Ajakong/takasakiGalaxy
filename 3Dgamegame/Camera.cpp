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
	//奥行0.1～1000までをカメラの描画範囲とする
	SetCameraNearFar(kCameraNear, kCameraFar);

	// FOV(視野角)を60度に
	SetupCamera_Perspective(kCameraFOV * (static_cast<float>(DX_PI_F) / 180.0f));

	m_pos = { 0.0f,300.0f, -100.0f };
	m_playerToCameraVec = { 0.f,300.f,-100.f };
	m_postLookPointPos = { 0,0,0 };
}

Camera::~Camera()
{
	// 処理なし.
}

void Camera::Update(const Vec3 LookPoint)
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
	Vec3 toVec = m_pos;
	Vec3 right = Vec3::Right();
	
	Vec3 zero(0, 0, 0);
	
	/*float angleZ = DX_PI_F / 2 + atan2(m_upVec.y, m_upVec.z);
	ansVelocity = { objVelocity.x * cos(angleX), objVelocity.x * sin(angleX) + objVelocity.z * sin(angleZ), objVelocity.z * cos(angleZ) };*/
	
	m_myQ.SetQuaternion(m_pos);
	//m_myQ.SetMove(m_pitchAngle, right);
	//
	//m_pos = m_myQ.Move(m_pos, zero);
	//
	//m_pos = m_myQ.Move(toVec, zero);

	float yawAngle = fmodf(m_cameraAngle, 360.f);
	m_myQ.SetMove(yawAngle, m_upVec);
	

	Vec3 item = LookPoint;
	m_pos = m_myQ.Move(m_pos,zero );
	
	Vec3 vecY = m_upVec * 300;
	Vec3 fowardVec = LookPoint - m_postLookPointPos;

	Vec3 pos = m_upVec*300+LookPoint-fowardVec*200;

	DrawSphere3D(pos.VGet(), 50, 8, 0xffffff, 0xffffff, true);
	SetCameraPositionAndTargetAndUpVec(pos.VGet(), LookPoint.VGet(), m_upVec.VGet());
	//SetCameraPositionAndTarget_UpVecY(VGet(0, 0.0f, -1000.0f), pos.VGet());

	//プレイヤーの前ベクトルと法線ベクトルの外積で軸が得られる
	
	//Vec3 cross = Cross(fowardVec, m_upVec);
	//Quaternion q;
	//q.SetQuaternion(pos);
	//if (m_upVec.y > 0)
	//	m_pitchAngle = (fmodf(atan2f(m_upVec.y, m_upVec.z) - m_pitchAngle - DX_PI_F / 2, DX_PI_F * 2)) * DX_PI_F / 360;
	//else
	//	m_pitchAngle = (fmodf(atan2f(m_upVec.y, m_upVec.z) - m_pitchAngle + DX_PI_F / 2, DX_PI_F * 2)) * DX_PI_F / 360;
	//q.SetMove(acosf(Dot(fowardVec, m_upVec) / 1), cross);
	//pos = q.Move(pos,item);
	
	
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
