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
	Vec3 cameraToPlayer(const Vec3& targetPos);
	
	const Vec3& GetPos() const { return m_pos; }

	float GetCameraAngle() const { return m_cameraAngle; }
	void SetUpVec(Vec3 upDir) { m_upVec = upDir; }

private:
	Quaternion m_myQ;
	Vec3	m_pos;			// ポジション.

	float m_cameraAngle = -DX_PI_F / 2;
	Vec3 m_velocity;
	Vec3 m_playerNormVec;
	Vec3 m_upVec;

};