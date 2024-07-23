#pragma once
#include"MyLib/Vec3.h"
#include"Quaternion.h"
using namespace std;

class Player;


class Camera
{
public:
	Camera();							// コンストラクタ.
	~Camera();							// デストラクタ.

	void Update(Vec3 LookPoint);	// 更新.
	void DebagDraw();
	Vec3 cameraToPlayer(const Vec3& targetPos);
	
	const Vec3& GetPos() const { return m_pos; }

	float GetCameraAngle() const { return m_cameraAngle; }
	void SetUpVec(Vec3 upDir) { m_upVec = upDir; }

private:
	Quaternion m_myQ;
	Vec3	m_pos;			// ポジション.
	Vec3 m_postLookPointPos;
	float m_cameraAngle = -DX_PI_F / 2;
	float m_pitchAngle;
	Vec3 m_velocity;
	Vec3 m_playerNormVec;
	Vec3 m_upVec;
	/// <summary>
	/// プレイヤーから見たカメラの位置ベクトル
	/// </summary>
	Vec3 m_playerToCameraVec;
	Vec3 m_fowardVec;
};